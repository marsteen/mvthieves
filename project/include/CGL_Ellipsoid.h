#ifndef CGL_ELLIPSOID
#define CGL_ELLIPSOID

#include "CGL_Patch.h"

class CGL_Ellipsoid : public CGL_Patch
{
    public:

        void CreateEllipsoid(int Sseg, float Srad, int Tseg, float Trad, float* DiffuseMaterial);

        void DrawWireFrame();

        void MakeObject();

        CGL_Ellipsoid();

        float mXrot;
        float mYrot;


    protected:

        void SetVertex(CVector3<float>* Origin, CVector3<float>* cf);

        int mTCircleSegs;
        int mSCircleSegs;

        float mDiffuseMaterial[4];
        float mAmbientMaterial[4];
        bool mUseTexture;
};

class CGL_EllipsoidPatched : public CGL_Ellipsoid
{
    public:

        CGL_EllipsoidPatched()
        {
            mPhase = 0;
            mPhaseCount = 0;
            mSpeed = 0;
            mSpeedCount = 0;
            mTexHandleList = NULL;
        }


        void Delete();
        void SetMultiTextures(unsigned int* TexHandles, int PatchesX, int PatchesY);
        void SetMotionTextures(unsigned int* TexHandles, int PatchesX, int PatchesY);
        char* GetFilename(int name);
        void SetPatches(int PatchesX, int PatchesY);
        void DrawPatch(int n, unsigned int TexHandle, GLfloat Anisotropic);
        void DrawPatch(int n);
        void DrawWithoutTexture(int Mode);
        void DrawWithoutTexture(int Mode, int* SegList);
        void MakePatch(int PatchX, int PatchY);
        void DrawPointSegment(int PatchNr, int pfaktor = 16);
        void GetLongLati(int Name, float* Long, float* Lati);
        void GetSegmentLongLati(int SegmentNr, float* Long, float* Lati, float* ns, float* nt);

        void NextNumXY(int n, int* nx, int* ny);

        void MakeObject();
        void DrawSingleSegment(int SegmentNr, float Anisotropic);
        void Init();
        void Draw();

        void InitDisplayList();
        void DrawDisplayList();
        void DrawDisplayListMotion();

        unsigned int* mTexHandleList;

        //SAngleList*   mAngleList;
        void Animate();

        float mRotY;
        int mPhase;                 // Zaehler für Animationsphasen
        int mPhaseCount;            // Anzahl der Animationsphasen
        int mSpeed;
        int mSpeedCount;

    protected:

        int mPatchesX;
        int mPatchesY;
        int mNumPatches;
        int mPointsRS;
        int mPointsRT;
        float mStepRS;
        float mStepRT;
        float mStartS;
        float mStartT;
        float mWidthS;  // Weite in Grad (rad)
        float mHeightT; // Hoehe in Grad (rad)


        char** mTextureFiles;
};

#endif // CGL_ELLIPSOID
