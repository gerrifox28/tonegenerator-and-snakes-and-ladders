# tonegenerator-and-snakes-and-ladders
A two part project built for the DE1-SoC board that features a user interface to play the classic children's game of Snakes and Ladders with stellar sound features from the DE1-SoC board speaker and a tone generator that generates tones from C5-G6 using the DE1-SoC board speaker.

This code is written in C++ using CLion. It is built for the DE1-SoC board using the proper constants from the official manual. The code can be put together and ran via the Terminal command line. 

The code is designed using object-oriented principles and split into .h and .cpp files for convience and improved code readability. 

The Tone Generator portion is represented via the midterm.h and midterm.cpp files with the main.cpp file connecting both pieces and a Makefile is conviently provided to compile the entire project in the command line. 

The DE1-SoC board is connected to a computer via a USB to Mini USB connection and uses Screen to connect the board to the computer. 

The project uses a user-friendly textual based user interface to navigate and select each part of the project. The Tone Generator receives tones to play via this textual UI as well as the game of Snakes and Ladders is played via command line prompt with a textual game board and textual game cues while accompanying sound is synced to the DE1-SoC board speaker. 
