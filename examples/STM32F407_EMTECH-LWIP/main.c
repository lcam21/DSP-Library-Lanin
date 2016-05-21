#include <ch.h>
#include <hal.h>

#include "lwipthread.h"
#include "web/web.h"

static THD_WORKING_AREA(waThread1, 256);
static THD_FUNCTION(Thread1, arg) {

	(void)arg;
	chRegSetThreadName("blinker");
	while (true) {
		palTogglePad(GPIOG, GPIOG_LED1);
		chThdSleepMilliseconds(500);
	}
}

int main(void) {

	halInit();
	chSysInit();
	lwipInit(NULL);

	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

	chThdCreateStatic(wa_http_server, sizeof(wa_http_server), WEB_THREAD_PRIORITY, http_server, NULL);


	while (TRUE) {
		chThdSleepMilliseconds(500);
	}
}
