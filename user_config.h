// -- Project -------------------------------------------
#define CLIENT                  ""                    // Client ID for the ESP
#define TYPE                    ""                    // Client ID for the ESP

// -- Wifi ----------------------------------------------
#define WIFI_SSID               ""                    // Wifi SSID
#define WIFI_PASS               ""                    // Wifi password

// -- POST ----------------------------------------------
#define POST_URI                ""                    // [POST] URI of service to POST to

// -- Other - Sensor ------------------------------------
#define PIN_DHT                 22                    // [Sensor] Pin for DHT sensor
#define PIN_SOIL                32                    // [Sensor] Soil Sensor pin
#define PIN_LIGHT               33                    // [Sensor] Light Sensor pin
#define PIN_POWER               34                    // [Sensor] Power Sensor pin

// -- Other - Helpers ------------------------------------
#define uS_TO_S_FACTOR          1000000               // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP           300                   // Time ESP32 will go to sleep (in seconds) 
#define TIME_TO_SNOOZE          5                     // Time ESP32 will go to sleep (in seconds) 
#define BUILTIN_LED             13                    // Not really used
#define SAMPLE                  10                    // the number fo time to sample then average a reading 