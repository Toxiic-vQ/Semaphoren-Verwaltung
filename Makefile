#Programm ausführen
run: compile
	./sem-02.o

#Programm kompilieren
compile: clean
	gcc sem-02.c -o sem-02.o

#erzeugte Ausführungsdateien löschen
clean:
	rm -vf sem-02.o
