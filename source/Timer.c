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
 * TIM0�жϷ�����
 ******************************************************************************/
void Tim0_IRQHandler(void)
{
    static uint8_t i;
    
    //Timer0 ģʽ0 ����ж�
    if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
    {
        Bt_Mode0_DisableIrq(TIM0);
		Bt_ClearIntFlag(TIM0,BtUevIrq); //�жϱ�־����
		Bt_M0_ARRSet(TIM0, 250);
		counter_5ms++;
		if(counter_5ms>=5)					//5ms��ʱ
		{									//��������˸������
			counter_5ms=0;
			LED_Update();					//�����λ����
			if(SYS_KeyFlag == 0) //ϵͳ���ڷǰ�������״̬
			KEY_Scan();						//����ɨ��
		}
         // Key Process Delay Count
		if((SYS_KeyFlag == 1)&&(Flag_100ms == 0))//(SYS_KeyFlag == 1) ��ʾ��⵽�˰����¼�,(Flag_100ms == 0) ��ʾ��ǰ100ms�İ���������ʱ��δ��ɣ�ȷ����100ms�ڲ����ظ��������¼�
		{
			SYS_KeyNumber ++;
			if(SYS_KeyNumber >= 70)			//100ms����������ʱ
			{
				SYS_KeyNumber = 0;
				Flag_100ms = 1;//��ʾ100ms�İ���������ʱ�Ѿ����
            }
		}
		if(Flag_w_busy == 1) //ͨ��æµ
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
				Flag_500ms=1;					//500ms��������ݸ��¼�ʱ
			//	Flag_answer=1;
				counter_2s++;
			if(counter_2s>=2)	
				Flag_read_2s=1;					//2s ͨѶ������
			}
		}

		Flag_answer++;							//ÿ��1ms��1
		
		if (LED_DisplayMode == SET_MODE)
		{
			LED_ReturnNum++;

			if(LED_ReturnNum>=2000)	   			// 1.5s��ʱ,����״̬ʱ��1.5s�ް�������,���Զ������������ݲ���������״̬
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

//ʱ�����ó�ʼ��
void App_ClockCfg(void)
{
    en_flash_waitcycle_t      enWaitCycle;
    stc_sysctrl_pll_cfg_t     stcPLLCfg;
    
    //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcPLLCfg);
    
    enWaitCycle = FlashWaitCycle1;
    Flash_WaitCycle(enWaitCycle);
    
    stcPLLCfg.enInFreq    = SysctrlPllInFreq4_6MHz;     //RCH 4MHz
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq8_12MHz;  //PLL ���48MHz
    stcPLLCfg.enPllClkSrc = SysctrlPllRch;              //����ʱ��Դѡ��RCH
    stcPLLCfg.enPllMul    = SysctrlPllMul2;            //4MHz x 2 = 8MHz
    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
    
    Sysctrl_SysClkSwitch(SysctrlClkPLL);  ///< ʱ���л�
}

//�˿����ó�ʼ��
void App_Timer0PortCfg(void)
{
    stc_gpio_cfg_t         stcLEDPortCfg;
    
    DDL_ZERO_STRUCT(stcLEDPortCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO ����ʱ��ʹ��
    
    Gpio_WriteOutputIO(STK_LED_PORT, STK_LED_PIN, FALSE);
    stcLEDPortCfg.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcLEDPortCfg);   //PD14����ΪLED ��������
}

//Timer0���ó�ʼ��
void App_Timer0Cfg(uint16_t u16Period)
{
    uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
    
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE); //Base Timer����ʱ��ʹ��
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //��ʱ��ģʽ
    stcBtBaseCfg.enCT       = BtTimer;                      //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv256;                 //PCLK/256
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //�Զ�����16λ������/��ʱ��
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                     //TIM0 ��ģʽ0���ܳ�ʼ��
    
    u16ArrValue = 0x10000 - u16Period;
    Bt_M0_ARRSet(TIM0, u16ArrValue);                        //��������ֵ(ARR = 0x10000 - ����)
    
    u16CntValue = 0x10000 - u16Period;
    Bt_M0_Cnt16Set(TIM0, u16CntValue);                      //���ü�����ֵ
    
    Bt_ClearIntFlag(TIM0,BtUevIrq);                         //���жϱ�־   
    Bt_Mode0_EnableIrq(TIM0);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);                 //TIM0�ж�ʹ��
}