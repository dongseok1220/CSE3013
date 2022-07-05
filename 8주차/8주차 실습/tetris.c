#include "tetris.h"

static struct sigaction act, oact;
int count;

long evalSize(RecNode* root) {
	for (int i = 0; i < CHILDREN_MAX; i++) {
		if (root->c[i]->level < VISIBLE_BLOCKS)
			return sizeof(RecNode) + evalSize(root->c[i]);
	}
}

int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	time_t start, stop;
	double duration;
	
	srand((unsigned int)time(NULL));
	createRankList();
	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY:
			start = time(NULL);
			play(); 
			move(2,0);
			printw("The total memory for the list : ");
			printw("%ld bytes\n", evalSize(recRoot));
			stop = time(NULL);
			duration = (double)difftime(stop, start);
			printw("The total time for sorting : %lf seconds\n",duration);
			move(4,0);
			getch();
			break; 
		case MENU_RANK: rank(); break;
		case MENU_REC : recommendedPlay(); break;
		case MENU_EXIT: exit = 1; if (root !=NULL) Free_NODE(root); if (recRoot != NULL) Delete_recNode(recRoot); break;
		default: break;
		}
	}
	free(recRoot);

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {  // recRoot initiallize 해줘야함 

	int i, j;

	for (j = 0; j < HEIGHT; j++)
		for (i = 0; i < WIDTH; i++)
			field[j][i] = 0;

	nextBlock[0] = rand() % 7;
	nextBlock[1] = rand() % 7;
	nextBlock[2] = rand() % 7;
	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH / 2 - 2;
	score = 0;
	gameOver = 0;
	timed_out = 0;

	recRoot = malloc(sizeof(RecNode));
	recRoot->level = 0;
	recRoot->accumulatedscore = 0;
	recRoot->f = field;
	Init_recNode(recRoot);
	recommend(recRoot);


	DrawOutline();
	DrawField();
	DrawBlock(blockY, blockX, nextBlock[0], blockRotate, ' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline() {
	int i, j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0, 0, HEIGHT, WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2, WIDTH + 10);
	printw("NEXT BLOCK1");
	DrawBox(3, WIDTH + 10, 4, 8);

	move(9, WIDTH + 10);
	printw("NEXT BLOCK2");
	DrawBox(10, WIDTH + 10, 4, 8);
	/* 2번째 next block을 보여주는 공간의 테두리*/

	move(16, WIDTH + 10);
	printw("SCORE");
	DrawBox(17, WIDTH + 10, 1, 8);
	/* score를 보여주는 공간의 태두리를 그린다.*/
}

int GetCommand() {
	int command;
	command = wgetch(stdscr);
	switch (command) {
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command) {
	int ret = 1;
	int drawFlag = 0;
	switch (command) {
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if ((drawFlag = CheckToMove(field, nextBlock[0], (blockRotate + 1) % 4, blockY, blockX)))
			blockRotate = (blockRotate + 1) % 4;
		break;
	case KEY_DOWN:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX + 1)))
			blockX++;
		break;
	case KEY_LEFT:
		if ((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX - 1)))
			blockX--;
		break;
	default:
		break;
	}
	if (drawFlag) DrawChange(field, command, nextBlock[0], blockRotate, blockY, blockX);
	return ret;
}

void DrawField() {
	int i, j;
	for (j = 0; j < HEIGHT; j++) {
		move(j + 1, 1);
		for (i = 0; i < WIDTH; i++) {
			if (field[j][i] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score) {
	move(18, WIDTH + 11);
	printw("%8d", score);
}

void DrawNextBlock(int* nextBlock) {
	int i, j;
	for (i = 0; i < 4; i++) {
		move(4 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[1]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}

	for (i = 0; i < 4; i++) {
		move(11 + i, WIDTH + 13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID, int blockRotate, char tile) {
	int i, j;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
				move(i + y + 1, j + x + 1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT, WIDTH + 10);
}

void DrawBox(int y, int x, int height, int width) {
	int i, j;
	move(y, x);
	addch(ACS_ULCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for (j = 0; j < height; j++) {
		move(y + j + 1, x);
		addch(ACS_VLINE);
		move(y + j + 1, x + width + 1);
		addch(ACS_VLINE);
	}
	move(y + j + 1, x);
	addch(ACS_LLCORNER);
	for (i = 0; i < width; i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}

		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			newRank(score);
			refresh();
			getch();

			return;
		}
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu() {
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int w, h; // 변수 width , height 

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 4; w++)
		{
			// block[currentBlock][blockRotate][h][w] 를 체크 1이면 -> 이동해서 필드체크
			// 필드가 1이면 return 0
			if (block[currentBlock][blockRotate][h][w])
			{
				if (f[h + blockY][w + blockX]) return 0; // 아니라면 너비랑 높이 체크해줌
				else if (h + blockY >= HEIGHT || w + blockX < 0 || w + blockX >= WIDTH) return 0; // 벗어나면 return 0
			}
		}
	}
	return 1; // 위의 조건을 다 만족하지 않으면 move 가능 return 1; 
}


void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	// command 에는 KEY_UP KEY_DOWN KEY_RIGHT KEY_LEFT 각각을 입력받고 들어왔으므로 이전블록정보 찾을 수 있음 
	int w, h;
	int preRotate = (blockRotate + 3) % 4; // 이전 로테이트
	switch (command)
	{
	case KEY_UP: // rotate
		for (h = 0; h < 4; h++)
		{
			for (w = 0; w < 4; w++) // 기존 block 지우기 rotate는 0~3이다. (현재 rotate+3) % 4 = 이전로테이트
			{
				if (block[currentBlock][preRotate][h][w]) // 이게 1이면
				{
					move(blockY + h + 1, blockX + w + 1);
					printw(".");
				}
			}
		}
		break;

	case KEY_DOWN: // y-- 이므로 현재보다 위로 한칸
		for (h = 0; h < 4; h++)
		{
			for (w = 0; w < 4; w++)
			{
				if (block[currentBlock][blockRotate][h][w])
				{
					move(blockY + h, blockX + w + 1);
					printw(".");
				}
			}
		}
		break;

	case KEY_RIGHT: // x++ 이므로 현재보다 왼쪽 한칸
		for (h = 0; h < 4; h++)
		{
			for (w = 0; w < 4; w++)
			{
				if (block[currentBlock][blockRotate][h][w])
				{
					move(blockY + h + 1, blockX + w);
					printw(".");
				}
			}
		}
		break;

	case KEY_LEFT: // x-- 이므로 현재보다 오른쪽 한칸
		for (h = 0; h < 4; h++)
		{
			for (w = 0; w < 4; w++)
			{
				if (block[currentBlock][blockRotate][h][w])
				{
					move(blockY + h + 1, blockX + w + 2);
					printw(".");
				}
			}
		}
		break;

	default:
		break;
	}
	DrawField(); // 그림자초기화
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
}

void BlockDown(int sig) {
	// 내려갈 수 있는지 확인 
	timed_out = 0; // 0으로 초기화 
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) // 리턴값이 1이면 가능 
	{
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); // 내려서 그림
	}
	else if (blockY == -1) // 게임오버조건 -> 블록이 천장에 닿음  
	{
		gameOver = TRUE;
		return; //함수 종료하고 게임종료
	}
	else // 블록추가 (점수계산? 차후추가)
	{
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field); // 라인차면 삭제 
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2]; // 0~6값 
		nextBlock[2] = rand() % 7;
		blockRotate = 0; blockY = -1; blockX = WIDTH / 2 - 2; // 초기설정으로 초기화

		recommend(recRoot);

		DrawNextBlock(nextBlock); // 다음블럭
		DrawField(); // 화면 갱신
		PrintScore(score);
	}
	//강의자료 p26-27의 플로우차트를 참고한다.
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int w, h;
	int touched = 0;
	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 4; w++)
		{
			if (block[currentBlock][blockRotate][h][w]) // 블록을 쌓는다
			{
				if (f[blockY + h + 1][blockX + w] || h + blockY + 1 == HEIGHT) touched++;
				f[blockY + h][blockX + w] = 1;
			}
		}
	}
	return touched * 10;
	//Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	int w, h;
	int flag; // flag
	int line = 0; // 지울라인 개수
	for (h = HEIGHT - 1; h >= 0; h--) // 아래에서부터 탐색하므로 거꾸로 
	{
		flag = 1; // 초기화
		for (w = 0; w < WIDTH; w++)
		{
			if (f[h][w] != 1)
			{
				flag = 0;
				break; // 하나라도 1이아니면 break
			}
		}
		if (flag) // 현재 h에 지울라인이 있음
		{
			line++; // 라인개수 세주고 필드값 내리기
			for (int temp = h - 1; temp >= 0; temp--) // 맨 윗줄은 따로 0으로 초기화
			{
				for (w = 0; w < WIDTH; w++)
					f[temp + 1][w] = f[temp][w];
			}
			for (w = 0; w < WIDTH; w++)
				f[0][w] = 0;
			h++; // 그 줄부터 다시 검사해야함
		}

	}
	return line * line * 100; // 점수리턴 

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
}

///////////////////////////////////////////////////////////////////////////
void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	int count = y; // y좌표값 체크하는 변수
	while (CheckToMove(field, blockID, blockRotate, count + 1, x)) // 한칸씩 아래로 내려갈 수 있는지 확인한다. 
	{
		count++;
	}
	DrawBlock(count, x, blockID, blockRotate, '/');
}
/* rank.txt에서 랭킹정보 읽고, 랭킹목록을 생성 함 */
void createRankList() {
	FILE* fp;
	int i, j;

	fp = fopen("rank.txt", "r");
	if (!fp) {
		fp = fopen("rank.txt", "w");
		return;
	}
	int num; // 랭크 개수
	fscanf(fp, "%d", &num);
	count = num; // count 는 전역변수로서 파일에 들어있는 노드의 개수

	if (!num) {
		printf("Empty");
		return;
	}

	int i_temp[count];
	char c_temp[NAMELEN]; // 임시변수 

	/* 기본 아이디어
	1. count만큼 랜덤한 중복되지 않는 숫자로 이진 tree 생성  -> 작다면 왼쪽 크다면 오른쪽 (rank를 나타내기 때문에)
	2. 이후 파일을 읽으며 1~count 까지 할당
	*/

	for (i = 0; i < count; i++)
		i_temp[i] = i + 1; // 배열에 1~ count 까지 할당 

	int ran;

	if (root == NULL) {
		root = malloc(sizeof(NODE));
		root->left = NULL; root->right = NULL;
		root->rank = i_temp[num / 2];
		swap(i_temp[num - 1], i_temp[num / 2]);
		num--;
	}
	for (i = 0; i < count - 1; i++)
	{
		ran = rand() % num; // 0~num-1 까지  
		Insert_NODE(root, i_temp[ran]);
		swap(i_temp[num - 1], i_temp[ran]);
		// 정렬된 배열에서 
		num--;
	}	// 1~count 까지 트리생성

	// 생성된 트리에 파일의 값 할당
	int t_data;

	for (i = 1; i <= count; i++)
	{
		fscanf(fp, "%s %d", c_temp, &t_data);
		Insert_FILE(root, t_data, c_temp, i);
	}

	fclose(fp);
}

NODE* Insert_NODE(NODE * node, int data) // struct 구조 rank에값을 항당 -> 처음 리스트를 만들때만사용 이후 사용x
{
	if (node == NULL) {
		NODE* temp = malloc(sizeof(NODE));
		temp->rank = data;
		temp->left = NULL; temp->right = NULL;
		return temp;
	} // 없으면 생성

	if (data < node->rank)
		node->left = Insert_NODE(node->left, data);
	else if (data > node->rank)
		node->right = Insert_NODE(node->right, data);
	return node;
}

NODE* Insert_FILE(NODE * node, int data, char* name, int rank)
{
	if (rank == node->rank) {
		node->data = data;
		strcpy(node->name, name);
	}
	else if (rank < node->rank) // rank 가 더 높다 -> 왼쪽으로
		node->left = Insert_FILE(node->left, data, name, rank);
	else if (rank > node->rank)  // rank가 더 작다 -> 오른쪽
		node->right = Insert_FILE(node->right, data, name, rank);

	return node;
}

void rank() {
	int X = 1, Y = 0, ch, i, j;
	clear();
	noecho();
	printw("1. list ranks form X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	ch = getch();
	echo();
	int check = 1;

	if (ch == '1') {
		printw("X: ");
		int check_x = scanw("%d\n", &X);
		if (check_x != 1) X = 1;
		printw("Y: ");
		int check_y = scanw("%d\n", &Y);
		if (check_y != 1) Y = count; // 혹은 count까지 
		printw("\tname\t|   score\n");
		printw("----------------------------\n");
		if (X > Y) {
			printw("\nsearch failure: no rank in the list\n");
		}

		NODE* curr = root;
		/*	for (i=X-1; i<Y; i++)
			{
			if (curr->right !=NULL) printw("%-16s|%-d\n", curr->right->name,curr->right->data);
			printw("%-16s|%-d\n",curr->name,curr->data);
			curr= curr->left;
			} */
		print_list(curr, X, Y, &check);
	}
	else if (ch == '2') {
		int ex = 0; // 존재하는지 체크하는 변수 
		printw("Input the name: ");
		char name[NAMELEN]; // 이름 입력받을 변수 선언 
		int check_name = scanw("%s", name);
		printw("\tname\t|   score\n");
		printw("----------------------------\n");
		print_list_2(root, name, &ex);
		if (!ex) printw("\nsearch failure: no name in the list\n");
	}
	else if (ch == '3') {
		printw("Input the rank: ");
		int d_rank; // 삭제할 rank
		int c_rank = scanw("%d", &d_rank);
		if (c_rank != 1) {
			printw("Nothing deleted\n");
		}
		else if (d_rank > count || d_rank <= 0)
			printw("\nsearch failure: the rank not in the list");
		else {
			root = Delete(root, d_rank);
			count--;
			printw("\nresult: the rank deleted\n");
			//printw("%s %d",root->name, root->data);	
			writeRankFile(root, 1); // 삭제 반영 
		}
	}
	check = 1;
	getch();
}

void writeRankFile(NODE * root, int first) {
	if (first == 1) {
		FILE* remove = fopen("rank.txt", "w+");
		fprintf(remove, "%d\n", count);
		fclose(remove);
	}
	FILE* fp = fopen("rank.txt", "a+");

	echo();

	if (root != NULL) {
		writeRankFile(root->left, first + 1);
		//	printw("%s %d\n", root->name, root->data);	
		fprintf(fp, "%s %d\n", root->name, root->data);
		fclose(fp);
		writeRankFile(root->right, first + 1);
	}
}

void newRank(int score) {
	clear();
	move(0, 0);
	// char str[NAMELEN+1];
	int i, j;
	echo();
	printw("your name: ");
	NODE* newNODE = (NODE*)malloc(sizeof(NODE));
	// scanf("%s",newNODE->name);
	getstr(newNODE->name);
	newNODE->data = score;
	newNODE->left = NULL; newNODE->right = NULL;
	count++;

	if (root == NULL) {
		root = newNODE;
	}
	else {
		NODE* curr = root;
		for (i = 0; i < count; i++)
		{
			if (curr->data >= newNODE->data) {
				if (curr->right == NULL) {
					curr->right = newNODE;
					break;
				}
				else curr = curr->right;
			}
			else if (curr->data < newNODE->data) {
				if (curr->left == NULL) {
					//printw("INSERT in %d next %d",i,curr->data);
					curr->left = newNODE;
					break;
				}
				else curr = curr->left;
			}
		}

	}
	writeRankFile(root, 1);
}

void print_list(NODE * root, int X, int Y, int* check)
{
	if (root == NULL) return;
	if (root != NULL) {
		print_list(root->left, X, Y, check);
		if (*check >= X && *check <= Y)
			printw("%-16s| %-d\n", root->name, root->data);
		(*check)++;
		print_list(root->right, X, Y, check);
	}
}

void print_list_2(NODE * root, char* name, int* check)
{
	if (root == NULL) return;
	if (root != NULL) {
		print_list_2(root->left, name, check);
		if (!strcmp(name, root->name)) {
			printw("%-16s| %-d\n", root->name, root->data);
			*check = 1;  // 같으면 출력 
		}
		print_list_2(root->right, name, check);
	}
}

NODE* Delete_NODE(NODE * r_node, int d_rank, int* check) // 해당 rank의 주소 반환 
{
	NODE temp_node;  // 삭제하고 싶은 노드의 정보 저장 
	NODE* d_node = root; // 위의 정보를 바탕으로 삭제할 노드 저장 
	NODE* p = NULL; // 위 노드의 부모노드 
	NODE* child; // 자식노드가 있을 때 사용할 노드들 

	find_rank(root, d_rank, check, &temp_node);
	*check = 1;

	while (d_node != NULL)
	{
		if (strcmp(d_node->name, temp_node.name) == 0 && temp_node.data == d_node->data) break;
		p = d_node;
		if (d_node->data < temp_node.data) {
			d_node = d_node->left;
		}
		else if (d_node->data >= temp_node.data) {
			d_node = d_node->right;
		}
	}

	//	printw("%s %d\n", p->name, p->data);
	//	printw("%s %d\n", d_node->name, d_node->data);

	if (d_node == NULL) return root;

	if (d_node->left == NULL && d_node->right == NULL) // 단말노드
	{
		if (p != NULL) {
			if (p->left == d_node) p->left = NULL;
			else p->right = NULL;
		}
		else root = NULL;
	}
	else if (d_node->left == NULL || d_node->right == NULL) // 자식노드가 한개 
	{
		if (d_node->left != NULL) child = d_node->left;
		else child = d_node->right;

		if (p != NULL) {
			if (p->left == d_node) p->left = child;
			else p->right = child;
		}
		else root = child;
	}
	else { // 자식노드가 두개
		p = d_node;
		child = d_node->left;

		while (child->right != NULL)
		{
			p = child;
			child = child->right;
		}
		if (p->left == child) p->left = child->left;
		else p->right = child->left;

		d_node->data = child->data;
		strcpy(d_node->name, child->name);
		d_node = child;
	}

	free(d_node); // 메모리해제 


	return root;

}
NODE* Delete(NODE * root, int d_rank)
{
	int check = 1;
	return Delete_NODE(root, d_rank, &check);	
}

NODE* find_rank(NODE * root, int d_rank, int* check, NODE * temp)
{
	if (root == NULL) {
	}
	if (root != NULL) {
		find_rank(root->left, d_rank, check, temp);
		if (*check == d_rank) {
			(*check)++;
			strcpy(temp->name, root->name);
			temp->data = root->data;
		}
		(*check)++;
		find_rank(root->right, d_rank, check, temp);
	}
}

NODE* Free_NODE(NODE * root)
{
	if (root != NULL) {
		Free_NODE(root->left);
		Free_NODE(root->right);
		free(root);
	}
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

void Init_recNode(RecNode * recRoot) // 자식노드를 childerenmax만큼 생성 
{
	RecNode** child = recRoot->c;
	for (int i = 0; i < CHILDREN_MAX; i++) {
		*(child + i) = malloc(sizeof(RecNode));
		child[i]->level = recRoot->level + 1; // depth 1증가
		child[i]->f = (char(*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
		if (child[i]->level < VISIBLE_BLOCKS)  // visible blocks 수보다 작을 때 까지
			Init_recNode(child[i]); // 자식노드의 자식노드 생성		
	}
}

int recommend(RecNode * root) {
	int max = 0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	int b_rotate, rec_x, rec_y;; // block rotat, recommend x,y 좌표
	int i = 0; // child 노드 관련 변수 
	int h, w;
	int depth = root->level + 1; // 계속 ++ 해줌 -> VISIBLE_BLOCKS 와 비교 할 변수 
	int score; // score저장할 임시변수 
	int X, Y, R; // x,y, rotete 저장할 임시변수 
	for (b_rotate = 0; b_rotate < 4; b_rotate++) // rotate
	{
		for (rec_x = -2; rec_x < WIDTH; rec_x++) // x좌표 이동 
		{
			rec_y=0; score=0; 
			if (CheckToMove(root->f, nextBlock[depth - 1], b_rotate, -1, rec_x)) { // 움직일 수 있다면 
				for (h = 0; h < HEIGHT; h++)
					for (w = 0; w < WIDTH; w++)
						root->c[i]->f[h][w] = root->f[h][w]; // 자식노드에 필드정보 복사

				while (CheckToMove(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y + 1, rec_x)) // 현재 필드에서 내릴수 있는만큼 아래로 
					rec_y++; 

		 		root->c[i]->accumulatedscore = root->accumulatedscore; // root 의 score를 처음에 할당 후 점수 누적 
				root->c[i]->accumulatedscore += AddBlockToField(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y, rec_x);
				root->c[i]->accumulatedscore += DeleteLine(root->c[i]->f);

				if (depth < VISIBLE_BLOCKS) score = recommend(root->c[i]); // 아직 다 안돌았으면 재귀적으로 다시돈다. 
				else score = root->c[i]->accumulatedscore; // 다 확인했을 경우 score를 저장  

				if (score > max) { // 더 좋은 점수를 내는 노드를 찾았을 경우 그 노드의 rec_x, rec_y , b_rotate로 바꿔주기
					max = score;
					X = rec_x;
					Y = rec_y;
					R = b_rotate; // pruning을 하기 위해서는 하나의 depth를 확인하고 가장큰 점수만 선택 
				}
				i++; // childe 노드 변수 증가 
			}
		}
	}
	if (root->level == 0) { // 모든 노드를 확인했을 경우 전역변수에 저장해놓았던 변수를 대입해줌 
		recommendX = X; recommendY = Y; recommendR = R;
	}
	return max;
}

void Delete_recNode(RecNode * root) // 만들었던 노드들 삭제 
{
	for (int i = 0; i < CHILDREN_MAX; i++) {
		if (root->c[i]->level < VISIBLE_BLOCKS)
			Delete_recNode(root->c[i]); // 가장 아래 자식노드부터 차례로 삭제해야함
		free(root->c[i]->f);
		free(root->c[i]);
	}
}
void recommendedPlay() {
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}
		blockX = recommendX;
		blockY = recommendY;
		blockRotate = recommendR;
		if (CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX) == 0) gameOver = 1;
		command = GetCommand();
		
	} while (!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}
