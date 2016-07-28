# make relationship file for JuYoShell_v1
JuYoShell_v1: Chronos.o JuYoShell_v1.o
	g++ -std=c++11 -o JuYoShell_v1 JuYoShell_v1.o Chronos.o -g0 -O3

JuYoShell_v1.o: JuYoShell_v1.cpp
	g++ -std=c++11 -c JuYoShell_v1.cpp -o JuYoShell_v1.o -g0 -O3

Chronos.o: Chronos.cpp Chronos.hpp
	g++ -std=c++11 -c Chronos.cpp -o Chronos.o -g0 -O3

clean: 
	rm *.o
	rm JuYoShell_v1
