# Настройки компилятора
CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = -lsqlite3

# Папки
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data

# Имя исполняемого файла
TARGET = $(BIN_DIR)/autopark

# Список исходных файлов
SRCS = main.c db_core.c crud.c reports.c
# Превращаем .c в .o внутри папки obj/
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Главная цель
all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Компиляция объектов в папку OBJ
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Создание папок
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DATA_DIR):
	mkdir -p $(DATA_DIR)

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
