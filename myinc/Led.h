#ifndef __LED_H
#define __LED_H

#define LEDON  0
#define LEDOFF 1

typedef enum {
    LED_RED1,
    LED_RED2,
    LED_RED3,
    LED_RED4,
    LED_RED5,
    LED_GREEN1,
    LED_GREEN2,
    LED_GREEN3,
    LED_GREEN4,
    LED_GREEN5,
    LED_DEV_NUM
}LED_DEV;

//控制单个led亮
void SetOneLight(LED_DEV setled);
// 控制所有LED状态
void SetAllLed(u16 state);
//控制led全灭
void SetAllOff();
//控制led全亮
void SetAllOn();

void SDChangeMode(u8 mode);  /*U盘安装效果*/
void SysStart();  /*开机效果*/
#endif // __LED_H