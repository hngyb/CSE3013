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
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

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
		bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			isdfs = true;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		bShowInfo = bChecked;
		// doTopmost(bChecked); // Use the checked value directly

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
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	int i, j;
	float x = 0;
	float y = 0;
	scale = (double)ofGetHeight() / (double)(wall_size * ((HEIGHT) / 2) + wall_width * (HEIGHT - (HEIGHT) / 2));

	ofSetColor(ofColor::black);

	for (i = 0; i < HEIGHT; i++) {
		y = wall_size * (i / 2) + wall_width * (i - i/2);
		y = y * scale;
		for (j = 0; j < WIDTH; j++) {
			x = wall_size * (j / 2) + wall_width * (j - j/2);
			x = x * scale;
			if (f[i][j] == '+') {
				ofDrawRectangle(x, y, wall_width*scale, wall_width*scale);
			}
			else if(f[i][j] == '-') {
				ofDrawRectangle(x, y, wall_size*scale, wall_width*scale);
			}
			else if (f[i][j] == '|') {
				ofDrawRectangle(x, y, wall_width*scale, wall_size*scale);
			}
		}
	}

	if (isdfs)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen)
			path_draw();
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
		printf("file name is %s\n", fileName);
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


			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다
			if (buffer.size()) {
				for (auto line : buffer.getLines()) {
					f.push_back(line);
				}
			}
			HEIGHT = f.size();
			WIDTH = f[0].size();

			mark.resize(HEIGHT);
			visited.resize(HEIGHT);
			shortest_path.resize(HEIGHT);

		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	f.clear();
	f.shrink_to_fit();

	mark.clear();
	mark.shrink_to_fit();

	visited.clear();
	visited.shrink_to_fit();

	shortest_path.clear();
	shortest_path.shrink_to_fit();

}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)
	dest_x = WIDTH - 2;
	dest_y = HEIGHT - 2;
	bool arrival_flag= false;
	int x, y;
	int next_x, next_y;
	int moved_x, moved_y;
	
	stack< pair<int, int>> s;
	vector<vector<pair<int, int>>> moved(HEIGHT);
	for (int i = 0; i < HEIGHT; i++)
		moved[i].resize(WIDTH);


	for (int i = 0; i < HEIGHT; i++) {
		mark[i].resize(WIDTH);
		fill(mark[i].begin(), mark[i].end(), false);
	}
	for (int i = 0; i < HEIGHT; i++) {
		visited[i].resize(WIDTH);
		fill(visited[i].begin(), visited[i].end(), false);
	}
	for (int i = 0; i < HEIGHT; i++) {
		shortest_path[i].resize(WIDTH);
		fill(shortest_path[i].begin(), shortest_path[i].end(), false);
	}


	s.push({ 1, 1 });
	visited[1][1] = true;
	while (s.size() && !arrival_flag) {
		y = s.top().first;
		x = s.top().second;
		mark[y][x] = true;
		s.pop();

		for (int d = 0; d < 4; d++) {
			next_y = y + dirY[d];
			next_x = x + dirX[d];

			if (visited[next_y][next_x])
				continue;
			if (f[next_y][next_x] != ' ')
				continue;

			visited[next_y][next_x] = true;
			s.push({ next_y, next_x });
			moved[next_y][next_x] = { y, x };

			if (next_y == dest_y && next_x == dest_x) {
				arrival_flag = true;
				break;
			}
		}
	}

	if (arrival_flag) {
		y = dest_y;
		x = dest_x;
		shortest_path[y][x] = true;

		while (y != 1 || x != 1) {
			moved_y = moved[y][x].first;
			moved_x = moved[y][x].second;
			shortest_path[moved_y][moved_x] = true;
			y = moved_y;
			x = moved_x;
		}
	}
	moved.clear();
	moved.shrink_to_fit();

	return true;
}
void ofApp::path_draw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	float x, y;
	int i, j;
	ofSetColor(ofColor::gray);
	for (i = 0; i < HEIGHT; i++) {
		y = wall_size * (i / 2) + wall_width * (i - i / 2) +
			(float)wall_size / 2 - (float)wall_width / 2;
		y *= scale;
		for (j = 0; j < WIDTH; j++) {
			x = wall_size * (j / 2) + wall_width * (j - j / 2) +
				(float)wall_size / 2 - (float)wall_width / 2;
			x *= scale;
			if (mark[i][j]) {
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, wall_width * scale, wall_width * scale);
				}
				else if (i % 2) {
					ofDrawRectangle(x - wall_size * scale + wall_width * scale, y, wall_size * scale, wall_width * scale);
				}
				else {
					ofDrawRectangle(x, y - wall_size * scale + wall_width * scale, wall_width * scale, wall_size * scale);
				}
			}
		}
	}

	for (i = 0; i < HEIGHT; i++) {
		y = wall_size * (i / 2) + wall_width * (i - i / 2) +
			(float)wall_size / 2 - (float)wall_width / 2;
		y *= scale;
		for (j = 0; j < WIDTH; j++) {
			float x = wall_size * (j / 2) + wall_width * (j - j / 2) +
				(float)wall_size / 2 - (float)wall_width / 2;
			x *= scale;
			if (shortest_path[i][j]) {
				ofSetColor(ofColor::red);
				if ((i == 1 && j == 1) || (i == dest_y && j == dest_x)) {
					ofSetColor(ofColor::green);
				}
				if ((i % 2) && (j % 2)) {
					ofDrawRectangle(x, y, wall_width * scale, wall_width * scale);
				}
				else if (i % 2) {
					ofDrawRectangle(x - wall_size * scale + wall_width * scale, y, wall_size * scale, wall_width * scale);
				}
				else {
					ofDrawRectangle(x, y - wall_size * scale + wall_width * scale, wall_width * scale, wall_size * scale);
				}
			}
		}
	}
}

