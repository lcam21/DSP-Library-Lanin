#include "MQTT.h"

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#define DBG(...) do { \
  chprintf((BaseSequentialStream*)&SD1, __VA_ARGS__); \
  chprintf((BaseSequentialStream*)&SD1, "\r\n"); \
} while(0)

static int getdata(unsigned char *buf, int len, void *u) {
  Wifi_t *w = (Wifi_t*)u;
  if (wifi_expect(w, len, MS2ST(250)))
    return wifi_sock_recv(w, buf, len);
  else
    return -1;
}

/**
 * @brief Connect to MQTT server
 * @param m MQTT Object
 * @param host Host URL or IP
 * @param port Server port  (usual 1883)
 * @retval true On sucess
 * @retval false On error
 */
bool mqtt_connect(MQTT_t *m, const char *host, int port) {
  Wifi_t *w = m->wifiLink;
  DBG("Connecting with %s:%d", host, port);
  if (wifi_sock_open(w, host, port)) {
    int retry = 30;
    while (!w->connected && retry-- > 0) {
      DBG("Wait for connection");
      wifi_stat(m->wifiLink);
    }
    if (!w->connected || !retry) {
      DBG("No connection established with server");
      return false;
    } else {
      int len;
      MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

      data.clientID.cstring = m->clientID;
      data.keepAliveInterval = 20;
      data.cleansession = 1;
      data.MQTTVersion = 3;

      len = MQTTSerialize_connect(m->buf, m->buflen, &data);
      if (!wifi_sock_send(w, m->buf, len)) {
        DBG("Can not send connect packet");
        return false;
      } else {
        unsigned char sp, ack;
        if (MQTTPacket_read(m->buf, m->buflen, getdata, w) != CONNACK) {
          DBG("Can not receive ACK packet");
          return false;
        }
        if (MQTTDeserialize_connack(&sp, &ack, m->buf, m->buflen) != 1
            || ack != 0) {
          DBG("Unable to connect, return code %d\n", ack);
          return false;
        }

        DBG("Connect sucessfull");
        return true;
      }
    }
  }
  return false;
}

bool mqtt_ping(MQTT_t *m) {
  int len = MQTTSerialize_pingreq(m->buf, m->buflen);
  if (!wifi_sock_send(m->wifiLink, m->buf, len)) {
    DBG("Can not send ping");
    return false;
  }
  return true;
}


/**
 * @brief Disconnect from server
 * @param m MQTT object
 * @retval true On sucess
 * @retval false On error
 */
bool mqtt_disconnect(MQTT_t *m) {
  int len = MQTTSerialize_disconnect(m->buf, m->buflen);
  if (!wifi_sock_send(m->wifiLink, m->buf, len)) {
    DBG("Can not send connect packet");
  }
  if (!wifi_sock_close(m->wifiLink)) {
    DBG("Can not close socket");
    return false;
  }
  return true;
}

/**
 * @brief Publish a news in a topic
 * @param m MQTT object
 * @param tp Topic to publish
 * @param pl Data to publish
 * @param pl_len Data len
 * @retval true if published correctly
 * @retval false if any error ocurr
 */
bool mqtt_publish(MQTT_t *m, const char *tp, const char *pl, size_t pl_len) {
  MQTTString topicString = {tp, {0, NULL}};
  int len = MQTTSerialize_publish(m->buf, m->buflen, 0, 0, 0, 0, topicString,
                                  (unsigned char*)pl, pl_len);
  if (!wifi_sock_send(m->wifiLink, m->buf, len)) {
    DBG("Send failed\r\n");
    return false;
  }

  return true;
}

/**
 * @brief Subscribe to topic on server (need connected server)
 * @param m MQTT object
 * @param topic Topic string
 * @retval true On sucess
 * @retval false On error
 */
bool mqtt_subscribe(MQTT_t *m, const char *topic) {
  int req_qos = 0;
  MQTTString topicString = {topic, {0, NULL}};
  int len = MQTTSerialize_subscribe(m->buf, m->buflen, 0, 0, 1, &topicString,
                                    &req_qos);
  if (!wifi_sock_send(m->wifiLink, m->buf, len)) {
    DBG("Subscribe not send");
    return false;
  }
  if (MQTTPacket_read(m->buf, m->buflen, getdata, m->wifiLink) != SUBACK) {
    DBG("Error expecting subscribe response");
    return false;
  }
  if (1) {
    unsigned short p_id;
    int qos_n;
    int qos_grant;
    if (!MQTTDeserialize_suback(&p_id, 1, &qos_n, &qos_grant, m->buf, len)) {
      DBG("Subscribe Deserialice ACK fail");
      return false;
    }
    if (qos_grant != 0) {
      DBG("Unexpected QoS granted %d", qos_grant);
      return false;
    }
    return true;
  }
}

/**
 * @brief Subscibe to all topics on MQTT handler list
 * @param m MQTT object with handler list initialized
 * @retval true On sucess
 * @retval false On error
 */
bool mqtt_subscribe_all(MQTT_t *m) {
  if (m->subscribers) {
    const MQTT_Subscriber_t *p;
    for (p = m->subscribers; p->topic; p++) {
      DBG("Subscribe to topic %s", p->topic);
      if (!mqtt_subscribe(m, p->topic)) {
        DBG("Cannot subscribe to topic %s", p->topic);
        return false;
      }
    }
  }
  return true;
}

static int mqtt_decode(MQTT_Publication_t *p, unsigned char *buf, int len) {
  return MQTTDeserialize_publish(&p->flag_dup, &p->flag_qos, &p->flag_retained,
                                 &p->msgid, &p->receivedTopic, &p->payload_in,
                                 &p->payloadlen_in, buf, len);
}

/**
 * @brief Check MQTT server for message for this
 * @param m MQTT object
 * @param pub Publication struct (writable)
 * @return Status of publication
 */
MQTT_SUBS_Status_t mqtt_check_for_msg(MQTT_t *m, MQTT_Publication_t *pub) {
  if (MQTTPacket_read(m->buf, m->buflen, getdata, m->wifiLink) == PUBLISH) {
    int rc = mqtt_decode(pub, m->buf, m->buflen);
    if (rc != 1) {
      DBG("Error %d deserialising packet published", rc);
      return MQTT_SUBS_ERROR_MSG;
    } else
      return MQTT_SUBS_ONMSG;
  } else {
    return MQTT_SUBS_BUSY;
  }
}

/**
 * @brief Dispatch publication to handler in MQTT object
 *
 * If MQTT object not contains handler for topic, this not take any efect
 *
 * @param m MQTT object
 * @param pub Publication getted with #mqtt_check_fro_msg
 */
void mqtt_dispatch(MQTT_t *m, const MQTT_Publication_t *pub) {
  const MQTT_Subscriber_t *p;
  MQTTLenString message = {pub->payloadlen_in, (char*)pub->payload_in};
  for (p = m->subscribers; p->topic; p++) {
    if (lstrcmpcstr(&pub->receivedTopic.lenstring, p->topic) == 0) {
      p->handler(m, &pub->receivedTopic.lenstring, &message);
    }
  }
}
