# Librairie Lumiere_JourNuit pour Arduino

Lumiere_JourNuit est une librairie Arduino qui permet de gérer simplement l'allumage et l'arrêt de certaines lumières en fonction de l'heure du jour d'une horloge simulée. Ces lumières, y compris la lumière du jour, sont utilisées pour une scène vivante par exemple pour le modélisme ferroviaire. Vous pouvez utiliser une horloge qui va décompter les minutes assez rapidement afin de simuler une journée en quelques minutes.

## Description

Cette librairie fourni plusieurs fonctions qui permettent d'avoir :

* Une liste de tâches, pour allumer ou éteindre des lumières, à une heure donnée.
* Une liste d'horaires pour allumer puis éteindre progressivement une lumière, ou un bandeau de lumières, afin de simuler l'éclairage habituellement fourni par le soleil pendant la journée.
* Une horloge qui va avancer d'une minute à un pas donné.
* La transmission via un bus RS485 de l'heure, en minutes, d'un Arduino à un autre, afin de pouvoir synchroniser plusieurs scènes. Chaque scène est pilotée par son Arduino et avec ces tâches et horaires de soleil dédiés.

## Résumé

La librairie peut être installée en récupérant une copie du projet git ou en téléchargeant le [fichier zip](http://www.martin-thierry.nom.fr/public/Lumiere_JourNuit-1_0.zip) puis en le dézippant dans le répertoire Arduino/librairie de votre IDE.

Après avoir installé cette librairie dans votre IDE Arduino, vous pouvez l'utiliser de façon simple, dans votre programme, en commençant par les étapes suivantes :

1. Importer la librairie dans le code du programme :

```
#include <Lumiere_JourNuit.h>
```

2. Déclarer un tableau d'horaires et compter le nombre de lignes qu'il contient, par exemple :

```
const struct parametres_activite liste_activites [] = {     
  {HM( 5,  0), 4, HIGH},
  {HM( 6, 30), 4, LOW},
  {HM(20, 10), 4, HIGH},
  {HM(23, 12), 4, LOW},

  {HM( 9, 10), 5, HIGH},
  {HM(13,  0), 5, LOW},
  {HM(14, 10), 5, HIGH},
  {HM(19,  0), 5, LOW}
};

const size_t nb_activites = sizeof (liste_activites) / sizeof (struct parametres_activite);
```

3. Déclarer l'objet lumiere_jour_nuit de classe LumiereJourNuit qui permet d'effectuer les tâches principales, par exemple :

```
LumiereJourNuit lumiere_jour_nuit (liste_activites, nb_activites, NULL, 0, 255);
```

4. Initialiser les entrées/sorties et l'horloge simulée, typiquement dans la fonction `setup()` avec le bloc de code suivant :

```
  const uint16_t delai_boucle = 100; // 100 ms = 0,1 seconde => 1 minute de la journée simulée, 2 minutes et 24 secondes => 1 journée simulée
                                     // 1000 ms = 1 seconde => 1 minute de la journée simulée, 24 minutes => 1 journée simulée

  lumiere_jour_nuit.begin (delai_boucle
```

5. Boucler sur les minutes et l'exécution des taches, typiquement dans la fonction `loop()` avec le bloc de code suivant :

```
  lumiere_jour_nuit.loop ();
  lumiere_jour_nuit.next ();
```

Ainsi, la librairie va allumer et éteindre régulièrement les led connectées aux sorties D4 et D5 de votre Arduino.

Lors du démarrage de l'Arduino, ou après téléversement du programme, l'horloge simulée commence à 0h.

## Auteur et licence

Cette librairie est distribuée gratuitement et sans aucune garantie, sous licence LGPL version 2.1 ou supérieure.

Cette librairie a été écrite par Thierry MARTIN, e-mail [lumiere_journuit@martin-thierry.nom.fr](mailto:lumiere_journuit@martin-thierry.nom.fr).

Si vous distribuez ou adaptez la librairie, vous devez laisser mon nom et conserver cette licence.

## Explications détaillées

### Initialisation de la planification

Le tableau `liste_activites` une liste de tâches. Chaque tâche est saisie dans une ligne du tableau, et contient les informations suivantes : l'horaire, la borne à laquelle la led est branchée et l'état allumé (HIGH) ou éteint (LOW). Le premier paramètre, l'horaire, doit être indiqué en minutes depuis minuit, afin de simplifier la saisie la macro HM permet d'indiquer l'heure et les minutes. Le second paramètre indique le numéro de la borne à laquelle la led est branchée, pour un Arduino UNO ou Nano vous indiquez directement le numéro, pour d'autres comme les ESP vous utilisez le nom comme D4 ou D5.

Le tableau doit contenir une ligne par horaire. La led sera allumée entre l'horaire portant l'état HIGH jusqu'à la minute précédent l'horaire avec l'état LOW. Il est possible d'allumer ou d'éteindre plusieurs led à la même minute. Il est possible d'allumer et d'éteindre la même led autant de fois que vous le souhaitez. La seule limitation à la longueur de la planification est la mémoire du Arduino.

Pour le cas particulier où vous souhaitez laisser allumée une led autour de minuit, par exemple entre 23h et 1h, il faudra allumer la led à 0h00, l'éteindre à 1h00 puis l'allumer à 23h00. Le fait d'allumer la led à 0h00 permet qu'elle soit allumée lors du démarrage de l'Arduino.

### Création de l'objet lumiere_jour_nuit

La création (instentation) de l'objet `lumiere_jour_nuit` nécessite 5 paramètres. Dans le premier exemple, seuls les 2 premiers sont réellement utilisés. Les 3 derniers sont détaillés plus loin pour la simulation du soleil.

Le premier paramètre est le tableau `liste_activites` et le second son nombre de lignes. La ligne de code commençant par `const site_t nb_activites` n'a pas besoin d'être modifiée, car elle permet de calculer le nombre de lignes. Le nombre de lignes est obtenu en divisant la taille totale utilisée par le tableau par la taille d'une ligne d'horaire.

### Démarrage de l'objet lumiere_jour_nuit

La ligne `lumiere_jour_nuit.begin(...)` permet d'initialiser l'objet. Il nécessite en paramètre la durée de la minute. Dans l'exemple la durée de 100 ms permet de simuler une journée en un peu plus de 2 minutes et 24 secondes. À titre d'exemple la valeur de 1000 ms est indiquée en commentaire et correspond à une durée de journée simulée de 24 minutes. Vous êtes libre d'adapter cette durée à vos besoins.

### Boucle de traitement

La boucle est composée de l'appel à deux fonctions qui ne nécessitent aucun paramètre :

1. `lumiere_jour_nuit.loop()` : Qui, en fonction de l'heure, va lire la planification afin d'allumer ou éteindre les led demandées.
2. `lumiere_jour_nuit.next()` : Qui fait une pause du délai indiqué lors du démarrage et incrémente l'heure simulée.

La décomposition du traitement en deux parties permet d'insérer d'autres fonctions comme décrit ci-après.

## Simulation du soleil

En plus de la déclaration de la liste des activités, décrite ci-dessus, il est possible de simuler l'éclairage du soleil au moyen d'un bandeau de led. Ces led vont s'allumer progressivement, rester au maximum pendant la journée puis la lumière décroît et enfin s'éteint pour la nuit.

Pour cela vous allez définir une seconde planification pour indiquer les heures de changement de luminosité et la luminosité atteinte à chacune de ces heures. Afin de simplifier le planning, la luminosité est indiqué en pourcentage, la valeur 100 permettant d'allumer les led au maximum. Le programme va calculer la progression à faire, minute par minute, entre les deux horaires consécutifs, afin d'ajuster l'intensité de la lumière des led qui simulent le soleil. Il est important d'indiquer les horaires dans l'ordre, sinon le calcul ne va pas fonctionner.

Par exemple la planification peut être définie comme suit :

```
const struct parametres_soleil liste_parametres_soleil [] = {
  {HM( 0, 0),   0},
  {HM( 7, 0),   0},
  {HM( 8, 0),  15},
  {HM( 9, 0),  85},
  {HM(13, 0), 100},
  {HM(17, 0),  85},
  {HM(18, 0),  15},
  {HM(19, 0),   0}};
```

Ici encore, vous indiquez qu'au démarrage du Arduino, à 0h, vous souhaitez éteindre le soleil. Ensuite le soleil va rester éteint jusqu'à 7h, puis s'allumer progressivement jusqu'à 15% à 8h, puis 85% à 9h, puis 100% à 13h. À partir de 13h il va décroitre jusqu'à 85% à 17h, puis 15% à 18h et s'éteindre complètement à 19h. De 19h à 7h il va rester éteint. Vous pouvez modifier ces horaires, mettre deux lignes à 100% afin d'avoir un soleil à son zénith pendant une certaine durée. À vous de tester afin de définir les horaires et pourcentages qui correspondent à la façon dont vous souhaitez que le soleil s'allume.

Cette liste de planification doit être ajoutée après celle d'allumage des led et être suivi du comptage du nombre de ligne de la façon suivante :

```
const size_t nb_horaires_soleil = sizeof (liste_parametres_soleil) / sizeof (struct parametres_soleil);
```

Vous pouvez définir une constante avec le numéro de la borne de l'Arduino utilisée pour connecter le bandeau de led. Il faut utiliser une sortie Arduino pouvant fonctionner en PWM afin que la lumière puisse varier. Le code est par exemple le suivant :

```
const uint8_t pin_soleil = 3; // Nécessite PWM
```

Enfin lors de la déclaration de l'objet `lumiere_jour_nuit` vous utilisez les trois derniers paramètres pour transmettre cette planification. Le troisième paramètre est le tableau avec la planification du soleil, le quatrième le nombre de lignes dans ce tableau et le cinquième le numéro de la borne utilisée pour allumer le soleil. Par exemple vous pouvez utiliser le code suivant :

```
LumiereJourNuit lumiere_jour_nuit (liste_activites, nb_activites, liste_parametres_soleil, nb_horaires_soleil, pin_soleil);
```

Il n'y a rien d'autre à modifier, les fonctions begin et loop vont exécuter le code correspondant à la planification du soleil. La fonction next n'est pas impactée par la présence ou l'absence d'un soleil.

## Ajout d'autres fonctions

Si vous souhaitez ajouter d'autres actions qui dépendent de l'heure et qui ne sont ni le soleil, ni l'allumage ni l'arrêt d'une borne de l'Arduino, vous pouvez ajouter dans la fonction loop du code entre les appels à `lumiere_jour_nuit.loop()` et `lumiere_jour_nuit.next()`.

L'appel à la fonction `lumiere_jour_nuit.get_heure_simulee()` retourne un entier de type `horaire` qui contient le nombre de minutes depuis minuit. Vous pouvez le comparer à un `HM(heure, minute)` pour déclencher des actions ou réaliser un calcul. Par exemple, vous pouvez faire avancer un moteur pas à pas pendant un certain temps afin de déplacer un élément sur la scène.

## Horloge maître/esclave

### Principe

Ce paragraphe décrit comment vous pouvez assembler deux scènes indépendantes et synchroniser la planification de leur éclairage. Chacune de ces scènes peut fonctionner indépendamment et contient son Arduino avec une planification de tâches dédiée. Lorsque les deux scènes (ou plus) sont utilisées ensemble, vous souhaitez certainement que leur horaire soit synchronisé, afin que les allumages et extinctions de lumières et du soleil soient cohérents.

Par contre, le soleil de chaque scène reste indépendant. Si, par exemple, une scène simule un soleil d'hiver et l'autre d'été, alors l'horaire de lever et coucher du soleil sera différent. Mais le décalage reste constant pendant toute la durée de l'illumination.

J'ai choisi un bus RS485 car, il permet :

* De connecter un nombre quelconque d'Arduino.
* D'avoir la même connexion pour le maître et l'esclave.
* De utiliser que deux fils.
* D'isoler électriquement les Arduino.
* De plus, pour le modélisme ferroviaire, ce bus est aussi utilisé pour connecter une multimaus à une centrale DCC.

### Détail du code

L'objet `lumiere_jour_nuit` doit être créé à partir de la classe `LumiereJourNuitHorlogeTransmise` au lieu de la classe `LumiereJourNuit` utilisée précédement. Trois nouveaux paramètres permettent d'indiquer les connexions de l'Arduino utilisées pour cummuniquer avec la carte MAX_485 (voir le schéma de montage lié à l'exemple lumiere_jour_nuit_synchronisation. Le code est par exemple le suivant :

```
const uint8_t pin_rs485_dir = 18;
const uint8_t pin_rs485_rx  =  2; // Nécessite une interruption
const uint8_t pin_rs485_tx  = 19;

LumiereJourNuitHorlogeTransmise lumiere_jour_nuit (liste_activites, nb_activites,
                                                   liste_parametres_soleil, nb_horaires_soleil, pin_soleil,
                                                   pin_rs485_rx, pin_rs485_tx, pin_rs485_dir);
```

Dans notre montage, la connexion aux MAX_485, ne nécessite que 3 fils, car les deux bornes de direction (RI et DO) sont reliées ensemble. La borne reliée à RX nécessite une interruption.

L'initialisation de l'objet `lumiere_jour_nuit` nécessite un paramètre supplémentaire, qui permet d'indiquer si on force l'Arduino comme maître, ou si on laisse le premier Arduino démarrer prendre ce rôle. Le code est par exemple le suivant pour un Arduino avec détection automatique du maître :

```
  lumiere_jour_nuit.begin (delai_boucle, false);
```

Le reste du code ne change pas.

## Branchement des led

Lors l'éclairage d'une partie de la scène ne nécessite qu'une led par planification, il est possible de relier directement la led à l'Arduino. Il faut ajouter une résistance pour adapter la tension. Par contre pour l'allumage de plusieurs led ou d'un bandeau de led, comme pour le soleil, il est préférable d'utiliser un montage avec un transistor ou un relai et une alimentation extérieur dimensionnée en conséquence.

Pour le branchement des led directement sur l'Arduino avec une résistance, vous pouvez consulter les exemples de la librairie qui sont accompagnés d'un schéma de montage sur plaque de test.

Par exemple, pour allumer un bandeau de led fonctionnant en 12V, j'ai trouvé un [tutoriel](http://nagashur.com/blog/2015/08/10/controleur-de-led-simple-utilisant-un-tip120/). La solution peut facilement être transposée à d'autres tensions, le principe étant d'utiliser un transistor afin d'amplifier la sortie de l'Arduino, soit en intensité soit en tension, ce qui permet de consommer plus de 5V/20mA.

## Exemples

Cette librairie est fournie avec 4 exemples. Chaque exemple est accompagné d'un schéma de test.

### lumiere_jour_nuit_simple

Il s'agit d'allumer et éteindre deux leds, à certaines heures et sur un cycle de 24h. Il ne comprend que le tableau parametres_activite et le code d'appel de la librairie, comme décrit dans le Résumé.

### lumiere_jour_nuit_soleil

Dans cet exemple, on ajoute en plus le soleil. Il comprend en plus le tableau parametres_soleil, comme décrit dans le paragraphe Simulation du soleil.

### lumiere_jour_nuit_synchronisation

Dans cet exemple, on ajoute la synchronisation entre plusieurs Arduino. La classe LumiereJourNuit et remplacée par LumiereJourNuitHorlogeTransmise.

### lumiere_jour_nuit_generique

Cet exemple commence par plusieurs macro qui peuvent être mises en commentaire. Selon que les macros sont définies ou non, cela permet d'utiliser une partie du code et d'obtenir le même résultat que l'un des trois exemples précédents. Le but est d'avoir une version plus souple pour passer d'un cas à l'autre.

## Limites

Lorsque vous utilisez la fonction de synchronisation de plusieurs Arduino, il n'est pas possible :

* De débrancher l'Arduino maître de l'horloge.
* De connecter sur le bus RS485 un Arduino qui est déjà démarré. Sinon il y aurait plusieurs maîtres de l'horloge et la librairie ne contient pas de mécanisme pour régler ce conflit.

Le pas d'avancement de l'horloge est une pause entre deux boucles. Le délai n'est pas précis et dépend des tâches réalisées par la boucle.

Si besoin n'hésitez pas à me contacter à [lumiere_journuit@martin-thierry.nom.fr](mailto:lumiere_journuit@martin-thierry.nom.fr).
