//
// Created by augtons on 22-12-12.
//

#include "wifi.h"
#include "driver/gpio.h"

static bool s_wifi_inited = false;

esp_netif_t *wifi_netif = NULL;

/**
 * @brief WiFi 的事件循环Handler
 * @param arg
 * @param event_base
 * @param event_id
 * @param event_data
 */
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    }

    if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI("TEST_ESP32", "Got IP: " IPSTR,  IP2STR(&event->ip_info.ip));

        gpio_set_level(GPIO_NUM_2, 1);
    }

    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        vTaskDelay(pdMS_TO_TICKS(500));
        esp_wifi_connect();
        gpio_set_level(GPIO_NUM_2, 0);
    }
}

void init_wifi(const char *ssid, const char *password) {
    esp_netif_init();
    esp_event_loop_create_default();

    if (wifi_netif == NULL) {
        wifi_netif = esp_netif_create_default_wifi_sta();
    }

    if (s_wifi_inited == false) {
        s_wifi_inited = true;

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        esp_wifi_init(&cfg);

        if (ssid || password) {
            wifi_sta_config_t wifi_config = {0};
            if (ssid != NULL) {
                strcpy((char*)wifi_config.ssid, ssid);
            }
            if (password != NULL) {
                strcpy((char*)wifi_config.password, password);
            }
            esp_wifi_set_config(WIFI_IF_STA, (wifi_config_t*)&wifi_config);
        }

        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL, NULL);
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL, NULL);

        esp_wifi_set_mode(WIFI_MODE_STA);
        esp_wifi_start();
    }

}