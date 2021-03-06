#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#include <math.h>
#include <MathDefines.h>
#include "SShapeParams.h"


//---------------------------------------------------------------------------
//
// Klasse:		SShapeParams
// Methode:		SShapeParams
//
//
//---------------------------------------------------------------------------

SShapeParams::SShapeParams()
{
    mOrigin.x = 0;
    mOrigin.y = 0;

    mPosition.x = 0;
    mPosition.y = 0;
    mScale = 1.0;

    SetRotation(0.0);
}


//---------------------------------------------------------------------------
//
// Klasse:		SShapeParams
// Methode:		Print
//
//
//---------------------------------------------------------------------------

std::string SShapeParams::Print() const
{
    stringstream mstr;

    mstr	<< "x=" << mPosition.x << " y=" << mPosition.y
            << " ox=" << mOrigin.y << " oy=" << mOrigin.y
            << " scale=" << mScale << " rot=" << mRotation;

    return mstr.str();
}


//---------------------------------------------------------------------------
//
// Klasse:		SShapeParams
// Methode:		SetRotation
//
//
//---------------------------------------------------------------------------

void SShapeParams::SetRotation(float r)
{
    mRotation = r;
    mSinRotation = sin(DEG_TO_RAD(r));
    mCosRotation = cos(DEG_TO_RAD(r));
}


//---------------------------------------------------------------------------
//
// Klasse:		SShapeParams
// Methode:		AddRotation
//
//
//---------------------------------------------------------------------------

void SShapeParams::AddRotation(float r)
{
    mRotation += r;
    while (mRotation < 0)
    {
        mRotation += 360.0;
    }

    while (mRotation > 360.0)
    {
        mRotation -= 360.0;
    }

    mSinRotation = sin(DEG_TO_RAD(mRotation));
    mCosRotation = cos(DEG_TO_RAD(mRotation));
}
