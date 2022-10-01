CC=gcc
CSSHELL=bin/3230SHELL\
 		build/isr_linked_list.o\
		build/shell_io.o\
		build/builtin_exit.o\
		build/builtin_timex.o\
		build/sig_handler.o\
		build/shell_exe.o

all: $(CSSHELL)

build/isr_linked_list.o: src/isr_linked_list.c
	$(CC) $< -c
	mv isr_linked_list.o build

build/shell_io.o: src/shell_io.c
	$(CC) $< -c
	mv shell_io.o build

build/builtin_exit.o: src/builtin_exit.c
	$(CC) $< -c
	mv builtin_exit.o build

build/builtin_timex.o: src/builtin_timex.c
	$(CC) $< -c
	mv builtin_timex.o build

build/shell_exe.o: src/shell_exe.c
	$(CC) $< -c
	mv shell_exe.o build


bin/3230SHELL: src/main.c\
			    build/shell_io.o\
			    build/shell_exe.o\
				build/builtin_exit.o\
				build/builtin_timex.o
	$(CC) $^ -o 3230SHELL
	mv 3230SHELL bin

clean:
	rm build/* bin/*
