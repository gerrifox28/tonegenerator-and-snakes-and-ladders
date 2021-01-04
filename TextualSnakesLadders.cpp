//
// Created by Gerri Fox on 10/5/20.
//

#include "TextualSnakesLadders.h"


TextualSnakesLadders::TextualSnakesLadders() {}


void TextualSnakesLadders::setUpBoard() {

    //initialize player 1 and 2 starting positions on board
    tempPlayer1Space = "1";
    tempPlayer2Space = "1";

//initialize player 1 and 2 locations
    player1 = 0;
    player2 = 0;


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


void TextualSnakesLadders::printBoard() {
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

void TextualSnakesLadders::rollDice() {

    srand(time(NULL)); // initialize random seed

    dice = rand() % 6 + 1; //get a new random dice roll between 1 and 6

    cout << "The dice rolled a " << dice << "!" << endl;
}

void TextualSnakesLadders::movePlayer1() {

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


}

void TextualSnakesLadders::movePlayer2() {

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


}

void TextualSnakesLadders::getEquation() {

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

void TextualSnakesLadders::getEquationChoices() {
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

void TextualSnakesLadders::getEquationAnswer() {
    int ans;
    if (sign == '+') { //if equation uses addition
        ans = num1 + num2;
    } else { //if equation uses subtraction
        ans = num1 - num2;
    }
    correctChoice = ans;
}

void TextualSnakesLadders::printEquation() {
    cout << "Please solve the following equation to get an extra turn!" << endl;
    if (sign == '+') {
        cout << num1 << "+" << num2 << "= ?" << endl;
    } else {
        cout << num1 << "-" << num2 << "= ?" << endl;
    }
    cout << "Choose your answer:" << choice1 << "or" << choice2 << endl;
}

int TextualSnakesLadders::getCorrectAns() {
    return correctChoice;
}



