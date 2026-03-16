/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2023-07-27

 ********************************************************************************************************************/
#include "headfile.h"
#include "isr.h"

// 关于内核频率的设定，可以查看board.h文件
// 在board_init中,已经将P54引脚设置为复位
// 如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可

void main()
{
	clock_init(SYSTEM_CLOCK_52M);	// 初始化系统频率,勿删除此句代码。
	board_init();					// 初始化寄存器,勿删除此句代码。
  //外部设备初始化
	DG_init();
	BMQ_init();
	DJ_init();
	ips114_init();
	ips114_clear(YELLOW);
	dl1b_init();



  pit_timer_ms(TIM_1,5);
    // 此处编写用户代码 例如外设初始化代码等
    
    // 此处编写用户代码 例如外设初始化代码等
	
	
    while(1)
    {
		// 此处编写需要循环执行的代码
				ips114_showstr(0,0,"data1:");        
        ips114_showfloat(6*8,0,ad_left_x,3,1);
				ips114_showstr(0,1,"data4:");        
        ips114_showfloat(6*8,1,ad_right_x,3,1);
		    ips114_showstr(0,2,"data2");        
        ips114_showfloat(6*8,2,ad_left_y,3,1);
        ips114_showstr(0,3,"data3:");        
        ips114_showfloat(6*8,3,ad_right_y,3,1);
        ips114_showstr(0,4,"dl1b:");    
        ips114_showint16(6*8,4,dl1b_distance_mm);	
        ips114_showstr(0,5,"spR:");      
        ips114_showint16(6*8,5,SJ_speedR);	  
        ips114_showstr(0,6,"diff:");        
        ips114_showfloat(6*8,6,ad_diff,3,1);
			  ips114_showstr(0,7,"juli:");        
        ips114_showint16(6*8,7,SJ_speedL);
        // 此处编写需要循环执行的代码
    }
}


