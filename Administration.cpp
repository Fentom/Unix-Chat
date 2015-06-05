#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/varargs.h>
#include <sys/types.h>
#include <fcntl.h>

#include "Fichier.ini"

int idSM;
MEMORY	*Tab;

int Menu();

int main(int argc, char * argv[])
{
	int choix = 0;
	pid_t pid = 0;

 	//------------------------------------- Creation de la SHARED MEMORY  ----------------------------
	if ((idSM = shmget(SHM, 0, NULL)) == -1) // On récupére la mémoire partagée, si elle existe
	{
		perror("Erreur shared memory");
		exit(1);
	}


	if((Tab = (MEMORY*)shmat(idSM, NULL, 0)) == (MEMORY*) -1)//On se raccroche à la mémoire avec un pointeur de struct memoire
	{
		perror("Erreur rattachement shared memory");
		exit(1);
	}

	do
	{
		choix = Menu();

		switch(choix)
		{
			case 1:
				printf("Entrer le pid du serveur a stopper : ");
				scanf("%d", &pid);

				kill(pid, SIGSTOP);
			break;
			case 2:
				printf("Entrer le pid du client a stopper : ");
				scanf("%d", &pid);

				kill(pid, SIGSTOP);
			break;
			case 3:
				printf("Entrer le pid du serveur a reprendre : ");
				scanf("%d", &pid);

				kill(pid, SIGCONT);
			break;
			case 4:
				printf("Entrer le pid du client a reprendre : ");
				scanf("%d", &pid);

				kill(pid, SIGCONT);
			break;
			case 5:
				printf("Entrer le pid du serveur a arreter : ");
				scanf("%d", &pid);

				kill(pid, SIGINT);
			break;
		}
	} while ( choix != 0);

	exit(0);
}

int Menu()
{
	int i, choix = 0;

	printf("ADMINISTRATION\n\n");

	for(i = 0 ; i < 2 ; i++)
	{
		printf("Serveur (%d) : %d\n", i+1, Tab->pid[i]);
	}

	printf("\n");
	printf("Personnes connectees : \n");

	for(i = 0 ; i < 5 ; i ++)
	{
		if(strcmp(Tab->User[i].NomUtilisateur, "") != 0)
		{
			printf("%s\t(%d)\n", Tab->User[i].NomUtilisateur, Tab->User[i].Pid);
		}
	}

	printf("\n");

	printf("\t 1 - Stopper un serveur :\n");
	printf("\t 2 - Stopper un client :\n");
	printf("\n");
	printf("\t 3 - Reprendre un serveur :\n");
	printf("\t 4 - Reprendre un client :\n");
	printf("\n");
	printf("\t 5 - Arreter un serveur :\n");
	printf("\n");
	printf("\t 0 - Terminer\n\n");

	printf("Votre choix? ");
	scanf("%d", &choix);

	return choix;
}
