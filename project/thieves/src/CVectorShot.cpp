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

using namespace std;

#include <CRectT.h>
#include <GLinclude.h>
#include <CFileIO.h>
#include <CStringTool.h>
#include <CRandom.h>
#include "CVectorShot.h"
#include "SGameContext.h"

extern float gGlobalScale;

//---------------------------------------------------------------------------
//
// Klasse:		CVectorShot
// Methode:		CVectorShot
//
//
//---------------------------------------------------------------------------

CVectorShot::CVectorShot()
{
    mShotColor[0] = mShotColor[1] = mShotColor[2] = 1.0;
}


//---------------------------------------------------------------------------
//
// Klasse:		CVectorShot
// Methode:		RunShot
//
//
//---------------------------------------------------------------------------

void CVectorShot::SetShotColor(float r, float g, float b, float a)
{
    mShotColor[0] = r;
    mShotColor[1] = g;
    mShotColor[2] = b;
    mShotColor[3] = a;
}


//---------------------------------------------------------------------------
//
// Klasse:		CVectorShot
// Methode:		RunShot
//
//
//---------------------------------------------------------------------------

void CVectorShot::Run(SGameContext* gac)
{
    mShapeParams.mPosition += mDirection * gGlobalScale;

    if (!gac->mPlayField.InRect(mShapeParams.mPosition))
    {
        mStatus = EUS_INACTIVE;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:		CVectorShot
// Methode:		Draw
//
//
//---------------------------------------------------------------------------

void CVectorShot::Draw(int Mode)
{
    CRandom Random;

    glPushMatrix();

    glTranslatef(mShapeParams.mPosition.x, mShapeParams.mPosition.y, 0.0);
    glScalef(gGlobalScale, gGlobalScale, 1.0);


    glRotatef(mShapeParams.mRotation, 0.0, 0.0, 1.0);
    mShapeParams.mRotation += 5.0;
    if (mShapeParams.mRotation > 360.0)
    {
        mShapeParams.mRotation -= 360;
    }



    float scale = Random.Get(4, 8);

    glScalef(scale, scale, 1.0);
    glBegin(GL_TRIANGLE_FAN);

    mShotColor[3] = 0.5;
    glColor4fv(mShotColor);
    glVertex2f(0.0, 0.0);

    mShotColor[3] = 0.0;
    glColor4fv(mShotColor);
    glVertex2f(-1.0, -1.0);
    glVertex2f(1.0, -1.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(-1.0, 1.0);
    glVertex2f(-1.0, -1.0);

    glEnd();

    mShotColor[3] = 1.0;
    glColor4fv(mShotColor);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPointSize(2.0 * gGlobalScale);
    glBegin(GL_POINTS);
    glVertex2f(0.0, 0.0);
    glEnd();

    glPopMatrix();
}
