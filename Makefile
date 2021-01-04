#Make File

mydisplay: main.o midterm.o TextualSnakesLadders.o
	g++ main.o midterm.o TextualSnakesLadders.o -o mydisplay


main.o: main.cpp midterm.h TextualSnakesLadders.h
	g++ -g -Wall -c main.cpp

midterm.o: midterm.cpp midterm.h TextualSnakesLadders.h
	g++ -g -Wall -c midterm.cpp

TextualSnakesLadders.o: TextualSnakesLadders.cpp TextualSnakesLadders.h
	g++ -g -Wall -c TextualSnakesLadders.cpp


clean:
	rm main.o midterm.o TextualSnakesLadders.o  mydisplay