#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

void update_field(int** map, int h, int w, int change, int standard) // 줄을 합칠 때 집합 set 같게 만들어주는 함수
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
	fprintf(fp, "\n"); // 미로의 맵

	int ran = rand() % 2; // 랜덤하게 택하기 위한 변수
	int flag = FALSE; // 각 집합 set은 적어도 1개이상의 수직경로가 존재해야하므로 flag를 통해 확인 
	int check_flag = -1;

	for (int i = 0; i < h; i++) {
		if (i == 0) fprintf(fp, "| ");
		else fprintf(fp, "+");
		for (int j = 0; j < w; j++) {
			ran = rand() % 2;
			if (i == 0) { // 첫시행시 set 랜덤으로 합치기 
				if (map[i][j] != map[i][j + 1] && j + 1 < w) {
					if (ran == TRUE) {
						map[i][j + 1] = map[i][j];
						fprintf(fp, "  ");
					}
					else fprintf(fp, "| ");
				}
			}
			else {
				if (ran == TRUE) { // true일 경우 수직경로 생성 
					map[i][j] = map[i - 1][j];
					flag = TRUE;
					fprintf(fp, " +");
				}
				else if (map[i - 1][j] != map[i - 1][j + 1] && flag == FALSE) { // 수직경로가 아직 생성되지는 않았는데 다음 set과 현재 set이 다를 경우
					map[i][j] = map[i - 1][j];
					fprintf(fp, " +");
				}
				else if (j + 1 == w - 1 && flag == FALSE) { // 마지막 칸에 왔는데도 아직 수직경로가 없을 경우
					map[i][j] = map[i - 1][j];
					fprintf(fp, " +");
				}
				else fprintf(fp, "-+");
			}
			if (flag == TRUE && map[i][j] != map[i][j + 1]) flag = FALSE; // flag 초기화
		}
		flag = FALSE;

		if (i != 0) {
			fprintf(fp, "\n");
			fprintf(fp, "| ");
			for (int k = 0; k < w - 1; k++)
			{
				int ran = rand() % 2;

				if (i == h - 1) { // 마지막줄은 인접한 셀의 set이 다르면 무조건 합치므로 따로 조건
					if (map[i][k] != map[i][k + 1]) {
						fprintf(fp, "  ");
						update_field(map, i, w, map[i][k + 1], map[i][k]); // 합칠 때 같은 set의 셀 모두 변경 
					}
					else fprintf(fp, "| ");
				}
				else {
					if (map[i][k] != map[i][k + 1] && ran == TRUE) { // 인접한 셀의 set이 다르고 ran이 true일 경우에만 합침
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
	fprintf(fp, "\n"); // 마지막 미로 맵

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
	free(map); // 메모리 해제 
	return 0;
}