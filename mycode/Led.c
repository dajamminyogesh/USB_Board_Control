#include "GPIO.h"
#include "Led.h"
#include "delay.h"

#define SET_LED_ON(V)   LED##V##_PIN = LEDON
#define SET_LED_OFF(V)  LED##V##_PIN = LEDOFF

#ifndef BV
#define BV(n)      (1 << (n))
#endif

u8 LedState = 0;
static u8 LastLedState = 0;
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
}

//控制指定led亮
void SetOneLight(LED_DEV setled){
    LedState = 0;
    LedState |= BV(setled);
    UpdateLedState();
}

// 控制所有LED状态
void SetAllLed(u8 state){
    LedState = state; 
    UpdateLedState();
}

//控制led全灭
void SetAllOff(){
    u8 LedState = 0;
    SetAllLed(LedState);
}

void SDMountLedMode()  /*U盘安装效果*/
{
    SetAllLed(0X0F);
    delayms(500);
    SetAllOff();
}

void SDUninstallLedMode()  /*U盘卸载效果*/
{
    SetAllLed(0XF0);    //红灯全亮
    delayms(500);
    SetAllOff();
}

void SysStart()  /*开机效果*/
{
    u8 i,TargLedState = 0; 
    for(i=8; i>0; i--)
    {
        TargLedState |= BV(i-1);
        SetAllLed(TargLedState);
        delayms(100);
        if(i==5) { TargLedState=0; delayms(400); }
    }
    delayms(200);
    SetAllOff();
}