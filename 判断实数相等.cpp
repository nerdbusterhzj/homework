#include<stdio.h>
#include<math.h>
int main()
{
double a,b;
scanf("%lf%lf",&a,&b);
if(fabs(a-b)<1e-6) printf("a==b\n");
else printf("a!=b");	
 } 
