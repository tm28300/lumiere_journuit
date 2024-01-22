/*
 Simule l'horaire d'une journée pour :
  * Allumer une ligne de led pour simuler l'éclairage du soleil.
    La lumière du soleil croit le matin puis décroît le soir et s'éteint la nuit.
  * Allumer et éteindre des led en fonction en fonction de l'heure.
    Au lieu de led on peut activer n'importe quel dispositif, pourvu qu'il nécessite simplement un état haut pour fonctionner et un état bas pour s'arrêter.
 Connexions :
  * Par défaut pin 3 pour le soleil.
  * Par défaut deux leds, pin 4 et 5.

 Written by Thierry MARTIN
 LGPL License, all text above must be included in any redistribution
*/

#include <Lumiere_JourNuit.h>

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

const uint8_t pin_soleil = 3; // Nécessite PWM

const struct parametres_soleil liste_parametres_soleil [] = {
  {HM( 0, 0),   0},
  {HM( 7, 0),   0},
  {HM( 8, 0),  15},
  {HM( 9, 0),  85},
  {HM(13, 0), 100},
  {HM(17, 0),  85},
  {HM(18, 0),  15},
  {HM(19, 0),   0}};

const size_t nb_horaires_soleil = sizeof (liste_parametres_soleil) / sizeof (struct parametres_soleil);

LumiereJourNuit lumiere_jour_nuit (liste_activites, nb_activites, liste_parametres_soleil, nb_horaires_soleil, pin_soleil);

void setup() {
  Serial.begin (115200);
  Serial.println ("Lumière jour/nuit avec soleil 22/01/2024");

  const uint16_t delai_boucle = 100; // 100 ms = 0,1 seconde => 1 minute de la journée simulée, 2 minutes et 24 secondes => 1 journée simulée
                                     // 1000 ms = 1 seconde => 1 minute de la journée simulée, 24 minutes => 1 journée simulée

  lumiere_jour_nuit.begin (delai_boucle);
}

void loop() {
  lumiere_jour_nuit.loop ();
  lumiere_jour_nuit.next ();
}
