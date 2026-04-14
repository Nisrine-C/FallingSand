# Falling Sand

This is a small project I built using **C++** and **Raylib**. A "falling sand" simulation where you can click around and watch colored grains of sand pile up and slide around in real-time.

![Gif](fallingSand.gif)
## How it works
I used a **Particle Class** and a **Vector** to keep track of the sand. 

* **Colors:** The sand changes color as you draw, creating a rainbow effect using HSV values.
* **Physics:** The sand slides off piles and fills in gaps.

## Controls
* **Left Click:** Hold and drag to spawn sand.
* **That's it.** 

## How to run it
+ Use make to compile.
+ The executable is in the bin folder.
+ I used the linux libraries so despite it being on Windows I believe you can compile it if you modify the MakeFile.

```bash
make
./bin/falling_sand.exe
```

## To-Do List
* [ ] Add different materials (water or stone?).
* [ ] Add an eraser.
* [ ] Make it handle even more particles without lagging. 


