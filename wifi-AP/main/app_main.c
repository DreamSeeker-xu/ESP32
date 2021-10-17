#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_event.h"

static void event_hander(void* event_handler_arg,
                                        esp_event_base_t event_base,
                                        int32_t event_id,
                                        void* event_data)
{
	 switch (event_id)
    {
		//AP开始
    case SYSTEM_EVENT_AP_START:
        printf("\nwifi_softap_start\n");
        break;
		//有STA连接上ESP32的AP
    case SYSTEM_EVENT_AP_STACONNECTED:
        printf("\nwifi_softap_connectted\n");
        break;
		//有STA断开ESP32的AP
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        printf("\nwifi_softap_disconnectted\n");
        break;
    default:
        break;
    }
   // return ESP_OK;
}
void app_main(void) {	

	//初始化NVS(ESP_ERROR_CHECK()检查函数的返回值错误)
	ESP_ERROR_CHECK( nvs_flash_init() );
	//初始化lwip相关
	esp_netif_init();
	//创建系统事件任务并初始化应用程序事件的回调函数
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	//创建具有TCP/IP堆栈的默认网络接口实例
	esp_netif_create_default_wifi_ap();

	//创建wifi初始化参数结构体，获取默认配置
	wifi_init_config_t wifi_init =  WIFI_INIT_CONFIG_DEFAULT();
	//初始wifi资源
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_init));
	//注册wifi和ip地址事件到默认事件循环
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&event_hander,NULL,NULL));
	//定义wifi配置参数
	wifi_config_t wificonfig = {
		.ap = {
			.ssid = "esp32",
			.ssid_len = 0,//SSID长度
			.max_connection = 1,//能连接的最大节点数
			.password = "12345678",
			.authmode = WIFI_AUTH_WPA_WPA2_PSK,//授权模式
		},
	};

	//设置wifi模式
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	//使能wifi
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP,&wificonfig));
	//启动wifi
	ESP_ERROR_CHECK(esp_wifi_start());
}

