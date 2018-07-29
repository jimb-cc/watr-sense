# watr-sense
Alternative firmware for HiGrow and other ESP32 based soil moisture sensors https://watr.works

Depending on configuration, the POST request will contain the following JSON Packet:

```javascript
{
	"body" : {
		"env" : {
			"temp_in_c" : 29,
			"humidity" : 61,
			"heatIndex" : 31.18839
		},
		"soil" : {
			"moisture" : 766
		},
		"device" : {
			"IP" : "192.168.1.249",
			"RSSI" : "-69",
			"type" : "HiGrow",
			"name" : "back_3",
			"chipid" : "6D32962DE6B4",
			"bootCount" : 2,
			"wakeup_reason" : "3",
			"vbatt_raw" : 4095,
			"vbatt" : 4198
		}
	}
} ```
