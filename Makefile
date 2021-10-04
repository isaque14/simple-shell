  
all:
	gcc bash.c -o bash -lreadline
clean:
	rm bash
