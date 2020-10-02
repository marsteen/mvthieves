//---------------------------------------------------------------------------
//
// PROJECT : Die Planeten
//
//
// AUTOR   : Martin Steen
//           email: martin@martin-steen.de
//
//
//----------------------------------------------------------------------------

#include "CG3DResourceList.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

extern CG3DGlobals* gGlobals;

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceImage
// Methode:		AddResRef
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceImage::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_TABLEAU:

            mTabRef = dynamic_cast<CG3DResourceTableau*>(res);
            CalcTextureKoors();
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceImage
// Methode:		SetRef
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceImage::CalcTextureKoors()
{
    for (int i = 0; i < 3; i++)
    {
        CRectT<int>* irc = mIrect + i;

        if (irc->Width() > 0)
        {
            CRectT<float>* trc = mTrect + i;

            trc->left = (float)irc->left / mTabRef->mTexWidth;
            trc->right = (float)irc->right / mTabRef->mTexWidth;
            trc->bottom = (float)irc->top / mTabRef->mTexHeight;
            trc->top = (float)irc->bottom / mTabRef->mTexHeight;

            /*
             *    cout << "Texture Koors = "
             *         << trc->left << " "
             *         << trc->top << " "
             *         << trc->right << " "
             *         << trc->bottom << endl;
             */
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceImage
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceImage::Draw(SG3DDrawParams* dp)
{
    DrawImage(0, dp);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceImage
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceImage::DrawImage(int RectNr, SG3DDrawParams* dp)
{
    CRectT<int>* rc = mIrect + RectNr;

    if (rc->Width() > 0)
    {
        CRectT<float>* tc = mTrect + RectNr;

        GLfloat WhiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };
        CVector3<float> Vertex;

        float ScaledWidth = rc->Width() * dp->mScale.x;
        float ScaledHeight = rc->Height() * dp->mScale.y;

        float x = dp->mPos.x;
        float y = AdjustY(dp->mPos.y) - ScaledHeight;

        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindTexture(GL_TEXTURE_2D, mTabRef->mTexHandle);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, WhiteMaterial);
        glColor3f(1.0, 1.0, 1.0);

        glBegin(GL_TRIANGLE_STRIP);
        Vertex.x = x;
        Vertex.y = y;
        Vertex.z = 0;
        glTexCoord2f(tc->left, tc->top);
        glVertex3fv(&Vertex.x);

        Vertex.x = x + ScaledWidth;
        Vertex.y = y;
        Vertex.z = 0;
        glTexCoord2f(tc->right, tc->top);
        glVertex3fv(&Vertex.x);

        Vertex.x = x;
        Vertex.y = y + ScaledHeight;
        Vertex.z = 0;
        glTexCoord2f(tc->left, tc->bottom);
        glVertex3fv(&Vertex.x);

        Vertex.x = x + ScaledWidth;
        Vertex.y = y + ScaledHeight;
        Vertex.z = 0;
        glTexCoord2f(tc->right, tc->bottom);
        glVertex3fv(&Vertex.x);

        glEnd();

        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceImage
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceImage::InsideImage(int RectNr, SG3DDrawParams* dp, CVector2<int>* Mouse)
{
    CRectT<float> rt;

    rt.Set(dp->mPos.x, dp->mPos.y, dp->mPos.x + Width(RectNr) * dp->mScale.x, dp->mPos.y + Height(RectNr) * dp->mScale.y);
    return rt.InRect(Mouse->x, Mouse->y);
}
