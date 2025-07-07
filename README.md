ft_irc
📚 Description

ft_irc est un serveur IRC (Internet Relay Chat) implémenté en C++98, permettant la communication instantanée via canaux et messages privés, reproduisant le comportement d’un serveur IRC conforme aux standards pour interopérabilité avec un client IRC réel.
🚀 Fonctionnalités

✅ Gestion de multiples clients simultanés sans blocage
✅ Authentification par mot de passe, définition de nickname et username
✅ Création et gestion de channels IRC
✅ Envoi et réception de messages privés
✅ Transmission des messages à tous les clients dans un channel
✅ Gestion des opérateurs et utilisateurs basiques
✅ Commandes opérateurs implémentées :

    KICK : Éjecter un client du channel

    INVITE : Inviter un client

    TOPIC : Modifier/afficher le thème

    MODE : Gérer modes i, t, k, o, l (invitation only, topic restriction, key, operator privilege, user limit)

✅ Gestion des entrées/sorties non bloquantes via poll()
✅ Gestion TCP/IP (IPv4 et IPv6)
⚙️ Compilation

Un Makefile est fourni avec les règles :

    $(NAME), all, clean, fclean, re

Compilez avec :Compilez avec :

```make```

🖥️ Utilisation

Lancez le serveur avec :

./ircserv <port> <password>

    port : Port sur lequel le serveur écoute (ex : 6667)

    password : Mot de passe requis pour l’authentification des clients

🧪 Test

Pour tester manuellement :

nc 127.0.0.1 6667

Utilisez Ctrl+D pour envoyer une commande fragmentée en plusieurs parties pour vérifier la reconstruction correcte côté serveur.

Nous recommandons également d’utiliser un client IRC comme référence (HexChat, irssi, weechat) pour valider la connexion et le fonctionnement des commandes IRC standards.
🛠️ Prérequis Techniques

    C++98 uniquement

    Pas de forking

    Gestion non bloquante des FD

    Une seule boucle poll() ou équivalent pour l’ensemble des opérations E/S

    Code propre, conforme à la norme

    Aucune bibliothèque externe ou Boost autorisée

🗂️ Structure du projet

    src/ : Contient le code source .cpp

    includes/ : Contient les headers .h/.hpp

    Makefile : Pour compiler facilement le projet

    ircserv : Binaire généré après compilation
