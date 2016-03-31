NAME=aes_ecb

all: src/*.cpp include/*.h
	grep TODO src/*.cpp include/*h
	g++ -g -Wall -std=c++11 -I include src/*.cpp -o $(NAME)

solve: $(NAME)
	./$(NAME) -i b64file -I raw -o raw -y encrypted -x "YELLOW SUBMARINE" -m decrypt -l

debug: $(NAME)
	gdb ./$(NAME)
