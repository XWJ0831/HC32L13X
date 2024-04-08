#ifndef __SERIAL_H
#define __SERIAL_H

unsigned int MYCRC16(unsigned char *ptr,unsigned char Len);
void Uart1_IRQHandler(void);
void App_PortInit(void);
void App_UartCfg(void);
void rc_process(void);
void answer_process(void);
void Read_process(void);
void Write_process(void);

#endif