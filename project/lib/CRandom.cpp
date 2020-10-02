//---------------------------------------------------------------------------
//
// PROJECT : thiefs
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include <cstdlib>
#include <CRandom.h>

using namespace std;


float CRandom::Get01()
{
    return rand() / float(RAND_MAX);
}


//---------------------------------------------------------------------------
//
// KLASSE        : CRandom
// METHODE       : Get
//
//
//
//---------------------------------------------------------------------------

float CRandom::Get(float Low, float High)
{
    return (Get01() * (High - Low)) + Low;
}


//---------------------------------------------------------------------------
//
// KLASSE        : CRandom
// METHODE       : GetInt
//
//
//
//---------------------------------------------------------------------------

int CRandom::GetInt(int Low, int High)
{
    return (int)((Get01() * (High - Low + 1)) + Low);
}
