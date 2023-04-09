#include <INTRINS.H>

/**
  * @brief 延时
  * @param n 延时时间 单位ms
  * @retval 无
  */
void Delay(unsigned int n)		//@11.0592MHz
{
	unsigned char i, j;
	while(n--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
			while (--j);
		} while (--i);
	}
}
