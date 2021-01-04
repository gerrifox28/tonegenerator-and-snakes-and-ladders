//
// Created by Gerri Fox on 10/25/20.
//

#include "midterm.h"

/** Write a 4-byte value at the specified general-purpose I/O location.  *
 * @param pBase
 * Base address returned by 'mmap'.
 * @parem offset  Offset where device is mapped.
 * @param value   Value to be written.
 * */
void midterm::RegisterWrite(char *pBase, unsigned int reg_offset, int value) {
    *(volatile unsigned int *) (pBase + reg_offset) = value;
}

/**  * Read a 4-byte value from the specified general-purpose I/O location.
 * *  * @param pBase         Base address returned by 'mmap'.
 * * @param offset  Offset where device is mapped.
 * * @return        Value read.  */
int midterm::RegisterRead(char *pBase, unsigned int reg_offset) {
    return *(volatile unsigned int *) (pBase + reg_offset);
}

/**  * Initialize general-purpose I/O
 * *  - Opens access to physical memory /dev/mem
 * *  - Maps memory into virtual address space
 *  * @param fd      File descriptor passed by reference, where the result
 *  *                of function 'open' will be stored.
 *  * @return  Address to virtual memory which is mapped to physical, or MAP_FAILED onerror.
 *  */

char *midterm::Initialize(int *fd) {
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
void midterm::Finalize(char *pBase, int fd) {
    if (munmap(pBase, LW_BRIDGE_SPAN) != 0) {
        cout << "ERROR: munmap() failed..." << endl;
        exit(1);
    }
    close(fd);    // close memory}
}

void midterm::playTone(string note) {
    // Initialize
    int fd;
    char *pBase = Initialize(&fd);
    int freq;
    string notes[15] = {"c4", "d4", "e4", "f4", "g4", "a4", "b4", "c5", "d5", "e5", "f5", "g5", "a5", "b5", "c6"};



//    int counter = (200000000 / tones[freq]) / 2; //convert 200 MHz to Hz
//    RegisterWrite(pBase, MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
//    RegisterWrite(pBase, MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
//    int entervalue = 0x000000;
    int loop = 0;
    //cin >> note;
    while (true) {
        for (int i = 0; i < 15; i++) {
            if (note == notes[i]) {
                freq = i;
            }
        }
        int counter = (200000000 / tones[freq]) / 2; //convert 200 MHz to Hz
        RegisterWrite(pBase, DIRECTION_BIT, 0xffffffff);
        RegisterWrite(pBase, MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
        RegisterWrite(pBase, MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
        int entervalue = 0x000000;
        while (loop < 5000) {
            if (RegisterRead(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0) {
                RegisterWrite(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flagbit
                entervalue = entervalue ^ 0x1;
                RegisterWrite(pBase, JP1, entervalue);
                loop++;
            }
        }

        string name;
        getline(cin, name);
        note = name;
        loop = 0;
        if (note == "0") {
            break;
        }

    }


    cout << "Terminating sound...!" << endl;
    Finalize(pBase, fd);
}

void midterm::playOneTone() {
    int fd;
    char *pBase = Initialize(&fd);
    int loop = 0;
    int outerloop = 0;
    while (outerloop < 3) {
        int freq;
        switch (outerloop) {
            case 0:
                freq = 659;
                break;
            case 1:
                freq = 783;
                break;
            case 2:
                freq = 1046;
                break;
            default:
                cout << "Inavlid" << endl;
        }

        int counter = (200000000 / freq) / 2; //convert 200 MHz to Hz
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
        outerloop++;
    }

    Finalize(pBase, fd);
}


void midterm::playGame() {
    TextualSnakesLadders *tsl = new TextualSnakesLadders; //initialize new text game object
    //DE1SoCSnakesLadders *dsl = new DE1SoCSnakesLadders; //initialize new board game object

    bool won = false;
    tsl->setUpBoard();

    //loop that keeps the game going until one player reaches the end (space 50 exactly)
    while (!won) {
        tsl->printBoard(); //print the board to the console

        cout << "It is now your turn!" << endl;

        bool loop = true;
        while (loop) {
            cout << "Please press \"Y\" to roll the dice." << endl;
            char y;
            cin >> y;
            if (y == 'Y' || y == 'y') {
                tsl->rollDice();
                loop = false;
            }
        }

        tsl->movePlayer1(); //move player 1 and set up equation
        if (player1 == 49) { //player 1 (user) wins
            won = true;
        }

        tsl->printBoard();
        tsl->getEquation(); //set up equation
        tsl->printEquation(); //print equation

        bool loop2 = true;
        while (loop2) {
            cout << "Please type your answer." << endl;
            int ans;
            cin >> ans;
            if (ans == tsl->getCorrectAns()) {
                //dsl->playHappySound();
                cout << "That is correct! Player 1 gets another turn!" << endl;
                //playOneTone();
                bool loop3 = true;
                while (loop3) {
                    cout << "Please press \"Y\" to roll the dice." << endl;
                    char y;
                    cin >> y;
                    if (y == 'Y' || y == 'y') {
                        tsl->rollDice();
                        loop3 = false;
                    }
                }
                tsl->movePlayer1();//player 1 gets another turn
                tsl->printBoard();  //print the board
                loop2 = false;
            } else {
                cout << "Sorry that answer is incorrect. The correct answer is " << correctChoice << endl;
                loop2 = false;
            }
        }

        if (player1 == 49) { //player 1 (user) wins
            won = true;
        }

        cout << "It is now the computer's turn." << endl;
        tsl->rollDice();
        tsl->movePlayer2(); //move player 2

        if (player2 == 49) { //player 2 (computer) wins
            won = true;
        }

    }

    //out of loop so one player must have won
    if (player1 == 49) { //player 1 (user) wins
        cout << "You win!" <<
             endl;
        //playOneTone();
    } else { //player 2 (computer) wins
        cout << "The computer won." <<
             endl;
    }
}
