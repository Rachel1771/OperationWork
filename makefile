#.PHONY:clean memoryAllocation
memoryAllocation:memoryAllocation.c	
	gcc -g $^ -o $@
.PHONY:clean
clean:
	rm memoryAllocation
