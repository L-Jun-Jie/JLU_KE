# 定义编译器
CC = g++

# 定义编译选项
CFLAGS = -Wall -std=c++11

# 定义目标文件
TARGET = animalExpert

# 定义源文件
SRC = main.cpp

# 定义头文件
HEADERS = AnimalExpert.h

# 默认目标
all: $(TARGET)

# 编译规则
$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# 清理目标
clean:
	rm -f $(TARGET)
