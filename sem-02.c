#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

//Funktion zum Simulieren von Wartezeiten
void active_wait(int cycles) {
    for (int x=0; x<cycles; x++) {
        for (int y=0; y<cycles; y++) {
            y++; y--;
        }
    }
}

int main() {

	//Zufallszahlengenerator initialisieren
	srand(time(NULL));
	//Ausgabepuffer leeren
	setbuf(stdout, NULL);

	//neue Semaporengruppe erstellen
	//ID der Semaphorengruppe wird in semid gespeichert
	//semget(key, Anzahl der Semaphoren, Zugriffsrechte)
	int semid = semget(0, 1, 0666 | IPC_CREAT);

	//Auf Fehler beim Erstellen der Semphorengruppe prüfen
	if(semid < 0) {
		printf("Fehler beim Erstellen der Semaphorengruppe!\n");
		exit(EXIT_FAILURE);
	}

	//Definition V-Operation - Verlassen der Semaphore (um 1 erhöhen)
	//struct hat 3 Bestandteile:
	//sem_num, sem_op, sem_flg
	//Nummer der Sempahore in der Gruppe, spezifiziert Operation (z.B. -1 oder +1), Flag setzen
	struct sembuf sem_v = {0, 1, 0};

	//Definition P-Operation - Passieren der Sempahore (um 1 dekrementieren)
	struct sembuf sem_p = {0, -1, 0};

	//Semaphorengruppe initalisieren + Fehlerüberprüfung
	if ((semctl(semid, 0, SETVAL, 1)) < 0) {
		printf("Fehler beim Initalisieren der Semaphorengruppe!\n");
		exit(EXIT_FAILURE);
		}

	//Prozess forken
	int id = fork();

    for (int i =100; i>0; i--) {
		//zufällige Zeit warten
		active_wait(rand() % 10000);

		//Vaterprozess
    	if (id > 0) {
			//Semaphore passieren + Fehlermeldung
			if ((semop(semid, &sem_p, 1)) < 0) {
				printf("Fehler beim Passieren der Semaphore (Vater)!\n");
				exit(EXIT_FAILURE);
				}

			//Ausgabe
			printf("VV");

			//Semaphore verlassen + Fehlermeldung
			if ((semop(semid, &sem_v, 1)) < 0) {
				printf("Fehler beim Verlassen der Semaphore (Vater)!\n");
				exit(EXIT_FAILURE);
				}
			}

		//Kindprozess
		else if (id == 0) {
			//Semaphore passieren + Fehlermeldung
			if ((semop(semid, &sem_p, 1)) < 0) {
				printf("Fehler beim Passieren der Semaphore (Kind)!\n");
				exit(EXIT_FAILURE);
				}

			//Ausgabe
        	printf("ss");

			//Semaphore verlassen + Fehlermeldung
			if ((semop(semid, &sem_v, 1)) < 0) {
				printf("Fehler beim Verlassen der Semaphore (Kind)!\n");
				exit(EXIT_FAILURE);
				}
       		}

		//Fehlermeldung falls Prozess-ID beim Forken < 0 ist
		else {
			printf("Fehler beim Forken!\n");
			exit(EXIT_FAILURE);
			}
	}

   exit(EXIT_SUCCESS);
}