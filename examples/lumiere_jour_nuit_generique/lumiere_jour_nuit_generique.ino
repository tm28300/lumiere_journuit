/*
 Simule l'horaire d'une journée pour :
  * Allumer une ligne de led pour simuler l'éclérage du soleil (optionnel).
    La lumière du soleil croit le matin puis décroît le soir et s'éteint la nuit.
  * Allumer et éteindre des led en fonction en fonction de l'heure.
    Au lieu de led on peut activer n'importe quel dispositif, pourvu qu'il nécessite simplement un état haut pour fonctionner et un état bas pour s'arrêter.
  * L'horaire peut être transmis d'un Arduino maître à des esclaves via une liaison RS485.
 Connexions :
  * Par défaut pin 3 pour le soleil.
  * Par défaut deux leds, pin 4 et 5.
  * Si on utilise comme maître un Arduino Nano, ou un autre Arduino qui n'a qu'une seule liaison série hardware, le RS485 utilise une liaison série software.
    Par défaut pin 18 pour la direction, 2 pour la réception et 19 pour l'émission.

 Written by Thierry MARTIN
 LGPL License, all text above must be included in any redistribution
*/

#define PRESENCE_SOLEIL
#define HORLOGE_MAITRE_ESCLAVE

#ifdef HORLOGE_MAITRE_ESCLAVE
#include <Lumiere_JourNuit_TransmissionHorloge.h>
#else
#include <Lumiere_JourNuit.h>
#endif

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

#ifdef PRESENCE_SOLEIL
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
#else
const uint8_t pin_soleil = 255; // Pas de soleil
const struct parametres_soleil* liste_parametres_soleil = NULL;
const size_t nb_horaires_soleil = 0;
#endif

#ifdef HORLOGE_MAITRE_ESCLAVE
const uint8_t pin_rs485_dir = 18;
const uint8_t pin_rs485_rx  =  2; // Nécessite une interruption
const uint8_t pin_rs485_tx  = 19;

LumiereJourNuitHorlogeTransmise
#else
LumiereJourNuit
#endif
lumiere_jour_nuit (liste_activites, nb_activites, liste_parametres_soleil, nb_horaires_soleil, pin_soleil
#ifdef HORLOGE_MAITRE_ESCLAVE
, pin_rs485_rx, pin_rs485_tx, pin_rs485_dir
#endif
);

void setup() {
  Serial.begin (115200);
  Serial.println ("Lumière jour/nuit générique 12/01/2024");

  const uint16_t delai_boucle = 100; // 100 ms = 0,1 seconde => 1 minute de la journée simulée, 2 minutes et 24 secondes => 1 journée simulée
                                     // 1000 ms = 1 seconde => 1 minute de la journée simulée, 24 minutes => 1 journée simulée

  lumiere_jour_nuit.begin (delai_boucle
#ifdef HORLOGE_MAITRE_ESCLAVE
  , false
#endif
  );
}

void loop() {
  lumiere_jour_nuit.loop ();
  lumiere_jour_nuit.next ();
}
