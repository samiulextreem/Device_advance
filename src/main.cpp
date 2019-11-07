#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <FS.h>
#include <string.h>
#include <U8g2lib.h>

#ifndef STASSID
#define STASSID "HSTU"
#define STAPSK "passcode"
#define BOOTPIN D5
#define NEXT D6
#define SELECT D7
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
bool ota_update_abailable = true;
U8G2_ST7565_ZOLEN_128X64_F_4W_SW_SPI LCD(U8G2_MIRROR, D0, D1, D2, D3, D4);

void LCD_print(String x);
void LCD_clear();



void setup(){
    Serial.begin(9600);
	LCD.begin();
	LCD.clearBuffer();
	LCD.setFont(u8g2_font_siji_t_6x10);
	LCD.setContrast(80);
	Serial.println("Hold boot pin to enter boot mode");
	pinMode(BOOTPIN,INPUT);
	pinMode(NEXT,INPUT);
	pinMode(SELECT,INPUT);
	delay(2000);
	SPIFFS.begin();

	if(digitalRead(BOOTPIN) == LOW){
		Serial.println("Entering boot mode.Trying to connect wifi......");
		LCD_print("Entering boot mode.Trying to connect wifi......");
		while(1==1){
			WiFi.mode(WIFI_STA);
			WiFi.begin(ssid, password);
			
			ArduinoOTA.onStart([]() {
				String type;
				if (ArduinoOTA.getCommand() == U_FLASH)
				{
					type = "sketch";
				}
				else
				{ // U_SPIFFS
					type = "filesystem";
				}

				// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
				Serial.println("Start updating " + type);
				LCD.clearDisplay();
				LCD.home();
				LCD.println("start updating .." + type);
				LCD.sendBuffer();
			});
			ArduinoOTA.onEnd([]() {
				Serial.println("\nEnd.rebooting in 1s");
				LCD_print("\nEND\n");
				LCD_print("rebooting in 1s......");
				delay(500);
				LCD.sendBuffer();
			});
			ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
				Serial.printf("uploading : %u%%\r", (progress / (total / 100)));
				LCD.setCursor(0,10);
				LCD.printf("uploading  : %u%%\r", (progress / (total / 100)));
				LCD.sendBuffer();
			});
			ArduinoOTA.onError([](ota_error_t error) {
				Serial.printf("Error[%u]: ", error);
				LCD.printf("Error[%u]: ", error);
				LCD.sendBuffer();
				if (error == OTA_AUTH_ERROR)
				{
					Serial.println("Auth Failed");
					LCD_print("auth failed");
				}
				else if (error == OTA_BEGIN_ERROR)
				{
					Serial.println("Begin Failed");
					LCD_print("Begin failed");
				}
				else if (error == OTA_CONNECT_ERROR)
				{
					Serial.println("Connect Failed");
					LCD_print("connection failed");
				}
				else if (error == OTA_RECEIVE_ERROR)
				{					
					Serial.println("Receive Failed");
					LCD_print("Recieve failed");
				}
				else if (error == OTA_END_ERROR)
				{
					Serial.println("End Failed");
					LCD_print("End failed");
				}
			});
			ArduinoOTA.begin();
			delay(1000);
			Serial.print("IP address: ");
			LCD_print("IP adress ......");
			Serial.println(WiFi.localIP());
			LCD.setCursor(0,20);
			LCD.print(WiFi.localIP());
			LCD.sendBuffer();
			while (WiFi.waitForConnectResult() == WL_CONNECTED)
			{
				Serial.println("wifi connected");
				ArduinoOTA.handle();
				delay(100);
			}
		}
	}
	if(digitalRead(BOOTPIN) == HIGH){
		LCD_print("Entering operational mode..");
		delay(50);
	}
}


void loop(){
	LCD_clear();
	Dir dir = SPIFFS.openDir("");
	while (dir.next()){
		String path = dir.fileName();
		Serial.print(path);
		LCD_print(path);
		File f = SPIFFS.open(path, "r");
		delay(500);
		while(digitalRead(NEXT) == HIGH){
			delay(200);
			if (digitalRead(SELECT) == LOW){
				String data;
				if (!f){
					Serial.println("file open failed");
					LCD_print("sorry.something wrong .file open failed");
				}
				else{
					for (unsigned int i = 1; i < (f.size() + 1); i++){
						data = data +(char)f.read();
						if(i-f.size()<60){
							Serial.println(data);
							LCD_print(data);
							delay(300);
							for (unsigned int p = 0; p < data.length() + 1; p++){
								data.remove(p);
							}
							while (digitalRead(NEXT) == HIGH)
							{
								if (digitalRead(NEXT) == LOW)
								{
									break;
								}
								delay(100);
							}
						}
						if (i % 60 == 0 ){
							Serial.println(data);
							LCD_print(data);
							for(unsigned int p = 0; p< data.length() +1;p++){
								data.remove(p);
							}
							delay(300);
							while (digitalRead(NEXT) == HIGH)
							{
								if (digitalRead(NEXT) == LOW)
								{
									break;
								}
								delay(100);
							}
						}
					}
					Serial.println("");
					Serial.println("THE END");
					LCD_print("THE END");
					Serial.println("------------------------------------");
					Serial.println("");
					delay(300);
					break;
				}
			}
			if (digitalRead(NEXT) == LOW){
				break;
			}
		}
	
	}
	delay(300);
}
void LCD_print(String x)
{
	LCD_clear();
	int x_pos = 0;
	int y_pos = 10;
	LCD.setCursor(x_pos, y_pos);

	for (int k = 0; k < 62; k++)
	{
		if (!x[k])
		{
			for (int i = 0; i < k + 1; i++)
			{
				LCD.print(x[i]);
				if((k - i) <4){
					LCD.sendBuffer();
				}
				if(i%4 == 0){
					LCD.sendBuffer();
				}
				

				if (i % 20 == 0 && i != 0)
				{
					y_pos = y_pos + 10;
					LCD.setCursor(x_pos, y_pos);
				}
				delay(5);
			}
			break;
		}
	}
}

void LCD_clear()
{
	LCD.clearDisplay();
	LCD.home();
}