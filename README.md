# SAE C3 - Socket

Auteurs : Artu Florient | Bonnel Noah | Fourny Nathan  
Date : 24/11/2023

## Présentation

Le sujet de cette SAE consistait principalement en la découvert de la notion de **socket**.  

L'objectif était de réaliser 5 versions différentes du jeu du pendu (en affichage console) avec le langage C. Mais le tout étant en **multijoueur** avec un serveur et un ou plusieurs clients en fonction des version.  

Les features principales des versions sont les suivantes :  

v0 : Un client doit trouver le mot donné par le serveur  
v1 : Deux clients s'affrontent simultanément (relié par le serveur)  
v2 : Un client choisi le mot que le second doit deviner (le client 1 reçoit les informations de la partie)  
v3 : Pareil que la version 2 mais le serveur peut gérer plusieurs parties à la fois  
v4 : Pareil que la version 2 mais le serveur connecte les 2 joueurs, puis les connectes entre eux et s'en va

à travers ces version l'objectif est de découvrir le fonctionnement des sockets sous toutes ses façettes.

## Travail effectués

Durant ces 10 jours de travails, nous sommes parvenus jusqu'à la réalisation de la version 3 !.

Ainsi, nous pouvons faire jouer 2 joueurs entre eux, et notre serveur peut acceuillir plusieurs parties à la fois.

## Démarches effectuées

Pour réaliser ce projet nous nous sommes répartis les tâches :  

Noah Bonnel : Gestion du projet, réalisation des organigrammes, etc...  
Artu Florient : Réalisation de la partie Serveur  
Fourny Nathan : Réalisation de la partie Client  

*Remarque: Ceci était nos rôles principaux mais chacun apportaientt son aide aux autres*

**Fonctionnement de la communication client-serveur**

Afin de rendre notre serveur plus sécurisé et par soucis de performance, les messages que le client et le serveur sont basés sur un système de code que nous avons nous même définis. (cf. Partie Codes).  

Ainsi, dans la version 1 et 2 nos messages entre les deux machines ne font que 10 octets maximum. Pour la version 2 et 3 nous avons décidé d'augmenter à 50 pour permettre le transfère d'information plus conséquente.

Notre volonté était que le client ne recevait jamais directement le mot à trouver, par soucis de sécurité. De ce fait, pour connaître le mot celui-ci était obligé de le chercher par lui même. Concernant les envois des informations. Lorsque le serveur reçoit la lettre que le client a envoyé, il défini si elle est correcte ou fausse. Dans le premier cas il envoie alors la position (en index) du ou des caractères dans le mot à trouver, sinon il incrémente les erreurs du joueur de 1 et le client affiche le pendu.  

## Partie Codes

**Tous les codes utilisés dans la communication client-serveur**

201 - Serveur envoi le nombre de lettre dans le mot  
202 - Client envoi une lettre  
203 - Lettre déjà envoyé par le client  
204 - Le serveur envoi que la lettre est pas le mot    
205 - Le serveur envoi que la lettre n'est pas dans le mot  
206 - Le client envoi un mot  
207 - Le serveur envoi que le mot a été trouvé  
208 - Le serveur envoi que le mot n'a pas été trouvé  
209 - Le serveur envoi que le client a perdu  
210 - Le serveur envoi que le client a gagné  
211 - Le serveur envoi que l'adversaire a perdu  
212 - Le serveur envoi que l'adersaire a gagné  
213 - Le serveur envoi que le client peut jouer  
214 - Le serveur envoi que le client peut choisi le mot à donner à son adversaire  
215 - Le client envoi le mot à donner à son adversaire au serveur  
216 - Le serveur informe que l'adversaire a trouvé le mot  
217 - Le serveur informe que l'adversaire a perdu la partie  
218 - Le serveur informe que l'adversaire a trouvé une lettre  
219 - Le serveur informe que l'adversaire a une tentative ratée  

101 - Erreur: ce n'est pas une lettre  
199 - Critical Error