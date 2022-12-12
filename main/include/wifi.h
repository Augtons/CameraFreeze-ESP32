//
// Created by augtons on 22-12-12.
//

#ifndef CAMERAFREEZE_ESP32_WIFI_H
#define CAMERAFREEZE_ESP32_WIFI_H

#include "utils.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_netif_ip_addr.h"

#ifdef __cplusplus
extern "C" {
#endif

extern esp_netif_t *wifi_netif;

void init_wifi(const char *ssid, const char *password);

#ifdef __cplusplus
}
#endif

#endif //CAMERAFREEZE_ESP32_WIFI_H
