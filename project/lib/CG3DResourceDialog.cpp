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

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceDialog
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceDialog::Draw(SG3DDrawParams* dp)
{
    if (mImgRef != NULL)
    {
        mImgRef->DrawImage(0, dp);
    }
    if (mBoxRef != NULL)
    {
        mBoxRef->Draw(dp);
    }
    CG3DResource::Draw(dp);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceDialog
// Methode:		AddResRef
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceDialog::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_IMAGE:

            mImgRef = dynamic_cast<CG3DResourceImage*>(res);
            break;

        case ERTYPE_BOX:

            mBoxRef = dynamic_cast<CG3DResourceBox*>(res);
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceDialog
// Methode:		UnscaledWidth
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


int CG3DResourceDialog::UnscaledWidth()
{
    if (mImgRef != NULL)
    {
        return mImgRef->Width(mStdImage);
    }
    if (mBoxRef != NULL)
    {
        return mBoxRef->UnscaledWidth();
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceDialog
// Methode:		UnscaledHeight
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceDialog::UnscaledHeight()
{
    if (mImgRef != NULL)
    {
        return mImgRef->Height(mStdImage);
    }
    if (mBoxRef != NULL)
    {
        return mBoxRef->UnscaledHeight();
    }
    return 0;
}
