#include <stdio.h>
#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

#define WIDTH 800 
#define HEIGHT 800 
#define PIXEL 10
#define ROWS HEIGHT/PIXEL
#define COLS WIDTH/PIXEL

int scene[ROWS][ROWS];

void static initSimulation(void); // Initialise the Scene
void static drawSimulation(void); // Draw Game ( one frame )
void static updateSimulation(void); // Update Simulation ( one frame)
void static updateDrawFrame(void); // Update and then Draw one frame 

vector<Vector2> active;

int main(int argc,char *argv[]){
  InitWindow(WIDTH,HEIGHT,"test");
  SetTargetFPS(60);
  initSimulation();
  while(!WindowShouldClose()) {
    updateDrawFrame();
  }
  CloseWindow();
  return 0;
}

void initSimulation() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            scene[i][j] = 0;
        }
    }
    // Pre allocation for potential sand pieces to maybe stop stuttering 
    active.reserve(ROWS*COLS);
}

void updateSimulation(){
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      int x = GetMouseX()/PIXEL;
      int y = GetMouseY()/PIXEL;
      //cout << "(" << GetMouseY()/PIXEL << "," << GetMouseX()/PIXEL << ")" << endl;
      for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 1; dx >= 0; --dx) {
          int nx = x + dx;
          int ny = y + dy;

          if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
            if (scene[ny][nx] == 0) {
              scene[ny][nx] = 1;
              active.push_back({(float)nx, (float)ny});
            }
          }
        }
      }   
    }

    //for(int i = ROWS-2 ; i>= 0 ; --i){
     // for( int j = 0 ; j < COLS ; ++j){
     //   if(scene[i][j] == 1 && scene[i+1][j] == 0 ) { 
         // scene[i][j] = 0;
         // scene[i+1][j] = 1 ;
       // }
     // }
    //}
    //static so there is no need to reallocate space for it each frame
    static vector<Vector2> nextActive;
    nextActive.clear();

    for (const auto& particle : active){
      int x = (int)particle.x;
      int y = (int)particle.y;
      
      if(y>= ROWS-1){
        continue;
      }

      if(scene[y+1][x] == 0) { 
        scene[y][x] = 0;
        scene[y+1][x] = 1 ;
        nextActive.push_back({(float)x,(float)(y+1)});
      } 

      else{
        int dir = (GetRandomValue(0, 1) == 0) ? 1 : -1;
        bool moved=false;

        if (x + dir >= 0 && x + dir < COLS && scene[y + 1][x + dir] == 0) {
          scene[y][x] = 0;
          scene[y + 1][x + dir] = 1;
          nextActive.push_back({(float)(x + dir), (float)(y + 1)});
          moved=true;
        } else if (x - dir >= 0 && x - dir < COLS && scene[y + 1][x - dir] == 0) {
          scene[y][x] = 0;
          scene[y + 1][x - dir] = 1;
          nextActive.push_back({(float)(x - dir), (float)(y + 1)});
          moved=true;
        }
        if (!moved) nextActive.push_back({(float)x,(float)y});
      } 
    }
    active= nextActive;

}

void drawSimulation(){ 
  BeginDrawing();
    for(int i=0 ; i < ROWS; ++i){
      for(int j=0 ; j < COLS; ++j){
        if(scene[i][j] == 1) DrawRectangle(j*PIXEL,i*PIXEL, PIXEL, PIXEL,WHITE);
        if(scene[i][j] == 0) DrawRectangle(j*PIXEL,i*PIXEL,PIXEL, PIXEL,BLACK);
      }
    }
  EndDrawing();
}

void updateDrawFrame(){
  updateSimulation();
  drawSimulation();
}
