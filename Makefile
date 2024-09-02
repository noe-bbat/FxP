CC=g++
CCFLAGS= -Wall -Werror -g 
LIBFLAGS= /usr/local/lib/libCatch2Main.a /usr/local/lib/libCatch2.a 
SRC= test_case.cpp
OBJ= $(SRC:.c=.o)
OBJ_TEST=$(SRC_TEST:.c=.o)
EXEC= testcase



all: $(EXEC)


$(EXEC): $(OBJ)
	$(CC)  $^ -o $@  $(LIBFLAGS)


%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

.depends:
	$(CC) -MM $(SRC) > .depends
	
clean:
	rm -f  $(EXEC) *.*~



