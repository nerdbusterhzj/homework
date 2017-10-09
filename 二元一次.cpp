#include<stdio.h>
 double func(double a1,double a2,double b1,double b2,double c1,double c2);
 int main()
 {
 	double a1,a2,b1,b2,c1,c2,x,y;
 	printf("«Î ‰»Îa1 :");
	scanf("%lf",&a1);
    printf("«Î ‰»Îa2 :");
	scanf("%lf",&a2);
    printf("«Î ‰»Îb1 :");
	scanf("%lf",&b1);
    printf("«Î ‰»Îb2 :");
	scanf("%lf",&b2);
	printf("«Î ‰»Îc1 :");
	scanf("%lf",&c1);
	printf("«Î ‰»Îc2 :");
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
