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

extern CG3DGlobals* gGlobals;
extern CG3DResourceList* gResourceList;


static float sWhiteColor[] = { 1.0, 1.0, 1.0, 1.0 };

CG3DResource::~CG3DResource()
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        delete cld->mRef;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ShowColor
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::ActivateTooltipp(bool On)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        //cout << "Check Tooltip: " << cld->mRef->mName << endl;

        if (cld->mRef->mType == ERTYPE_TOOLTIPP)
        {
            //cout << " set ACTIVE on: " << On << endl;

            cld->mStatus.Set(ESTAT_ACTIVE, On);
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ShowColor
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::ShowColor(char* s, float* f)
{
    // cout << s;
    // cout << " R=" << f[0] << " G=" << f[1] << " B=" << f[2] << " A=" << f[3] << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		AdjustY
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResource::AdjustY(float y)
{
    return gGlobals->mWindowHeight - y;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ParseColor
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResource::ParseColor(string& ColorString, float* Color)
{
    CHexConv HexConv;

    const char* col = ColorString.c_str();

    if (strlen(col) == 8)
    {
        Color[0] = HexConv.ByteToFloat(col[0], col[1]);
        Color[1] = HexConv.ByteToFloat(col[2], col[3]);
        Color[2] = HexConv.ByteToFloat(col[4], col[5]);
        Color[3] = HexConv.ByteToFloat(col[6], col[7]);
        return true;
    }
    cout << "***** Colorlen must be 8 digits: "<< ColorString << endl;
    return false;
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

float CG3DResource::Width(SG3DDrawParams* dp)
{
    return UnscaledWidth() * dp->mScale.x;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Heigth
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

float CG3DResource::Height(SG3DDrawParams* dp)
{
    return UnscaledHeight() * dp->mScale.y;
}


int CG3DResource::UnscaledWidth()
{
    cout << "***** Error no implementation of CG3DResource::UnscaledWidth() for resources of type " << GetTypeName() << endl;
    cout << "***** Resource name=" << mName << endl;
    //cout << "Childs=" << mChilds.size() << endl;
    //cout << "Refs=" << mRefs.size() << endl;
    return 0;
}


int CG3DResource::UnscaledHeight()
{
    cout << "***** Error no implementation of CG3DResource::UnscaledHeight() for resources of type " << GetTypeName() << endl;
    cout << "***** Resource name=" << mName << endl;

    return 0;
}


bool CG3DResource::Check(void)
{
    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ResolveRefs
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

const char* CG3DResource::GetTypeName()
{
    static char errtxt[64];

    static const char* TypeNames[] =
    {
        /* 00 */ "ANY",
        /* 01 */ "VOID",
        /* 02 */ "KNOT",
        /* 03 */ "STRING",
        /* 04 */ "BUTTON",
        /* 05 */ "DIALOG",
        /* 06 */ "IMAGE",
        /* 07 */ "TABLEAU",
        /* 08 */ "FONT",
        /* 09 */ "FRAME",
        /* 10 */ "BOX",
        /* 11 */ "OPTIONBUTTON",
        /* 12 */ "ROOT",
        /* 13 */ "LINE",
        /* 14 */ "TEXTFIELD",
        /* 15 */ "SLIDER"
    };

    if ((mType > 15) || (mType < 0))
    {
        sprintf(errtxt, "?=%d", mType);
        return errtxt;
    }

    return TypeNames[mType];
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ResolveRefs
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResChild* CG3DResource::FindChild(EResourceType type)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mRef->mType == type)
        {
            //cout << "+++++ FOUND! " << cld->mRef->mName << endl;
            return &(*cld);
        }
    }
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		SetColorWhite
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::SetColorWhite()
{
    glColor4fv(sWhiteColor);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		SetColorWhite
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::SetColorWhite(float** ColPtr)
{
    *ColPtr = sWhiteColor;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ResolveRefs
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResRef* CG3DResource::FindRef(EResourceType type)
{
    if (!mRefs.empty())
    {
        for (list<CG3DResRef>::iterator ref = mRefs.begin();
            ref != mRefs.end();
            ref++)
        {
            if (ref->mRef != NULL)
            {
                //cout << "Test " << ref->mRef->mName << endl;

                if (ref->mRef->mType == type)
                {
                    //cout << "+++++ FOUND! " << cld->mRef->mName << endl;
                    return &(*ref);
                }
            }
        }
    }
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    static
// Methode:		intent
//
// Parameter: r - Anzahl der Leerzeichen zum Einruecken
//
// Return:
//
//---------------------------------------------------------------------------

static void intent(int ir)
{
    for (int i = 0; i < ir; i++)
    {
        cout << ' ';
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Adjust
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::Adjust(SG3DDrawParams* pa)
{
    //static int iv;
    //iv++;
    //intent(iv);
    //cout << "Adjust " << mName << endl;

    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        //intent(iv+1);
        //cout << "Adjust:" << cld->mRef->mName << endl;

        cld->CalcAlignPosition(pa);
        cld->mRef->Adjust(&cld->mDrawParams);

        //cout << "ok " << cld->mRef->mName << endl;
    }
    //intent(iv);
    //cout << "Adjust OK " << mName << endl;
    //iv--;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		ResolveRefs
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::AddResRef(CG3DResource* res)
{
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Show
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::Show()
{
    cout << "Name=" << mName << endl;
    cout << "  Type=";

    switch (mType)
    {
        case ERTYPE_VOID:
            cout << "void";
            break;

        case ERTYPE_STRING:
            cout << "string";
            break;

        case ERTYPE_BUTTON:
            cout << "button";
            break;

        case ERTYPE_OBUTTON:
            cout << "option button";
            break;

        case ERTYPE_DIALOG:
            cout << "dialog";
            break;

        case ERTYPE_IMAGE:
            cout << "image";
            break;

        case ERTYPE_TABLEAU:
            cout << "tableau";
            break;
    }
    cout << endl;
    if (!mChilds.empty())
    {
        for (list<CG3DResChild>::iterator cld = mChilds.begin();
            cld != mChilds.end();
            cld++)
        {
            cout << "  Child=" << cld->mName << endl;
        }
    }
    if (!mRefs.empty())
    {
        for (list<CG3DResRef>::iterator ref = mRefs.begin();
            ref != mRefs.end();
            ref++)
        {
            cout << "  Ref=" << ref->mName << endl;
        }
    }
    cout << "Show ok" << endl;
}


#if 1
//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		FrontDraw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::FrontDraw(SG3DDrawParams* pa)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            cld->CalcAlignPosition(pa);
            cld->mRef->FrontDraw(&(cld->mDrawParams));

            if (cld->mStatus.Get(ESTAT_FRONTDRAW))
            {
                cld->mStatus.Clr(ESTAT_FRONTDRAW);
                cld->mRef->Draw(&(cld->mDrawParams));
                cld->mStatus.Set(ESTAT_FRONTDRAW);
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		BackDraw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::BackDraw(SG3DDrawParams* pa)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            cld->CalcAlignPosition(pa);
            cld->mRef->BackDraw(&cld->mDrawParams);

            if (cld->mStatus.Get(ESTAT_BACKDRAW))
            {
                cld->mStatus.Clr(ESTAT_BACKDRAW);
                cld->mRef->Draw(&cld->mDrawParams);
                cld->mStatus.Set(ESTAT_BACKDRAW);
            }
        }
    }
}


#endif

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::Draw(SG3DDrawParams* pa)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
/*
 *      if (cld->mRef->mType == ERTYPE_LINE)
 *      {
 *          cout << "name=" << cld->mRef->mName << " stat=" << cld->mStatus.Get(ESTAT_ACTIVE) << endl;
 *      }
 */

        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            cld->Draw(pa);
        }

/*
 *      if (cld->mStatus.Get(ESTAT_ACTIVE) &&
 *         !cld->mStatus.Get(ESTAT_BACKDRAW) &&
 *         !cld->mStatus.Get(ESTAT_FRONTDRAW))
 *      {
 *          cld->Draw(pa);
 *      }
 */
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Show
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResource::Inside(SG3DDrawParams* dp, CVector2<int>* Mouse)
{
    CRectT<float> rc;

    rc.Set(dp->mPos.x, dp->mPos.y, dp->mPos.x + Width(dp), dp->mPos.y + Height(dp));
    return rc.InRect(Mouse->x, Mouse->y);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Show
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResource::Showparams(SG3DDrawParams* dp)
{
    /*
     * cout << "Rollover Test:" << mName << " X=" << dp->mPos.x << " Y=" << dp->mPos.y << endl;
     * cout << " UW=" << UnscaledWidth() << " UH=" << UnscaledHeight() << endl;
     * cout << " W=" << Width(dp) << " H=" << Height(dp) << endl;
     * cout << " SX=" << dp->mScale.x << " SY=" << dp->mScale.y << endl;
     */

    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Show
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResource::Rollover(SG3DDrawParams* dp, CVector2<int>* Mouse, bool TestRollover)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mStatus.Get(ESTAT_ACTIVE))
        {
            cld->CalcAlignPosition(dp);
            cld->mRef->Rollover(&cld->mDrawParams, Mouse, cld->mStatus.Get(ESTAT_TESTROLLOVER));
        }
    }

    if (TestRollover)
    {
        return Inside(dp, Mouse);
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		SetParents
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


void CG3DResource::SetParents(CG3DResource* parent)
{
    mParent = parent;
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        cld->mRef->SetParents(this);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Activate
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::Activate(bool v)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if ((cld->mRef->mType == ERTYPE_STRING) ||
            (cld->mRef->mType == ERTYPE_LINE))
        {
            cld->mStatus.Set(ESTAT_ACTIVE, true);
        }
        else
        {
            cld->mStatus.Set(ESTAT_ACTIVE, v);
            if (cld->mStatus.Get(ESTAT_ACTIVATE_CHILDREN))
            {
                cld->mRef->Activate(v);
            }
        }
    }
    Reset(ERESET_INIT);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		Activate
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::Deactivate()
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        cld->mStatus.Clr(ESTAT_ACTIVE);
        cld->mRef->Reset(ERESET_DEACTIVATE);
        cld->mRef->Deactivate();
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		DeactivateKnotGroup
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::DeactivateKnotGroup(CG3DResourceKnot* GroupKnot)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mRef->mType == ERTYPE_KNOT)
        {
            CG3DResourceKnot* kn = dynamic_cast<CG3DResourceKnot*> (cld->mRef);
            if (kn->Group() == GroupKnot->Group())
            {
                if (kn->mName != GroupKnot->mName)
                {
                    cld->mStatus.Set(ESTAT_ACTIVE, false);
                }
                else
                {
                    cld->mStatus.Set(ESTAT_ACTIVE, true);
                }
            }
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		DeactivateButtonGroup
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::DeactivateButtonGroup(CG3DResourceButton* GroupButton)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        //cout << "  Test Child:" << cld->mName;

        if (cld->mRef->IsButton())
        {
            CG3DResourceButton* bt = dynamic_cast<CG3DResourceButton*> (cld->mRef);
            if (bt->Group() == GroupButton->Group())
            {
                if (bt->mName != GroupButton->mName)
                {
                    //cout << " deactivate";
                    bt->Reset(ERESET_DEACTIVATE);
                    bt->SetProperty(EBTPROP_ACTIVE, false);
                    cld->mRef->Deactivate();
                }
            }
        }
        else
        {
            cld->mRef->DeactivateButtonGroup(GroupButton);
        }

        //cout << endl;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResource
// Methode:		SearchChild
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResource::SearchChild(string& Name)
{
    for (list<CG3DResChild>::iterator cld = mChilds.begin();
        cld != mChilds.end();
        cld++)
    {
        if (cld->mRef->mName == Name)
        {
            throw(CSearchChild(&(*cld)));
        }
        else
        {
            cld->mRef->SearchChild(Name);
        }
    }
}


/*
 * int* CG3DResource::SearchInt(int x)
 * {
 *  for (list<int>::iterator cld  = MyList.begin();
 *                                                   cld != MyList.end();
 *                                                   cld++)
 *  {
 *      if (*cld == x)
 *      {
 *          return &(*cld);
 *      }
 *  }
 *  return NULL;
 * }
 */
