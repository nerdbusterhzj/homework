#include<stdio.h>
int sushu(int a)
{
	int b,j=0,y=0;
	if(a==1||a==0) y=0;
	else if(a>1)
	{
	 for(b=2;b<a;b++)
	 {
	 	if(a%b==0) j++;
	 }
	if(j==0) y=1;	 	
    }
    return y;
 } 
 int main()
 {
 	int a,b,c,n1,n2,n3,z1,z2,z3,z4,z5,z6,z7,s=0,d=0,k=0;
 	for(int i=100;i<=999;i++)
 	{
 		a=i/100;
 		b=(i%100)/10;
 		c=i%10;
 		n1=a+b+c;
 		n2=a*b*c;
 		n3=a*a+b*b+c*c;
 		z1=sushu(i);
 		z2=sushu(n1);
 		z3=sushu(n2);
 		z4=sushu(n3);
 		if(z1==1&&z2==1&&z3==1&&z4==1)
 		{
 			s++;
 			d=d+i;
 			if(s==3) k=i;
		 }
	}
 	printf("%d\n%d\n%d\n",s,d,k);
 	return 0;
 }
