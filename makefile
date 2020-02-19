math: Main.o
	g++ Main.o -o math

MOain.o: Main.cpp
	g++ -c Main.cpp

clean:
	rm *.o math