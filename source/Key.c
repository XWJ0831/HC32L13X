#include "stdint.h"
#include "gpio.h"
#include "LED.h"
#define		FLAG_KEY_ON			0x00		// ON/OFF Key
#define		FLAG_KEY_UP			0x01		// Up Key Value
#define 	FLAG_KEY_DOWN		0x02		// DOWN Key Value
#define 	FLAG_KEY_ZH			0x03		// ���DOWN Key Value
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
//����ɨ�账�����//
//#################################################################//
// ������ʼ��
void KEY_InitVar(void)
{
	stc_gpio_cfg_t stcGpioCfg;
    
    ///< ��GPIO����ʱ���ſ�
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
    
    ///< �˿ڷ�������->����
    stcGpioCfg.enDir = GpioDirIn;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvL;
    ///< �˿�����������->��
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY1��ʼ��
    Gpio_Init(GpioPortB, GpioPin10, &stcGpioCfg); 
	
	///< �˿ڷ�������->����
    stcGpioCfg.enDir = GpioDirIn;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvL;
    ///< �˿�����������->��
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY2��ʼ��
    Gpio_Init(GpioPortA, GpioPin11, &stcGpioCfg); 
	
	///< �˿ڷ�������->����
    stcGpioCfg.enDir = GpioDirIn;
    ///< �˿�������������->����������
    stcGpioCfg.enDrv = GpioDrvL;
    ///< �˿�����������->��
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
    ///< �˿ڿ�©�������->��©����ر�
    stcGpioCfg.enOD = GpioOdDisable;
    ///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
    stcGpioCfg.enCtrlMode = GpioAHB;
    ///< GPIO IO USER KEY3��ʼ��
    Gpio_Init(GpioPortA, GpioPin12, &stcGpioCfg); 
	
	KEY_tmCode = FLAG_KEY_NO;
	KEY_Code = FLAG_KEY_NO;
	SYS_KeyFlag = 0;
	tmkey_zh = 1;		
	tmkey_on = 1;
	tmkey_up = 1;
	tmkey_down = 1;
}

//����ɨ�����
void KEY_Scan(void)
{
	

	if ((tmkey_on == 1) && (key_on == 0))		// ���ؼ�����
	{
		KEY_tmCode = FLAG_KEY_ON;//��ʾ��⵽�˿��ؼ����µ��¼�
		SYS_KeyNumber = 0;//���¿�ʼ����������ʱ�ļ���
		SYS_KeyFlag = 1;//��ʾ�����˰����¼�
		tmkey_on = key_on;//�� tmkey_on ����Ϊ��ǰ�� key_on ֵ���Ա��´μ��ʹ��
		return;
	}
	else
	{
		tmkey_on = key_on;
	}
	

	if ((tmkey_up == 1)&&(key_up == 0)&& (key_down != 0))	 		// ����������
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

	if ((tmkey_down == 1) && (key_down == 0) && (key_up != 0))		// �½�������
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
	KEY_Code = FLAG_KEY_NO;//���֮ǰ�İ���״̬���Ա�ϵͳ�������¼��ʹ����µİ����¼�
}

// ������λȷ�ϳ���
void KEY_OK(void)
{


	if (key_on == 0)					// on/off������
	{
		if(KEY_tmCode == FLAG_KEY_ON)	// �������ΰ�����ͬ��ȷ�ϣ�
		{
		   KEY_Code = FLAG_KEY_ON;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// �����ΰ�����ͬ������
		{
		   KEY_tmCode = FLAG_KEY_NO;
		   KEY_Code = FLAG_KEY_NO;
		   SYS_KeyFlag = 0;
		}
		return;
	}

	if (key_up == 0)					// ���ϼ�����
	{
		if(KEY_tmCode == FLAG_KEY_UP)	// �������ΰ�����ͬ��ȷ�ϣ�
		{
		   KEY_Code = FLAG_KEY_UP;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// �������ΰ�����ͬ��������
		{
		   KEY_tmCode = FLAG_KEY_NO;
		   KEY_Code = FLAG_KEY_NO;
		   SYS_KeyFlag = 0;
		}
		return;
	}
	
	if (key_down ==	0)					// ���¼�����
	{
		if(KEY_tmCode == FLAG_KEY_DOWN)	// �������ΰ�����ͬ��ȷ�ϣ�
		{
		   KEY_Code = FLAG_KEY_DOWN;
		   KEY_tmCode = FLAG_KEY_NO;
		}
		else						// �������ΰ�����ͬ��������
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


//�������������
void KEY_UpProcess(void)				//����ģʽʱ����������Ч
{
	Flag_key_send=1;
	if(LED_DisplayMode == RUN_MODE)		//����ģʽʱ����������������ģʽ
	{	
		LED_DisplayMode = SET_MODE;
		LED_SetData = heat_set;
	}
	else if(LED_DisplayMode == SET_MODE)		//����ģʽʱ����������������ֵ
	{	
	//		LED_SetData=heat_set;
		if(LED_SetData<44)
			LED_SetData++;

		heat_set = LED_SetData;
		Flag_w = 1;//д�������1������д��
		Flag_w_an=0;
		//Write_process();
	}
	else if(LED_DisplayMode == VAL_MODE)
	{
		UI_val=1;
	}

	LED_Change(); 
}

// �½����������
void KEY_DownProcess(void)				//����ģʽʱ���½�����Ч
{	
	Flag_key_send=1;
	if(LED_DisplayMode == RUN_MODE)		//����ģʽʱ���½�����������ģʽ
	{	
		LED_DisplayMode = SET_MODE;
		LED_SetData = heat_set;
	}
	else if(LED_DisplayMode == SET_MODE)		//����ģʽʱ���½�����������ֵ
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

// ON/OFF���������
void KEY_OnProcess(void)
{
		Flag_key_send=1;
		if(Flag_system==1)		//ϵͳ����״̬������״̬�л�
		{	
		

		//	heat_set=heat_temp;
			Flag_w_off = 1;//д�������־λ��1��д��δ���
			Flag_w_an=0;//δ�յ�Ӧ��
		//	Write_process();
		//	Flag_system=0;
		//	LED_CS3=0;

		}
		else
		{
			
		//	heat_set=heat_temp;

		//	Write_process();
			Flag_w = 1;//д�������1������д��
		//	Write_process();
			Flag_w_an=0;//δ�յ�Ӧ��
		//	Flag_system=1;
		//	LED_CS3=1;

		}
}
	// ���̴������
void KEY_Process(void)
{
	if(Flag_system==1)		//ϵͳ��������״̬ʱ���а�����������
	{
		switch(KEY_Code)
		{

			case FLAG_KEY_ON:			//���ÿ��ؼ��������
				KEY_OnProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
			case FLAG_KEY_UP:			//�����������������
				KEY_UpProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
			case FLAG_KEY_DOWN:			//�����½����������
				KEY_DownProcess();
				KEY_Code = FLAG_KEY_NO;
				LED_ReturnNum = 0;
				break;
				
	
		}
	}
	else				//ϵͳ��������״̬ʱ���˿��ؼ�������������ʧЧ��������
	{
		if(KEY_Code==FLAG_KEY_ON)
		{
			KEY_OnProcess();
			KEY_Code = FLAG_KEY_NO;
			LED_ReturnNum = 0;
		}
	}
}


