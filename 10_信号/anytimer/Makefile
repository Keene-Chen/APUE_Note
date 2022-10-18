# Project : 多任务计时器实现
# Version : 0.1.0
# Author  : KeeneChen
# DateTime: 2022.10.17-17:30:43

# variable
CC := gcc
TARGET := anytimer
OBJ := anytimer_test.o anytimer.o 

# explicit rules
$(TARGET):$(OBJ)
	gcc $^ -o $@

# pseudo target
.PHONY:
clean:
	rm -rf $(TARGET) 
cleanall:
	rm -rf *.o $(TARGET)