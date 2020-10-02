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

#define APP_NAME    "ripoff2"

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstdio>

#include <GLinclude.h>
#include <math.h>
#include <CVector3T.h>
#include <COpenGL.h>
#include <CFileIO.h>
#include <CException.h>
#include <CRandom.h>


#include <CG3DGlobals.h>
#include "CSdlThiefs.h"

using namespace std;

// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : CSdlThiefs
//
//
//
// ---------------------------------------------------------------------------

CSdlThiefs::CSdlThiefs()
{
    mInitFlag = false;
    mShowInterface = true;
    mFullscreen = false;
    mResGlobals = new CG3DGlobals;
    mInitEnemies = false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::SetResolution(int w, int h)
{
    mResGlobals->mWindowWidth = w;
    mResGlobals->mWindowHeight = h;

    mGameContext.mPlayField.Set(0, 0, w, h);

    if (!mInitFlag)
    {
        InitResources();
        mInitFlag = true;
    }
    else
    {
        //gG3Dinterface->SendCommand(EG3DcomReload);
        //gG3Dinterface->SendCommand(EG3DcomAdjust);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : FindShape
//
//
//
// ---------------------------------------------------------------------------

CVectorShape* CSdlThiefs::FindShape(const char* Shapename)
{
    list<CVectorShape*>::iterator is;

    for (is = mShapeList.begin(); is != mShapeList.end(); ++is)
    {
        //cout << "Test:" << (*is)->mName << endl;
        if ((*is)->mName == Shapename)
        {
            return *is;
        }
    }
    stringstream mstr;

    mstr << "Shape not found:" << Shapename;
    throw(CException(0, mstr.str().c_str()));
    return NULL;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : NewAttackWave
//
//
//
// ---------------------------------------------------------------------------


void CSdlThiefs::NewAttackWave()
{
    CRandom Random;

    for (int i = 0; i < 12; i++)
    {
        float px, py;
        CEnemyUnit* EnemyUnit = new CEnemyUnit;
        EnemyUnit->mVectorShape = FindShape("enemy1");

        EnemyUnit->InitRandomPosition(&mGameContext, &px, &py);
        EnemyUnit->SetPosition(px, py);
        mUnitList.push_back(EnemyUnit);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : InitBait
//
//
//
// ---------------------------------------------------------------------------


void CSdlThiefs::InitBait()
{
    struct SKoor
    {
        int x, y;
    };

    SKoor OffTab[] =
    {
        { -20, -30 },
        {  20, -30 },

        { -30, -10 },
        {	0, -10 },
        {  30, -10 },

        { -30,	10 },
        {	0,	10 },
        {  30,	10 },

        { -20,	30 },
        {  20,	30 }
    };


    for (int i = 0; i < 10; i++)
    {
        CBaitUnit* BaitUnit = new CBaitUnit;
        BaitUnit->mVectorShape = FindShape("bait");

        float px = mGameContext.CenterX() + (OffTab[i].x * 2);
        float py = mGameContext.CenterY() + (OffTab[i].y * 2);
        //cout << "px=" << px << " py=" << py << endl;

        BaitUnit->SetPosition(px, py);
        mBaitList.push_back(BaitUnit);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : InitEnemies
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::InitEnemies()
{
    try
    {
        ReadShapes("shapes.vsh");

        mPlayer1.mVectorShape = FindShape("player1");
        //mPlayer2.mVectorShape  = FindShape("player1");
        mGameContext.mPlayer = &mPlayer1;
        mGameContext.mBaitList = &mBaitList;


        NewAttackWave();
        InitBait();
    }
    catch (CException& ex)
    {
        cout << "***** " << ex.mErrstr << endl;
        exit(0);
    }
    mInitEnemies = true;
    cout << "CSdlThiefs::InitEnemies OK" << endl;

    return;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawUnitPoints
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawUnitPoints(list<CVectorUnit*>& ulist)
{
    for (list<CVectorUnit*>::iterator ie = ulist.begin();
        ie != ulist.end();
        ++ie)
    {
        (*ie)->DrawPoints();
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawUnitList
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawUnitList(list<CVectorUnit*>& ulist, bool Shotable)
{
    vector<CVector2<float> > NewExplos;

    for (list<CVectorUnit*>::iterator ie = ulist.begin();
        ie != ulist.end();
        ++ie)
    {
        (*ie)->Draw(0);
        if (Shotable)
        {
            ShotHit(*ie, &NewExplos);
        }
    }

    if (Shotable)
    {
        for (vector<CVector2<float> >::iterator iv = NewExplos.begin();
            iv != NewExplos.end();
            ++iv)
        {
            CExplosion* ex = new CExplosion;
            ex->Init(iv->x, iv->y);
            mExploList.push_back(ex);
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawEnemies
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawEnemies()
{
    //cout << "CSdlThiefs::DrawEnemies START" << endl;
    float White[] = { 1.0, 1.0, 1.0, 1.0 };

    glColor4f(0.9, 0.8, 0.8, 1.0);

    glPointSize(1.5);
    glLineWidth(1.0);
    glColor4f(0.5, 0.5, 0.52, 1.0);

    DrawUnitList(mBaitList, false);
    DrawUnitList(mUnitList, true);
    DrawUnitList(mExploList, false);

    glColor4fv(White);
    DrawUnitList(mShotList, false);

    DrawUnitPoints(mBaitList);
    DrawUnitPoints(mUnitList);

    glColor4fv(White);
    DrawUnitPoints(mExploList);

    //cout << "RemoveInactive START" << endl;
    RemoveInactive(mUnitList);
    RemoveInactive(mShotList);
    RemoveInactive(mExploList);
    RemoveInactive(mBaitList);

    //cout << "RemoveInactive OK" << endl;
    //cout << "CSdlThiefs::DrawEnemies OK" << endl;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : Shot
//
//
//
// ---------------------------------------------------------------------------


void CSdlThiefs::Shot(int Player)
{
    CVectorShot* shot = new CVectorShot;

    mShotList.push_back(shot);

    shot->SetPosition(mPlayer1.mShapeParams.mPosition.x, mPlayer1.mShapeParams.mPosition.y);
    shot->mDirection.x = 3.5 * mPlayer1.mShapeParams.mSinRotation;
    shot->mDirection.y = -3.5 * mPlayer1.mShapeParams.mCosRotation;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : RemoveInactive
//
//
//
// ---------------------------------------------------------------------------
#if 1
void CSdlThiefs::RemoveInactive(list<CVectorUnit*>& ulist)
{
    list<CVectorUnit*>::iterator ie;

    vector<CVectorUnit*> RemoveVec;

    for (ie = ulist.begin(); ie != ulist.end(); ++ie)
    {
        if ((*ie)->mStatus == EUS_INACTIVE)
        {
            RemoveVec.push_back(*ie);
        }
    }

    vector<CVectorUnit*>::iterator iev;

    for (iev = RemoveVec.begin(); iev != RemoveVec.end(); ++iev)
    {
        ulist.remove(*iev);
        delete *iev;
    }
}


#endif



// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : RunUnits
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::RunUnits()
{
    mPlayer1.Run(&mGameContext);

    for (list<CVectorUnit*>::iterator ie = mUnitList.begin();
        ie != mUnitList.end();
        ++ie)
    {
        (*ie)->Run(&mGameContext);
    }

    for (list<CVectorUnit*>::iterator ie = mShotList.begin();
        ie != mShotList.end();
        ++ie)
    {
        (*ie)->Run(&mGameContext);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ShotHit
//
//
//
// ---------------------------------------------------------------------------

bool CSdlThiefs::ShotHit(CVectorUnit* enemy, vector<CVector2<float> >* explo)
{
    bool r = false;

    for (list<CVectorUnit*>::iterator ie = mShotList.begin();
        (ie != mShotList.end()) && (!r);
        ++ie)
    {
        float xpos = (*ie)->PosX();
        float ypos = (*ie)->PosY();
        if (enemy->Inside(xpos, ypos))
        {
            (*ie)->Destroy();
            enemy->Destroy();

            CVector2<float> expos(xpos, ypos);
            explo->push_back(expos);

            r = true;
        }
    }
    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : TestInside
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::TestInside()
{
/*
 *  mPlayer1.SetColor(1.0, 1.0, 1.0, 1.0);
 *  bool inside = false;
 *
 *  for (list<CVectorUnit*>::iterator ie  = mUnitList.begin();
 *                                    ie != mUnitList.end() && (!inside);
 ++ie)
 *  {
 *
 *      if ((*ie)->mVectorShape != NULL)
 *      {
 *          if ((*ie)->Inside(mPlayer1))
 *          {
 *              mPlayer1.SetColor(0.0, 0.0, 1.0, 1.0);
 *              inside = true;
 *          }
 *      }
 *  }
 */
    /*
     * for (list<CVectorUnit*>::iterator ie  = mShotList.begin();
     *                                ie != mShotList.end();
     ++ie)
     * {
     *  if (ShotHit(*ie))
     *  {
     *      CExplosion* ex = new CExplosion;
     *      ex->Init((*ie)->PosX(), (*ie)->PosY());
     *      mUnitList.push_back(ex);
     *  }
     * }
     */
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : InitResources
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::InitResources()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : MouseWheel
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::MouseWheel(int d)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : Zoom
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::Zoom(float f)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : LeftMouseButtonDown
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::LeftMouseButtonDown()
{
    mGameContext.mLeftButton = true;
    Shot(0);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : LeftMouseButtonUp
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::LeftMouseButtonUp()
{
    mGameContext.mLeftButton = false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : RightMouseButtonDown
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::RightMouseButtonUp()
{
    mGameContext.mRightButton = false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : RightMouseButtonDown
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::RightMouseButtonDown()
{
    mGameContext.mRightButton = true;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : MouseMotionLeft
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::MouseMotionLeft(int x, int y)
{
/*
 *  mPlayer1.AddPosX(-x);
 *  mPlayer1.AddPosY(y);
 */
}


void CSdlThiefs::MouseMotionRight(int x, int y)
{
}


void CSdlThiefs::Draw3DObjects(void)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : MouseMotion
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::MouseMotion(int x, int y)
{
    mPlayer1.AddRotation(x / 2);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : LoadEnvTexture
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::LoadEnvTexture(bool Anaglyph)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : SpecialkeysAction
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::SpecialkeysAction(int key)
{
    switch (key)
    {
        case GLUT_KEY_F1:

            cout << "F1" << endl;
            break;
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : KeyboardAction
//
//
//
// ---------------------------------------------------------------------------



void CSdlThiefs::KeyboardAction(unsigned char key)
{
    switch (key)
    {
        case 'r':

            break;

        case 'q':
        case 27:

            exit(0);
            break;
    }
}


//TestIntersect

// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ManageInterface
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ManageInterface(CGL_Mouse* Mouse)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : Draw2DObjects
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::Draw2DObjects()
{
    if (mInitFlag)
    {
        if (!mInitEnemies)
        {
            InitEnemies();
        }


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glLineWidth(1.0);

        mPlayer1.DrawPoints();
        mPlayer1.Draw(0);

        RunUnits();
        DrawEnemies();
        TestInside();

        if (mUnitList.size() == 0)
        {
            NewAttackWave();
        }


        glDisable(GL_BLEND);
    }
}
