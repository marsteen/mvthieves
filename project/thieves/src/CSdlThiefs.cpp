//------------------------------------------------------------------------------
//
// PROJECT : Thieves
//
// FILE    : CSdlApp.cpp
//
// VERSION : 1.0
//
// AUTOR   : Martin Steen
//
//---------------------------------------------------------------------------

#define APP_NAME    "thieves"

#ifdef _WIN32
#include <windows.h>
#endif


#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <cstdio>

#include <GLinclude.h>
#include <math.h>
#include <CVector3T.h>
#include <CFileIO.h>
#include <CException.h>
#include <CRandom.h>
#include <CKeymap.h>
#include <CGL_FramebufferObject.h>

#include <CG3DGlobals.h>
#include "CSdlThiefs.h"

using namespace std;


static CGL_FramebufferObject* StaticFramebufferObject;

int gGlobalWindowHeight;
float gGlobalLineWidth = 1.0;
float gGlobalScale = 1.0;
float gGlobalScorePos = 0;

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
    mInitEnemies = false;
    mExploded = false;
    mGameContext.mPlayers = 0;
    mScore[0] = mScore[1] = 0;
    mLevel = 0;
    mHighScore = 0;
    mGameStatus = EGAMESTATUS_WAITING;
    mScreenSaveMode = false;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ~CSdlThiefs
//
//
//
// ---------------------------------------------------------------------------

CSdlThiefs::~CSdlThiefs()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : InitGame
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::InitGame()
{
    //mOpenAL.Init();
    if (!mSdlSound.InitAudio())
    {
        exit(0);
    }
    // cout << "Loading sounds.." << endl;
    SDL_WM_SetCaption("Thieves", "Thieves");
    SDL_WM_SetIcon(SDL_LoadBMP("files/icon.bmp"), NULL);


    mSdlSound.LoadWav("files/sounds/explosion1.wav");
    mSdlSound.LoadWav("files/sounds/explosion2.wav");
    mSdlSound.LoadWav("files/sounds/schuss2.wav");

    mSdlSound.LoadWav("files/sounds/1_y.wav"); // 3
    mSdlSound.LoadWav("files/sounds/2_y.wav");
    mSdlSound.LoadWav("files/sounds/3_y.wav");
    mSdlSound.LoadWav("files/sounds/4_y.wav");
    mSdlSound.LoadWav("files/sounds/4_y.wav");
    mSdlSound.LoadWav("files/sounds/5_y.wav");
    mSdlSound.LoadWav("files/sounds/6_y.wav");
    mSdlSound.LoadWav("files/sounds/7_y.wav");
    mSdlSound.LoadWav("files/sounds/8_y.wav");
    mSdlSound.LoadWav("files/sounds/9_y.wav");
    mSdlSound.LoadWav("files/sounds/10_y.wav"); // 13


    STextureParams tp;

    tp.mMinFilter = GL_NEAREST;
    tp.mMagFilter = GL_NEAREST;
    mMainTex.LoadTexture(GsysPath("files/thieves.tga"), NULL);
    mGameOverTex.LoadTexture(GsysPath("files/gameover.tga"), NULL);
    ReadHighScore();

    mSdlSound.StartPlaying();



    //cout << "Loading sounds ok" << endl;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : FinishGame
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::FinishGame()
{
    mSdlSound.Finish();
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
    mGameContext.mPlayField.Set(0, 0, w, h);
    gGlobalWindowHeight = h;

    if (!mInitFlag)
    {
        mInitFlag = true;
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


struct SLevel
{
    int mThiefs;
    int mThiefs2;
    int mFighters;
    int mBombers;
};

static SLevel sLevels[] =
{
    {  2, 0,  0,  0 }, // 0
    {  3, 0,  0,  0 },
    {  4, 0,  0,  1 },
    {  3, 1,  0,  0 },
    {  4, 1,  0,  0 },

    {  0, 0,  5,  5 }, // 5
    {  2, 4,  1,  0 },
    {  4, 1,  1,  0 },
    {  3, 2,  0,  1 },
    {  4, 2,  2,  1 },

    {  0, 2,  6,  8 }, // 10
    {  5, 2,  2,  1 },
    {  5, 2,  2,  0 },
    {  6, 2,  2,  1 },
    {  6, 2,  3,  0 },

    {  0, 0,  8,  8 }, // 15
    {  7, 2,  3,  2 },
    {  7, 3,  3,  2 },
    {  8, 3,  3,  2 },
    { 10, 3,  3,  2 },

    {  0, 0, 10, 10 }, // 20
    { 13, 2,  4,  2 },
    { 13, 3,  4,  3 },
    { 14, 3,  4,  4 },
    { 16, 4,  4,  5 }, // 24
};



void CSdlThiefs::NewAttackWave(bool Reset)
{
    if (Reset)
    {
        mGameContext.mEnemyMaxSpeed = 2.2;
        ClearList(mUnitList);
        mLevel = 0;
    }

    float ef = 1.0;

    if (mGameContext.mPlayers == 2)
    {
        ef = 1.5;
    }

    int EnemyLevel;

    if (mLevel > 24)
    {
        EnemyLevel = 20  + (mLevel % 5);
    }
    else
    {
        EnemyLevel = mLevel;
    }


    int Enemies = sLevels[EnemyLevel].mThiefs * ef;
    int Enemies2 = sLevels[EnemyLevel].mThiefs2 * ef;
    int Fighters = sLevels[EnemyLevel].mFighters;
    int Bombers = sLevels[EnemyLevel].mBombers;



    //CRandom Random;
    for (int i = 0; i < Enemies; i++)
    {
        float px, py;
        CEnemyUnit* EnemyUnit = new CEnemyUnit(ETYPE_THIEF);
        EnemyUnit->mVectorShape = FindShape("enemy1");

        EnemyUnit->InitRandomPosition(&mGameContext, &px, &py);
        EnemyUnit->SetPosition(px, py);
        mUnitList.push_back(EnemyUnit);
    }


    for (int i = 0; i < Enemies2; i++)
    {
        float px, py;
        CEnemyUnit* EnemyUnit = new CEnemyUnit(ETYPE_THIEF2);
        EnemyUnit->mVectorShape = FindShape("enemy4");

        EnemyUnit->InitRandomPosition(&mGameContext, &px, &py);
        EnemyUnit->SetPosition(px, py);
        mUnitList.push_back(EnemyUnit);
    }


    for (int i = 0; i < Fighters; i++)
    {
        float px, py;
        CEnemyUnit* EnemyUnit = new CEnemyUnit(ETYPE_FIGHTER);
        EnemyUnit->mVectorShape = FindShape("enemy2");
        EnemyUnit->InitRandomPosition(&mGameContext, &px, &py);
        EnemyUnit->SetPosition(px, py);
        mUnitList.push_back(EnemyUnit);
    }

    for (int i = 0; i < Bombers; i++)
    {
        float px, py;
        CEnemyUnit* EnemyUnit = new CEnemyUnit(ETYPE_BOMBER);
        EnemyUnit->mVectorShape = FindShape("enemy3");
        EnemyUnit->InitRandomPosition(&mGameContext, &px, &py);
        EnemyUnit->SetPosition(px, py);
        mUnitList.push_back(EnemyUnit);
    }



    if (mGameContext.mEnemyMaxSpeed < ENEMY_MAX_SPEED)
    {
        mGameContext.mEnemyMaxSpeed += 0.3;
    }

    mLevel += 1;
    if (mGameContext.mPlayers > 0)
    {
        mLevelDrawScale = 20.0;

        if (mLevel == 1)
        {
            mLevelDrawWait = 50;
        }
        else
        {
            mLevelDrawWait = 100;
        }
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
        float x, y;
    };

    const SKoor OffTab[] =
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

        float px = mGameContext.CenterX() + (OffTab[i].x * 2 * gGlobalScale);
        float py = mGameContext.CenterY() + (OffTab[i].y * 2 * gGlobalScale);
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
        ReadShapes("files/shapes.vsh");

        mPlayer[0].mVectorShape = FindShape("player1");
        mPlayer[1].mVectorShape = FindShape("player2");

        mPlayer[0].mNumber = 0;
        mPlayer[1].mNumber = 1;

        for (int i = 0; i <= 9; i++)
        {
            char Name[3];
            Name[0] = 'z';
            Name[1] = '0' + i;
            Name[2] = 0;
            mZiffern[i].mVectorShape = FindShape(Name);
        }

        mGameContext.mPlayer = mPlayer;
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
    //cout << "CSdlThiefs::InitEnemies OK" << endl;

    return;
}


#define YPOS_SCORE    (100 + gGlobalScorePos) //(gGlobalWindowHeight - 300)

// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawScore
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawScore(int xpos, int Score, const float* Color)
{
    // ,,
    float gs = gGlobalScale * gGlobalScale;
    //float ypos = gGlobalWindowHeight / 2;


    float ypos = YPOS_SCORE; // mGameContext.mPlayField.bottom;

    //cout << "ypos=" << ypos << endl;

    for (int i = 0; i < 6; i++)
    {
        int z = Score % 10;
        mZiffern[z].SetColorPtr(Color, 0.6);
        mZiffern[z].SetPosition(float(xpos) + (6 - i) * 32 * gs, ypos);
        mZiffern[z].Draw(0);

        Score /= 10;
        if (Score == 0)
        {
            break;
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawCenterScore
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawCenterScore(int Score, int xpos, const float* Color)
{
    float gs = gGlobalScale * gGlobalScale;


    float ypos = YPOS_SCORE;
    int dscore = Digits(Score);

    xpos += Digits(Score) * 32.0 * gs / 2;


    //cout << "w="  << mGameContext.mPlayField.Width()
    //     << " h=" << mGameContext.mPlayField.Height()  << endl;

    //cout << "gGlobalScale=" << gGlobalScale << endl;

    for (int i = 0; i < dscore; i++)
    {
        int z = Score % 10;
        mZiffern[z].SetColorPtr(Color, 0.6);
        mZiffern[z].SetPosition(xpos, ypos);
        mZiffern[z].Draw(0);
        Score /= 10;
        xpos -= 32 * gs;

        if (z == 0)
        {
            // cout << mZiffern[z].PrintShapeParams() << endl;
        }
    }

    //yoff += 1;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawLevel
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawLevel()
{
    if (mLevelDrawScale > 1.0)
    {
        if (mLevelDrawWait > 0)
        {
            mLevelDrawWait--;
        }
        else
        {
            float Color[] = { 1.0, 1.0, 0.2, 1.0 };

            float Alpha = mLevelDrawScale / 20.0;
            Color[3] = Alpha;

            int lz = mLevel;
            int digits; // Anzahl der Stellen
            float xpos = mGameContext.CenterX();
            float ypos = mGameContext.CenterY();

            for (int i = 0; i < 4; i++)
            {
                lz /= 10;
                if (lz == 0)
                {
                    digits = i+1;
                    break;
                }
            }

            lz = mLevel;
            xpos += (12 * mLevelDrawScale * gGlobalScale) * (digits - 1);

            for (int i = 0; i < digits; i++)
            {
                int z = lz % 10;
                mZiffern[z].SetColorPtr(Color, Alpha * 0.6);
                mZiffern[z].mShapeParams.mScale = mLevelDrawScale;
                mZiffern[z].SetPosition(xpos, ypos);
                mZiffern[z].Draw(0);
                mZiffern[z].mShapeParams.mScale = 1;

                xpos -= 24 * mLevelDrawScale * gGlobalScale;

                lz /= 10;
                if (lz == 0)
                {
                    break;
                }
            }
            mLevelDrawScale *= 0.98;
        }
    }
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

#define CAN_BE_SHOT		1
#define CAN_HIT			2

void CSdlThiefs::DrawUnitList(list<CVectorUnit*>& ulist, int HitStatus)
{
    vector<CVector2<float> > NewExplos;

    for (list<CVectorUnit*>::iterator ie = ulist.begin();
        ie != ulist.end();
        ++ie)
    {
        (*ie)->Draw(0);
        if (HitStatus & CAN_BE_SHOT)
        {
            ShotHit(*ie, &NewExplos);
        }
        if (HitStatus & CAN_HIT)
        {
            if (TestCrash(*ie))
            {
                (*ie)->Destroy();
            }
        }
    }

    if (HitStatus & (CAN_BE_SHOT | CAN_HIT))
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
// METHODE       : PlayerExplodes
//
// Spieler explodiert
//
// ---------------------------------------------------------------------------

void CSdlThiefs::PlayerExplodes(int p)
{
    mPlayer[p].mTimeOut = 200;
    NewDebris(mPlayer[p].PosX(), mPlayer[p].PosY(), 5, mPlayer[p].GetColor(), mPlayer[p].mDirection);

    mExploded = true;
    mSdlSound.PlayWav(SOUND_EXPLO2);
    mGameContext.mShootTime = 300;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : TestPlayerCrash
//
//
//
// ---------------------------------------------------------------------------

bool CSdlThiefs::TestPlayerCrash()
{
    bool r = false;

    if (mGameContext.mPlayers == 2)
    {
        if ((mPlayer[0].mTimeOut == 0) &&
            (mPlayer[1].mTimeOut == 0))
        {
            // Spieler zusammengestossen?
            if (mPlayer[0].Inside(mPlayer[1]))
            {
                // beide explodieren
                PlayerExplodes(0);
                PlayerExplodes(1);
                r = true;
            }
        }
    }
    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : TestCrash
//
//
//
// ---------------------------------------------------------------------------

bool CSdlThiefs::TestCrash(CVectorUnit* vu)
{
    bool r = false;

    for (int p = 0; p < mGameContext.mPlayers; p++)
    {
        if (mPlayer[p].mTimeOut == 0)
        {
            if (mPlayer[p].Inside(*vu))
            {
                PlayerExplodes(p);
                NewDebris(vu->PosX(), vu->PosY(), 1, vu->GetColor(), vu->mDirection);
                r = true;

/*
 *      mPlayer[p].mTimeOut = 200;
 *      NewDebris(mPlayer[p].PosX(), mPlayer[p].PosY(), 5, mPlayer[p].GetColor(), mPlayer[p].mDirection);
 *      NewDebris(vu->PosX(), vu->PosY(), 1, vu->GetColor(), vu->mDirection);
 *      r = true;
 *              mExploded = true;
 *
 *      mSdlSound.PlayWav(SOUND_EXPLO2);
 */
            }
        }
    }

    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : TestAvoid
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::TestAvoid()
{
    //cout << "TestAvoid " << mUnitList.size() << endl;
    for (list<CVectorUnit*>::iterator i1 = mUnitList.begin();
        i1 != mUnitList.end();
        ++i1)
    {
        if ((*i1)->mStatus != EUS_ENY_GOHOME2)
        {
            bool bAvoid = false;
            float MinDist = 32.0;
            list<CVectorUnit*>::iterator inext = i1;

            for (list<CVectorUnit*>::iterator i2 = ++inext;
                i2 != mUnitList.end();
                ++i2)
            {
                float dist = (*i1)->Distance(*i2);

                //cout << "dist=" << dist << endl;

                if (dist < MinDist)
                {
                    if ((*i1)->mStatus != EUS_ENY_AVOID)
                    {
                        (*i1)->mOldStatus = (*i1)->mStatus;
                        (*i1)->mStatus = EUS_ENY_AVOID;
                    }
                    MinDist = dist;
                    (*i1)->SetTarget(*i2);
                    bAvoid = true;
                }
            }
            if (!bAvoid)
            {
                if ((*i1)->mOldStatus != EUS_INVALID)
                {
                    (*i1)->mStatus = (*i1)->mOldStatus;
                    (*i1)->mOldStatus = EUS_INVALID;
                }
            }
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : TestEnemyShoot
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::TestEnemyShoot()
{
    EnemyShotHit();
    if (mGameContext.mShooter != NULL)
    {
        ShotEnemy(mGameContext.mShooter);
        mGameContext.mShooter = NULL;
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : NewDebris
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::NewDebris(float xpos, float ypos, int Fakt, const float* Color, const CVector2<float>& ddir)
{
    CDebrisUnit* du = new CDebrisUnit;

    du->Init(xpos, ypos, Fakt, Color, ddir);
    mDebrisList.push_back(du);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawDebris
//
//
//
// ---------------------------------------------------------------------------


void CSdlThiefs::DrawDebris()
{
    vector<CDebrisUnit*> RemoveVec;

    for (list<CDebrisUnit*>::iterator it = mDebrisList.begin();
        it != mDebrisList.end();
        ++it)
    {
        if (!(*it)->Draw())
        {
            RemoveVec.push_back(*it);
        }
    }

    for (vector<CDebrisUnit*>::iterator iev = RemoveVec.begin();
        iev != RemoveVec.end();
        ++iev)
    {
        mDebrisList.remove(*iev);
        delete *iev;
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
    float Red[] = { 1.0, 0.5, 0.5, 1.0 };

    glColor4f(0.9, 0.8, 0.8, 1.0);

    glPointSize(1.5 * gGlobalScale);
    glLineWidth(1.0 * gGlobalScale);
    glColor4f(0.5, 0.5, 0.52, 1.0);

    DrawUnitList(mBaitList, 0);
    DrawUnitList(mUnitList, CAN_BE_SHOT | CAN_HIT);
    DrawUnitList(mExploList, 0);
    TestPlayerCrash();

    glColor4fv(White);
    DrawUnitList(mShotList, 0);
    glColor4fv(Red);
    DrawUnitList(mEnemyShotList, 0);

/*
 * DrawUnitPoints(mBaitList);
 * DrawUnitPoints(mUnitList);
 */
    glColor4fv(White);
    DrawUnitPoints(mExploList);

    //cout << "RemoveInactive START" << endl;
    RemoveInactive(mUnitList);
    RemoveInactive(mShotList);
    RemoveInactive(mExploList);
    RemoveInactive(mBaitList);
    RemoveInactive(mEnemyShotList);

    {
        const float GruenColor[] = { 0.0, 0.5, 0.0, 0.5 };
        int ScorePos = mGameContext.mPlayField.Width() * 0.6f;

        //,,


        DrawCenterScore(mScore[0], mGameContext.mPlayField.Width() * 0.75f, GruenColor);

        if (mGameContext.mPlayers == 2)
        {
            const float BlauColor[] = { 0.3, 0.3, 0.6, 0.5 };
            DrawCenterScore(mScore[1], mGameContext.mPlayField.Width() * 0.25f, BlauColor);
        }
        DrawLevel();
    }

/*
 *  for (int c = 0; c < 5; c++)
 *  {
 *  mZeichen[c].SetPosition(c * 32 + 100, 300);
 *  mZeichen[c].Draw(0);
 *  }
 */

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

#define SHOT_SPEED			20.0
#define ENEMY_SHOT_SPEED	8.0

void CSdlThiefs::Shot(int PlayerNum)
{
    CPlayerUnit* Player = mPlayer + PlayerNum;

    if (Player->mTimeOut == 0)
    {
        CVectorShot* shot = new CVectorShot;
        mShotList.push_back(shot);

        shot->SetPosition(Player->mShapeParams.mPosition.x, Player->mShapeParams.mPosition.y);
        shot->mDirection.x = Player->mShapeParams.mSinRotation *  SHOT_SPEED;
        shot->mDirection.y = Player->mShapeParams.mCosRotation * -SHOT_SPEED;
        shot->SetShotColor(1.0, 1.0, 1.0, 1.0);
        shot->mPlayerNum = PlayerNum;

        mSdlSound.PlayWav(SOUND_SCHUSS);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : Shot
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ShotEnemy(CVectorUnit* who)
{
    CVectorShot* shot = new CVectorShot;

    mEnemyShotList.push_back(shot);

    shot->SetPosition(who->mShapeParams.mPosition.x, who->mShapeParams.mPosition.y);
    shot->mDirection.x = who->mShapeParams.mSinRotation *  ENEMY_SHOT_SPEED;
    shot->mDirection.y = who->mShapeParams.mCosRotation * -ENEMY_SHOT_SPEED;
    shot->SetShotColor(1.0, 0.0, 0.0, 1.0);

    mSdlSound.PlayWav(SOUND_SCHUSS);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ClearList
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ClearList(list<CVectorUnit*>& ulist)
{
    list<CVectorUnit*>::iterator ie;

    for (ie = ulist.begin(); ie != ulist.end(); ++ie)
    {
        (*ie)->mStatus = EUS_INACTIVE;
    }
    RemoveInactive(ulist);
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
// METHODE       : RunPlayers
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::RunPlayers()
{
    for (int i = 0; i < mGameContext.mPlayers; i++)
    {
        mPlayer[i].Run(&mGameContext);
    }
}


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


    for (list<CVectorUnit*>::iterator ie = mEnemyShotList.begin();
        ie != mEnemyShotList.end();
        ++ie)
    {
        (*ie)->Run(&mGameContext);
    }

    for (list<CVectorUnit*>::iterator ie = mBaitList.begin();
        ie != mBaitList.end();
        ++ie)
    {
        (*ie)->Run(&mGameContext);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : EnemyShotHit
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::EnemyShotHit()
{
    for (list<CVectorUnit*>::iterator ie = mEnemyShotList.begin();
        ie != mEnemyShotList.end();
        ++ie)
    {
        float xpos = (*ie)->PosX();
        float ypos = (*ie)->PosY();

        for (int p = 0; p < mGameContext.mPlayers; p++)
        {
            if (mPlayer[p].mTimeOut == 0)
            {
                if (mPlayer[p].Inside(xpos, ypos))
                {
                    PlayerExplodes(p);
                    (*ie)->Destroy();
                }
            }
        }
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

void CSdlThiefs::BonusScore()
{
    int bl = min(mLevel, 20);

    for (int p = 0; p < mGameContext.mPlayers; p++)
    {
        mScore[p] += bl * mBaitList.size();
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
            CVectorShot* vshot = dynamic_cast<CVectorShot*>(*ie);
            CEnemyUnit* eu = dynamic_cast<CEnemyUnit*>(enemy);

            int pt;
            switch (eu->mEnemyType)
            {
                case ETYPE_THIEF:
                    pt = 5;
                    break;

                case ETYPE_THIEF2:
                    pt = 9;
                    break;

                case ETYPE_FIGHTER:
                    pt = 8;
                    break;

                case ETYPE_BOMBER:
                    pt = 12;
                    break;
            }
            mScore[vshot->mPlayerNum] += pt;


            (*ie)->Destroy();
            enemy->Destroy();

            CVector2<float> expos(xpos, ypos);
            explo->push_back(expos);
            mSdlSound.PlayWav(SOUND_EXPLO1);
            NewDebris(xpos, ypos, 1, (*ie)->GetColor(), enemy->mDirection);

            r = true;
        }
    }
    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ParseMouseRel
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ParseMouseRel(int xrel, int yrel)
{
    float f1 = fabs(float(xrel));
    float f2 = 1.1;
    float frot = pow(f1, f2) / mMouseFaktor;

    //mPlayer[0].AddRotation(-float(xrel) / 7.0);

    if (xrel < 0)
    {
        mPlayer[0].AddRotation(frot);
    }
    else
    {
        mPlayer[0].AddRotation(-frot);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : StartGame
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::StartGame()
{
    mSdlSound.PlayMusic(3);
    mSdlSound.NewMusic(3);
    mGameStatus = EGAMESTATUS_PLAYING;
    ClearList(mUnitList);
    ClearList(mBaitList);
    InitBait();
    NewAttackWave(true);
    mScore[0] = mScore[1] = 0;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ParseKeys
//
//
//
// ---------------------------------------------------------------------------

bool CSdlThiefs::ParseKeys(SDLKey key, bool down)
{
    bool bQuit = -1;

    //cout << "key=" << (int) key << endl;

    if (mGameStatus != EGAMESTATUS_PLAYING)
    {
        switch (key)
        {
            case SDLK_1:

                if (!down)
                {
                    mGameContext.mPlayers = 1;
                    StartGame();
                }
                break;


            case SDLK_2:

                if (!down)
                {
                    mGameContext.mPlayers = 2;
                    StartGame();
                }
                break;

            case SDLK_ESCAPE:

                if (down)
                {
                    bQuit = false;
                }
                break;
        }
    }
    else
    {
        switch (CKeymap::GetMappedKey(key))
        {
            //
            // Spielsteuerung
            //


            case SDLK_9:

                if (down)
                {
                    mScreenSaveMode = !mScreenSaveMode;

                    if (!mScreenSaveMode)
                    {
                        mSaveScreenGL.Write();
                    }
                }


                break;

            case SDLK_ESCAPE:

                if (down)
                {
                    if (mGameStatus == EGAMESTATUS_PLAYING)
                    {
                        GotoWaitStatus();
                    }
                    else
                    {
                        bQuit = false;
                    }
                }
                break;

            //
            // Player 1
            //

            case SDLK_LEFT:
                mPlayer[0].mRotSpeed = 0.9;
                mPlayer[0].mLeftTurn = down;
                break;

            case SDLK_RIGHT:
                mPlayer[0].mRotSpeed = 0.9;
                mPlayer[0].mRightTurn = down;
                break;

            case SDLK_UP:
                mPlayer[0].mMoveForward = down;
                break;

            case SDLK_DOWN:
                mPlayer[0].mMoveBackward = down;
                break;

            case SDLK_DELETE:
            case SDLK_RCTRL:

                if (down)
                {
                    Shot(0);
                }
                break;

            //
            // Player 2
            //

            case SDLK_a:
                mPlayer[1].mRotSpeed = 0.9;
                mPlayer[1].mLeftTurn = down;
                break;

            case SDLK_d:
                mPlayer[1].mRotSpeed = 0.9;
                mPlayer[1].mRightTurn = down;
                break;

            case SDLK_w:
                mPlayer[1].mMoveForward = down;
                break;

            case SDLK_s:
                mPlayer[1].mMoveBackward = down;
                break;

            case SDLK_TAB:
            case SDLK_LCTRL:

                if (down)
                {
                    Shot(1);
                }
                break;
        }
    }
    return bQuit;
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
// METHODE       : GameLoop
//
//
//
// ---------------------------------------------------------------------------

#define FB_WIDTH		960 * 2
#define FB_HEIGHT		540 * 2

//#define USE_FB

void CSdlThiefs::GameLoop()
{
    //cout << "CSdlThiefs::GameLoop" << endl;

#ifdef USE_FB
    if (StaticFramebufferObject == NULL)
    {
        StaticFramebufferObject = new CGL_FramebufferObject;
        StaticFramebufferObject->CreateFramebufferObject(FB_WIDTH, FB_HEIGHT);
    }
#endif

    mGameContext.mPlayField.Set(0, 0, FB_WIDTH, FB_HEIGHT);

#ifdef USE_FB
    StaticFramebufferObject->DrawToFrameBuffer(true);
#endif

    Draw2DObjects();

#ifdef USE_FB
    StaticFramebufferObject->DrawToFrameBuffer(false);
    StaticFramebufferObject->DrawTexture(mXres, mYres);
#endif



    if (mScreenSaveMode)
    {
        mSaveScreenGL.SaveScreen(mGameContext.mPlayField.Width(), mGameContext.mPlayField.Height());
    }
    SDL_GL_SwapBuffers();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : DrawCenterTex
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::DrawCenterPatch(CGL_Patch2d* pat)
{
    const float zspeed = 1.0003f;
    static float nx = 1.0f;
    static float nd = zspeed;


    const float Scale = gGlobalScale * nx;

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    pat->mPos.x = (mGameContext.mPlayField.Width() / 2) - (pat->mWidth / 2 * Scale);
    pat->mPos.y = (mGameContext.mPlayField.Height() / 2)- (pat->mHeight / 2 * Scale);
    pat->DrawScaled(Scale);

    nx *= nd;
    if (nx > 1.5f)
    {
        nd = 1.0f / zspeed;
    }
    if (nx < 1.0f)
    {
        nd = zspeed;
    }

    //pat->Draw();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : GotoWaitStatus
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::GotoWaitStatus()
{
    mGameContext.mPlayers = 0;
    mGameStatus = EGAMESTATUS_WAITING;
    mSdlSound.StopMusic();
    ClearList(mUnitList);
    ClearList(mBaitList);
    NewAttackWave(true);
    InitBait();
    mGameContext.mPlayers = 0;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : Digits
//
//
//
// ---------------------------------------------------------------------------

int CSdlThiefs::Digits(int k)
{
    stringstream mstr;

    mstr << k;
    return mstr.str().size();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : WriteHighScore
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::WriteHighScore()
{
    ofstream of(GsysPath("hiscore.txt"));

    if (of.good())
    {
        of << mHighScore << endl;
        of.close();
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : ReadHighScore
//
//
//
// ---------------------------------------------------------------------------

void CSdlThiefs::ReadHighScore()
{
    ifstream infile(GsysPath("hiscore.txt"));

    if (infile.good())
    {
        infile >> mHighScore;
        infile.close();
    }
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
        if (mExploded)
        {
            glClearColor(0xFF, 0xFF, 0xFF, 0xFF);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.0, 0.0, 0.0, 0.0);
            mExploded = false;
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }


        mOpenGL.StartProjectionView();
        //cout << "CSdlThiefs::Draw2DObjects" << endl;
        if (!mInitEnemies)
        {
            //cout << "InitEnemies START" << endl;
            InitEnemies();
            //cout << "InitEnemies OK" << endl;
        }


        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glLineWidth(1.0);

        //mPlayer1.DrawPoints();

        for (int i = 0; i < mGameContext.mPlayers; i++)
        {
            mPlayer[i].Draw(0);
        }

        RunPlayers();
        RunUnits();
        DrawEnemies();
        DrawDebris();
        TestAvoid();
        TestEnemyShoot();


        if (mGameStatus == EGAMESTATUS_WAITING)
        {
            float RedColor[] = { 1.0, 0.4, 0.4, 0.6 };
            DrawCenterPatch(&mMainTex);

            DrawCenterScore(mHighScore, mGameContext.mPlayField.Width() / 2, RedColor);
        }
        else
        if (mGameStatus == EGAMESTATUS_GAMEOVER)
        {
            DrawCenterPatch(&mGameOverTex);
            if (--mGameOverTime == 0)
            {
                GotoWaitStatus();
            }
        }



        if (mGameContext.mBaitStolen)
        {
            int Music = (10 - mBaitList.size()) + SOUND_MUSIC;
            mGameContext.mBaitStolen = false;
            mSdlSound.NewMusic(Music);

            if (mBaitList.size() == 0)
            {
                if (mGameStatus == EGAMESTATUS_PLAYING)
                {
                    mGameStatus = EGAMESTATUS_GAMEOVER;
                    mGameOverTime = 250;
                    if (mScore[0] > mHighScore)
                    {
                        mHighScore = mScore[0];
                        WriteHighScore();
                    }
                    if (mScore[1] > mHighScore)
                    {
                        mHighScore = mScore[1];
                        WriteHighScore();
                    }
                }
                else
                {
                    GotoWaitStatus();
                }
                mGameContext.mPlayers = 0;
            }
        }

        if (mUnitList.size() == 0)
        {
            BonusScore();
            NewAttackWave();
        }

        if (mGameContext.mShootTime > 0)
        {
            mGameContext.mShootTime--;
        }


        glDisable(GL_BLEND);
    }
}
