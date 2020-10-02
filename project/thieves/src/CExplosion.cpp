/*
 *
 * // ---------------------------------------------------------------------------
 * //
 * // KLASSE        : CExplosion
 * // METHODE       :
 * //
 * //
 * //
 * // ---------------------------------------------------------------------------
 *
 *
 *
 */


#define EXPLO_SPEED    10.3

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <GLinclude.h>
#include <CVector3T.h>
#include <COpenGL.h>
#include <CFileIO.h>
#include <CException.h>
#include <CRandom.h>
#include "CExplosion.h"

using namespace std;

extern float gGlobalScale;

//---------------------------------------------------------------------------
//
// Klasse:		CExplosion
// Methode:   ~CExplosion
//
//
//---------------------------------------------------------------------------

CExplosion::~CExplosion()
{
    //cout << "***** ~CExplosion" << endl;
    mPointVec.clear();
    delete[] mSpikeVector;
}


//---------------------------------------------------------------------------
//
// Klasse:		CExplosion
// Methode:   Init
//
//
//---------------------------------------------------------------------------

void CExplosion::Init(float xpos, float ypos)
{
    //cout << "CExplosion::Init START" << endl;
    CRandom xrand;

    mStep = 0;
    mSpikes = xrand.Get(40, 50);

    mSpikeVector = new CVector2<float>[mSpikes];
    mSpikeLen = new int[mSpikes];

    mShapeParams.mOrigin.x = xpos;
    mShapeParams.mOrigin.y = ypos;


    //cout << "Spikes=" << mSpikes << endl;

    for (int i = 0; i < mSpikes; i++)
    {
        float Angle = DEG_TO_RAD(float(rand() % 3600) / 10);
        float Len = xrand.Get(100, 200) * gGlobalScale;

        //cout << "Angle=" << Angle << endl;

        mSpikeVector[i].x = cos(Angle) * Len;
        mSpikeVector[i].y = sin(Angle) * Len;
        mSpikeLen[i] = Len;
    }
    //cout << "CExplosion::Init OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:		CExplosion
// Methode:
//
//
//---------------------------------------------------------------------------

void CExplosion::Draw(int Mode)
{
    //cout << "CExplosion::Draw START" << endl;



    glColor4f(1.0, 1.0, 0.5, 0.4);

    if (Mode == 0)
    {
        //mStep += 5.1;
        mStep += EXPLO_SPEED;
        mPointVec.clear();

        if (mStep > 180.0)
        {
            mStatus = EUS_INACTIVE;
        }
        else
        {
            for (int i = 0; i < mSpikes; i++)
            {
                float x = mShapeParams.mOrigin.x;
                float y = mShapeParams.mOrigin.y;
                float fa = sin(DEG_TO_RAD(mStep));

                CVector2<float> Point;

                Point.x = x + mSpikeVector[i].x * fa;
                Point.y = y + mSpikeVector[i].y * fa;

                mPointVec.push_back(Point);
            }
        }
    }


    glBegin(GL_LINES);
    for (int i = 0; i < mSpikes; i++)
    {
        glVertex2f(mShapeParams.mOrigin.x, mShapeParams.mOrigin.y);
        glVertex2f(mPointVec[i].x, mPointVec[i].y);
    }
    glEnd();

    //cout << "CExplosion::Draw OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:		CExplosion
// Methode:   DrawPoints
//
//
//---------------------------------------------------------------------------
void CExplosion::DrawPoints()
{
    float ExploColor[] = { 1.0, 1.0, 0.5, 0.8 };

    glColor4fv(ExploColor);
    glBegin(GL_POINTS);
    for (int i = 0; i < mPointVec.size(); i++)
    {
        glVertex2f(mPointVec[i].x, mPointVec[i].y);
    }
    glEnd();
}
