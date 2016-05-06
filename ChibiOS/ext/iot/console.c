#include <ch.h>
#include <hal.h>

#include "ngshell.h"
#include "compilation_date.h"
#include "chprintf.h"
#include "mqtt_test.h"

// Macro de utilidad
#ifndef ARRAY_LEN
#define ARRAY_LEN(a) ((int)(sizeof(a) / sizeof(*(a))))
#endif

// Emula sscanf
#define sscanf mini_sscanf

// Emula printf sobre SD1
#define printf(...) chprintf((BaseSequentialStream*)&SD1, __VA_ARGS__)

// Emula fprintf sobre un SDx cualquiera
#define fprintf(u, ...) chprintf((BaseSequentialStream*)(u), __VA_ARGS__)

// Backdeclaration de min_sscanf
extern int mini_sscanf(const char *str, const char *fmt, ...);

// Backdeclaration para main mqtt
extern void mqtt_init(void);

/*----------------------------------------------------------------------------*/

int cmd_uname(int argc, char **argv) {
  (void)argc;
  (void)argv;

  printf("ChibiOS " CH_KERNEL_VERSION //
      " in " PORT_INFO " over "//
      PORT_CORE_VARIANT_NAME " ["//
      PORT_ARCHITECTURE_NAME "]\r\n"//
      "Compiled on %s %s\r\n",//
      COMPILATION_DATE, COMPILATION_TIME);
  return 0;
}

/*----------------------------------------------------------------------------*/

static int cmd_wifi(int argc, char **argv) {
  int i;
  fprintf(&SD1, "Press <ESC> key to terminate.\r\nWriting :");
  for (i = 1; i < argc; i++) {
    fprintf(&SD1, "%s ", argv[i]);
    fprintf(&SD4, "%s", argv[i]);
    if (i < argc - 1)
      fprintf(&SD4, " ");
  }
  fprintf(&SD1, "\r\nReceiving:\r\n");
  fprintf(&SD4, "\r\n");
  while (sdGetTimeout(&SD1, TIME_IMMEDIATE) != 27) {
    msg_t c = sdGetTimeout(&SD4, TIME_IMMEDIATE);
    if (c == Q_TIMEOUT) {
      chThdYield();
    } else {
      sdPut(&SD1, c);
    }
  }
  return 0;
}

/*----------------------------------------------------------------------------*/

static int cmd_bridge(int argc, char **argv) {
  if (argc != 1) {
    fprintf(&SD1, "Usage %s\r\n", argv[0]);
    return -1;
  }
  fprintf(&SD1, "\r\n<ESC> to terminate\r\n");
  while (1) {
    msg_t c;
    c = sdGetTimeout(&SD1, TIME_IMMEDIATE);
    //if (c == 27)
    //  break;
    if (c != Q_TIMEOUT)
      sdPut(&SD4, c);
    c = sdGetTimeout(&SD4, TIME_IMMEDIATE);
    if (c != Q_TIMEOUT)
      sdPut(&SD1, c);
  }
  return 0;
}

/*----------------------------------------------------------------------------*/

int cmd_reset(int argc, char **argv) {
  if (argc != 1) {
    fprintf(&SD1, "Usage %s\r\n", argv[0]);
    return -1;
  }
  chprintf((BaseSequentialStream*)&SD1, "Reseting...\r\n");
  NVIC_SystemReset();
  return 0;
}

int cmd_mqtt(int argc, char **arg){
	(void)argc;
	(void)arg;

	mqtt_init();
}

/*----------------------------------------------------------------------------*/

// type *const name guarda el array en ROM
static SerialDriver *const serial_devices[] = {&SD1, &SD4};

int cmd_serial(int argc, char **argv) {
  int baud, n;
  if (argc != 3) {
    fprintf(&SD1, "Usage %s <serialno> <baudrate>\r\n", argv[0]);
    return -1;
  }
  if (sscanf(argv[1], "%i", &n) != 1) {
    fprintf(&SD1, "Can not convert %s to integer <serialno>\r\n", argv[1]);
    return -1;
  }
  if (n < 0 || n > ARRAY_LEN(serial_devices)) {
    fprintf(&SD1, "Non existing serial %d\r\n", n);
  }
  if (sscanf(argv[2], "%i", &baud) != 1) {
    fprintf(&SD1, "Can not convert %s to integer <serialno>\r\n", argv[2]);
    return -1;
  }
  sdStop(serial_devices[n]);
  sdStart(serial_devices[n], &(SerialConfig) {baud, 0, 0, 0});
  fprintf(&SD1, "Setting SD%d to %d bauds\r\n", n, baud);
  return 0;
}

/*----------------------------------------------------------------------------*/

int cmd_ps(int argc, char **argv) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  if (argc > 1) {
    printf("Usage: %s\r\n", argv[0]);
    return -1;
  }
  printf("    addr    stack prio refs  state time name\r\n");
  tp = chRegFirstThread();
  do {
    printf("%08lx %08lx %4lu %4lu   %9s %s\r\n", (uint32_t )tp,
           (uint32_t )tp->p_ctx.r13, (uint32_t )tp->p_prio,
           (uint32_t )(tp->p_refs - 1), states[tp->p_state], tp->p_name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
  return 0;
}

/*----------------------------------------------------------------------------*/

// Lista de comandos de ngshell
static const ngshell_cmd_t cmdlist[] = { //
    {"wifi", cmd_wifi}, //
    {"espbridge", cmd_bridge}, //
    {"uname", cmd_uname}, //
    {"reset", cmd_reset}, //
    {"mqtt", cmd_mqtt}, //
    {"serial", cmd_serial}, //
    {"ps", cmd_ps}, //
    {NULL, NULL} //
    };

// Funcion de escritura para ngshell sobre sd1
static void sd1_put(char c) {
  static char oldChar = 0;
  if (c == '\n' && oldChar != '\r')
    sdPut(&SD1, '\r');
  sdPut(&SD1, oldChar = c);
}

// Funcion de lectura para ngshell sobre sd1
static int sd1_get(void) {
  return sdGet(&SD1);
}

// Interfaz de IO de consola para ngshell
static const rdline_io_t sh_io = {sd1_put, sd1_get};

// Largo maximo de linea 80 caracteres
#define SH1_LINELEN 80
// Historial maximo 10 lineas
#define SH1_HISTORYLEN 10

// Buffers para ngshell
static char sh_buf[SH1_LINELEN];
static char sh_arena[RDLINE_HISTORY_ARENA_SIZE(SH1_LINELEN, SH1_HISTORYLEN)];
// Configuracon de la shell
static const RDLINE_CONFIG(sh_rd, sh_buf, &sh_io, sh_arena);

// Objeto shell con su lista de comandos
static NGSHELL_DECLARE(sh, "sh> ", cmdlist, &sh_rd);

int command_console(void) {

  // Si se sale del comando, ingresar a la consola para esperar comandos
  fprintf(&SD1, "\r\n\r\nChibiOS console\r\n");
  cmd_uname(0, NULL);
  return ngshell_process(&sh);
}
