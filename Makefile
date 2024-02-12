all: wordle

wordle: wordle.cpp
	g++ wordle.cpp -o wordle

clean:
	rm wordle
