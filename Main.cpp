#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <qapplication.h>

#include "Client.h"

#include "Fichier.ini"
#include <signal.h>

void HNouveauMessage(int);
void HNouvellePub(int);
void handlerSigInt(int);
void handlerSigAlrm(int);

MESSAGE	M;

int 	idMsg;
int idSM;
MEMORY  *Tab;

Client*	F1;

pid_t pidRecherche;

int main(int argc,char* argv[])
{
	// Armement du signal SIGINT pour envoyer une requete ENDWINDOW
	struct sigaction sigact;

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = handlerSigInt;
    sigaction(SIGINT, &sigact, NULL);


    // Armement du signal SIGUSR1
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = HNouveauMessage;
    sigaction(SIGUSR1, &sigact, NULL);

    // Armement du signal SIGUSR2 pour la pub
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = HNouvellePub;
    sigaction(SIGUSR2, &sigact, NULL);

    // Armement du signal SIGALRM
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = handlerSigAlrm;
    sigaction(SIGALRM, &sigact, NULL);

    // Recuperation de la file de message
    if((idMsg = msgget(KEY, 0)) == -1)
    {
        perror("Erreur:");
        exit(1);
    }

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

    if(Tab->nbrFen == 5)
    {
        printf("Nbr de fenetre max atteint.\n");
        exit(1);
    }
    
    // Envoi du pid au serveur pour qu'il nous ajoute dans sa table
	M.Type = 1L;
	M.idPid = getpid();
	M.Requete = NEWWINDOW;
	strcpy(M.Donnee, "NOLOG");

	if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
    { 
        perror("Erreur:");
         exit(1);
    }

    // Ouverture de la fenetre
	QApplication appl(argc,argv);
	F1 = new Client();
	F1->show();
	return appl.exec();
}

void HNouveauMessage(int Sig)
{
    int i;

    printf("RECU?");
    fflush(stdout);

    while(msgrcv (idMsg, &M, sizeof(MESSAGE) - sizeof(long), getpid(), IPC_NOWAIT) != -1)
    {
        printf("RECU\n");
        fflush(stdout);

        switch(M.Requete)
        {
            case ENVOYER:
                F1->setMessage(M.Donnee);

                if(strcmp(M.Donnee, "Connexion\n") == 0)
                {   
                    // cette fonction va rendre des box en mode readonly & va en débloquer d'autre
                    F1->setConnexion();
                }
            break;

            case LOGIN:
                for(i = 0 ; i < 5 ; i++)
                {
                    if(strlen(F1->getPersonne(i)) == 0)
                    {
                        F1->setPersonne(i, M.Donnee);
                        i = 5;
                    }
                }
            break;

            case TERMINER:
                for (int i = 0; i < 5; i++)
                {
                    if(strcmp(F1->getPersonne(i), M.Donnee) == 0)
                    {
                        F1->setPersonne(i, "");
                        F1->setCheckbox(i, false);
                        i = 5;
                    }
                }
            break;

            case RECHERCHER:
                printf("J'ai recu un gsm!! (GSM: %s)\n", M.Donnee);
                pidRecherche = M.idPid;
                F1->setGsm(M.Donnee);
            break;
        }
    }
}

void HNouvellePub(int Sig)
{
    F1->setPublicite(Tab->pub);
}

void handlerSigAlrm(int Sig)
{
    F1->Terminer();
    F1->setMessage("(Raison) Client timed out\n");
}

void handlerSigInt(int Sig)
{
    M.Type = 1L;
    M.idPid = getpid();
    M.Requete = ENDWINDOW;

    if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
    { 
        perror("Erreur:");
         exit(1);
    }

    exit(0);
}
