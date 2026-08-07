#include "InputManager.hpp"

int main() {
    POINT *mousePoint;
    CursorPosition cursor; 
    while (true) {
        if (GLInputDown(KB_SHIFT)) std::cout << "SHIFT" << std::endl;
        else if (GLInputDown(KB_SPACE)) std::cout << "SPACE" << std::endl;
        else if (GLInputDown(KB_ESC)) std::cout << "ESC" << std::endl;
        else if (GLInputDown(MOU_LEFT)) std::cout << "CLIC LEFT" << std::endl;
        else if (GLInputDown(MOU_RIGHT)) std::cout << "CLIC DROIT" << std::endl;
        else if (GLInputDown(MOU_MID)) std::cout << "CLIC MOLETTE" << std::endl;
        else if (GLInputDown(MOU_BUTTON1)) std::cout << "CLIC BOUTON 1" << std::endl;
        else if (GLInputDown(MOU_BUTTON2)) std::cout << "CLIC BOUTON 2" << std::endl;
        else if (GLInputDown(MOU_ZOOM)) std::cout << "ZOOM" << std::endl;
        else {
            GetCursorPos(mousePoint);
            cursor = MOUGetPosition();
            // std::cout << "POSITION CURSEUR : ";
            // std::cout<< "(" << mousePoint->x << "," << mousePoint->y << ") "; //<< std::endl;
            // std::cout << "(" << cursor.x << "," << cursor.y << ")" << std::endl;
        }

        Sleep(10);
    }
    return 0;
}