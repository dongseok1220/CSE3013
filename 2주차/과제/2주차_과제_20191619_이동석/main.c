#include "Header.h"

void main() // 메인 함수이다.
{
	int ca; // 입력할 케이스의 개수이다.
	scanf("%d",&ca);	//입력받는다.

	int *book=(int *)malloc(sizeof(int)*ca); // book에 케이스의 개수만큼만 동적할당한다.


	int num[10]={0, }; //num배열의 값을 0으로 초기화해준다.

	int i,j; // 포문에서 사용할 정수형 변수 i,j 이다.

	for (i=0;i<ca;i++) // 케이스의 개수만큼 for문으로 각각의 페이지를 입력받는다.
	{
		scanf("%d",&book[i]);
	}

	for (i=0;i<ca;i++) // 케이스만큼 check_num 함수를 돌리고, output함수로 출력한다.
	{
		check_num(book,i,num);
		output(num);
	}
	free(book); // 동적할당한 메모리를 해제한다.
}


