CC=mpicc
CCFLAGS=-std=c99 -Wall -Werror
LDFLAGS=
DIR_INC=include
DIR_SRC=src
DIR_OBJ=obj
DIR_BIN=bin

.PHONY: hello hello-solution clean

hello: $(DIR_BIN)/hello/main
hello-solution: $(DIR_BIN)/hello/solution

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	$(CC) $(CCFLAGS) -I $(DIR_INC)/$(dir $*) -o $@ -c $<

$(DIR_BIN)/hello/main: $(DIR_OBJ)/hello/main.o
$(DIR_BIN)/hello/solution: $(DIR_OBJ)/hello/solution.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(DIR_OBJ)/*/*.o $(DIR_BIN)/*/*
