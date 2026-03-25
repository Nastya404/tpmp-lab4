CC = gcc
# -Iinclude говорит компилятору искать заголовочные файлы в папке include
CFLAGS = -Wall -Wextra -Iinclude
# Для Windows в системе часто нет dev-библиотеки sqlite3, поэтому используем локальную амальгамацию.
LDFLAGS =

# Папки проекта
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/autopark

# Список исходных файлов в папке src
ALL_SRCS = $(wildcard $(SRC_DIR)/*.c)
# Включаем sqlite3.c (амальгамация) в сборку
SRCS = $(ALL_SRCS)
# Превращаем src/file.c в obj/file.o
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Флаги для сторонних библиотек (без -Wall -Wextra)
SQLITE_CFLAGS = -Iinclude -Wno-unused-but-set-variable -Wno-unused-parameter

# Унифицированные команды для создания/удаления папок
ifeq ($(OS),Windows_NT)
	MKDIR_BIN = if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	MKDIR_OBJ = if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	RMDIR_OBJ = if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	RMDIR_BIN = if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
else
	MKDIR_BIN = mkdir -p "$(BIN_DIR)"
	MKDIR_OBJ = mkdir -p "$(OBJ_DIR)"
	RMDIR_OBJ = rm -rf "$(OBJ_DIR)"
	RMDIR_BIN = rm -rf "$(BIN_DIR)"
endif

all: $(TARGET)

# Сборка исполняемого файла
$(TARGET): $(OBJS)
	@$(MKDIR_BIN)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Специальное правило для sqlite3.c
$(OBJ_DIR)/sqlite3.o: $(SRC_DIR)/sqlite3.c
	@$(MKDIR_OBJ)
	$(CC) $(SQLITE_CFLAGS) -c $< -o $@

# Компиляция объектов из src в obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RMDIR_OBJ)
	@$(RMDIR_BIN)

run: all
	.\bin\autopark.exe	

.PHONY: all clean run
