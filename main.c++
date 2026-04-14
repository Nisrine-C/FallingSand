#include <stdio.h>
#include <iostream>
#include <raylib.h>
#include <vector>
#include <string>

using namespace std;

#define WIDTH 960 
#define HEIGHT 540
#define PIXEL 5
#define OFFSET 10
#define ROWS (HEIGHT/PIXEL)-OFFSET
#define COLS (WIDTH/PIXEL)-OFFSET
#define SATURATION 0.8f
#define VALUE 0.8f



enum ParticleType {
  SAND,
  WATER
};

// Classes
class Particle {
  public:
    int x, y;
    float hue;
    int speed;
    Color color;
    int stagnant;
    ParticleType type;
    Particle(int _x, int _y, Color _color, ParticleType _type) 
        : x(_x), y(_y), color(_color), type(_type) { // Updated constructor
      speed = 1;
      stagnant = 0;
    }
    void setSpeed(int speed){
      this->speed = speed;
    }

};

class Button {
public:
    std::string message;
    Rectangle rect;      // Stores x, y, width, height
    int borderThickness;
    Color background;
    Color borderColor;

    Button(std::string text, float x, float y, float w, float h, int b, Color bg, Color bc) {
        message = text;
        rect = { x, y, w, h };
        borderThickness = b;
        background = bg;
        borderColor = bc;
    }

    // Draws the button with its border
    void Draw() {
        // Draw border first (slightly larger)
        DrawRectangle(rect.x - borderThickness, rect.y - borderThickness, 
                      rect.width + (borderThickness * 2), rect.height + (borderThickness * 2), borderColor);
        
        // Draw the main button body
        DrawRectangleRec(rect, background);

        // Draw the text (centered roughly)
        int fontSize = 20;
        int textWidth = MeasureText(message.c_str(), fontSize);
        DrawText(message.c_str(), rect.x + (rect.width/2) - (textWidth/2), 
                 rect.y + (rect.height/2) - (fontSize/2), fontSize, WHITE);
    }

    // Returns true if the mouse is over the button AND clicked
    bool IsPressed() {
        return CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
    
    // Returns true just if the mouse is hovering (useful for changing colors)
    bool IsHovered() {
        return CheckCollisionPointRec(GetMousePosition(), rect);
    }
};



// Global variables
static Particle* scene[ROWS][COLS];
static std::vector<Particle*> active;
static int hueValue =1;
static int pencil_size  =1;
static string sizeStr;
static ParticleType currentMaterial = SAND;
Button sandBtn("", OFFSET*PIXEL + 100 , HEIGHT - 6 *PIXEL, 20, 20, 2, BEIGE, BROWN);
Button waterBtn("",OFFSET*PIXEL+ 100 +20 + OFFSET , HEIGHT - 6 *PIXEL, 20, 20, 2, BLUE, DARKBLUE);
// Function declarations
void static initSimulation(void); // Initialise the Scene
void static drawSimulation(void); // Draw Game ( one frame )
void static updateSimulation(void); // Update Simulation ( one frame)
void static updateDrawFrame(void); // Update and then Draw one frame 
vector<Particle*> nextActive(void); // Generate the next fram of active particles

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
  sizeStr = "Size : " + to_string(pencil_size);
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      scene[i][j] = nullptr;
    }
  }
}

void updateSimulation(){
  sizeStr = "Size : " + std::to_string(pencil_size);
  if (sandBtn.IsPressed()) currentMaterial = SAND;
  if (waterBtn.IsPressed()) currentMaterial = WATER;;
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    int x = GetMouseX() / PIXEL;
    int y = GetMouseY() / PIXEL;
    
    // Offset to center the brush on the mouse
    int offset = pencil_size / 2;

    for (int dy = 0; dy <pencil_size; dy++) {
      for (int dx = 0; dx < pencil_size; dx++) {
        // Subtracting the offset centers the square
        int nx = x + dx - offset; 
        int ny = y + dy - offset;

        if (nx >= OFFSET && nx < COLS && ny >= OFFSET && ny < ROWS) {
          if (scene[ny][nx] == nullptr) {
            Color col = (currentMaterial == SAND)? BEIGE : BLUE;
            Particle* p = new Particle(nx, ny, col,currentMaterial);
            scene[ny][nx] = p;
            active.push_back(p);
          }
        }
      }
    }
  }    //static so there is no need to reallocate space for it each frame
  float mouseWheel = GetMouseWheelMove();
  if (mouseWheel != 0) {
    pencil_size += (int)mouseWheel;
    if (pencil_size < 1) pencil_size = 1;
  }
  active= nextActive();
  //hueValue = (hueValue + 1)%360;
}

vector<Particle*> nextActive() {
    static vector<Particle*> nextActiveList;
    nextActiveList.clear();

    for (Particle* p : active) {
        bool moved = false;
        int dir = (GetRandomValue(0, 1) == 0) ? 1 : -1;

        // Sub-stepping for speed/acceleration
        for (int i = 0; i < p->speed; ++i) {
            int x = p->x;
            int y = p->y;
            if (y >= ROWS - 1) break;

            // 1. STRAIGHT DOWN (Including Sand sinking into Water)
            Particle* below = scene[y + 1][x];
            if (below == nullptr || (p->type == SAND && below->type == WATER)) {
                if (below != nullptr && below->type == WATER) {
                    // Swap Sand and Water
                    scene[y + 1][x] = p;
                    scene[y][x] = below;
                    p->y++;
                    below->y--;
                    nextActiveList.push_back(below); // Keep water active as it's pushed up
                } else {
                    scene[y][x] = nullptr;
                    p->y++;
                    scene[p->y][p->x] = p;
                }
                moved = true;
            } 
            // 2. DIAGONALS (Including Sand sinking)
            else if (x + dir >= 0 && x + dir < COLS && 
                    (scene[y + 1][x + dir] == nullptr || (p->type == SAND && scene[y + 1][x + dir]->type == WATER))) {
                Particle* diag = scene[y + 1][x + dir];
                if (diag != nullptr && diag->type == WATER) {
                    scene[y + 1][x + dir] = p;
                    scene[y][x] = diag;
                    p->y++; p->x += dir;
                    diag->y--; diag->x -= dir;
                    nextActiveList.push_back(diag);
                } else {
                    scene[y][x] = nullptr;
                    p->y++; p->x += dir;
                    scene[p->y][p->x] = p;
                }
                moved = true;
            }
            // 3. WATER SIDEWAYS FLOW
            else if (p->type == WATER) {
                if (x + dir >= 0 && x + dir < COLS && scene[y][x + dir] == nullptr) {
                    scene[y][x] = nullptr;
                    p->x += dir;
                    scene[p->y][p->x] = p;
                    moved = true;
                } else if (x - dir >= 0 && x - dir < COLS && scene[y][x - dir] == nullptr) {
                    scene[y][x] = nullptr;
                    p->x -= dir;
                    scene[p->y][p->x] = p;
                    moved = true;
                } else {
                    break; 
                }
            } else {
                break; 
            }
        }

        if (moved) {
            p->stagnant = 0;
            if (p->speed < 5) p->speed++;
            nextActiveList.push_back(p);
        } else {
            p->stagnant++;
            p->speed = 1;
            // Water stays awake longer to level out; Sand sleeps after 120 frames
            if (p->stagnant < 120 || p->type == WATER) {
                nextActiveList.push_back(p);
            }
        }
    }
    return nextActiveList;
}
void updateColor(Particle* p){
  p->hue+=0.5;
  if(p->hue > 360) p->hue=1;
  p->color = ColorFromHSV(p->hue,SATURATION,VALUE);
}
void drawSimulation(){ 
  BeginDrawing();
    ClearBackground(BLACK);
    for (int y = OFFSET; y < ROWS; y++) {
        for (int x = OFFSET; x < COLS; x++) {
            if (scene[y][x] != nullptr) {
             // updateColor(scene[y][x]);
               if(PIXEL == 1) 
                DrawPixel(x * PIXEL, y * PIXEL, scene[y][x]->color);
               else DrawRectangle(x * PIXEL, y * PIXEL, PIXEL, PIXEL, scene[y][x]->color);
                
            }
        }
    }
    
    DrawRectangle(0, HEIGHT - 8*PIXEL , WIDTH, 8*PIXEL, GRAY);
    sandBtn.Draw();
    waterBtn.Draw();
    if (currentMaterial == SAND) DrawRectangleLinesEx(sandBtn.rect, 3, YELLOW);
    else DrawRectangleLinesEx(waterBtn.rect, 3, DARKBLUE);
    DrawText(sizeStr.c_str(), OFFSET * PIXEL,HEIGHT - 6 *PIXEL, 20, WHITE);
  EndDrawing();
}

void updateDrawFrame(){
  updateSimulation();
  drawSimulation();
}
