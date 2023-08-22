# Project : 令牌桶封装(多线程)实现
# Version : 0.1.2
# Author  : KeeneChen
# DateTime: 2022.10.21-9:50:33

# variable
CC := gcc
CFLAGS += -pthread
LDFLAGS += -pthread
TARGET := mytbf
OBJ := mytbf_test.o mytbf_thread.o 

# explicit rules
$(TARGET):$(OBJ)
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

# pseudo target
.PHONY:
clean:
	rm -rf $(TARGET) 
cleanall:
	rm -rf *.o $(TARGET)