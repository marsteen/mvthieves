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

//---------------------------------------------------------------------------
//
// Klasse:    CGL_RectPatch
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceImage::DrawPatch(int RectNr)
{
    if ((mStatus & CGL_STATUS_ACTIVE) && (mStatus & CGL_STATUS_VISIBLE))
    {
        CVector3<float> Vertex;

        CRectT<int>* rc = mIrect + RectNr;

        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, mTexHandle);

        glBegin(GL_TRIANGLE_STRIP);

        Vertex.x = mPosition.x;
        Vertex.y = mPosition.y;
        Vertex.z = 0;
        glTexCoord2f(left, top);
        glVertex3fv(Vertex.v());

        Vertex.x = mPosition.x + rc->Width();
        Vertex.y = mPosition.y;
        Vertex.z = 0;
        glTexCoord2f(right, top);
        glVertex3fv(Vertex.v());

        Vertex.x = mPosition.x;
        Vertex.y = mPosition.y + rc->Height();
        Vertex.z = 0;
        glTexCoord2f(left, bottom);
        glVertex3fv(Vertex.v());

        Vertex.x = mPosition.x  + rc->Width();
        Vertex.y = mPosition.y + rc->Height();
        Vertex.z = 0;
        glTexCoord2f(right, bottom);
        glVertex3fv(Vertex.v());

        glEnd();

        glDisable(GL_TEXTURE_2D);
    }

/*
 *  for (CGL_PatchChild* pc = mChildList->GetFirst(); pc != NULL; pc = mChildList->GetNext())
 *  {
 *      pc->DrawPatch();
 *  }
 */
}
