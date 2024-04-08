#include "ddl.h"
#include "bt.h"
#include "gpio.h"
#include "flash.h"
#include "LED.h"
#include "Key.h"

extern uint8_t  SYS_KeyFlag;
extern uint16_t SYS_KeyNumber;
extern uint8_t Flag_w_busy;
extern uint8_t Flag_500ms;
extern uint16_t counter_500ms;
extern uint16_t counter_2s;
extern uint8_t Flag_read_2s;
extern uint16_t Flag_answer;
extern uint16_t LED_DisplayMode;
extern uint16_t LED_ReturnNum;
extern uint8_t Flag_Return;
uint16_t counter_5ms;
uint8_t Flag_100ms;
uint8_t TT_COUNT;
/*******************************************************************************
 * TIM0中断服务函数
 ******************************************************************************/
void Tim0_IRQHandler(void)
{
    static uint8_t i;
    
    //Timer0 模式0 溢出中断
    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
    {
        Bt_Mode0_DisableIrq(TIM0);
		Bt_ClearIntFlag(TIM0,BtUevIrq); //中断标志清零
		Bt_M0_ARRSet(TIM0, 250);
		counter_5ms++;
		if(counter_5ms>=5)					//5ms计时
		{									//工作灯闪烁计数器
			counter_5ms=0;
			LED_Update();					//数码管位更新
			if(SYS_KeyFlag == 0) //系统处于非按键处理状态
			KEY_Scan();						//按键扫描
		}
         // Key Process Delay Count
		if((SYS_KeyFlag == 1)&&(Flag_100ms == 0))//(SYS_KeyFlag == 1) 表示检测到了按键事件,(Flag_100ms == 0) 表示当前100ms的按键消抖延时尚未完成，确保在100ms内不会重复处理按键事件
		{
			SYS_KeyNumber ++;
			if(SYS_KeyNumber >= 70)			//100ms按键消抖延时
			{
				SYS_KeyNumber = 0;
				Flag_100ms = 1;//表示100ms的按键消抖延时已经完成
            }
		}
		if(Flag_w_busy == 1) //通信忙碌
		{
			TT_COUNT++;

			if(TT_COUNT >= 250)
			{
			TT_COUNT=0;
			Flag_w_busy = 0;
			
			}
		}
		if(Flag_500ms==0)
		{	counter_500ms++;
			if(counter_500ms >= 400)
			{
				Flag_500ms=1;					//500ms数码管数据更新计时
			//	Flag_answer=1;
				counter_2s++;
			if(counter_2s>=2)	
				Flag_read_2s=1;					//2s 通讯读数据
			}
		}

		Flag_answer++;							//每过1ms加1
		
		if (LED_DisplayMode == SET_MODE)
		{
			LED_ReturnNum++;

			if(LED_ReturnNum>=2000)	   			// 1.5s计时,设置状态时若1.5s无按键操作,则自动保存设置数据并返回运行状态
			{
				Flag_Return=1;
				
			}
			else
				Flag_Return=0;
		}
		
//		INT_TMR0();
		Bt_Mode0_EnableIrq(TIM0);
    }
}

//时钟配置初始化
void App_ClockCfg(void)
{
    en_flash_waitcycle_t      enWaitCycle;
    stc_sysctrl_pll_cfg_t     stcPLLCfg;
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcPLLCfg);
    
    enWaitCycle = FlashWaitCycle1;
    Flash_WaitCycle(enWaitCycle);
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq8_12MHz;  //PLL 输出48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //输入时钟源选择RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul2;            //4MHz x 2 = 8MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
    
    Sysctrl_SysClkSwitch(SysctrlClkPLL);  ///< 时钟切换
}

//端口配置初始化
void App_Timer0PortCfg(void)
{
    stc_gpio_cfg_t         stcLEDPortCfg;
    
    DDL_ZERO_STRUCT(stcLEDPortCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO 外设时钟使能
    
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);
    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcLEDPortCfg);   //PD14设置为LED 控制引脚
}

//Timer0配置初始化
void App_Timer0Cfg(uint16_t u16Period)
{
    uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
    
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE); //Base Timer外设时钟使能
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //定时器模式
    stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 //PCLK/256
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //自动重载16位计数器/定时器
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化
    
    u16ArrValue = 0x10000 - u16Period;
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //设置重载值(ARR = 0x10000 - 周期)
    
    u16CntValue = 0x10000 - u16Period;
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //设置计数初值
    
    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //清中断标志   
    Bt_Mode0_EnableIrq(TIM0);                               //使能TIM0中断(模式0时只有一个中断)
    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0中断使能
}