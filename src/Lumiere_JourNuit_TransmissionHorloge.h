#include <MAX_RS485.h>

#include "Lumiere_JourNuit.h"

#ifndef Lumiere_JourNuit_TransmissionHorloge_h
#define Lumiere_JourNuit_TransmissionHorloge_h

class LumiereJourNuitHorlogeTransmise : private LumiereJourNuit {
    public:
        LumiereJourNuitHorlogeTransmise (const parametres_activite tableau_activite[], size_t nombre_actitive,
                                         const parametres_soleil tableau_soleil[], size_t nombre_horaires_soleil,
                                         uint8_t pin_soleil,
                                         uint8_t pin_rs485_rx, uint8_t pin_rs485_tx, uint8_t pin_rs485_dir);
        void begin (uint16_t delai_minute, bool maitre_horloge);
        void loop ();
        void next (); // Mise à jour de l'heure du maitre
    private:
        bool _maitre_horloge;
        MAX_RS485 _rs485;
};

#endif
