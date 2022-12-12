//
// Created by augtons on 22-12-12.
//

#ifndef CAMERAFREEZE_ESP32_MQTT_H
#define CAMERAFREEZE_ESP32_MQTT_H

#include "utils.h"
#include "conf.h"
#include "mqtt_client.h"

#ifdef __cplusplus
extern "C" {
#endif

extern esp_mqtt_client_handle_t mqtt_client;

void mqtt_app_start();

#ifdef __cplusplus
}
#endif

#endif //CAMERAFREEZE_ESP32_MQTT_H
