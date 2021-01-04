//
// Created by Gerri Fox on 10/23/20.
//


#include "midterm.h"


using namespace std;
//
//// Physical base address of FPGA Devices
//const unsigned int LW_BRIDGE_BASE = 0xFF200000; // Base offset
//// Length of memory-mapped IO window
//const unsigned int LW_BRIDGE_SPAN = 0x00DEC700;  // Address map size/
//// / Cyclone V FPGA device addresses
//const unsigned int LEDR_BASE = 0x00000000;  // Leds offset
//const unsigned int SW_BASE = 0x00000040;  // Switches offset
//const unsigned int KEY_BASE = 0x00000050;  // Push buttons offset
//
////0xFFFEC600 -0xFF200000 = 0xDEC600
//const unsigned int MPCORE_PRIV_TIMER_LOAD_OFFSET = 0xDEC600;
////  Points  to  LOAD Registerconst
//const unsigned int MPCORE_PRIV_TIMER_COUNTER_OFFSET = 0xDEC604;
////  Points  to COUNTER Registerconst
//const unsigned int MPCORE_PRIV_TIMER_CONTROL_OFFSET = 0xDEC608;
////  Points  to CONTROL Registerconst
//const unsigned int MPCORE_PRIV_TIMER_INTERRUPT_OFFSET = 0xDEC60C;
////  Points  to INTERRUPT Register
//
//const unsigned int JP1 = 0x00000060; //speaker offset
//
//const unsigned int DIRECTION_BIT = 0x00000064; //direction bit offset
//
//const unsigned int tones[15] = {261, 293, 329, 349, 392, 440, 493, 523, 587, 659, 698, 783, 880, 987, 1046};
//
//
///** Write a 4-byte value at the specified general-purpose I/O location.  *
// * @param pBase
// * Base address returned by 'mmap'.
// * @parem offset  Offset where device is mapped.
// * @param value   Value to be written.
// * */
//void RegisterWrite(char *pBase, unsigned int reg_offset, int value) {
//    *(volatile unsigned int *) (pBase + reg_offset) = value;
//}
//
///**  * Read a 4-byte value from the specified general-purpose I/O location.
// * *  * @param pBase         Base address returned by 'mmap'.
// * * @param offset  Offset where device is mapped.
// * * @return        Value read.  */
//int RegisterRead(char *pBase, unsigned int reg_offset) {
//    return *(volatile unsigned int *) (pBase + reg_offset);
//}
//
///**  * Initialize general-purpose I/O
// * *  - Opens access to physical memory /dev/mem
// * *  - Maps memory into virtual address space
// *  * @param fd      File descriptor passed by reference, where the result
// *  *                of function 'open' will be stored.
// *  * @return  Address to virtual memory which is mapped to physical, or MAP_FAILED onerror.
// *  */
//
//char *Initialize(int *fd) {
//    // Open /dev/mem to give access to physical addresses
//    *fd = open("/dev/mem", (O_RDWR | O_SYNC));
//    if (*fd == -1)   //  check for errors in openning /dev/mem
//    {
//        cout << "ERROR: could not open /dev/mem..." << endl;
//        exit(1);
//    }   // Get a mapping from physical addresses to virtual addresses
//    char *virtual_base = (char *) mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, *fd, LW_BRIDGE_BASE);
//    if (virtual_base ==
//        MAP_FAILED)       // check for errors
//    {
//        cout << "ERROR: mmap() failed..." << endl;
//        close(*fd);            // close memory before exiting
//        exit(1);        // Returns 1 to the operating system;
//    }
//    return virtual_base;
//}
//
///**  * Close general-purpose I/O.
// * *  * @param pBase   Virtual address where I/O was mapped.
// * * @param fd      File descriptor previously returned by 'open'.  */
//void Finalize(char *pBase, int fd) {
//    if (munmap(pBase, LW_BRIDGE_SPAN) != 0) {
//        cout << "ERROR: munmap() failed..." << endl;
//        exit(1);
//    }
//    close(fd);    // close memory}
//}
//
//void playTone(string note) {
//    // Initialize
//    int fd;
//    char *pBase = Initialize(&fd);
//    int freq;
//    string notes[15] = {"c4", "d4", "e4", "f4", "g4", "a4", "b4", "c5", "d5", "e5", "f5", "g5", "a5", "b5", "c6"};
//
//
//
////    int counter = (200000000 / tones[freq]) / 2; //convert 200 MHz to Hz
////    RegisterWrite(pBase, MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
////    RegisterWrite(pBase, MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
////    int entervalue = 0x000000;
//    int loop = 0;
//    //cin >> note;
//    while (true) {
//        for (int i = 0; i < 15; i++) {
//            if (note == notes[i]) {
//                freq = i;
//            }
//        }
//        int counter = (200000000 / tones[freq]) / 2; //convert 200 MHz to Hz
//        RegisterWrite(pBase, DIRECTION_BIT, 0xffffffff);
//        RegisterWrite(pBase, MPCORE_PRIV_TIMER_LOAD_OFFSET, counter);
//        RegisterWrite(pBase, MPCORE_PRIV_TIMER_CONTROL_OFFSET, 3);
//        int entervalue = 0x000000;
//        while (loop < 5000) {
//            if (RegisterRead(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET) != 0) {
//                RegisterWrite(pBase, MPCORE_PRIV_TIMER_INTERRUPT_OFFSET, 1); // reset timer flagbit
//                entervalue = entervalue ^ 0x1;
//                RegisterWrite(pBase, JP1, entervalue);
//                loop++;
//            }
//        }
//
//        string name;
//        getline(cin, name);
//        note = name;
//        loop = 0;
//        if (note == "0") {
//            break;
//        }
//
//    }
//
//
//    cout << "Terminating sound...!" << endl;
//    Finalize(pBase, fd);
//}

int main(void) {

    midterm *m = new midterm;

    int ans = 0;
    cout << "Program Starting...!" << endl;
    while (ans != 3) {
        cout << "Main Menu:" << endl;
        cout << "Please select one of the options below: " << endl;
        cout << "1. Play a tone" << endl;
        cout << "2. Play Snakes and Ladders" << endl;
        cout << "3. Exit the program." << endl;

        cin >> ans;
        string tone;

        switch (ans) {
            case 1:
                cout << "Please enter the tone between C4 and C6 that you would like to play or press 0 to stop:"
                     << endl;
                cin >> tone;
                m->playTone(tone);
                break;
            case 2:
                cout << "Let's play Snakes and Ladders!" << endl;
                m->playGame();
                break;
            case 3:
                //does nothing as will exit the while loop
                break;
            default:
                cout << "Please select a valid option." << endl;

        }
    }

    return 0;

}




