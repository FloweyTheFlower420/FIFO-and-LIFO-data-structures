CC=g++
OBJ=v_alloc.o examples.o
OUT=examples.elf

%.o : %.cpp
	$(CC) -c $< -o $@
$(OUT) : $(OBJ)
	$(CC) $(OBJ) -o $@
