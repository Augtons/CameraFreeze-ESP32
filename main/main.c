#include "utils.h"
#include "wifi.h"
#include "mqtt.h"

#include "driver/gpio.h"

#define TAG     "MAIN"

static int nowStatus = 1;
static bool mqtt_app_started = false;

static void mqtt_data_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;

    if (event_id == MQTT_EVENT_DATA) {
        if(event->data[0] == '0') {
            nowStatus = 0;
            ESP_LOGI(TAG, "远程摄像头被冻结 (x)\n");
        } else if(event->data[0] == '1') {
            nowStatus = 1;
            ESP_LOGI(TAG, "远程摄像头已解冻 ( )\n");
        } else {
            ESP_LOGI(TAG, "啥也不是\n");
        }
    }
}

static void wifi_connect_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    // 只执行一次
    if (mqtt_app_started == false) {
        mqtt_app_start();
        esp_mqtt_client_register_event(mqtt_client, MQTT_EVENT_DATA, mqtt_data_handler, NULL);
        mqtt_app_started = true;
    }
}

_Noreturn
static void key_scan(void *arg) {
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        if (gpio_get_level(GPIO_NUM_0) == 0){
            vTaskDelay(pdMS_TO_TICKS(20));
            if (gpio_get_level(GPIO_NUM_0) != 0) { continue; }

            char data;
            if(nowStatus == 0){
                data = '1';
            } else {
                data = '0';
            }

            esp_mqtt_client_publish(mqtt_client, CAMERA_CTRL_CTRL_TOPIC, &data, 1, 0, false);

            while (gpio_get_level(GPIO_NUM_0) == 0) { //等待松手
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }

}

void app_main(void)
{
    init_nvs();

    // Init Button
    gpio_config_t cfg_gpio = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pin_bit_mask = BIT64(0)
    };
    gpio_config(&cfg_gpio);

    // Init LED
    gpio_config_t led_gpio = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = BIT64(2)
    };
    gpio_config(&led_gpio);
    gpio_set_level(GPIO_NUM_2, 0);

    // Init and connect Wi-Fi
    init_wifi(MY_WIFI_SSID, MY_WIFI_PASSWORD);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_connect_handler, NULL, NULL);

    // Key Scan
    xTaskCreate(key_scan, "gpio", 2048, NULL, 1, NULL);
}