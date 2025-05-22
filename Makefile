# Компилятор и флаги
CC = gcc
CFLAGS = -I./include -I./mongoose -Wall -Wextra -pthread -DMG_ENABLE_HTTP=1
DEBUG_CFLAGS = -g -O0
RELEASE_CFLAGS = -O2 -DNDEBUG

# Режим сборки (debug/release)
ifeq ($(MODE),debug)
    CFLAGS += $(DEBUG_CFLAGS)
else
    CFLAGS += $(RELEASE_CFLAGS)
endif

# Исходные файлы
SRC = src/main.c src/http_handler.c src/responses.c src/sleep_logic.c src/time_utils.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

# Mongoose
MG_PATH = mongoose
MG_OBJ = $(MG_PATH)/mongoose.o
MG_LIB = $(MG_PATH)/libmongoose.a

# Целевой исполняемый файл
TARGET = bin/sleep_server

# Ресурсы
RESOURCES = css/styles.css templates/sleep.html

# Основная цель
all: $(TARGET) copy-resources

# Сборка основного приложения
$(TARGET): $(OBJ) $(MG_LIB)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(MG_LIB)

# Создание статической библиотеки Mongoose
$(MG_LIB): $(MG_OBJ)
	ar rcs $@ $^

# Компиляция Mongoose
$(MG_OBJ): $(MG_PATH)/mongoose.c $(MG_PATH)/mongoose.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Правило для объектных файлов
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Генерация зависимостей
%.d: %.c
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) -MF $@ $<

# Копирование ресурсов
copy-resources: $(RESOURCES)
	@mkdir -p bin/css bin/templates
	@cp -f $(RESOURCES) bin/

# Очистка
clean:
	rm -f $(OBJ) $(DEP) $(TARGET) $(MG_OBJ) $(MG_LIB)
	rm -rf bin/

# Запуск
run: $(TARGET) copy-resources
	./$(TARGET)

.PHONY: all clean run copy-resources

-include $(DEP)