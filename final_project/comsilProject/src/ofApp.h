#pragma once

#include "ofMain.h"

class Cell { // 미로의 셀을 구성하는 클래스
private:
    int row; // 셀의 row 값(y좌표)
    int column; // 셀의 column 값(x좌표)
    bool walls[4]; // 셀의 상하좌우 벽
    bool visited; // 셀 방문 여부
    int direction[4] = {0, 1, 2, 3}; // 0: top, 1: right, 2: bottom, 3: left
    
public:
    Cell(int row, int column){
        this->row = row;
        this->column = column;
        visited = false;
        
        for (int i = 0; i < 4; i++){ // 셀의 상하좌우 벽을 세운다.
            walls[i] = true;
        }
    };
    
    void setVisited(bool b){ // 방문 표시
        visited = b;
    }
    
    bool isVisited(){ // 방문 여부 확인
        return visited;
    }
    
    int getRow(){ // 셀의 row 값 return
        return row;
    }
    
    int getColumn(){ // 셀의 column 값 return
        return column;
    }
    
    bool *getWalls(){ // 셀의 벽 정보 return
        return walls;
    }
    
    void connectCell(Cell &next){ // 파라미터로 받은 셀과 연결한다.
        if (column > next.column) { // 다음 셀이 현재 셀의 왼쪽에 위치
          walls[3] = false; // 현재 셀의 왼쪽 벽을 제거한다.
          next.walls[1] = false; // 다음 셀의 오른쪽 벽을 제거한다.
        } else if (column < next.column) { // 다음 셀이 현재 셀의 오른쪽에 위치
          walls[1] = false; // 현재 셀의 오른쪽 벽을 제거한다.
          next.walls[3] = false; // 다음 셀의 왼쪽 벽을 제거한다.
        } else if (row > next.row) { // 다음 셀이 현재 셀의 위쪽에 위치
          walls[0] = false; // 현재 셀의 위쪽 벽을 제거한다.
          next.walls[2] = false; // 다음 셀의 아래쪽 벽을 제거한다.
        } else if (row < next.row) { // 다음 셀이 현재 셀의 아래쪽에 위치
          walls[2] = false; // 현재 셀의 아래쪽 벽을 제거한다.
          next.walls[0] = false; // 다음 셀의 위쪽 벽을 제거한다.
        }
    }
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        Cell *findNextCell();
        int getPos(int row, int column);
    
    float windowWidth, windowHeight;
    
    bool start_flag; // 시작 flag
    bool draw_flag; // draw flag
    
    int width; // 미로의 넓이
    int height; // 미로의 높이
    int cell_size; // 셀의 크기
    
    vector<Cell> maze; // Cell로 이루어진 미로 정보
    stack<Cell *> s; // DFS를 위한 스택
    Cell *current; // 현재 셀
};
