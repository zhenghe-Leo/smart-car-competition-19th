///*********************************************************************************************************************
// * COPYRIGHT NOTICE
// * Copyright (c) 2020,逐飞科技
// * All rights reserved.
// * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
// *
// * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
// * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
// *
// * @file       		isr
// * @company	   		成都逐飞科技有限公司
// * @author     		逐飞科技(QQ790875685)
// * @version    		查看doc内version文件 版本说明
// * @Software 			MDK FOR C251 V5.60
// * @Target core		STC32F12K
// * @Taobao   			https://seekfree.taobao.com/
// * @date       		2020-4-14
// ********************************************************************************************************************/
#include "headfile.h"
#include "isr.h"

int Direction_PWM;
int speed;
int flag_zhiL=0,flag_zhiR=0;
int distance=0,detection=0,round=0,bizhang=0;
//UART1中断
void UART1_Isr() interrupt 4
{
    uint8 res;
	static uint8 dwon_count;
    if(UART1_GET_TX_FLAG)
    {
        UART1_CLEAR_TX_FLAG;
        busy[1] = 0;
    }
    if(UART1_GET_RX_FLAG)
    {
        UART1_CLEAR_RX_FLAG;
        res = SBUF;
        //程序自动下载
        if(res == 0x7F)
        {
            if(dwon_count++ > 20)
                IAP_CONTR = 0x60;
        }
        else
        {
            dwon_count = 0;
        }
    }
}

//UART2中断
void UART2_Isr() interrupt 8
{
    if(UART2_GET_TX_FLAG)
	{
        UART2_CLEAR_TX_FLAG;
		busy[2] = 0;
	}
    if(UART2_GET_RX_FLAG)
	{
        UART2_CLEAR_RX_FLAG;
		//接收数据寄存器为：S2BUF
		if(wireless_module_uart_handler != NULL)
		{
			// 该函数为函数指针
			// 再初始化无线模块的时候，设置该函数地址
			wireless_module_uart_handler(S2BUF);
		}
	}
}


//UART3中断
void UART3_Isr() interrupt 17
{
    if(UART3_GET_TX_FLAG)
	{
        UART3_CLEAR_TX_FLAG;
		busy[3] = 0;
	}
    if(UART3_GET_RX_FLAG)
	{
        UART3_CLEAR_RX_FLAG;
		//接收数据寄存器为：S3BUF

	}
}


//UART4中断
void UART4_Isr() interrupt 18
{
    if(UART4_GET_TX_FLAG)
	{
        UART4_CLEAR_TX_FLAG;
		busy[4] = 0;
	}
    if(UART4_GET_RX_FLAG)
	{
        UART4_CLEAR_RX_FLAG;
		//接收数据寄存器为：S4BUF;


	}
}

#define LED P52
void INT0_Isr() interrupt 0
{
	LED = 0;	//点亮LED
}
void INT1_Isr() interrupt 2
{
 


}
void INT2_Isr() interrupt 10
{
	INT2_CLEAR_FLAG;  //清除中断标志
}
void INT3_Isr() interrupt 11
{
	INT3_CLEAR_FLAG;  //清除中断标志
}

void INT4_Isr() interrupt 16
{
	INT4_CLEAR_FLAG;  //清除中断标志
}

void TM0_Isr() interrupt 1
{

}
void TM1_Isr() interrupt 3
{
 DGZ_read();
 BMQ_read();
 dl1b_get_distance ();
/*************************元素判断程序*****************************/
	/******************直线、弯道和丢线判断************************/
		if(ad_left_y>ad_right_y&&ad_left_x<15&&ad_right_x<5&detection==0){
		flag_zhiL=1;}
		else if(ad_left_y<ad_right_y&&ad_left_x<5&&ad_right_x<15&&detection==0){
				flag_zhiR=1;}
//环岛		
	  if(ad_left_x>75&&ad_right_x>75&&ad_diff>20&&ad_right_y==0&&ad_left_y>0&&detection==0&&flag_zhiL==0&&flag_zhiR==0){
		detection=1;
		distance+=(SJ_speedL+SJ_speedR)/2;
		gpio_mode(P3_2,GPO_PP);				
		P32=1;}
		if(detection==1&&3500<distance<3700)	{
			distance+=(SJ_speedL+SJ_speedR)/2;
			deviation=30;
			}		
		else if(distance>3700){
			distance=0;
			detection=0;
}	
			
//避障		
//	  if(15<ad_left_x<70&&15<ad_right_x<70&&dl1b_distance_mm<1000&&bizhang==0){
//		bizhang=1;}
//		if(bizhang==1&&distance<1500)	{
//		deviation=-90;
//		distance+=(SJ_speedL+SJ_speedR)/2;
//		}		
//		else if(bizhang==1&&1500<distance<3000)	{
//		deviation=70;
//		distance+=(SJ_speedL+SJ_speedR)/2;
//		}		
//		else if(bizhang==1&&3000<distance<3500)	{
//		deviation=-90;
//		distance+=(SJ_speedL+SJ_speedR)/2;
//		}
//		else if(bizhang==1&&3500<distance){ //还是用电感判断？
////		bizhang=0;
//		distance=0;
//		}
		
		if(flag_zhiL==1&&flag_zhiR==0){
			QW_speedL=-120;
			QW_speedR=70;
			flag_zhiL=0;}
		else if(flag_zhiL==0&&flag_zhiR==1){
			QW_speedL=70;
			QW_speedR=-120;
			flag_zhiR=0;}
		else if(flag_zhiL==0&&flag_zhiR==0){
	      if(ad_sum<1&&ad_diff<1){
        speed=0;}
				else{
				speed=50;}//可以到65 但是需要写出环 左拐弯需要改
	duty=deviation*0.05+(deviation-deviation_last)*0.56;//0.4 0.56
	if (duty>0.5) duty=0.5;
	if (duty<-0.5) duty=-0.5;
	QW_speedL = speed*(1-duty);
	QW_speedR = speed*(1+duty);
			}
/*************************PID循迹程序*****************************/
//				if(ad_sum<1&&ad_diff<1){
//        speed=0;}
//				else{
//				speed=50;}
//  duty=deviation*0.05+(deviation-deviation_last)*0.56;//0.4 0.56
//	if (duty>0.5) duty=0.5;
//	if (duty<-0.5) duty=-0.5;
//	QW_speedL = speed*(1-duty);
//	QW_speedR = speed*(1+duty);
	ZLPIDL();
	ZLPIDR();
	PWML = PWM3;
	PWMR = PWM4;
//	WZPIDL();
//	WZPIDR();
//	PWML = PWM5;
//	PWMR = PWM6;
//			
	PWM();

}
void TM2_Isr() interrupt 12
{
	TIM2_CLEAR_FLAG;  //清除中断标志
	
}
void TM3_Isr() interrupt 19
{
	TIM3_CLEAR_FLAG; //清除中断标志
	
}
extern void pit_callback(void);
void TM4_Isr() interrupt 20
{
	TIM4_CLEAR_FLAG; //清除中断标志
	ccd_collect();	 //CCD采集数据
//	pit_callback();
}

//void  INT0_Isr()  interrupt 0;
//void  TM0_Isr()   interrupt 1;
//void  INT1_Isr()  interrupt 2;
//void  TM1_Isr()   interrupt 3;
//void  UART1_Isr() interrupt 4;
//void  ADC_Isr()   interrupt 5;
//void  LVD_Isr()   interrupt 6;
//void  PCA_Isr()   interrupt 7;
//void  UART2_Isr() interrupt 8;
//void  SPI_Isr()   interrupt 9;
//void  INT2_Isr()  interrupt 10;
//void  INT3_Isr()  interrupt 11;
//void  TM2_Isr()   interrupt 12;
//void  INT4_Isr()  interrupt 16;
//void  UART3_Isr() interrupt 17;
//void  UART4_Isr() interrupt 18;
//void  TM3_Isr()   interrupt 19;
//void  TM4_Isr()   interrupt 20;
//void  CMP_Isr()   interrupt 21;
//void  I2C_Isr()   interrupt 24;
//void  USB_Isr()   interrupt 25;
//void  PWM1_Isr()  interrupt 26;
//void  PWM2_Isr()  interrupt 27;