#include	"Task.h"
#include	"Led.h"
#include	"GPIO.h"
#include	"config.h"
#include "Buzzer.h"
#include "delay.h"

SYS_MODE systemMode;
static u16 IdleCount = 0;      /*延时计数*/
static u16 Count;
static u8 FlowCount = 4;
static u8 isHasSD = false;
static u8 isChangeSD = false;
static u8 flowdir = 0;
static u8 isIdleState = false;
static u8 led1sta,led2sta,led3sta,led4sta,led5sta;

/*主逻辑*/
void MainTaskLoop()
{
  if(~LEDACT_PIN != isHasSD){     /*U盘插入处理*/
    isChangeSD = true;
    if (LEDACT_PIN)    { isHasSD = false; }
    else               { isHasSD = true; }           
  }

  if(systemMode == SYSBUSY && !isChangeSD){    /*加热模式*/
    u8 nowledsta = 0;
    nowledsta |= (led1sta << 0);
    nowledsta |= (led2sta << 1);
    nowledsta |= (led3sta << 2);
    nowledsta |= (led4sta << 3);
    nowledsta |= (led5sta << 4);
    SetAllLed(nowledsta);
  }
  else if(!isIdleState){
    IdleCount++;
    if(IdleCount > 30){ /*进入空闲状态  5秒*/   
      isIdleState = true;
      if(isHasSD)  systemMode = AUTOHASSD; /*有U盘*/
      else            systemMode = AUTONOSD;  /*无U盘*/
      IdleCount = 0;
    }
  }
}

/*自动模式流水灯循环*/
void SysAutoLoop()
{
  if(systemMode == AUTOHASSD)
  {
    Count++;
    if(Count > 35)
    {
      if(FlowCount==6) flowdir=0;
      else if(FlowCount==4) flowdir=1;
      SetAllLed(3 << FlowCount);
      if(flowdir) FlowCount++;
      else        FlowCount--;
      
      Count = 0;
    }
  }
  else if(systemMode == AUTONOSD)
  {
    Count++;
    if(Count > 35)
    {
      if(FlowCount==7) flowdir=0;
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
  if(HEAT1_PIN){ led2sta = 1;  systemMode = SYSBUSY; }       /*加热中，对应加热灯点亮*/
  else           led2sta = 0;
   
  if(HEAT2_PIN){ led3sta = 1; systemMode = SYSBUSY; }
  else           led3sta = 0;

  if(HOTBED_PIN){ led4sta = 1; systemMode = SYSBUSY; }
  else           led4sta = 0;
  
  if(systemMode == SYSBUSY){    /*U盘指示灯*/
    IdleCount = 0; FlowCount = 0;
    if(isHasSD)      { led5sta = 1; led1sta = 0;}
    else             { led5sta = 0; led1sta = 0;}

    if(isIdleState) HotStart();		/*开始*/
  }
  else
  {
    if(isIdleState) HotStart();		/*开始*/
    isIdleState = false;
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