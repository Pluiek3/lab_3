# Определение компилятора
CC = gcc

# Опции компиляции
CFLAGS = -I./include -I./mongoose -Wall -Wextra -pthread -DMG_ENABLE_HTTP=1

# Исходные файлы
SRC = src/main.c src/http_handler.c src/responses.c src/sleep_logic.c src/time_utils.c mongoose/mongoose.c

# Объектные файлы
OBJ = $(SRC:.c=.o)

# Имя целевого исполняемого файла
TARGET = sleep_server

# Основное правило сборки
all: $(TARGET)

# Правило для создания исполняемого файла
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Правило для создания объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Правило для очистки сгенерированных файлов
clean:
	rm -f $(OBJ) $(TARGET)

# Правило для запуска сервера
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
