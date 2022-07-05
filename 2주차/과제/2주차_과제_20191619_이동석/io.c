#include "Header.h"

int output(int *num_temp)  // 계산한 0~9까지 숫자의 개수를 출력하기 위한 함수이다.
{
	int i; // 정수형 변수 선언
	int *num=num_temp; // 0~9 까지의 개수가 들어있는 배열

	for (i=0;i<10;i++) // for 문을 통해 num[i]의 값을 출력해주고, num[i]를 0으로 초기화해준다.
	{
		printf("%d ", num[i]);
		num[i]=0;
	}
	printf("\n");

	return 0;
}


