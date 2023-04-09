#include <REGX52.H>
#include "UART.H"

#define Speed 25	//0~100  

sbit STATE = P1^7;
sbit LEFT = P2^1;
sbit MIDDLE = P2^0;
sbit RIGHT = P2^2;
sbit LF = P3^6;
sbit LB = P3^7;
sbit RF = P3^5;
sbit RB = P3^4;

unsigned char Mode = 0;

void Delayus(unsigned char n)		//@11.0592MHz
{
	unsigned char i;

	while(n--)
	{
		i = 2;
		while (--i);
	}
}

void Forward(void)
{
	LF = 1;
	LB = 0;
	RF = 1;
	RB = 0;
}

void Back(void)
{
	LF = 0;
	LB = 1;
	RF = 0;
	RB = 1;
}

void Left(void)
{
	LF = 0;
	LB = 1;
	RF = 1;
	RB = 0;
}

void Right(void)
{
	LF = 1;
	LB = 0;
	RF = 0;
	RB = 1;
}

void Stop(void)
{
	LF = 1;
	LB = 1;
	RF = 1;
	RB = 1;
}

//低速
void Forward_S(void)
{
	LF = 1;
	LB = 0;
	RF = 1;
	RB = 0;
	Delayus(Speed);
	LF = 1;
	LB = 1;
	RF = 1;
	RB = 1;
	Delayus(100-Speed);
}

void Back_S(void)
{
	LF = 0;
	LB = 1;
	RF = 0;
	RB = 1;
	Delayus(Speed);
	LF = 1;
	LB = 1;
	RF = 1;
	RB = 1;
	Delayus(100-Speed);
}

void Left_S(void)
{
	LF = 0;
	LB = 1;
	RF = 1;
	RB = 0;
	Delayus(Speed);
	LF = 1;
	LB = 1;
	RF = 1;
	RB = 1;
	Delayus(100-Speed);
}

void Right_S(void)
{
	LF = 1;
	LB = 0;
	RF = 0;
	RB = 1;
	Delayus(Speed);
	LF = 1;
	LB = 1;
	RF = 1;
	RB = 1;
	Delayus(100-Speed);
}

void Obstacle_Avoidance(void)
{
	unsigned int i;
	if(LEFT != 0 && MIDDLE != 0 && RIGHT != 0)  //无障碍
	{
		Forward_S();
	}
	if(LEFT == 0 && RIGHT != 0)  //左方有障碍
	{
		Right_S();
	}
	if(RIGHT == 0 && LEFT != 0)  //右方有障碍
	{
		Left_S();
	}
	if((RIGHT == 0 && LEFT == 0) || (MIDDLE == 0 && LEFT != 0 && RIGHT != 0))  //前方有障碍
	{
		for(i=0;i<50;i++)
		{
			Back_S();
		}
		for(i=0;i<60000;i++)
		{
			Left_S();
			if((RIGHT != 0 && LEFT != 0 && MIDDLE != 0) || Mode != 2)
			{
				break;
			}
		}
	}
}

void Following(void)
{
	if(LEFT != 0 && MIDDLE != 0 && RIGHT != 0)  //无目标
	{
		Stop();
	}
	if(LEFT == 0 && RIGHT != 0)  //左方有目标
	{
		Left_S();
	}
	if(RIGHT == 0 && LEFT != 0)  //右方有目标
	{
		Right_S();
	}
	if((RIGHT == 0 && LEFT == 0) || (MIDDLE == 0 && LEFT != 0 && RIGHT != 0))  //前方有目标
	{
		Forward_S();
	}
}

void main(void)
{
	UART_Init();
	while(1)
	{
		if(Mode == 1)  //遥控模式
		{
			while(1)
			{
				if(Mode != 1)
				{
					break;
				}
//				if(STATE == 0)
//				{
//					Stop();
//					break;
//				}
			}
		}
		if(Mode == 2)  //避障模式
		{
			while(1)
			{
				Obstacle_Avoidance();
				if(Mode != 2)
				{
					break;
				}
//				if(STATE == 0)
//				{
//					Stop();
//					Mode = 0;
//					break;
//				}
			}
		}
		if(Mode == 3)  //跟随模式
		{
			while(1)
			{
				Following();
				if(Mode != 3)
				{
					break;
				}
//				if(STATE == 0)
//				{
//					Stop();
//					Mode = 0;
//					break;
//				}
			}
		}
	}
}

void UART_Routine(void) interrupt 4
{
	char bluetooth = 0;
	EA = 0;
	if(RI == 1)
	{
		bluetooth = SBUF;
		RI = 0;
		switch(bluetooth)
		{
			case'5':Mode = 1;break;
			case'6':Mode = 2;break;
			case'7':Mode = 3;break;
		}
		if(Mode == 1)
		{
			switch(bluetooth)
			{
				case'0':Stop();break;
				case'1':Forward();break;
				case'2':Back();break;
				case'3':Left();break;
				case'4':Right();break;
			}
		}
	}
	EA = 1;
}
