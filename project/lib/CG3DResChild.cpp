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
#include "CG3DReslistInterface.h"

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResChild
// Methode:		CG3DResChild
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResChild::CG3DResChild()
{
    mAlignment = 0;
    mStretchMode = ESTRMODE_NONE;
    mStatus = ESTAT_ACTIVE;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		GetMaxY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::GetMaxY(SG3DDrawParams* ParentDp, float* MaxY)
{
    for (list<CG3DResChild>::iterator cld = mRef->mChilds.begin();
        cld != mRef->mChilds.end();
        cld++)
    {
        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            SG3DDrawParams TmpPa;

            cld->GetParams(&TmpPa, ParentDp);

            /*
             *    cout << cld->mRef->mName
             *         << " Y=" << TmpPa.mPos.y
             *         << " OY=" << cld->mOrigParams.mPos.y
             *         << " H=" << cld->mRef->Height(&TmpPa) << endl;
             */

            float CldMaxY = TmpPa.mPos.y + cld->mRef->Height(&TmpPa);
            if (CldMaxY > *MaxY)
            {
                *MaxY = CldMaxY;
            }
            cld->GetMaxY(&TmpPa, MaxY);
        }
    }
}


#if 0

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		GetMaxY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::GetMaxY(SG3DDrawParams* ParentDp, float* MaxY)
{
    for (list<CG3DResChild>::iterator cld = mRef->mChilds.begin();
        cld != mRef->mChilds.end();
        cld++)
    {
        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            SG3DDrawParams TmpPa;

            cld->GetParams(&TmpPa, ParentDp);

            float CldMaxY = TmpPa.mPos.y + cld->Height();
            if (CldMaxY > *MaxY)
            {
                *MaxY = CldMaxY;
            }
            cld->GetMaxY(&TmpPa, MaxY);
        }
    }
}


#endif



//---------------------------------------------------------------------------
//
// Klasse:    CG3DResChild
// Methode:		CalcAlignPosition
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::CalcAlignPosition(SG3DDrawParams* pa)
{
    /*
     * cout << "CalcAlignPosition START " << endl;
     * cout << " Name:" << mRef->mName << endl;
     * cout << " X=" << dp->mPos.x << " Y=" << dp->mPos.y
     *   << " SX=" << dp->mScale.x << " SY=" << dp->mScale.y
     *   << " Width=" << Width() << " Height=" << Height() << endl;
     *
     * cout << "Stretchmode=" << mStretchMode << endl;
     * cout << "Alignment=" << mAlignment << endl;
     */


    static CG3DResChild* LastObject;
    static CG3DResChild* LastReference;

    float* PosX = &(mDrawParams.mPos.x);
    float* PosY = &(mDrawParams.mPos.y);

    GetDrawParams(pa);


    /*
     * cout << "CalcAlignPosition NAME=" << mRef->mName << endl;
     * cout << "X=" << *PosX << endl;
     * cout << "Y=" << *PosY << endl;
     */

    if (mRef == NULL)
    {
        cout << "***** NULL Error in CG3DResChild::CalcAlignPosition" << endl;
        return;
    }

    if (mStretchMode & ESTRMODE_X)
    {
        mDrawParams.mScale.x = pa->mParentWidth / Width(&mOrigParams);
    }

    if (mStretchMode & ESTRMODE_Y)
    {
        mDrawParams.mScale.y = pa->mParentHeight / Height(&mOrigParams);
    }


    if (mAlignment & ERSTAT_HALIGN_CENTER)
    {
        *PosX += (pa->mParentWidth / 2) - (Width() / 2);
    }

    if (mAlignment & ERSTAT_VALIGN_CENTER)
    {
        *PosY += (pa->mParentHeight / 2) - (Height() / 2);
    }

    if (mAlignment & ERSTAT_HALIGN_RIGHT)
    {
        *PosX += pa->mParentWidth - Width();
    }

    if (mAlignment & ERSTAT_VALIGN_BOTTOM)
    {
        *PosY += pa->mParentHeight - Height();
    }
    if (mAlignment & ERSTAT_VALIGN_BOTTOM2)
    {
        *PosY += pa->mParentHeight;
    }


    if (mAlignment & ERSTAT_HALIGN_RIGHTOBJECT)
    {
        if (LastObject == NULL)
        {
            cout << "****** LastObject=NULL in CG3DResChild::CalcAlignPosition" << endl;
            exit(0);
        }
        else
        {
            if (mRef->mName == "BigBox2")
            {
                cout << "LastObject=" << LastObject->mRef->mName << endl;
            }

            *PosX += LastObject->mDrawParams.mPos.x + LastObject->Width();
        }
    }

    if (mAlignment & ERSTAT_VALIGN_BOTTOMOBJECT)
    {
        if (LastReference == NULL)
        {
            cout << "****** LastReference=NULL in CG3DResChild::CalcAlignPosition" << endl;
            exit(0);
        }
        else
        {
            float MaxY = LastReference->Height();

            //cout << "Object Name=" << mName << " Height=" << MaxY << endl;
            //cout << "LastReference Name=" << LastReference->mName << " MaxY=" << MaxY << endl;
            SG3DDrawParams ParentDp = LastReference->mOrigParams;
            ParentDp.mPos.Set(0, 0);
            LastReference->GetMaxY(&ParentDp, &MaxY);

            //cout << "  GetMaxY=" << MaxY << endl;

            *PosY = MaxY + LastReference->mDrawParams.mPos.y;
        }
    }
    LastObject = this;
    if (LastObject->mStatus.Get(ESTAT_REFERENCE))
    {
        LastReference = LastObject;
    }
    mDrawParams.mParentWidth = Width();
    mDrawParams.mParentHeight = Height();
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Width
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResChild::Width(SG3DDrawParams* dp)
{
    return mRef->Width(dp);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Height
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResChild::Height(SG3DDrawParams* dp)
{
    return mRef->Height(dp);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Width
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResChild::Width()
{
    return mRef->Width(&mDrawParams);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Height
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResChild::Height()
{
    return mRef->Height(&mDrawParams);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		GetParams
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::GetParams(SG3DDrawParams* pa, SG3DDrawParams* Parent)
{
    pa->mPos = Parent->mPos + mOrigParams.mPos;

    if (mStatus.Get(ESTAT_INHERITSCALE))
    {
        pa->mScale.x = Parent->mScale.x * mOrigParams.mScale.x;
        pa->mScale.y = Parent->mScale.y * mOrigParams.mScale.y;
    }
    else
    {
        pa->mScale = mOrigParams.mScale;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		GetChildParams
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::GetDrawParams(SG3DDrawParams* Parent)
{
    GetParams(&mDrawParams, Parent);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResChild
// Methode:		Adjust
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::Adjust(SG3DDrawParams* dp)
{
    for (list<CG3DResChild>::iterator cld = mRef->mChilds.begin();
        cld != mRef->mChilds.end();
        cld++)
    {
        SG3DDrawParams cdp;
        cld->GetDrawParams(dp);
        cld->Adjust(&cdp);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResChild
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::Draw(SG3DDrawParams* pa)
{
    CalcAlignPosition(pa);
    mRef->Draw(&mDrawParams);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResChild
// Methode:		SetPosition
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResChild::SetPosition(CVector2<float>* NewPos, unsigned int Flags)
{
    if (Flags & SPF_X_ONLY)
    {
        mOrigParams.mPos.x = NewPos->x;
    }
    else
    if (Flags & SPF_Y_ONLY)
    {
        mOrigParams.mPos.y = NewPos->y;
    }
    else
    {
        mOrigParams.mPos = *NewPos;
    }

    if (mRef->mType == ERTYPE_SLIDER)
    {
        CG3DResourceSlider* sld = dynamic_cast<CG3DResourceSlider*> (mRef);
        float* vtest;

        if (sld->mSliderType == ESLIDER_HORIZONTAL)
        {
            vtest = &mOrigParams.mPos.x;
        }
        else
        {
            vtest = &mOrigParams.mPos.y;
        }

        if (*vtest < sld->mSliderMin)
        {
            *vtest = sld->mSliderMin;
        }
        if (*vtest > sld->mSliderMax)
        {
            *vtest = sld->mSliderMax;
        }
    }
    *NewPos = mOrigParams.mPos;
}
