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
extern CG3DResourceList* gResourceList;

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		CG3DResourceButton
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResourceButton::CG3DResourceButton()
{
    mContent = NULL;
    mBoxRef = NULL;
    mStringColor = NULL;
    mBtStatus = EBTSTAT_NORMAL;
    mObutton = NULL;
    mJoinRef = NULL;
    mProperties = EBTPROP_ROLLOVER | EBTPROP_TEXTROLLOVER | EBTPROP_AUTOGROUP;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		SetStatusRollover
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::SetStatusRollover()
{
    if (mProperties.Get(EBTPROP_ROLLOVER))
    {
        if (mBtStatus != EBTSTAT_ACTIVE)
        {
            mBtStatus = EBTSTAT_ROLLOVER;
        }
    }
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

bool CG3DResourceButton::Inside(SG3DDrawParams* dp, CVector2<int>* Mouse)
{
    //cout << "Buttonname=" << mName << endl;

    bool i = false;

    if (mImgRef != NULL)
    {
        i = mImgRef->InsideImage(mStdImage, dp, Mouse);

        if (mImgRef->Width(1) > 0)
        {
            SetStatusRollover();
        }
    }

    if (!i)
    {
        if (mBoxRef != NULL)
        {
            i = mBoxRef->Inside(dp, Mouse);
            SetStatusRollover();
        }
    }

    if (!i)
    {
        if (mContent != NULL)
        {
            i = mContent->mRef->Inside(dp, Mouse);
            SetStatusRollover();
        }
    }

    if (i)
    {
        throw(CInsideObject(this));
    }
    else
    {
        Reset(ERESET_ROLLOVER);
    }

    return i;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		UnscaledWidth
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceButton::UnscaledWidth()
{
    if (mImgRef != NULL)
    {
        return mImgRef->UnscaledWidth();
    }
    else
    if (mBoxRef != NULL)
    {
        return mBoxRef->UnscaledWidth();
    }
    else
    if (mContent != NULL)
    {
        return mContent->mRef->UnscaledWidth();
    }
    else
    {
        //cout << "***** CG3DResourceButton::UnscaledWidth Empty Button:" << mName << endl;
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		UnscaledHeight
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceButton::UnscaledHeight()
{
    if (mImgRef != NULL)
    {
        return mImgRef->UnscaledHeight();
    }
    else
    if (mBoxRef != NULL)
    {
        return mBoxRef->UnscaledHeight();
    }
    else
    if (mContent != NULL)
    {
        return mContent->mRef->UnscaledHeight();
    }
    else
    {
        //cout << "***** CG3DResourceButton::UnscaledHeight Empty Button:" << mName << endl;
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		ResetStringColor
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::ResetStringColor()
{
    if (mProperties.Get(EBTPROP_TEXTROLLOVER))
    {
        if (mContent != NULL)
        {
            mContent->mRef->Reset(ERESET_ROLLOVER);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		DrawButton
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::DrawButton(SG3DDrawParams* dp)
{
    //cout << "CG3DResourceButton::DrawButton NAME=" << mName << endl;

    if (mImgRef != NULL)
    {
        mImgRef->DrawImage(mBtStatus, dp);
    }
    else
    if (mBoxRef != NULL)
    {
        mBoxRef->DrawBox(mBtStatus, dp);
    }


    if ((mContent != NULL) && mProperties.Get(EBTPROP_TEXTROLLOVER))
    {
        if (mContent->mRef->mType == ERTYPE_STRING)
        {
            CG3DResourceString* StrRef = dynamic_cast<CG3DResourceString*> (mContent->mRef);
            if (mBtStatus == EBTSTAT_ROLLOVER)
            {
                StrRef->mColorRef = gGlobals->mRolloverColor;
            }
            else
            {
                StrRef->mColorRef = mStringColor;
            }
        }
    }
    //cout << "OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::Draw(SG3DDrawParams* dp)
{
    CG3DResource::Draw(dp);
    DrawButton(dp);

    if (mContent != NULL)
    {
        mContent->Draw(dp);
    }
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

void CG3DResourceButton::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_IMAGE:

            mImgRef = dynamic_cast<CG3DResourceImage*>(res);

            if (!mProperties.Get(EBTPROP_KEEPTXTROLL))
            {
                mProperties.Clr(EBTPROP_TEXTROLLOVER);
            }
            break;

        case ERTYPE_BOX:

            mBoxRef = dynamic_cast<CG3DResourceBox*>(res);
            break;

        case ERTYPE_BUTTON:

            mJoinRef = dynamic_cast<CG3DResourceButton*>(res);
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		KlappEin
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::KlappEin()
{
    //if (mProperties.Get(EBTPROP_KLAPPBT))
    {
        SetButtonStatus(EBTSTAT_NORMAL);
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

void CG3DResourceButton::Reset(int Mode)
{
    switch (Mode)
    {
        case ERESET_ROLLOVER:

            if (mBtStatus == EBTSTAT_ROLLOVER)
            {
                mBtStatus = EBTSTAT_NORMAL;
            }
            ResetStringColor();
            break;

        case ERESET_DEACTIVATE:

            KlappEin();
            Reset(ERESET_ROLLOVER);
            break;
    }
}


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

void CG3DResourceButton::Show()
{
    CG3DResource::Show();
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		GetGroupParent
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::GetGroupParent(CG3DResource* pr)
{
    if (mProperties.Get(EBTPROP_AUTOGROUP))
    {
        if (pr != NULL)
        {
            if ((pr->mType == ERTYPE_DIALOG) ||
                (pr->mType == ERTYPE_ROOT))

            {
                mGroupParent = pr;
            }
            else
            {
                if (pr->mParent != NULL)
                {
                    GetGroupParent(pr->mParent);
                }
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		Adjust
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::Adjust(SG3DDrawParams* dp)
{
    //cout << "CG3DResourceButton::Adjust " << mName << endl;

    if (mContent != NULL)
    {
        CG3DResourceString* StrRef = dynamic_cast<CG3DResourceString*> (mContent->mRef);



        if (StrRef != NULL)
        {
            //cout << "StrRef=" << StrRef->mName << endl;
            mStringColor = StrRef->mColorRef;
        }
    }
    //cout << "1" << endl;

    if (mImgRef != NULL)
    {
        if (mImgRef->Width(0) == 0)
        {
            //cout << "  mStdImage=1"<< endl;
            mStdImage = 1;
        }
    }
    //cout << "2" << endl;


    GetGroupParent(mParent);

    //cout << "3" << endl;

    CG3DResource::Adjust(dp);

    //cout << "CG3DResourceButton::Adjust OK " << mName << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		SetButtonStatus
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::SetButtonStatus(EG3DButtonStatus bst)
{
    if (mImgRef != NULL)
    {
        if (mImgRef->Width(bst) != 0)
        {
            mBtStatus = bst;
        }
    }
    else
    if (mBoxRef != NULL)
    {
        if (mBoxRef->HasStatus(bst))
        {
            mBtStatus = bst;
        }
    }

    switch (bst)
    {
        case EBTSTAT_NORMAL:

            mProperties.Clr(EBTPROP_ACTIVE);
            break;

        case EBTSTAT_ACTIVE:

            mProperties.Set(EBTPROP_ACTIVE);
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		ClickAction
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::ClickAction()
{
    //cout << "Button ClickAction: " << mName << " BtStatus=" << mBtStatus << endl;

    if (mProperties.Get(EBTPROP_ACTIVE))
    {
        SetButtonStatus(EBTSTAT_NORMAL);

        Deactivate();
        ResetStringColor();
    }
    else
    {
        if (mObutton != NULL)
        {
            //cout << "mObutton=" << mObutton->mName << endl;

            mObutton->SetContent(mContent);
            mObutton->Activate(false);
            mBtStatus = EBTSTAT_NORMAL;
            mObutton->mBtStatus = EBTSTAT_NORMAL;
            mObutton->SetProperty(EBTPROP_ACTIVE, false);
        }
        else
        {
            if (!mProperties.Get(EBTPROP_NOACTIVE))
            {
                //cout << "!!! Activate" << endl;
                SetButtonStatus(EBTSTAT_ACTIVE);
                DisableRadioGroup();
                Activate(true);
                SetParents(mParent);
            }
            else
            {
                Reset(ERESET_DEACTIVATE);
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceButton
// Methode:		DisableRadioGroup
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceButton::DisableRadioGroup()
{
    if (mGroup >= 0)
    {
        mGroupParent->DeactivateButtonGroup(this);
    }
}
