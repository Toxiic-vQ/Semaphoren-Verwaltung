#include <stdio.h>		//für printf, setbuf, ...
#include <stdlib.h>		//für exit
#include <unistd.h>		//für fork
#include <sys/ipc.h>		//für Semaphorenflags
#include <sys/sem.h>		//für Semaphorenfunktionen
#include <time.h>		//für Zufallszahlen

//Funktion zum Simulieren von Wartezeiten
void active_wait(int cycles) {
    for (int x=0; x<cycles; x++) {
        for (int y=0; y<cycles; y++) {
            y++; y--;
        }
    }
}

//Main-Funktion
int main() {

	//neue Semaporengruppe erstellen
	//ID der Semaphorengruppe wird in semid gespeichert
	//semget(key, Anzahl der Semaphoren, Zugriffsrechte)
	//der Key ist 0, da wir keine bestehende Semaphorengruppe verwenden wollen
	//hier wird eine Semaphorengruppe mit nur einer Semaphore erstellt
	//die Zugriffsrechte sind 0666, also kann jeder lesen und schreiben
	int semid = semget(0, 1, 0666 | IPC_CREAT);

	//Auf Fehler beim Erstellen der Semphorengruppe prüfen
	//bei einem Fehler wird das Programm beendet
	if(semid < 0) {
		printf("Fehler beim Erstellen der Semaphorengruppe!\n");
		exit(EXIT_FAILURE);
	}

	//struct sembuf hat 3 Bestandteile:
	//sem_num, sem_op, sem_flg
	//Nummer der Sempahore in der Gruppe, spezifiziert Operation (z.B. -1 oder +1), Flag setzen
	//da wir nur eine Semaphore haben, ist die Nummer immer 0
	//die sem_flg ist 0, da wir keine Flags benötigen

	//Definition P-Operationen - Passieren der Sempahore (um 1 dekrementieren)
	struct sembuf sem_p = {0, -1, 0};

	//Definition V-Operationen - Verlassen der Semaphore (um 1 erhöhen)
	struct sembuf sem_v = {0, 1, 0};

	//Semaphorengruppe initalisieren + Fehlerüberprüfung
	//semctl(ID der Semaphorengruppe, Nummer der Semaphore, Operation, Wert)
	//hier wird die Semaphore mit dem Wert 1 initalisiert
	if ((semctl(semid, 0, SETVAL, 1)) < 0) {
		printf("Fehler beim Initalisieren der ersten Semaphore!\n");
		exit(EXIT_FAILURE);
		}

	//Prozess forken
	int id = fork();

	//Zufallszahlengenerator initialisieren
	srand(time(NULL) + getpid());
	//Ausgabepuffer leeren
	setbuf(stdout, NULL);

    for (int i =100; i>0; i--) {

		//Semaphore passieren + Fehlerüberprüfung
		//ab hier beginnt der kritische Codeabschnitt
		//wenn ein Prozess die Semaphore passiert, kann kein anderer Prozess die Semaphore passieren
		//semop(ID der Semaphorengruppe, struct sembuf, Anzahl der Operationen)
		if ((semop(semid, &sem_p, 1)) < 0) {
			printf("Fehler beim Passieren der  Semaphore!\n");
			exit(EXIT_FAILURE);
		}

		//Vaterprozess
    	if (id > 0) {
			//Ausgabe
			printf("VV");
		}

		//Kindprozess
		else if (id == 0) {
			//Ausgabe
			printf("ss");
		}

		//Fehler beim Forken + Programm beenden
		else {
			printf("Fehler beim Forken!\n");
			exit(EXIT_FAILURE);
		}

		//Wartezeit simulieren
		active_wait(rand() % 10000);

		//Semaphore verlassen + Fehlerüberprüfung
		//hier endet der kritische Codeabschnitt
		if ((semop(semid, &sem_v, 1)) < 0) {
			printf("Fehler beim Verlassen der  Semaphore!\n");
			exit(EXIT_FAILURE);
		}
	}

	//Prozess beenden
	exit(EXIT_SUCCESS);
}
