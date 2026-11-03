
#include "infrarouge.h"

void InfrarougeInit()
{
    IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
}

bool InfrarougeDetection()
{
    if (IrReceiver.decode())
    {
        bool reveil = false;

        if (IrReceiver.decodedIRData.protocol == NEC &&
            IrReceiver.decodedIRData.address == 0x00 &&
            IrReceiver.decodedIRData.command == COMMANDE_REVEIL)
        {
            reveil = true;
        }

        IrReceiver.resume();

        return reveil;
    }

    return false;
}
