#ifndef CANGLE_H
#define CANGLE_H

template<typename Tfloat>
class CAngle
{
    public:

        void Trim360(Tfloat& a1);
        Tfloat Diff(const Tfloat a1, const Tfloat a2) const;
};


//---------------------------------------------------------------------------
//
// Klasse:		CAngle<Tfloat>
// Methode:   Diff
//
// Parameter: a1  = Winkel
//
// Return:    a1 wird auf den Bereich 0..360 Grad getrimmt
//
//---------------------------------------------------------------------------

template<typename Tfloat>
void CAngle<Tfloat>::Trim360(Tfloat& a1)
{
    while (a1 > 360.0)
    {
        a1 -= 360.0;
    }

    while (a1 < 0.0)
    {
        a1 += 360.0;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:		CAngle<Tfloat>
// Methode:   Diff
//
// Parameter: a1, a2 = Winkel (0..360 Grad)
//
// Return:    a2 - a1
//
//---------------------------------------------------------------------------

template<typename Tfloat>
Tfloat CAngle<Tfloat>::Diff(const Tfloat a1, const Tfloat a2) const
{
    Tfloat diff;

    if (a2 > a1)
    {
        Tfloat d1 = a2 - a1;
        Tfloat d2 = (a1 + 360.0) - a2;

        if (d2 < d1)
        {
            diff = -d2;
        }
        else
        {
            diff = d1;
        }
    }
    else
    {
        Tfloat d1 = a1 - a2;
        Tfloat d2 = (a2 + 360.0) - a1;

        if (d2 < d1)
        {
            diff = d2;
        }
        else
        {
            diff = -d1;
        }
    }
    return diff;
}


#endif
