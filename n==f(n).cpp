#include<stdio.h>
int main()
{
	int a,d=0;
	for(a=1;a<10000000;a++)
	{
	     int i,j=1,k=1,s=0,n=0;
	     i=a;
		 while(i/k!=0)
	   {
		if(i/k>0) s++;
		k=k*10;
	   }
	    for(int a=1;a<s;a++)
	   {
		j=j*10;
	   }
	    while(j>=1)
	   {
		if(i/j==1) n++;
		i=i%j;
		j=j/10;
	   }
	    d=d+n;
	    if(d==a) printf("%d ",d);
	}
}
