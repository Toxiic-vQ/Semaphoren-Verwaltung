run: compile
	./sem-02.o

compile: clean
	gcc sem-02.c -o sem-02.o

clean:
	rm -vf sem-02.o
