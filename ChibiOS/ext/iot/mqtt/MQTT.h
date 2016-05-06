/**
 * @file    MQTT.h
 * @brief   MQTT Pub/Sub over wifi
 * @version 0.1
 * @date    05/11/2015 11:32:17
 * @author  mribelotta
 */

#ifndef MQTT_H_
#define MQTT_H_

#include "wifi.h"
#include "MQTTPacket.h"
#include "MQTTSubscribe.h"
#include "MQTTPublish.h"
#include "lstrutil.h"

/**
 * @brief Macro to declare subscription handler
 */
#define MQTT_SUBSHANDLER(name) \
    void name(MQTT_t *m, const MQTTLenString *topic, const MQTTLenString *data)

/**
 * Macro to easy MQTT object creation
 */
#define MQTT_NEW(w, c, b, s) ((MQTT_t){w, c, (unsigned char*)(b), sizeof(b), s})

typedef struct MQTT_s MQTT_t;

/**
 * @brief Subscription handler function typedef
 * @param m MQTT object
 * @param topic Topic to respones
 * @param data Data from publication
 */
typedef void (MQTT_SubsHandler_t)(MQTT_t *m, const MQTTLenString *topic,
                                  const MQTTLenString *data);

/**
 * @brief Publication check return status
 * @see mqtt_check_for_msg
 */
typedef enum {
  MQTT_SUBS_ONMSG,    //!< MQTT_SUBS_ONMSG Published mesagge incomming
  MQTT_SUBS_BUSY,     //!< MQTT_SUBS_BUSY Link busy
  MQTT_SUBS_ERROR_NET,     //!< MQTT_SUBS_ERROR_NET Network error
  MQTT_SUBS_ERROR_MSG,     //!< MQTT_SUBS_ERROR_MSG Module network
} MQTT_SUBS_Status_t;

/**
 * @brief Subscription descriptor
 */
typedef struct {
  const char *topic; //!< Topic to subscrobe
  MQTT_SubsHandler_t *handler; //!< Pointer to handle function
} MQTT_Subscriber_t;

/**
 * @brief MQTT object
 */
typedef struct MQTT_s {
  Wifi_t *wifiLink; //!< Wifi Link to operate
  const char *clientID; //!< Client ID
  unsigned char *buf; //!< Working buffer
  int buflen; //!< Working buffer space
  const MQTT_Subscriber_t *subscribers; //!< NULL terminated subscriber array
} MQTT_t;

/**
 * @brief MQTT publication data
 */
typedef struct {
  unsigned char flag_dup;
  int flag_qos;
  unsigned char flag_retained;
  unsigned short msgid;
  int payloadlen_in;
  unsigned char* payload_in;
  MQTTString receivedTopic;
} MQTT_Publication_t;

extern bool mqtt_connect(MQTT_t *m, const char *host, int port);
extern bool mqtt_disconnect(MQTT_t *m);
extern bool mqtt_ping(MQTT_t *m);
extern bool mqtt_publish(MQTT_t *m, const char *tp, const char *pl,
                         size_t pl_len);
extern bool mqtt_subscribe(MQTT_t *m, const char *topic);
extern bool mqtt_subscribe_all(MQTT_t *m);
extern MQTT_SUBS_Status_t mqtt_check_for_msg(MQTT_t *m,
                                             MQTT_Publication_t *pub);
extern void mqtt_dispatch(MQTT_t *m, const MQTT_Publication_t *pub);

#endif /* MQTT_H_ */
