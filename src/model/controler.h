#ifndef CONTROLER_H
#define CONTROLER_H

#include "message.h"

class Controler
{
public:
    Controler(const SerialConnectionParams &params);

private:
    Message messageProceser_;
};

#endif // CONTROLER_H
