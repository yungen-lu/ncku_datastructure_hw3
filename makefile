3-1: F74109040_hw3-1.cpp
	g++ -O2 -std=gnu++11 -pedantic F74109040_hw3-1.cpp -o F74109040_hw3-1
3-2: F74109040_hw3-2.cpp
	g++ -O2 -std=gnu++11 -pedantic F74109040_hw3-2.cpp -o F74109040_hw3-2
3-3: F74109040_hw3-3.cpp
	g++ -O2 -std=gnu++11 -pedantic F74109040_hw3-3.cpp -o F74109040_hw3-3
3-4: F74109040_hw3-4.cpp
	g++ -O2 -std=gnu++11 -pedantic F74109040_hw3-4.cpp -o F74109040_hw3-4
all: 3-1 3-2 3-3 3-4


clean3-1:
	rm -rf F74109040_hw3-1
clean3-2:
	rm -rf F74109040_hw3-2
clean3-3:
	rm -rf F74109040_hw3-3
clean3-4:
	rm -rf F74109040_hw3-4
cleanAll: clean3-1 clean3-2 clean3-3 clean3-4 cleanDebugFiles

cleanDebugFiles:
	rm -rf *.dSYM

