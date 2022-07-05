#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void main()
{
	int ca;

	scanf("%d",&ca);

	int *book=(int *)malloc(sizeof(int) * ca);

	int i,j,k;
	
	for (i=0;i<ca;i++)
	{
		scanf("%d",&book[i]);
	}

	char check[11]=""; // 숫자를 받아서 문자로 변환 
	int num[10]={0, }; // 0~9까지 몇개인지
	
	int t=0;

	while (t!=ca) {
	for (i=1;i<=book[t];i++)
	{
		sprintf(check, "%d", i);
		k=strlen(check);
		for (j=0;j<k;j++) 
		{
			switch (check[j]) 
			{
				case '0' :
					num[0]++; break;
				case '1' :
					num[1]++; break;
				case '2' :
					num[2]++; break;
				case '3' :
					num[3]++; break;
				case '4' :
					num[4]++; break;
				case '5' :
					num[5]++; break;
				case '6' :
					num[6]++; break;
				case '7' :
					num[7]++; break;
				case '8' :
					num[8]++; break;
				case '9' :
					num[9]++; break;
				default :
					break;
			}
		} 
	}

	for (i=0;i<10;i++)
	{
		printf("%d ",num[i]);
		num[i]=0;
	}
	printf("\n");
	t++;

	}
	free(book);
}

