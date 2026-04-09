#include <stdio.h>
#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

#define WIDTH 800 
#define HEIGHT 600 
#define PIXEL 5
#define ROWS HEIGHT/PIXEL
#define COLS WIDTH/PIXEL
#define PENCIL_SIZE 4
#define SATURATION 0.8f
#define VALUE 0.8f

// Classes
class Particle {
  public:
    int x, y;
    float hue;
    int speed;
    Color color;

    Particle(int _x, int _y, float _hue) : x(_x), y(_y), hue(_hue) {
      color = ColorFromHSV(hue,SATURATION,VALUE);
      speed=1;
    }

    void setSpeed(int speed){
      this->speed = speed;
    }

};

// Global variables
static Particle* scene[ROWS][COLS];
static std::vector<Particle*> active;
static int hueValue =1;

// Function declarations
void static initSimulation(void); // Initialise the Scene
void static drawSimulation(void); // Draw Game ( one frame )
void static updateSimulation(void); // Update Simulation ( one frame)
void static updateDrawFrame(void); // Update and then Draw one frame 


int main(int argc,char *argv[]){
  // Initialisation 
  InitWindow(WIDTH,HEIGHT,"Falling sand");
  initSimulation();

  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    updateDrawFrame();
  }
  CloseWindow();
  return 0;
}


void initSimulation() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      scene[i][j] = nullptr;
    }
  }
  // Pre allocation for potential sand pieces to maybe stop stuttering 
  active.reserve(ROWS*COLS);
}

void updateSimulation(){
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      int x = GetMouseX() / PIXEL;
      int y = GetMouseY() / PIXEL;
    
      // Offset to center the brush on the mouse
      int offset = PENCIL_SIZE / 2;

      for (int dy = 0; dy < PENCIL_SIZE; dy++) {
        for (int dx = 0; dx < PENCIL_SIZE; dx++) {
            // Subtracting the offset centers the square
            int nx = x + dx - offset; 
            int ny = y + dy - offset;

            if (nx >= 0 && nx < COLS && ny >= 0 && ny < ROWS) {
                if (scene[ny][nx] == nullptr) {
                    Particle* p = new Particle(nx, ny, hueValue);
                    scene[ny][nx] = p;
                    active.push_back(p);
                }
            }
        }
    }
    // Increment hue slightly each frame for a smooth rainbow
    hueValue += 1.0f;
    if (hueValue > 360) hueValue = 0;
}    //static so there is no need to reallocate space for it each frame
    static vector<Particle*> nextActive;
    nextActive.clear();

    for (Particle* p : active){
      int x = (int)p->x;
      int y = (int)p->y;
      
      if(y>= ROWS-1){
        continue;
      }
      bool moved=false;
      if(scene[y+1][x] == nullptr) { 
        scene[y][x] = nullptr;
        p->y++;
        scene[p->y][x]=p;
        nextActive.push_back(p);
        moved = true;
      } 

      else{
        int dir = (GetRandomValue(0, 1) == 0) ? 1 : -1;
;

        if (x + dir >= 0 && x + dir < COLS && scene[y + 1][x + dir] == nullptr) {
          scene[y][x] = nullptr;
          p->y = y++;
          p->x +=dir;
          scene[p->y][p->x] = p;
          nextActive.push_back(p);
          moved=true;
        } else if (x - dir >= 0 && x - dir < COLS && scene[y + 1][x - dir] == nullptr) {
          scene[y][x] = nullptr;
          p->y++;
          p->x -=dir;
          scene[p->y][p->x] = p;
          nextActive.push_back(p);
          moved=true;
        }
        if (!moved) nextActive.push_back(p);
      } 
    }
    active= nextActive;
    hueValue = (hueValue + 1)%360;
}

void drawSimulation(){ 
  BeginDrawing();
    ClearBackground(BLACK);
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            if (scene[y][x] != nullptr) {
                scene[y][x]->color= ColorFromHSV(scene[y][x]->hue+0.75,SATURATION,VALUE);
                scene[y][x]->hue +=0.75;
               if(PIXEL == 1) 
                DrawPixel(x * PIXEL, y * PIXEL, scene[y][x]->color);
               else DrawRectangle(x * PIXEL, y * PIXEL, PIXEL, PIXEL, scene[y][x]->color);
                
            }
        }
    }
  EndDrawing();
}

void updateDrawFrame(){
  updateSimulation();
  drawSimulation();
}
