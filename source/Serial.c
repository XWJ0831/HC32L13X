#include "stdint.h"
#include "ddl.h"
#include "uart.h"
#include "gpio.h"
#include "sysctrl.h"
#include "LED.h"
#define     Add_ZD			0X01


extern uint8_t RC_buf[16];
extern uint8_t TX_buf[10];
extern uint8_t Flag_erro;
extern uint16_t output_wendu1;
extern uint16_t heat_set;
extern uint8_t Flag_w_busy;
extern uint8_t Flag_system;
extern uint8_t Flag_w_an;
extern uint8_t Flag_w;
extern uint8_t Flag_w_off;
extern uint8_t flag_rc;
extern uint16_t counter_RT;
extern uint8_t UI_val;
extern uint8_t val_display;
extern uint16_t LED_DisplayMode;
uint16_t RxCnt;
uint16_t TxCnt;
uint8_t u8AddrFlg = 0;
uint16_t u16AddrData = 0;
uint16_t erro_value;
uint16_t TX_count;
uint16_t I_value = 20;
uint16_t U_value = 10;
// ͨ�Ŵ���and״̬���ݽṹ
struct ERROR_BITS
{
	unsigned int	ON_OFF    :1;	// ����·����״̬		
	unsigned int	HOT_xq    :1;	// ��Ȧ����	1	
	unsigned int	HOT_water :1;	// ���ȱ�־		
	unsigned int	IGBT_Error:1;	// IGBT����	2	
	unsigned int	U_HIGH    :1;	// ��ѹ	3	
	unsigned int 	U_LOW     :1;	// Ƿѹ4		
	unsigned int	I_HIGH    :1;	// ����	5
	unsigned int	IL        :1;	// ©��6

	unsigned int    Sensor_in :1;	// ��ˮ������7
	unsigned int    Sensor_out:1;	// ��ˮ������8
	unsigned int    tem_switch:1;	// �¶ȿ��ض���9
	unsigned int    KEEP1  	  :5;	// ����
};



union Stauts_Error
{
	unsigned int	   all;
 	struct ERROR_BITS  E_bits;  
};
union Stauts_Error heater_state;
//*****************************************************************//
//*****************************************************************//
//CRC ��λ�ֽ�ֵ��
const unsigned char  CRCH[256] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ;
//CRC��λ�ֽ�ֵ�� 
const unsigned char  CRCL[256] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

//CRC16У�����

unsigned int MYCRC16(unsigned char *ptr,unsigned char Len) 	//��������һ��intֵ 
{
	unsigned char CRC_H = 0xFF ;		//��CRC�ֽڳ�ʼ��
	unsigned char CRC_L = 0xFF ;		//��CRC �ֽڳ�ʼ��
	unsigned char dex;					//CRCѭ���е�����
	while(Len--)						//������Ϣ������ 
	{
		dex=CRC_H ^ *ptr++;				//����CRC
		CRC_H=CRC_L ^ CRCH[dex]; 
		CRC_L=CRCL[dex]; 
	} 
	return ((unsigned int)CRC_H << 8 | CRC_L); 
}

//UART1�жϺ���
void Uart1_IRQHandler(void)
{
    //���ݽ����ж�
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);
        if(0 == u8AddrFlg)
        {
            u8AddrFlg = 1;
            u16AddrData = M0P_UART1->SBUF;                     //���ֽ�Ϊ��ַ�ֽ�
            if(u16AddrData&0x100)
            {
                M0P_UART1->SCON_f.ADRDET = 0;                  //���㣬׼����������֡
            }
        }else
        {
            RC_buf[RxCnt++] = Uart_ReceiveData(M0P_UART1); //�����ֽ�
        }
    }
    //���ݷ����ж�
    if(Uart_GetStatus(M0P_UART1, UartTC))
    {
        Uart_ClrStatus(M0P_UART1, UartTC);
        if(TxCnt>7)                                          //����Ѿ����Ͱ˸������ֽ�
        {	TxCnt = 0;
            RxCnt = 0;
            Uart_DisableIrq(M0P_UART1,UartTxIrq);              //��ֹ�����ж�
            Uart_EnableIrq(M0P_UART1,UartRxIrq);               //ʹ�ܽ����ж�
			flag_rc=1;					//���ܱ�־λ����Ϊ1����ʾ���յ�����Ч������֡
        }else                                                  //�����û�з��Ͱ˸��ֽ�
        {
            Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);     //�������������ֽ�
            TxCnt++;
        }
    }

}

//������������
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //ʹ��GPIOģ��ʱ��

    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf1);          //����PA09 �˿�ΪURART1_TX

    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf1);          //����PA10 �˿�ΪURART1_RX
}

//��������
void App_UartCfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);

    ///< ��������ʱ��
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);///<ʹ��uart1ģ��ʱ��

    ///<UART Init
    stcCfg.enRunMode        = UartMskMode3;          ///<ģʽ3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bitֹͣλ
    stcCfg.enMmdorCk        = UartMskEven;           ///<ż����
    stcCfg.stcBaud.u32Baud  = 9600;                  ///<������9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<ͨ��������Ƶ����
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<�������ʱ�ӣ�PCLK��Ƶ��ֵ
    Uart_Init(M0P_UART1, &stcCfg);                   ///<���ڳ�ʼ��
	
    ///<UART�ж�ʹ��
    Uart_ClrStatus(M0P_UART1,UartRC);                ///<���������
    Uart_ClrStatus(M0P_UART1,UartTC);                ///<���������
    Uart_EnableIrq(M0P_UART1,UartRxIrq);             ///<ʹ�ܴ��ڽ����ж�
    Uart_EnableIrq(M0P_UART1,UartTxIrq);             ///<ʹ�ܴ��ڽ����ж�
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);       ///<ϵͳ�ж�ʹ��

}

//�����룬�¶ȣ��趨�¶ȣ���ѹ������
void rc_process(void)//����У�鴦��
{

		unsigned int temp=0,temperature_temp,heat_set_temp;//���������� unsigned int ���͵ı�����������ʱ�洢���ݺͼ���У��͡�
		

			if(RC_buf[1]!=0x03&&RC_buf[1]!=0x06)//������յ������ݵĵڶ����ֽڼȲ��� 0x03 Ҳ���� 0x06����˵�������벻�ԣ��������־ Flag_erro ����Ϊ3��
			{
			//���CODE
			//�����벻��
				Flag_erro=3;
	
			}

			else if(RC_buf[1]==0x03)//������յ������ݵĵڶ����ֽ��� 0x03�����ʾ���յ��˶�ȡ���ݵ����
			{
				temp=MYCRC16(RC_buf,13);//������յ������ݵ�CRC16У���

       			if((temp>>8)!=RC_buf[13]||(unsigned char)temp!=RC_buf[14])//���CRC16У��ͣ����У��Ͳ�ƥ�䣬�򽫴����־ Flag_erro ����Ϊ4��
				{
				//���CODE
				//У���벻��
				Flag_erro=4;
		
				}

			else if(RC_buf[2]!=0x0a)//�������λ���� 0x0a���򽫴����־ Flag_erro ����Ϊ5��һ������Ĵ������������룬�¶ȣ��趨�¶ȣ���ѹ������
				{
					//���CODE
					//����λ����
					Flag_erro=5;
		
				}
			else
				{
				 	if(RC_buf[5]>0x01||RC_buf[7]>0x01)			//������״̬������
					{
					//���CODE
					//�����ݳ���Χ
					Flag_erro=6;
					}
					else 
					{
					//���CODE
					//ȡ������
					erro_value=RC_buf[3];//�ӽ��յ���������ȡ�� erro_value �ĸ�λ�ֽ�
					erro_value=erro_value<<8;
					erro_value=erro_value + RC_buf[4];//����յ������ݵĵ�λ�ֽ���ӣ��õ������� erro_value

					temperature_temp=RC_buf[5];
					temperature_temp=(temperature_temp<<8)+RC_buf[6];//���¶ȵĸ�λ�ֽ�����8λ��Ȼ������յ������ݵĵ�λ�ֽ���ӣ��õ��������¶�ֵ��
					output_wendu1=temperature_temp/10;//���õ����¶�ֵ����10���õ�����¶�

					temperature_temp=RC_buf[7];
					temperature_temp=(temperature_temp<<8)+RC_buf[8];//���������趨�¶ȵĸ�λ�ֽ�����8λ��Ȼ������յ������ݵĵ�λ�ֽ���ӣ��õ������ļ������趨�¶�ֵ
					heat_set_temp=temperature_temp/10;//���õ��ļ������趨�¶�ֵ����10���õ�ʵ�ʵļ������趨�¶�
					if(heat_set_temp!=0x0000)			
					{
						heat_set=heat_set_temp;//���ʵ�ʵļ������趨�¶��Ƿ�Ϊ0�������Ϊ0������� heat_set��
					}

					temperature_temp=RC_buf[9];
					temperature_temp=(temperature_temp<<8)+RC_buf[10];
					U_value=temperature_temp/10;//ͬ���ķ������õ������ĵ�ѹ����10���õ�ʵ�ʵĵ�ѹ
	

					temperature_temp=RC_buf[11];
					temperature_temp=(temperature_temp<<8)+RC_buf[12];
					I_value=temperature_temp/10;////ͬ���ķ������õ������ĵ�������10���õ�ʵ�ʵĵ���
					
					
					Flag_erro=0;//�������־ Flag_erro ����Ϊ0����ʾû�з�������
					Flag_w_busy=0;//��ͨ��æµ��־ Flag_w_busy ����Ϊ0����ʾͨ�Ų����Ѿ����
	
					}
				}
			//	Flag_rc1=0;
				 
			}
			else if(RC_buf[1]==0x06)//������յ������ݵĵڶ����ֽ��� 0x06�����ʾ���յ���д�����ݵ�Ӧ��
			{
				temp=MYCRC16(RC_buf,6);//������յ������ݵ�CRC16У��͡�

       			if((temp>>8)!=RC_buf[6]||(unsigned char)temp!=RC_buf[7])//���CRC16У��ͣ����У��Ͳ�ƥ�䣬�򽫴����־ Flag_erro ����Ϊ7��
				{
				//���CODE
				//У���벻��
				Flag_erro=7;
				}
				else
				{	
				 	if(RC_buf[2]!=TX_buf[2]||RC_buf[3]!=TX_buf[3]||RC_buf[4]!=TX_buf[4]||RC_buf[5]!=TX_buf[5])//����յ������ݲ���֮ǰ���͵����ݵ�Ӧ���򽫴����־ Flag_erro ����Ϊ9��
					{
						Flag_erro=9;	//�����ظ�����	
					}
					else{
						if(RC_buf[4]==0x00&&RC_buf[5]==0x00)//д������Ϊ0
							Flag_system=0;//ϵͳ����״̬
						else	
							Flag_system=1;//ϵͳ����״̬
					    Flag_erro=0;	//�����ظ���ȷ
						}	
					Flag_w_busy=0;		//�ͷ�ͨѶ
					//Flag_w_an=0;
			
				}
				Flag_w_an=1;			//���յ�Ӧ��
				Flag_w = 0;           //��д���־���㣬��ʾд����������
				Flag_w_off = 0;	       //��д�������־����
			}

			flag_rc=0;//�����ձ�־���㣬��ʾ���մ�����ɡ�
		
}
//////////////////////////�ӻ��ظ�����///////////////////////////////
void answer_process(void)
{	
	if(counter_RT<30)//ͨ�Ż�Ӧ������
	{
/*
		if(erro_value&0x0004)
			LED_CS3=1;
		else
			LED_CS3=0;	
*/
		if(UI_val==0)//���� UI_val ��ֵѡ����ʾ��ѹֵ (U_value) �����ֵ (I_value) �� val_display
			{	
			val_display=U_value;
			}
		else if(UI_val==1)
			{
			val_display=I_value;	
			}
			heater_state.all=erro_value;//�� erro_value ��ֵ���� heater_state �ṹ�����

			if (heater_state.E_bits.ON_OFF)//����·����״̬
				{
				if(heat_set==0x0000)	//����������趨�¶�Ϊ0�����趨�¶�����Ϊ40
					heat_set=40;

	//			if(Flag_system==0)
	//				{
	//				Flag_w_off=1;
	//				Flag_key_send=1;		
	//				}
				}
	//		else if(Flag_system==1)
	//				Flag_w=1;

			if(heater_state.E_bits.HOT_water)// ���ȱ�־
				{
				Gpio_SetIO(GpioPortB,GpioPin15);//����ʱLED_CS3����������
				}
			else	Gpio_ClrIO(GpioPortB, GpioPin15);
				
		if(Flag_erro!=0||(erro_value&0xfffa)!=0)//��� Flag_erro ��Ϊ0������ erro_value �ĵ���λ��Ϊ0,��LED����ʾģʽ����Ϊ ERR_MODE
		{
	
			LED_DisplayMode=ERR_MODE;
		
			if((erro_value&0xfffa)!=0 )
			{
			LED_DisplayMode=ALARM_MODE;
					
			
			if(heater_state.E_bits.HOT_xq)		Flag_erro=1;//��Ȧ����	1
			if(heater_state.E_bits.IGBT_Error)	Flag_erro=2;//IGBT����	2
			if(heater_state.E_bits.U_HIGH)		Flag_erro=3;// ��ѹ	3
			if(heater_state.E_bits.U_LOW)		Flag_erro=4;// Ƿѹ4
			if(heater_state.E_bits.I_HIGH)		Flag_erro=5;// ����	5
			if(heater_state.E_bits.IL)			Flag_erro=6;// ©��6
			if(heater_state.E_bits.Sensor_in)	Flag_erro=7;// ��ˮ������7
			if(heater_state.E_bits.Sensor_out)	Flag_erro=8;// ��ˮ������8
			if(heater_state.E_bits.tem_switch)	Flag_erro=9;// �¶ȿ��ض���9
		//	if(erro_value&0x0001)Flag_erro=i;	
			

		//	Flag_erro=alarm;
			}
	
		}
		else
		{
			if(LED_DisplayMode==ERR_MODE||LED_DisplayMode==ALARM_MODE)
			LED_DisplayMode=RUN_MODE;
			
		}

	}

	else
	{
	LED_DisplayMode=ERR_MODE;
	Flag_erro = 8;
	}
	
}

void Read_process(void)
{
	unsigned int temp = 0;
	Flag_w_busy = 1;//ͨ��æµ

	TX_buf[0]=Add_ZD;//��ַ��
	TX_buf[1]=0x03;//�����루����
	TX_buf[2]=0x00;
	TX_buf[3]=0x00;//�Ĵ�����ʼ��ַ��16���ֽڣ�
	TX_buf[4]=0X00;
	TX_buf[5]=0x05;//�Ĵ���������5��
	temp=MYCRC16(TX_buf,6);//���� CRC16 ������������ TX_buf ��ǰ6��Ԫ�ص� CRC16 У��ֵ����������洢�� temp ������ 	
	TX_buf[6]=temp>>8;//crc��λУ����
	TX_buf[7]=temp;//crc��λУ���루unsigned charΪһ���ֽڰ�λ��
	TX_count=8;//��ʾҪ���͵����ݳ���Ϊ8���ֽ�
	
	Uart_DisableIrq(M0P_UART1,UartRxIrq);			//��ֹ�����ж�
	Uart_EnableIrq(M0P_UART1,UartTxIrq);            //ʹ�ܷ����ж�
    Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);

}
//׼��һ�����ݣ������д��ڷ��Ͳ���������һЩ�̶������ݺͱ�����ֵ��ϳ�һ֡���ݣ���ͨ�����ڷ��ͳ�ȥ
void Write_process(void)
{

	unsigned int temp=0;
	Flag_w_busy=1; //ͨ��æµ

	TX_buf[0]=Add_ZD;//��ַ��
	TX_buf[1]=0x06;//�����루д��
	TX_buf[2]=0x00;
	TX_buf[3]=0x02;//�Ĵ�����ַ
	temp=heat_set*10;//�����¶�
	TX_buf[5]=temp;//�¶�ֵ�Ͱ�λ
	TX_buf[4]=temp>>8;	//�¶�ֵ�߰�λ
	temp=MYCRC16(TX_buf,6);//���� CRC16 ������������ TX_buf ��ǰ6��Ԫ�ص� CRC16 У��ֵ����������洢�� temp ������
	TX_buf[6]=temp>>8;//crc��λУ����
	TX_buf[7]=temp;//crc��λУ����
	TX_count=8;//��ʾҪ���͵����ݳ���Ϊ8���ֽ�

	Uart_DisableIrq(M0P_UART1,UartRxIrq);			//��ֹ�����ж�
	Uart_EnableIrq(M0P_UART1,UartTxIrq);            //ʹ�ܷ����ж�
    Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);
	
}
