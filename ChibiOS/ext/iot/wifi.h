/**
 * @file    wifi.h
 * @brief   ESP8266 WiFi Client side interfaz
 * @version 0.1
 * @date    02/07/2015 11:03:31
 * @author  mribelotta
 */

#ifndef WIFI_H_
#define WIFI_H_

#include <ch.h>
#include <hal.h>

#include <e_ringbuffer.h>
#include "iotconf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  WIFI_STAT_ERR = 0,
  WIFI_STAT_GOTIP = 2,
  WIFI_STAT_CONNECT = 3,
  WIFI_STAT_DISCONNECT = 4,
} wifi_stat_t;

typedef struct {
  SerialDriver *serial; //!< Serial port link
  int serialBaud; //!< Serial baudrate
  systime_t timeout; //!< Timeout (in systicks) for operation
  SerialDriver *debug; //!< Debug link (can be NULL)
  char *scrach_buf; //!< Buffer for operation (80 or more. 128-256 recomended)
  size_t scrach_size; //!< Scratch buffer size
  char *rcv_buf;
  size_t rcv_size;
} WifiConfig_t;

/**
 * @brief ESP8266 wifi object
 */
typedef struct {
  const WifiConfig_t *config; //!< Object configuration
  bool connected; //!< socket used list, init as 0
  struct {
    enum {
      IPD_WAIT_IPD_PLUS,
      IPD_WAIT_IPD_I,
      IPD_WAIT_IPD_P,
      IPD_WAIT_IPD_D,
      IPD_WAIT_IPD_COMMA,
      IPD_READ_PACKLEN,
      IPD_READ_PACKDATA,
      IPD_ON_DATA,
      IPD_ON_ERR,
    } st;
    size_t len;
    ERingBuffer buffer;
  } ipd;
} Wifi_t;

extern void wifi_init(Wifi_t *w, const WifiConfig_t *conf);
extern bool wifi_restart(Wifi_t *w);
extern bool wifi_join(Wifi_t *w, const char *SSID, const char *pass);
extern bool wifi_leave(Wifi_t *w);
extern bool wifi_sock_open(Wifi_t *w, const char *host, int port);
extern bool wifi_sock_close(Wifi_t *w);
extern bool wifi_sock_send(Wifi_t *w, const void *buf, size_t len);
extern int wifi_sock_avail(Wifi_t *w);
extern int wifi_expect(Wifi_t *w, int count, systime_t tout);
extern int wifi_sock_recv(Wifi_t *w, void *b, size_t max);
extern wifi_stat_t wifi_stat(Wifi_t *w);
extern bool wifi_resync(Wifi_t *w, int max);
extern void wifi_flush(Wifi_t *w);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_H_ */
