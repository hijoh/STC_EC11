#include <STC8.H>
#include <INTRINS.H>
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;


/*************	引脚声明	**************/
sbit LED1=P2^0;  // 定义LED1，低电平有效，指示时间0分钟
sbit LED2=P2^1;  // 定义LED2，低电平有效，指示时间15分钟
sbit LED3=P2^2;  // 定义LED3，低电平有效，指示时间45分钟
sbit LED4=P2^3;  // 定义LED4，低电平有效，指示时间60分钟
sbit LED5=P2^4;  // 定义LED5，低电平有效，指示错误报警
sbit FM  =P2^5;  // 定义蜂鸣器，低电平有效，声音提示
sbit HOT =P2^6;  // 定义发热管输出，高电平有效，加热工作
sbit FAN =P2^7;  // 定义风扇输出，高电平有效，风扇
sbit UL  =P0^0;  // 定义紫外灯，高电平有效，跟加热管同时工作
sbit PA = P3^2; // 定义EC11编码器的A脉冲引脚
sbit PN = P3^3; // 定义EC11编码器开关引脚
sbit PB = P3^4; // 定义EC11编码器的B脉冲引脚
/*************	本地变量声明	**************/
bit power,hotfan,an;
char dat,tmp,tmp2;
u16 tmt;
u16 millisecond,second,time1,time2;
/*************	本地函数声明	**************/
void SysInit();         //系统初始化
void Timer0_Init(void);	//定时器0初始化
void Int0_Init(void);		//外部中断0初始化
void Int1_Init(void);		//外部中断0初始化
void fmsound(u8 dt);	  //蜂鸣器次数
void Delay100ms(); 
void Delay500ms();
/******************************************/
/********  外部中断0中断服务函数  *********/
void INT0_Isr() interrupt 0
{
 if(!power)return;
 //if(dat==0)tmt=0;	
 //if(tmt>=500)dat=0;//超过2秒复位
 if(PB)dat++;//左转
 if(!PB)dat--;//右转
 if(dat>=3) //左转3个脉冲
 {
  tmp++;dat=0;tmt=0;an=1;
	if(tmp>3)tmp=3;
	if(tmp==1)second=900;
	if(tmp==2)second=2700;	
	if(tmp==3)second=3600;
	//if(tmp==1)second=15;
	//if(tmp==2)second=45;	
	//if(tmp==3)second=60;
 }
 if(dat<=-3)//右转3个脉冲	
 {
  tmp--;dat=0;tmt=0;an=1;
	if(tmp<0)tmp=0; 
	if(tmp==1)second=900;
	if(tmp==2)second=2700;	
	//if(tmp==1)second=15;
	//if(tmp==2)second=45;	
	if(tmp==0)second=0; 
 }
}
/**********  定时器0中断服务函数  *********/
void TM0_Isr() interrupt 1
{
 millisecond++;time1++;
 if(millisecond>1000)
 {
  if(second>0)second--;
  millisecond=0;
 }
 if(HOT==0&&hotfan==1)//关热管后风扇继续开30秒
 {
  time2++;
	if(time2>30000)
	{
	 hotfan=0;FAN=0;
	 time2=0;
	}
 }
 if(tmt<500)tmt++;
}
/********* 外部中断1中断服务函数  *********/
void INT1_Isr() interrupt 2
{
 time1=0;
 while(!PN)
 {
  if(power==0&&time1>500)//关机状态下按下0.5秒开机
	{
	 power=1;
	 break;
	}
  if(power==1&&time1>3000)//开机状态下按下3秒关机
  {
	 power=0;second=0;
	 break;	
	}
 }	 
}
/**********  主函数  *****************/
void main()
{ 
 SysInit();
 while(1)
 {
  while(!power){LED1=1;LED2=1;LED3=1;LED4=1;LED5=0;HOT=0;UL=0;}
  while(power)
	{
	 if(tmp==0){LED1=0;LED2=1;LED3=1;LED4=1;LED5=1;HOT=0;UL=0;}
	 if(tmp==1){LED1=1;LED2=0;LED3=1;LED4=1;LED5=1;}
	 if(tmp==2){LED1=1;LED2=1;LED3=0;LED4=1;LED5=1;}	
	 if(tmp==3){LED1=1;LED2=1;LED3=1;LED4=0;LED5=1;}
	 //if(second>15&&second<=45)tmp=2; 
	 //if(second>0&&second<=15)tmp=1;  
		 
	 if(second>900&&second<2700)tmp=2; 
	 if(second>0&&second<900)tmp=1; 	 
	 if(second==0&&tmp==1)
   {
	  LED1=0;LED2=1;LED3=1;LED4=1;LED5=1;HOT=0;UL=0;tmp=0;
	  fmsound(1);
	 }		

	 
   while(an&&power)
	 {
	  if(tmt<500&&tmp==0){LED1=0;LED2=1;LED3=1;LED4=1;LED5=1;}
	  if(tmt<500&&tmp==1){LED1=1;LED2=0;LED3=1;LED4=1;LED5=1;}
	  if(tmt<500&&tmp==2){LED1=1;LED2=1;LED3=0;LED4=1;LED5=1;}	
	  if(tmt<500&&tmp==3){LED1=1;LED2=1;LED3=1;LED4=0;LED5=1;}
	  if(tmt>=500&&tmp==0)
    {
	   LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;
		 LED1=0;Delay500ms();
	   LED1=1;Delay500ms();
		 LED1=0;Delay500ms();
	   LED1=1;Delay500ms();
	   LED1=0;Delay500ms();
	   LED1=1;Delay500ms();	
		 LED1=0;LED2=1;LED3=1;LED4=1;LED5=1;HOT=0;UL=0;an=0;
		}
	  if(tmt>=500&&tmp==1)
    {
	   LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;
		 LED2=0;Delay500ms();
	   LED2=1;Delay500ms();
		 LED2=0;Delay500ms();
	   LED2=1;Delay500ms();
	   LED2=0;Delay500ms();
	   LED2=1;Delay500ms();				
		 LED1=1;LED2=0;LED3=1;LED4=1;LED5=1;HOT=1;HOT=1;FAN=1;UL=1;hotfan=1;an=0;
		}
	  if(tmt>=500&&tmp==2)
    {
	   LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;
		 LED3=0;Delay500ms();
	   LED3=1;Delay500ms();
		 LED3=0;Delay500ms();
	   LED3=1;Delay500ms();
	   LED3=0;Delay500ms();
	   LED3=1;Delay500ms();					
		 LED1=1;LED2=1;LED3=0;LED4=1;LED5=1;HOT=1;HOT=1;FAN=1;UL=1;hotfan=1;an=0;
		}	
	  if(tmt>=500&&tmp==3)
    {
	   LED1=1;LED2=1;LED3=1;LED4=1;LED5=1;
		 LED4=0;Delay500ms();
	   LED4=1;Delay500ms();
		 LED4=0;Delay500ms();
	   LED4=1;Delay500ms();
	   LED4=0;Delay500ms();
	   LED4=1;Delay500ms();		
		 LED1=1;LED2=1;LED3=1;LED4=0;LED5=1;HOT=1;HOT=1;FAN=1;UL=1;hotfan=1;an=0;
		}		  
	 }		 
	}
 }
}

/***********系统初始化***********/
void SysInit()
{
 P0M0 = 0Xff;P0M1 = 0X00;
 P2M0 = 0Xff;P2M1 = 0X00;
 P3M0 = 0X00;P3M1 = 0X00;
 P3PU &= ~0x08; 
 LED1=1;LED2=1;LED3=1;
 LED4=1;LED5=1;FM=1;
 HOT=0;FAN=0;UL=0;
 power=0;hotfan=0;
 millisecond=0;
 second=0;time2=0;
 dat=0;tmp=0;an=0;
 Timer0_Init();
 Int0_Init();
 Int1_Init();
 EA = 1; 
}


/**********  定时器0初始化函数  *********/
void Timer0_Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80; //定时器时钟1T模式
	TMOD &= 0xF0; //设置定时器模式
	TL0 = 0xCD;		//设置定时初始值
	TH0 = 0xD4;		//设置定时初始值
	TF0 = 0;			//清除TF0标志
	TR0 = 1;			//定时器0开始计时
	ET0 = 1;      //使能定时器中断
	PT0 = 1;      //定时器0高优先级          
}

/**********  外部中断0初始化函数  *********/
void Int0_Init(void)
{
 IT0=1;//INT0下降沿有效
 EX0=1;//使能INT0中断
}
/**********  外部中断0初始化函数  *********/
void Int1_Init(void)
{
 IT1=1;//INT1下降沿有效
 EX1=1;//使能INT1中断
}
/**********  蜂鸣器  *********/
void fmsound(u8 dt)
{
 u8 i;
 for(i=0;i<dt;i++)
 {
	FM=0;
	Delay100ms();Delay100ms();
	FM=1;
	Delay100ms();Delay100ms();	
 }
}
/**********  100ms延时程序  *********/
void Delay100ms()		//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	i = 6;
	j = 157;
	k = 59;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay500ms()
{
 Delay100ms();
 Delay100ms();
 Delay100ms();		
 Delay100ms();
 Delay100ms();
}