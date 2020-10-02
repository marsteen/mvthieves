#ifndef SSHAPEPARAMS_H
#define SSHAPEPARAMS_H

#include <vector>
#include <CVector3T.h>

struct SShapeParams
{
    public:

        SShapeParams();

        void AddRotation(float r);
        void SetRotation(float r);

        std::string Print() const;

        CVector2<float> mPosition;
        CVector2<float> mOrigin;
        float			mRotation;
        float			mSinRotation;
        float			mCosRotation;
        float			mScale;
};

#endif
