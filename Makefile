all:
	make 01
	make 02
	make 03
	make 04
	make 05
	make 06

01:
	gcc -g 01_stack_static.c -o 01.out

02: 
	gcc -g 02_stack_dynamic_multiplication.c -o 02.out

03:
	gcc -g 03_stack_dynamic_addition.c -o 03.out

04:
	gcc -g 04_stack_unsing_single_linked_list.c -o 04.out

05: 
	gcc -g 05_singly_linked_list.c  -o 05.out

06:
	gcc -g 06_singly_linked_list_optimizations.c -o 06.out

clean:
	rm -rf *.o *.out *.dSYM/ Debug/