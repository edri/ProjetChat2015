Directives de compilation pour Linux:

La compilation nécessite un compilateur c++ ainsi qu'une installation de la bibliothèque Qt en version 5 avec les modles supplémentaires gérant SQLite et les Websockets. Sur une distribution basée sur Debian, cela implique les paquets suivants:

g++ make qt5-qmake libqt5core5a libqt5sql5-sqlite libqt5websockets5-dev

La compilation s'effectue dans le dossier src/client respectivement src/server. Il est possible d'ouvrir les fichiers chator.pro et chatorServer.pro qui s'y trouvent avec QtCreator qui va se charger de la compilation ou d'y exécuter les commandes suivantes:

export QT_SELECT="qt5" && qmake && make
