#include<stdio.h>
int main()
{
	double num,nc;
	int na;
	printf("����һ������"); 
	scanf("%lf",&num);
	na=num;
	nc=num-na;
	if(nc>0.5) printf("%d",na+1);
	else printf("%d",na);
	return 0; 
 } 
