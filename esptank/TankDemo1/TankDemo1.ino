//define motor pwm pins, can be changed to anything in most cases (l/r = left/right F/R = Forward/Reverse)
const int lF = 13;
const int lR = 12;
const int rF = 14;
const int rR = 16;

//buzzer pindef
const int buzz = 5;

//Drive function w/ Input of -1023 to 1023 for Left & Right Motor
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

int buzzer = 0;

//define motor & buzzer pins as outputs
void setup()
{
	pinMode(lF, OUTPUT);
	pinMode(lR, OUTPUT);
	pinMode(rF, OUTPUT);
	pinMode(rR, OUTPUT);
	pinMode(buzz, OUTPUT);
}

void loop()
{
	//buzzer loop (run through the drive loop, and every 3rd time, turn on the buzzer for 500msecs)
	if (buzzer <= 2)
	{
		//forward
		drive(1023, 1023);
		delay(1000);
		drive(0, 0);
		delay(100);

		//back
		drive(-1023, -1023);
		delay(1000);
		drive(0, 0);
		delay(100);
	
		//left
		drive(1023, -1023);
		delay(1000);
		drive(0, 0);
		delay(100);

		//right
		drive(-1023, 1023);
		delay(1000);
		drive(0, 0);
		delay(100);

	}
	else
	{
		buzzer = 0;
	
		//horn
		digitalWrite(buzz, 1);
		delay(500);
		digitalWrite(buzz, 0);
		delay(100);
	}
	buzzer++;	
}
