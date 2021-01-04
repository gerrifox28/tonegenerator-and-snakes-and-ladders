//
// Created by Gerri Fox on 10/17/20.
//
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "TextualSnakesLadders.h"
//#include "DE1SoCSnakesLadders.h"

using namespace std;

// Physical base address of FPGA Devices
const unsigned int LW_BRIDGE_BASE = 0xFF200000; // Base offset
// Length of memory-mapped IO window
const unsigned int LW_BRIDGE_SPAN = 0x00DEC700;  // Address map size/
// / Cyclone V FPGA device addresses
const unsigned int LEDR_BASE = 0x00000000;  // Leds offset
const unsigned int SW_BASE = 0x00000040;  // Switches offset
const unsigned int KEY_BASE = 0x00000050;  // Push buttons offset

//0xFFFEC600 -0xFF200000 = 0xDEC600
const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0xDEC600;
//  Points  to  LOAD Registerconst
const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0xDEC604;
//  Points  to COUNTER Registerconst
const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0xDEC608;
//  Points  to CONTROL Registerconst
const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0xDEC60C;
//  Points  to INTERRUPT Register

const unsigned int JP1 = 0x00000060; //speaker offset

const unsigned int DIRECTION_BIT = 0x00000064; //direction bit offset

const unsigned int tones[15] = {261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 698, 783, 880, 987, 1046};

//array for key spaces
int keySpaces[4] = {10, 17, 34, 39};

//array for key of where to go after landing on a key space
int destinationSpaces[4] = {30, 35, 5, 21};


int dice; //variable to represent number rolled on dice

string board[50]; //initialize board

string boardval[50] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                       "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                       "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
                       "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
                       "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"};


//variables for moving players
string tempPlayer1Space = "1";
string tempPlayer2Space = "1";

//initialize player 1 and 2 locations
int player1 = 0;
int player2 = 0;

//math equation variables
int num1;
int num2;
char sign;

//math equation answer choices
int choice1;
int choice2;

int correctChoice; //keeps track of which choice is the correct choice

bool won = false;

//prototypes

/**
 * Sets up the board,
 */
void setUpBoard();

/**
     * Rolls the dice by getting a random value from 0 to 6.
     */
void rollDice();

/**
 * Prints the board to the console in it's proper grid showing numbers of each space
 * or a ladder or snake. Prints each space character with two spaces in between and
 * a newline after every 10 spaces. Board is printed so the numbers increase and a
 * player can weave through the board following the number order without
 * jumping.
 */
void printBoard();

/**
 * Moves player 1 based on the random dice roll. Resets the value of player 1
 * as well as updates the board to draw the position of player 1. Checks to see
 * if the player has landed on a key space (bottom of ladder or top of snake)
 * and moves player 1 accordingly if necessary. Prints the board if this happens to
 * show user where they first moved to.
 */
void movePlayer1();

/**
 * Moves player 2 based on the random dice roll. Resets the value of player 2
 * as well as updates the board to draw the position of player 2. Checks to see
 * if the player has landed on a key space (bottom of ladder or top of snake)
 * and moves player 2 accordingly if necessary. Prints the board if this happens to
 * show user where they first moved to.
 */
void movePlayer2();

/**
 * Gets two random numbers between 0 and 9 as well as uses a random value to set
 * the sign of the equation (either 0 for + or 1 for *). Makes sure that the equation
 * answer is one digit long, updates num2 until it is if it originally isn't.
 * Calls getEquationChoices() to set up the two multiple choice answers
 * the user can pick from.
 */
void getEquation();

/**
 * Gets two choices for the user to pick from to guess the equation answer.
 * One choice is the correct answer as determined by calling getEquationAnswer.
 * The choice is chosen by a random number, if it is 0, then it is choice 1, if it is 1,
 * then it is choice 2. Then, to get the next choice,
 * it either increases or decreases by 1 as appropriate, ensuring
 * that both options are only one digit long.
 */
void getEquationChoices();

void printEquation();

void getEquationAnswer();

int getCorrectAns();

void playOneTone();

/** Write a 4-byte value at the specified general-purpose I/O location.  *
 * @param pBase
 * Base address returned by 'mmap'.
 * @parem offset  Offset where device is mapped.
 * @param value   Value to be written.
 * */
void RegisterWrite(char *pBase, unsigned int reg_offset, int value) {
    *(volatile unsigned int *) (pBase + reg_offset) = value;
}

/**  * Read a 4-byte value from the specified general-purpose I/O location.
 * *  * @param pBase         Base address returned by 'mmap'.
 * * @param offset  Offset where device is mapped.
 * * @return        Value read.  */
int RegisterRead(char *pBase, unsigned int reg_offset) {
    return *(volatile unsigned int *) (pBase + reg_offset);
}

/**  * Initialize general-purpose I/O
 * *  - Opens access to physical memory /dev/mem
 * *  - Maps memory into virtual address space
 *  * @param fd      File descriptor passed by reference, where the result
 *  *                of function 'open' will be stored.
 *  * @return  Address to virtual memory which is mapped to physical, or MAP_FAILED onerror.
 *  */

char *Initialize(int *fd) {
    // Open /dev/mem to give access to physical addresses
    *fd = open("/dev/mem", (O_RDWR | O_SYNC));
    if (*fd == -1)   //  check for errors in openning /dev/mem
    {
        cout << "ERROR: could not open /dev/mem..." << endl;
        exit(1);
    }   // Get a mapping from physical addresses to virtual addresses
    char *virtual_base = (char *) mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, *fd, LW_BRIDGE_BASE);
    if (virtual_base ==
        MAP_FAILED)       // check for errors
    {
        cout << "ERROR: mmap() failed..." << endl;
        close(*fd);            // close memory before exiting
        exit(1);        // Returns 1 to the operating system;
    }
    return virtual_base;
}

/**  * Close general-purpose I/O.
 * *  * @param pBase   Virtual address where I/O was mapped.
 * * @param fd      File descriptor previously returned by 'open'.  */
void Finalize(char *pBase, int fd) {
    if (munmap(pBase, LW_BRIDGE_SPAN) != 0) {
        cout << "ERROR: munmap() failed..." << endl;
        exit(1);
    }
    close(fd);    // close memory}
}



/**
 * Plays a game of Snakes and ladders with the user representing player 1 (represented using a '+' and the
 * computer representing player 2, represented using a '-'. The user has the option for a power up if
 * they answer a random math equation right, they can get extra turn. The computer plays as normal always.
 * @return 0 when the game has ended
 */
int main() {

    // TextualSnakesLadders *tsl = new TextualSnakesLadders; //initialize new text game object
    //DE1SoCSnakesLadders *dsl = new DE1SoCSnakesLadders; //initialize new board game object

    setUpBoard();

    //loop that keeps the game going until one player reaches the end (space 50 exactly)
    while (!won) {
        printBoard(); //print the board to the console

        cout << "It is now your turn!" << endl;

        bool loop = true;
        while (loop) {
            cout << "Please press \"Y\" to roll the dice." << endl;
            char y;
            cin >> y;
            if (y == 'Y' || y == 'y') {
                rollDice();
                loop = false;
            }
        }

        movePlayer1(); //move player 1 and set up equation
        if (player1 == 49) { //player 1 (user) wins
            won = true;
            break;
        }

        printBoard();
        getEquation(); //set up equation
        printEquation(); //print equation

        bool loop2 = true;
        while (loop2) {
            cout << "Please type your answer." << endl;
            int ans;
            cin >> ans;
            if (ans == getCorrectAns()) {
                //dsl->playHappySound();
                cout << "That is correct! Player 1 gets another turn!" << endl;
                playOneTone();
                bool loop3 = true;
                while (loop3) {
                    cout << "Please press \"Y\" to roll the dice." << endl;
                    char y;
                    cin >> y;
                    if (y == 'Y' || y == 'y') {
                        rollDice();
                        loop3 = false;
                    }
                }
                movePlayer1();//player 1 gets another turn
                printBoard();  //print the board
                loop2 = false;
            } else {
                cout << "Sorry that answer is incorrect. The correct answer is " << correctChoice << endl;
                loop2 = false;
            }
        }

        if (player1 == 49) { //player 1 (user) wins
            won = true;
            break;
        }

        cout << "It is now the computer's turn." << endl;
        rollDice();
        movePlayer2(); //move player 2

        if (player2 == 49) { //player 2 (computer) wins
            won = true;
            break;
        }

    }

    //out of loop so one player must have won
    if (player1 == 49) { //player 1 (user) wins
        cout << "You win!" <<
             endl;
        playOneTone();
    } else { //player 2 (computer) wins
        cout << "The computer won." <<
             endl;
    }

    return 0; //exit the program

}



void setUpBoard() {


    //set piece values
    for (int i = 0; i < 50; i++) {
        board[i] = boardval[i];
    }

    //assign ladders and snakes
    //ladder straight up and down on right side of board
    board[10] = "|-|";
    board[29] = "|-|";
    board[30] = "|-|";

    //diagonal ladder
    board[17] = "/-/";
    board[23] = "/-/";
    board[35] = "/-/";

    //long curvy snake in middle
    board[5] = "(";
    board[14] = ")";
    board[25] = "(";
    board[34] = ")";

    //straight snake
    board[39] = "(";
    board[21] = ")";
}


void printBoard() {
    //print array with space numbers and snakes and ladders
    for (int i = 40; i < 50; i++) {
        cout << board[i] + "  ";
    }
    cout << "\n";

    for (int i = 39; i >= 30; i--) {
        cout << board[i] + "  ";
    }

    cout << "\n";

    for (int i = 20; i < 30; i++) {
        cout << board[i] + "  ";
    }
    cout << "\n";

    for (int i = 19; i >= 10; i--) { //print these backwards as to weave through board
        if (i == 11) {
            cout << board[i] + " ";
        } else {
            cout << board[i] + "  ";
        }

    }

    cout << "\n";


    for (int i = 0; i < 10; i++) {
        cout << board[i] + "   "; //print each element value with 2 spaces in between
    }
    cout << "\n";
}

void rollDice() {

    srand(time(NULL)); // initialize random seed

    dice = rand() % 6 + 1; //get a new random dice roll between 1 and 6

    cout << "The dice rolled a " << dice << "!" << endl;
}

void movePlayer1() {

    if (player1 + 1 + dice > 50) {
        cout << "You must roll land less than or exactly on space 50." << endl;
        return; //do nothing as cannot go past end
    } else {
        board[player1] = tempPlayer1Space; //reset board back to normal value
        player1 = player1 + dice; //move player based on dice roll
    }


    for (int k = 0; k < 4; k++) { //check is player has landed on the bottom of a ladder or top of a snake
        if (player1 == keySpaces[k]) {
            if (player1 == 10 || player1 == 17) {
                cout << "You landed on a ladder!" << endl;
            } else {
                cout << "You landed on a snake :(" << endl;
            }
            string temp = board[player1]; //get value at where player is about to move to
            if (temp == "C") {
                board[player1] = "PC"; //update board to show where the player has moved to
            } else {
                board[player1] = "P"; //update board to show where the player has moved to
            }
            printBoard(); //to show user updated space before either a snake or ladder
            board[player1] = temp; //reset after displaying as player is about to move again
            player1 = destinationSpaces[k]; //change player to end of either ladder or snake
            cout << "You are now on space " << player1 + 1 << endl;
        }
    }

//    if (board[player1] == "C") {//if player 1 moves to where player 2 is
//        tempPlayer1Space = boardval[player1]; //reset back to value for next time as player 2 will have moved
//    }

    if (board[player1] == "C") { //if the computer is currently on the space
        board[player1] = "PC";
        tempPlayer1Space = boardval[player1]; //get value at where player is about to move to

    } else {
        tempPlayer1Space = board[player1]; //get value at where player is about to move to
        board[player1] = "P"; //update board to show where the player has moved to
    }


    cout << "You are on space " << player1 + 1 << endl;

    if (player1 == 49) {
        won = true;
    }

}

void movePlayer2() {

    if (player2 + 1 + dice > 50) {
        cout << "The computer must roll land less than or exactly on space 50." << endl;
        return; //do nothing as cannot go past end
    } else {
        board[player2] = tempPlayer2Space; //reset board back to normal value
        player2 = player2 + dice; //move player based on dice roll
    }

    for (int k = 0; k < 4; k++) { //check is player has landed on the bottom of a ladder or top of a snake
        if (player2 == keySpaces[k]) {
            if (player2 == 10 || player2 == 17) {
                cout << "The computer landed on a ladder :(" << endl;
            } else {
                cout << "The computer landed on a snake!" << endl;
            }
            string temp = board[player2]; //get value at where player is about to move to
            if (temp == "P") {
                board[player2] = "PC"; //update board to show where the player has moved to
            } else {
                board[player2] = "C"; //update board to show where the player has moved to
            }
            printBoard(); //to show user updated space before either a snake or ladder
            board[player2] = temp; //reset after displaying as player is about to move again
            player2 = destinationSpaces[k]; //change player to end of either ladder or snake
        }
    }

    if (board[player2] == "P") { //if the computer is currently on the space
        board[player2] = "PC";
        tempPlayer2Space = boardval[player2]; //get value at where player is about to move to

    } else {
        tempPlayer2Space = board[player2]; //get value at where player is about to move to
        board[player2] = "C"; //update board to show where the player has moved to
    }

    cout << "The computer is on space " << player2 + 1 << endl;

    if (player2 == 49) {
        won = true;
    }

}

void getEquation() {

    srand(time(NULL)); // initialize random seed
    //get math question
    num1 = rand() % 10; //random num between 0 and 9
    //cout << num1 << endl;
    num2 = rand() % 10; //random num between 0 and 9
    //cout << num2 << endl;

    if (rand() % 2 == 0) { //determine what sign for the equation
        sign = '+';
        while (num1 + num2 >= 10) { //if the answer is more than 1 digit
            num2 = rand() % 10; //get new num random num between 0 and 9
        }

    } else {
        sign = '-';
        while (num1 - num2 < 0) { //if the answer is negative
            num1 = rand() % 10; //get new num random num between 0 and 9
        }
    }
    getEquationChoices(); //get answer choices
}

void getEquationChoices() {
    getEquationAnswer();
    if (rand() % 2 == 0) { //determines if choice 1 or 2 will be correct
        choice1 = correctChoice;
        // correctChoice = 3;
        choice2 = rand() % 10; //get a random answer for the other choice
        while (choice2 == choice1) {
            choice2 =
                    rand() % 10; //get another random answer for the other choice if the original equals the correct
        }
    } else {
        choice2 = correctChoice;
        //correctChoice = 2;
        choice1 = rand() % 10; //get a random answer for the other choice
        while (choice1 == choice2) {
            choice1 =
                    rand() % 10; //get another random answer for the other choice if the original equals the correct
        }
    }
}

void getEquationAnswer() {
    int ans;
    if (sign == '+') { //if equation uses addition
        ans = num1 + num2;
    } else { //if equation uses subtraction
        ans = num1 - num2;
    }
    correctChoice = ans;
}

void printEquation() {
    cout << "Please solve the following equation to get an extra turn!" << endl;
    if (sign == '+') {
        cout << num1 << "+" << num2 << "= ?" << endl;
    } else {
        cout << num1 << "-" << num2 << "= ?" << endl;
    }
    cout << "Choose your answer:" << choice1 << "or" << choice2 << endl;
}

int getCorrectAns() {
    return correctChoice;
}





void playOneTone() {
    int fd;
    char *pBase = Initialize(&fd);
    int loop = 0;
//    int outerloop = 0;
//    while (outerloop < 3) {
//        int freq;
//        switch (outerloop) {
//            case 0:
//                freq = 659;
//                break;
//            case 1:
//                freq = 783;
//                break;
//            case 2:
//                freq = 1046;
//                break;
//            default:
//                cout << "Inavlid" << endl;
//        }

    int counter = (200000000 / 440) / 2; //convert 200 MHz to Hz
    RegisterWrite(pBase, DIRECTION_BIT, 0xffffffff);
    RegisterWrite(pBase, MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
    RegisterWrite(pBase, MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
    int entervalue = 0x000000;
    while (loop < 1000) {
        if (RegisterRead(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0) {
            RegisterWrite(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flagbit
            entervalue = entervalue ^ 0x1;
            RegisterWrite(pBase, JP1, entervalue);
            loop++;
        }
    }
    // outerloop++;
    //}

    Finalize(pBase, fd);
}




