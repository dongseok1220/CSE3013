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
		int HEIGHT;//�̷��� ����
		int WIDTH;//�̷��� �ʺ�
		char** input;//�ؽ�Ʈ ������ ��� ������ ��� ������ �迭�̴�.
		int** visited;//�湮���θ� ������ ����
		int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
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
		int y[4] = { 0, 1 , -1, 0 }; // ���� ��Ÿ���� �迭
		void draw_maze(); // �̷α׸��� �Լ�

		ofImage star; // ���׸� �̹���
		ofImage full_star; // ���ִ� �� �̹���
		ofImage empty_star; // ����ִ� �� �̹���
		int star_x[3] = {10, }, star_y[3] = { 10, }; // 3������ �� �̹����� ���� x,y��ǥ
		int s_flag[3] = {0, }; // 3���� �� �̹����� ���� flag ���� 1�̸� �׸��� 0�̸� �׸��� �ʴ´�.
		void draw_star(); // ���� ���õ� draw�Լ�

		ofImage knight; // ��� �̹���
		int knight_x = 10, knight_y = 10; // ��� �̹����� x,y��ǥ
		int k_flag = 0;// ����̹����� ���� flag ����
		void draw_knight(); // ��� �̹����� �׸��� �Լ�

		ofImage princess; // ���� �̹���
		int princess_x = 10, princess_y = 10; // ���� �̹����� x,y��ǥ
		int p_flag = 0;// flag ����
		void draw_princess(); // �׸��� �Լ�

		ofImage bomb; // ���� �̹���
		int bomb_x[3] = { 10, }, bomb_y[3] = { 10, }; // 3���� ������ ���� x,y
		int b_flag[3] = { 0, }; // flag
		void draw_bomb(); // �׸��� �Լ�

		ofImage Happy_end; // clear���� �� ���� �̹���
		ofImage gameover; // gameover���� �� ���� �̹���
		int H_flag = 0; // H_flag�� ���� clear �Ǵ� over �Ǻ�
		int enter_flag; // ������ ������ ������ �� ���� ����� ����
		void draw_end(); //���������Ȳ �׸���

		int endTime;
		int startTime; // ������ �ð��� ������ ������� �ϱ� ���ؼ� ���� ������

		int f_flag = 0; // ���� ���� Ȯ��

		ofImage zombie; // ���� �̹���
		int z_flag = 0; // ���� flag
		float zombie_x , zombie_y; // ���� x,y
		int left, right; // ���� �������� �Ǵ� ���������� ��� �����̰� �� ����
		void draw_zombie(); // ���� �׸���

		ofImage skill; // ��ų �̹���
		char skill_num; // ��ų ��밡��Ƚ��
		void draw_skill(); // �׸��� �Լ�

		ofImage box; // ����
		int box_flag; // ���� flag
		void draw_box(); // ���� �׸���
		int box_x, box_y; // ���� ��ǥ

		ofImage key; // ����
		int key_flag; // ���� flag
		void draw_key(); // ���� �׸�

		ofImage attack; // ���� ��ų �̹���
		int attack_flag; // ���� ��ų flag
		void draw_attack();// ���� ��ų �׸�

		int door_flag; // ���� ������ ��Ȳ
		void draw_door(); // ���ָ� ���ε��� ����

		ofImage lever; // ���� �̹���
		ofImage levershift; // ������ ���� �̹���
		int lever_flag; // flag 
		int lever_shift_flag; //flag
		void draw_lever(); // ���� �����Լ�
};

/* �̹��� ��ó */
/*
https://www.pngegg.com/ko/png-euwiu
<a href="https://kr.freepik.com/vectors/technology">Technology ���ʹ� stockgiu - kr.freepik.com�� ������</a>
<a href='https://kr.freepik.com/vectors/technology'>Technology ���ʹ� stockgiu - kr.freepik.com�� ������</a>
<a href="https://www.flaticon.com/free-icons/pixel" title="pixel icons">Pixel icons created by Freepik - Flaticon</a>
<a href='https://kr.freepik.com/vectors/technology'>Technology ���ʹ� gstudioimagen - kr.freepik.com�� ������</a>
*/