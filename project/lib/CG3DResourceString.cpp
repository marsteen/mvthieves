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

extern CG3DGlobals* gGlobals;

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

void CG3DResourceString::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_FONT:

            mFontRef = dynamic_cast<CG3DResourceFont*>(res);
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		DrawString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceString::Draw(CVector2<float>* Pos)
{
    char* tx = (char*)mText[gGlobals->mLanguage].c_str();

    //cout << "Draw String " << tx << endl;

    if (tx[0] == 0)
    {
        tx = (char*)mText[0].c_str();
    }


    if (mColorRef != NULL)
    {
        glColor4fv(mColorRef);
        mFontRef->DrawString(tx, (int)Pos->x, (int)Pos->y);
        SetColorWhite();
    }
    else
    {
        mFontRef->DrawString(tx, (int)Pos->x, (int)Pos->y);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		DrawString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceString::Draw(const char* str, CVector2<float>* Pos)
{
    if (mColorRef != NULL)
    {
        glColor4fv(mColorRef);
        mFontRef->DrawStringRaw(str, Pos->x, Pos->y);
        SetColorWhite();
    }
    else
    {
        mFontRef->DrawStringRaw(str, Pos->x, Pos->y);
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		Reset
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceString::Reset(int mode)
{
    switch (mode)
    {
        case ERESET_ROLLOVER:

            mColorRef = mOrigColor;
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		DrawString
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceString::Draw(SG3DDrawParams* dp)
{
    Draw(&(dp->mPos));
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		GetText
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

inline char* CG3DResourceString::GetText()
{
    char* tx = (char*)mText[gGlobals->mLanguage].c_str();

    if (tx[0] == 0)
    {
        tx = (char*)mText[0].c_str();
    }
    return tx;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		UnscaledWidth
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceString::UnscaledWidth()
{
    return mFontRef->FontWidth(GetText());
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceString
// Methode:		UnscaledHeight
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int CG3DResourceString::UnscaledHeight()
{
    return mFontRef->FontHeight();
}
