idf_component_register (
    SRCS "mqtt.c" "wifi.c"
    INCLUDE_DIRS "include"
    REQUIRES mqtt esp_wifi nvs_flash
)