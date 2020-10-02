#ifndef CVECTORUNIT_H
#define CVECTORUNIT_H

#include <string>
#include "CVectorShape.h"

class SGameContext;

//---------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------

enum EUnitStatus
{
    EUS_INVALID,
    EUS_INACTIVE,      // All: inactive

    // Enemy states

    EUS_ENY_START,      // Enemy: Start
    EUS_ENY_GOBAIT,     // Enemy: go for the bait
    EUS_ENY_GOHOME1,    // Enemy: go home with bait
    EUS_ENY_GOHOME2,    // Enemy: go home with bait
    EUS_ENY_ATTACK,     // Enemy: attack the player
    EUS_ENY_CIRCLE,     // Enemy: run in circles
    EUS_ENY_AVOID,      // Enemy: avoid target


    // Bait states

    EUS_BAIT_STAY,      // Bait:  stay here
    EUS_BAIT_HIJACKED,  // Bait:  hijacked (moving)

    // Player states

    EUS_PLAYER_NOPOS,   // No position
    EUS_PLAYER_RUN      // Normal
};

//---------------------------------------------------------------------------
//
//
//---------------------------------------------------------------------------

class CVectorUnit
{
    public:

        CVectorUnit()
        {
            mVectorShape = NULL;
            mStatus = EUS_INVALID;
            mOldStatus = EUS_INVALID;
            mDirection.Set(0, 0);
            mSpeed = 0;
        }


        virtual ~CVectorUnit() { }

        void SetPosition(float x, float y);
        void SetColor(float r, float g, float b, float a);
        void SetColorPtr(const float* Color, float PointStrenght);
        const float* GetColor();
        void AddPosX(float x);
        void AddPosY(float x);
        void AddRotation(float r);

        float PosX() const { return mShapeParams.mPosition.x; }
        float PosY() const { return mShapeParams.mPosition.y; }
        std::string PrintShapeParams() const;

        virtual void DrawPoints();
        virtual void Draw(int Mode);
        virtual void Run(SGameContext* gac);
        virtual void Destroy();
        virtual void SetTarget(const CVectorUnit* vu);

        bool Inside(CVectorUnit& vu);
        bool Inside(float x, float y);
        float Distance(const CVectorUnit* vu) const;
        void DirOffset(float Offset, CVector2<float>* OffPos);

        CVectorShape* mVectorShape;
        SShapeParams mShapeParams;
        EUnitStatus mStatus;
        EUnitStatus mOldStatus;
        CVector2<float> mDirection;
        float mSpeed;
};

#endif
