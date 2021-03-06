#ifndef SGAMECONTEXT_H
#define SGAMECONTEXT_H

#include <list>
#include <CRectT.h>
#include "CVectorUnit.h"

struct SGameContext
{
    SGameContext()
    {
        mPlayer = NULL;
        mBaitList = NULL;
        mEnemyMaxSpeed = 2.2;
        mBaitStolen = false;
        mShooter = NULL;
        mShootTime = 25;
    }


    float CenterX() { return mPlayField.Width() / 2.0; }
    float CenterY() { return mPlayField.Height() / 2.0; }

    CRectT<float>				mPlayField;
    bool						mLeftButton;
    bool						mRightButton;

    float						mEnemyMaxSpeed;
    CVectorUnit*				mPlayer;
    int							mPlayers;
    std::list<CVectorUnit*>*	mBaitList;
    CVectorUnit*				mShooter;
    int							mShootTime;
    int							mBaitStolen;
};

#endif
