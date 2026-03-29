CC = gcc

CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/autopark

ALL_SRCS = $(wildcard $(SRC_DIR)/*.c)

# ОС-зависимая логика 
ifeq ($(OS),Windows_NT)
    # Windows: используем sqlite3.c
    SRCS = $(ALL_SRCS)
    SQLITE_OBJ = $(OBJ_DIR)/sqlite3.o
else
    # Linux: исключаем sqlite3.c
    SRCS = $(filter-out $(SRC_DIR)/sqlite3.c, $(ALL_SRCS))
    LDFLAGS += -lsqlite3
endif

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Флаги только для sqlite3.c
SQLITE_CFLAGS = -Iinclude -Wno-unused-but-set-variable -Wno-unused-parameter

#  команды 
ifeq ($(OS),Windows_NT)
	MKDIR_BIN = if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
	MKDIR_OBJ = if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
	RMDIR_OBJ = if exist "$(OBJ_DIR)" rmdir /s /q "$(OBJ_DIR)"
	RMDIR_BIN = if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
	RUN_CMD = .\bin\autopark.exe
else
	MKDIR_BIN = mkdir -p "$(BIN_DIR)"
	MKDIR_OBJ = mkdir -p "$(OBJ_DIR)"
	RMDIR_OBJ = rm -rf "$(OBJ_DIR)"
	RMDIR_BIN = rm -rf "$(BIN_DIR)"
	RUN_CMD = ./bin/autopark
endif

# сборка 
all: $(TARGET)

$(TARGET): $(OBJS)
	@$(MKDIR_BIN)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Только для Windows
$(OBJ_DIR)/sqlite3.o: $(SRC_DIR)/sqlite3.c
	@$(MKDIR_OBJ)
	$(CC) $(SQLITE_CFLAGS) -c $< -o $@

# Общая компиляция
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR_OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RMDIR_OBJ)
	@$(RMDIR_BIN)

run: all
	$(RUN_CMD)

.PHONY: all clean run

