/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#include<stack>
#include<queue>
#define MAX 20
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Game"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

	star.load("images/star.png");
	knight.load("images/knight.png");
	princess.load("images/princess.jpg");
	bomb.load("images/bomb.jpg");
	Happy_end.load("images/happy_ending.jpg");
	empty_star.load("images/empty_star.png");
	full_star.load("images/full_star.png");
	zombie.load("images/zombie.jpg");
	gameover.load("images/game_over.png");
	skill.load("images/remove_trap.png");
	attack.load("images/attack.png");
	box.load("images/box.jpg");
	lever.load("images/lever.jpg");
	levershift.load("images/lever_shift.jpg");
	key.load("images/key.jpg"); // image 로드 

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		readFile();
	}
	if(title == "Exit") {
		ofExit(); // Quit the application
	}
	//
	// Window menu
	//
	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}
} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	draw_maze(); // 미로 생성
	
	if (f_flag) {
		draw_knight(); // 기사 그리기
		draw_star(); // 별과 관련된 이미지 그리기
		draw_bomb(); // 함정과 관련된 이미지 그리기
		draw_princess(); // 구출해야하는 공주 이미지 그리기
		draw_skill(); // 사용할 수 있는 스킬 이미지 그리기
		draw_attack(); // 공격키 이미지 그리기
		draw_door(); // 이미지는 아니지만, 공주가 갇혀있음을 그려주는 함수
		draw_zombie(); // 좌우로만 움직이는 좀비 하나를 그려준다.
		draw_box(); // 열쇠로 잠겨있는 상자 그리기
		draw_key(); // 상자를 열수있는 열쇠 그리기
		draw_lever(); // 공주를 구출 할 수 있는 레버 생성
		draw_end(); // 플레이가 종료되는 조건에 따른 이미지 그리기

	}
} // end Draw

void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	int temp_y = knight_x - 10; 
	int temp_x = knight_y - 10;
	temp_x = temp_x / MAX; temp_y = temp_y / MAX;
	temp_x += 1; temp_y += 1; // input에 넣기 위해 원래 값으로 바꿔줌
	
	if (p_flag && enter_flag !=1) {
		if (key == OF_KEY_RIGHT) {
			if (knight_x < (WIDTH - 3) * MAX) { // 오른쪽으로 갈 수 있는지?
				if (input[temp_x][temp_y + 1] != '|')
					knight_x += 40;
			}
		}
		if (key == OF_KEY_LEFT) {
			if (knight_x >= 50) {// 왼쪽으로 갈 수 있는지
				if (input[temp_x][temp_y - 1] != '|')
					knight_x -= 40;
			}
		}
		if (key == OF_KEY_UP) {
			if (knight_y >= 50) { // 위쪽으로 갈 수 있는지
				if (input[temp_x - 1][temp_y] != '-')
					knight_y -= 40;
			}
		}
		if (key == OF_KEY_DOWN) { // 아래족으로 갈 수 있는지
			if (knight_y < (HEIGHT - 3) * MAX) {
				if (input[temp_x + 1][temp_y] != '-')
					knight_y += 40;
			}
		}
	}
	//printf("%d %d %d %d\n", knight_x, knight_y, star_x, star_y);
	// Remove or show screen info

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	if (key == 'q') { // 프로그램 종료
		freeMemory();
		ofExit();
	}
	if (key == 'o') { // 파일 읽기
		system("cls");
		readFile();
		enter_flag = 0; // 파일을 읽고 난 후에는 키 다시 정상작동
	}
	if (key == 'r' && skill_num >'0') { // 주변 함정 없애는 스킬 
		if (skill_num > '0')
			skill_num--;
		cout << "Skill left :" << skill_num << endl;
		for (int i = 0; i < 3; i++) { // 3가지 함정에 대해 확인해주는 for문
			if (knight_x + 40 == bomb_x[i] - 10 && knight_y == bomb_y[i] - 10) {
				bomb_x[i] = -1; bomb_y[i] = -1;
				b_flag[i] = 0;
			}
			if (knight_x - 40 == bomb_x[i] - 10 && knight_y == bomb_y[i] - 10) {
				bomb_x[i] = -1; bomb_y[i] = -1;
				b_flag[i] = 0;
			}
			if (knight_x == bomb_x[i] - 10 && knight_y + 40 == bomb_y[i] - 10) {
				bomb_x[i] = -1; bomb_y[i] = -1;
				b_flag[i] = 0;
			}
			if (knight_x == bomb_x[i] - 10 && knight_y - 40 == bomb_y[i] - 10) {
				bomb_x[i] = -1; bomb_y[i] = -1;
				b_flag[i] = 0;
			}
		}
	}
	if (key == 'a') { // 공격했을 때, 상하좌우에 좀비가 있으면 좀비 죽음
		if (knight_x + 40 == (((int)zombie_x * MAX) - 10)  && knight_y == ((int)zombie_y * MAX) - 10) {
			z_flag = 0;
		}
		if (knight_x - 40 == (((int)zombie_x * MAX) - 10)  && knight_y == ((int)zombie_y * MAX) - 10) {
			z_flag = 0;
		}
		if (knight_x == ((int)zombie_x * MAX) - 10 && knight_y -40 == ((int)zombie_y * MAX) - 10) {
			z_flag = 0;
		}
		if (knight_x == ((int)zombie_x * MAX) - 10 && knight_y + 40 == ((int)zombie_y * MAX) - 10) {
			z_flag = 0;
		}
	}
} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	if (f_flag) freeMemory(); // 파일이 처음여는게 아니면 free
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");
		//ofSetFullscreen(true); // 화면 최대

		// 각종 flag 들 초기화
		f_flag = 1; // 파일오픈
		s_flag[0] = 1; s_flag[1] = 1; s_flag[2] = 1; //3개의 별에대해 1이면 그림
		b_flag[0] = 1; b_flag[1] = 1; b_flag[2] = 1; //3개의 함정에대해 1이면 그림
		p_flag = 1; // 1이면 공주를 그림
		H_flag = 0; // 1이면 game clear 2면 gameover
		z_flag = 1; // 1이면 좀비생성
		skill_num = '3'; // ‘r’를 눌러 사용할 수 있는 스킬의 개수
		enter_flag = 0; // 게임이 끝나면 1로바뀌며 누를 수 있는 키제한
		lever_flag = 0; // 1이면 레버 생성
		lever_shift_flag = 0; // 1이면 움직인 레버 그리기
		box_flag = 0; // 1이면 상자그리기
		key_flag = 0; // 1이면 열쇠 그리기

		right = 1; // 좀비 이동방향
		left = 0; // 좀비 이동방향

		// 좌표변수 초기화
		knight_x = 10, knight_y = 10;
		princess_x = 10, princess_y = 10;

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		//printf("file name is %s\n", fileName);
		//cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		//printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				//cout << "We found the target file." << endl;
				cout << "Game start" << endl;
				cout << "Remember three traps" << endl;
				cout << "You can remove traps by using skill" << endl;
				cout << "Use your skills in front of traps." << endl;
				cout << "Collect three stars" << endl;
				cout << "Good luck. Save princess" << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;
			
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				cnt++;
				if (cnt == 1) {
					vector<string> words = ofSplitString(line, "-");
					WIDTH = 2 * words.size() - 1;
				}
			}
			HEIGHT = cnt; // 파일 읽고 height , width 얻기
			if (HEIGHT * WIDTH < 49) { // 미로가 너무작으면 에러가 나기때문에 예외 처리
				cout << "Too small to play game" << endl;
				return false;
			}
			// cout << HEIGHT << WIDTH;
			input = (char **)malloc(sizeof(char *) *HEIGHT);
			if (input == NULL) return false;

			for (cnt = 0; cnt < HEIGHT; cnt++)
				input[cnt] = (char *)malloc(sizeof(char) *WIDTH);
			cnt = 0;

			visited = (int **)malloc(sizeof(int *) *HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				visited[i] = (int *)malloc(sizeof(int) *WIDTH);

			if (visited == NULL) return false;

			int init = 1; 
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				for (int j = 0; j < WIDTH; j++) {
					input[cnt][j] = line[j];
					if (input[cnt][j] == '+' || input[cnt][j] == '-' || input[cnt][j] == '|') visited[cnt][j] = 0;
					else {
						visited[cnt][j] = 10000* cnt + j; // visited와 stack 이용해서 dfs함수에서 사용
					}
				}
				cnt++;
			}

			DFS(); //visited 에 값 할당
			srand(time(NULL)); // 랜덤함수 사용하기 위해서

			int random;
			int flag = 0; // 3개의 함정 위치가 모두 정해지는지 확인하는 변수
			int flag2 = 0; // 3개의 별의 위치가 모두 정해지는지 확인하는 변수
			//int flag3 = 0; // 박스의 위치가 정해지는지 확인하는 변수
			int complex = 0; // 별의 위치가 너무 가까운걸 방지하기 위한 변수
			box_x = 0; box_y = 0; // 박스 위치 고정
			while (flag2 != 3 || flag != 3 ) { // 3개씩 덜 생성되면 다시 for loop 
				for (int i = 1; i < HEIGHT - 1; i++) {
					for (int j = 1; j < WIDTH - 1; j++) {
						random = rand() % 2;
						complex++;
						/*
						if (flag3 == 0 && visited[i][j] > 10000 && random) {
							box_x = j; box_y = i;
							flag3 = 1;
						}
						*/
						if (visited[i][j] == 1 && random && i % 2 == 1 && j % 2 == 1 && flag != 3) {
							if (i == 1 && j == 1) continue;
							bomb_y[flag] = (i * MAX); 
							bomb_x[flag] = (j * MAX); // 함정 x,y 좌표 설정
							flag++;
						}
						else if (visited[i][j] > 1 && random && i % 2 == 1 && j % 2 == 1 && flag2 != 3 && complex > WIDTH / 2) {
							star_y[flag2] = (i * MAX);
							star_x[flag2] = (j * MAX); // 별 x,y좌표 설정
							//printf("%d %d\n", star_y[flag2], star_x[flag2]);
							flag2++;
							complex = 0;
						}
					}
				}
			}
			endTime = (unsigned)time(NULL);
			endTime += 3; // 3초차이
 			
			zombie_x = 1;
			zombie_y = HEIGHT - 2; // 좀비 위치 고정

		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

void ofApp::freeMemory() {
	for (int i = 0; i < HEIGHT; i++)
		free(input[i]);
	free(input);

	for (int i = 0; i < HEIGHT; i++)
		free(visited[i]);
	free(visited);

	//cout << "MEMORY FREE" << endl;
	//TO DO
	// malloc한 memory를 free해주는 함수
}

bool ofApp::DFS()//DFS탐색을 하는 함수 실습때와 같음
{
	// 기본생각 - 진짜 path는 1로 가짜 path는 2로 설정 
	
	// 다시 DFS를 이미 호출했는데 다시 호출했을 때, 오류 방지용
	for (int cnt = 0; cnt < HEIGHT; cnt++) {
		for (int j = 0; j < WIDTH; j++) {
			if (input[cnt][j] == '+' || input[cnt][j] == '-' || input[cnt][j] == '|') visited[cnt][j] = 0;
			else {
				visited[cnt][j] = 10000 * cnt + j;
			}
		}
	}

	int i, j, t=0;
	int flag = 1;

	path.push(visited[1][1]);
	visited[1][1] = 1;

	while (!path.empty()) {

		i = path.top(); 
		i = i / 10000;
		j = path.top();
		j = j % 10000; 
		flag = 1; // i,j 구하기

		if (i == HEIGHT - 2 && j == WIDTH - 2) break; // 목표지점

		for (t = 0; t < 4; t++) {
			if (i + y[t] < HEIGHT && i +y[t] >=0 && j+x[t] >=0 && j+x[t] < WIDTH ) {
				if (visited[i + y[t]][j + x[t]] > 10000) {
					path.push(visited[i + y[t]][j + x[t]]);
					visited[i + y[t]][j + x[t]] = 1;
					flag = 0;
					break;
				}
			}
		}
		if (flag)  {
			visited[i][j] = 2;
			path.pop();
		}
	}
	while (!path.empty())
		path.pop();

	return TRUE;
}

void ofApp::draw_knight()
{
	ofSetColor(255, 255, 255);
	knight.draw(knight_x, knight_y, MAX, MAX);
}

/* 별은 총 3개가 있다. 별을 다 먹는 것 역시 게임의 목표로 설정한다.*/
void ofApp::draw_star()
{
	for (int i = 0; i < 3; i++) {
		if (knight_x == star_x[i] - 10 && knight_y == star_y[i] - 10) {
			s_flag[i] = 0;
		}
		if (s_flag[i] == 0) full_star.draw(10 + i * MAX * 2, (2 * HEIGHT) * 10 - 10, 2 * MAX, 2 * MAX);
		if (s_flag[i]) {
			ofSetColor(255, 255, 255);
			star.draw(star_x[i] - 10, star_y[i] - 10, MAX, MAX);
			empty_star.draw(10 + i * MAX * 2, (2 * HEIGHT) * 10 - 10, 2 * MAX, 2 * MAX);
		}
	}
}
/* 폭탄의 개수도 3개이다. 폭탄은 일정시간이 지나면 보이지 않으며, 스킬(r)로 무력화가 가능하다.*/
void ofApp::draw_bomb() 
{
	startTime = (unsigned)time(NULL);
	for (int i = 0; i < 3; i++) {
		if (b_flag[i]) {
			ofSetColor(255, 255, 255);
			bomb.draw(bomb_x[i] - 10, bomb_y[i] - 10, MAX, MAX);
		}
	}

	if (endTime - startTime == 0) { // 시간이 다되면 flag를 0으로 해서 더이상 그리지 않는다. 
		b_flag[0] = 0;
		b_flag[1] = 0;
		b_flag[2] = 0;
	}
}

void ofApp::draw_princess()
{
	ofSetColor(255, 255, 255);
	princess.draw(10 + MAX * (WIDTH - 3), 10 + MAX * (HEIGHT - 3), MAX, MAX);
}

void ofApp::draw_skill()
{
	ofSetColor(255, 255, 255);
	skill.draw(WIDTH * 20 + MAX, MAX, 5 * MAX, 5 * MAX);
}

void ofApp::draw_zombie() // 좀비가 좌우로 왔다갔다 하도록 조건 설정
{
	if (z_flag) {
		zombie.draw((zombie_x * MAX) - 10, (zombie_y * MAX) - 10, MAX, MAX);
			
		if (zombie_x * MAX - 10 < (WIDTH - 3) * MAX && right) {
			if (input[(int)zombie_y][(int)zombie_x + 1] != '|') {
				zombie_x += 0.1;
				right = 1;
			}
			else {
				right = 0;
				left = 1;
			}
		}
		if (zombie_x * MAX - 10 >= 10 && left) {
			if (input[(int)zombie_y][(int)zombie_x - 1] != '|') {
				zombie_x -= 0.1;
				left = 1;
			}
			else {
				left = 0;
				right = 1;
			}

		}
	}
}

void ofApp::draw_end()
{
	if (knight_x == 10 + MAX * (WIDTH - 3) && knight_y == 10 + MAX * (HEIGHT - 3) && p_flag == 1) { // 공주랑 만낫을때
		cout << "Game Clear" << endl;
		p_flag = 0; // 공주 구출
		H_flag = 1;
		enter_flag = 1;
	}
	if (!H_flag) { // 함정밟았을 때
		for (int i = 0; i < 3; i++) {
			if (knight_x == bomb_x[i] - 10 && knight_y == bomb_y[i] - 10) {
				cout << "Game Over" << endl;
				H_flag = 2;
				enter_flag = 1;
			}
		}
	}
	if (z_flag) { // 좀비 만났을 때
		if (knight_x == ((int)zombie_x * MAX) - 10 && knight_y == ((int)zombie_y * MAX) - 10 && !H_flag) {
			cout << "Game Over" << endl;
			H_flag = 2;
			enter_flag = 1;
			k_flag = 0;
		}
	}
	if (H_flag == 1) { // ending_flag; 해피앤딩
		ofSetColor(255, 255, 255);
		Happy_end.draw((WIDTH - 8) * 10, (HEIGHT - 8) * 10, MAX * 6, MAX * 6);
	}
	else if (H_flag == 2) { // 게임오버
		ofSetColor(255, 255, 255);
		gameover.draw((WIDTH / 4) * 10, (HEIGHT / 4) * 10, MAX * (WIDTH / 2), MAX*(HEIGHT / 2));
	}
}

void ofApp::draw_door()
{
	int ran;

	if (lever_shift_flag == 0) {
		input[HEIGHT - 3][WIDTH - 2] = '-';
		input[HEIGHT - 2][WIDTH - 3] = '|'; // input을 변경해서 공주를 가둔다. 
	}
	else if (lever_shift_flag) {
		//input[HEIGHT - 3][WIDTH - 2] = 1;
		input[HEIGHT - 2][WIDTH - 3] = 1;
	}
}

void ofApp::draw_key() // 열쇠 오픈 열쇠 먹으면 다시 flag = 0;
{
	if (z_flag == 0 && key_flag == 0) {
		key.draw(((int)zombie_x * MAX) - 10, ((int)zombie_y * MAX) - 10, MAX, MAX);
		key_flag = 1;
	}
	if (key_flag == 1) {
		key.draw(((int)zombie_x * MAX) - 10, ((int)zombie_y * MAX) - 10, MAX, MAX);
		if (knight_x == ((int)zombie_x * MAX) - 10 && knight_y == ((int)zombie_y * MAX) - 10)
			key_flag = -1;
	}
}

void ofApp::draw_attack()
{
	ofSetColor(255, 255, 255);
	attack.draw(WIDTH * 20 + MAX, 6 * MAX, 5 * MAX, 5 * MAX);
}

void ofApp::draw_box() // 박스는 3가지의 별을 다 먹어야 보임 // 열쇠로 열면 다시 안보임 
{
	box_flag = 0;

	for (int i = 0; i < 3; i++)
	{
		if (s_flag[i] == 0 && lever_flag == 0 && lever_shift_flag ==0) box_flag = 1;
		else {
			box_flag = 0;
			return;
		}
	}
	if (box_flag) {
		box.draw(box_x *MAX + 10, box_y *MAX + 10, MAX, MAX);
	}

}

void ofApp::draw_lever() // 레버는 가까이가면 움직인 래버로 그려주어야함 열쇠랑 상자랑 만났을 때 그려주어야함
{
	if (key_flag && box_x*MAX + 10 == knight_x && box_y*MAX + 10 == knight_y) {
		lever_flag = 1;
		box_flag = 0;
	}
	if (lever_flag) {
		lever.draw(10, (WIDTH - 2)*MAX - 10, MAX, MAX);
		if (knight_x - 40 == 10 && knight_y == (WIDTH - 2) * MAX -10) {
			lever_flag = 0;
			lever_shift_flag = 1;
		}
		else if ( knight_x == 10 && knight_y + 40 == (WIDTH - 2) * MAX - 10) {
			lever_flag = 0;
			lever_shift_flag = 1;
		}
	}
	if (lever_shift_flag == 1) {
		levershift.draw(10, (WIDTH - 2)*MAX - 10, MAX, MAX);
	}

}

void ofApp::draw_maze()
{
	// 기본적으로 i가 y좌표 j가 x좌표가 된다고 생각한다. 
	// 이후 크기는 임의로 MAX배로 해준다. 
	for (int i = 1; i < HEIGHT - 1; i++) { // input 배열의 겉 부분은 직선으로 표현 -> i,j는 1부터 그리고 기존 너비 높이 보다  1작게
		if (i == 1) {
			ofDrawLine(0, 0, (WIDTH - 1) * MAX, 0); // 좌측 상단 부터 길이 MAX배 가로 선 
			ofDrawLine(0, 0, 0, (HEIGHT - 1) * MAX); // 세로도 마찬가지 세로 선  
			ofDrawLine(0, (HEIGHT - 1) * MAX, (WIDTH - 1) * MAX, (HEIGHT - 1) * MAX);
			ofDrawLine((WIDTH - 1) * MAX, 0, (WIDTH - 1) * MAX, (HEIGHT - 1) * MAX); // 큰 네모를 만든다. 10*10 미로의 경우 실제 할당되는 HEIGHT 와 WIDTH의 값은 11*11 이다. 따라서 -1씩 해준 후 그려준다. 
		}
		for (int j = 1; j < WIDTH - 1; j++) {
			if (input[i][j] == '-') // 세로 벽을 만나면, 세로 줄 한칸 -1 부터 +1 까지 그려준다. 
				ofDrawLine((j - 1) * MAX, i * MAX, (j + 1) * MAX, i * MAX);
			else if (input[i][j] == '|') // 가로 벽을 만나면, 가로 줄 한칸 -1 부터 +1 까지 그려준다.  왜냐하면 텍스트 상 연결 부분이 + 로 표현되어있기 때문에 벽을 기준으로 아래 위, 양옆을 그려주어야한다. 
				ofDrawLine(j * MAX, (i - 1) * MAX, j * MAX, (i + 1) * MAX);
		}
	}
}