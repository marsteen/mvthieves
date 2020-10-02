#ifndef CGL_CAMERA
#define CGL_CAMERA

#include <CVector3T.h>

class CGL_Camera
{
    public:

        CGL_Camera()
        {
            Reset();
        }


        void Reset();

        void operator=(CGL_Camera& cam)
        {
            mStandort = cam.mStandort;
            mVispoint = cam.mVispoint;
            mUpvector = cam.mUpvector;
        }


        void LookAt();

        CVector3<float> mStandort;
        CVector3<float> mVispoint;
        CVector3<float> mUpvector;
};

#endif
