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
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceLine::Draw(SG3DDrawParams* dp)
{
    //  cout << "CG3DResourceLine::Draw " << Pos->x << " " << Pos->y << endl;

    CVector3<float> LineStart;
    CVector3<float> LineEnd;

    LineStart.x = mLineStart.x + dp->mPos.x;
    LineStart.y = AdjustY(mLineStart.y + dp->mPos.y);
    LineStart.z = 0;
    LineEnd.x = (mLineEnd.x * dp->mScale.x) +  dp->mPos.x;
    LineEnd.y = AdjustY((mLineEnd.y * dp->mScale.y) +  dp->mPos.y);
    LineEnd.z = 0;

    //cout << "Color=" << mColor[0] << " " << mColor[1] << " " << mColor[2] << endl;

    glColor4fv(mColor);
    glBegin(GL_LINES);
    glVertex2fv(&LineStart.x);
    glVertex2fv(&LineEnd.x);
    glEnd();
    glColor4f(1.0, 1.0, 1.0, 1.0);
}


#if 0
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

void CG3DResourceImage::DrawImage(int RectNr, CVector2<float>* Pos, CVector2<float>* Scale)
{
    CRectT<int>* rc = mIrect + RectNr;

    if (rc->Width() > 0)
    {
        CRectT<float>* tc = mTrect + RectNr;

        GLfloat WhiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };
        CVector3<float> Vertex;

        float ScaledWidth = rc->Width() * Scale->x;
        float ScaledHeight = rc->Height() * Scale->y;

        float x = Pos->x;
        float y = gGlobals->mWindowHeight - Pos->y - ScaledHeight;

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


#endif
