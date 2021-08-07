#pragma once

#include "ofMain.h"

class waterObj{
    private:
        float *waterPos = new float[2];
        
    public:
        waterObj(){
            waterPos[0] = 0;
            waterPos[1] = 0;
        }
        ~waterObj(){
            delete[] waterPos;
        }
        void start(int x, int y){
            waterPos[0] = x;
            waterPos[1] = y;
        }
        int getX(){
            return waterPos[0];
        }
        int getY(){
            return waterPos[1];
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
    
    /* WaterFall-related member variables Regions */
    
    // flag variables
    int draw_flag;
    int load_flag;
    
    // Line segment and dot related variables
    int num_of_line, num_of_dot;
    float dot_diameter;
    vector<vector<int>> lines;
    vector<vector<int>> dots;

    /* WaterFall-related member functions */
    waterObj water;
    int waterFall = 0;
    int waterFall_flag;
    int spd = 0;
    int falling;
    vector<vector<int>> route;
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void initializeWaterLines(); // 2nd week portion.
		
};


