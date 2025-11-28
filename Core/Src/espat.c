/*
 * espat.c
 *
 *  Created on: Jan 31, 2025
 *      Author: meldundas
 */

#include "espat.h"

#include "dht.h"
extern DHT_data dht_data;

extern ATC_HandleTypeDef espat;
extern uint32_t count;
extern float thermTemp;
extern bool connected;

int16_t resp = 0;
uint8_t responseData[80];

uint16_t cipstartCount = 0;
uint16_t notSentCount = 0;
uint16_t sentCount = 0;
uint16_t wifiNotConnectedCount = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->Instance == USART1) {
		ATC_IdleLineCallback(&espat, Size);
	}
}

int WiFiConnect() {
	char data[80];
	//After 1st programming need to AT+CWMODE=1
	resp = ATC_SendReceive(&espat, "AT+CWMODE=1\r\n", 100, NULL, 100, 1,
			"OK\r\n");

	//check if connected to network
	//2: ESP32-C3 station has connected to an AP, and got an IPv4 address.
	//resp == 1 when we are connected and have an ip
	resp = ATC_SendReceive(&espat, "AT+CWSTATE?\r\n", 100, NULL, 1000, 1,
			"+CWSTATE:2");

	//if not connected reset esp32
	if (!resp) {
		HAL_GPIO_WritePin(WIFI_nRST_GPIO_Port, WIFI_nRST_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(WIFI_nRST_GPIO_Port, WIFI_nRST_Pin, GPIO_PIN_SET);
		HAL_Delay(5000);

		//check network connection again
		//if not automatically connected on reset, connect to network

		resp = ATC_SendReceive(&espat, "AT+CWSTATE?\r\n", 100, NULL, 1000, 1,
				"+CWSTATE:2");
		if (!resp) {

			/********* AT+CWJAP="SSID","PASSWD" **********/
			//resp: 	1 if connected
			//			0 if not
			sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);

			resp = ATC_SendReceive(&espat, data, 200, NULL, 10000, 1, "OK\r\n"); //"GOT IP\r\n"
		}

	}

	return resp;

//	resp = ATC_SendReceive(&espat, "AT+CIPMUX=0\r\n", 1000, NULL, 100, 1, "OK\r\n");
}

int sendToThingspeak() {
	char data[80];
	char local_buf[80];

	if (!WiFiConnect()) {
		connected = false;
		wifiNotConnectedCount++;
	} else {
		connected = true;
		//send to thingspeak
		resp = ATC_SendReceive(&espat,
				"AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n", 500, NULL,
				5000, 1, "OK\r\n");

		sprintf(data, "GET /update?api_key=%s&field%d=%0.1f&field%d=%0.1f\r\n",
				WRITEAPIKEY, 1, dht_data.temp, 2, dht_data.hum);
		int len = strlen(data);

		sprintf(local_buf, "AT+CIPSEND=%d\r\n", len);
		resp = ATC_SendReceive(&espat, local_buf, 100, NULL, 100, 1, "OK\r\n");

		resp = ATC_SendReceive(&espat, data, 1000, NULL, 100, 1, "SEND OK\r\n");

		resp == 1 ? sentCount++ : notSentCount++;

//Closing the connection makes it worse
//	resp = ATC_SendReceive(&espat, "AT+CIPCLOSE\r\n", 1000, NULL, 100, 1, "OK\r\n");

	}

	//resp: 1 data sent
	//		0 error in tx
	return resp;

}

int getNTP() {
	uint8_t data[50] = { };
	uint8_t *myptr = data;
	uint8_t time[50] = { };

	HAL_GPIO_WritePin(WIFI_nRST_GPIO_Port, WIFI_nRST_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(WIFI_nRST_GPIO_Port, WIFI_nRST_Pin, GPIO_PIN_SET);
	HAL_Delay(5000);

	resp = ATC_SendReceive(&espat, "AT+CIPSNTPCFG=1,-8,\"time.google.com\"\r\n",
			100, NULL, 5000, 1, "OK\r\n");

	resp = ATC_SendReceive(&espat, "AT+CIPSNTPTIME?\r\n", 100, NULL, 5000, 2,
			"+CIPSNTPTIME", "+TIME_UPDATED");

	strcpy(responseData, espat.pRxBuff);

//	copy_until_delim(time, data, resp);

	return resp;
}

int sendToMQTT() {
	char data[80];
//	char local_buf[80];

	if (!WiFiConnect()) {
		wifiNotConnectedCount++;

		//not connected to wifi
		return 0;
	} else {

		//setup publish string
		sprintf(data, "AT+MQTTPUB=0,\"esp32c3/button\",\"%0.1f\",0,0\r\n",
				dht_data.temp);

		resp =
				ATC_SendReceive(&espat,
						"AT+MQTTUSERCFG=0,1,\"publisher\",\"esp32c3\",\"112233\",0,0,\"\"\r\n",
						100, NULL, 500, 1, "OK\r\n");

		resp = ATC_SendReceive(&espat, "AT+MQTTCONN?\r\n", 100, NULL, 100, 1,
				"+MQTTCONN:0,4,1");

		//if not connected, try to connect
		//setup for automatic reconnection
		//broker.emqx.io
		if (!resp) {
			resp = ATC_SendReceive(&espat,
					"AT+MQTTCONN=0,\"35.172.255.228\",1883,1\r\n", 100, NULL,
					5000, 1, "OK\r\n");
		}

		//send MQTT
		//1 -> OK
		//0 -> error
		resp = ATC_SendReceive(&espat, data, 100, NULL, 500, 1, "OK\r\n");

		return resp;
	}
//Closing the connection makes it worse
//	resp = ATC_SendReceive(&espat, "AT+CIPCLOSE\r\n", 1000, NULL, 100, 1, "OK\r\n");

}

int subscribeToMQTT() {
	char data[80];

	if (!WiFiConnect()) {
		wifiNotConnectedCount++;

		//not connected to wifi
		return 0;
	} else {

		//setup subscribe string
		sprintf(data, "AT+MQTTSUB=0,\"esp32c3/led\",1\r\n");

		resp =
				ATC_SendReceive(&espat,
						"AT+MQTTUSERCFG=0,1,\"subscriber\",\"esp32c3\",\"112233\",0,0,\"\"\r\n",
						100, NULL, 500, 1, "OK\r\n");

		resp = ATC_SendReceive(&espat, "AT+MQTTCONN?\r\n", 100, NULL, 100, 1,
				"+MQTTCONN:0,4,1");

		//if not connected, try to connect
		//setup for automatic reconnection
		//broker.emqx.io
		if (!resp) {
			resp = ATC_SendReceive(&espat,
					"AT+MQTTCONN=0,\"35.172.255.228\",1883,1\r\n", 100, NULL,
					5000, 1, "OK\r\n");
		}

		//send MQTT
		//1 -> OK
		//0 -> error
		resp = ATC_SendReceive(&espat, data, 100, NULL, 500, 1, "OK\r\n");

		return resp;
	}
//Closing the connection makes it worse
//	resp = ATC_SendReceive(&espat, "AT+CIPCLOSE\r\n", 1000, NULL, 100, 1, "OK\r\n");

}

