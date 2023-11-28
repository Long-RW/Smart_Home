/* Switch Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <driver/uart.h>
#include <driver/gpio.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_schedule.h>
#include <esp_rmaker_scenes.h>
#include <esp_rmaker_console.h>
#include <esp_rmaker_ota.h>

#include <esp_rmaker_common_events.h>

#include <app_wifi.h>
#include <app_insights.h>

#include "app_priv.h"
static const int RX_BUFF_SIZE = 1024;
#define TXD1_PIN (GPIO_NUM_10)
#define RXD1_PIN (GPIO_NUM_9)
#define UART1 UART_NUM_1
int num = 0;

void uart_init(void){
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // We won't use a buffer for sending data.
    uart_driver_install(UART1, RX_BUFF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART1, &uart_config);
    uart_set_pin(UART1, TXD1_PIN, RXD1_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

static const char *TAG = "app_main";
esp_rmaker_device_t *switch_device;
esp_rmaker_device_t *led1_switch;
esp_rmaker_device_t *led2_switch;
esp_rmaker_device_t *led3_switch;
esp_rmaker_device_t *led4_switch;
esp_rmaker_device_t *led5_switch;
esp_rmaker_device_t *motor1_switch;
esp_rmaker_device_t *motor2_switch;
esp_rmaker_device_t *motor3_switch;
esp_rmaker_device_t *motor5_switch;
/* Callback to handle commands received from the RainMaker cloud 
    led1_switch = esp_rmaker_switch_device_create("Den Phong Khach", NULL, false);
    led2_switch = esp_rmaker_switch_device_create("Den Phong Ngu 1", NULL, false);
    led3_switch = esp_rmaker_switch_device_create("Den Phong Ngu 2", NULL, false);
    led4_switch = esp_rmaker_switch_device_create("Den Gara", NULL, false);
    led5_switch = esp_rmaker_switch_device_create("Den Bep", NULL, false);
    led6_switch = esp_rmaker_switch_device_create("Den Nha Tam", NULL, false);

    motor1_switch = esp_rmaker_switch_device_create("Cua so Khach", NULL, false);
    motor2_switch = esp_rmaker_switch_device_create("Cua Gara", NULL, false);
    motor3_switch = esp_rmaker_switch_device_create("Cua so Bep", NULL, false);
    motor4_switch = esp_rmaker_switch_device_create("MOTOR4", NULL, false);
    motor5_switch = esp_rmaker_switch_device_create("Quat Phong Ngu", NULL, false);
*/
static esp_err_t write_cb(const esp_rmaker_device_t *device, const esp_rmaker_param_t *param,
            const esp_rmaker_param_val_t val, void *priv_data, esp_rmaker_write_ctx_t *ctx)
{   
    char *device_name = esp_rmaker_device_get_name(device);
    if (ctx) {
        ESP_LOGI(TAG, "Received write request via : %s", esp_rmaker_device_cb_src_to_str(ctx->src));
    }
    if (strcmp(device_name, "Den Phong Khach") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":1,\"LED1\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":1,\"LED1\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    if (strcmp(device_name, "Den Phong Ngu 1") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":2,\"LED2\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":2,\"LED2\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    if (strcmp(device_name, "Den Phong Ngu 2") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":3,\"LED3\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":3,\"LED3\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    if (strcmp(device_name, "Den Gara") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":4,\"LED4\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":4,\"LED4\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    if (strcmp(device_name, "Den Bep") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":5,\"LED5\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":5,\"LED5\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    if (strcmp(device_name, "Cua so Khach") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":7,\"MOTOR1\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":7,\"MOTOR1\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }

    if (strcmp(device_name, "Cua Gara") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0};  
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":8,\"MOTOR2\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":8,\"MOTOR2\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }

    if (strcmp(device_name, "Cua so Bep") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0};  
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":9,\"MOTOR3\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":9,\"MOTOR3\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }


    if (strcmp(device_name, "Quat Phong Ngu") == 0){
        if (strcmp(esp_rmaker_param_get_name(param), ESP_RMAKER_DEF_POWER_NAME) == 0)
        {
            ESP_LOGI(TAG, "Received value = %s for %s - %s",
                     val.val.b ? "true" : "false", esp_rmaker_device_get_name(device),
                     esp_rmaker_param_get_name(param));
            app_driver_set_state(val.val.b);
            esp_rmaker_param_update_and_report(param, val);
            bool switch_state = val.val.b;
            char tx_data[30] = {0}; 
            if(switch_state == true){
                strcpy(tx_data, "{\"msg_id\":11,\"MOTOR5\":1}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
            else{
                strcpy(tx_data, "{\"msg_id\":11,\"MOTOR5\":0}");
                uart_write_bytes(UART1, tx_data, strlen(tx_data));
            }
        }
    }
    return ESP_OK;

    return ESP_OK;
}
/* Event handler for catching RainMaker events */
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == RMAKER_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_INIT_DONE:
                ESP_LOGI(TAG, "RainMaker Initialised.");
                break;
            case RMAKER_EVENT_CLAIM_STARTED:
                ESP_LOGI(TAG, "RainMaker Claim Started.");
                break;
            case RMAKER_EVENT_CLAIM_SUCCESSFUL:
                ESP_LOGI(TAG, "RainMaker Claim Successful.");
                break;
            case RMAKER_EVENT_CLAIM_FAILED:
                ESP_LOGI(TAG, "RainMaker Claim Failed.");
                break;
            case RMAKER_EVENT_LOCAL_CTRL_STARTED:
                ESP_LOGI(TAG, "Local Control Started.");
                break;
            case RMAKER_EVENT_LOCAL_CTRL_STOPPED:
                ESP_LOGI(TAG, "Local Control Stopped.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled RainMaker Event: %"PRIi32, event_id);
        }
    } else if (event_base == RMAKER_COMMON_EVENT) {
        switch (event_id) {
            case RMAKER_EVENT_REBOOT:
                ESP_LOGI(TAG, "Rebooting in %d seconds.", *((uint8_t *)event_data));
                break;
            case RMAKER_EVENT_WIFI_RESET:
                ESP_LOGI(TAG, "Wi-Fi credentials reset.");
                break;
            case RMAKER_EVENT_FACTORY_RESET:
                ESP_LOGI(TAG, "Node reset to factory defaults.");
                break;
            case RMAKER_MQTT_EVENT_CONNECTED:
                ESP_LOGI(TAG, "MQTT Connected.");
                break;
            case RMAKER_MQTT_EVENT_DISCONNECTED:
                ESP_LOGI(TAG, "MQTT Disconnected.");
                break;
            case RMAKER_MQTT_EVENT_PUBLISHED:
                ESP_LOGI(TAG, "MQTT Published. Msg id: %d.", *((int *)event_data));
                break;
            default:
                ESP_LOGW(TAG, "Unhandled RainMaker Common Event: %"PRIi32, event_id);
        }
    } else if (event_base == APP_WIFI_EVENT) {
        switch (event_id) {
            case APP_WIFI_EVENT_QR_DISPLAY:
                ESP_LOGI(TAG, "Provisioning QR : %s", (char *)event_data);
                break;
            case APP_WIFI_EVENT_PROV_TIMEOUT:
                ESP_LOGI(TAG, "Provisioning Timed Out. Please reboot.");
                break;
            case APP_WIFI_EVENT_PROV_RESTART:
                ESP_LOGI(TAG, "Provisioning has restarted due to failures.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled App Wi-Fi Event: %"PRIi32, event_id);
                break;
        }
    } else if (event_base == RMAKER_OTA_EVENT) {
        switch(event_id) {
            case RMAKER_OTA_EVENT_STARTING:
                ESP_LOGI(TAG, "Starting OTA.");
                break;
            case RMAKER_OTA_EVENT_IN_PROGRESS:
                ESP_LOGI(TAG, "OTA is in progress.");
                break;
            case RMAKER_OTA_EVENT_SUCCESSFUL:
                ESP_LOGI(TAG, "OTA successful.");
                break;
            case RMAKER_OTA_EVENT_FAILED:
                ESP_LOGI(TAG, "OTA Failed.");
                break;
            case RMAKER_OTA_EVENT_REJECTED:
                ESP_LOGI(TAG, "OTA Rejected.");
                break;
            case RMAKER_OTA_EVENT_DELAYED:
                ESP_LOGI(TAG, "OTA Delayed.");
                break;
            case RMAKER_OTA_EVENT_REQ_FOR_REBOOT:
                ESP_LOGI(TAG, "Firmware image downloaded. Please reboot your device to apply the upgrade.");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled OTA Event: %"PRIi32, event_id);
                break;
        }
    } else {
        ESP_LOGW(TAG, "Invalid event received!");
    }
}

void app_main()
{
    /* Initialize Application specific hardware drivers and
     * set initial state.
     */
    uart_init();
    esp_rmaker_console_init();
    app_driver_init();
    app_driver_set_state(DEFAULT_POWER);

    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    /* Initialize Wi-Fi. Note that, this should be called before esp_rmaker_node_init()
     */
    app_wifi_init();

    /* Register an event handler to catch RainMaker events */
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_COMMON_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(APP_WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(RMAKER_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    /* Initialize the ESP RainMaker Agent.
     * Note that this should be called after app_wifi_init() but before app_wifi_start()
     * */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "MySmartHome", "Switch");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialise node. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }

    /* Create a Switch device.
     * You can optionally use the helper API esp_rmaker_switch_device_create() to
     * avoid writing code for adding the name and power parameters.
     */
    switch_device = esp_rmaker_device_create("Switch", ESP_RMAKER_DEVICE_SWITCH, NULL);
    led1_switch = esp_rmaker_switch_device_create("Den Phong Khach", NULL, false);
    led2_switch = esp_rmaker_switch_device_create("Den Phong Ngu 1", NULL, false);
    led3_switch = esp_rmaker_switch_device_create("Den Phong Ngu 2", NULL, false);
    led4_switch = esp_rmaker_switch_device_create("Den Gara", NULL, false);
    led5_switch = esp_rmaker_switch_device_create("Den Bep", NULL, false);

    motor1_switch = esp_rmaker_switch_device_create("Cua so Khach", NULL, false);
    motor2_switch = esp_rmaker_switch_device_create("Cua Gara", NULL, false);
    motor3_switch = esp_rmaker_switch_device_create("Cua so Bep", NULL, false);
    motor5_switch = esp_rmaker_switch_device_create("Quat Phong Ngu", NULL, false);
    /* Add the write callback for the device. We aren't registering any read callback yet as
     * it is for future use.
     */
    esp_rmaker_device_add_cb(switch_device, write_cb, NULL);
    esp_rmaker_device_add_cb(led1_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(led2_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(led3_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(led4_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(led5_switch, write_cb, NULL);

    esp_rmaker_device_add_cb(motor1_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(motor2_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(motor3_switch, write_cb, NULL);
    esp_rmaker_device_add_cb(motor5_switch, write_cb, NULL);
    /* Add the standard name parameter (type: esp.param.name), which allows setting a persistent,
     * user friendly custom name from the phone apps. All devices are recommended to have this
     * parameter.
     */
    esp_rmaker_device_add_param(switch_device, esp_rmaker_name_param_create(ESP_RMAKER_DEF_NAME_PARAM, "Switch"));

    /* Add the standard power parameter (type: esp.param.power), which adds a boolean param
     * with a toggle switch ui-type.
     */
    esp_rmaker_param_t *power_param = esp_rmaker_power_param_create(ESP_RMAKER_DEF_POWER_NAME, DEFAULT_POWER);
    esp_rmaker_device_add_param(switch_device, power_param);

    /* Assign the power parameter as the primary, so that it can be controlled from the
     * home screen of the phone apps.
     */
    esp_rmaker_device_assign_primary_param(switch_device, power_param);

    /* Add this switch device to the node */
    esp_rmaker_node_add_device(node, switch_device);
    esp_rmaker_node_add_device(node, led1_switch);
    esp_rmaker_node_add_device(node, led2_switch);
    esp_rmaker_node_add_device(node, led3_switch);
    esp_rmaker_node_add_device(node, led4_switch);
    esp_rmaker_node_add_device(node, led5_switch);

    esp_rmaker_node_add_device(node, motor1_switch);
    esp_rmaker_node_add_device(node, motor2_switch);
    esp_rmaker_node_add_device(node, motor3_switch);
    esp_rmaker_node_add_device(node, motor5_switch);
    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable timezone service which will be require for setting appropriate timezone
     * from the phone apps for scheduling to work correctly.
     * For more information on the various ways of setting timezone, please check
     * https://rainmaker.espressif.com/docs/time-service.html.
     */
    esp_rmaker_timezone_service_enable();

    /* Enable scheduling. */
    esp_rmaker_schedule_enable();

    /* Enable Scenes */
    esp_rmaker_scenes_enable();

    /* Enable Insights. Requires CONFIG_ESP_INSIGHTS_ENABLED=y */
    app_insights_enable();

    /* Start the ESP RainMaker Agent */
    esp_rmaker_start();

    err = app_wifi_set_custom_mfg_data(MGF_DATA_DEVICE_TYPE_SWITCH, MFG_DATA_DEVICE_SUBTYPE_SWITCH);
    /* Start the Wi-Fi.
     * If the node is provisioned, it will start connection attempts,
     * else, it will start Wi-Fi provisioning. The function will return
     * after a connection has been successfully established
     */
    err = app_wifi_start(POP_TYPE_RANDOM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wifi. Aborting!!!");
        vTaskDelay(5000/portTICK_PERIOD_MS);
        abort();
    }
}
