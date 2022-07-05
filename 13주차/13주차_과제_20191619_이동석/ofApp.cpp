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
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false;
	isBFS = false;
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
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS",false,false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();
	// test
	hPopup = menu->AddPopupMenu(hMenu, "HELLO");
	menu->AddPopupItem(hPopup, "HELLO");

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
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			if (isBFS) isBFS = 0;
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			if (isdfs) isdfs = 0;
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

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
	// test
	if (title == "HELLO") cout << "HELLO" << endl;
} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.

	// add code here
	// �⺻������ i�� y��ǥ j�� x��ǥ�� �ȴٰ� �����Ѵ�. 
	// ���� ũ��� ���Ƿ� 20��� ���ش�. 
	for (int i = 1; i < HEIGHT - 1; i++) { // input �迭�� �� �κ��� �������� ǥ�� -> i,j�� 1���� �׸��� ���� �ʺ� ���� ����  1�۰�
		if (i == 1) {
			ofDrawLine(0, 0, (WIDTH-1) * 20, 0); // ���� ��� ���� ���� 20�� ���� �� 
			ofDrawLine(0, 0, 0, (HEIGHT-1) * 20); // ���ε� �������� ���� ��  
			ofDrawLine(0, (HEIGHT-1) * 20, (WIDTH-1) * 20, (HEIGHT-1) * 20); 
			ofDrawLine((WIDTH-1) * 20, 0, (WIDTH-1) * 20, (HEIGHT-1) * 20); // ū �׸� �����. 10*10 �̷��� ��� ���� �Ҵ�Ǵ� HEIGHT �� WIDTH�� ���� 11*11 �̴�. ���� -1�� ���� �� �׷��ش�. 
		} 
 		for (int j = 1; j < WIDTH - 1; j++) {
			if (input[i][j] == '-') // ���� ���� ������, ���� �� ��ĭ -1 ���� +1 ���� �׷��ش�. 
				ofDrawLine((j-1) * 20, i * 20, (j+1) * 20, i * 20);
			else if (input[i][j] == '|') // ���� ���� ������, ���� �� ��ĭ -1 ���� +1 ���� �׷��ش�.  �ֳ��ϸ� �ؽ�Ʈ �� ���� �κ��� + �� ǥ���Ǿ��ֱ� ������ ���� �������� �Ʒ� ��, �翷�� �׷��־���Ѵ�. 
				ofDrawLine(j * 20, (i-1) * 20, j * 20, (i+1) * 20);
		}
	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
	if (isBFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			bfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
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

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}
	
	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	if (key == 'q')
		ofExit();
	if (key == 'd') {
		if (isOpen)
		{
			if (isBFS) isBFS = 0;
			DFS();
			bShowInfo = true;
		}
		else
			cout << "you must open file first" << endl;
	}
	if (key == 'o') {
		readFile();
	}
	if (key == 'b') {
		//doTopmost(true); 
		if (isOpen)
		{
			if (isdfs) isdfs = 0;
			BFS();
			bShowInfo = true;
		}
		else
			cout << "you must open file first" << endl;
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
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		//printf("file name is %s\n", fileName);
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
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
			HEIGHT = cnt;
			// cout << HEIGHT << WIDTH;
			input = (char **)malloc(sizeof(char *) *HEIGHT);
			for (cnt = 0; cnt < HEIGHT; cnt++)
				input[cnt] = (char *)malloc(sizeof(char) *WIDTH);
			cnt = 0;

			visited = (int **)malloc(sizeof(int *) *HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				visited[i] = (int *)malloc(sizeof(int) *WIDTH);

			visited_2 = (int **)malloc(sizeof(int *) *HEIGHT);
			for (int i = 0; i < HEIGHT; i++)
				visited_2[i] = (int *)malloc(sizeof(int) *WIDTH);

			int init = 1; 
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				for (int j = 0; j < WIDTH; j++) {
					input[cnt][j] = line[j];
					if (input[cnt][j] == '+' || input[cnt][j] == '-' || input[cnt][j] == '|') visited[cnt][j] = 0;
					else {
						visited[cnt][j] = 10000* cnt + j; // visited�� stack queue ���̺귯�� �̿��Ͽ� dfs, bfs ���� , ���� �����¿� �����ϱ� ���� ����迭 ���߿� ���� 
						visited_2[cnt][j] = 10000 * cnt + j;
					}
				}
				cnt++;
			}
			
			/*
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++)
					cout << visited[i][j] << " ";
				cout << endl;
			}
			cout << HEIGHT << " " << WIDTH << endl;
			*/
			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�
			
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

	for (int i = 0; i < HEIGHT; i++)
		free(visited_2[i]);
	free(visited_2);

	while (!path.empty())
		path.pop();
	path.pop();

	while (!path_bfs.empty())
		path_bfs.pop();
	path_bfs.pop();

	for (int i = 0; i < HEIGHT; i++)
		free(temp[i]);
	free(temp);

	cout << "MEMORY FREE" << endl;
	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	// �⺻���� - ��¥ path�� 1�� ��¥ path�� 2�� ���� 
	int x[4] = { 1, 0, 0, -1 };
	int y[4] = { 0, 1 , -1, 0 };
	
	// �ٽ� DFS�� �̹� ȣ���ߴµ� �ٽ� ȣ������ ��, ���� ������
	for (int cnt = 0; cnt < HEIGHT; cnt++) {
		for (int j = 0; j < WIDTH; j++) {
			if (input[cnt][j] == '+' || input[cnt][j] == '-' || input[cnt][j] == '|') visited[cnt][j] = 0;
			else {
				visited[cnt][j] = 10000 * cnt + j;
				//visited_2[cnt][j] = 10000 * cnt + j;
			}
		}
	}

	int i, j, t=0;
	int flag = 1;
	int target = visited[HEIGHT-1][WIDTH-1] ; 
	path.push(visited[1][1]);
	visited[1][1] = 1;

	while (!path.empty()) {

		i = path.top(); 
		i = i / 10000;
		j = path.top();
		j = j % 10000; 
		flag = 1; 

		if (i == HEIGHT - 2 && j == WIDTH - 2) break;

		for (t = 0; t < 4; t++) {
			if (visited[i + y[t]][j + x[t]] >10000) {
				path.push(visited[i + y[t]][j + x[t]]);
				visited[i + y[t]][j + x[t]] = 1;
				flag = 0;
				break;
			}
		}
		if (flag)  {
			visited[i][j] = 2;
			path.pop();
		}
	}
	/*
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			cout << visited[i][j] << " ";
		cout << endl;
	}
	*/
	while (!path.empty())
		path.pop();

	isdfs = 1;
	
	return TRUE;
}
void ofApp::dfsdraw()
{
	ofSetColor(0, 0, 255);
	ofDrawCircle(20, 20, 5); // ����� 
	ofSetColor(0, 255, 0);
	ofDrawCircle(20 * (WIDTH - 2), 20 * (HEIGHT - 2), 5); // ������

	int i, j; 
	ofSetColor(0, 0, 0);
	
	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			ofSetColor(0, 0, 0);
			if (visited[i][j] == 1 && visited[i - 1][j] == 1)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited[i][j] == 1 && visited[i][j+1] == 1) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j+1), 20 * i);
			if (visited[i][j] == 1 && visited[i+1][j] == 1) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
		    if (visited[i][j] == 1 && visited[i][j-1] == 1) // ����� ����
				ofDrawLine(20 * (j-1), 20 * i, 20 * (j), 20 * i);

			ofSetColor(255,0,0);

			if (visited[i][j] == 2 && visited[i - 1][j] == 2)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited[i][j] == 2 && visited[i][j + 1] == 2) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j + 1), 20 * i);
			if (visited[i][j] == 2 && visited[i + 1][j] == 2) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
			if (visited[i][j] == 2 && visited[i][j - 1] == 2) // ����� ����
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j), 20 * i);

			if (visited[i][j] == 1 && visited[i - 1][j] == 2)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited[i][j] == 1 && visited[i][j + 1] == 2) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j + 1), 20 * i);
			if (visited[i][j] == 1 && visited[i + 1][j] == 2) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
			if (visited[i][j] == 1 && visited[i][j - 1] == 2) // ����� ����
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j), 20 * i);
		}
	}


	//TO DO 
	//DFS�� ������ ����� �׸���. (3���� ����)
}

bool ofApp::BFS()
{
	int x[4] = { 1, 0, 0, -1 };
	int y[4] = { 0, 1 , -1, 0 };

	// �ٽ� BFS�� �̹� ȣ���ߴµ� �ٽ� ȣ������ ��, ���� ������
	for (int cnt = 0; cnt < HEIGHT; cnt++) {
		for (int j = 0; j < WIDTH; j++) {
			if (input[cnt][j] == '+' || input[cnt][j] == '-' || input[cnt][j] == '|') visited[cnt][j] = 0;
			else {
				//visited[cnt][j] = 10000 * cnt + j;
				visited_2[cnt][j] = 10000 * cnt + j;
			}
		}
	}

	int i, j, t = 0;
	int num = visited_2[1][1];
	int target = visited_2[HEIGHT - 1][WIDTH - 1];
	//path_bfs.push = visited[1][1]; // visit �� push
	path_bfs.push(visited_2[1][1]);

	/*
	cout << path_bfs.front().visit << endl;
	cout << path_bfs.front().old_visit << endl;
	*/

	temp = (int **)malloc(sizeof(int *) * HEIGHT);
	for (i = 0; i < HEIGHT; i++)
		temp[i] = (int *)malloc(sizeof(int) *WIDTH);

	for (i = 0; i < HEIGHT; i++)
		for (j = 0; j < WIDTH; j++) {
			temp[i][j] = -1;
		}

	temp[1][1] = visited_2[1][1];

	visited_2[1][1] = 1; // 1�� �ʱ�ȭ 

	while (!path_bfs.empty()) {
		i = path_bfs.front();
		i = i / 10000;
		j = path_bfs.front();
		j = j % 10000;
		
		path_bfs.pop();

		if (i == HEIGHT - 2 && j == WIDTH - 2) break;

		for (t = 0; t < 4; t++) {
			if (visited_2[i + y[t]][j + x[t]] > 10000) {
				path_bfs.push(visited_2[i + y[t]][j + x[t]]);
				temp[i + y[t]][j + x[t]] = num;
				visited_2[i + y[t]][j + x[t]] = 2;
			}
		}
		num = path_bfs.front();
	}
	visited_2[i][j] = 1;

	while (i != 1 || j != 1) {
		num = temp[i][j];
		i = num / 10000;
		j = num % 10000;
		visited_2[i][j] = 1;
	}
	/*
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			cout << visited_2[i][j] << " ";
		cout << endl;
	}
	*/
	isBFS = 1;

	while (!path_bfs.empty())
		path_bfs.pop();
	//path_bfs.pop();

	for (int i = 0; i < HEIGHT; i++)
		free(temp[i]);
	free(temp);

	return TRUE;
}

void ofApp::bfsdraw()
{
	ofSetColor(0, 0, 255);
	ofDrawCircle(20, 20, 5); // ����� 
	ofSetColor(0, 255, 0);
	ofDrawCircle(20 * (WIDTH - 2), 20 * (HEIGHT - 2), 5); // ������

	int i, j;
	ofSetColor(0, 0, 0);

	for (i = 1; i < HEIGHT - 1; i++)
	{
		for (j = 1; j < WIDTH - 1; j++)
		{
			ofSetColor(0, 0, 0);
			if (visited_2[i][j] == 1 && visited_2[i - 1][j] == 1)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited_2[i][j] == 1 && visited_2[i][j + 1] == 1) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j + 1), 20 * i);
			if (visited_2[i][j] == 1 && visited_2[i + 1][j] == 1) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
			if (visited_2[i][j] == 1 && visited_2[i][j - 1] == 1) // ����� ����
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j), 20 * i);

			ofSetColor(255, 0, 0);

			if (visited_2[i][j] == 2 && visited_2[i - 1][j] == 2)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited_2[i][j] == 2 && visited_2[i][j + 1] == 2) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j + 1), 20 * i);
			if (visited_2[i][j] == 2 && visited_2[i + 1][j] == 2) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
			if (visited_2[i][j] == 2 && visited_2[i][j - 1] == 2) // ����� ����
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j), 20 * i);

			if (visited_2[i][j] == 1 && visited_2[i - 1][j] == 2)  // ����� �� 
				ofDrawLine(20 * j, 20 * (i - 1), 20 * j, 20 * i);
			if (visited_2[i][j] == 1 && visited_2[i][j + 1] == 2) // ����� ������ 
				ofDrawLine(20 * (j), 20 * i, 20 * (j + 1), 20 * i);
			if (visited_2[i][j] == 1 && visited_2[i + 1][j] == 2) // ����� �Ʒ�
				ofDrawLine(20 * j, 20 * i, 20 * j, 20 * (i + 1));
			if (visited_2[i][j] == 1 && visited_2[i][j - 1] == 2) // ����� ����
				ofDrawLine(20 * (j - 1), 20 * i, 20 * (j), 20 * i);
		}
	}
}