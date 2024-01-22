/*
 Fichier Lumiere_JourNuit.cpp
 
 Cette librairie permet d'allumer et éteindre des led pour simuler l'éclairage d'une scène le jour et la nuit, ainsi que d'un soleil.
 
 Written by Thierry MARTIN
 LGPL License, all text above must be included in any redistribution
*/

#ifndef Lumiere_JourNuit_h
#define Lumiere_JourNuit_h

typedef uint16_t horaire;

#define HM(H, M) (H * 60 + M)

struct parametres_activite {
  horaire heure_debut;
  uint8_t numero_pin;
  uint8_t valeur_led;
};

struct parametres_soleil {
  horaire heure_debut;
  uint8_t valeur_debut;
};

class LumiereJourNuit {
    public:
        LumiereJourNuit (const parametres_activite tableau_activite[], size_t nombre_actitive, const parametres_soleil tableau_soleil[], size_t nombre_horaires_soleil, uint8_t pin_soleil);
        void begin (uint16_t delai_minute);
        // Délai pour une minute
        // 100 ms = 0,1 seconde => 1 minute de la journée simulée, 2 minutes et 24 secondes => 1 journée simulée
        // 1000 ms = 1 seconde => 1 minute de la journée simulée, 24 minutes => 1 journée simulée
        // Si le délai pour une minute n'est pas précisé alors le mode est toujours esclave
        void loop ();
        void next (); // Minute suivante
        horaire get_heure_simulee () { // Permet de lire l'heure simulée afin d'ajouter des fonctions dans la boucle
          return _heure_simulee;
        }
    protected:
        void set_heure_simulee (uint8_t nouvelle_heure, uint8_t nouvelle_minute) {
          _heure_simulee = nouvelle_heure * 60 + nouvelle_minute;
        }
        void set_delai_minute (uint8_t delai_minute) {
          _delai_minute = delai_minute;
        }
    private:
        uint16_t _delai_minute;
        horaire _heure_simulee;
        const size_t _nombre_activite;
        const size_t _nombre_horaire_soleil;
        const uint8_t _pin_soleil;
        const parametres_activite* _tableau_activite;
        const parametres_soleil* _tableau_horaire_soleil;

    friend class TransmissionHorlogeJourNuit;
};

#endif
