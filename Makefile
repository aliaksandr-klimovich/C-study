CC=gcc
CFLAGS=-g

all:
	make 01 02 03 04 05 06 07 09

01:
	$(CC) $(CFLAGS) 01_stack_static.c -o 01.out

02:
	$(CC) $(CFLAGS) 02_stack_dynamic_multiplication.c -o 02.out

03:
	$(CC) $(CFLAGS) 03_stack_dynamic_addition.c -o 03.out

04:
	$(CC) $(CFLAGS) 04_stack_unsing_single_linked_list.c -o 04.out

05:
	$(CC) $(CFLAGS) 05_singly_linked_list.c -o 05.out

06:
	$(CC) $(CFLAGS) 06_singly_linked_list_optimizations.c -o 06.out

07:
	$(CC) $(CFLAGS) 07_singly_linked_list_sort.c -o 07.out

09:
	$(CC) $(CFLAGS) 09_huffman_archiver.c -o 09.out

clean:
	rm -rf *.o *.out *.dSYM/ Debug/