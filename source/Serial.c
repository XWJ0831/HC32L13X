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
// 通信错误and状态数据结构
struct ERROR_BITS
{
	unsigned int	ON_OFF    :1;	// 主回路开关状态		
	unsigned int	HOT_xq    :1;	// 线圈过热	1	
	unsigned int	HOT_water :1;	// 加热标志		
	unsigned int	IGBT_Error:1;	// IGBT错误	2	
	unsigned int	U_HIGH    :1;	// 过压	3	
	unsigned int 	U_LOW     :1;	// 欠压4		
	unsigned int	I_HIGH    :1;	// 过流	5
	unsigned int	IL        :1;	// 漏电6

	unsigned int    Sensor_in :1;	// 进水传感器7
	unsigned int    Sensor_out:1;	// 出水传感器8
	unsigned int    tem_switch:1;	// 温度开关动作9
	unsigned int    KEEP1  	  :5;	// 保留
};



union Stauts_Error
{
	unsigned int	   all;
 	struct ERROR_BITS  E_bits;  
};
union Stauts_Error heater_state;
//*****************************************************************//
//*****************************************************************//
//CRC 高位字节值表
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
//CRC低位字节值表 
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

//CRC16校验程序

unsigned int MYCRC16(unsigned char *ptr,unsigned char Len) 	//函数返回一个int值 
{
	unsigned char CRC_H = 0xFF ;		//高CRC字节初始化
	unsigned char CRC_L = 0xFF ;		//低CRC 字节初始化
	unsigned char dex;					//CRC循环中的索引
	while(Len--)						//传输消息缓冲区 
	{
		dex=CRC_H ^ *ptr++;				//计算CRC
		CRC_H=CRC_L ^ CRCH[dex]; 
		CRC_L=CRCL[dex]; 
	} 
	return ((unsigned int)CRC_H << 8 | CRC_L); 
}

//UART1中断函数
void Uart1_IRQHandler(void)
{
    //数据接收中断
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);
        if(0 == u8AddrFlg)
        {
            u8AddrFlg = 1;
            u16AddrData = M0P_UART1->SBUF;                     //首字节为地址字节
            if(u16AddrData&0x100)
            {
                M0P_UART1->SCON_f.ADRDET = 0;                  //清零，准备接收数据帧
            }
        }else
        {
            RC_buf[RxCnt++] = Uart_ReceiveData(M0P_UART1); //数据字节
        }
    }
    //数据发送中断
    if(Uart_GetStatus(M0P_UART1, UartTC))
    {
        Uart_ClrStatus(M0P_UART1, UartTC);
        if(TxCnt>7)                                          //如果已经发送八个数据字节
        {	TxCnt = 0;
            RxCnt = 0;
            Uart_DisableIrq(M0P_UART1,UartTxIrq);              //禁止发送中断
            Uart_EnableIrq(M0P_UART1,UartRxIrq);               //使能接收中断
			flag_rc=1;					//接受标志位设置为1，表示接收到了有效的数据帧
        }else                                                  //如果还没有发送八个字节
        {
            Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);     //继续发送数据字节
            TxCnt++;
        }
    }

}

//串口引脚配置
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); //使能GPIO模块时钟

    ///<TX
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA, GpioPin9, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin9, GpioAf1);          //配置PA09 端口为URART1_TX

    ///<RX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA, GpioPin10, &stcGpioCfg);
    Gpio_SetAfMode(GpioPortA, GpioPin10, GpioAf1);          //配置PA10 端口为URART1_RX
}

//串口配置
void App_UartCfg(void)
{
    stc_uart_cfg_t    stcCfg;

    DDL_ZERO_STRUCT(stcCfg);

    ///< 开启外设时钟
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);///<使能uart1模块时钟

    ///<UART Init
    stcCfg.enRunMode        = UartMskMode3;          ///<模式3
    stcCfg.enStopBit        = UartMsk1bit;           ///<1bit停止位
    stcCfg.enMmdorCk        = UartMskEven;           ///<偶检验
    stcCfg.stcBaud.u32Baud  = 9600;                  ///<波特率9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;       ///<通道采样分频配置
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq(); ///<获得外设时钟（PCLK）频率值
    Uart_Init(M0P_UART1, &stcCfg);                   ///<串口初始化
	
    ///<UART中断使能
    Uart_ClrStatus(M0P_UART1,UartRC);                ///<清接收请求
    Uart_ClrStatus(M0P_UART1,UartTC);                ///<清接收请求
    Uart_EnableIrq(M0P_UART1,UartRxIrq);             ///<使能串口接收中断
    Uart_EnableIrq(M0P_UART1,UartTxIrq);             ///<使能串口接收中断
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);       ///<系统中断使能

}

//错误码，温度，设定温度，电压，电流
void rc_process(void)//接收校验处理
{

		unsigned int temp=0,temperature_temp,heat_set_temp;//定义了三个 unsigned int 类型的变量，用于临时存储数据和计算校验和。
		

			if(RC_buf[1]!=0x03&&RC_buf[1]!=0x06)//如果接收到的数据的第二个字节既不是 0x03 也不是 0x06，则说明功能码不对，将错误标志 Flag_erro 设置为3。
			{
			//添加CODE
			//功能码不对
				Flag_erro=3;
	
			}

			else if(RC_buf[1]==0x03)//如果接收到的数据的第二个字节是 0x03，则表示接收到了读取数据的命令。
			{
				temp=MYCRC16(RC_buf,13);//计算接收到的数据的CRC16校验和

       			if((temp>>8)!=RC_buf[13]||(unsigned char)temp!=RC_buf[14])//检查CRC16校验和，如果校验和不匹配，则将错误标志 Flag_erro 设置为4。
				{
				//添加CODE
				//校验码不对
				Flag_erro=4;
		
				}

			else if(RC_buf[2]!=0x0a)//如果数据位不是 0x0a，则将错误标志 Flag_erro 设置为5，一共五个寄存器数：错误码，温度，设定温度，电压，电流
				{
					//添加CODE
					//数据位不对
					Flag_erro=5;
		
				}
			else
				{
				 	if(RC_buf[5]>0x01||RC_buf[7]>0x01)			//读运行状态后排列
					{
					//添加CODE
					//有数据超范围
					Flag_erro=6;
					}
					else 
					{
					//添加CODE
					//取错误码
					erro_value=RC_buf[3];//从接收到的数据中取得 erro_value 的高位字节
					erro_value=erro_value<<8;
					erro_value=erro_value + RC_buf[4];//与接收到的数据的低位字节相加，得到完整的 erro_value

					temperature_temp=RC_buf[5];
					temperature_temp=(temperature_temp<<8)+RC_buf[6];//将温度的高位字节左移8位，然后与接收到的数据的低位字节相加，得到完整的温度值。
					output_wendu1=temperature_temp/10;//将得到的温度值除以10，得到输出温度

					temperature_temp=RC_buf[7];
					temperature_temp=(temperature_temp<<8)+RC_buf[8];//将加热器设定温度的高位字节左移8位，然后与接收到的数据的低位字节相加，得到完整的加热器设定温度值
					heat_set_temp=temperature_temp/10;//将得到的加热器设定温度值除以10，得到实际的加热器设定温度
					if(heat_set_temp!=0x0000)			
					{
						heat_set=heat_set_temp;//检查实际的加热器设定温度是否不为0，如果不为0，则更新 heat_set。
					}

					temperature_temp=RC_buf[9];
					temperature_temp=(temperature_temp<<8)+RC_buf[10];
					U_value=temperature_temp/10;//同样的方法，得到完整的电压除以10，得到实际的电压
	

					temperature_temp=RC_buf[11];
					temperature_temp=(temperature_temp<<8)+RC_buf[12];
					I_value=temperature_temp/10;////同样的方法，得到完整的电流除以10，得到实际的电流
					
					
					Flag_erro=0;//将错误标志 Flag_erro 设置为0，表示没有发生错误。
					Flag_w_busy=0;//将通信忙碌标志 Flag_w_busy 设置为0，表示通信操作已经完成
	
					}
				}
			//	Flag_rc1=0;
				 
			}
			else if(RC_buf[1]==0x06)//如果接收到的数据的第二个字节是 0x06，则表示接收到了写入数据的应答
			{
				temp=MYCRC16(RC_buf,6);//计算接收到的数据的CRC16校验和。

       			if((temp>>8)!=RC_buf[6]||(unsigned char)temp!=RC_buf[7])//检查CRC16校验和，如果校验和不匹配，则将错误标志 Flag_erro 设置为7。
				{
				//添加CODE
				//校验码不对
				Flag_erro=7;
				}
				else
				{	
				 	if(RC_buf[2]!=TX_buf[2]||RC_buf[3]!=TX_buf[3]||RC_buf[4]!=TX_buf[4]||RC_buf[5]!=TX_buf[5])//如果收到的数据不是之前发送的数据的应答，则将错误标志 Flag_erro 设置为9。
					{
						Flag_erro=9;	//按键回复错误	
					}
					else{
						if(RC_buf[4]==0x00&&RC_buf[5]==0x00)//写入数据为0
							Flag_system=0;//系统休眠状态
						else	
							Flag_system=1;//系统正常状态
					    Flag_erro=0;	//按键回复正确
						}	
					Flag_w_busy=0;		//释放通讯
					//Flag_w_an=0;
			
				}
				Flag_w_an=1;			//已收到应答
				Flag_w = 0;           //将写入标志清零，表示写入操作已完成
				Flag_w_off = 0;	       //将写入结束标志清零
			}

			flag_rc=0;//将接收标志清零，表示接收处理完成。
		
}
//////////////////////////从机回复处理///////////////////////////////
void answer_process(void)
{	
	if(counter_RT<30)//通信回应计数器
	{
/*
		if(erro_value&0x0004)
			LED_CS3=1;
		else
			LED_CS3=0;	
*/
		if(UI_val==0)//根据 UI_val 的值选择显示电压值 (U_value) 或电流值 (I_value) 给 val_display
			{	
			val_display=U_value;
			}
		else if(UI_val==1)
			{
			val_display=I_value;	
			}
			heater_state.all=erro_value;//将 erro_value 的值赋给 heater_state 结构体变量

			if (heater_state.E_bits.ON_OFF)//主回路开关状态
				{
				if(heat_set==0x0000)	//如果开启且设定温度为0，则将设定温度设置为40
					heat_set=40;

	//			if(Flag_system==0)
	//				{
	//				Flag_w_off=1;
	//				Flag_key_send=1;		
	//				}
				}
	//		else if(Flag_system==1)
	//				Flag_w=1;

			if(heater_state.E_bits.HOT_water)// 加热标志
				{
				Gpio_SetIO(GpioPortB,GpioPin15);//加热时LED_CS3亮，否则灭
				}
			else	Gpio_ClrIO(GpioPortB, GpioPin15);
				
		if(Flag_erro!=0||(erro_value&0xfffa)!=0)//如果 Flag_erro 不为0，或者 erro_value 的低三位不为0,将LED的显示模式设置为 ERR_MODE
		{
	
			LED_DisplayMode=ERR_MODE;
		
			if((erro_value&0xfffa)!=0 )
			{
			LED_DisplayMode=ALARM_MODE;
					
			
			if(heater_state.E_bits.HOT_xq)		Flag_erro=1;//线圈过热	1
			if(heater_state.E_bits.IGBT_Error)	Flag_erro=2;//IGBT错误	2
			if(heater_state.E_bits.U_HIGH)		Flag_erro=3;// 过压	3
			if(heater_state.E_bits.U_LOW)		Flag_erro=4;// 欠压4
			if(heater_state.E_bits.I_HIGH)		Flag_erro=5;// 过流	5
			if(heater_state.E_bits.IL)			Flag_erro=6;// 漏电6
			if(heater_state.E_bits.Sensor_in)	Flag_erro=7;// 进水传感器7
			if(heater_state.E_bits.Sensor_out)	Flag_erro=8;// 出水传感器8
			if(heater_state.E_bits.tem_switch)	Flag_erro=9;// 温度开关动作9
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
	Flag_w_busy = 1;//通信忙碌

	TX_buf[0]=Add_ZD;//地址码
	TX_buf[1]=0x03;//功能码（读）
	TX_buf[2]=0x00;
	TX_buf[3]=0x00;//寄存器起始地址（16个字节）
	TX_buf[4]=0X00;
	TX_buf[5]=0x05;//寄存器个数（5）
	temp=MYCRC16(TX_buf,6);//调用 CRC16 函数计算数组 TX_buf 的前6个元素的 CRC16 校验值，并将结果存储到 temp 变量中 	
	TX_buf[6]=temp>>8;//crc高位校验码
	TX_buf[7]=temp;//crc低位校验码（unsigned char为一个字节八位）
	TX_count=8;//表示要发送的数据长度为8个字节
	
	Uart_DisableIrq(M0P_UART1,UartRxIrq);			//禁止接受中断
	Uart_EnableIrq(M0P_UART1,UartTxIrq);            //使能发送中断
    Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);

}
//准备一组数据，并进行串口发送操作。它将一些固定的数据和变量的值组合成一帧数据，并通过串口发送出去
void Write_process(void)
{

	unsigned int temp=0;
	Flag_w_busy=1; //通信忙碌

	TX_buf[0]=Add_ZD;//地址码
	TX_buf[1]=0x06;//功能码（写）
	TX_buf[2]=0x00;
	TX_buf[3]=0x02;//寄存器地址
	temp=heat_set*10;//设置温度
	TX_buf[5]=temp;//温度值低八位
	TX_buf[4]=temp>>8;	//温度值高八位
	temp=MYCRC16(TX_buf,6);//调用 CRC16 函数计算数组 TX_buf 的前6个元素的 CRC16 校验值，并将结果存储到 temp 变量中
	TX_buf[6]=temp>>8;//crc高位校验码
	TX_buf[7]=temp;//crc低位校验码
	TX_count=8;//表示要发送的数据长度为8个字节

	Uart_DisableIrq(M0P_UART1,UartRxIrq);			//禁止接受中断
	Uart_EnableIrq(M0P_UART1,UartTxIrq);            //使能发送中断
    Uart_SendDataIt(M0P_UART1, TX_buf[TxCnt]);
	
}
