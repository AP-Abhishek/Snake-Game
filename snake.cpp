#include<iostream>
#include<conio.h>
#include<windows.h>
#include<chrono>
#include<thread>
#include<vector>
#include<deque>

using namespace std;

const int BOARD_HEIGHT = 10;
const int BOARD_WIDTH = 10;
const int TARGET_FPS = 4;
const int SNAKE_SPEED = 250;
const int INITIAL_SNAKE_LENGTH = 3;
enum Directions { Quit, Left, Right, Up, Down, None };

const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BOLD_YELLOW = "\033[1;33m";
const string BLUE = "\033[34m";
const string BOLD_CYAN = "\033[1;36m";

void getStartPosition(int &snake_x, int &snake_y, int &snake_direction,  deque<vector<int>> &snake);
void updateBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], deque<vector<int>> &snake, vector<int> &fruit_pos);
void windowSetup();
void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH]);
Directions getDirection();
vector<int> spawnFruit(deque<vector<int>> &snake);

int main()
{
    // Game Variables
    bool running = true;
    bool playing = false;
    char board[BOARD_HEIGHT][BOARD_WIDTH] = {0};
    
    int score = 0;

    int snake_x = 0;
    int snake_y = 0;
    int snake_direction = 0;
    deque<vector<int>> snake;

    vector<int> fruit_pos = {0, 0};
    bool fruit_consumed = false;

    // Game setup
    getStartPosition(snake_x, snake_y, snake_direction, snake);
    fruit_pos = spawnFruit(snake);
    updateBoard(board, snake, fruit_pos);
    windowSetup();

    // Game Loop
    auto last_movement_time = chrono::high_resolution_clock::now();
    const chrono::milliseconds frameDuration(1000/TARGET_FPS);
    while (running)
    {
        if (!playing)
        {
            cout << BOLD_YELLOW << " ===== " << BOLD_CYAN << " Snake Game " << BOLD_YELLOW << " =====" << RESET << endl;
            cout << " Press any key to start the game..." << endl;
            _getch();
            playing = true;
        }

        auto frame_start = chrono::high_resolution_clock::now();

        // Input Handling
        Directions direction = getDirection();
        if (direction == Directions::Quit)
        {
            running = false;
            break;
        }
        else if (direction != Directions::None)
        {
            bool isOpposite = 
                (direction == Directions::Left && snake_direction == Directions::Right) || 
                (direction == Directions::Right && snake_direction == Directions::Left) || 
                (direction == Directions::Up && snake_direction == Directions::Down) || 
                (direction == Directions::Down && snake_direction == Directions::Up);

            if (!isOpposite) {
                snake_direction = direction;
            }
        }

        auto current_time = chrono::high_resolution_clock::now();
        auto time_since_last_move = chrono::duration_cast<chrono::milliseconds>(current_time - last_movement_time);

        if (time_since_last_move.count() >= SNAKE_SPEED)
        {
            int next_x = snake_x;
            int next_y = snake_y;
            switch(snake_direction)
            {
            case Directions::Left:
                next_x--;
                break;
            case Directions::Right:
                next_x++;
                break;
            case Directions::Up:
                next_y--;
                break;
            case Directions::Down:
                next_y++;
                break;
            }
            
            // Board Collsion
            if (next_x < 0 || next_x >= BOARD_WIDTH || next_y < 0 || next_y >= BOARD_HEIGHT)
            {
                running = false;
                break;
            }
           
            // Fruit Collision
            if (vector<int>({next_x, next_y}) == fruit_pos)
            {
                score++;
                fruit_consumed = true;
            }
        
            // Self Collision
            for (int i = 0 ; i < snake.size() ; i++)
            {
                if (next_x == snake[i][0] && next_y == snake[i][1])
                {
                    running = false;
                    break;
                }
            }

            if (!running)
            {
                break;
            }
         
            snake_x = next_x;
            snake_y = next_y;
            snake.push_front({ snake_x, snake_y });
          
            if (!fruit_consumed) {
                snake.pop_back();
            }
            else
            {
                fruit_pos = spawnFruit(snake);
            }
            
            updateBoard(board, snake, fruit_pos);
            fruit_consumed = false;
            last_movement_time = current_time;
        }

        // Screen Refreshing
        COORD cursor_position;
        cursor_position.X = 0;
        cursor_position.Y = 0;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor_position);

        // system("cls");
        cout << BOLD_YELLOW << " ===== " << BOLD_CYAN << " Snake Game " << BOLD_YELLOW << " =====" << RESET << endl;
        cout << " Press " << RED << "'Q'" << RESET << " or " << RED << "'q'" << RESET << " to quit\n" << endl;
        cout << BOLD_YELLOW << "   === " << GREEN << "SCORE: " << score << BOLD_YELLOW << " ===\n" << RESET << endl;
        printBoard(board);

        // Frame-rate managing
        auto frame_end = chrono::high_resolution_clock::now();
        auto elapsed_time = chrono::duration_cast<chrono::milliseconds>(frame_end - frame_start);
        if (elapsed_time < frameDuration)
        {
            this_thread::sleep_for(frameDuration - elapsed_time);
        }
    }

    if (!running)
    {
        cout << endl << BOLD_YELLOW << " ===== " << RED << "Game Over" << BOLD_YELLOW << " =====" << RESET << endl;
    }
    
    return 0;
}

void getStartPosition(int &snake_x, int &snake_y, int &snake_direction,  deque<vector<int>> &snake)
{
    srand(time(0));
    do {
        snake_x = rand() % BOARD_HEIGHT;
    } while (snake_x < 3 || snake_x > 7);
    
    do {
        snake_y = rand() % BOARD_WIDTH;
    } while (snake_y < 3 || snake_y > 7);

    snake_direction = (rand() % 4) + 1;

    switch(snake_direction)
    {
    case Directions::Left:
        for (int i = 0 ; i < INITIAL_SNAKE_LENGTH ; i++)
        {
            snake.push_back({ snake_x, snake_y });
            snake_x++;
        }
        break;
    case Directions::Right:
        for (int i = 0 ; i < INITIAL_SNAKE_LENGTH ; i++)
        {
            snake.push_back({ snake_x, snake_y });
            snake_x--;
        }
        break;
    case Directions::Up:
        for (int i = 0 ; i < INITIAL_SNAKE_LENGTH ; i++)
        {
            snake.push_back({ snake_x, snake_y });
            snake_y++;
        }
        break;
    case Directions::Down:
        for (int i = 0 ; i < INITIAL_SNAKE_LENGTH ; i++)
        {
            snake.push_back({ snake_x, snake_y });
            snake_y--;
        }
        break;
    }
    snake_x = snake[0][0];
    snake_y = snake[0][1];
}

void updateBoard(char board[BOARD_HEIGHT][BOARD_WIDTH], deque<vector<int>> &snake, vector<int> &fruit_pos)
{
    for (int i = 0 ; i < BOARD_HEIGHT ; i++)
    {
        for (int j = 0 ; j < BOARD_WIDTH ; j++)
        {
            board[i][j] = 'X';
        }
    }

    for (int i = 0 ; i < snake.size() ; i ++)
    {
        board[snake[i][1]][snake[i][0]] = 'O';
    }

    board[fruit_pos[1]][fruit_pos[0]] = '0';
}

void windowSetup()
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 20;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console_handle, &info);
    system("cls");
}

Directions getDirection()
{
    if (!_kbhit())
    {
        return Directions::None;
    }

    int input = _getch();

    if (input == 224 || input == 0)
    {
        int direction = _getch();
        switch (direction)
        {
        case 72:
            return Directions::Up;
        case 80:
            return Directions::Down;
        case 75:
            return Directions::Left;
        case 77:
            return Directions::Right;
        default:
            return Directions::None;
        }
    }
    
    if (input == 'Q' || input == 'q')
    {
        return Directions::Quit;
    }

    return Directions::None;
}

void printBoard(char board[BOARD_HEIGHT][BOARD_WIDTH])
{
    for (int i = 0 ; i < BOARD_HEIGHT ; i++)
    {
        cout << "  ";
        for (int j = 0 ; j < BOARD_WIDTH ; j++)
        {
            if (board[i][j] == 'O')
            {
                cout << YELLOW << 'O' << RESET << " ";
            }
            else if (board[i][j] == '0')
            {
                cout << RED << '0' << RESET << " ";
            }
            else
            {
                cout << BLUE << 'X' << RESET << " ";
            }
        }
        cout << endl;
    }
}

vector<int> spawnFruit(deque<vector<int>> &snake)
{
    int x = 0;
    int y = 0;

    auto isPlaceable = [&](int x, int y)
    {
        for (auto pos: snake)
        {
            if (x == pos[0] && y == pos[1]) 
            {
                return false;
            }
        }
        return true;
    };

    do {
        x = rand() % BOARD_WIDTH;
        y = rand() % BOARD_HEIGHT;
    } while (!isPlaceable(x,y));

    return {x, y};
}
