#include "Header.h"

void main() // ���� �Լ��̴�.
{
	int ca; // �Է��� ���̽��� �����̴�.
	scanf("%d",&ca);	//�Է¹޴´�.

	int *book=(int *)malloc(sizeof(int)*ca); // book�� ���̽��� ������ŭ�� �����Ҵ��Ѵ�.


	int num[10]={0, }; //num�迭�� ���� 0���� �ʱ�ȭ���ش�.

	int i,j; // �������� ����� ������ ���� i,j �̴�.

	for (i=0;i<ca;i++) // ���̽��� ������ŭ for������ ������ �������� �Է¹޴´�.
	{
		scanf("%d",&book[i]);
	}

	for (i=0;i<ca;i++) // ���̽���ŭ check_num �Լ��� ������, output�Լ��� ����Ѵ�.
	{
		check_num(book,i,num);
		output(num);
	}
	free(book); // �����Ҵ��� �޸𸮸� �����Ѵ�.
}


