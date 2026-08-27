# Snake Game (C++)

A classic, console-based implementation of the retro Snake game, written entirely in C++. 

## The Backstory
I built this game during a power outage when I had no internet and nothing else to do. It started as a quick time-pass project to play a classic game right in the terminal, and it turned into a fun exercise in handling C++ console rendering, input tracking, and game state management.

## Features
* **Smooth Console Rendering:** Uses Windows API cursor positioning to update the board without the flickering usually associated with clearing the console (`system("cls")`).
* **Time-Based Movement:** Utilizes the `<chrono>` library to manage frame rates and snake speed smoothly.
* **Colored UI:** Implements ANSI escape codes to render a colored terminal interface (Yellow snake, Red fruit, Blue grid).
* **Classic Mechanics:** Eat the fruit to grow, avoid hitting the walls, and avoid crashing into your own tail.

## Controls
* **Arrow Keys:** Move Up, Down, Left, Right
* **Q:** Quit the game (from the menu or during gameplay)
* **R:** Restart directly from the Game Over screen

## Download and Play (Windows)
If you want to play the game without setting up a C++ compiler, you can download the standalone Windows executable:

- Link: [Download Snake Game](https://github.com/AP-Abhishek/Snake-Game/releases/tag/v1.0.0)
- Extract the downloaded files if necessary and run the `.exe`.
- **Note:** Because this is an indie project without a paid digital signature, Windows SmartScreen may flag the executable. Click **More info** and then **Run anyway** to launch the game.

## How to Compile from Source
To compile or modify the game from the source code, you will need a C++ compiler (like GCC/MinGW) installed on your Windows machine. 

*Note: This code relies on `<windows.h>` and `<conio.h>`, making it specific to Windows environments.*

1. Clone this repository:
   ```bash
   git clone https://github.com/AP-Abhishek/Snake-Game.git
   ```
1. Navigate to the project folder:
    ```bash
    cd Snake-Game
    ```
1. Compile the code using g++:
    ```bash
    g++ snake.cpp -o SnakeGame
    ```
1. Run the compiled executable:
    ```bash
    SnakeGame.exe
    ```

## Project Structure
```
Snake-Game
├─ README.md
└─ snake.cpp
```
