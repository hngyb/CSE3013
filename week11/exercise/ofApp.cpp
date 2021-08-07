#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
    waterFall_flag = 0;
}

//--------------------------------------------------------------
void ofApp::update(){
    if(waterFall_flag==1){
        route.clear();
        water.start(dots[waterFall][0], dots[waterFall][1]);
        float water_X = water.getX();
        float water_Y = water.getY();
        float slop_A;
        float slop_B;

        while(water_Y < 728){
            water_Y++;
            for(int i = 0; i<lines.size(); i++){
                if(water_X >= lines[i][0] && water_X <= lines[i][2]){
                    slop_A = (double)(lines[i][3] - lines[i][1])/(double)(lines[i][2] - lines[i][0]);
                    // slop_B = (water_Y-lines[i][1])/(water_X-lines[i][0]);
                    if((int)water_Y == (int)(lines[i][1] + (water_X-lines[i][0])*slop_A)){
                        water_X += (1/slop_A);
                        water_Y--;
                        water_Y+=slop_A;
                        break;
                    }
                }
            }
            vector<int> tmp;
            tmp.push_back(water_X);
            tmp.push_back(water_Y);
            route.push_back(tmp);
        }
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);
    
    
    ofSetLineWidth(5);
    if( draw_flag ){
        
        
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        for (int i = 0; i<lines.size(); i++){
            ofDrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        }

        for (int i = 0; i<dots.size(); i++){
            if(i == waterFall){
                ofSetColor(255,0,0);
                ofDrawCircle(dots[i][0], dots[i][1], 10);
                ofSetColor(0, 0, 0);
            }
            else{
                ofSetColor(0, 0, 0);
                ofDrawCircle(dots[i][0], dots[i][1], 10);
            }
        }
        
        // 2nd week portion.
        if(waterFall_flag){
            for(int i = 0 ;i<spd;i++){
                ofSetColor(0,0,255);
                ofDrawCircle(route[i][0], route[i][1], 3);
            }
            
            spd=spd+10;
            if(spd > route.size()){
                spd = route.size() - 1;
                falling = 0;
            }
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        
        // Free the dynamically allocated memory exits.
        lines.clear();
        dots.clear();
        route.clear();
        vector<vector<int>>().swap(lines);
        vector<vector<int>>().swap(dots);
        vector<vector<int>>().swap(route);
        
        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if( !load_flag) return;
        
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
        draw_flag = 1;
        draw();
        
    }
    if (key == 's'){
        // 2nd week portion.
        falling = 1;
        waterFall_flag = 1;
       
    }
    if (key == 'e'){
        // 2nd week portion.
        falling = 0;
        waterFall_flag = 0;
        spd = 0;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    
    if (key == OF_KEY_RIGHT){
        if(!falling){
            falling = 0;
            waterFall_flag = 0;
            spd = 0;
            waterFall = (waterFall + 1) % num_of_dot;
            cout << "Selcted Dot Coordinate is (" << dots[waterFall][0] << ", " << dots[waterFall][1] << ")" << endl;}
    }
    if (key == OF_KEY_LEFT){
        if(!falling){
            falling = 0;
            waterFall_flag = 0;
            spd = 0;
            waterFall = (waterFall + num_of_dot - 1 ) % num_of_dot;
            cout << "Selcted Dot Coordinate is (" << dots[waterFall][0] << ", " << dots[waterFall][1] << ")" << endl;
        }
    }
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
    
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
    
    
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
        
        if( words.size() == 1){
            if(num_of_line){
                input_type = 1;
            }
            if( input_type == 0){ // Input for the ndumber of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
            }
        }
        else if (words.size() >= 2){
            int x1,y1,x2,y2;
            if( input_type == 0){ // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                
                if(y1 > 0 && y2 > 0){
                    vector<int> tmp;
                    tmp.push_back(x1);
                    tmp.push_back(y1);
                    tmp.push_back(x2);
                    tmp.push_back(y2);
                    lines.push_back(tmp);
                }
            }
            else{ // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                
                if(y1 > 0){
                    vector<int> tmp;
                    tmp.push_back(x1);
                    tmp.push_back(y1);
                    dots.push_back(tmp);
                }
            }
        } // End of else if.

    } // End of for-loop (Read file line by line).
    
    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    ;
}


