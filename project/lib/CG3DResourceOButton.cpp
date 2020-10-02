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


CG3DResourceOButton::CG3DResourceOButton() : CG3DResourceButton()
{
    mProperties.Clr(EBTPROP_ROLLOVER);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceOButton
// Methode:		GetDropImagePos
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceOButton::GetDropImagePos(SG3DDrawParams* dp, SG3DDrawParams* cdp)
{
    cdp->mPos.x = dp->mPos.x + (mImgRef->Width(0) * dp->mScale.x);
    cdp->mPos.y = dp->mPos.y;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceOButton
// Methode:		SetOptionParams
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceOButton::SetOptionParams(float x, float y, float sx, float sy)
{
    mContChild.mOrigParams.mPos.Set(x, y);
    mContChild.mOrigParams.mScale.Set(sx, sy);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceOButton
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceOButton::Draw(SG3DDrawParams* dp)
{
    DrawButton(dp);

    if (mDropImageRef != NULL)
    {
        SG3DDrawParams DropImgParams;
        GetDropImagePos(dp, &DropImgParams);
        mDropImageRef->DrawImage(mBtStatus, &DropImgParams);
    }



/*
 *  if (mOptionField != NULL)
 *  {
 *      CG3DResChild   cld;
 *      SG3DDrawParams cdp;
 *
 *      cld.mOrigParams.mPos   = mOptionPos;
 *      cld.mOrigParams.mScale = mOptionScale;
 *      dp->mParentWidth = Width(dp);
 *      dp->mParentHeight = Height(dp);
 *
 *      cld.CalcAlignPosition(dp);
 *      mOptionField->Draw(&cld.mDrawParams);
 *  }
 */

    CG3DResource::Draw(dp);

    if (mContent != NULL)
    {
        mContChild.mRef = mContent->mRef;
        mContChild.mStatus = mContent->mStatus;
        mContChild.mAlignment = mContent->mAlignment;
        mContChild.mStretchMode = mContent->mStretchMode,

        //cout << "Option " << mName << " OX=" << mContChild.mOrigParams.mPos.x
        //     << " OY=" << mContChild.mOrigParams.mPos.y << endl;

        mContChild.Draw(dp);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		Reset
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceOButton::Reset(int mode)
{
    switch (mode)
    {
        case ERESET_INIT:

            for (list<CG3DResRef>::iterator ref = mRefs.begin();
                ref != mRefs.end();
                ref++)
            {
                if (ref->mRef->mType == ERTYPE_BUTTON)
                {
                    CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*> (ref->mRef);
                    bt->SetOButton(this);
                    bt->SetProperty(EBTPROP_TEXTROLLOVER, false);
                }
            }
            break;
    }
    CG3DResourceButton::Reset(mode);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceOButton::Inside(SG3DDrawParams* dp, CVector2<int>* Mouse)
{
    if (CG3DResourceButton::Inside(dp, Mouse))
    {
        return true;
    }

    if (mDropImageRef != NULL)
    {
        SG3DDrawParams DropImgParams;
        GetDropImagePos(dp, &DropImgParams);
        if (mDropImageRef->InsideImage(mStdImage, &DropImgParams, Mouse))
        {
            throw(CInsideObject(this));
            return true;
        }
        else
        {
            Reset(ERESET_ROLLOVER);
        }
    }

    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		AddResRef
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceOButton::AddResRef(CG3DResource* res)
{
    if (mImgRef == NULL)
    {
        CG3DResourceButton::AddResRef(res);
    }
    else
    {
        switch (res->mType)
        {
            case ERTYPE_IMAGE:

                mDropImageRef = dynamic_cast<CG3DResourceImage*>(res);
                break;

            default:

                CG3DResourceButton::AddResRef(res);
                break;
        }
    }
}
