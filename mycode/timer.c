#include "timer.h"
#include "GPIO.h"
#include "Led.h"
#include "Task.h"

static unsigned int T0Count1 = 0;

/********************* Timer0中断函数************************/
void Timer0_Isr(void) interrupt 1
{
	T0Count1++;
	if(T0Count1>10)
	{
		T0Count1=0;
    	MainTaskLoop();
	}
}

void Timer0_Init(void)		//1毫秒@24.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x40;				//设置定时初始值
	TH0 = 0xA2;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}

