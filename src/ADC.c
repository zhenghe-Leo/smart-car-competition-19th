#include "headfile.h"
#include "isr.h"

#define func_limit_ab(x ,a , b) ((x) < (a) ? (a) : ( (x) > (b) ? (b) : (x)))
#define ADC_LEFTX_CHANNEL ADC_P06      // 定义左横向电感通道 
#define ADC_RIGHTX_CHANNEL ADC_P15      // 定义右横向电感通道 
#define ADC_LEFTY_CHANNEL ADC_P11      // 定义左竖向电感通道 
#define ADC_RIGHTY_CHANNEL ADC_P14     // 定义右竖向电感通道 
#define AD_LXMAX 100             // 定义左横向最大电感值
#define AD_LXMIN 20        // 定义左横向最小电感值
#define AD_RXMAX 120             // 定义右横向最大电感值
#define AD_RXMIN 25        // 定义右横向最小电感值
#define AD_LYMAX 90             // 定义左竖向最大电感值
#define AD_LYMIN 10        // 定义左竖向最小电感值
#define AD_RYMAX 130        // 定义右竖向最大电感值
#define AD_RYMIN 30             // 定义右竖向最小电感值
float ad_left_x; 
float ad_right_x; 
float ad_left_y; 
float ad_right_y;
float left_value;
float right_value;
float ad_sum; 
float ad_diff; 
float deviation,deviation_last; 
float i,temp,k;
float ad_valu[4][5];
float adc_sum	[4];
float ad_average[4];
float duty;

void DGZ_read()
	{

	for (i=0;i<4;i++){
	{
		ad_valu[0][i]= adc_once(ADC_P06, ADC_8BIT);
    ad_valu[1][i]= adc_once(ADC_P11, ADC_8BIT);
    ad_valu[2][i]= adc_once(ADC_P14, ADC_8BIT);
    ad_valu[3][i]= adc_once(ADC_P15, ADC_8BIT);
	}
		for (k=0;k<4;k++)
		{
			if(ad_valu[i][k] > ad_valu[i][k+1])        //前面的比后面的大  则进行交换
								{
									 temp = ad_valu[i][k+1];
									 ad_valu[i][k+1] = ad_valu[i][k];
									 ad_valu[i][k] = temp;
								}
		}
		for (k=0;k<4;k++)
		{
			if(ad_valu[i][8-k] > ad_valu[i][9-k])        //前面的比后面的大  则进行交换
								{
									 temp = ad_valu[i][k+1];
									 ad_valu[i][k+1] = ad_valu[i][k];
									 ad_valu[i][k] = temp;
								}
		}		
	}

		for (i=1;i<3;i++)//取掉最大最小进行均值滤波
		{
			adc_sum[0]+=ad_valu[0][i];
			adc_sum[1]+=ad_valu[1][i];
			adc_sum[2]+=ad_valu[2][i];
			adc_sum[3]+=ad_valu[3][i];
	  }
		for(i=0;i<4;i++)
		{
		 ad_average[i]=adc_sum[i]/3;//0~4分别对应 L5、L3、L6,L2
			adc_sum[i]=0;}		
		
		ad_left_x=ad_average[0];
    ad_left_y=ad_average[1];
    ad_right_y=ad_average[2];
    ad_right_x=ad_average[3];
			
//// 采集左横向电感数值 采集5次求平均    
//ad_left_x =adc_mean_filter(ADC_LEFTX_CHANNEL, 5);

//// 采集右横向电感数值 采集5次求平均
//ad_right_x =adc_mean_filter(ADC_RIGHTX_CHANNEL, 5);

//// 采集左竖向电感数值 采集5次求平均 
//ad_left_y =adc_mean_filter(ADC_LEFTY_CHANNEL, 5); 

//// 采集右竖向电感数值 采集5次求平均
//ad_right_y =adc_mean_filter(ADC_RIGHTY_CHANNEL, 5); 

// 限制幅值
ad_left_x  =func_limit_ab(ad_left_x,AD_LXMIN, AD_LXMAX);

ad_right_x  =func_limit_ab(ad_right_x,AD_RXMIN, AD_RXMAX);

ad_left_y  =func_limit_ab(ad_left_y,AD_LYMIN, AD_LYMAX);

ad_right_y  =func_limit_ab(ad_right_y,AD_RYMIN, AD_RYMAX);

// 归一化到范围0-100
ad_left_x = (ad_left_x- AD_LXMIN) * 100 / (AD_LXMAX- AD_LXMIN);

ad_right_x = (ad_right_x- AD_RXMIN) * 100 / (AD_RXMAX- AD_RXMIN);

ad_left_y = (ad_left_y- AD_LYMIN) * 100 / (AD_LYMAX- AD_LYMIN);

ad_right_y = (ad_right_y- AD_RYMIN) * 100 / (AD_RYMAX- AD_RYMIN);

// 计算电感向量值
left_value  = func_limit_ab(sqrt(ad_left_x  * ad_left_x  + 1.4*ad_left_y  * ad_left_y ), 0, 100);

right_value = func_limit_ab(sqrt(ad_right_x * ad_right_x + 1.4*ad_right_y * ad_right_y), 0, 100);

  ad_sum= left_value + right_value;         // 计算电感之和 

// 计算电感之差

  ad_diff= left_value - right_value; 

//if(ad_sum>20)                                     // 边界保护 

//{ 

// 方法一 
deviation_last=deviation;
deviation = (ad_diff * 100) / (ad_sum + 1);// 计算差比和数值 并放大一百倍 
//deviation = (ad_diff << 7 )/(ad_sum + 1); // 计算差比和数值 并放大128倍
//}
}