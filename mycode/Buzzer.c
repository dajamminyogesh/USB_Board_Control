#include "Buzzer.h"
#include "Delay.h"
#include "GPIO.H"

#define Set_Buzzer()  	BUZZER_PIN = 1
#define Reset_Buzzer()  BUZZER_PIN = 0

void BuzzerPlay(unsigned int delaytime){
	unsigned int i = 0;
	Set_Buzzer();
	delayms(delaytime);
	Reset_Buzzer();
}

void Buzzer_config()
{
	Reset_Buzzer(); /*关蜂鸣器*/
}

void HotStart()		/*加热开始*/
{
  BuzzerPlay(30);
  delayms(20);
  BuzzerPlay(20);
  delayms(20);
  BuzzerPlay(15);
}