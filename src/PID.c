#include "headfile.h"
#include "isr.h"

int KP_L=75,KI_L=12,KD_L=5;//75 12 0
int KP_R=75,KI_R=12,KD_R=5;
int KP_L2=60,KI_L2=2,KD_L2=0;
int KP_R2=60,KI_R2=2,KD_R2=0;
int PWM3=0,PWM4=0;
int PWM5=0,PWM6=0;
int L_err[3]={0,0,0};
int R_err[3]={0,0,0};
int L_err2[3]={0,0,0};
int R_err2[3]={0,0,0};
int I_L=0,I_R=0;
int L_sum=0,R_sum=0;

	/***********************************************/
	void ZLPIDL()
{
    L_err[2] = L_err[1];
	  L_err[1] = L_err[0];
	  L_err[0] = (QW_speedL - SJ_speedL);
	  I_L=(KP_L * (L_err[0]-L_err[1]) + KI_L * L_err[0] + KD_L * (L_err[0] - 2*L_err[1]+L_err[2]));
	
		if (I_L>3000)
		I_L=3000;
		else if(I_L<-3000)
		I_L=-3000;

		PWM3+=I_L;
		
		if (PWM3 > 7000) 
		PWM3 = 7000;
	  else if (PWM3 < -7000)
		PWM3 = -7000;
	}
			/***********************************************/
   void ZLPIDR()
{
		R_err[2] = R_err[1];
  	R_err[1] = R_err[0];
	  R_err[0] = (QW_speedR - SJ_speedR);
	  I_R=(KP_R * (R_err[0]-R_err[1]) + KI_R * R_err[0] + KD_R * (R_err[0] - 2*R_err[1]+R_err[2]));
		
		if (I_R>3000)
		I_R=3000;
		else if(I_R<-3000)
		I_R=-3000;

		PWM4+=I_R;

	  if (PWM4 > 7000) //7000
		PWM4 = 7000;
	  else if (PWM4 < -7000)
		PWM4 = -7000;
	}
	/***********************************************/
	void WZPIDL()
{
	  L_err2[1] = L_err2[0];
	  L_err2[0] = (QW_speedL - SJ_speedL);
		L_sum+=	 L_err2[0];

	
		if (L_sum>3000)
		L_sum=3000;
		else if(L_sum<-3000)
		L_sum=-3000;

	  PWM5=KP_L2 * L_err2[0]+ KI_L2 * L_sum + KD_L2 * (L_err2[0] - L_err2[1]);
		
		if (PWM5 > 7000) 
		PWM5 = 7000;
	  else if (PWM5 < -7000)
		PWM5 = -7000;
	}
			/***********************************************/
   void WZPIDR()
{
  	R_err2[1] = R_err2[0];
	  R_err2[0] = (QW_speedR - SJ_speedR);
		R_sum+=	 R_err2[0];
	
		if (R_sum>3000)
		R_sum=3000;
		else if(R_sum<-3000)
		R_sum=-3000;

	  PWM6=KP_R2 * R_err2[0]+ KI_R2 * R_sum + KD_R2 * (R_err2[0] - R_err2[1]);

	  if (PWM6 > 7000) //7000
		PWM6 = 7000;
	  else if (PWM6 < -7000)
		PWM6 = -7000;
	}