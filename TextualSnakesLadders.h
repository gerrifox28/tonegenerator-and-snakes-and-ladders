//
// Created by Gerri Fox on 10/5/20.
//

#ifndef MIDTERM_TEXTUALSNAKESLADDERS_H
#define MIDTERM_TEXTUALSNAKESLADDERS_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

//array for key spaces
const int keySpaces[4] = {10, 17, 34, 39};

//array for key of where to go after landing on a key space
const int destinationSpaces[4] = {30, 35, 5, 21};

const string boardval[50] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
                             "11", "12", "13", "14", "15", "16", "17", "18", "19", "20",
                             "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
                             "31", "32", "33", "34", "35", "36", "37", "38", "39", "40",
                             "41", "42", "43", "44", "45", "46", "47", "48", "49", "50"};


/**
 * Represents a game of Snakes and Ladders in a textual interface.
 */
class TextualSnakesLadders {
public: //public variables and methods




    int dice; //variable to represent number rolled on dice

    string board[50]; //initialize board



//variables for moving players
    string tempPlayer1Space;
    string tempPlayer2Space;

//initialize player 1 and 2 locations
    int player1;
    int player2;

//math equation variables
    int num1;
    int num2;
    char sign;

//math equation answer choices
    int choice1;
    int choice2;

    int correctChoice; //keeps track of which choice is the correct choice

    /**
     * Constructor that sets up the board by initializing each piece to its respective number value
     * based on index as well as initializing the ladders and snakes.
     */
    TextualSnakesLadders();

    // ~TextualSnakesLadders();


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

    /**
     * Gets the answer of the equation using the randomly selected numbers and sign.
     */
    void getEquationAnswer();

    /**
     * Prints the equation to the console.
     */
    void printEquation();

    /**
     * Returns the right answer to the current equation.
     * @return the right answer to the current equation.
     */
    int getCorrectAns();

    void setUpBoard();

};


#endif //MIDTERM_TEXTUALSNAKESLADDERS_H
