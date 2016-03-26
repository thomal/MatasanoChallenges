NAME=breakRepkeyXOR

all: *.cpp *.h
	g++ -g -Wall -std=c++11 *.cpp -o $(NAME) -lm

debug: $(NAME)
	gdb ./$(NAME)
