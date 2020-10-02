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

void CG3DResourceClipframe::Draw(SG3DDrawParams* dp)
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(mRect.left, mRect.top, mRect.Width(), mRect.Height());
    CG3DResource::Draw(dp);
    glDisable(GL_SCISSOR_TEST);
}
