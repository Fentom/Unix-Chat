#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
   { char	NomUtilisateur[20];
     char	Gsm[20];
   } UTILISATEUR;

UTILISATEUR	Donnee[] = 
   { {"aaa","111"},
     {"wagner","222"},
     {"ccc","333"},
     {"ddd","444"},
     {"MerceD","555"},
     {"fff","666"},
     {"ggg","777"},
     {"hhh","888"},
     {"iii","999"},
     {"jjj","100"}
   };

int main()
{
int	hd;
if ((hd = open("Utilisateur.dat",O_RDWR | O_CREAT | O_EXCL,0644)) == -1)
   { perror("Err. de creation du fichier");
     exit(1);
   }

int	i = 0;
while (i < 10)
   { if (write(hd,&Donnee[i],sizeof(UTILISATEUR)) != sizeof(UTILISATEUR))
      { perror("Err. de write");
        exit(1);
      }
     i++;
   }
close(hd);
exit(0);
}
