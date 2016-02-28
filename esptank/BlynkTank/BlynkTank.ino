#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//blynk applet auth key
char auth[] = "54045ebfa1744b67aa9e054426a9612c";

//motor control pin defs
const int lF = 13;
const int lR = 12;
const int rF = 14;
const int rR = 16;

//virtual joy defs
int joylF = 0;
int joylR = 0;
int joyrF = 0;
int joyrR = 0;

void setup()
{
	//wifi login (auth, "usrname", "pass")
	Blynk.begin(auth, "belkin.7e2", "67bm7749");

	//motor control pinModes
	pinMode(lF, OUTPUT);
	pinMode(lR, OUTPUT);
	pinMode(rF, OUTPUT);
	pinMode(rR, OUTPUT);
}

void loop()
{
	//blynk datagrab
	Blynk.run();

	//pwmvars => motor pwm
	analogWrite(lF, joylF);
	analogWrite(lR, joylR);
	analogWrite(rF, joyrF);
	analogWrite(rR, joyrR);
}	

//Left virtjoy datagrab interrupt => pwmvars
BLYNK_WRITE(V0)
{
	int joyL = param[0].asInt();

	if (joyL > 0)
	{
		joylF = joyL;
		joylR = 0;
	}
	else if (joyL == 0)
	{
		joylF = 0;
		joylR = 0;
	}
	else
	{
		joylR = map(joyL, -1, -1023, 1, 1023);
		joylF = 0;
	}
}

//Right virtjoy datagrab interrupt => pwmvars
BLYNK_WRITE(V1)
{
	int joyR = param[0].asInt();

	if (joyR > 0)
	{
		joyrF = joyR;
		joyrR = 0;
	}
	else if (joyR == 0)
	{
		joyrF = 0;
		joyrR = 0;
	}
	else
	{
		joyrR = map(joyR, -1, -1023, 1, 1023);
		joyrF = 0;
	}
}




