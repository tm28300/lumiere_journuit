//#define DEBUG_SERIE

#include <Arduino.h>

#include "Lumiere_JourNuit.h"

LumiereJourNuit::LumiereJourNuit (const parametres_activite tableau_activite[], size_t nombre_actitive, const parametres_soleil tableau_horaire_soleil[], size_t nombre_horaire_soleil, uint8_t pin_soleil)
  : _nombre_activite (nombre_actitive),
    _nombre_horaire_soleil (nombre_horaire_soleil),
    _pin_soleil (pin_soleil),
    _tableau_activite (tableau_activite),
    _tableau_horaire_soleil (tableau_horaire_soleil)
{
}

void LumiereJourNuit::begin (uint16_t delai_minute) {
  _heure_simulee = 0;
  _delai_minute = delai_minute ? delai_minute : 100;

  // Initialisation des led
  size_t num_activite;
  for (num_activite = 0 ; num_activite < _nombre_activite ; ++num_activite) {
    uint8_t pin_activite = _tableau_activite [num_activite].numero_pin;
    pinMode (pin_activite, OUTPUT);
    digitalWrite (pin_activite, LOW);
  }

  if (_pin_soleil) {
    // Initialisation du soleil
    pinMode (_pin_soleil, OUTPUT);
    analogWrite (_pin_soleil, 0);
  }
}

void LumiereJourNuit::loop () {
#ifdef DEBUG_SERIE
    char tampon_heure [6];
    sprintf (tampon_heure, "%2d:%02d", _heure_simulee / 60, _heure_simulee % 60);
    Serial.println (tampon_heure);
#endif

// Allume ou éteint les lumières selon l'heure
  size_t num_activite;
  for (num_activite = 0 ; num_activite < _nombre_activite ; ++num_activite) {
    if (_tableau_activite [num_activite].heure_debut == _heure_simulee) {
      digitalWrite (_tableau_activite [num_activite].numero_pin, _tableau_activite [num_activite].valeur_led);
    }
  }

// Réglage de l'ensoleillement
  if (_pin_soleil) {
    size_t num_horaire_soleil;
    for (num_horaire_soleil = 0 ; num_horaire_soleil < _nombre_horaire_soleil; ++num_horaire_soleil) {
      if (_heure_simulee >= _tableau_horaire_soleil [num_horaire_soleil].heure_debut &&
           (num_horaire_soleil + 1 == _nombre_horaire_soleil ||
            _heure_simulee < _tableau_horaire_soleil [num_horaire_soleil + 1].heure_debut)) {
        unsigned int debut_periode_soleil = _tableau_horaire_soleil [num_horaire_soleil].heure_debut;
        unsigned int duree_periode_soleil = ((num_horaire_soleil + 1 == _nombre_horaire_soleil) ?
                                             (24 * 60) :
                                             (_tableau_horaire_soleil [num_horaire_soleil + 1].heure_debut)) -
                                            debut_periode_soleil;
        unsigned int debut_valeur_soleil = _tableau_horaire_soleil [num_horaire_soleil].valeur_debut;
        unsigned int fin_valeur_soleil = (num_horaire_soleil + 1 == _nombre_horaire_soleil) ?
                                          (debut_valeur_soleil) :
                                          (_tableau_horaire_soleil [num_horaire_soleil + 1].valeur_debut);
        unsigned int valeur_soleil = debut_valeur_soleil + ((float) fin_valeur_soleil - (float) debut_valeur_soleil) * float (_heure_simulee - debut_periode_soleil) / float (duree_periode_soleil);
        analogWrite (_pin_soleil, valeur_soleil);
#ifdef DEBUG_SERIE
        Serial.print ("Début période ");
        Serial.print (debut_periode_soleil);
        Serial.print (", durée période ");
        Serial.print (duree_periode_soleil);
        Serial.print (", valeur début ");
        Serial.print (debut_valeur_soleil);
        Serial.print (", valeur fin ");
        Serial.print (fin_valeur_soleil);
        Serial.print (", valeur soleil ");
        Serial.println (valeur_soleil);
#endif
        break;
      }
    }
  }
}

void LumiereJourNuit::next () {
  delay (_delai_minute);

// Minute suivante
  ++_heure_simulee;
  if (_heure_simulee >= 24 * 60) {
    _heure_simulee = 0;
  }
}
