# Construire le projet
Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de les copiers dans le repertoire `build` avec l'application `bibliotheque`


# Question 1 : C++ Fonctionnalité utilisée

Dans mon code,j’ai utilisé les pointeurs intelligents "unique_ptr" afin d'assuer la gestion des livres et des utilisateurs dans la bibliothèque. On n’a pas vraiment vu cette notion en détail en classe, mais elle m’a beaucoup aidé à comprendre comment gérer la mémoire plus simplement en C++. À la place de devoir créer et supprimer manuellement chaque objet avec new et delete, les "unique_ptr" fait les deux actions automatiquement. Dès qu’un livre ou un utilisateur n’est plus utilisé, la mémoire se libère directement. C’est une facon de faire plus propre et sécuritaire, surtout dans notre cas vu que la bibliothèque contient beaucoup d'objets à manipuler.


# Question 2 : C++ Options de développement possible
Comme autre solution plus adaptée pour la gestion de bibliothèque serait d'utiliser une base de données comme MySQL, au lieu de faire ça sur des fichiers texte.C'est une manière de procéder beaucoup plus intutive et rapide.Par exemple, si on veux faire une recherche ou gérer plusieurs utilisateurs en même temps. De plus, cela permet d'éviter de perdre les données. Le programme 
C++ peut très bien fonctionner en ce connectant à une base de données grâce à une librairie tel que MySQL Connector C++. Les données seront stockées dans une vrai base et manipulées avec des requêtes SQL, comme pour ajouter, chercher ou même emprunter des livres.Je pense que cette solution est beaucoup plus pertinente pour rendre le programme plus professionnel et sécuritaire.




numero DA : 2280930
Nom Prénom : Saad Lina