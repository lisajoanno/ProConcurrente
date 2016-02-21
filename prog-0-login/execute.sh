# Lance l'execution du simulateur de diffusion de la chaleur

# Options disponibles : 
#  -i : nombre d'iterations a executer
#  -e [012345] : etape du programme a executer
#  -s [0123456789] : taille du probleme e executer
#  -t [012345] : nombre de thread à creer
#  -m : mesure et affichage du temps d'execution (consommation du CPU) suppression de toute trace d’execution
#  -M : mesure et affichage du temps d'execution (temps de reponse utilisateur), suppression de toute trace d’execution
#  -a : affichage de la température initiale et de la température finale pour les indices correspondant au quart supérieur gauche du tableau, pour les indices modulo 2s

# Par defaut 
./main -i 10000 -e 012345 -s 024 -t 13 -m

