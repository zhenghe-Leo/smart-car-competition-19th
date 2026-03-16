#include "headfile.h"
#include "isr.h"
//变量定义
int SJ_speedR,SJ_speedL;
int QW_speedL,QW_speedR;
int PWML,PWMR;
float dataa,datab,datac,datad,error1,error_v,error_p;



/**********************电感初始化***********************/
void DG_init()
{
	adc_init(ADC_P06, ADC_SYSclk_DIV_2); 
  adc_init(ADC_P11, ADC_SYSclk_DIV_2);  
  adc_init(ADC_P14, ADC_SYSclk_DIV_2);   
  adc_init(ADC_P15, ADC_SYSclk_DIV_2);
//	adc_init(ADC_P06, ADC_8BIT);
//  adc_init(ADC_P11, ADC_8BIT);
//  adc_init(ADC_P14, ADC_8BIT);
//  adc_init(ADC_P15, ADC_8BIT);
}

/**********************编码器初始化***********************/
void BMQ_init()
{
	SJ_speedL=0;
	SJ_speedR=0;
	ctimer_count_init(PLUSE_LEFT);
	ctimer_count_init(PLUSE_RIGHT);
}

/**********************电机初始化***********************/
void DJ_init()
{
	pwm_init(PWM_1,17000,0);
  pwm_init(PWM_2,17000,0);
	gpio_mode(P1_0,GPO_PP);
  gpio_mode(P2_4,GPO_PP);
}
/**********************电感信号处理***********************/
void DG_read()
{
	int y,u,h;
  int min=20;
  int max=232;
	float ad_valu1[4][5];
  float ad_sum1[4];
  float ad_average1[4];
	
	//限幅
	for (y=0;y<4;y++){
	{
		ad_valu1[0][y]= adc_once(ADC_P06, ADC_8BIT);
    ad_valu1[1][y]= adc_once(ADC_P11, ADC_8BIT);
    ad_valu1[2][y]= adc_once(ADC_P14, ADC_8BIT);
    ad_valu1[3][y]= adc_once(ADC_P15, ADC_8BIT);
	}
		for (h=0;h<4;h++)
		{
			if(ad_valu1[y][h] > ad_valu1[y][h+1])        //前面的比后面的大  则进行交换
								{
									 u = ad_valu1[y][h+1];
									 ad_valu1[y][h+1] = ad_valu1[y][h];
									 ad_valu1[y][h] = u;
								}
		}
		for (h=0;h<4;h++)
		{
			if(ad_valu1[y][8-h] > ad_valu1[y][9-h])        //前面的比后面的大  则进行交换
								{
									 u = ad_valu1[y][h+1];
									 ad_valu1[y][h+1] = ad_valu1[y][h];
									 ad_valu1[y][h] = u;
								}
		}		
	}

		for (y=1;y<4;y++)//取掉最大最小进行均值滤波
		{
			ad_sum1[0]+=ad_valu1[0][y];
			ad_sum1[1]+=ad_valu1[1][y];
			ad_sum1[2]+=ad_valu1[2][y];
			ad_sum1[3]+=ad_valu1[3][y];
	  }
		for(y=0;y<4;y++)
		{
		 ad_average1[y]=ad_sum1[y]/3;//0~4分别对应 L5、L3、L6,L2
			ad_sum1[y]=0;}
		
	  for(y=0;y<4;y++){
		if (ad_average1[y]<min) ad_average1[y]=min;}
	//归一化
		dataa=(ad_average1[0]-min)/(max-min)*100;
		datab=(ad_average1[1]-min)/(max-min)*100;
		datac=(ad_average1[2]-min)/(max-min)*100;
		datad=(ad_average1[3]-min)/(max-min)*100;
	//差比和
		error1= 100*(sqrt(dataa*dataa+datab*datab)-sqrt(datac*datac+datad*datad))/(1+(sqrt(dataa*dataa+datab*datab)+sqrt(datac*datac+datad*datad)));
		error_v=(datab-datac)*100/(datab+datac);
		error_p=(dataa-datad)*100/(dataa+datad);
		if(error_p>100) error_p=100;
		else if(error_p<-100) error_p=-100;
		if(error_v>100) error_v=100;
		else if(error_v<-100) error_v=-100;
		if(error1>100) error1=100;
		else if(error1<-100) error1=-100;
		
}
/**********************编码器信号处理***********************/
void BMQ_read()
{
	SJ_speedL=ctimer_count_read(PLUSE_LEFT);
	SJ_speedR=ctimer_count_read(PLUSE_RIGHT);

	if (DIR_LEFT==1) SJ_speedL=SJ_speedL;
	else SJ_speedL=-SJ_speedL;
  if (DIR_RIGHT==1) SJ_speedR=SJ_speedR;
	else SJ_speedR=-SJ_speedR;
	
			
	ctimer_count_clean(PLUSE_RIGHT);
  ctimer_count_clean(PLUSE_LEFT);

}

/**********************PID循迹***********************/
///**********************分段式PID***********************/
//	void FDPID()
//{
//	float L_err[2],R_err[2];
//	float PWML,PWMR;
//	float KP_La,KP_Lb,KD_L,KP_Ra,KP_Rb,KD_R;
//	int A=2000;

//	L_err[1]=L_err[0];
//	L_err[0]=QW_speedL-SJ_speedL;
//	
//	R_err[1]=R_err[0];
//	R_err[0]=QW_speedR-SJ_speedR;
//	
//	PWML=KP_La*L_err[0]+KP_Lb*L_err[0]*L_err[0]*L_err[0]+KD_L*(L_err[0]-L_err[1]);
//	PWMR=KP_Ra*R_err[0]+KP_Rb*R_err[0]*R_err[0]*R_err[0]+KD_R*(R_err[0]-R_err[1]);
//	
//    if (PWML>=A){
//			PWML=A;}
//		else if(PWML<=-A){
//			PWML=-A;}
//		
//		if (PWMR>=A){
//			PWMR=A;}
//		else if(PWMR<=-A){
//			PWMR=-A;}	
//		}
/**********************电机驱动***********************/
	void PWM()
{ 
	     if (PWML>0)
				{
						DIR_2 = 0;
						pwm_duty(PWM_2,PWML);
				}
				else if (PWML<0)
				{
						DIR_2 = 1;
						pwm_duty(PWM_2,-PWML);
				}


				if (PWMR>0)
				{
						DIR_1 = 0;
						pwm_duty(PWM_1,PWMR);
				}
				else if (PWMR<0)
				{
						DIR_1 = 1;
						pwm_duty(PWM_1,-PWMR);
				}     
	}

