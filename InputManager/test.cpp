#include "InputManager.hpp"

int main() {
    int i = 0;
    while (true) {
        if (KBInputDown(KB_A)) {
            i++;
            std::cout << i << std::endl;
        }
        if (KBInputDown(KB_SHIFT)) std::cout << "SHIFT" << std::endl;
        else if (KBInputDown(KB_SPACE)) std::cout << "SPACE" << std::endl;
        else if (KBInputDown(KB_ESC)) std::cout << "ESC" << std::endl;

        Sleep(10);
    }
    return 0;
}