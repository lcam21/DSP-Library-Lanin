#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <slre.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "wifi.h"

#define HAVE_DATA_EVENT EVENT_MASK(1)

static void u_printf(SerialDriver *sd, const char *fmt, ...) {
  if (sd) {
    va_list ap;
    va_start(ap, fmt);
    chvprintf((BaseSequentialStream*)sd, fmt, ap);
    va_end(ap);
  }
}

#define DBG(w, ...) do { \
  if ((w)->config->debug) \
    u_printf((w)->config->debug, __VA_ARGS__); \
    u_printf((w)->config->debug, "\r\n"); \
  } while(0)

typedef struct {
  Wifi_t *chan;
  const char **patterns;
  int pattern_count;
} FindObj_t;

static SerialDriver *ffind_debug_out = NULL;
#define FIND_DBG_ON(w) ffind_debug_out = w->config->debug
#define FIND_DBG_OFF(w) ffind_debug_out = NULL

typedef enum {
  RESPONSE_READY,
  RESPONSE_BUSY,
  RESPONSE_NOCHANGE,
  RESPONSE_LINKED,
  RESPONSE_NOLINK,
  RESPONSE_NOIP,
  RESPONSE_ALREADY_CONNECT,
  RESPONSE_CONNECT,
  RESPONSE_SENDOK,
  RESPONSE_PROMPT,
  RESPONSE_OK,
  RESPONSE_ERROR,
  RESPONSE_NORESP,
} RESPONSE_t;

static const char *RESPONSES[] = { //
    [RESPONSE_READY] = "ready", //
    [RESPONSE_BUSY] = "busy", //
    [RESPONSE_NOCHANGE] = "no change", //
    [RESPONSE_LINKED] = "Linked", //
    [RESPONSE_NOLINK] = "link is not", //
    [RESPONSE_NOIP] = "no ip", //
    [RESPONSE_ALREADY_CONNECT] = "ALREAY CONNECT", //
    [RESPONSE_CONNECT] = "CONNECT", //
    [RESPONSE_SENDOK] = "SEND OK", //
    [RESPONSE_PROMPT] = ">", //
    [RESPONSE_OK] = "OK", //
    [RESPONSE_ERROR ] = "ERROR", //
    [RESPONSE_NORESP] = "\xffTIMEOUT\xff" //
    };

#define RESPONSES_CNT (sizeof(RESPONSES)/(sizeof(*(RESPONSES))))

static void do_ipd_fsm(char c, Wifi_t *w) {
  if (c == 255)
    return;
#if 0
  if (c >= 32 && c < 127)
  u_printf(&SD1, "< %c,%d>", c, w->ipd.st);
  else
  u_printf(&SD1, "<%02X,%d>", c, w->ipd.st);
#endif
  switch (w->ipd.st) {
  default:
  case IPD_ON_ERR:
    DBG(w, "ipd_fsm error on %d", w->ipd.st);
  case IPD_ON_DATA:
  case IPD_WAIT_IPD_PLUS:
    if (c == '+')
      w->ipd.st = IPD_WAIT_IPD_I;
    break;
  case IPD_WAIT_IPD_I:
    if (c == 'I')
      w->ipd.st = IPD_WAIT_IPD_P;
    else
      w->ipd.st = IPD_WAIT_IPD_I;
    break;
  case IPD_WAIT_IPD_P:
    if (c == 'P')
      w->ipd.st = IPD_WAIT_IPD_D;
    else
      w->ipd.st = IPD_WAIT_IPD_I;
    break;
  case IPD_WAIT_IPD_D:
    if (c == 'D')
      w->ipd.st = IPD_WAIT_IPD_COMMA;
    else
      w->ipd.st = IPD_WAIT_IPD_I;
    break;
  case IPD_WAIT_IPD_COMMA:
    if (c == ',') {
      w->ipd.len = 0;
      w->ipd.st = IPD_READ_PACKLEN;
    } else
      w->ipd.st = IPD_ON_ERR;
    break;
  case IPD_READ_PACKLEN:
    if (isdigit(((int )c))) {
      w->ipd.len = (w->ipd.len * 10) + (c - '0');
    } else if (c == ':')
      w->ipd.st = IPD_READ_PACKDATA;
    else
      w->ipd.st = IPD_ON_ERR;
    break;
  case IPD_READ_PACKDATA:
    e_ringbuffer_write(&w->ipd.buffer, &c, sizeof(char));
    w->ipd.len--;
    if (w->ipd.len > 0)
      w->ipd.st = IPD_READ_PACKDATA;
    else
      w->ipd.st = IPD_ON_DATA;
    break;
  }
}

static msg_t wifi_get_char(Wifi_t *w) {
  msg_t c = sdGetTimeout(w->config->serial, TIME_IMMEDIATE);
  do_ipd_fsm(c, w);
  return c;
}

void wifi_flush(Wifi_t *w) {
  while (wifi_get_char(w) != Q_TIMEOUT )
    ;
}

/**
 * @brief Find an string on serial channel
 * @param ctx Find context
 * @return Index of founded pattern
 * @retval -1 if not found any pattern
 */
int ffind(const FindObj_t *ctx) {
  int i;
  const char *pattern[ctx->pattern_count];
  size_t capture_count = 0;
  systime_t startTime = chVTGetSystemTime();
  systime_t endTime = startTime + ctx->chan->config->timeout;

  memcpy(pattern, ctx->patterns, sizeof(pattern));
  while (chVTIsTimeWithinX(chVTGetSystemTime(), startTime, endTime)) {
    msg_t c = wifi_get_char(ctx->chan);
    if (c == Q_TIMEOUT) {
      chThdYield();
    } else {
      startTime = chVTGetSystemTime();
      u_printf(ffind_debug_out, "[%c]", (char)c);
      if (ctx->chan->config->scrach_buf
          && capture_count < ctx->chan->config->scrach_size) {
        ctx->chan->config->scrach_buf[capture_count++] = c;
        ctx->chan->config->scrach_buf[capture_count] = '\0';
      }
      for (i = 0; i < ctx->pattern_count; i++) {
        if (c == *(pattern[i])) {
          pattern[i]++;
          if (*(pattern[i]) == '\0') {
            return i;
          }
        } else {
          pattern[i] = ctx->patterns[i];
        }
      }
    }
  }
  return RESPONSE_NORESP;
}

static int at_cmd(Wifi_t *w, const char *fmt, ...) {
  va_list ap;
  int response;
  va_start(ap, fmt);
  if (w->config->debug) {
    chprintf((BaseSequentialStream*)w->config->debug, "Execute: ");
    chvprintf((BaseSequentialStream*)w->config->debug, fmt, ap);
    chprintf((BaseSequentialStream*)w->config->debug, "\r\n");
  }
  wifi_flush(w);
  u_printf(w->config->serial, "AT+");
  chvprintf((BaseSequentialStream *)w->config->serial, fmt, ap);
  u_printf(w->config->serial, "\r\n");
  response = ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT}));
  DBG(w, "[%s]:{ %s }\r\n***", RESPONSES[response], w->config->scrach_buf);
  va_end(ap);
  return response;
}

static bool wifi_reset(Wifi_t *w) {
  int response;
  u_printf(w->config->serial, "AT+RST\r\n");
  response = ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT}));
  DBG(w, "Response: %s", RESPONSES[response]);
  if (response == RESPONSE_OK) {
    response = ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT}));
    return response == RESPONSE_READY;
  } else {
    DBG(w, "No response OK");
    return false;
  }
}

static bool wifi_ping(Wifi_t *w) {
  int response;
  u_printf(w->config->serial, "AT\r\n");
  response = ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT}));
  DBG(w, "Response: %s", RESPONSES[response]);
  return response == RESPONSE_OK;
}

void wifi_init(Wifi_t *w, const WifiConfig_t *conf) {
  w->config = conf;
  w->connected = false;
  w->ipd.len = 0;
  w->ipd.st = IPD_WAIT_IPD_PLUS;

  palSetPadMode(GPIOC, 10, PAL_MODE_ALTERNATE(8) | PAL_STM32_OSPEED_HIGHEST); // U2-TX
  palSetPadMode(GPIOC, 11, PAL_MODE_ALTERNATE(8) | PAL_STM32_OSPEED_HIGHEST); // U2-RX

  sdStart(w->config->serial, &((SerialConfig) {conf->serialBaud, 0,
                                               USART_CR2_STOP1_BITS, 0}));
  e_ringbuffer_init(&w->ipd.buffer, w->config->rcv_buf, w->config->rcv_size);
}

bool wifi_restart(Wifi_t *w) {
  if (!wifi_reset(w)) {
    DBG(w, "Reset fail");
    return false;
  }
  chThdSleepSeconds(2);
  if (!wifi_ping(w)) {
    DBG(w, "No ping");
    return false;
  }
  u_printf(w->config->serial, "ATE0\r\n");
  if (ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT})) != RESPONSE_OK) {
    DBG(w, "No echo OFF");
    return false;
  }
  switch (at_cmd(w, "CWMODE=1")) {
  case RESPONSE_OK:
  case RESPONSE_NOCHANGE:
    break;
  default:
    DBG(w, "No CWMODE to 1");
    return false;
  }
  switch (at_cmd(w, "CIPMUX=0")) {
  case RESPONSE_OK:
  case RESPONSE_NOCHANGE:
    break;
  default:
    DBG(w, "No CIPMUX to 0");
    return false;
  }
  return true;
}

/**
 * @brief Connect to AP
 * @param w Wifi object
 * @param SSID SSID of AP
 * @param pass Password
 * @retval true if AP accept connection
 * @retval false if fail
 */
bool wifi_join(Wifi_t *w, const char *SSID, const char *pass) {
  if (at_cmd(w, "CWJAP=\"%s\",\"%s\"", SSID, pass) == RESPONSE_OK) {
    w->connected = false;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Leave current AP
 * @param w Wifi object
 * @retval true if leave
 * @retval false if fail
 */
bool wifi_leave(Wifi_t *w) {
  return at_cmd(w, "CWQAP") == RESPONSE_OK;
}

/**
 * @brief Open TCP socket connection
 * @param w Wifi object
 * @param host IP or DNS resolvable name of server
 * @param port Port number of connection
 * @retval true On connect
 * @retval false On fail
 */
bool wifi_sock_open(Wifi_t *w, const char *host, int port) {
  switch (at_cmd(w, "CIPSTART=\"TCP\",\"%s\",%d", host, port)) {
  case RESPONSE_CONNECT:
    w->connected = false;
    return ffind(&((FindObj_t) {w, RESPONSES, RESPONSES_CNT})) == RESPONSE_OK;
  default:
    w->connected = false;
    return false;
  }
}

/**
 * @brief close communication socket
 * @param w Wifi object
 * @param sock socket number to close
 * @retval true if sucess
 * @retval false if fail
 */
bool wifi_sock_close(Wifi_t *w) {
  wifi_stat_t currentStatus = wifi_stat(w);
  if (currentStatus == WIFI_STAT_CONNECT) {
    if (at_cmd(w, "CIPCLOSE") == RESPONSE_OK) {
      w->connected = false;
      return true;
    } else {
      return false;
    }
  } else {
    DBG(w, "WARN: CIPSTATUS=%d\r\n"
        "Not connected. Ignore it\r\n",
        currentStatus);
    return true;
  }
}

/**
 * @brief Send packet over socket
 * @param w Wifi socket object
 * @param sock Socket to use
 * @param b Buffer to send
 * @param len Bytes to send
 * @retval true if sucess
 * @retval false if fail
 */
bool wifi_sock_send(Wifi_t *w, const void *b, size_t len) {
  int response = 0;
  FindObj_t fObj = {w, RESPONSES, RESPONSES_CNT};
  const char *ptr = (const char*)b;

  int res=at_cmd(w, "CIPSEND=%d", len);
  if ( (res== RESPONSE_PROMPT) || (res== RESPONSE_OK)) {
    //if (ffind(&fObj) == RESPONSE_PROMPT) {
      while (len--)
        sdPut(w->config->serial, *ptr++);
      response = ffind(&fObj);
      DBG(w, "<<%d:%s>>", response, RESPONSES[response]);
      if (response == RESPONSE_SENDOK) {
        DBG(w, "SENDs OK");
        return true;
      } else
        DBG(w, "No response OK");
    //} else
    //  DBG(w, "No promt");
  } else
    DBG(w, "Send fail");
  return false;
}

int wifi_sock_avail(Wifi_t *w) {
  return e_ringbuffer_used(&w->ipd.buffer);
}

int wifi_expect(Wifi_t *w, int count, systime_t tout) {
  systime_t startTime = chVTGetSystemTime();
  systime_t endTime = startTime + tout;
  while (chVTIsTimeWithinX(chVTGetSystemTime(), startTime, endTime)) {
    if (wifi_sock_avail(w) < count) {
      msg_t c = wifi_get_char(w);
      if (c == Q_TIMEOUT)
        chThdYield();
    }
  }
  return wifi_sock_avail(w);
}

/**
 * @brief Wait for wifi socket pack
 * @param w Wifi object
 * @param sock Pointer to store current receiver socket
 * @param b Buffer for store received data
 * @param max Maximun len of buffer
 * @note timeout is w->config->timeout
 * @return The received len, 0 on timeout or -1 for error
 */
int wifi_sock_recv(Wifi_t *w, void *b, size_t max) {
  systime_t startTime = chVTGetSystemTime();
  systime_t endTime = startTime + w->config->timeout;
  while (chVTIsTimeWithinX(chVTGetSystemTime(), startTime, endTime)) {
    if (e_ringbuffer_used(&w->ipd.buffer) >= max) {
      break;
    } else {
      msg_t c = wifi_get_char(w);
      if (c == Q_TIMEOUT)
        chThdYield();
    }
  }
  return e_ringbuffer_read(&w->ipd.buffer, b, max);
}

#if 0
/**
 * @brief Wait for wifi socket pack
 * @param w Wifi object
 * @param sock Pointer to store current receiver socket
 * @param b Buffer for store received data
 * @param max Maximun len of buffer
 * @param tout System timeout
 * @return The received len, 0 on timeout or -1 for error
 */
int wifi_sock_recv_tout(Wifi_t *w, int *sock, void *b, size_t max,
    systime_t tout) {
#if 1
  systime_t startTime = chVTGetSystemTime();
  systime_t endTime = startTime + tout;
  while (chVTIsTimeWithinX(chVTGetSystemTime(), startTime, endTime)) {

    if (wifi_get_char(w) == Q_TIMEOUT)
    chThdYield();
  }
#else
  IPD_STATE_t fsm = IPD_WAIT_IPD_PLUS;
  IPD_DATA_t d = {0, 0, 0, (char*)b, max};
  systime_t startTime = chVTGetSystemTime();
  systime_t endTime = startTime + tout;
  while (chVTIsTimeWithinX(chVTGetSystemTime(), startTime, endTime)) {
    msg_t c = sdGetTimeout(w->config->serial, TIME_IMMEDIATE);
    if (c == Q_TIMEOUT) {
      chThdYield();
    } else {
      startTime = chVTGetSystemTime();
      endTime = startTime + tout;
      fsm = ipd_fsm(fsm, c, &d);
      switch (fsm) {
        case IPD_ON_DATA:
        if (sock)
        *sock = d.sock;
        return d.len;
        case IPD_ON_ERR:
        return -1;
        default:
        ;
      }
    }
  }
  return 0;
#endif
}
#endif

typedef struct slre_cap slre_cap_t;

static int str_regex(const char *re, char *s, slre_cap_t *c, int n) {
  size_t len = strlen(s);
  int r = slre_match(re, s, len, c, n, 0);
  if (r > 0) {
    int i;
    for (i = 0; i < n; i++) {
      ((char*)c[i].ptr)[c[i].len] = 0;
    }
  }
  return r;
}

extern long mini_strtol(const char *nptr, char **ptr, int base);

wifi_stat_t wifi_stat(Wifi_t *w) {
  static const char * const re_a = "STATUS:(\\d+)";
  struct slre_cap caps[7];
  int i;
  if (at_cmd(w, "CIPSTATUS") != RESPONSE_OK) {
    DBG(w, "IPSTATUS No response Ok\r\n");
    return WIFI_STAT_ERR;
  }
  w->connected = false;
  if ((i = str_regex(re_a, w->config->scrach_buf, caps, 1)) > 0) {
    static const char * const re_b = "\\+CIPSTATUS:(\\d+),\"TCP\","
        "\"(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\",(\\d+),(\\d+)";
    int j = 0;
    char *str = w->config->scrach_buf + i + 1;
    int len = strlen(str);
    int currentStatus = mini_strtol(caps[0].ptr, NULL, 10);
    while (j < len && (i = str_regex(re_b, str + j, caps, 7)) > 0) {
      w->connected = true;
      j += i;
    }
    return (wifi_stat_t)currentStatus;
  } else
    return WIFI_STAT_ERR;
}

bool wifi_resync(Wifi_t *w, int max) {
  int cnt;
  for (cnt = 0; cnt < max; cnt++) {
    DBG(w, "resync %d", cnt);
    if (wifi_ping(w)) {
      return true;
    }
  }
  return false;
}

#if 0
static char wifi_internal_buffer[2048];
static char app_buf[2048];

void wifi_test(void) {
  Wifi_t wifiObj = WIFI_NEW(&SD4, S2ST(10), wifi_internal_buffer, &SD1);
  Wifi_t *w = &wifiObj;

  DBG(w, "\r\n==============================="
      "======================================\r\n"
      "Init wifi");

#define DIE(w, ...) do { DBG(w, __VA_ARGS__); return; } while(0)

  if (!wifi_init(w))
  DIE(w, ">>> {ERROR}: wifi_init fail");

  if (at_cmd(w, "CWLAP") != RESPONSE_OK)
  DIE(w, ">>> {ERROR}: List AP response != OK");

  DBG(w, "Join to network");
  if (!wifi_join(w, "Et_Bari", "8rjz-tbtu-enm1"))
  DIE(w, ">>> {ERROR}: List AP response != OK");

  DBG(w, "Show IP");
  at_cmd(w, "CIFSR");

  do {
    int sock, rsock, received;
    size_t toSend;

    DBG(w, "Connecting to google.com:80");
    sock = wifi_sock_open(w, "google.com", 80);
    if (sock == -1)
    DIE(w, ">>> {ERROR}: Socket open fail");

    toSend = chsnprintf(app_buf, sizeof(app_buf), "GET / HTTP/1.0\r\n\r\n");

    DBG(w, "Send GET");
    if (!wifi_sock_send(w, sock, app_buf, toSend))
    DIE(w, ">>> {ERROR}: Send GET fail");

    received = wifi_sock_recv(w, &rsock, app_buf, sizeof(app_buf));
    if (received == -1)
    DIE(w, ">>> {ERROR}: Socket recv fail");

    DBG(w, "Received %d bytes on socket %d", received, rsock);
    DBG(w, "Received buffer:\r\n"
        "************************************\r\n"
        "%s\r\n"
        "************************************\r\n",
        app_buf);

    if (!wifi_sock_close(w, sock))
    DIE(w, ">>> {ERROR}: Socket close fail");

  }while (0);
  u_printf(&SD1, "Disconnecting...\r\n");
  if (!wifi_leave(w))
  DIE(w, ">>> {ERROR}: Can not leave AP");

  u_printf(&SD1, "\r\n********** STOP!");
  chThdSleepSeconds(2);
}
#endif
