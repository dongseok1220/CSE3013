#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

void update_field(int** map, int h, int w, int change, int standard) // ���� ��ĥ �� ���� set ���� ������ִ� �Լ�
{
	//for (int i=0; i<h;i++) 
	for (int j = 0; j < w; j++) {
		if (map[h][j] == change) map[h][j] = standard;
	}
}

int main(void)
{
	FILE* fp = fopen("maze.maz", "w+");

	srand(time(0));
	int h, w;
	scanf("%d", &w);
	scanf("%d", &h);

	int** map = malloc(sizeof(int*) * h);
	for (int i = 0; i < h; i++)
		map[i] = malloc(sizeof(int) * w);

	for (int i = 0; i < h; i++)
		for (int j = 0; j < w; j++)
			map[i][j] = (w + 1) * i + 1 + j;


	for (int i = 0; i < 2 * w + 1; i++)
	{
		if (i % 2) fprintf(fp, "-");
		else fprintf(fp, "+");
	}
	fprintf(fp, "\n"); // �̷��� ��

	int ran = rand() % 2; // �����ϰ� ���ϱ� ���� ����
	int flag = FALSE; // �� ���� set�� ��� 1���̻��� ������ΰ� �����ؾ��ϹǷ� flag�� ���� Ȯ�� 
	int check_flag = -1;

	for (int i = 0; i < h; i++) {
		if (i == 0) fprintf(fp, "| ");
		else fprintf(fp, "+");
		for (int j = 0; j < w; j++) {
			ran = rand() % 2;
			if (i == 0) { // ù����� set �������� ��ġ�� 
				if (map[i][j] != map[i][j + 1] && j + 1 < w) {
					if (ran == TRUE) {
						map[i][j + 1] = map[i][j];
						fprintf(fp, "  ");
					}
					else fprintf(fp, "| ");
				}
			}
			else {
				if (ran == TRUE) { // true�� ��� ������� ���� 
					map[i][j] = map[i - 1][j];
					flag = TRUE;
					fprintf(fp, " +");
				}
				else if (map[i - 1][j] != map[i - 1][j + 1] && flag == FALSE) { // ������ΰ� ���� ���������� �ʾҴµ� ���� set�� ���� set�� �ٸ� ���
					map[i][j] = map[i - 1][j];
					fprintf(fp, " +");
				}
				else if (j + 1 == w - 1 && flag == FALSE) { // ������ ĭ�� �Դµ��� ���� ������ΰ� ���� ���
					map[i][j] = map[i - 1][j];
					fprintf(fp, " +");
				}
				else fprintf(fp, "-+");
			}
			if (flag == TRUE && map[i][j] != map[i][j + 1]) flag = FALSE; // flag �ʱ�ȭ
		}
		flag = FALSE;

		if (i != 0) {
			fprintf(fp, "\n");
			fprintf(fp, "| ");
			for (int k = 0; k < w - 1; k++)
			{
				int ran = rand() % 2;

				if (i == h - 1) { // ���������� ������ ���� set�� �ٸ��� ������ ��ġ�Ƿ� ���� ����
					if (map[i][k] != map[i][k + 1]) {
						fprintf(fp, "  ");
						update_field(map, i, w, map[i][k + 1], map[i][k]); // ��ĥ �� ���� set�� �� ��� ���� 
					}
					else fprintf(fp, "| ");
				}
				else {
					if (map[i][k] != map[i][k + 1] && ran == TRUE) { // ������ ���� set�� �ٸ��� ran�� true�� ��쿡�� ��ħ
						fprintf(fp, "  ");
						update_field(map, i, w, map[i][k + 1], map[i][k]);
					}
					else fprintf(fp, "| ");
				}

			}
		}
		fprintf(fp, "| ");
		fprintf(fp, "\n");
	}



	for (int i = 0; i < 2 * w + 1; i++)
	{
		if (i % 2) fprintf(fp, "-");
		else fprintf(fp, "+");
	}
	fprintf(fp, "\n"); // ������ �̷� ��

	fclose(fp);
	/* for debuging

		for (int i = 0; i < 2 * w + 1; i++)
		{
			if (i % 2) printf("-");
			else printf("+");
		}
		printf("\n");

		int ran = rand() % 2;
		int flag = FALSE;
		int check_flag = -1;

		for (int i = 0; i < h; i++) {
			if (i==0) printf("| ");
			else printf("+");
			for (int j = 0; j < w; j++) {
				ran = rand()%2;
				if (i == 0) {
					if (map[i][j] != map[i][j + 1] && j + 1 < w) {
						if (ran == TRUE) {
							map[i][j + 1] = map[i][j];
							printf("  ");
						}
						else printf("| ");
					}
				}
				else {
					if (ran == TRUE ) {
						map[i][j] = map[i - 1][j];
						flag = TRUE;
						printf(" +");
					}
					else if (map[i-1][j] != map[i-1][j + 1] && flag == FALSE) {
						map[i][j] = map[i - 1][j];
						printf(" +");
					}
					else if (j + 1 == w - 1 && flag == FALSE) {
						map[i][j] = map[i - 1][j];
						printf(" +");
					}
					else printf("-+");
				}
				if (flag == TRUE && map[i][j] != map[i][j + 1]) flag = FALSE;
			}
			flag = FALSE;

			if (i != 0) {
				printf("\n");
				printf("| ");
				for (int k = 0; k < w - 1; k++)
				{
					int ran = rand() % 2;

					if (i == h - 1) {
						if (map[i][k] != map[i][k + 1] ) {
							printf("  ");
							update_feild(map, h, w, map[i][k + 1], map[i][k]);
						}
						else printf("| ");
					}
					else {
						if (map[i][k] != map[i][k + 1] && ran == TRUE ) {
							printf("  ");
							update_feild(map, h, w, map[i][k + 1], map[i][k]);
						}
						else printf("| ");
					}

				}
			}
			printf("|");
			printf("\n");
		}



		for (int i = 0; i < 2 * w + 1; i++)
		{
			if (i % 2) printf("-");
			else printf("+");
		}
		printf("\n");

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++)
				printf("%d ", map[i][j]);
			printf("\n");
		}
	*/

	for (int i = 0; i < h; i++)
		free(map[i]);
	free(map); // �޸� ���� 
	return 0;
}