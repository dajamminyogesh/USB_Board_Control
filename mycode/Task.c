#include	"Task.h"
#include	"Led.h"
#include	"GPIO.h"
#include	"config.h"
#include "Buzzer.h"
#include "delay.h"

static u8 isHasSD = false;                            /*是否有U盘*/
static u8 isHoting = false;                           /*是否加热*/
static u8 isChangeSD = false;                         /*U盘状态是否改变*/
static u8 isChangeState = false;                      /*状态是否改变*/
static u8 LastState = 0;                              /*上次状态 0未加热 1加热*/
static u8 led1sta,led2sta,led3sta,led4sta,led5sta,led6sta;    /*指示LED状态*/

static u8   LedMode = 0;                               /*流水模式：1有U盘 2无U盘*/   
static u8   flowdir = 0;                               /*流水灯方向*/
static u16  IdleCount = 0;                             /*空闲延时计数*/
static u8   FlowCount = 4;                             /*流水LED计数*/
static u16  Count = 0;                                 /*流水延时计数*/

static void StateCheack();   /*输入状态检测*/
static void UpdateLeds();    /*加热中，LED指示状态*/

/*主逻辑*/
void MainTaskLoop()
{
  StateCheack();                                /*输入状态检测*/
  if(isChangeSD) return;                        /*检测到U盘状态变化，展示效果*/
  if(isHoting)  { UpdateLeds(); LedMode = 0;}   /*加热中，LED指示状态*/
  else                                          /*进入空闲流水模式*/
  {           
    IdleCount++;
    if(IdleCount > 50){                         /*进入空闲状态  5秒*/   
      if(isHasSD)  LedMode = 1;                 /*有U盘*/
      else         LedMode = 2;                 /*无U盘*/
      IdleCount = 0;
    }
  }
}

static void StateCheack()   /*输入状态检测*/
{
  if(~LEDACT_PIN != isHasSD){     /*U盘插入处理*/
    isChangeSD = true;
    if (LEDACT_PIN)    { isHasSD = false; led6sta = 1; led1sta = 0;}  
    else               { isHasSD = true;  led6sta = 0; led1sta = 0;}       
  }

  if(~HEAT1_PIN)  led2sta = 1; else led2sta = 0;    /*加热中，对应加热灯点亮*/
  if(~HEAT2_PIN)  led3sta = 1; else led3sta = 0;
  if(~HOTBED_PIN) led4sta = 1; else led4sta = 0;
  if(~LEDPCT_PIN) led5sta = 1; else led5sta = 0;

  if(led2sta || led3sta || led4sta || led5sta) { if(!LastState) {isHoting = true;  isChangeState = true; LastState = 1;}}
  else                                         { if(LastState)  {isHoting = false; isChangeState = true; LastState = 0;}}
}

static void UpdateLeds()  /*加热中，LED指示状态*/
{
  u8 nowledsta = 0;
  nowledsta |= (led1sta << 0);
  nowledsta |= (led2sta << 1);
  nowledsta |= (led3sta << 2);
  nowledsta |= (led4sta << 3);
  nowledsta |= (led5sta << 4);
  nowledsta |= (led6sta << 5);
  SetAllLed(nowledsta);
}

/*自动模式流水灯循环*/
void SysAutoLoop()
{
  if(LedMode == 1)
  {
    Count++;
    if(Count > 35)
    {
      if(FlowCount==7) flowdir=0;
      else if(FlowCount==4) flowdir=1;
      SetAllLed(3 << FlowCount);
      if(flowdir) FlowCount++;
      else        FlowCount--;
      
      Count = 0;
    }
  }
  else if(LedMode == 2)
  {
    Count++;
    if(Count > 35)
    {
      if(FlowCount==8) flowdir=0;
      else if(FlowCount==4) flowdir=1;
      SetOneLight(FlowCount);
      if(flowdir) FlowCount++;
      else        FlowCount--;
      
      Count = 0;
    }
  }
}

/*U盘插拔检测*/
void USBCheack()
{
  if(isChangeState && (LastState == 1))
  {
    HotStart();
    isChangeState = false;
  }
  if(isChangeState && (LastState == 0) && LedMode > 0)
  {
    HotStart();
    isChangeState = false;
  }
  if(isChangeSD & (!isHasSD))
  {
    BuzzerPlay(100);
    SDMountMode();
    BuzzerPlay(50);
    isChangeSD = false;
  }
  else if(isChangeSD & isHasSD)
  {
    BuzzerPlay(50);
    SDUninstallMode();
    BuzzerPlay(100);
    isChangeSD = false;
  }
}
