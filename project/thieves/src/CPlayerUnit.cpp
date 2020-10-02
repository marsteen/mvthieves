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
#include <string>

using namespace std;

#include "SGameContext.h"
#include "CPlayerUnit.h"

extern float gGlobalScale;

// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : CPlayerUnit
//
//
//
// ---------------------------------------------------------------------------

CPlayerUnit::CPlayerUnit()
{
    mStatus = EUS_PLAYER_NOPOS;

    mLeftTurn = false;
    mRightTurn = false;
    mMoveForward = false;
    mMoveBackward = false;
    mTimeOut = 0;
}


#define PLAYER_SPEED    4

// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : Draw
//
//
//
// ---------------------------------------------------------------------------

void CPlayerUnit::Draw(int Mode)
{
    if (mTimeOut == 0)
    {
        CVectorUnit::Draw(0);
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : Reset
//
//
//
// ---------------------------------------------------------------------------

void CPlayerUnit::Reset()
{
    mLeftTurn = false;
    mRightTurn = false;
    mMoveForward = false;
    mMoveBackward = false;
    mTimeOut = 0;
    mDirection.Set(0, 0);
    mSpeed = 0;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : MoveForward
//
//
//
// ---------------------------------------------------------------------------

void CPlayerUnit::MoveForward()
{
    float SpeedLimit = gGlobalScale * 2 * PLAYER_SPEED;
    float SpeedFakt = gGlobalScale * 0.1 * PLAYER_SPEED;

    if (mMoveForward)
    {
        mSpeed += SpeedFakt;
        if (mSpeed > SpeedLimit)
        {
            mSpeed = SpeedLimit;
        }
    }
    else
    if (mMoveBackward)
    {
        mSpeed -= SpeedFakt;
        if (mSpeed < -SpeedLimit)
        {
            mSpeed = -SpeedLimit;
        }
    }
    else
    {
        if (mSpeed > 0)
        {
            mSpeed -= 0.05 * gGlobalScale;
            if (mSpeed < 0)
            {
                mSpeed = 0;
            }
        }
        else
        if (mSpeed < 0)
        {
            mSpeed += 0.05 * gGlobalScale;
            if (mSpeed > 0)
            {
                mSpeed = 0;
            }
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : Move
//
//
//
// ---------------------------------------------------------------------------

void CPlayerUnit::Move()
{
    if (mLeftTurn)
    {
        if (mRotSpeed < 6.0)
        {
            mRotSpeed *= 1.05;
        }
        AddRotation(mRotSpeed);
    }
    else
    if (mRightTurn)
    {
        if (mRotSpeed < 6.0)
        {
            mRotSpeed *= 1.05;
        }
        AddRotation(-mRotSpeed);
    }
    MoveForward();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CPlayerUnit
// METHODE       : Run
//
//
//
// ---------------------------------------------------------------------------

void CPlayerUnit::Run(SGameContext* gco)
{
    if (mTimeOut == 0)
    {
        if (mStatus == EUS_PLAYER_NOPOS)
        {
            if (mNumber == 0)
            {
                SetPosition(gco->CenterX() * 1.5, gco->CenterY());
            }
            else
            {
                SetPosition(gco->CenterX() * 0.5, gco->CenterY());
            }
            mStatus = EUS_PLAYER_RUN;
            //cout << "Player X=" << PosX() << " Y=" << PosY() << endl;
        }



        CVector2<float> OldPos = mShapeParams.mPosition;
        mDirection.x = mSpeed * mShapeParams.mSinRotation;
        mDirection.y = -mSpeed * mShapeParams.mCosRotation;
        mShapeParams.mPosition += mDirection;


        if (!gco->mPlayField.InRect(mShapeParams.mPosition))
        {
            mShapeParams.mPosition = OldPos;
            mSpeed = -mSpeed / 2;
        }
        Move();
    }
    else
    {
        if (--mTimeOut == 0)
        {
            mStatus = EUS_PLAYER_NOPOS;
            Reset();
            Run(gco);
            Draw(0);
        }
    }
}
