#include "stdint.h"
#include "gpio.h"

#define		RUN_MODE		0x00								// Running Mode
#define		ERR_MODE		0x01								// Setting Mode
#define		SET_MODE		0x02								// Program Mode
#define		ALARM_MODE		0x03								// alarm   Mode
#define		VAL_MODE		0x04								// value   Mode	
uint8_t  LED_Index;
uint16_t LED_Value[2];
uint16_t LED_DisplayMode;
uint8_t LED_FlickerFlag;
uint16_t LED_SetData;
uint16_t LED_ReturnNum;
extern uint8_t Flag_erro;
extern uint8_t Flag_system;
extern uint16_t output_wendu1;
extern uint8_t val_display;

const unsigned int LED_Tab[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	
// LED显示初始化
void LED_Init(void)
{
	LED_Index = 0;
	LED_Value[0] = 0;			// Clear LED
	LED_Value[1] = 0;			// Clear LED
	LED_DisplayMode = RUN_MODE;	// Working Mode
//	Adjust_menu = 0;
	LED_FlickerFlag = 0;		// Flicker Flag
	LED_SetData = 0;
	LED_ReturnNum = 0;
	
	stc_gpio_cfg_t stcGpioCfg;
    
    ///< 打开GPIO外设时钟门控
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED1关闭
    Gpio_ClrIO(GpioPortB, GpioPin13);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortB, GpioPin13, &stcGpioCfg);
   
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED2关闭
    Gpio_ClrIO(GpioPortB, GpioPin14);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortB, GpioPin14, &stcGpioCfg);	
    
    ///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED3关闭
    Gpio_ClrIO(GpioPortB, GpioPin15);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDa关闭
    Gpio_ClrIO(GpioPortA, GpioPin0);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDb关闭
    Gpio_ClrIO(GpioPortA, GpioPin1);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDc关闭
    Gpio_ClrIO(GpioPortA, GpioPin2);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDd关闭
    Gpio_ClrIO(GpioPortA, GpioPin3);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDe关闭
    Gpio_ClrIO(GpioPortA, GpioPin4);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin4, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDf关闭
    Gpio_ClrIO(GpioPortA, GpioPin5);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin5, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDg关闭
    Gpio_ClrIO(GpioPortA, GpioPin6);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin6, &stcGpioCfg);
	
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口上下拉配置->下拉
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDdp关闭
    Gpio_ClrIO(GpioPortA, GpioPin7);
    
    ///< GPIO IO LED端口初始化
    Gpio_Init(GpioPortA, GpioPin7, &stcGpioCfg);
}

// 正常数据显示程序
//************************************************************************************************************************//
//对显示数据val进行位拆分,分别拆成十、个位两个不同的数据,并存入指针LED_Value对应的二个位置//
//程序入口为val寄存器,出口为指针LED_Value//
//************************************************************************************************************************//
void LED_ValDisplay(int val)
{
	int	tmp, tmp1;

	// 判断数据是否越限
	if (val > 99) tmp = 99;
	else if (val < 0) tmp = 0;
	else tmp = val;
	
// 十位数代码（10）
	tmp1 = tmp / 10;
	if(tmp1 == 0)
		LED_Value[1] = 0;
	else
	LED_Value[1] = LED_Tab[tmp1%10];
// 个位数代码（1）
	LED_Value[0] = LED_Tab[tmp%10];
}


//500ms定时数据亮灭显示程序
//************************************************************************************************************************//
//对显示数据val进行位拆分,分别拆成十位和个位两个不同的数据,并存入指针LED_Value对应的两个位置中,百位则默认为0//
//程序入口为val寄存器,出口为指针LED_Value//
//************************************************************************************************************************//
void LED_DataDisplay(int val)
{
	int	tmp2;

	//Program Mode
	tmp2 = val % 100;
	if(LED_FlickerFlag == 1)
	{	
		LED_Value[0] = 0;
		LED_Value[1] = 0;
//		LED_Value[2] = 0;
	}
	else
	{	
//		LED_Value[2] = 0;
		LED_Value[1] = LED_Tab[tmp2 / 10];
		LED_Value[0] = LED_Tab[val % 10];
	}

}


//数码管片选端口选通程序
void LED_Select(int i)
{
 	switch(i)
	{
		case 1:
			Gpio_ClrIO(GpioPortB, GpioPin14);//LED2关闭
			Gpio_SetIO(GpioPortB,GpioPin13);//LED1打开
			break;
		case 0:
			Gpio_ClrIO(GpioPortB, GpioPin13);//LED1关闭
			Gpio_SetIO(GpioPortB,GpioPin14);//LED2打开
			break;
		case 2:
			Gpio_ClrIO(GpioPortB, GpioPin13);//LED1关闭
			Gpio_ClrIO(GpioPortB, GpioPin14);//LED2关闭
			break;

	}
}

//5ms数码管位更新显示程序
void LED_Update(void)
{
	PORTD = 0;//清空 PORTD 寄存器的内容，将所有的 LED 灯熄灭
	// Select LED
	LED_Select(LED_Index);	
	// Transmit Display Data
	PORTD = LED_Value[LED_Index];		//数码管显示赋值
	LED_Index = (LED_Index + 1) % 2;	//片选端口循环
}

//1s数据更新程序
void LED_Change(void)
{
	if(Flag_system==1)
	{
		switch(LED_DisplayMode)
		{
			case RUN_MODE:			//运行模式
				LED_ValDisplay(output_wendu1);
				break;
			case ERR_MODE:			//错误模式

				LED_ValDisplay(Flag_erro);
				LED_Value[1]=0x79;
				break;
			case SET_MODE:			//设置模式	
				LED_ValDisplay(LED_SetData);
		//		LED_Value[1] = 0x73;
				break;
			case ALARM_MODE:			//报警模式
				LED_ValDisplay(Flag_erro);
				LED_Value[1] = 0x77;
				break;
			case VAL_MODE:			//参数显示模式
				LED_ValDisplay(val_display);
			//	LED_Value[1] = 0x77;
				break;
		}
	}
	else			//系统休眠状态时不显示任何东西
	{

		LED_Value[0] = 0;
		LED_Value[1] = 0;
	}
}
