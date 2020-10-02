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
#include <CHexConv.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>


extern CG3DGlobals* gGlobals;

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceBox
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


CG3DResourceBox::CG3DResourceBox()
{
    mBoxColor[0] = NULL;
    mFrameColor[0] = NULL;
    mBoxColor[1] = NULL;
    mFrameColor[1] = NULL;
    mBoxColor[2] = NULL;
    mFrameColor[2] = NULL;
    mStdColor = -1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceBox
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceBox::Draw(SG3DDrawParams* dp)
{
    DrawBox(0, dp);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceBox
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceBox::DrawBox(int RectNr, SG3DDrawParams* dp)
{
    if ((mBoxColor[RectNr] != NULL) || (mFrameColor[RectNr] != NULL))
    {
        if (mBrect.Width() > 0)
        {
            GLfloat WhiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };
            CVector3<float> Vertex;

            float ScaledWidth = mBrect.Width() * dp->mScale.x;
            float ScaledHeight = mBrect.Height() * dp->mScale.y;

            float x = dp->mPos.x;
            float y = AdjustY(dp->mPos.y) - ScaledHeight;

            CRectT<float> rc;
            float xc = x + 0.5;
            float yc = y + 0.5;
            rc.Set(xc, yc, xc + ScaledWidth, yc + ScaledHeight);

            if (mBoxColor[RectNr] != NULL)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glColor4fv(mBoxColor[RectNr]);

                glBegin(GL_TRIANGLE_STRIP);
                Vertex.x = rc.left;
                Vertex.y = rc.top;
                glVertex2fv(&Vertex.x);

                Vertex.x = rc.right;
                Vertex.y = rc.top;
                glVertex2fv(&Vertex.x);

                Vertex.x = rc.left;
                Vertex.y = rc.bottom;
                glVertex2fv(&Vertex.x);

                Vertex.x = rc.right;
                Vertex.y = rc.bottom;
                glVertex2fv(&Vertex.x);

                glEnd();
            }

            if (mFrameColor[RectNr] != NULL)
            {
                glColor4fv(mFrameColor[RectNr]);
                glBegin(GL_LINE_LOOP);

                // 1

                Vertex.x = rc.left;
                Vertex.y = rc.top;
                glVertex2fv(&Vertex.x);

                Vertex.x = rc.right;
                Vertex.y = rc.top;
                glVertex2fv(&Vertex.x);

                // 2

                Vertex.x = rc.right;
                Vertex.y = rc.bottom;
                glVertex2fv(&Vertex.x);

                Vertex.x = rc.left;
                Vertex.y = rc.bottom;
                glVertex2fv(&Vertex.x);


                glEnd();
            }
            glColor4fv(WhiteMaterial);
            glDisable(GL_BLEND);
        }
    }
    CG3DResource::Draw(dp);

    /*
     * else
     * {
     *  mPosition = *Pos;
     * }
     */
}
