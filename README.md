<h2>Programm zur Verwaltung von Prozess mithilfe von Semaphoren</h2>

<h3>Programmbeschreibung: </h3>
In diesem Programm wird ein Prozess geforkt. Der Vater- sowie Kindprozess durchlaufen eine Schleife 100 mal und sollen abwechselnd Zeichen ausgeben.
  
<h3>Grober Programmablauf: </h3>
- Erstellung der Semaphorengruppe <br>
- Initialisierung der Semaphorengruppe <br>
- Prozess forken <br>
- Schleife mit 100 Durchläufen: <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - Semaphore passieren (<b>Beginn des kritischen Codeabschnitts</b>) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - wenn Prozess = Vater (PID > 0) --> Ausgabe "VV" <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - wenn Prozess = Kind (PID = 0) --> Ausgabe "ss" <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - Wartezeit simulieren <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - Semaphore verlassen (<b>Ende des kritischen Codeabschnitts</b>) <br>
- Prozess beenden 

<h3> Nutzung und erwarteter Output: </h3>
  Mithilfe des Makefile kann das Programm durch "make run" kompiliert und ausgeführt werden. <br> <br>
  
```
felix@kubuntu:/sem-02$ make run
rm -vf sem-02.o
gcc sem-02.c -o sem-02.o
./sem-02.o
VVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVssVVss
```
