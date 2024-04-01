#include "GPIO.h"
#include "Led.h"
#include "delay.h"
#include "string.h"

#define SET_LED_ON(V)   LED##V##_PIN = LEDON
#define SET_LED_OFF(V)  LED##V##_PIN = LEDOFF

#ifndef BV
#define BV(n)      (1 << (n))
#endif

// LED 状态数组
unsigned char led_status[LED_DEV_NUM] = {0};

//更新LED状态
void UpdateLedState()
{
    // 根据 LED 状态数组更新单片机引脚
    P1 = (led_status[8] << 0) | (led_status[5] << 7);
    P3 = (led_status[9] << 0) | (led_status[4] << 1) | (led_status[1] << 3) | (led_status[6] << 4)
                              | (led_status[2] << 5) | (led_status[7] << 6) | (led_status[3] << 7);
    P5 = (led_status[0] << 4);
}

//控制led全灭
void SetAllOff(){
    u16 LedState = 0;
    SetAllLed(LedState);
}

//控制指定led亮
void SetOneLight(LED_DEV setled)
{
    SetAllOff();
    led_status[setled] = LEDON;
    UpdateLedState();
}

// 控制所有LED状态
void SetAllLed(u16 state){
    u8 i = 0;
    for(i; i < LED_DEV_NUM; i++)
    {
        if(state & BV(i)) led_status[i] = LEDON;
        else              led_status[i] = LEDOFF;
    }
    UpdateLedState();
}

void SDChangeMode(u8 mode)  /*U盘安装效果*/
{
    u8 i = 0;
    for(i; i < LED_DEV_NUM; i++)
    {
        if(i < LED_DEV_NUM/2) led_status[i] = !mode;
        else                  led_status[i] = mode;
    }
    UpdateLedState();
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
        if(i==(LED_DEV_NUM/2 + 1)) 
        { 
            TargLedState=0; 
            delayms(200); 
        }
    }
    delayms(200);
    SetAllOff();
}