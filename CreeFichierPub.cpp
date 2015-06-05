#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct
{
  char	Pub[255];
} PUB;

PUB	Donnee[] = 
   { {"Vous en avez marre des publicites? Payez 5e et fini les pubs!"},
     {"Il y a toujours moins chere ailleurs! Zonama.com, essayez la difference!"},
     {"Quand il y en a plus, y'en a encore avec Fiusbel, profitez de nos taux avantageux!"},
     {"Pourquoi lire quand on peut ecouter ses livres? livreenligne.com!"},
     {"La référence en matiere de tchat? Evidemment, c'est UnixTchat!"},
     {"Vivez des experiences incroyable avec Lantah-Koh!"},
     {"Trop de pub? N'oubliez pas que vous pouvez vous en debarasser en payant 5e!"},
     {"Pour noel, offrez lui un nouveau sarouel!"},
     {"L'equipe TchatUnix vous souhaite a tous et a toutes, de bonnes fetes!"},
     {"Partez a New York pour 200e avec AirHairs!"}
   };

int main()
{
  int	hd;
  if ((hd = open("Pub.dat",O_RDWR | O_CREAT | O_EXCL,0644)) == -1)
     { perror("Err. de creation du fichier");
       exit(1);
     }

  int	i = 0;
  while (i < 10)
     { if (write(hd,&Donnee[i],sizeof(PUB)) != sizeof(PUB))
        { perror("Err. de write");
          exit(1);
        }
       i++;
     }
  close(hd);
  exit(0);
}
