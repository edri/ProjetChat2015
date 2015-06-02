Directives d'exécution:

Sur Linux, il est nécessaire de disposer des bibliothèques OpenSSL et Qt5, il faut s'assurer de disposer des paquets suivants: libcrypto++ libqt5core5a libqt5gui5 libqt5sql5-sqlite libqt5websockets5. Les dossiers sont bin/client/Linux/ respectivement bin/server/Linux/.

Sur Windows, les bibliothèques partagées (DLL) sont incluses dans le dossier d'exécution, ce qui en augmente sensiblement la taille. Les dossiers sont bin/client/Windows/ respectivement bin/server/Windows/.


Le client peut être exécuté directement depuis son répertoire bin/client/<plateforme>/.

Le serveur peut être exécuté directement depuis son répertoire bin/serveur/<plateforme>/ à condition que le dossier dans lequel il se trouve soit éditable. En effet, le serveur va générer et utiliser une base de données en tant que fichier db.sqlite ainsi qu'écrire des images dans son dossier img/.
Si exécuté depuis un support en lecture seule, il affichera une erreur. Dans ce cas, il est possible de simplement déplacer le dossier bin/serveur/<plateforme>/ à un endroit disponible en écriture et l'exécuter.
