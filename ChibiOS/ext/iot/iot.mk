IOT_PATH := ${CHIBIOS}/ext/iot
MQTT_PATH = ${CHIBIOS}/ext/iot/mqtt

IOTSRC = $(wildcard $(IOT_PATH)/*.c) $(wildcard $(MQTT_PATH)/*.c)

IOTINC = $(IOT_PATH) $(MQTT_PATH)
