NAME=pkcs7

all: src/*.cpp include/*.h
	grep TODO src/*.cpp include/*h
	g++ -g -Wall -std=c++11 -I include src/*.cpp -o $(NAME)

solve: $(NAME)
	./$(NAME) -i raw -I hex -o hex -x 14 -y "YELLOW SUBMARINE" -l

debug: $(NAME)
	gdb ./$(NAME)
