#ifndef CBAITUNIT_H
#define CBAITUNIT_H

#include "CVectorUnit.h"

class CBaitUnit : public CVectorUnit
{
    public:

        CBaitUnit();
        void Run(SGameContext* gac);



    protected:

        float mPulsar;
        float mPulsarSpeed;
};

#endif
