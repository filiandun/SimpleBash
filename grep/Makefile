CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

CLANG_FORMAT_CONFIG = ../../materials/linters/.clang-format

TARGET = s21_grep

CMN = ../common/file_io.c ../common/ptr_array.c
SRCS = src/grep.c src/grep_core.c src/grep_parser.c src/grep_config.c
HDRS = include/grep_core.h include/grep_parser.h include/grep_config.h

INCLUDES = -Iinclude -Isrc -I../common

TESTS_PATH = tests/tests.sh

$(TARGET):
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCS) $(CMN) -o $@

.PHONY: help clean rebuild debug soft format-check format cppcheck leaks test all


all: $(TARGET)

test:
	bash $(TESTS_PATH)


leaks:
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) a tests/test_files/test_1_grep.txt
	@echo
	valgrind --tool=memcheck --leak-check=yes ./$(TARGET) a no_file.txt

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(INCLUDES) src/*.c include/*.h

format:
	@if [ -f "$(CLANG_FORMAT_CONFIG)" ]; then \
		clang-format -style=file:$(CLANG_FORMAT_CONFIG) -n $(SRCS) $(HDRS); \
		clang-format -style=file:$(CLANG_FORMAT_CONFIG) -i $(SRCS) $(HDRS); \
		echo "SUCCES $(CLANG_FORMAT_CONFIG): $(SRCS) $(HDRS)"; \
	else \
		echo "ERROR: clang-format not found in $(CLANG_FORMAT_CONFIG)"; \
		exit 1; \
	fi

format-check:
	@if [ -f "$(CLANG_FORMAT_CONFIG)" ]; then \
		clang-format -style=file:$(CLANG_FORMAT_CONFIG) -n $(SRCS) $(HDRS);\
		echo "SUCCES $(CLANG_FORMAT_CONFIG): $(SRCS) $(HDRS)"; \
	else \
		echo "ERROR: clang-format not found in $(CLANG_FORMAT_CONFIG)"; \
		exit 1; \
	fi


soft:
	$(CC) -Wall -Wextra -std=c11 $(SRCS) $(CMN) $(INCLUDES) -o $(TARGET)
	
debug:
	$(CC) -g -Wall -Wextra -std=c11 $(SRCS) $(CMN) $(INCLUDES) -o $(TARGET)
	gdb $(TARGET)

rebuild: clean all

clean:
	rm -f $(TARGET)


help:
	@echo "Доступные цели:"
	@echo "  make (all)  		- собрать"
	@echo
	@echo "  make test  		- запустить тесты"
	@echo
	@echo "  make format  		- запустить clang-format -i"
	@echo "  make format-check  	- запустить clang-format -n"
	@echo
	@echo "  make leaks  		- запустить leaks"
	@echo
	@echo "  make cppcheck  	- запустить cppcheck"
	@echo
	@echo "  make soft			- собрать мягко (без -Werror)"
	@echo "  make debug			- запустить отладку"
	@echo "  make rebuild  		- пересобрать"
	@echo "  make clean  		- удалить все скомпилированные файлы"
	@echo "  make help   		- подсказка"
