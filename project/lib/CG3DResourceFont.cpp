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

extern CG3DGlobals* gGlobals;

void CG3DResourceFont::Adjust(SG3DDrawParams* dp)
{
    //cout << "+++++ CG3DResourceFont::Adjust wh=" << mWindowHeight << endl;

    mWindowHeight = gGlobals->mWindowHeight;
}
