//---------------------------------------------------------------------------
//
// Klasse:	CGL__GLU_Sphere
//
//---------------------------------------------------------------------------

#ifndef CGL_GLU_SPHERE
#define CGL_GLU_SPHERE

#include "CGL_GLU_Object.h"

class CGL_GLU_Sphere : public CGL_GLU_Object
{
    public:

        CGL_GLU_Sphere()
        {
            mUseTexture = false;
        }


        void CreateSphere(float Radius, int Slices, int Stacks, float* DiffuseMaterial);
        void CreateTexturedSphere(float Radius, int Slices, int Stacks, const char* TextureFile, const char* AlphaFile);
        void CreateTexturedInsideSphere(float Radius, int Slices, int Stacks, char* TextureFile, char* AlphaFile);
        void MakeObject();

    protected:


        float mRadius;
        int mSlices;
        int mStacks;
        float mDiffuseMaterial[4];
        float mAmbientMaterial[4];
        bool mUseTexture;
};

#endif
