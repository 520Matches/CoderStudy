#include <stdio.h>

int fun(void)
{
	int a = 1, b = 2;
	return (a > b) ? 3 : 4;
}

int main(void)
{
	fun();
	return 0;
}
