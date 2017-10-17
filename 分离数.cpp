#include<stdio.h>
int main()
{
	double num,nc;
	int na;
	printf("输入一个数："); 
	scanf("%lf",&num);
	na=num;
	nc=num-na;
	printf("整数部分为%d,小数部分为%lf",na,nc);
	return 0;
 } 
