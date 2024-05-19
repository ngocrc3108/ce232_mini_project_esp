#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <dht.h>

#define SENSOR_TYPE DHT_TYPE_AM2301
#define CONFIG_EXAMPLE_DATA_GPIO 4
//#define CONFIG_EXAMPLE_INTERNAL_PULLUP

void dht_test(void *pvParameters)
{
    float temperature, humidity;

#ifdef CONFIG_EXAMPLE_INTERNAL_PULLUP
    gpio_set_pull_mode(CONFIG_EXAMPLE_DATA_GPIO, GPIO_PULLUP_ONLY);
#endif

    while (1)
    {
        if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void app_main() {
    xTaskCreate(dht_test, "dht_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}
