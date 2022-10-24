# Project : 信号量
# Version : 0.1.0
# Author  : KeeneChen
# DateTime: 2022.10.24-8:46:33

# variable
CC := gcc
CFLAGS += -pthread
LDFLAGS += -pthread
TARGET := mysem
OBJ := mysem_test.o mysem.o 

# explicit rules
$(TARGET):$(OBJ)
	gcc $^ -o $@ $(CFLAGS) $(LDFLAGS)

# pseudo target
.PHONY:
clean:
	rm -rf $(TARGET) 
cleanall:
	rm -rf *.o $(TARGET)