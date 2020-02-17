#include <stdio.h>

int fun(void)
{
	int a = 1, b = 2;
	if(a > b)
	{
		return 3;
	}
	else
	{
		return 4;
	}
}

int main(void)
{
	fun();
	return 0;
}
