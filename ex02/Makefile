NAME = PmergeMe 

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -Iinclude
LDFLAGS =

SANFLAGS = -g -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer

SRC_DIR = src/
OBJ_DIR = obj/
SRC_FILES = main.cpp PmergeMe.cpp CountedInt.cpp
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_FILES:.cpp=.o))

TEST = test/test.sh

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += $(SANFLAGS)
debug: LDFLAGS += $(SANFLAGS)
debug: fclean all

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

val: re
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)
	
leak_check: debug
	ASAN_OPTIONS=detect_leaks=1 \
	LSAN_OPTIONS=use_registers=0:report_objects=1 \
	./$(NAME)

test: debug
	./$(TEST)

.PHONY: all debug clean fclean re val leak_check test
