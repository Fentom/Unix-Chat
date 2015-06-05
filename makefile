.SILENT:

EXEC=DossierFinal2014 Serveur CreeFichierUtilisateur CreeFichierPub Recherche Administration AffPub
CC=g++ -c -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -I/usr/local/qt/mkspecs/default -I. -I. -I/usr/local/qt/include

OBJ=Client.o Main.o moc_Client.o Serveur.o

all: $(EXEC)

DossierFinal2014:	$(OBJ)
					echo Creation de DossierFinal2014
					g++ -Wl,-R,/usr/local/qt/lib -o DossierFinal2014 Main.o Client.o moc_Client.o -L/usr/local/qt/lib -L/usr/openwin/lib -lqt

Serveur:	Serveur.cpp
			echo Creation de Serveur
			g++ -o Serveur Serveur.cpp

CreeFichierUtilisateur:	CreeFichierUtilisateur.cpp
						echo Creation de CreeFichierUtilisateur
						g++ -o CreeFichierUtilisateur CreeFichierUtilisateur.cpp

CreeFichierPub:	CreeFichierPub.cpp
				echo Creation de CreeFichierPub
				g++ -o CreeFichierPub CreeFichierPub.cpp

Recherche:	Recherche.cpp
			echo Creation de Recherche
			g++ -o Recherche Recherche.cpp

Administration:	Administration.cpp
				echo Creation de Administration
				g++ -o Administration Administration.cpp

AffPub:	AffPub.cpp
		echo Creation de AffPub
		g++ -o AffPub AffPub.cpp

Client.o:	Client.cpp Client.h
			echo Creation de Client.o
			$(CC) -o Client.o Client.cpp

Main.o:		Main.cpp
			echo Creation de Main.o
			$(CC) -o Main.o Main.cpp

moc_Client.o:	moc_Client.cpp
				echo Creation de moc_Client.o
				$(CC) -o moc_Client.o moc_Client.cpp
