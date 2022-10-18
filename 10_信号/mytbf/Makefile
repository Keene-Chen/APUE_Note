# Project : 令牌桶封装实现
# Version : 0.1.1
# Author  : KeeneChen
# DateTime: 2022.10.18-20:45:56

# variable
CC := gcc
TARGET := mytbf
OBJ := mytbf_test.o mytbf_refactor.o 

# explicit rules
$(TARGET):$(OBJ)
	gcc $^ -o $@

# pseudo target
.PHONY:
clean:
	rm -rf $(TARGET) 
cleanall:
	rm -rf *.o $(TARGET)