#include "ofApp.h"
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15);
    ofBackground(255, 255, 255);
    
    start_flag = false;
    draw_flag = true;
    
    height = 20;
    width = 20;
    cell_size = 20;
    
    ofSetWindowShape(width*cell_size*1.2, height*cell_size*1.2); // 윈도우창 사이즈 조절
    
    windowWidth = ofGetWidth();
    windowHeight = ofGetHeight();
    
    // 윈도우창 중앙 위치
    ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);
    
    for (int row = 0; row < height; row++){ // 미로의 높이, 넓이 정보를 활용하여 maze를 초기화한다.
        for (int column = 0; column < width; column++){
            maze.push_back(Cell(row, column));
        }
    }
    current = &maze[0]; // maze[0](cell(0,0))을 현재 셀로 초기화한다.
    
}

//--------------------------------------------------------------
void ofApp::update(){
    /*
     미로를 생성하는 함수이다.
     */
    if (start_flag == true){
        int speed = 1; // draw 속도(미로 탐색 속도)를 조절하기 위한 변수, 값이 증가할수록 화면이 보이는 draw 속도가 빨라진다.
        while(speed > 0){
            current->setVisited(true); // 현재 셀을 방문 표시한다
            Cell *next = findNextCell(); // 현재 셀을 기준으로 다음 방문할 셀을 찾는다.
            
            // 방문할 셀이 존재한다면 방문 표시를 한 뒤,
            // 스택에 현재 셀을 push하고 현재 셀과 다음 셀을 연결한다.
            // 현재 셀을 다음 셀로 이동한다.
            if (next != NULL){
                next->setVisited(true);
                s.push(current);
                current->connectCell(*next);
                current = next;
            }
            else if (s.size() > 0){ // 스택이 비어있지 않다면, 현재 셀을 스택의 top으로 이동한다.
                current = s.top();
                s.pop();

            } else if (s.size() == 0){ // 스택의 사이즈가 0이라면 탐색이 종료된다.
                break;
            }
            
            speed--;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (draw_flag == true){
        // 윈도우 사이즈에 맞추어 미로의 크기를 조절한다.
        while(height * cell_size >= ofGetHeight() || width * cell_size >= ofGetWidth()){
            cell_size = cell_size * 0.9;
        }
        while(height * cell_size <= ofGetHeight() * 0.8 && width * cell_size <= ofGetWidth() * 0.8){
            cell_size = cell_size * 1.1;
        }
        
        float x1, y1, x2, y2;
        size_t i;
        int w = 0;
        int h = 0;
        
        ofSetColor(ofColor::black);
        ofSetLineWidth(cell_size*0.1);
        
        // 미로 정보를 통해 미로를 그린다. (line 사용)
        for (i = 0; i < maze.size(); i++){
            w = i % width;
            bool *walls = maze[i].getWalls();
            if (walls[0]){ // 위쪽 벽을 그린다.
                x1 = w * cell_size;
                x2 = (w+1) * cell_size;
                y1 = h * cell_size;
                y2 = h * cell_size;
                ofDrawLine(x1, y1, x2, y2);
            }
            if (walls[1]){ // 오른쪽 벽을 그린다.
                x1 = (w+1) * cell_size;
                x2 = x1;
                y1 = h * cell_size;
                y2 = (h+1) * cell_size;
                ofDrawLine(x1, y1, x2, y2);
            }
            if (walls[2]){ // 아래쪽 벽을 그린다.
                x1 = w * cell_size;
                x2 = (w+1) * cell_size;
                y1 = (h+1) * cell_size;
                y2 = (h+1) * cell_size;
                ofDrawLine(x1, y1, x2, y2);
            }
            if (walls[3]){ // 왼쪽 벽을 그린다.
                x1 = w * cell_size;
                x2 = x1;
                y1 = h * cell_size;
                y2 = (h + 1) * cell_size;
                ofDrawLine(x1, y1, x2, y2);
            }
            
            if((i+1) % width == 0){
                h = h + 1;
            }
        }
        
        // maze generator 시각화
        // 빨간 동그라미가 움직이며 실시간으로 미로를 생성한다. (DFS의 탐색 경로)
        int current_y = current->getRow();
        int current_x = current->getColumn();
        ofSetColor(ofColor::red);
        ofDrawCircle((current_x*cell_size + (current_x+1)*cell_size)/2, (current_y*cell_size + (current_y+1)*cell_size)/2, cell_size*0.2);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's'){ // 미로 생성 시작
        start_flag = true;
    }
    
    if (key == 'p'){ // 미로 생성 일시정지
        start_flag = false;
    }
    
    if (key == 'q'){ // 프로그램 종료 및 메모리 할당 해제
        draw_flag = false;
        
        while(!s.empty()) s.pop();
        maze.clear();
        maze.shrink_to_fit();
        
        cout << "Dynamically allocated memory has been freed." << endl;
        _Exit(0);
    }
    
    if (key == 'c'){ // 미로 정보 변경 (사용자 입력)
        cout << "Height of Maze: ";
        cin >> height;
        cout << "width of Maze: ";
        cin >> width;
        
        // 미로의 최대 높이, 넓이 제한
        if (height > 40) height = 40;
        if (width > 40) width = 40;
        
        start_flag = false;
        draw_flag = false;
        
        maze.clear();
        maze.shrink_to_fit();
        while(!s.empty()) s.pop();
        
        for (int row = 0; row < height; row++){
            for (int column = 0; column < width; column++){
                maze.push_back(Cell(row, column));
            }
        }
        current = &maze[0];
        
        start_flag = true;
        draw_flag = true;
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

Cell *ofApp::findNextCell(){
    /*
    현재 셀(current)을 기준으로 방문 가능한 다음 셀을 찾는 함수이다.
    Cell의 pointer를 반환한다.
     */
    vector<Cell *> neighbors; // 이웃 셀의 정보를 담을 vector를 초기화한다.
    int currentRow = current->getRow();
    int currentColumn = current->getColumn();
    
    int neighborPos[4]; // 이웃 셀들의 위치 정보
    neighborPos[0] = getPos(currentRow - 1, currentColumn);
    neighborPos[1] = getPos(currentRow, currentColumn - 1);
    neighborPos[2] = getPos(currentRow + 1, currentColumn);
    neighborPos[3] = getPos(currentRow, currentColumn + 1);
    
    for (int i = 0; i < 4; i++){ // 이웃 셀들 중에서 방문이 가능하고, 아직 방문하지 않은 셀들을 neighbors에 저장한다.
        if(neighborPos[i]!= -1 && !maze[neighborPos[i]].isVisited()){
            neighbors.push_back(&maze[neighborPos[i]]);
        }
    }
    
    if(neighbors.size() > 0){ // neighbors 중에서 방문할 다음 셀을 랜덤하게 선택한다.
        return neighbors.at(rand() % neighbors.size());
    }
    
    return NULL; // 방문 가능한 이웃 셀이 없다면 NULL을 반환한다.
}

int ofApp::getPos(int row, int column){
    /*
     파라미터로 받은 row, column 값이 maze내 존재 가능한지 파악한다.
     존재할 수 없는 위치이면, -1을 반환하고, 존재 가능하다면, 셀의 인덱스를 반환한다.
     */
    if(row < 0 || column < 0 || column > width -1 || row > height - 1)
        return -1;
    else
        return column + row * width; // maze는 일차원으로 저장되어 있기 때문에 인덱스 조정이 필요하다.
}
