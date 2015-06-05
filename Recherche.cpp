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

int idMsg;

int main(int argc, char * argv[])
{
	int TypeRecherche, hd, uExist;
	MESSAGE M;
	UTILISATEUR UserBuff;

	struct flock verrou;

	TypeRecherche = atoi(argv[1]); // Type de recherche demandé
	M.Type = atoi(argv[2]);		   // Le pid du process auquel on doit répondre
	strcpy(M.Donnee, argv[3]);     // Le nom du user à rechercher

	uExist = 0;

	//---------------------------- Creation de la FILE DE MESSAGE -------------------------------------
	if ((idMsg = msgget(KEY,0)) == -1) // On récupére la file de message, si elle existe
	{
		perror("Erreur file de message");
		exit(1);
	} 

	printf("\t(FILS) Paramètre recu : %d, %d, %s\n", TypeRecherche, M.Type, M.Donnee);
	M.idPid = getpid();
	M.Requete = RECHERCHER;

	switch(TypeRecherche)
	{
		case SEARCH_SOME1ELSE:
			if ((hd = open(USERS,O_RDONLY)) == -1)
			{ 
				perror("Erreur de FIOuverture()");
				exit(1);
			}
			while(uExist == 0 && read(hd, &UserBuff, sizeof(UTILISATEUR)))
			{
				if(strcmp(UserBuff.NomUtilisateur, M.Donnee) == 0)
				{
					strcpy(M.Donnee, UserBuff.Gsm);
					uExist = 1;
				}
			}

			if(uExist == 0) // utilisateurs pas trouvé
			{
				strcpy(M.Donnee, "Nom pas trouve");

			    if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
			    { 
			        perror("Erreur:");
			        exit(1);
			    }
			    kill(M.Type, SIGUSR1);
			    close(hd);
			    exit(0);
			}

			if (lseek(hd, -sizeof(UTILISATEUR), SEEK_CUR) == -1)
			{
				perror("Err. de lseek:");
				exit(1);
			}

	        verrou.l_type = F_RDLCK; // on verrouille l'enregistrement en lecture
	        verrou.l_whence = SEEK_CUR;
	        verrou.l_start = 0;
	        verrou.l_len = sizeof(UTILISATEUR);

		    if((fcntl(hd, F_SETLK, &verrou)) == -1)
	    	{
	    		//Le fichier est déjà verrouiller, il est donc en modification.
	    		strcpy(M.Donnee, "Modification");

	    		if (msgsnd (idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
	            {
		            perror("Erreur:");
		            exit(1);
	            }
	            kill(M.Type, SIGUSR1);
	            close(hd);
	    		exit(0);
	    	}

	    	close(hd);
			printf("\t(FILS) Voici le gsm trouvé : %s\n", M.Donnee);

    		if (msgsnd (idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
            {
	            perror("Erreur:");
	            exit(1);
            }
            kill(M.Type, SIGUSR1);
		break;
		case SEARCH_HIMSELF:
			if((hd = open(USERS, O_RDWR)) == -1)
			{
				perror("Erreur de FIOuverture()");
				exit(1);
			}
			while(uExist == 0 && read(hd, &UserBuff, sizeof(UTILISATEUR)))
			{
				if(strcmp(UserBuff.NomUtilisateur, M.Donnee) == 0)
				{
					strcpy(M.Donnee, UserBuff.Gsm);
					uExist = 1;
				}
			}
			printf("\t(FILS) Struct UserBUff (USER:%s)(NEW GSM:%s)\n", UserBuff.NomUtilisateur, UserBuff.Gsm);

			if(uExist == 0) // utilisateurs pas trouvé
			{
				strcpy(M.Donnee, "Nom pas trouve");

			    if( (msgsnd(idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0)) == -1)
			    { 
			        perror("Erreur:");
			        exit(1);
			    }
			    kill(M.Type, SIGUSR1);
			    close(hd);
			    exit(0);
			}

			if (lseek(hd, -sizeof(UTILISATEUR), SEEK_CUR) == -1)
			{
				perror("Err. de lseek:");
				exit(1);
			}

	        verrou.l_type = F_WRLCK; // on verrouille l'enregistrement en ecriture
	        verrou.l_whence = SEEK_CUR;
	        verrou.l_start = 0;
	        verrou.l_len = sizeof(UTILISATEUR);

		    if((fcntl(hd, F_SETLK, &verrou)) == -1)
	    	{
	    		//Le fichier est déjà verrouiller.
	    		strcpy(M.Donnee, "Verrouiller"); // cas impossible puisque seul le user en question peut modifier son prore tel.

	    		if (msgsnd (idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
	            {
		            perror("Erreur:");
		            exit(1);
	            }
	            kill(M.Type, SIGUSR1);
	            close(hd);
	    		exit(0);
	    	}

			printf("\t(FILS) Voici le gsm trouvé : %s\n", M.Donnee);

    		if (msgsnd (idMsg, &M, sizeof(MESSAGE) - sizeof(long), 0) == -1)
            {
	            perror("Erreur:");
	            exit(1);
            }
            kill(M.Type, SIGUSR1);

	        if( msgrcv(idMsg,&M,sizeof(M) - sizeof(long), getpid(), 0) == -1 )
	        {
	            perror("Erreur:");
	            exit(0);
	        }

	        switch(M.Requete)
	        {
	        	case ANNULER:
	        		printf("\t(FILS) On annule la modification!!\n");
	        		close(hd);
	        	break;
	        	case MODIFIER:
	        		strcpy(UserBuff.Gsm, M.Donnee);
	        		printf("\t(FILS) On valide la modification! (USER:%s)(NEW GSM:%s)\n", UserBuff.NomUtilisateur, UserBuff.Gsm);

					if (write(hd, &UserBuff, sizeof(UTILISATEUR)) != sizeof(UTILISATEUR))
					{
						perror("Err. de write");
						exit(1);
					}

	        		close(hd);
	        	break;
	        }
		break;
	}
	
	exit(0);
}
