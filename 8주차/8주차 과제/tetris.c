#include "tetris.h"
#include "rank.h"
#include<math.h>

static struct sigaction act, oact;
float a, b, c, d, e;

long evalSize(RecNode* root) {
	for (int i = 0; i < CHILDREN_MAX; i++) {
		if (root->c[i]->level < VISIBLE_BLOCKS)
			return sizeof(RecNode) + evalSize(root->c[i]);
	}
}

float CheckHoles(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int w, h;
	float weight = 0;
	float holes = 0;
	int i = 1;
	int flag = 0;
	float ave = 0;
	int check=0;
	float high = 0;
	int highest=0, lowest=0;
	float touched=0, b_touched=0, s_touched=0;

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 4; w++)
		{
			if (block[currentBlock][blockRotate][h][w]) 
			{
				if ((f[h + blockY + 1][w + blockX] == 1)) touched++;
				if ((h + blockY + 1 == HEIGHT)) b_touched++;
				if ((w + blockX  == 0 || w + blockX + 1 == WIDTH)) s_touched++;
				if ((f[h + blockY][w + blockX - 1] == 1 || f[h + blockY][w + blockX + 1] == 1)) touched++;

				f[blockY + h][blockX + w] = 1;
			}
		}
	}
	for (w = 0; w < WIDTH; w++)
		for (h = 0; h < HEIGHT; h++) {
			if (f[h][w] == 1) {
				if (highest < HEIGHT -h + 1) highest = HEIGHT -h +1;
				if (lowest > HEIGHT -h + 1) lowest = HEIGHT -h +1; 
				high += HEIGHT - h;
				ave += HEIGHT - h + 1 - check;
				check = HEIGHT - h + 1;
				if (ave < 0) ave = -ave;
				break;
			}
		}
	for (w=0; w<WIDTH; w++){
		for (h=0; h<HEIGHT;h++) {
			if (f[h][w] == 1) {
				while (1) {
					if (h+i >= HEIGHT ) break;
					if (f[h+i][w] ==0 ) holes = holes + 1;
					i++;
				}
				i=1;
				break;
			}
		}
	}

	return -(3.3423)*high + -(8.8249)*holes + (3.8273)*touched + (2.215)*s_touched + (4.8305)*b_touched -(HEIGHT-blockY -1) -ave; 
}
int main() {
	int exit = 0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);

	createRankList();
	while (!exit) {
		clear();
		switch (menu()) {
		case MENU_PLAY:
			play();
			break;
		case MENU_RANK: rank(); break;
		case MENU_REC:
			recommendedPlay();
			if (recRoot != NULL) Delete_recNode(recRoot);
			break;
		case MENU_EXIT: exit = 1; if (root != NULL) Free_NODE(root);  break;
		default: break;
		}
	}
	free(recRoot);

	endwin();
	system("clear");
	return 0;
}

void InitTetris() {  

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
	Init_recNode(recRoot);;
	modified_recommend(recRoot);
	//recommend(recRoot);


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



int CheckToMove(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int w, h; 

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 4; w++)
		{
			
			
			if (block[currentBlock][blockRotate][h][w])
			{
				if (f[h + blockY][w + blockX]) return 0; 
				else if (h + blockY >= HEIGHT || w + blockX < 0 || w + blockX >= WIDTH) return 0; 
			}
		}
	}
	return 1; 
}


void DrawChange(char f[HEIGHT][WIDTH], int command, int currentBlock, int blockRotate, int blockY, int blockX) {
	
	int w, h;
	int preRotate = (blockRotate + 3) % 4; 
	switch (command)
	{
	case KEY_UP: 
		for (h = 0; h < 4; h++)
		{
			for (w = 0; w < 4; w++) 
			{
				if (block[currentBlock][preRotate][h][w]) 
				{
					move(blockY + h + 1, blockX + w + 1);
					printw(".");
				}
			}
		}
		break;

	case KEY_DOWN: 
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

	case KEY_RIGHT: 
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

	case KEY_LEFT: 
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
	DrawField(); 
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	
	
	
}

void BlockDown(int sig) {
	
	timed_out = 0; 
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY + 1, blockX)) 
	{
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); 
	}
	else if (blockY == -1) 
	{
		gameOver = TRUE;
		return; 
	}
	else 
	{
		//score += CheckHoles(field, nextBlock[0],blockRotate, blockY,blockX);
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field); 
		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2]; 
		nextBlock[2] = rand() % 7;
		blockRotate = 0; blockY = -1; blockX = WIDTH / 2 - 2; 

		//modified_recommend(recRoot);
		recommend(recRoot);
		DrawNextBlock(nextBlock); 
		DrawField(); 
		PrintScore(score);
	}
	
}

int AddBlockToField(char f[HEIGHT][WIDTH], int currentBlock, int blockRotate, int blockY, int blockX) {
	int w, h;
	int touched = 0;
	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 4; w++)
		{
			if (block[currentBlock][blockRotate][h][w]) 
			{
				if (f[blockY + h + 1][blockX + w] || h + blockY + 1 == HEIGHT) touched++;
				f[blockY + h][blockX + w] = 1;
			}
		}
	}
	return touched * 10;
	
}

int DeleteLine(char f[HEIGHT][WIDTH]) {
	int w, h;
	int flag; 
	int line = 0; 
	for (h = HEIGHT - 1; h >= 0; h--) 
	{
		flag = 1; 
		for (w = 0; w < WIDTH; w++)
		{
			if (f[h][w] != 1)
			{
				flag = 0;
				break; 
			}
		}
		if (flag) 
		{
			line++; 
			for (int temp = h - 1; temp >= 0; temp--) 
			{
				for (w = 0; w < WIDTH; w++)
					f[temp + 1][w] = f[temp][w];
			}
			for (w = 0; w < WIDTH; w++)
				f[0][w] = 0;
			h++; 
		}

	}
	return line * line * 100; 

	
	
}


void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
}

void DrawShadow(int y, int x, int blockID, int blockRotate) {
	int count = y; 
	while (CheckToMove(field, blockID, blockRotate, count + 1, x)) 
	{
		count++;
	}
	DrawBlock(count, x, blockID, blockRotate, '/');
}

void DrawRecommend(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

void Init_recNode(RecNode * recRoot) 
{
	RecNode** child = recRoot->c;
	for (int i = 0; i < CHILDREN_MAX; i++) {
		*(child + i) = malloc(sizeof(RecNode));
		child[i]->level = recRoot->level + 1; 
		child[i]->f = (char(*)[WIDTH])malloc(sizeof(char) * HEIGHT * WIDTH);
		if (child[i]->level < VISIBLE_BLOCKS)  
			Init_recNode(child[i]); 
	}
}

float recommend(RecNode * root) {
	float max = -100000; 

	int b_rotate, rec_x, rec_y;; 
	int i = 0; 
	int h, w;
	int depth = root->level + 1; 
	float score; 
	int X, Y, R; 
	for (b_rotate = 0; b_rotate < 4; b_rotate++) 
	{
		for (rec_x = WIDTH - 1; rec_x > -4; rec_x--) 
		{
			rec_y = 0; score = 0;
			if (CheckToMove(root->f, nextBlock[depth - 1], b_rotate, -1, rec_x)) { 
				for (h = 0; h < HEIGHT; h++)
					for (w = 0; w < WIDTH; w++)
						root->c[i]->f[h][w] = root->f[h][w]; 

				while (CheckToMove(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y + 1, rec_x)) 
					rec_y++;

				root->c[i]->accumulatedscore = root->accumulatedscore; 
				root->c[i]->accumulatedscore += CheckHoles(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y, rec_x);
				root->c[i]->accumulatedscore +=DeleteLine(root->c[i]->f);
				if (depth < VISIBLE_BLOCKS) score = recommend(root->c[i]); 
				else score = root->c[i]->accumulatedscore; 

				if (score > max) { 
					max = score;
					X = rec_x;
					Y = rec_y;
					R = b_rotate; 
				}
				i++; 
			}
		}
	}
	if (root->level == 0) { 
		recommendX = X; recommendY = Y; recommendR = R;
	}
	return max;
}

void Delete_recNode(RecNode * root) 
{
	for (int i = 0; i < CHILDREN_MAX; i++) {
		if (root->c[i]->level < VISIBLE_BLOCKS)
			Delete_recNode(root->c[i]); 
		free(root->c[i]->f);
		free(root->c[i]);
	}
}
void recommendedPlay() {
	int command;

	time_t start, stop;
	double duration;
/*
	FILE* fp = fopen("weight.txt", "r");
	int t_score; 

	fscanf(fp, "%d", &t_score);
	fscanf(fp, "%f %f %f %f %f", &a, &b, &c, &d, &e); 
	fclose(fp);
*/
	srand((unsigned int)time(NULL));
	clear();
	start = time(NULL);
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();

	do {
		if (timed_out == 0) {
			ualarm(10000, 0);
			timed_out = 1;
		}

		move(10, 33);
		printw("The total memory for the struct : ");
		printw("%ld bytes\n", evalSize(recRoot));
		move(12, 33);
		stop = time(NULL);
		duration = (double)difftime(stop, start);
		printw("The total time for playing : %lf seconds\n", duration);
		move(14, 33);
		printw("The score / time(m) is %lf\n", 60 * (score / duration));

		blockX = recommendX;
		blockY = recommendY;
		blockRotate = recommendR;

		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			stop = time(NULL);
			DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
			move(HEIGHT / 2, WIDTH / 2 - 4);
			printw("Good-bye!!");
			newRank(score);
			refresh();
			getch();

			return;
		}
		if (CheckToMove(field, nextBlock[0], blockRotate, blockY, blockX) == 0) gameOver = 1;
		command = GetCommand();

	} while (!gameOver);

	stop = time(NULL);
	move(10, 33);
	printw("The total memory for the struct : ");
	printw("%ld bytes\n", evalSize(recRoot));
	duration = (double)difftime(stop, start);
	move(12, 33);
	printw("The total time for playing : %lf seconds\n", duration);
	move(14, 33);
	printw("The score / time(m) is %lf\n", 60 * (score / duration));
/*
	FILE* fp_2 = fopen("weight.txt", "w");
	if (t_score < score) {
		fprintf(fp_2, "%d\n", t_score);
		fprintf(fp_2, "%f %f %f %f %f", a, b, c, d, e);
	} 
	if (score > 20000  ) {
                FILE *fp_3 = fopen("output.txt", "a+");
                fprintf(fp_3,"%d %f %f %f %f %f\n",score,a,b,c,d,e);
                fclose(fp_3);
        }
	
		fprintf(fp_2, "%d\n", score);

		a=(rand()%10000);
		b=(rand()%10000);
		c=(rand()%10000);
		d=(rand()%10000);
		e=(rand()%10000);

		fprintf(fp_2, "%f %f %f %f %f", 3+a/10000,8+b/10000,3+c/10000,2+d/10000,4+e/10000);
*/	
	alarm(0);
	DrawBox(HEIGHT / 2 - 1, WIDTH / 2 - 5, 1, 10);
	move(HEIGHT / 2, WIDTH / 2 - 4);
	printw("GameOver!!");
	getch();
	newRank(score);
	refresh();
	getch();
//	fclose(fp_2);
}

float modified_recommend(RecNode * root)
{
	float max = -10000; 

	int b_rotate, rec_x, rec_y,X,Y,R; 
	int i = 0,j; 
	int h, w;
	int depth = root->level + 1; 
	float score_1=-9999, score_2=-9999,score_3=-9999,score_4=-9999,score_5=-9999,score_6=-9999; 
	
	RecNode* temp = NULL;
	RecNode* temp_2 = root->c[1];
	RecNode* temp_3 = root->c[2];
	RecNode* temp_4 = root->c[3];
	RecNode* temp_5 = root->c[4];
	RecNode* temp_6 = root->c[5];

	if (root == NULL) return -10000;

	for (b_rotate = 0; b_rotate < 4; b_rotate++) 
	{
		for (rec_x = WIDTH - 1; rec_x > -4; rec_x--)
		{
			rec_y = 0; 
			if (CheckToMove(root->f, nextBlock[depth - 1], b_rotate, -1, rec_x)) { 
				for (h = 0; h < HEIGHT; h++)
					for (w = 0; w < WIDTH; w++)
						root->c[i]->f[h][w] = root->f[h][w]; 

				while (CheckToMove(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y + 1, rec_x)) 
					rec_y++;

				root->c[i]->accumulatedscore = root->accumulatedscore;
				root->c[i]->accumulatedscore += CheckHoles(root->c[i]->f, nextBlock[depth - 1], b_rotate, rec_y, rec_x);
				root->c[i]->accumulatedscore +=(0.8234) *(DeleteLine(root->c[i]->f)/50);
				score_1 = root->c[i]->accumulatedscore;
					
				if (score_1 > max) {
					max = score_1; 	
					temp = root->c[i];
					temp->accumulatedscore = max; 
					temp->X = rec_x; temp->Y = rec_y; temp->R = b_rotate;
				}
				else if (score_1 > temp_2->accumulatedscore) {
					score_2 = score_1;
					temp_2 = root->c[i];
					temp_2->accumulatedscore = score_1; 
					temp_2->X = rec_x; temp_2->Y = rec_y; temp_2->R = b_rotate;
				}
				else if (score_1 > temp_3->accumulatedscore) {
					score_3 = score_1;
					temp_3 = root->c[i];
					temp_3->accumulatedscore = score_1;
					temp_3->X = rec_x; temp_3->Y = rec_y; temp_3->R = b_rotate;
				}
				else if (score_1 > temp_4->accumulatedscore) {
					score_4 = score_1;
					temp_4 = root->c[i];
					temp_4->accumulatedscore = score_1;
					temp_4->X = rec_x; temp_4->Y = rec_y; temp_4->R = b_rotate;
				}
				else if (score_1 > temp_5->accumulatedscore) {
                                        score_5 = score_1;
                                        temp_5 = root->c[i];
                                        temp_5->accumulatedscore = score_1;
                                        temp_5->X = rec_x; temp_5->Y = rec_y; temp_5->R = b_rotate;
                }
				else if (score_1 > temp_5->accumulatedscore) {
					score_6 = score_1;
					temp_6 = root->c[i];
					temp_6->accumulatedscore = score_1;
					temp_6->X = rec_x; temp_6->Y = rec_y; temp_6->R = b_rotate;
				}
				i++;
			}
		}
	}
	score_1 = max;
	if (depth < VISIBLE_BLOCKS) score_1 = modified_recommend(temp);
	if (depth < VISIBLE_BLOCKS) score_2 = modified_recommend(temp_2);
	if (depth < VISIBLE_BLOCKS) score_3 = modified_recommend(temp_3);
	if (depth < VISIBLE_BLOCKS) score_4 = modified_recommend(temp_4);
	if (depth < VISIBLE_BLOCKS) score_5 = modified_recommend(temp_5);
	if (depth < VISIBLE_BLOCKS) score_5 = modified_recommend(temp_6);

	if (root->level == 0) {
		if (max < score_2 ) {
			recommendX = temp_2->X; recommendY = temp_2->Y; recommendR = temp_2->R;
			max = score_2;
		}
		if (max < score_3) {
			recommendX = temp_3->X; recommendY = temp_3->Y; recommendR = temp_3->R;
			max = score_3;
		}
		if (max < score_4){
			recommendX = temp_4->X; recommendY = temp_4->Y; recommendR = temp_4->R;
			max = score_4;
		}
		if (max <score_5) {
			recommendX = temp_5->X; recommendY = temp_5->Y; recommendR = temp_5->R;
            max = score_5;	
		}
		if (max < score_6) {
			recommendX = temp_6->X; recommendY = temp_6->Y; recommendR = temp_6->R;
			max = score_6;
		}
		else {
			recommendX = temp->X; recommendY = temp->Y; recommendR = temp->R;
		}
	}	
	
	if (max < score_2) 
		max = score_2;
	if (max < score_3)
		max = score_3;
	if (max < score_4)
		max = score_4;
	if (max < score_5)
		max = score_5;
	if (max < score_6)
		max = score_6;

	return max;
}
