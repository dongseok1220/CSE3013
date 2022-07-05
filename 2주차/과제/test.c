#include<stdio.h>
#include<stdlib.h>

int test(int *temp)
{
	int *arr=temp;
	printf("%d",*arr);
	printf("%d",arr[1]);
}

void main()
{
	int *a=(int *)malloc(sizeof(int) * 3);
	a[0]=1;
	a[1]=2;
	test(a);
	free(a);
}
