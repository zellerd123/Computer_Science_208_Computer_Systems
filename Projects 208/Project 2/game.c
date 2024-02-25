/*****
 * Project 02: Grid game
 * COSC 208, Introduction to Computer Systems, Fall 2023
 *****/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>


typedef struct 
{
    int value;
    int color; 
}tile;
//Global variables for keeping track of score and highest tile hit
static int score = 0;  
static int maxTile = 0; 

const char* tileColors[] = { //global variable for the background colors of the tiles
    "\033[40m",
    "\033[48;5;230m",  // 2
    "\033[48;5;224m",  // 4
    "\033[48;5;208m",  // 8
    "\033[48;5;202m",  // 16
    "\033[48;5;196m",  // 32
    "\033[48;5;160m",  // 64
    "\033[48;5;226m",  // 128
    "\033[48;5;220m",  // 256
    "\033[48;5;154m",  // 512
    "\033[48;5;34m",   // 1024
    "\033[48;5;21m"    // 2048
};

void addScore(int amount) {
    score += amount;
}

void resetScore(){
    score = 0; 
}

int getScore() {
    return score;
}

void resetMaxTile(){
    maxTile = 0;
}

int getMaxTile(){
    return maxTile;
}

void assignMaxTile(int amount){
    maxTile = amount; 
}

tile ** generateBoard()
{
int size = 4;

    if (size < 3){
        printf("ERROR: BOARD SIZE MUST BE BIGGER THAN 2");
    }

    tile ** tiles = (tile **)malloc(sizeof(tile *) * size);
     if (!tiles) {
        return NULL; //mem failure
    }
    for (int i = 0; i < size; i++){
        tiles[i] = (tile *)malloc(sizeof(tile) * size);
        if (!tiles[i]) {
            for (int j = 0; j < i; j++) {
                free(tiles[j]);
            }
            free(tiles);
            return NULL;
        }
        for (int j = 0; j < size; j++){
            tiles[i][j].value = 0;
            tiles[i][j].color
     = 0; 
        }
    }

    return tiles;
}

int boardFull(int size, tile ** board)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(board[i][j].value == 0)
            {
                return 0;
            }
        }
    }
    return 1;
}


tile ** getRandom(tile ** board)
{
   // printf("Rand Function called");
    //int rows[4] = {0,1,2,3};
    //int cols[4] = {0,1,2,3};
    //int size = sizeof(rows) / sizeof(rows[0]);
    int size = 4;
    int randomRow = rand() % size;
    //int randPosRow = rows[randomIndex];
    int randomCol = rand() % size;
    //int randPosCol = cols[randomIndex];

    if(board[randomRow][randomCol].value == 0)
    {
        //printf("This is the second test /n");
        int randomNumber = (rand() % 4) + 1;
        if (randomNumber != 4){
            board[randomRow][randomCol].value = 2;
            board[randomRow][randomCol].color
    ++;
        }
        else{
             board[randomRow][randomCol].value = 4;
             board[randomRow][randomCol].color
         = 2;
        }

        return board;
    }

    else
    {
        return getRandom(board);
    }
}

void shiftLeft(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        int last= 0;
        for(int j = 0; j < size; j++)
        {
            if(board[i][j].value != 0)
            {
                board[i][last].value = board[i][j].value;
                board[i][last].color
         = board[i][j].color
        ;
                if(last != j)
                {
                    board[i][j].value = 0;
                     board[i][j].color
                 = 0;
                }
                last++;
            }
        }
    }
}

void combineTilesLeft(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size -1; j++)
        {
            if(board[i][j].value != 0 && board[i][j].value == board[i][j +1].value)
            {
                board[i][j].value *= 2;
                board[i][j].color
        ++;
                addScore(board[i][j].value);
                if (board[i][j].value >= getMaxTile()){
                    assignMaxTile(board[i][j].value);
                }
                board[i][j+1].value = 0;
                board[i][j+1].color
         = 0;
            }
        }
    }
}

void combineTilesUp(int size, tile **board) 
{
    for (int j = 0; j < size; j++) 
    {
        for (int i = 0; i < size - 1; i++) 
        {
            if (board[i][j].value != 0 && board[i][j].value == board[i + 1][j].value) 
            {
                board[i][j].value *= 2;
                board[i][j].color
        ++;
                addScore(board[i][j].value);
                if (board[i][j].value >= getMaxTile()){
                    assignMaxTile(board[i][j].value);
                }
                board[i + 1][j].value = 0;
                board[i + 1][j].color
         = 0;
            }
        }
    }
}

void combineTilesRight(int size, tile **board) 
{
    for (int i = 0; i < size; i++) 
    {
        for (int j = size - 1; j > 0; j--) { 
            if (board[i][j].value != 0 && board[i][j].value == board[i][j - 1].value) 
            {
                board[i][j].value *= 2; 
                board[i][j].color
        ++;
                addScore(board[i][j].value);
                if (board[i][j].value >= getMaxTile()){
                    assignMaxTile(board[i][j].value);
                }
                board[i][j - 1].value = 0; 
                board[i][j - 1].color
         = 0;
            }
        }
    }
}

void combineTilesDown(int size, tile **board) 
{
    for (int j = 0; j < size; j++) 
    {
        for (int i = size - 1; i > 0; i--) 
        { 
            if (board[i][j].value != 0 && board[i][j].value == board[i - 1][j].value) 
            {
                board[i][j].value *= 2; 
                board[i][j].color
        ++;
                //printf("this is current board value = %d", board[i][j].value);
                addScore(board[i][j].value);
                if (board[i][j].value >= getMaxTile()){
                    assignMaxTile(board[i][j].value);
                }
                board[i - 1][j].value = 0; 
                board[i - 1][j].color
         = 0;
            }
        }
    }
}

int canMoveLeft(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 1; j < size; j++)
        {
            if((board[i][j].value != 0 && board[i][j-1].value ==  0) || (board[i][j].value != 0 && board[i][j-1].value == board[i][j].value))
            {
                return 1;
            }
        }
    }
    return 0;
}

int canMoveRight(int size, tile **board){ 
    for(int i = 0; i < size; i++){ 
        for (int j = 0; j < size - 1; j++){
            
            if((board[i][j].value != 0 && board[i][j+1].value == 0) ||(board[i][j].value != 0 && (board[i][j].value == board[i][j+1].value))){
                return 1; 
            }

        }
    }
    return 0;
}

int canMoveDown(int size, tile **board){
   for(int i = 0; i < size; i++){ 
        for (int j = 0; j < size - 1; j++){
            if((board[j][i].value != 0 && board[j+1][i].value == 0) || (board[j][i].value != 0 && (board[j][i].value == board[j+1][i].value))){
                return 1; 
            }

        }
   }
   return 0;
}

int canMoveUp(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 1; j < size; j++)
        {
            if((board[j][i].value != 0 && board[j-1][i].value ==  0) ||(board[j][i].value != 0 && board[j-1][i].value == board[j][i].value))
            {
                return 1;
            }
        }
    }
    return 0;
}




int canMakeMove(int size, tile **board)
{
    int check = boardFull(size, board);
    if(check == 0)
    {
        return 1;
    }

    for(int i = 0; i < size; i++)
    {
        for(int j =0; j < size-1; j++)
        {
            if(board[i][j].value == board[i][j+1].value|| board[j][i].value == board[j+1][i].value)
            {
                return 1;
            }
        }
    }

    return 0;
}

int getDirection()
{
    char move;
    printf("Enter your move (w - up, a - left, s - down, d - right, q - quit):");
    scanf(" %c", &move);

    switch(move)
    {
        case 'w':
            return 1;
        case 'a':
            return 2;
        case 's':
            return 3;
        case 'd':
            return 4;
        case 'q':
            return -1;
        default:
            return 0;
    }
}

int hasWon(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(board[i][j].value == 2048)
            {
                return 1;
            }
        }
    }
    return 0;
}


void printIt(tile **board)
{
    int currScore = getScore();
     printf("\n Current score is: %d \n", currScore);
    for (int i = 0; i < 4; i++) 
    {
        for (int j = 0; j < 4; j++) 
        {
            int spaces = 5;
            int currval = board[i][j].value;
            //if (currval == 0) spaces--;
            while (currval >= 10){
                spaces--;
                currval /= 10;
            }
            printf("%s", tileColors[board[i][j].color
    ]);
            for (int i = 0; i < spaces; i++) {
                  printf(" ");
             }
            printf("%d \033[0m", board[i][j].value);
        }
        printf("\n"); // Newline for a new row
    }
}

void shiftRight(int size, tile **board)
{
    for(int i = 0; i < size; i++)
    {
        int last = size -1;
        for(int j = size -1; j >= 0; j--)
        {
            if(board[i][j].value != 0)
            {
                board[i][last].value = board[i][j].value;
                board[i][last].color
         = board[i][j].color
        ;
                if(last != j)
                {
                    board[i][j].value = 0;
                    board[i][j].color
             = 0;
                }
                last--;
            }
        }
    }
}

void shiftUp(int size, tile **board)
{
    for(int j = 0; j < size; j++)
    {
        int last = 0;
        for(int i = 0; i < size; i++)
        {
            if(board[i][j].value != 0)
            {
                board[last][j].value = board[i][j].value;
                board[last][j].color
         = board[i][j].color
        ;
                if(last != i)
                {
                    board[i][j].value = 0;
                    board[i][j].color
             = 0;
                }
                last++;
            }
        }
    }
}

void shiftDown(int size, tile **board)
{
    for(int j = 0; j < size; j++)
    {
        int last = size -1;
        for(int i = size -1; i >= 0; i--)
        {
            if(board[i][j].value != 0)
            {
                board[last][j].value = board[i][j].value;
                board[last][j].color
         = board[i][j].color
        ;
                if(last != i)
                {
                    board[i][j].value = 0;
                    board[i][j].color
             = 0;
                }
                last--;
            }
        }
    }
}

int robotTestQuestion(){
    char test;
    printf("Would you like to play yourself or do the robot test? (play game - g, robot - r, any other key - play game):");
    scanf(" %c", &test);
    switch(test)
    {
        case 'r':
            return 1;
        case 'g':
            return 0;
        default:
            return 0;
    }

    return test;
    }

void cleanGame(tile **gameBoard, int size){
    for (int i = 0; i < size; ++i) 
    {
        free(gameBoard[i]);
    }
    free(gameBoard);
}

tile ** instantiateBoard(){
    tile **gameBoard = generateBoard();

    // Start with two random tiles
    gameBoard = getRandom(gameBoard);
    gameBoard = getRandom(gameBoard);
    return gameBoard; 
}

bool singleMove(tile **gameBoard, int size, int direction, bool isPlayer){
    bool moved = false; 

        switch(direction) 
        {
            case 1: // Up
                if (isPlayer) system("clear");
                if(canMoveUp(size, gameBoard)){
                    shiftUp(size, gameBoard);
                    combineTilesUp(size, gameBoard);
                    shiftUp(size, gameBoard);
                    moved = true;

                }
                else{
                    if (isPlayer) printf("You can't move up! Choose another direction.\n");
                }  


                break;
            case 2: // Left
                if (isPlayer) system("clear");
                if (canMoveLeft(size, gameBoard)){
                    shiftLeft(size, gameBoard);
                    combineTilesLeft(size, gameBoard);
                    shiftLeft(size, gameBoard);
                    moved = true;
                }
                else{
                    if (isPlayer) printf("You can't move left! Choose another direction.\n");
                }
                break;
            case 3: // Down
                if (isPlayer) system("clear");
                if (canMoveDown(size, gameBoard)){
                    shiftDown(size, gameBoard);
                    combineTilesDown(size, gameBoard);
                    shiftDown(size, gameBoard);
                    moved = true;
                }
                else{
                    if (isPlayer) printf("You can't move down! Choose another direction.\n");
                }
                
                break;
            case 4: // Right
                if (isPlayer) system("clear");
                 if (canMoveRight(size, gameBoard)){
                    shiftRight(size, gameBoard);
                    combineTilesRight(size, gameBoard);
                    shiftRight(size, gameBoard);
                    moved = true;
                }
                else{
                    if (isPlayer) printf("You can't move right! Choose another direction.\n");
                }
                break;
            default: // Invalid input
                if (isPlayer) system("clear");
                if (isPlayer) printf("Invalid move. Please enter 'w', 'a', 's', 'd', or 'q'.\n");
                
        }
    return moved; 

}

void playerGameState(){

    int size = 4; // It's better to use a variable than hard-coded values, for easier adjustments later.
    tile **gameBoard = instantiateBoard();
    bool isPlayer = true;

    bool gameContinue = true, moved;
    int direction;

    while (gameContinue) 
    {
        //system("clear"); // This clears the console screen (system("cls") for Windows)
        printIt(gameBoard);

        if (hasWon(size, gameBoard)) {
            printf("You've made a 2048 tile! Congratulations!\n");
            break; // Exit the loop
        }

        if (!canMakeMove(size, gameBoard)) {
            printf("Game over: no moves left!\n");
            break; // Exit the loop
        }

        direction = getDirection();
        if (direction == -1){ // Quit
                gameContinue = false;
                printf("You chose to quit the game.\n");
                continue;
        } // Skip adding a new tile and just exit
        moved = singleMove(gameBoard, size, direction, isPlayer);

        if (moved) 
        {
            // Only add a new tile if the board changed
            gameBoard = getRandom(gameBoard);
        }
    }
    // Cleanup
    cleanGame(gameBoard, size);
}

int robotLogic(tile **gameBoard, int size, int wentUpLast){
    int direction = 0;
    bool isPlayer = false;
    if (canMoveUp(size, gameBoard) && !wentUpLast){
        direction = 1; 
        singleMove(gameBoard, size, direction, isPlayer);
        return direction;
    }
    else if (canMoveRight(size, gameBoard)){
        direction = 4;
        singleMove(gameBoard, size, direction, isPlayer);
       /* if (canMoveRight(size, gameBoard)){
            return 1; 
        }*/
        return direction;
    }
    else{
        for (int i = 0; i < size; i++){
            if (gameBoard[i][3].value == 0){
                for (int j = 0; j < size; j++){
                    if(gameBoard[0][j].value == 0){
                        if (i >= j){
                            direction = 3;
                            singleMove(gameBoard, size, direction, isPlayer);
                            return direction;
                        }
                        else{
                            direction = 2; 
                            singleMove(gameBoard, size, direction, isPlayer);
                            return 1; //so that thinks direction last time was up to call right shift
                        }
                    }
                    direction = 2; 
                    singleMove(gameBoard, size, direction, isPlayer);
                    return 1; //so that thinks direction last time was up to call right shift
                }
            }    
        }
        direction = 3; 
        singleMove(gameBoard, size, direction, isPlayer);
        for (int i = 0; i < size-1; i++){ 
            if (gameBoard[i][2].value == gameBoard[i][3].value){
                 return 1;
            }
        }
        return 3;
    }

    return direction;
}


int simulateRobotGame(){
    //printf("You chose to simulate the robot game");
    tile **gameBoard = instantiateBoard();
    int size = 4; 
    bool gameContinue = true, moved = false, wentUpLast = false;
    while (gameContinue) 
    {
        if (hasWon(size, gameBoard)) {
            break; // Exit the loop
        }

        if (!canMakeMove(size, gameBoard)) {
            break; // Exit the loop
        }


        moved = robotLogic(gameBoard, size, wentUpLast);
        if (moved == 1){
            wentUpLast = true; 
        }
        else{
            wentUpLast = false; 
        }
        if (moved) 
        {
            // Only add a new tile if the board changed
            gameBoard = getRandom(gameBoard);
        }
    }
    int score = getScore();
    cleanGame(gameBoard, size);
    return score; 
}

void displayProgressBar(int current, int total){
    int barLength = 50; // Length of the progress bar in console
    int position = (int)((double)current / total * barLength);

    putchar('[');
    for (int i = 0; i < barLength; i++) {
        if (i < position) {
            putchar('=');
        } else if (i == position) {
            putchar('>');
        } else {
            putchar(' ');
        }
    }
    putchar(']');
    printf(" %d%% (%d/%d)\r", current * 100 / total, current, total);
    fflush(stdout); 
}

void testRobot(int numSimulations){
    int successCount = 0;
    int totalScore = 0;
    int highestTile = 0; 
    
    for (int i = 0; i < numSimulations; i++) {
        int score = simulateRobotGame();
        int currentMaxTile = getMaxTile(); 
        totalScore += score;
        resetScore();
        if (currentMaxTile > highestTile) highestTile = currentMaxTile;
        if (currentMaxTile >= 2048) { 
            successCount++;
        }
        resetMaxTile();
        displayProgressBar(i + 1, numSimulations);
    }

    printf("\nRobot Success Rate: %d/%d\n", successCount, numSimulations);
    printf("Average Score: %d\n", totalScore / numSimulations);
    printf("Highest Tile Reached %d\n", highestTile);

}

int main() 
{
     //printf("\033[41mR\033[42mG\033[44mB\033[0m\n");
    // Initialization
    srand((unsigned) time(NULL));
    printf("2048!\n\n");
    if (!robotTestQuestion()){
        playerGameState();
    }
    else{
        testRobot(1000);
    }
   


}
