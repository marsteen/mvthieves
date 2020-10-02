//---------------------------------------------------------------------------
//
// Klasse:	CGL_GLU_Object
//
//---------------------------------------------------------------------------

#ifndef CGL_GLU_OBJECT
#define CGL_GLU_OBJECT

#include <CGL_Object.h>

class CGL_GLU_Object : public CGL_Object
{
    protected:

        GLUquadricObj* mQuadObj;

        CGL_GLU_Object()
        {
            mQuadObj = gluNewQuadric();
        }


        ~CGL_GLU_Object()
        {
            gluDeleteQuadric(mQuadObj);
        }


        void SetParams(int Orient, int DrawStyle, int Normal, bool TextCoord);
};

#endif
