La compilation nécessite un compilateur c++ ainsi qu'une installation de la bibliothèque OpenSSL et Qt en version 5 avec les modles supplémentaires gérant SQLite et les Websockets.



Directives de compilation pour Linux:
Sur une distribution basée sur Debian, cela implique d'installer les paquets suivants:

g++ make libssl-dev libcrypto++-dev qt5-qmake libqt5core5a libqt5gui5 libqt5sql5-sqlite libqt5websockets5-dev

La compilation s'effectue dans les dossiers src/client respectivement src/server. Il est possible d'ouvrir les fichiers chator.pro et chatorServer.pro qui s'y trouvent avec QtCreator qui va se charger de la compilation ou d'y exécuter les commandes suivantes:

export QT_SELECT="qt5" && qmake && make



Directives de compilation pour Windows:
Les bibliothèques nécessaires sont téléchargeables aux adresses suivantes:

http://slproweb.com/download/Win32OpenSSL-1_0_2a.exe
http://www.qt.io/download/

Une fois ceci fait, l'environnement Qt complet est disponible sur la machine, incluant le compilateur.
La compilation s'effectue dans les dossiers src/client respectivement src/server. Il faut d'ouvrir les fichiers chator.pro et chatorServer.pro qui s'y trouvent avec QtCreator qui va se charger de la compilation.



Les exécutables résultants seront automatiquement placés dans l'arborescence des dossiers bin/ (voir bin/Readme.txt pour plus d'informations pour l'exécution).
