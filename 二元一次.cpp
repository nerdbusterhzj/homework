#include<stdio.h>
 double func(double a1,double a2,double b1,double b2,double c1,double c2);
 int main()
 {
 	double a1,a2,b1,b2,c1,c2,x,y;
 	printf("������a1 :");
	scanf("%lf",&a1);
    printf("������a2 :");
	scanf("%lf",&a2);
    printf("������b1 :");
	scanf("%lf",&b1);
    printf("������b2 :");
	scanf("%lf",&b2);
	printf("������c1 :");
	scanf("%lf",&c1);
	printf("������c2 :");
	scanf("%lf",&c2);
	y=func(a1,a2,b1,b2,c1,c2);
	x=(c1-b1*y)/a1;
	printf("y= %lf.\n",y);
	printf("x= %lf.\n",x);
	return 0;
 }
 double func(double a1,double a2,double b1,double b2,double c1,double c2)
{
	double z;
	z=(a1*c2-c2*a1)/(a2*b1-b2*a1);
	return z;	
}    
