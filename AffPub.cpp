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

int main(int argc, char * argv[])
{
	int i, hd;
	PUB BuffPub;

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

	if ((hd = open(PUBS,O_RDONLY)) == -1)
	{ 
		perror("Erreur de FIOuverture()");
		exit(1);
	}

	while(1)
	{
		if(read(hd, &BuffPub, sizeof(PUB)) == 0)
		{
			lseek(hd, 0, SEEK_SET);
			read(hd, &BuffPub, sizeof(PUB));
		}

		strcpy(Tab->pub, BuffPub.Pub);

		for(i = 0 ; i < 5 ; i++)
		{
			if(Tab->User[i].Pid != 0)
			{
				kill(Tab->User[i].Pid, SIGUSR2);
			}
		}

		sleep(5);
	}

	exit(0);
}
