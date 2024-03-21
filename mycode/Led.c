#include "GPIO.h"
#include "Led.h"
#include "delay.h"

#define SET_LED_ON(V)   LED##V##_PIN = LEDON
#define SET_LED_OFF(V)  LED##V##_PIN = LEDOFF

#ifndef BV
#define BV(n)      (1 << (n))
#endif

u16 LedState = 0;
static u16 LastLedState = 0;
//更新LED状态
void UpdateLedState()
{
    if(LedState & BV(0))  SET_LED_ON(1);  else SET_LED_OFF(1);
    if(LedState & BV(1))  SET_LED_ON(2);  else SET_LED_OFF(2);
    if(LedState & BV(2))  SET_LED_ON(3);  else SET_LED_OFF(3);
    if(LedState & BV(3))  SET_LED_ON(4);  else SET_LED_OFF(4);
    if(LedState & BV(4))  SET_LED_ON(5);  else SET_LED_OFF(5);
    if(LedState & BV(5))  SET_LED_ON(6);  else SET_LED_OFF(6);
    if(LedState & BV(6))  SET_LED_ON(7);  else SET_LED_OFF(7);
    if(LedState & BV(7))  SET_LED_ON(8);  else SET_LED_OFF(8);
    if(LedState & BV(8))  SET_LED_ON(9);  else SET_LED_OFF(9);
    if(LedState & BV(9))  SET_LED_ON(10);  else SET_LED_OFF(10);
}

//控制指定led亮
void SetOneLight(LED_DEV setled){
    LedState = 0;
    LedState |= BV(setled);
    UpdateLedState();
}

// 控制所有LED状态
void SetAllLed(u16 state){
    LedState = state; 
    UpdateLedState();
}

//控制led全灭
void SetAllOff(){
    u16 LedState = 0;
    SetAllLed(LedState);
}

void SDMountMode()  /*U盘安装效果*/
{
    SetAllLed(0X0F);
    delayms(200);
    SetAllOff();
}

void SDUninstallMode()  /*U盘卸载效果*/
{
    SetAllLed(0XF0);    //红灯全亮
    delayms(200);
    SetAllOff();
}

void SysStart()  /*开机效果*/
{
    u8 i;
    u16 TargLedState = 0; 
    for(i=LED_DEV_NUM; i>0; i--)
    {
        TargLedState |= BV(i-1);
        SetAllLed(TargLedState);
        delayms(40);
        if(i==6) { TargLedState=0; delayms(200); }
    }
    delayms(200);
    SetAllOff();
}