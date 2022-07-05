/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

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
*/

	#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon
#include<time.h>

class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key); // Traps escape key if exit disabled
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		bool readFile();
		void freeMemory();
		bool DFS();
		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		char** input;//텍스트 파일의 모든 정보를 담는 이차원 배열이다.
		int** visited;//방문여부를 저장할 포인
		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
		// Menu
		ofxWinMenu * menu; // Menu object
		void appMenuFunction(string title, bool bChecked); // Menu return function

		// Used by example app
		ofTrueTypeFont myFont;
        ofImage myImage;
		float windowWidth, windowHeight;
		HWND hWnd; // Application window
		HWND hWndForeground; // current foreground window

		// Example menu variables
		bool bShowInfo;
		bool bFullscreen;
		bool bTopmost;
		// Example functions
 		void doFullScreen(bool bFull);
		void doTopmost(bool bTop);

		stack<int> path;

		int x[4] = { 1, 0, 0, -1 };
		int y[4] = { 0, 1 , -1, 0 }; // 방향 나타내는 배열
		void draw_maze(); // 미로그리는 함수

		ofImage star; // 별그림 이미지
		ofImage full_star; // 차있는 별 이미지
		ofImage empty_star; // 비어있는 별 이미지
		int star_x[3] = {10, }, star_y[3] = { 10, }; // 3가지의 별 이미지에 대한 x,y좌표
		int s_flag[3] = {0, }; // 3가지 별 이미지에 대한 flag 변수 1이면 그리고 0이면 그리지 않는다.
		void draw_star(); // 별과 관련된 draw함수

		ofImage knight; // 기사 이미지
		int knight_x = 10, knight_y = 10; // 기사 이미지의 x,y좌표
		int k_flag = 0;// 기사이미지에 대한 flag 변수
		void draw_knight(); // 기사 이미지를 그리는 함수

		ofImage princess; // 공주 이미지
		int princess_x = 10, princess_y = 10; // 공주 이미지의 x,y좌표
		int p_flag = 0;// flag 변수
		void draw_princess(); // 그리는 함수

		ofImage bomb; // 함정 이미지
		int bomb_x[3] = { 10, }, bomb_y[3] = { 10, }; // 3가지 함정에 대한 x,y
		int b_flag[3] = { 0, }; // flag
		void draw_bomb(); // 그리는 함수

		ofImage Happy_end; // clear했을 때 나올 이미지
		ofImage gameover; // gameover했을 때 나올 이미지
		int H_flag = 0; // H_flag를 통해 clear 또는 over 판별
		int enter_flag; // 게임이 끝나면 움직일 수 없게 만드는 변수
		void draw_end(); //게임종료상황 그리기

		int endTime;
		int startTime; // 함정이 시간이 지나면 사라지게 하기 위해서 만든 변수들

		int f_flag = 0; // 파일 오픈 확인

		ofImage zombie; // 좀비 이미지
		int z_flag = 0; // 좀비 flag
		float zombie_x , zombie_y; // 좀비 x,y
		int left, right; // 좀비를 왼쪽으로 또는 오른쪽으로 계속 움직이게 할 변수
		void draw_zombie(); // 좀비 그리기

		ofImage skill; // 스킬 이미지
		char skill_num; // 스킬 사용가능횟수
		void draw_skill(); // 그리는 함수

		ofImage box; // 상자
		int box_flag; // 상자 flag
		void draw_box(); // 상자 그리기
		int box_x, box_y; // 상자 좌표

		ofImage key; // 열쇠
		int key_flag; // 열쇠 flag
		void draw_key(); // 열쇠 그림

		ofImage attack; // 공격 스킬 이미지
		int attack_flag; // 공격 스킬 flag
		void draw_attack();// 공격 스킬 그림

		int door_flag; // 공주 갇히는 상황
		void draw_door(); // 공주를 가두도록 설정

		ofImage lever; // 레버 이미지
		ofImage levershift; // 움직인 레버 이미지
		int lever_flag; // flag 
		int lever_shift_flag; //flag
		void draw_lever(); // 레버 관련함수
};

/* 이미지 출처 */
/*
https://www.pngegg.com/ko/png-euwiu
<a href="https://kr.freepik.com/vectors/technology">Technology 벡터는 stockgiu - kr.freepik.com가 제작함</a>
<a href='https://kr.freepik.com/vectors/technology'>Technology 벡터는 stockgiu - kr.freepik.com가 제작함</a>
<a href="https://www.flaticon.com/free-icons/pixel" title="pixel icons">Pixel icons created by Freepik - Flaticon</a>
<a href='https://kr.freepik.com/vectors/technology'>Technology 벡터는 gstudioimagen - kr.freepik.com가 제작함</a>
*/