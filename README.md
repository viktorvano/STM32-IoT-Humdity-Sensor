# STM32-IoT-Humdity-Sensor
 IoT humidity sensor using STM32 and AHT15
  
## Info about ESP8266 12F and IP Addresses  
  
###### ESP8266 12F Boot Modes  
  
![alt text](https://github.com/viktorvano/RGB-LED-Strip/blob/main/screenshots%20and%20files/boot-modes.jpg?raw=true)  
  
###### Network Settings info  
  
Your IP addresses in your LAN will be different and you need to change them in the project accortding to your router's network setting.  
ESP8266 will have a dofferent IP address in your LAN and also your smartphone will have different IP aswell.  
Android Studio project has a hardcoded IP address of the ESP8266 server and also LED Strip Scheduler has hard coded IP addresses of the ESP and the smartphone.  
  
## Wiring Diagram  
  

![alt text](https://github.com/viktorvano/STM32-IoT-Humdity-Sensor/blob/main/documents/schematic.png?raw=true)    
  
  
#### Screenshots and Pictures  
Web Page screenshot  
![alt text](https://github.com/viktorvano/STM32-IoT-Humdity-Sensor/blob/main/documents/web-humidity.png?raw=true)  
  
IoT sensor photo  
![alt text](https://github.com/viktorvano/STM32-IoT-Humdity-Sensor/blob/main/documents/photo.jpg?raw=true)  
  
AHT15 Humidity sensor read registers  
![alt text](https://github.com/viktorvano/STM32-IoT-Humdity-Sensor/blob/main/documents/AHT15-translated.png?raw=true)    
  
  
## Software  
  
#### WiFi Credentials  
Update your WiFi credentials in ESP8266.h:  
```C
#define WiFi_Credentials	"AT+CWJAP=\"WiFiSSID\",\"WiFiPASSWORD\"\r\n"
```  
  
#### Code Snipped of ESP8266.c  
Function messageHandler() does two things:
- responds to "GET" request sent by a browser and sends back a response  
- auto reconnects to a WiFi Access Point, if disconnected  
  
Response is in sendData(), where:  
- ```"HTTP/1.1 200 OK\r\n"``` says that the response by server is OK  
- ```"Content-Length: %i\r\n"``` gives info about upcomming page html code length  
- ```"Content-Type: text/html\r\n\r\n"``` says that the upcomming code is html (other option for a plain text is ```"Content-Type: text/plain\r\n\r\n"```)  
  
In "outputString" is the HTML code, where:  
- ```<title>STM32 IoT</title>``` in the title visible in a web browser tab  
- ```<meta http-equiv=\"refresh\" content=\"30\">``` makes the webpage auto refresh in 30 seconds  
- ```<body><h1>Humidity: %i%%</h1></body>``` is body of the HTML code
```C
void messageHandler()
{
	__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE);
	int position = 0;
	if((position = string_contains((char*)buffer, "GET", buffer_index)) != -1)
	{
		sendData();
	}else if(string_contains((char*)buffer, "+CWJAP:", buffer_index) != -1
			&& (string_contains((char*)buffer, "FAIL", buffer_index) != -1
			|| string_contains((char*)buffer, "DISCONNECT", buffer_index) != -1))
	{
		HAL_UART_Transmit(&huart1, (uint8_t*)WiFi_Credentials, strlen(WiFi_Credentials), 100);
	}
	ESP_Clear_Buffer();
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

void sendData()//sends data compatible with a browser
{
	char outputString[300], cipsend[50], response[600];
	memset(outputString, 0, 300);
	memset(cipsend, 0, 50);
	memset(response, 0, 600);

	sprintf(outputString, "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><title>STM32 IoT</title><meta http-equiv=\"refresh\" content=\"30\"></head><body><h1>Humidity: %i%%</h1></body></html>", (int)AHT15_relative_humidity);
	sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %i\r\nContent-Type: text/html\r\n\r\n%s", strlen(outputString), outputString);
	sprintf(cipsend, "AT+CIPSEND=0,%i\r\n", strlen(response));

	HAL_UART_Transmit(&huart1, (uint8_t*)cipsend, strlen(cipsend), 100);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)response, strlen(response), 100);
	HAL_Delay(50);
	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPCLOSE=0\r\n", strlen("AT+CIPCLOSE=0\r\n"), 100);
}
```  
Code snippet of main.c, where:
- humidity is measured every 30 seconds
- messageHandler() is called when ESP8266 sends a string to STM32
- the whole system is reset every 24 hours for safety to prevent ESP8266 firmware from getting stuck (or STM32 also)
```C
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);// 20 ms
  HAL_TIM_Base_Start_IT(&htim3);// 1 s
  HAL_Delay(100);
  AHT15_reset();
  HAL_Delay(100);
  AHT15_init();
  AHT15_read_data();
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  ESP_Server_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(measure_humidity)
	  {
		  AHT15_read_data();
		  measure_humidity = 0;
	  }

	  if(messageHandlerFlag)
	  {
		  messageHandler();
		  messageHandlerFlag = 0;
	  }

	  if(seconds > 86400)// 24 hours
		  NVIC_SystemReset();
  }
  /* USER CODE END 3 */
```
