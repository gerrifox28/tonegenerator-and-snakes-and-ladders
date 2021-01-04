//
// Created by Gerri Fox on 10/25/20.
//

#ifndef FINALMIDTERM_MIDTERM_H
#define FINALMIDTERM_MIDTERM_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>

#include "TextualSnakesLadders.h"

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






class midterm : public TextualSnakesLadders{
public:
    /** Write a 4-byte value at the specified general-purpose I/O location.  *
 * @param pBase
 * Base address returned by 'mmap'.
 * @parem offset  Offset where device is mapped.
 * @param value   Value to be written.
 * */
    void RegisterWrite(char *pBase, unsigned int reg_offset, int value);

    /**  * Read a 4-byte value from the specified general-purpose I/O location.
 * *  * @param pBase         Base address returned by 'mmap'.
 * * @param offset  Offset where device is mapped.
 * * @return        Value read.  */
    int RegisterRead(char *pBase, unsigned int reg_offset);

    /**  * Initialize general-purpose I/O
 * *  - Opens access to physical memory /dev/mem
 * *  - Maps memory into virtual address space
 *  * @param fd      File descriptor passed by reference, where the result
 *  *                of function 'open' will be stored.
 *  * @return  Address to virtual memory which is mapped to physical, or MAP_FAILED onerror.
 *  */

    char *Initialize(int *fd);

    /**  * Close general-purpose I/O.
 * *  * @param pBase   Virtual address where I/O was mapped.
 * * @param fd      File descriptor previously returned by 'open'.  */
    void Finalize(char *pBase, int fd);


    void playTone(string note);

    void playGame();

    void playOneTone();


};


#endif //FINALMIDTERM_MIDTERM_H
