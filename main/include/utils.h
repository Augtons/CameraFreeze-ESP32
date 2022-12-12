//
// Created by augtons on 22-12-12.
//

#ifndef CAMERAFREEZE_ESP32_UTILS_H
#define CAMERAFREEZE_ESP32_UTILS_H

#include <stdlib.h>
#include <string.h>

#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_event.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief 用于初始化nvs
*/
void init_nvs();

/**
 * @brief
 */


#ifdef __cplusplus
}
#endif

#endif //CAMERAFREEZE_ESP32_UTILS_H
