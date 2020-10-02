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
// Klasse:    CG3DResourceTooltipp
// Methode:   CG3DResourceTooltipp
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResourceTooltipp::CG3DResourceTooltipp()
{
    mContent = NULL;

    mBoxColor[0] = gGlobals->mTTboxColor;
    mFrameColor[0] = gGlobals->mTTframeColor;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTooltipp
// Methode:   AddResRef
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceTooltipp::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_STRING:

            mContent = dynamic_cast<CG3DResourceString*> (res);
            mContent->mColorRef = gGlobals->mTTfontColor;
            mContent->mOrigColor = gGlobals->mTTfontColor;
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTooltipp
// Methode:   Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceTooltipp::Draw(SG3DDrawParams* dp)
{
    //cout << "CG3DResourceTooltipp::Draw" << endl;

    mBrect.Set(0, 0, UnscaledWidth()+16, UnscaledHeight() + 2);

    if (dp->mPos.x + mBrect.right > gGlobals->mWindowWidth)
    {
        dp->mPos.x = gGlobals->mWindowWidth - mBrect.right;
    }

    if (dp->mPos.y + mBrect.bottom > gGlobals->mWindowHeight)
    {
        dp->mPos.y = gGlobals->mWindowHeight - mBrect.bottom;
    }

    dp->mPos.x -= 8;
    CG3DResourceBox::Draw(dp);
    dp->mPos.x += 8;

    mContent->Draw(dp);
}
