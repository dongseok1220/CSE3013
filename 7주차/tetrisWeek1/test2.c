﻿#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){

	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
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

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	if (blockY != -1 && blockX != WIDTH / 2 - 2) {
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
				if (block[blockID][blockRotate][i][j] == 1 && i + y >= 0) {
					move(i + y + 1, j + x + 1);
					attron(A_REVERSE);
					printw("%c", tile);
					attroff(A_REVERSE);
				}
			}
	}
	else if (blockRotate==0) {
		if (blockID == 0 || blockID == 3 || blockID == 5)
		{
			
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++) {
					if (block[blockID][blockRotate][i][j] == 1) {
						move(i + y + 1, j + x + 1);
						attron(A_REVERSE);
						printw("%c", tile);
						attroff(A_REVERSE);
					}
				}
		}
		else if (blockID == 1 || blockID == 2 || blockID == 4 || blockID == 6)
		{
			y = y - 1;
			for (i = 0; i < 4; i++)
				for (j = 0; j < 4; j++) {
					if (block[blockID][blockRotate][i][j] == 1) {

						if (blockRotate == 0 || blockRotate == 2)
						move(i + y + 1, j + x + 1);
						attron(A_REVERSE);
						printw("%c", tile);
						attroff(A_REVERSE);
					}
				}
		}
	}
	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int w,h; // 변수 width , height 
	
	for ( h=0; h<4; h++) 
	{
		for (w=0; w<4; w++) 
		{
			// block[currentBlock][blockRotate][h][w] 를 체크 1이면 -> 이동해서 필드체크
			// 필드가 1이면 return 0
			if (block[currentBlock][blockRotate][h][w])
			{
				if (f[h+blockY][w+blockX]) return 0; // 아니라면 너비랑 높이 체크해줌
				else if ( h+blockY >= HEIGHT || w+blockX < 0 || w+blockX >= WIDTH ) return 0; // 벗어나면 return 0
			}
                }
        }
	return 1; // 위의 조건을 다 만족하지 않으면 move 가능 return 1; 
}


void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// command 에는 KEY_UP KEY_DOWN KEY_RIGHT KEY_LEFT 각각을 입력받고 들어왔으므로 이전블록정보 찾을 수 있음 
	int w,h;
	int preRotate=(blockRotate+3)%4; // 이전 로테이트
	if (currentBlock == 1 || currentBlock==2 || currentBlock==4 || currentBlock==6) blockY++;
	switch (command)
	{
		case KEY_UP : // rotate
			for ( h=0; h<4; h++) 
			{
				for ( w=0; w<4; w++) // 기존 block 지우기 rotate는 0~3이다. (현재 rotate+3) % 4 = 이전로테이트
				{
					if (block[currentBlock][preRotate][h][w]) // 이게 1이면
					{
						move(blockY+h+1,blockX+w+1);
						printw(".");
					}
				}
			}	 
			break;

		case KEY_DOWN : // y-- 이므로 현재보다 위로 한칸
			for (h=0; h<4; h++)
			{
				for (w=0; w<4; w++)
				{
					if (block[currentBlock][blockRotate][h][w])
					{
						move(blockY+h,blockX+w+1);
						printw(".");
					}
				}
			}
			break;

		case KEY_RIGHT : // x++ 이므로 현재보다 왼쪽 한칸
			for (h=0; h<4; h++)
			{	
				for (w=0; w<4; w++)
				{
					if (block[currentBlock][blockRotate][h][w])
					{
						move(blockY+h+1, blockX+w);
						printw(".");
					}
				}
			}
			break;

		case KEY_LEFT : // x-- 이므로 현재보다 오른쪽 한칸
			for (h=0; h<4; h++)
			{
				for (w=0; w<4; w++)
				{
					if (block[currentBlock][blockRotate][h][w])
					{
						move(blockY+h+1, blockX+w+2);
						printw(".");
					}
				}
			}
			break;
				
		default : 
			break; 
	}

	DrawBlock(blockY, blockX, currentBlock, blockRotate,' '); // 블록 그리기

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
}

void BlockDown(int sig){
	// 내려갈 수 있는지 확인 
	timed_out=0; // 0으로 초기화 
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX)) // 리턴값이 1이면 가능 
	{
		blockY++; 
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX); // 내려서 그림
	}
	else if (blockY==-1) // 게임오버조건 -> 블록이 천장에 닿음  
	{
		gameOver=TRUE;
		return; //함수 종료하고 게임종료
	}
	else // 블록추가 (점수계산? 차후추가)
	{
		AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score+=DeleteLine(field); // 라인차면 삭제 
		nextBlock[0]=nextBlock[1];
		nextBlock[1]=rand()%7; // 0~6값 

		blockRotate=0; blockY=-1; blockX=WIDTH/2 -2; // 초기설정으로 초기화
		//DrawBlock(blockY, blockX, nextBlock[0],blockRotate, ' ');
		DrawNextBlock(nextBlock); // 다음블럭
		DrawField(); // 화면 갱신
		PrintScore(score);		
	}
	//강의자료 p26-27의 플로우차트를 참고한다.
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	int w,h;

	for (h=0; h<4; h++) 
	{
		for (w=0; w<4; w++)
		{
			if (block[currentBlock][blockRotate][h][w]) // 블록을 쌓는다
			{
				f[blockY+h][blockX+w]=1;
			}
		}
	}

	//Block이 추가된 영역의 필드값을 바꾼다.
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	int w,h;
	int flag; // flag
	int line=0; // 지울라인 개수
	for ( h= HEIGHT-1; h>=0; h--) // 아래에서부터 탐색하므로 거꾸로 
	{
		flag=1; // 초기화
		for (w=0; w<WIDTH; w++)
		{
			if (f[h][w]!=1) 
			{
				flag=0;
				break; // 하나라도 1이아니면 break
			} 
		}
		if (flag) // 현재 h에 지울라인이 있음
		{
			line++; // 라인개수 세주고 필드값 내리기
			for (int temp=h-1; temp>=0; temp--) // 맨 윗줄은 따로 0으로 초기화
			{
				for (w=0; w<WIDTH; w++) 
					f[temp+1][w]=f[temp][w]; 
			}
			for (w=0; w<WIDTH; w++) 
				f[0][w]=0;
			h++; // 그 줄부터 다시 검사해야함
		}

	}
	return line*line*100; // 점수리턴 

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
