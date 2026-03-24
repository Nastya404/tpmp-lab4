CC = gcc
# -Iinclude говорит компилятору искать заголовочные файлы в папке include
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lsqlite3

# Папки проекта
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/autopark

# Список исходных файлов в папке src
ALL_SRCS = $(wildcard $(SRC_DIR)/*.c)
# Исключаем из списка sqlite3.c
SRCS = $(filter-out $(SRC_DIR)/sqlite3.c, $(ALL_SRCS))
# Превращаем src/file.c в obj/file.o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Компиляция объектов из src в obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
