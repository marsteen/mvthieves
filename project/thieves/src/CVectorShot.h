#ifndef CVECTORSHOT_H
#define CVECTORSHOT_H

#include "CVectorUnit.h"

class CVectorShot : public CVectorUnit
{
    public:

        CVectorShot();


        void Draw(int Mode);
        void Run(SGameContext* gac);
        void SetShotColor(float r, float g, float b, float a);

        int mPlayerNum;


    protected:

        float mShotColor[4];
};


#endif
