#include "Header.h"

int check_num (int *temp, int t, int *num_temp)	// 0~9까지의 개수를 세는 함수이다.
{
	char check[11]=""; 	// check 의 배열 값을 초기화 해준다
	int *book=temp; // bookd에 메인에서 준 temp 대입 사용자가 입력한 페이지가 저장되어있다.
	int i,j,k; // 사용할 변수 선언
	int *num=num_temp; // num_temp는 숫자i의 개수를 저장하는 배열이다.
	for (i=1;i<=book[t];i++) // for 문을 1 부터 사용자가 입력한 페이지까지 돌린다.
	{
		sprintf(check, "%d", i); // 숫자를 문자로 변환하는 작업이다.
		k=strlen(check); // 숫자를 문자로 변환하고 strlen을 이용해 자릿수 확인한다.

		for (j=0;j<k;j++) // 0부터 k-1번째 인덱스까지 for문
		{
			switch (check[j])  // switch 문을 통해 0~9까지 i와 같은 문자가 있으면 num[i] 을 ++ 해준다.
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

	return 0;

}
