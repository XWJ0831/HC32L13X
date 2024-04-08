#include "stdint.h"
#include "gpio.h"
#include "LED.h"
#define		FLAG_KEY_ON			0x00		// ON/OFF Key
#define		FLAG_KEY_UP			0x01		// Up Key Value
#define 	FLAG_KEY_DOWN		0x02		// DOWN Key Value
#define 	FLAG_KEY_ZH			0x03		// 组合DOWN Key Value
#define 	FLAG_KEY_NO			0x04		// NO Key Value
#define	 	key_up			GpioPin10
#define	 	key_on			GpioPin11
#define	 	key_down		GpioPin12

uint8_t  KEY_tmCode;
uint8_t  KEY_Code;
uint8_t  tmkey_zh;
uint8_t  tmkey_on;
uint8_t  tmkey_up;
uint8_t  tmkey_down;
uint8_t Flag_system;
uint8_t Flag_key_send;
uint16_t LED_DisplayMode;
uint16_t LED_SetData;
uint8_t Flag_w_an;
extern uint16_t SYS_KeyNumber;
extern uint8_t  SYS_KeyFlag;
extern uint8_t Flag_w;
extern uint8_t Flag_w_off;
extern uint8_t UI_val;
extern uint8_t Flag_w;
extern uint16_t heat_set;
extern	uint16_t LED_ReturnNum;
//#################################################################//
//按键扫描处理程序//
//#################################################################//
// 按键初始化
void KEY_InitVar(void)
{
	stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    ///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY1初始化
    Gpio_Init(GpioPortB, GpioPin10, &stcGpioCfg); 
	
	///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY2初始化
    Gpio_Init(GpioPortA, GpioPin11, &stcGpioCfg); 
	
	///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->无
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< 端口输入/输出值寄存器总线控制模式配置->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY3初始化
    Gpio_Init(GpioPortA, GpioPin12, &stcGpioCfg); 
	
	KEY_tmCode = FLAG_KEY_NO;
	KEY_Code = FLAG_KEY_NO;
	SYS_KeyFlag = 0;
	tmkey_zh = 1;		
	tmkey_on = 1;
	tmkey_up = 1;
	tmkey_down = 1;
}

//按键扫描程序
void KEY_Scan(void)
{
	

	if ((tmkey_on == 1) && (key_on == 0))		// 开关键按下
	{
		KEY_tmCode = FLAG_KEY_ON;//表示检测到了开关键按下的事件
		SYS_KeyNumber = 0;//重新开始按键消抖延时的计数
		SYS_KeyFlag = 1;//表示发生了按键事件
		tmkey_on = key_on;//将 tmkey_on 更新为当前的 key_on 值，以备下次检测使用
		return;
	}
	else
	{
		tmkey_on = key_on;
	}
	

	if ((tmkey_up == 1)&&(key_up == 0)&& (key_down != 0))	 		// 上升键按下
	{
		KEY_tmCode = FLAG_KEY_UP;
		SYS_KeyNumber = 0;
		SYS_KeyFlag = 1;
		tmkey_up = key_up;
		return;
	}
	else
	{
		tmkey_up = key_up;
	}

	if ((tmkey_down == 1) && (key_down == 0) && (key_up != 0))		// 下降键按下
	{
		KEY_tmCode = FLAG_KEY_DOWN;
		SYS_KeyNumber = 0;
		SYS_KeyFlag = 1;
		tmkey_down = key_down;
		return;
	}
	else
	{
		tmkey_down = key_down;
	}
	

	KEY_tmCode = FLAG_KEY_NO;
	SYS_KeyFlag = 0;
	KEY_Code = FLAG_KEY_NO;//清除之前的按键状态，以便系统可以重新检测和处理新的按键事件
}

// 按键键位确认程序
void KEY_OK(void)
{


	if (key_on == 0)					// on/off键按下
	{
		if(KEY_tmCode == FLAG_KEY_ON)	// 连续两次按键相同（确认）
		{
		   KEY_Code = FLAG_KEY_ON;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// 连两次按键不同（放弃
		{
		   KEY_tmCode = FLAG_KEY_NO;
		   KEY_Code = FLAG_KEY_NO;
		   SYS_KeyFlag = 0;
		}
		return;
	}

	if (key_up == 0)					// 向上键按下
	{
		if(KEY_tmCode == FLAG_KEY_UP)	// 连续两次按键相同（确认）
		{
		   KEY_Code = FLAG_KEY_UP;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// 连续两次按键不同（放弃）
		{
		   KEY_tmCode = FLAG_KEY_NO;
		   KEY_Code = FLAG_KEY_NO;
		   SYS_KeyFlag = 0;
		}
		return;
	}
	
	if (key_down ==	0)					// 向下键按下
	{
		if(KEY_tmCode == FLAG_KEY_DOWN)	// 连续两次按键相同（确认）
		{
		   KEY_Code = FLAG_KEY_DOWN;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// 连续两次按键不同（放弃）
		{
		   KEY_tmCode = FLAG_KEY_NO;
		   KEY_Code = FLAG_KEY_NO;
		   SYS_KeyFlag = 0;
		}
		return;
	}

	

	KEY_tmCode = FLAG_KEY_NO;
	KEY_Code = FLAG_KEY_NO;
	SYS_KeyFlag = 0;
}


//上升键处理程序
void KEY_UpProcess(void)				//错误模式时按上升键无效
{
	Flag_key_send=1;
	if(LED_DisplayMode == RUN_MODE)		//运行模式时按上升键进入设置模式
	{	
		LED_DisplayMode = SET_MODE;
		LED_SetData = heat_set;
	}
	else if(LED_DisplayMode == SET_MODE)		//设置模式时按上升键增加设置值
	{	
	//		LED_SetData=heat_set;
		if(LED_SetData<44)
			LED_SetData++;

		heat_set = LED_SetData;
		Flag_w = 1;//写入操作置1，正在写入
		Flag_w_an=0;
		//Write_process();
	}
	else if(LED_DisplayMode == VAL_MODE)
	{
		UI_val=1;
	}

	LED_Change(); 
}

// 下降键处理程序
void KEY_DownProcess(void)				//错误模式时按下降键无效
{	
	Flag_key_send=1;
	if(LED_DisplayMode == RUN_MODE)		//运行模式时按下降键进入设置模式
	{	
		LED_DisplayMode = SET_MODE;
		LED_SetData = heat_set;
	}
	else if(LED_DisplayMode == SET_MODE)		//设置模式时按下降键减少设置值
	{
		if(LED_SetData>35)
			LED_SetData--;

		heat_set = LED_SetData;
		Flag_w=1;
		Flag_w_an=0;
		//Write_process();
	}
	else if(LED_DisplayMode == VAL_MODE)
	{
		UI_val=0;
	}

	 LED_Change();

}

// ON/OFF键处理程序
void KEY_OnProcess(void)
{
		Flag_key_send=1;
		if(Flag_system==1)		//系统休眠状态与正常状态切换
		{	
		

		//	heat_set=heat_temp;
			Flag_w_off = 1;//写入结束标志位置1，写入未完成
			Flag_w_an=0;//未收到应答
		//	Write_process();
		//	Flag_system=0;
		//	LED_CS3=0;

		}
		else
		{
			
		//	heat_set=heat_temp;

		//	Write_process();
			Flag_w = 1;//写入操作置1，正在写入
		//	Write_process();
			Flag_w_an=0;//未收到应答
		//	Flag_system=1;
		//	LED_CS3=1;

		}
}
	// 键盘处理程序
void KEY_Process(void)
{
	if(Flag_system==1)		//系统处于运行状态时所有按键正常处理。
	{
		switch(KEY_Code)
		{

			case FLAG_KEY_ON:			//调用开关键处理程序
				KEY_OnProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
			case FLAG_KEY_UP:			//调用上升键处理程序
				KEY_UpProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
			case FLAG_KEY_DOWN:			//调用下降键处理程序
				KEY_DownProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
				
	
		}
	}
	else				//系统处于休眠状态时除了开关键，其它按键均失效，不处理。
	{
		if(KEY_Code==FLAG_KEY_ON)
		{
			KEY_OnProcess();
			KEY_Code = FLAG_KEY_NO;
			LED_ReturnNum = 0;
		}
	}
}


