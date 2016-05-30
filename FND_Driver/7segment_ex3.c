#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <stdint.h>
#include <time.h>

#define SDATA 6
#define STR_CLK 13
#define SHR_CLK 19
#define SHR_CLEAR 26

void allclear(void);

void init(void)
{
	if(wiringPiSetupGpio() == -1){
		fpintf(stderr, "wiringPiSetupGpio() error\n");
		exit(1);
	}
	pinMode(SDATA, OUTPUT);
	pinMode(STR_CLK, OUTPUT);
	pinMode(SHR_CLK, OUTPUT);
	pinMode(SHR_CLEAR, OUTPUT);
	allclear();
}
void allclear(void)
{
	digitalWrite(SHR_CLEAR, 0);
	digitalWrite(SHR_CLEAR, 1);
	digitalWrite(STR_CLK, 0);
	digitalWrite(STR_CLK, 1);
}

void set8(uint8_t value)
{
	int i;
	for(i = 0 ; i < 8 ; i++){
		int mask = 0b1 << i;
		if((value & mask) == 0)
			digitalWrite(SDATA, 0);
		else
			digitalWrite(SDATA, 1);
			digitalWrite(SHR_CLK, 0);
			digitalWrite(SHR_CLK, 1);
	}
	digitalWrite(STR_CLK,0);
	digitalWrite(STR_CLK,1);
}

int main(void)
{
	time_t t;
	struct tm *tm;
	uint8_t arr[]={	~0b10000000,
					~0b01000000,
					~0b00100000,
					~0b00010000,
					~0b00001000,
					~0b00000100,
					~0b00000010,
					~0b00000001,
					~0b11111110,
					~0b11100110};
	int digit1, digit2;
	init();
	pinMode(DIGIT1, OUTPUT);
	pinMode(DIGIT2, OUTPUT);

	setbuf(stdout,NULL);
	for(;;){
		time(&t);
		tmp = localtime(&t);
		printf("%02d:%02d:%02d\r", tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
		digit1 = tmp->tm_sec / 10;
		digit2 = tmp->tm_sec % 10;
		digitalWrite(DIGIT1, HIGH);
		digitalWrite(DIGIT2, LOW);
		set8(arr[digit1]);
		delay(10);

		digitalWrite(DIGIT1, LOW);
		digitalWrite(DIGIT2, HIGH);
		set8(arr[digit2]);
		delay(1000);
	}
	return 1;
}
