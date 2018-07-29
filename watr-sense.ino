#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHTesp.h>
#include "user_config.h"

const size_t bufferSize = JSON_OBJECT_SIZE(7);
char JSONmessageBuffer[400];

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR char name[15] = CLIENT;

DynamicJsonBuffer jsonBuffer(bufferSize);
JsonObject& root = jsonBuffer.createObject();

DHTesp dht;


///////////////////////////////////////////////////
// This code is intended to run on a Higrow sensor
// It will take sesnor reading from the Moisture probe 
// And the Temp/Humidity (DHT11).  
// This data will then be blended with other meta data
// to form a JSON document.
// 
// A connection is then made to the local WiFi AP and
// a post request is made to a RESTful end point to 
// capture the data
//
// The device will then go to sleep for a period of time
//
// This code is offered with no implied warenty and is to
// be used at your own risk.
//
// Useful links:
// https://youtu.be/1SCbf845I4g
//
////////////////////////////////////////////////////




void setup()
{
   

    Serial.begin(115200);
    delay(10);
    ++bootCount;

    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    
    getDHT();
    getWater();
    
    wifiConnect();
    getDevice();

    root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);
    
    postData();
    delay(200);


    digitalWrite(2, HIGH);       // sets the digital pin 13 on
    delay(100);                  // waits for a second
    digitalWrite(2, LOW);        // sets the digital pin 13 off


    Serial.println("Going to sleep now");
    esp_deep_sleep_start();
}

void loop()
{


}


void wifiConnect()
{
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        ++count;
        Serial.print(" .");
        if (count > 50)
        {
            Serial.println("\nFailed to connect so having a snooze");
            esp_sleep_enable_timer_wakeup(TIME_TO_SNOOZE * uS_TO_S_FACTOR);
            esp_deep_sleep_start();
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}



void getDHT()
{
    dht.setup(PIN_DHT);
    delay(1000);

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();

    if(temperature>0) 
    {
        JsonObject& env = root.createNestedObject("env");
        env["temp_in_c"] = temperature;
        env["humidity"] = humidity;
        env["heatIndex"] = dht.computeHeatIndex(temperature, humidity, false);
    }
    else Serial.println("Failed to read a temperature");

}


void getDevice()
{

    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();

    uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
    Serial.printf("***ESP32 Chip ID = %04X%08X\n",(uint16_t)(chipid>>32),(uint32_t)chipid);//print High 2 bytes
    char buffer[200];
    sprintf(buffer, "%04X%08X",(uint16_t)(chipid>>32),(uint32_t)chipid);
    //sprintf(buffer, "esp32%" PRIu64, ESP.getEfuseMac());

    int vbatt_raw = 0;
    for (int i=0;i<SAMPLE;i++)
    {
        vbatt_raw += analogRead(PIN_POWER);
        delay(100);
    }
    vbatt_raw = vbatt_raw/SAMPLE;
    //float vbatt = map(vbatt_raw, 0, 4096, 0, 4200);


    JsonObject& device = root.createNestedObject("device");
    device["IP"] = WiFi.localIP().toString();
    device["RSSI"] = String(WiFi.RSSI());
    device["type"] = TYPE;
    device["name"] = name;
    device["chipid"] = buffer;
    device["bootCount"] = bootCount;
    device["wakeup_reason"] = String(wakeup_reason);
    device["vbatt_raw"] = vbatt_raw;
    device["vbatt"] = map(vbatt_raw, 0, 4096, 0, 4200);

}



void getWater()
{
    JsonObject& soil = root.createNestedObject("soil");
    float water = analogRead(PIN_SOIL);
    water = map(water, 0, 4095, 0, 1023);
    water = constrain(water, 0, 1023);
    soil["moisture"] = water;
}


void postData()
{

  if(WiFi.status()== WL_CONNECTED)
   {   //Check WiFi connection status
   
     HTTPClient http;    //Declare object of class HTTPClient
   
     http.begin(POST_URI);      //Specify request destination
     http.addHeader("Content-Type", "application/json");  //Specify content-type header
   
     //Serial.println(JSONmessageBuffer);

     int httpCode = http.POST(JSONmessageBuffer);   //Send the request
     String payload = http.getString();                  //Get the response payload
   
     Serial.println(httpCode);   //Print HTTP return code
     Serial.println(payload);    //Print request response payload
   
     Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());

     http.end();  //Close connection
   
   }
   else
   {   
      Serial.println("Error in WiFi connection");   
   } 


}
