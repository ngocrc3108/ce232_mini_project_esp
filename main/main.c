#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>
#include <connection/wifi.h>
#include <connection/mqtt.h>

#define SENSOR_TYPE DHT_TYPE_AM2301
#define CONFIG_EXAMPLE_DATA_GPIO 4

void dht_test(void *pvParameters)
{
    float temperature, humidity;

    gpio_set_pull_mode(CONFIG_EXAMPLE_DATA_GPIO, GPIO_PULLUP_ONLY);

    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK) {
            printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
            char buffer[256];
            sprintf(buffer, "humidity=%.1f&temperature=%.1f", humidity, temperature);
            if(mqtt_publish("/sensor/dht22", buffer) == -1) {
                ESP_LOGE("DEBUG", "can not send mqtt message");
            }
        }
        else
            printf("Could not read data from sensor\n");

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main() {
    wifi_init_sta();
    mqtt_app_start();
    xTaskCreate(dht_test, "dht_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}
