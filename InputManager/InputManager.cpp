#include "InputManager.hpp"

// Fonctions relative au clavier
bool KBInputDown(int inputTarget) { // Retourne si une touche spécifique du clavier a été "triggered"
    // Important à savoir : Si le bit d'ordre élevé a pour valeure 1, la touche est "down" (pressée); sinon, elle est "up" (pas pressée).
    return GetKeyState(inputTarget) & 0x8000;
};

int KBLastPressed() { // Retourne la dernière touche préssée

}; 

void *KBLivePressed() { // Retourne une liste de toutes les touches du clavier qui sont préssées en temps réel
    
}; 


// Fonctions liées à la souris
bool MOUInputClicked(int inputTarget) { // Retourne si une touche spécifique de la souris a été "triggered"

}; 

int MOULastPressed() { // Retourne la dernière touche "triggered" par la souris

};

void* MOULivePressed() { // Retourne une liste de toutes les touches de la souris qui sont "triggered" en temps rée

}; 

MousePosition *MOUGetPosition() { // Retourne les positions x et y de la souris par rapport à l'écran

}; 