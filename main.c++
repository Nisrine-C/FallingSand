#include <stdio.h>
#include <iostream>
#include <raylib.h>

using namespace std;

#define WIDTH 800 
#define HEIGHT 800 
#define PIXEL 10
#define ROWS HEIGHT/PIXEL
#define COLS WIDTH/PIXEL

int scene[COLS][ROWS];

void DrawSimulation(){ 
  BeginDrawing();
    ClearBackground(WHITE);
    for(int i=0 ; i < COLS; ++i){
      for(int j=ROWS-1 ; j > 0; --j){
        if(scene[i][j] == 1) DrawRectangle(i*PIXEL,j*PIXEL, PIXEL, PIXEL,RED);
        if(scene[i][j] == 0) DrawRectangle(i*PIXEL,j*PIXEL,PIXEL, PIXEL,WHITE);
      }
    }
  EndDrawing();
}



int main(int argc,char *argv[]){
  InitWindow(WIDTH,HEIGHT,"test");
  SetTargetFPS(60);
  for(int i=0 ; i < COLS ; ++i){
    for(int j=0 ; j < ROWS; ++j){
      scene[i][j] = 0; 
    }
  }

  ClearBackground(RAYWHITE);
  while(!WindowShouldClose()) {
    DrawSimulation();
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      scene[GetMouseX()/PIXEL][GetMouseY()/PIXEL] = 1;
      cout << "(" << GetMouseX() / PIXEL << ","<< GetMouseY() / PIXEL << ")";
      cout << endl;
      //changeState();
    }
  }
  CloseWindow();
  return 0;
}
