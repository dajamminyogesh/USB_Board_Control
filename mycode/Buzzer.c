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

void HotHandle()		/*加热开始*/
{
  BuzzerPlay(60);
  delayms(40);
  BuzzerPlay(40);
  delayms(40);
  BuzzerPlay(30);
}