CC=gcc
FLAG=-O2 -Wall
CSSHELL=bin/3230SHELL\
 		build/isr_linked_list.o\
		build/isr_dynamic_array.o\
		build/shell_io.o\
		build/builtin_exit.o\
		build/builtin_timex.o\
		build/cmd_parser.o\
		build/shell_exe.o

TESTS=biuld/parser_test.o\
	  build/shell_exe_test.o\
	  bin/parser_test\
	  bin/shell_exe_test

all: $(CSSHELL) $(TESTS)

build/isr_linked_list.o: src/isr_linked_list.c
	$(CC) $< -c $(FLAG)
	mv isr_linked_list.o build

build/isr_dynamic_array.o: src/isr_dynamic_array.c
	$(CC) $< -c $(FLAG)
	mv isr_dynamic_array.o build


build/shell_io.o: src/shell_io.c
	$(CC) $< -c $(FLAG)
	mv shell_io.o build

build/builtin_exit.o: src/builtin_exit.c
	$(CC) $< -c $(FLAG)
	mv builtin_exit.o build

build/builtin_timex.o: src/builtin_timex.c
	$(CC) $< -c $(FLAG)
	mv builtin_timex.o build

build/cmd_parser.o: src/cmd_parser.c
	$(CC) $< -c $(FLAG)
	mv cmd_parser.o build

build/shell_exe.o: src/shell_exe.c
	$(CC) $< -c $(FLAG)
	mv shell_exe.o build

build/main.o: src/main.c
	$(CC) $< -c $(FLAG)
	mv main.o build

bin/3230SHELL:  build/main.o\
			    build/shell_io.o\
			    build/shell_exe.o\
				build/isr_dynamic_array.o\
				build/isr_linked_list.o\
				build/builtin_exit.o\
				build/builtin_timex.o\
				build/cmd_parser.o
	$(CC) $^ -o bin/3230SHELL $(FLAG)

build/parser_test.o: test/parser_test.c
	$(CC) $< -c $(FLAG)
	mv parser_test.o build

build/shell_exe_test.o: test/shell_exe_test.c
	$(CC) $< -c $(FLAG)
	mv shell_exe_test.o build

bin/parser_test: build/parser_test.o\
				 build/cmd_parser.o\
				 build/isr_linked_list.o\
				 build/isr_dynamic_array.o
	$(CC) $^ -o bin/parser_test $(FLAG)

bin/shell_exe_test: build/shell_exe_test.o\
				    build/cmd_parser.o\
					build/isr_dynamic_array.o\
					build/isr_linked_list.o\
					build/shell_exe.o
	$(CC) $^ -o bin/shell_exe_test $(FLAG)

clean:
	rm build/* bin/*
