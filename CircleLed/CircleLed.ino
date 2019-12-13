/*
 Name:		gift.ino
 Created:	2018/6/24 20:34:23
 Author:	TiAmo
*/
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN_LED			6


//蓝牙的引脚
#define PIN_BT_RX		10
#define PIN_BT_TX		11


// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS		16

//从EEPROM里面读取当前模式的地址
#define MODEL_DRESS		0
//亮度的地址
#define BRITNESS_DRESS	1
//延时的地址 也就是频率
#define DELAYVAL_DRESS	2

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_LED, NEO_GRB + NEO_KHZ800);

SoftwareSerial BT(PIN_BT_RX, PIN_BT_TX);



uint8_t model, delayval, britness;


void setup() {
	ledsInit();
}
void loop() {
	delayval = 600;
	//serialFun();
	fun1();
	fun2();
	fun3();
	fun4();
	fun5();
}
void serialFun() {
	if (BT.available())
	{
		String str = BT.readString();
		Serial.println(str);
		dealStr(str);
	}
	if (model >= 1 && model <= 5)
	{
		showByNum(model);
	}
	else if (model == 0)
	{
		showByNum(random(1, 5 + 1));
	}
	else {
		Serial.println("Wrong model");
		BT.println("Wrong model");
		delay(2000);
	}
}
void dealStr(String str) {
	if (str.indexOf('M') != -1)
	{
		model = str.substring(1).toInt();
		EEPROM.update(MODEL_DRESS, model);

		BT.println("Model:" + String(model) + '\t' + "OK");
	}
	if (str.indexOf('B') != -1)
	{
		britness = str.substring(1).toInt();
		EEPROM.update(BRITNESS_DRESS, britness);
		BT.println("Britness:" + String(britness) + '\t' + "OK");
	}
	if (str.indexOf('D') != -1)
	{
		delayval = str.substring(1).toInt();
		EEPROM.update(DELAYVAL_DRESS, delayval);
		BT.println("Delay:" + String(delayval) + '\t' + "OK");
	}

}
void showByNum(int num) {
	switch (num)
	{
	case 1:
		fun1();
		break;
	case 2:
		fun2();
		break;
	case 3:
		fun3();
		break;
	case 4:
		fun4();
		break;
	case 5:
		fun5();
	default:
		break;
	}
}
void ledsInit() {
	randomSeed(analogRead(A0));
	model = EEPROM.read(MODEL_DRESS);
	delayval = EEPROM.read(DELAYVAL_DRESS);
	britness = EEPROM.read(BRITNESS_DRESS);

	Serial.begin(9600);
	BT.begin(9600);

	Serial.println("Model:" + String(model));
	Serial.println("Britness:" + String(britness));
	Serial.println("Delay:" + String(delayval));

	BT.println("Model:" + String(model));
	BT.println("Britness:" + String(britness));
	BT.println("Delay:" + String(delayval));

	pixels.begin(); // This initializes the NeoPixel library.
	pixels.setBrightness(britness);
}


/*
轮圈 每颗灯珠随机颜色
*/
void fun1() {
	pixels.clear();
	pixels.setBrightness(britness);
	//int delayval = 100; // delay for half a second
	for (int i = 0; i < NUMPIXELS; i++) {

		// pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
		pixels.setPixelColor(i, pixels.Color(random(0, 255), random(0, 255), random(0, 255))); // Moderately bright green color.

		pixels.show(); // This sends the updated pixel color to the hardware.

		delay(delayval); // Delay for a period of time (in milliseconds).

	}
	for (int i = 0; i < NUMPIXELS; i++)
	{
		pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.

		pixels.show(); // This sends the updated pixel color to the hardware.
		delay(delayval); // Delay for a period of time (in milliseconds).
	}
}
/*
所有灯珠按正弦渐变
*/
void fun2() {
	pixels.clear();
	pixels.setBrightness(britness);
	//const int delayval = 20;
	int numRandom1 = random(15, 70);
	int numRandom2 = random(15, 70);
	int numRandom3 = random(15, 70);
	for (int i = 1; i <= 360; i++)
	{
		int r = 0, g = 0, b = 0;

		r = 127 + 127 * sin((i + numRandom1) * 3.1415926 / 180);
		g = 127 + 127 * sin((i + numRandom2) * 3.1415926 / 180);
		b = 127 + 127 * sin((i + numRandom3) * 3.1415926 / 180);
		for (int j = 0; j < NUMPIXELS; j++)
		{
			pixels.setPixelColor(j, pixels.Color(r, g, b)); // Moderately bright green color.
		}
		pixels.show(); // This sends the updated pixel color to the hardware.
		delay(delayval / 4); // Delay for a period of time (in milliseconds).

	}
}
/*
随机颜色转圈
*/
void fun3() {
	pixels.clear();
	pixels.setBrightness(britness);
	const int count = 10;
	//const int delayval = 200;
	int r, g, b;
	r = random(0, 255);
	g = random(0, 255);
	b = random(0, 255);
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < NUMPIXELS; j++)
		{
			if (j % 2 == 0) {
				pixels.setPixelColor(j, pixels.Color(r, g, b));
			}
			else
			{
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
			}
		}
		pixels.show();
		delay(delayval);
		for (int j = 0; j < NUMPIXELS; j++)
		{
			if (j % 2 == 1) {
				pixels.setPixelColor(j, pixels.Color(r, g, b));
			}
			else
			{
				pixels.setPixelColor(j, pixels.Color(0, 0, 0));
			}
		}
		pixels.show();
		delay(delayval);

	}
}
/*
随机颜色从两边转
*/
void fun4() {
	pixels.clear();
	pixels.setBrightness(britness);
	int r, g, b;
	r = random(0, 255);
	g = random(0, 255);
	b = random(0, 255);
	//const int delayval = 100;
	for (int i = 0; i < (NUMPIXELS / 2); i++)
	{
		for (int j = 0; j < (NUMPIXELS / 2) - i; j++)
		{
			for (int k = 0; k < (NUMPIXELS / 2) - i; k++)
			{
				if (j == k)
				{
					pixels.setPixelColor(k, pixels.Color(r, g, b));
					pixels.setPixelColor(NUMPIXELS - k - 1, pixels.Color(r, g, b));
				}
				else {
					pixels.setPixelColor(k, pixels.Color(0, 0, 0));
					pixels.setPixelColor(NUMPIXELS - k - 1, pixels.Color(0, 0, 0));
				}
			}
			delay(delayval);
			pixels.show();

		}
	}
}
/*
随机颜色 类似太极的追逐转
*/
void fun5() {
	pixels.clear();
	pixels.setBrightness(britness);
	int r, g, b;
	r = random(0, 255);
	g = random(0, 255);
	b = random(0, 255);
	int leds = 3;	//一端三盏灯
	int quanshu = 5;	//转的圈数
	for (int i = 0; i < quanshu; i++)
	{
		for (int j = 0; j < (NUMPIXELS / 2); j++)
		{
			pixels.clear();
			for (int k = 0; k < leds; k++)
			{//这个循环设置该亮的灯
				pixels.setPixelColor((j + k) % NUMPIXELS, pixels.Color(r, g, b));	//开始半圈的leds个灯
				pixels.setPixelColor((j + k + (NUMPIXELS / 2)) % NUMPIXELS, pixels.Color(r, g, b));//另一个半圈的leds个灯
			}
			pixels.show();
			delay(delayval);
		}
	}
}