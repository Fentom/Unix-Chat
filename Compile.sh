g++ -c -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -I/usr/local/qt/mkspecs/default -I. -I. -I/usr/local/qt/include -o Main.o Main.cpp
g++ -c -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -I/usr/local/qt/mkspecs/default -I. -I. -I/usr/local/qt/include -o Client.o Client.cpp
g++ -c -Wall -W -O2  -DQT_NO_DEBUG -DQT_SHARED -I/usr/local/qt/mkspecs/default -I. -I. -I/usr/local/qt/include -o moc_Client.o moc_Client.cpp

g++ -Wl,-R,/usr/local/qt/lib -o DossierFinal2014 Main.o Client.o moc_Client.o -L/usr/local/qt/lib -L/usr/openwin/lib -lqt 
g++ -o Serveur Serveur.cpp
g++ -o CreeFichierUtilisateur CreeFichierUtilisateur.cpp
