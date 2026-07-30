#include "InputManager.hpp"

// Fonctions relative au clavier
bool GLInputDown(int inputTarget) { // Retourne si une touche spécifique du clavier a été "triggered"
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

CursorPosition MOUGetPosition() { // Retourne les positions x et y DU CURSEUR DE LA SOURIS par rapport à l'écran et renvoie un CursorPosition negatif si echec
    POINT cursorPosTemp;
    CursorPosition cursorPosRET; // CursorPosition qui sera retourné
    if (GetCursorPos(&cursorPosTemp)) {;
        cursorPosRET.x = cursorPosTemp.x;
        cursorPosRET.y = cursorPosTemp.y;
        return cursorPosRET;
    }
    else {
        return {-1,-1};
    }
}; 