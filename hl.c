#include <STC8.H>
#include <INTRINS.H>
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;


/*************	��������	**************/
sbit LED1=P2^0;  // ����LED1���͵�ƽ��Ч��ָʾʱ��0����
sbit LED2=P2^1;  // ����LED2���͵�ƽ��Ч��ָʾʱ��15����
sbit LED3=P2^2;  // ����LED3���͵�ƽ��Ч��ָʾʱ��45����
sbit LED4=P2^3;  // ����LED4���͵�ƽ��Ч��ָʾʱ��60����
sbit LED5=P2^4;  // ����LED5���͵�ƽ��Ч��ָʾ���󱨾�
sbit FM  =P2^5;  // ������������͵�ƽ��Ч��������ʾ
sbit HOT =P2^6;  // ���巢�ȹ�������ߵ�ƽ��Ч�����ȹ���
sbit FAN =P2^7;  // �������������ߵ�ƽ��Ч������
sbit UL  =P0^0;  // ��������ƣ��ߵ�ƽ��Ч�������ȹ�ͬʱ����
sbit PA = P3^2; // ����EC11��������A��������
sbit PN = P3^3; // ����EC11��������������
sbit PB = P3^4; // ����EC11��������B��������
/*************	���ر�������	**************/
bit power,hotfan,an;
char dat,tmp,tmp2;
u16 tmt;
u16 millisecond,second,time1,time2;
/*************	���غ�������	**************/
void SysInit();         //ϵͳ��ʼ��
void Timer0_Init(void);	//��ʱ��0��ʼ��
void Int0_Init(void);		//�ⲿ�ж�0��ʼ��
void Int1_Init(void);		//�ⲿ�ж�0��ʼ��
void fmsound(u8 dt);	  //����������
void Delay100ms(); 
void Delay500ms();
/******************************************/
/********  �ⲿ�ж�0�жϷ�����  *********/
void INT0_Isr() interrupt 0
{
 if(!power)return;
 //if(dat==0)tmt=0;	
 //if(tmt>=500)dat=0;//����2�븴λ
 if(PB)dat++;//��ת
 if(!PB)dat--;//��ת
 if(dat>=3) //��ת3������
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
 if(dat<=-3)//��ת3������	
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
/**********  ��ʱ��0�жϷ�����  *********/
void TM0_Isr() interrupt 1
{
 millisecond++;time1++;
 if(millisecond>1000)
 {
  if(second>0)second--;
  millisecond=0;
 }
 if(HOT==0&&hotfan==1)//���ȹܺ���ȼ�����30��
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
/********* �ⲿ�ж�1�жϷ�����  *********/
void INT1_Isr() interrupt 2
{
 time1=0;
 while(!PN)
 {
  if(power==0&&time1>500)//�ػ�״̬�°���0.5�뿪��
	{
	 power=1;
	 break;
	}
  if(power==1&&time1>3000)//����״̬�°���3��ػ�
  {
	 power=0;second=0;
	 break;	
	}
 }	 
}
/**********  ������  *****************/
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

/***********ϵͳ��ʼ��***********/
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


/**********  ��ʱ��0��ʼ������  *********/
void Timer0_Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80; //��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0; //���ö�ʱ��ģʽ
	TL0 = 0xCD;		//���ö�ʱ��ʼֵ
	TH0 = 0xD4;		//���ö�ʱ��ʼֵ
	TF0 = 0;			//���TF0��־
	TR0 = 1;			//��ʱ��0��ʼ��ʱ
	ET0 = 1;      //ʹ�ܶ�ʱ���ж�
	PT0 = 1;      //��ʱ��0�����ȼ�          
}

/**********  �ⲿ�ж�0��ʼ������  *********/
void Int0_Init(void)
{
 IT0=1;//INT0�½�����Ч
 EX0=1;//ʹ��INT0�ж�
}
/**********  �ⲿ�ж�0��ʼ������  *********/
void Int1_Init(void)
{
 IT1=1;//INT1�½�����Ч
 EX1=1;//ʹ��INT1�ж�
}
/**********  ������  *********/
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
/**********  100ms��ʱ����  *********/
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