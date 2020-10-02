//---------------------------------------------------------------------------
//
// Klasse:	CGL_Object
//
//---------------------------------------------------------------------------

#ifndef CGL_OBJECT
#define CGL_OBJECT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CGL_Texture.h"
#include <CVector3T.h>

enum ECGL_STATUS
{
    CGL_STATUS_ALPHA = 32
};

#define DEG_TO_RAD(x)		((x) / 57.29577951)
#define RAD_TO_DEG(x)		((x) * 57.29577951)

class CGL_Object : public CGL_Texture
{
    public:

        void Init();
        void DeleteDisplayList();
        virtual void MakeObject();
        virtual void Draw();

        CGL_Object();
        ~CGL_Object();

    protected:

        void SetNormalVertex(CVector3<float>* Origin, CVector3<float>* cf);

        GLuint mDisplayListHandle;
        GLfloat mMaterial[4];
        unsigned int mStatus;
};

#endif
