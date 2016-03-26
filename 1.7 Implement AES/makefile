NAME=aes_ecb

all: src/*.cpp include/*.h
	grep TODO src/*.cpp include/*h
	g++ -g -Wall -std=c++11 -I include src/*.cpp -o $(NAME)

debug: $(NAME)
	gdb ./$(NAME)
