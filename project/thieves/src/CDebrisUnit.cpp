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
#include <CRandom.h>
#include <MathDefines.h>
#include <CVector3T.h>
#include "CDebrisUnit.h"

extern float gGlobalScale;

//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   CDebrisUnit
//
//
//---------------------------------------------------------------------------

CDebrisUnit::CDebrisUnit()
{
    mDebris = NULL;
    mDebrisCount = 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   ~CDebrisUnit
//
//
//---------------------------------------------------------------------------

CDebrisUnit::~CDebrisUnit()
{
    delete[] mDebris;
    delete[] mScrap;
    mDebris = NULL;
    mScrap = NULL;
    mDebrisCount = 0;
    mScrapCount = 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   Draw
//
//
//---------------------------------------------------------------------------

void CDebris::Draw()
{
    float pcolor[] = { 0.8, 0.9, 0.3, 1.0 };

    pcolor[3] = float(mLife) / 256;
    glColor4fv(pcolor);
    glVertex2fv(mPos.v());
    mPos += mDir;
    mDir *= 0.98;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   Init
//
//
//---------------------------------------------------------------------------

void CScrap::Draw()
{
    if (mColor != NULL)
    {
        mColor[3] = float(mLife) / 256;
        glColor4fv(mColor);
    }

    glPushMatrix();
    glTranslatef(mPos.x, mPos.y, 0.0);
    glRotatef(mRotation, 0.0, 0.0, 1.0);

    mRotation += mRotSpeed;

    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 3; i++)
    {
        glVertex2f(mShape[i].x, mShape[i].y);
    }
    glEnd();
    glPopMatrix();

    mPos += mDir;
    mDir *= 0.98;
    mRotSpeed *= 0.985;
}


//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   Init
//
//
//---------------------------------------------------------------------------

void CDebrisUnit::Init(float xpos, int ypos, int BigFaktor, const float* KColor, const CVector2<float>& dvec)
{
    CRandom Random;

    //
    // Debris
    //

    mDebrisCount = Random.GetInt(100, 255) * BigFaktor;
    mDebris = new CDebris[mDebrisCount];

    for (int i = 0; i < mDebrisCount; i++)
    {
        CDebris* db = mDebris + i;
        float Angle = DEG_TO_RAD(Random.Get(0, 360));
        float Impuls = Random.Get(0, 10) * gGlobalScale;

        db->mPos.x = xpos;
        db->mPos.y = ypos;


        db->mDir.x = Impuls * cos(Angle);
        db->mDir.y = Impuls * sin(Angle);
        db->mLife = Random.GetInt(100, 200);
        db->mDir.x += dvec.x * gGlobalScale;
        db->mDir.y += dvec.y * gGlobalScale;
    }


    //
    // Scrap
    //

    mScrapCount = Random.GetInt(10, 20) * BigFaktor;
    mScrap = new CScrap[mScrapCount];

    for (int i = 0; i < 3; i++)
    {
        mScrapColor[i] = 0.7;
        if (KColor != NULL)
        {
            mScrapColor[i] = KColor[i];
        }
        else
        {
            mScrapColor[i] = 0.7;
        }
    }

    for (int i = 0; i < mScrapCount; i++)
    {
        CScrap* sc = mScrap + i;

        float Angle = DEG_TO_RAD(Random.Get(0, 360));
        float Impuls = Random.Get(0, 10) * gGlobalScale;

        sc->mPos.x = xpos;
        sc->mPos.y = ypos;


        sc->mDir.x = Impuls * cos(Angle);
        sc->mDir.y = Impuls * sin(Angle);
        sc->mDir.x += dvec.x * gGlobalScale;
        sc->mDir.y += dvec.y * gGlobalScale;
        sc->mRotation = Random.Get(0, 360);
        sc->mLife = Random.GetInt(100, 200);
        sc->mRotSpeed = Random.Get(0, 20) - 10;
        sc->mColor = mScrapColor;

        for (int i = 0; i < 3; i++)
        {
            sc->mShape[i].x = Random.Get(4, 8) * Random.Sign() * gGlobalScale;
            sc->mShape[i].y = Random.Get(4, 8) * Random.Sign() * gGlobalScale;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CDebrisUnit
// Methode:   Draw
//
//
//---------------------------------------------------------------------------

bool CDebrisUnit::Draw()
{
    //cout << "CDebrisUnit::Draw Count=" << mDebrisCount << endl;

    bool alive = false;
    CDebris* db = mDebris;

    glBegin(GL_POINTS);
    for (int i = 0; i < mDebrisCount; i++)
    {
        if (db->mLife > 0)
        {
            db->mLife--;
            db->Draw();
            alive = true;
        }
        db++;
    }
    glEnd();
    //
    //
    //

    CScrap* sc = mScrap;

    for (int i = 0; i < mScrapCount; i++)
    {
        if (sc->mLife > 0)
        {
            sc->mLife--;
            sc->Draw();
            alive = true;
        }
        sc++;
    }
    return alive;
}
