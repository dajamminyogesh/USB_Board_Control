#include "GPIO.h"
#include "MATH.H"
#include "STDIO.H"
#include "Led.h"
#include "config.h"
#include "delay.h"
#include "Task.h"
#include "Timer.h"
#include "Buzzer.h"

int main() {  		// MCU STC8H1K08  // 晶振频率 24Mhz
  GPIO_config();
  Buzzer_config();
  SysStart();
  Timer0_Init();
  EA = 1;
  while (1) 
  {
    MainTaskHandle();
  }
}