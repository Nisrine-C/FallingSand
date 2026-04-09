#include <stdio.h>
#include <iostream>
#include <raylib.h>
#include <list>

using namespace std;

#define WIDTH 800 
#define HEIGHT 800 
#define PIXEL 10
#define ROWS HEIGHT/PIXEL
#define COLS WIDTH/PIXEL

int scene[ROWS][ROWS];

void static initSimulation(void); // initialise the scene
void static drawSimulation(void); // Draw Game ( one frame )
void static updateSimulation(void); // Update Simulation ( one frame)
void static updateDrawFrame(void); // Update and then Draw one frame 

list<Vector2> active;

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
}

void updateSimulation(){
    if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      int x = GetMouseX()/PIXEL;
      int y = GetMouseY()/PIXEL;
      //cout << "(" << GetMouseY()/PIXEL << "," << GetMouseX()/PIXEL << ")" << endl;
      if(x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        active.push_back(Vector2({(float)x,(float)y+1}));
        active.push_back(Vector2({(float)x+1,(float)y+1}));
        active.push_back(Vector2({(float)x,(float)y}));
        active.push_back(Vector2({(float)x+1,(float)y}));
        scene[y+1][x] =1;
        scene[y+1][x+1]=1;
        scene[y][x] = 1; 
        scene[y][x+1] =1;
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
    list<Vector2> nextActive;
    for ( auto it = active.begin(); it != active.end() ;){
     // cout << "(" << it->y << "," << it->x << ")" << endl;
      int x = (int)it->x;
      int y = (int)it->y;
      int rand = GetRandomValue(0,1);
      
      if(y>= ROWS-1){
        it++;
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
      it++;
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
