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
// LED��ʾ��ʼ��
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
    
    ///< ��GPIO����ʱ���ſ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
    
    ///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED1�ر�
    Gpio_ClrIO(GpioPortB, GpioPin13);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortB, GpioPin13, &stcGpioCfg);
   
    ///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED2�ر�
    Gpio_ClrIO(GpioPortB, GpioPin14);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortB, GpioPin14, &stcGpioCfg);	
    
    ///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LED3�ر�
    Gpio_ClrIO(GpioPortB, GpioPin15);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDa�ر�
    Gpio_ClrIO(GpioPortA, GpioPin0);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDb�ر�
    Gpio_ClrIO(GpioPortA, GpioPin1);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin1, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDc�ر�
    Gpio_ClrIO(GpioPortA, GpioPin2);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin2, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDd�ر�
    Gpio_ClrIO(GpioPortA, GpioPin3);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDe�ر�
    Gpio_ClrIO(GpioPortA, GpioPin4);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin4, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDf�ر�
    Gpio_ClrIO(GpioPortA, GpioPin5);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin5, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDg�ر�
    Gpio_ClrIO(GpioPortA, GpioPin6);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin6, &stcGpioCfg);
	
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
    stcGpioCfg.enDir = GpioDirOut;
    ///< �˿�����������->����
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdEnable;
    
    ///< LEDdp�ر�
    Gpio_ClrIO(GpioPortA, GpioPin7);
    
    ///< GPIO IO LED�˿ڳ�ʼ��
    Gpio_Init(GpioPortA, GpioPin7, &stcGpioCfg);
}

// ����������ʾ����
//************************************************************************************************************************//
//����ʾ����val����λ���,�ֱ���ʮ����λ������ͬ������,������ָ��LED_Value��Ӧ�Ķ���λ��//
//�������Ϊval�Ĵ���,����Ϊָ��LED_Value//
//************************************************************************************************************************//
void LED_ValDisplay(int val)
{
	int	tmp, tmp1;

	// �ж������Ƿ�Խ��
	if (val > 99) tmp = 99;
	else if (val < 0) tmp = 0;
	else tmp = val;
	
// ʮλ�����루10��
	tmp1 = tmp / 10;
	if(tmp1 == 0)
		LED_Value[1] = 0;
	else
	LED_Value[1] = LED_Tab[tmp1%10];
// ��λ�����루1��
	LED_Value[0] = LED_Tab[tmp%10];
}


//500ms��ʱ����������ʾ����
//************************************************************************************************************************//
//����ʾ����val����λ���,�ֱ���ʮλ�͸�λ������ͬ������,������ָ��LED_Value��Ӧ������λ����,��λ��Ĭ��Ϊ0//
//�������Ϊval�Ĵ���,����Ϊָ��LED_Value//
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


//�����Ƭѡ�˿�ѡͨ����
void LED_Select(int i)
{
 	switch(i)
	{
		case 1:
			Gpio_ClrIO(GpioPortB, GpioPin14);//LED2�ر�
			Gpio_SetIO(GpioPortB,GpioPin13);//LED1��
			break;
		case 0:
			Gpio_ClrIO(GpioPortB, GpioPin13);//LED1�ر�
			Gpio_SetIO(GpioPortB,GpioPin14);//LED2��
			break;
		case 2:
			Gpio_ClrIO(GpioPortB, GpioPin13);//LED1�ر�
			Gpio_ClrIO(GpioPortB, GpioPin14);//LED2�ر�
			break;

	}
}

//5ms�����λ������ʾ����
void LED_Update(void)
{
	PORTD = 0;//��� PORTD �Ĵ��������ݣ������е� LED ��Ϩ��
	// Select LED
	LED_Select(LED_Index);	
	// Transmit Display Data
	PORTD = LED_Value[LED_Index];		//�������ʾ��ֵ
	LED_Index = (LED_Index + 1) % 2;	//Ƭѡ�˿�ѭ��
}

//1s���ݸ��³���
void LED_Change(void)
{
	if(Flag_system==1)
	{
		switch(LED_DisplayMode)
		{
			case RUN_MODE:			//����ģʽ
				LED_ValDisplay(output_wendu1);
				break;
			case ERR_MODE:			//����ģʽ

				LED_ValDisplay(Flag_erro);
				LED_Value[1]=0x79;
				break;
			case SET_MODE:			//����ģʽ	
				LED_ValDisplay(LED_SetData);
		//		LED_Value[1] = 0x73;
				break;
			case ALARM_MODE:			//����ģʽ
				LED_ValDisplay(Flag_erro);
				LED_Value[1] = 0x77;
				break;
			case VAL_MODE:			//������ʾģʽ
				LED_ValDisplay(val_display);
			//	LED_Value[1] = 0x77;
				break;
		}
	}
	else			//ϵͳ����״̬ʱ����ʾ�κζ���
	{

		LED_Value[0] = 0;
		LED_Value[1] = 0;
	}
}
