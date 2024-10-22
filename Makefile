CC=gcc
FLAG=-O2 -Wall
CSSHELL=bin/3230SHELL\
 		build/isr_linked_list.o\
		build/isr_dynamic_array.o\
		build/isr_hash_table.o\
		build/shell_io.o\
		build/builtin_exit.o\
		build/builtin_timex.o\
		build/cmd_parser.o\
		build/shell_exe.o\
		build/proc_mag.o\
		build/sig_handler.o\
		build/main.o

TESTS=biuld/parser_test.o\
	  build/shell_exe_test.o\
	  build/hash_table_test.o\
	  bin/parser_test\
	  bin/shell_exe_test\
	  bin/hash_table_test.o

all: $(CSSHELL)

build/isr_linked_list.o: src/isr_linked_list.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv isr_linked_list.o $@

build/isr_dynamic_array.o: src/isr_dynamic_array.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv isr_dynamic_array.o $@

build/isr_hash_table.o: src/isr_hash_table.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv isr_hash_table.o $@

build/shell_io.o: src/shell_io.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv shell_io.o $@

build/builtin_exit.o: src/builtin_exit.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv builtin_exit.o $@

build/builtin_timex.o: src/builtin_timex.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv builtin_timex.o $@

build/cmd_parser.o: src/cmd_parser.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv cmd_parser.o $@

build/shell_exe.o: src/shell_exe.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv shell_exe.o $@

build/proc_mag.o: src/proc_mag.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv proc_mag.o $@

build/sig_handler.o: src/sig_handler.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv sig_handler.o $@

build/main.o: src/main.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv main.o $@

bin/3230SHELL:  build/main.o\
			    build/shell_io.o\
			    build/shell_exe.o\
				build/isr_dynamic_array.o\
				build/isr_linked_list.o\
				build/isr_hash_table.o\
				build/builtin_exit.o\
				build/builtin_timex.o\
				build/cmd_parser.o\
				build/sig_handler.o\
				build/proc_mag.o
	mkdir -p bin
	$(CC) $^ -o bin/3230SHELL $(FLAG)

build/parser_test.o: test/parser_test.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv parser_test.o $<

build/shell_exe_test.o: test/shell_exe_test.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv shell_exe_test.o $<

build/hash_table_test.o: test/hash_table_test.c
	$(CC) $< -c $(FLAG)
	mkdir -p build
	mv hash_table_test.o $<

bin/parser_test: build/parser_test.o\
				 build/cmd_parser.o\
				 build/isr_linked_list.o\
				 build/isr_dynamic_array.o
	mkdir -p bin
	$(CC) $^ -o bin/parser_test $(FLAG)

bin/shell_exe_test: build/shell_exe_test.o\
				    build/cmd_parser.o\
					build/isr_dynamic_array.o\
					build/isr_linked_list.o\
					build/shell_exe.o
	mkdir -p bin
	$(CC) $^ -o bin/shell_exe_test $(FLAG)

bin/hash_table_test: build/hash_table_test.o\
					 build/isr_hash_table.o\
					 build/isr_linked_list.o
	mkdir -p bin
	$(CC) $^ -o bin/hash_table_test $(FLAG)


clean:
	rm build/* bin/*
