#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Windows.h> // GetKeyState
#include <iostream> // cout

// IMPORTANT :
// GL = GLobal -> clavier, souris
// KB = "KeyBoard" -> clavier
// MOU = Mouse -> souris


// IMPORTANT : La "casse" des lettres n'est pas pris en compte!
// Cela veut dire que KB_A est aussi égal à KB_a, vois-tu ?

// Toutes les macros des touches du clavier (évite de vérifier les touches avec des valeures hexadecimales...)
#define KB_0 0x60
#define KB_1 0x61
#define KB_2 0x62
#define KB_3 0x63
#define KB_4 0x64
#define KB_5 0x65
#define KB_6 0x66
#define KB_7 0x67
#define KB_8 0x68
#define KB_9 0x69

#define KB_A 0x41
#define KB_B 0x42
#define KB_C 0x43
#define KB_D 0x44
#define KB_E 0x45
#define KB_F 0x46
#define KB_G 0x47
#define KB_H 0x48
#define KB_I 0x49
#define KB_J 0x4A
#define KB_K 0x4B
#define KB_L 0x4C
#define KB_M 0x4D
#define KB_N 0x4E
#define KB_O 0x4F
#define KB_P 0x50
#define KB_Q 0x51
#define KB_R 0x52
#define KB_S 0x53
#define KB_T 0x54
#define KB_U 0x55
#define KB_V 0x56
#define KB_W 0x57
#define KB_X 0x58
#define KB_Y 0x59
#define KB_Z 0x5A

#define KB_SHIFT 0x10
#define KB_SPACE 0x20
#define KB_ESC 0x1B

// Toutes les macros des touches de la souris
#define MOU_LEFT 0x01
#define MOU_RIGHT 0x02
#define MOU_MID 0x04
#define MOU_BUTTON1 0x06
#define MOU_BUTTON2 0x05
#define MOU_ZOOM 0xFB

struct CursorPosition { // Simple structure contenant les positions x and y de la souris par rapport à l'écran
    LONG x; // Fait référence à tagPoint::x
    LONG y; // Fait référence à tagPoint::y
};     

bool GLInputDown(int inputTarget); // Retourne si une touche spécifique du clavier ou  a été "triggered"

bool KBInputToggle(int inputTarget); // Retourne si une touche spécifique du clavier a été "toggled"
// Note pour implémentation : Si le bit d'ordre bas a pour valeure 1, 

int KBLastPressed(); // Retourne la dernière touche préssée
void *KBLivePressed(); // Retourne une liste de toutes les touches du clavier qui sont préssées en temps réel

bool MOUInputClicked(int inputTarget); // Retourne si une touche spécifique de la souris a été "triggered"
int MOULastPressed(); // Retourne la dernière touche "triggered" par la souris
void* MOULivePressed(); // Retourne une liste de toutes les touches de la souris qui sont "triggered" en temps réel
CursorPosition MOUGetPosition(); // Retourne les positions x et y DU CURSEUR DE LA SOURIS par rapport à l'écran et renvoie un CursorPosition negatif si echec
// C'est pourquoi cette fonction renvoie void et non pas un CursorPosition comme il était dis de base..

#endif