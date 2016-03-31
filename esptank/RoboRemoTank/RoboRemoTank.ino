#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//wificonfig
WiFi.setMode(WIFI_AP);
const char *ssid = "esptank";
const char *pw = "huzzah!8266";
IPAddress ip(192, 168, 0, 1); 
IPAddress netmask(255, 255, 255, 0);
const int port = 9876;
WiFiServer server(port);
WiFiClient client;

//RoboRemo datagrab buffer array (cmd)
char cmd[100]; //buffer
int cmdIndex;  //char reference within buffer
unsigned long lastCmdTime = 60000;
unsigned long aliveSentTime = 0;

// checks if cmd starts with st
boolean cmdStartsWith(const char *st)
{
	for(int i=0;;i++)
	{
		if(st[i]==0) return true;
		if(cmd[i]==0) return false;
		if(cmd[i]!=st[i]) return false;;
	}
	return false;
}

//joyIn intermediary values (exeCmd => joyX => looped drive command)
int joyL = 0;
int joyR = 0;

//extra pinDefs
const int buzz = 5;

//execute command from cmd buffer
void exeCmd()
{
	//reset lastCmdTime timer
	lastCmdTime = millis();

	//check for "ch1" id value => joyL
	if(cmdStartsWith("ch1"))
	{
		joyL = (int)atof(cmd+4);
	}

	//check for "ch2" id value => joyR
	if(cmdStartsWith("ch2"))
	{
		joyR = (int)atof(cmd+4);
	}

	//check for "chb" btn => buzzer
	if(cmdStartsWith("chb 1"))
	{
		digitalWrite(buzz, 1);
	}
	else
	{
		digitalWrite(buzz, 0);
	}
}

//motor control pinDefs (left/right | Forward/Reverse)
const int lF = 13;
const int lR = 12;
const int rF = 14;
const int rR = 16;

//motor control (pwm motors w/ input from -1023 to 1023 on each channel)
void drive(int L, int R)
{
	//Left Side
	if (L > 0)
	{
		analogWrite(lF, L);
		analogWrite(lR, 0);
	}
	else if (L == 0)
	{
		analogWrite(lF, 0);
		analogWrite(lR, 0);
	}
	else
	{
		analogWrite(lR, map(L, -1, -1023, 1, 1023));
		analogWrite(lF, 0);
	}

	//Right Side
	if (R > 0)
	{
		analogWrite(rF, R);
		analogWrite(rR, 0);
	}
	else if (R == 0)
	{
		analogWrite(rF, 0);
		analogWrite(rR, 0);
	}
	else
	{
		analogWrite(rR, map(R, -1, -1023, 1, 1023));
		analogWrite(rF, 0);
	}
}

void setup()
{
	//reset values
	delay(1000);
	cmdIndex = 0;

	//start TCP server
	WiFi.softAPConfig(ip, ip, netmask);
	WiFi.softAP(ssid, pw);
	server.begin();

	//serial wifi info display (must reset)
	Serial.begin(115200);
	Serial.println("\n\nESP8266 Tank v1 via RoboRemo");
	Serial.println((String)"SSID: " + ssid);
	Serial.println((String)"PASS: " + pw);
	Serial.println((String)"@ " + ip.toString() + ":" + port);
	//Serial.end();

	//motor control pinModes
	pinMode(lF, OUTPUT);
	pinMode(lR, OUTPUT);
	pinMode(rF, OUTPUT);
	pinMode(rR, OUTPUT);

	//extra pinModes
	pinMode(buzz, OUTPUT);
}

void loop()
{
	//contact loss behavior
	if(millis() - lastCmdTime > 5000)
	{
		drive(0, 0);
	}

	//if no client => open server
	if(!client.connected()) {
		client = server.available();
		return;
	}

	//RoboRemo datagrab
	if(client.available())
	{
		char c = (char)client.read(); //read char from RoboRemo

		if(c == '\n') //if at command end
		{
			cmd[cmdIndex] = 0; //reset buffer
			exeCmd(); //execute command
			cmdIndex = 0; //reset buffer reference
		}
		else {
		cmd[cmdIndex] = c; //add to cmd buffer
		if(cmdIndex<99) cmdIndex++;
		}
	}

	//RoboRemo cmd => drive command
	drive(joyL, joyR);

	//alive signal => RoboRemo "LED" (id='alive')
	if(millis() - aliveSentTime > 500)
	{
		client.write("alive 1\n");
		aliveSentTime = millis();
	}
}
