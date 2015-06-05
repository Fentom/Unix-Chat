/* Serveur.c
*/
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
#include <sys/sem.h>
#include <errno.h>

#include "Fichier.ini"

union semun {
	int val;
} arg;

int idMsg;
int idSM;
int idSemSM;
MEMORY	*Tab;

void Requete_NewWindow(MESSAGE M);
void Requete_Login(MESSAGE M);
void Requete_EndWindow(MESSAGE M);
void Requete_Terminer(MESSAGE M);
void Requete_Envoyer(MESSAGE M);
void Requete_Accepter(MESSAGE M);
void Requete_Refuser(MESSAGE M);
void Requete_Rechercher(MESSAGE M);
void AfficheTab();

void handlerSigInt(int);
void handlerSigChld(int);

void sem_wait(int idSem);
void sem_signal(int idSem);

int main()
{
	int	rc, i, found;
	char B[80];
	MESSAGE	M;
	pid_t idFils;

	struct sigaction sigact;

	// Armement du signal SIGINT
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = handlerSigInt;
    sigaction(SIGINT, &sigact, NULL);

    // Armement du signal SIGCHLD
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = handlerSigChld;
    sigaction(SIGCHLD, &sigact, NULL);


	//---------------------------- Creation de la FILE DE MESSAGE -------------------------------------
	if ((idMsg = msgget(KEY,IPC_CREAT|IPC_EXCL|0600)) == -1)
	{ 
		if ((idMsg = msgget(KEY,0)) == -1) // On récupére la file de message, si elle existe
		{
			perror("Erreur file de message");
			exit(1);
		} 
	}

 	//------------------------------------- Creation de la SHARED MEMORY  ----------------------------
	if((idSM = shmget(SHM, sizeof(MEMORY), IPC_CREAT|IPC_EXCL|0600)) == -1)
	{
	  if ((idSM = shmget(SHM, 0, NULL)) == -1) // On récupére la mémoire partagée, si elle existe
	  {
	    perror("Erreur shared memory");
	    exit(1);
	  }
	}

	if((Tab = (MEMORY*)shmat(idSM, NULL, 0)) == (MEMORY*) -1)//On se raccroche à la mémoire avec un pointeur de struct memoire
	{
	  perror("Erreur rattachement shared memory");
	  exit(1);
	}

	//---------------------- Creation du SEMAPHORE pour la shared memory  ----------------------------
	if((idSemSM = semget(SEMA, 1, IPC_CREAT|IPC_EXCL|0600)) == -1)
	{
	  if((idSemSM = semget(SEMA, 0, 0)) == -1)
	  {
	    perror("Erreur creation semaphore");
	    exit(1);
	  }
	}
	else
	{
		// Initialisation du SEMAPHORE pour la shared memory 

		arg.val = 1;
		semctl(idSemSM, 0, SETVAL, arg);
	}

	//------------------ Avant de commencer, on va inscrire son pid dans la mémoire partagé
	found = 0;
	sem_wait(idSemSM);
	for (i = 0; i < 2 && found == 0; ++i)
	{
		if(Tab->pid[i] == 0)
		{
			Tab->pid[i] = getpid();
			found = 1;
		}
	}
	sem_signal(idSemSM);

	if (i == 2 && found == 0)
	{
		printf("Erreur deux serveurs sont deja presents");
		exit(1);
	}

	//---------------------- Creation du PROCESS AFFPUB  ----------------------------
	if ((idFils = fork()) == -1)
	{
		perror("Erreur creation fils AffPub:");
		exit(1);
	}

	if (!idFils)
	{
		// Code du Fils
		if(execl("./AffPub", "AffPub", NULL) == -1 )
		{
			perror("Erreur creation du fils AffPub: ");
			exit(1);
		}
		printf("Creation du fils AffPub\n");
	}

	while(1)
	{
	  if( msgrcv(idMsg,&M,sizeof(M) - sizeof(long),1,0) == -1 )
	  {
			if(errno == EINTR)
                continue;

	  	perror("Erreur:");
	  	exit(0);
	  }

		switch(M.Requete)
		{ 
			case NEWWINDOW:
				sem_wait(idSemSM);
				Requete_NewWindow(M);
				sem_signal(idSemSM);
			break;

			case LOGIN:
				sem_wait(idSemSM);
				Requete_Login(M);
				sem_signal(idSemSM);
			break;

			case ENDWINDOW:
				sem_wait(idSemSM);
				Requete_EndWindow(M);
				sem_signal(idSemSM);
			break;

			case TERMINER:
				sem_wait(idSemSM);
				Requete_Terminer(M);
				sem_signal(idSemSM);
			break;

			case RECHERCHER:
				Requete_Rechercher(M);
			break;

			case ANNULER:
			break;

			case MODIFIER:
			break;

			case ENVOYER:
				sem_wait(idSemSM);
				Requete_Envoyer(M);
				sem_signal(idSemSM);
			break;

			case ACCEPTER:
				sem_wait(idSemSM);
				Requete_Accepter(M);
				sem_signal(idSemSM);
			break;

			case REFUSER:
				sem_wait(idSemSM);
				Requete_Refuser(M);
				sem_signal(idSemSM);
			break;
		}
	}
}

void handlerSigInt(int Sig)
{
	int found = 0, i;

	printf("Liberation des ressources\n");
	for (i = 0; i < 2; ++i)
	{
		if(Tab->pid[i] != 0)
			found++;
	}

	if (found == 2)
	{
		for (i = 0; i < 2; ++i)
		{
			if(Tab->pid[i] == getpid())
				Tab->pid[i] = 0;
		}
	}
	else
	{
		msgctl(idMsg,IPC_RMID, NULL);
		shmctl(idSM, IPC_RMID, 0);
		semctl(idSemSM, 0, IPC_RMID, 0);
	}

	exit(0);
}

void handlerSigChld(int Sig)
{
	int status = 0;

	printf("Un de mes fils s'est termine.\n");
	wait(&status);
}

void Requete_NewWindow(MESSAGE M)
{
	int i;

	printf("Une fenetre vient de se connecter. (PID:%d) \n", M.idPid);
	for(i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == 0)
		{
			Tab->User[i].Pid = M.idPid;
			i = 5;
			Tab->nbrFen++;
		}
	}
	AfficheTab();
}

void Requete_Login(MESSAGE M)
{
	int i, uExist, hd;
	MESSAGE NewUser;
	UTILISATEUR UserBuff;

	uExist = 0;

	// Recherche si le client est deja connecte ou non
	for(i = 0 ; i < 5 ; i++)
	{
		if(strcmp(Tab->User[i].NomUtilisateur, M.Donnee) == 0)
		{
			uExist = 1;
		}
	}
	// Si client n'est pas connecte on va alors le chercher dans le fichier user
	if(uExist == 0)
	{
		if ((hd = open(USERS,O_RDONLY)) == -1)
		{ 
			perror("Erreur de FIOuverture()");
			exit(1);
		}

		while(read(hd, &UserBuff, sizeof(UTILISATEUR)) && uExist == 0)
		{
			if(strcmp(UserBuff.NomUtilisateur, M.Donnee) == 0)
			{
				for(i = 0 ; i < 5 ; i++)
				{
					if(Tab->User[i].Pid == M.idPid)
					{
						uExist = 1;
						strcpy(Tab->User[i].NomUtilisateur, M.Donnee);
						i = 5;
					}
				}
			}
		}
		close(hd);

		if(uExist == 0)
		{
			printf("Utilisateur non trouve\n");

			NewUser.Requete = ENVOYER;
			NewUser.Type = M.idPid;
			strcpy(NewUser.Donnee, "Utilisateur non trouve\n");

			if (msgsnd(idMsg, &NewUser, sizeof(MESSAGE) - sizeof(long), 0) == -1)
			{
				perror("Erreur:");
				exit(0);
			}
			kill(NewUser.Type, SIGUSR1);
		}
		else
		{
			printf("Utilisateur trouve\n");

			// On va envoyer ces informations pour que le client puisse bloquer ses champs NomLogin etc.
			NewUser.Requete = ENVOYER;
			NewUser.Type = M.idPid;
			strcpy(NewUser.Donnee, "Connexion\n");

			if (msgsnd(idMsg, &NewUser, sizeof(MESSAGE) - sizeof(long), 0) == -1)
			{
				perror("Erreur:");
				exit(0);
			}
			kill(NewUser.Type, SIGUSR1);

			//On prépare déjà la nouvelle structure pour pouvoir envoyer au user qui vient de se co,  tous les 
			//autres users déjà connecté.
			NewUser.Type = M.idPid;
			NewUser.Requete = LOGIN;

			// On informe tous les autres clients que quelqu'un vient de se connecter
			for( i = 0 ; i < 5 ; i++)
			{
				if(Tab->User[i].Pid != M.idPid && Tab->User[i].Pid != 0 && strcmp(Tab->User[i].NomUtilisateur, "") != 0)
				{
					M.Type = Tab->User[i].Pid;

					if (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
					{
						perror("Erreur:");
						exit(0);
					}

					// On en profite pour envoyer l'utilisateur trouvé au user qui vient de se log
					strcpy(NewUser.Donnee, Tab->User[i].NomUtilisateur);
					if (msgsnd(idMsg, &NewUser, sizeof(MESSAGE) - sizeof(long), 0) == -1)
					{
						perror("Erreur:");
						exit(0);
					}

					kill(M.Type, SIGUSR1);
					kill(NewUser.Type, SIGUSR1);
				}
			}	
		}
		

	}
	else
	{
		printf("Utilisateur deja existant\n");

		NewUser.Requete = ENVOYER;
		NewUser.Type = M.idPid;
		strcpy(NewUser.Donnee, "Utilisateur deja existant\n");

		if (msgsnd(idMsg, &NewUser, sizeof(MESSAGE) - sizeof(long), 0) == -1)
		{
			perror("Erreur:");
			exit(0);
		}

		kill(NewUser.Type, SIGUSR1);
	}

	AfficheTab();
}

void Requete_EndWindow(MESSAGE M)
{
	int i, j;

	printf("Une fenetre vient de se deconnecter. (PID:%d) \n", M.idPid);
	for(i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == M.idPid)
		{
			Tab->User[i].Pid = 0;
			strcpy(Tab->User[i].NomUtilisateur, "");

			for(j = 0 ; j < 5 ; j++)
				Tab->User[i].Autre[j] = 0;

			i = 5;
			Tab->nbrFen--;
		}
	}
	AfficheTab();
}

void Requete_Terminer(MESSAGE M)
{
	int i, j;

	printf("Un utilisateur vient de se deconnecter (USER: %s) \n", M.Donnee);

	for(i = 0 ; i < 5 ; i++)
	{
		for(j = 0 ; j < 5 ; j++)
		{
			if(Tab->User[i].Autre[j] == M.idPid)
				Tab->User[i].Autre[j] = 0;
		}

		if(Tab->User[i].Pid == M.idPid)
		{
			strcpy(Tab->User[i].NomUtilisateur, "");
			
			for(j = 0 ; j < 5 ; j++)
				Tab->User[i].Autre[j] = 0;
		}
	}
	// On prévient tous les users connectés que le user n'est plus là
	M.idPid = getpid();

	for (i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid != 0 && strcmp(Tab->User[i].NomUtilisateur, "") != 0 )
		{
			M.Type = Tab->User[i].Pid;

			if (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
			{
				perror("Erreur:");
				exit(0);
			}

			kill(M.Type, SIGUSR1);					
		}
	}
	AfficheTab();
}

void Requete_Rechercher(MESSAGE M)
{
	int i, TypeRecherche, connected;
	pid_t idFils;

	char TSearch[3];
	char Pid[10];

	connected = 0;
	printf("pid : %d, type : %d, message : %s\n", M.idPid, M.Type, M.Donnee);

	// Verification si le user est connecte ou non
	for (i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == M.idPid)
		{
			if (strcmp(Tab->User[i].NomUtilisateur, "") != 0 )
			{
				connected = 1;
				if(strcmp(Tab->User[i].NomUtilisateur, M.Donnee) == 0)
					TypeRecherche = SEARCH_HIMSELF;
				else
					TypeRecherche = SEARCH_SOME1ELSE;

				i = 5;
			}
		}	
	}

	if(connected == 1)
	{
		sprintf(TSearch, "%d", TypeRecherche);
		sprintf(Pid, "%d", M.idPid);

		if(TypeRecherche == SEARCH_HIMSELF)
			printf("C'est un SEARCH_HIMSELF\n");
		if(TypeRecherche == SEARCH_SOME1ELSE)
			printf("C'est un SEARCH_SOME1ELSE\n");

		if ((idFils = fork()) == -1)
		{
			perror("Erreur creation fils:");
			exit(1);
		}

		if (!idFils)
		{
			// Code du Fils
			if(execl("./Recherche", "Recherche", TSearch, Pid, M.Donnee, NULL) == -1 )
			{
				perror("Erreur creation du fils: ");
				exit(1);
			}
			printf("Creation du fils recherche\n");
		}
	}
	else
	{
		printf("Impossible d'effectuer une recherche, le client n'est pas connecte. (PID: %d)\n", M.idPid);
	}
}

void Requete_Envoyer(MESSAGE M)
{
	int i, j, uExist;
	char	BuffNomUtilisateur[20], BuffDonnee[255];

	//Verification si le client est loggé
	uExist = 0;
	for(i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == M.idPid)
		{	
			if(strcmp(Tab->User[i].NomUtilisateur, "") != 0)
			{
				uExist = 1;
				strcpy(BuffNomUtilisateur, Tab->User[i].NomUtilisateur);
			}
		}
	}
	if (uExist == 1)
	{
		sprintf(BuffDonnee, "(%s) %s \n",BuffNomUtilisateur, M.Donnee);
		strcpy(M.Donnee, BuffDonnee);

		// Envoi du message au client que le user a accepter
		for(i = 0 ; i < 5 ; i++)
		{
			if(Tab->User[i].Pid == M.idPid)
			{
				M.idPid = getpid();

				for(j = 0 ; j < 5 ; j++)
				{
					if(Tab->User[i].Autre[j] != 0)
					{
						M.Type = Tab->User[i].Autre[j];

						msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0);
						kill(M.Type, SIGUSR1);
					}
				}

				i = 5; // Forçage de boucle.
			}
		}
	}
	//Il n'est pas connecte!
	else
	{
		printf("Impossible d'envoyer le message, pas connecte. (PID:%d) \n", M.idPid);

		M.Type = M.idPid;
		M.idPid = getpid();
		strcpy(M.Donnee, "Vous n'etes pas actuellement connecte \n");

		msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0);
		kill(M.Type, SIGUSR1);
	}
}

void Requete_Accepter(MESSAGE M)
{
	int i, j, k;

	for(i = 0 ; i < 5 ; i++)
	{
		if(strcmp(Tab->User[i].NomUtilisateur, M.Donnee) == 0)
		{
			k = Tab->User[i].Pid; // k est le pid du client accepté
			i = 5;
		}
	}
	for(i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == M.idPid)
		{
			for( j = 0 ; j < 5 ; j++)
			{
				if(Tab->User[i].Autre[j] == 0)
				{
					Tab->User[i].Autre[j] = k;
					j = 5;
				}
			}
			i = 5;
		}
	}
	AfficheTab();
}

void Requete_Refuser(MESSAGE M)
{
	int i, j, k;

	for(i = 0 ; i < 5 ; i++)
	{
		if(strcmp(Tab->User[i].NomUtilisateur, M.Donnee) == 0)
		{
			k = Tab->User[i].Pid; // k est le pid du client accepté
			i = 5;
		}
	}
	for(i = 0 ; i < 5 ; i++)
	{
		if(Tab->User[i].Pid == M.idPid)
		{
			for( j = 0 ; j < 5 ; j++)
			{
				if(Tab->User[i].Autre[j] == k)
				{
					Tab->User[i].Autre[j] = 0;
					j = 5;
				}
			}
			i = 5;
		}
	}
	AfficheTab();
}

void AfficheTab()
{
  int i = 0;
  printf("-------------------------TABWINDOW--------------------------\n");
  while (i < 5) 
     { printf("%5d --%-10s-- %5d %5d %5d %5d %5d\n",
           Tab->User[i].Pid,Tab->User[i].NomUtilisateur,
           Tab->User[i].Autre[0],Tab->User[i].Autre[1],Tab->User[i].Autre[2],
           Tab->User[i].Autre[3],Tab->User[i].Autre[4]);
       i++;
     }

  printf("------------------------------------------------------------\n");
  return ;
}

void sem_wait(int idSem) 
{
    struct sembuf semBuffLoc;
    semBuffLoc.sem_num = 0;
    semBuffLoc.sem_flg = 0;
    semBuffLoc.sem_op = -1;
    semop(idSem, &semBuffLoc, 1);
}
void sem_signal(int idSem) 
{
    struct sembuf semBuffLoc;
    semBuffLoc.sem_num = 0;
    semBuffLoc.sem_flg = 0;
    semBuffLoc.sem_op = 1;
    semop(idSem, &semBuffLoc, 1);
}
