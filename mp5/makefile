lab6: lab6.o
	gcc -Wall -o lab6 lab6.c
run:
	./snort6 5 0 321 | ./lab6 1 0
	@echo " . . . . . . . . . . . . . . . . . . . . ."
	./snort6 10 0 123 > file.data
	@echo " . . . . . . . . . . . . . . . . . . . . ."
	./lab6 2 1 < file.data > file.txt
	@echo " . . . . . . . . . . . . . . . . . . . . ."
	./snort6 10000 0 231 | ./lab6 4 1 > /dev/null
	@echo " . . . . . . . . . . . . . . . . . . . . ."
	./snort6 1000000 0 312 | ./lab6 4 1 > /dev/null
	@echo " . . . . . . . . . . . . . . . . . . . . ."
port:
	./snort6 1000000 1 | ./lab6 5 > /dev/null
clean:
	rm *.o