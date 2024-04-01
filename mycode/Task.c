#include	"Task.h"
#include	"Led.h"
#include	"GPIO.h"
#include	"config.h"
#include "Buzzer.h"
#include "delay.h"

#define LEDG_START_NUM  4
static u8 isHasSD = false;                            /*是否有U盘*/
static u8 isHoting = false;                           /*是否加热*/
static u8 isChangeSD = false;                         /*U盘状态是否改变*/
static u8 isChangeState = false;                      /*状态是否改变*/

static u8 lastisIdle = false;
static u8 led1sta,led2sta,led3sta,led4sta,led5sta;    /*指示LED状态*/

typedef enum {LED_INIT, LED_HOTING, LED_SDCHANGR, LED_IDLEPRE, LED_IDLE} led_mode_t;
led_mode_t led_mode;

static u8   flowdir = 0;                               /*流水灯方向*/
static u16  IdleCount = 0;                             /*空闲延时计数*/
static u8   FlowCount = LEDG_START_NUM;                /*流水LED计数*/

static void StateCheack();   /*输入状态检测*/
static void UpdateLeds();    /*加热中，LED指示状态*/
static void SDMountHandle();
static void SDUninstallHandle();
static void HasSDHandle();
static void NOSDHandle();

/*主逻辑*/
void MainTaskLoop()
{
  StateCheack();                                                               /*输入状态检测*/
  if(isChangeSD)      { led_mode = LED_SDCHANGR; IdleCount = 0; }   /*检测到U盘状态变化，展示效果*/ 
  else if(isHoting)   /*加热中，LED指示状态*/
  { 
    if(led_mode == LED_IDLE) lastisIdle = true; 
    led_mode = LED_HOTING;   
    IdleCount = 0;
  }
  else                                                                         /*进入空闲流水模式*/
  {           
    IdleCount++;
    if(IdleCount > 2000){                                                       /*进入空闲状态  5秒*/
      led_mode = LED_IDLE;
      IdleCount = 0;
    }
  }
}

/*事件处理*/
void MainTaskHandle()
{
  if(isChangeState)
  {
    if(!isHoting && led_mode == LED_IDLE ) { HotHandle(); isChangeState = false; }
    else if(isHoting && lastisIdle)        { HotHandle(); isChangeState = false; lastisIdle = false;}
  }
  if(isChangeSD)
  {
    if(isHasSD) SDMountHandle();
    else        SDUninstallHandle();
    isChangeSD = false;
    UpdateLeds();
  }
  else if(led_mode == LED_HOTING) 
  {
    UpdateLeds();
  }
  else if(led_mode == LED_IDLE)
  {
    if(isHasSD) HasSDHandle();
    else        NOSDHandle();
  }
}

static void StateCheack()   /*输入状态检测*/
{
  if(~LEDACT_PIN != isHasSD){     /*U盘插入处理*/
    isChangeSD = true;
    if (LEDACT_PIN)    { isHasSD = false; led5sta = 0; led1sta = 0;}  
    else               { isHasSD = true;  led5sta = 1; led1sta = 0;}       
  }

  if(!HEAT1_PIN)  led2sta = 1; else led2sta = 0;    /*加热中，对应加热灯点亮*/
  if(!HEAT2_PIN)  led3sta = 1; else led3sta = 0;
  if(!HOTBED_PIN) led4sta = 1; else led4sta = 0;

  if(led2sta || led3sta || led4sta) { if(!isHoting) { isHoting = true;  isChangeState = true;}}
  else                              { if(isHoting)  { isHoting = false; isChangeState = true;}}
}

static void UpdateLeds()  /*加热中，LED指示状态*/
{
  u8 nowledsta = 0;
  nowledsta |= (led1sta << 0);
  nowledsta |= (led2sta << 1);
  nowledsta |= (led3sta << 2);
  nowledsta |= (led4sta << 3);
  nowledsta |= (led5sta << 4);
  SetAllLed(nowledsta);
}

static void SDMountHandle()  /*U盘安装*/
{
  BuzzerPlay(100);
  SDUninstallLedMode();
  BuzzerPlay(200);
}

static void SDUninstallHandle() /*U盘卸载*/
{
  BuzzerPlay(200);
  SDMountLedMode();
  BuzzerPlay(100);
}

static void HasSDHandle()
{
  if(FlowCount >= 6) flowdir=0;
  else if(FlowCount <= 4) flowdir=1;
  SetAllLed(3 << FlowCount);
  delayms(800);
  if(flowdir) FlowCount++;
  else        FlowCount--;
}

static void NOSDHandle()
{
  if(FlowCount >= 7) flowdir=0;
  else if(FlowCount <= 4) flowdir=1;
  SetOneLight(FlowCount);
  delayms(800);
  if(flowdir) FlowCount++;
  else        FlowCount--;
}
