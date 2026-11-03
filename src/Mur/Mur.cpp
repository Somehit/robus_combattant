
#include "Mur.h"

void mur() {

    
    while (!isObstacle()) {
        moveForward(0.01f);
    }

    
    rotate(ROTATE_RIGHT);

    
    moveForward(0.30f);

    
    rotate(ROTATE_LEFT);

    
    moveForward(0.50f);

    
    rotate(ROTATE_LEFT);

    
    moveForward(0.30f);

    
    rotate(ROTATE_RIGHT);
}
