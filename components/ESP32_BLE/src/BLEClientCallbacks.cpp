/*
 * BLEClientCallbacks.cpp
 *
 *  Created on: Jul 5, 2017
 *      Author: kolban
 */
#include "sdkconfig.h"
#if defined(CONFIG_BT_ENABLED)
#include "BLEClient.h"
#include <esp_log.h>
static const char* LOG_TAG = "BLEClientCallbacks";

void BLEClientCallbacks::onConnect(BLEClient* pClient) {
	ESP_LOGD(LOG_TAG, ">> onConnect(): Default");
	ESP_LOGD(LOG_TAG, "<< onConnect()");
}
#endif /* CONFIG_BT_ENABLED */
