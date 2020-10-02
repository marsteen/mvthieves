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

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceRoot
// Methode:		CalcAlignPosition
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceRoot::CalcAlignPosition()
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        cld->CalcAlignPosition(&mDrawParams);
    }
}
