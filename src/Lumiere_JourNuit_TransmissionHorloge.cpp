//#define DEBUG_SERIE

#include <Arduino.h>

#include "Lumiere_JourNuit_TransmissionHorloge.h"

struct message_heure {
  uint8_t identifiant_message;
  uint8_t heure;
  uint8_t minute;
};

LumiereJourNuitHorlogeTransmise::LumiereJourNuitHorlogeTransmise (const parametres_activite tableau_activite[], size_t nombre_actitive,
                                                                  const parametres_soleil tableau_soleil[], size_t nombre_horaires_soleil,
                                                                  uint8_t pin_soleil,
                                                                  uint8_t pin_rs485_rx, uint8_t pin_rs485_tx, uint8_t pin_rs485_dir)
  : LumiereJourNuit (tableau_activite, nombre_actitive, tableau_soleil, nombre_horaires_soleil, pin_soleil),
    _maitre_horloge (false),
    _rs485 (pin_rs485_rx, pin_rs485_tx, pin_rs485_dir, pin_rs485_dir)
{
}

void LumiereJourNuitHorlogeTransmise::begin (uint16_t delai_minute, bool maitre_horloge) {
  static const uint16_t duree_attente_maitre = 10000; // 10 secondes
  _maitre_horloge = maitre_horloge;
  // Initialisation du bus pour partager l'horloge
  _rs485.begin (19200, 10); // Avec une liaison série software ne pas utiliser une grande vitesse d'échange
  if (!_maitre_horloge) {
    _maitre_horloge = true;
    const unsigned long millis_debut = millis ();
    while (millis_debut + duree_attente_maitre > millis ()) {
      if (_rs485.available ()) {
        _maitre_horloge = false;
        break;
      }
      delay (1);
    }
  }
#ifdef DEBUG_SERIE
  Serial.println (_maitre_horloge ? "Maître horloge." : "Esclave horloge.");
#endif
  LumiereJourNuit::begin (_maitre_horloge ? delai_minute : 1);
}

void LumiereJourNuitHorlogeTransmise::loop () {
  horaire heure_actuelle = LumiereJourNuit::get_heure_simulee ();
  if (_maitre_horloge) {
    message_heure message;
    message.identifiant_message = 250;
    message.heure = heure_actuelle / 60;
    message.minute = heure_actuelle % 60;
    _rs485.write ((uint8_t*) &message, sizeof (message));
  }
  else {
    horaire nouvelle_heure = heure_actuelle;
    uint8_t heure_recue;
    uint8_t minute_recue;
    if (_rs485.available ()) {
      uint8_t identifiant_message = _rs485.read ();
      if (identifiant_message == 250) {
        unsigned long millis_debut = millis ();
        while (!_rs485.available () && millis () - millis_debut <= 100) {
          delay (1);
        }
        heure_recue = _rs485.read ();
        while (!_rs485.available () && millis () - millis_debut <= 100) {
          delay (1);
        }
        minute_recue = _rs485.read ();
        if (heure_recue < 24 && minute_recue < 60) {
          nouvelle_heure = heure_recue * 60 + minute_recue;
        }
#ifdef DEBUG_SERIE
        else {
          Serial.print ("Erreur heure transmise, heure ");
          Serial.print (heure_recue);
          Serial.print (", minute ");
          Serial.println (minute_recue);
        }
#endif
      }
      else {
#ifdef DEBUG_SERIE
        Serial.print ("Erreur identifiant ");
        Serial.println (identifiant_message);
#endif
        while (_rs485.available ()) {
          _rs485.read ();
        }
      }
    }
    if (nouvelle_heure == heure_actuelle) {
      delay (1);
      return;
    }
    LumiereJourNuit::set_heure_simulee (heure_recue, minute_recue);
  }
  LumiereJourNuit::loop ();
}

void LumiereJourNuitHorlogeTransmise::next () {
  if (_maitre_horloge) {
    LumiereJourNuit::next ();
  }
}
