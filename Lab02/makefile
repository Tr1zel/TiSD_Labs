CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -Werror -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla 

all: app.exe

app.exe: io_funcs.o table_funcs.o main.o
	$(CC) $(CFLAGS) $^ -o $@

io_funcs.o: io_funcs.c
	$(CC) $(CFLAGS) -c $^ -o $@

table_funcs.o: table_funcs.c
	$(CC) $(CFLAGS) -c $^ -o $@ 

main.o: main.c 
	$(CC) $(CFLAGS) -c $^ -o $@ 

.PHONY: all clean

clean: 
	rm -rf *.o *.exe 