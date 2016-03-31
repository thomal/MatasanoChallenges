NAME=aes_ecb_detect

all: src/*.cpp include/*.h
	grep TODO src/*.cpp include/*h
	g++ -g -Wall -std=c++11 -I include src/*.cpp -o $(NAME)

solve: $(NAME)
	./$(NAME) -i hexfile_tokens -y ciphertexts

debug: $(NAME)
	gdb ./$(NAME)
