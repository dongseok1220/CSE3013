#include "Header.h"

int check_num (int *temp, int t, int *num_temp)	// 0~9������ ������ ���� �Լ��̴�.
{
	char check[11]=""; 	// check �� �迭 ���� �ʱ�ȭ ���ش�
	int *book=temp; // bookd�� ���ο��� �� temp ���� ����ڰ� �Է��� �������� ����Ǿ��ִ�.
	int i,j,k; // ����� ���� ����
	int *num=num_temp; // num_temp�� ����i�� ������ �����ϴ� �迭�̴�.
	for (i=1;i<=book[t];i++) // for ���� 1 ���� ����ڰ� �Է��� ���������� ������.
	{
		sprintf(check, "%d", i); // ���ڸ� ���ڷ� ��ȯ�ϴ� �۾��̴�.
		k=strlen(check); // ���ڸ� ���ڷ� ��ȯ�ϰ� strlen�� �̿��� �ڸ��� Ȯ���Ѵ�.

		for (j=0;j<k;j++) // 0���� k-1��° �ε������� for��
		{
			switch (check[j])  // switch ���� ���� 0~9���� i�� ���� ���ڰ� ������ num[i] �� ++ ���ش�.
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
