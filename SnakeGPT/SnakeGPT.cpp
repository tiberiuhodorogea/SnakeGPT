#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream> // Include the fstream header for file operations
#include <thread>
 
using namespace std;

const int width = 40;  // Make the arena wider (4 times the initial width)
const int height = 20; // Make the arena taller (2 times the initial height)
int snakeX, snakeY, fruitX, fruitY;
int tailX[100], tailY[100];
int nTail;
enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirecton dir;
bool gameOver;
int score = 0;
int enemyX, enemyY; // Enemy variables

void Setup() {
    gameOver = false;
    dir = STOP;
    snakeX = width / 2;
    snakeY = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    nTail = 0;
    enemyX = rand() % width; // Initialize enemy position
    enemyY = rand() % height;
}

void Draw() {
    system("cls"); // Clear the console
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                cout << "#"; // Left border
            if (i == snakeY && j == snakeX)
                cout << "O"; // Snake's head
            else if (i == fruitY && j == fruitX)
                cout << "F"; // Fruit
            else if (i == enemyY && j == enemyX)
                cout << "P"; // Pirate (Enemy)
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o"; // Snake's tail
                        printTail = true;
                    }
                }
                if (!printTail)
                    cout << " ";
            }
            if (j == width - 1)
                cout << "#"; // Right border
        }
        cout << endl;
    }

    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}
void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = snakeX;
    tailY[0] = snakeY;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        snakeX--;
        break;
    case RIGHT:
        snakeX++;
        break;
    case UP:
        snakeY--;
        break;
    case DOWN:
        snakeY++;
        break;
    default:
        break;
    }

    if (snakeX >= width) snakeX = 0; else if (snakeX < 0) snakeX = width - 1;
    if (snakeY >= height) snakeY = 0; else if (snakeY < 0) snakeY = height - 1;

    // Check if the snake collides with itself (hits its own tail)
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == snakeX && tailY[i] == snakeY)
            gameOver = true;
    }

    // Check if the snake collides with the enemy (pirate)
    if (snakeX == enemyX && snakeY == enemyY)
        gameOver = true;

    // Calculate the distances from the enemy to the snake in both dimensions
    int distanceX = snakeX - enemyX;
    int distanceY = snakeY - enemyY;

    // Update the enemy's position based on the distance to the snake
    if (abs(distanceX) > abs(distanceY)) {
        // Move horizontally towards the snake
        if (distanceX > 0)
            enemyX++;
        else
            enemyX--;
    }
    else {
        // Move vertically towards the snake
        if (distanceY > 0)
            enemyY++;
        else
            enemyY--;
    }

    // Ensure the enemy stays within the bounds of the game board
    if (enemyX >= width) enemyX = 0; else if (enemyX < 0) enemyX = width - 1;
    if (enemyY >= height) enemyY = 0; else if (enemyY < 0) enemyY = height - 1;

    if (snakeX == fruitX && snakeY == fruitY) {
        score += 10;
        nTail++;
        fruitX = rand() % width;
        fruitY = rand() % height;
    }
}


// Function to load the high score from the file
int LoadHighScore() {
    int highScore = 0;
    ifstream file("highscore.txt");
    if (file) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

// Function to save the high score to the file
void SaveHighScore(int highScore) {
    ofstream file("highscore.txt");
    if (file) {
        file << highScore;
        file.close();
    }
}

int main() {
    int highScore = LoadHighScore();
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }

    // Check if the current score is higher than the high score
    if (score > highScore) {
        highScore = score;
        SaveHighScore(highScore);
    }

    cout << "Game Over! Your Score: " << score << endl;
    cout << "High Score: " << highScore << endl;

    // Add this line to wait for user input before closing the console window
    system("pause");

    return 0;
}