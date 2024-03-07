# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -Wall -Wextra -std=c++23
# 输出目录
BUILD_DIR = ../build

# 目标文件
TARGET = $(BUILD_DIR)/ipmsg
# 源文件
SRCS = broadcast.cpp filelist.cpp  main.cpp tcp_send_file.cpp user.cpp write_log.cpp file.cpp
SRCS += keyboard.cpp pack_unpack.cpp udp_progress.cpp userlist.cpp history.cpp cfg_init.cpp
# 目标文件
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# 默认目标
all: $(TARGET)

# 生成目标文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 编译每个源文件
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 创建输出目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# 清理生成的文件
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
