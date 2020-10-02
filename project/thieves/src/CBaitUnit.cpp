//---------------------------------------------------------------------------
//
// PROJECT : RIP OFF 2
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------


#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

#include <GLinclude.h>
#include <CFileIO.h>
#include <CStringTool.h>
#include <CRandom.h>
#include "CBaitUnit.h"

// ---------------------------------------------------------------------------
//
// KLASSE        : CBaitUnit
// METHODE       : CBaitUnit
//
//
//
// ---------------------------------------------------------------------------

CBaitUnit::CBaitUnit()
{
    CRandom Random;

    mPulsar = Random.Get(0.0, 360.0);
    mPulsarSpeed = Random.Get(45, 50);
    mStatus = EUS_BAIT_STAY;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CBaitUnit
// METHODE       : Run
//
//
//
// ---------------------------------------------------------------------------

void CBaitUnit::Run(SGameContext* gac)
{
    //std::cout << "Plusar=" << mPulsar << endl;
    mPulsar += mPulsarSpeed;
    if (mPulsar > 360)
    {
        mPulsar -= 360;
    }
    mShapeParams.mScale = 1.0 + (sin(DEG_TO_RAD(mPulsar)) / 13.0);
}
