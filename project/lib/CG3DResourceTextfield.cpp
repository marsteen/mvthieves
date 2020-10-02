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

static float sBlackColor[] = { 0.0, 0.0, 0.0, 1.0 };

//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTextfield
// Methode:		SendChar
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

CG3DResourceTextfield::CG3DResourceTextfield()
{
    mTextPos.Set(0, 0);
    mBlinkCounter = 0;
    mFrequency = 50;
    mActive = false;
    mColorRef = sBlackColor;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTextfield
// Methode:		SendChar
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceTextfield::SendChar(unsigned char key)
{
    if (key >= ' ')
    {
        if (mTextField.size() < mMaxSize)
        {
            mTextField.push_back(key);
        }
    }
    else
    {
        switch (key)
        {
            case 8: // Backspace

                if (mTextField.size() > 0)
                {
                    mTextField.resize(mTextField.size()-1);
                }
                break;
        }
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTextfield
// Methode:		Draw
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceTextfield::Draw(SG3DDrawParams* dp)
{
    mImgRef->DrawImage(0, dp); // Bild


    SG3DDrawParams tp;
    char* txt = (char*)mTextField.c_str();

    tp.mPos.Set(dp->mPos.x + mTextPos.x, dp->mPos.y + mTextPos.y);

    if (mActive)
    {
        cout	<< "Name=" << mName
                << " " << mColorRef[0]
                << " " << mColorRef[1]
                << " " << mColorRef[2]
                << " " << mColorRef[3] << endl;

        glColor4fv(mColorRef);
        mFontRef->DrawString(txt, (int)tp.mPos.x, (int)tp.mPos.y);
        SetColorWhite();

        if (mBlinkCounter++ == mFrequency)
        {
            mShowCursor = !mShowCursor;
            mBlinkCounter = 0;
        }
        if (mShowCursor)
        {
            tp.mPos.x += mFontRef->FontWidth(txt);
            mImgRef->DrawImage(1, &tp); // Cursor
        }
    }
    else
    {
        float Blue[] = { 0.0, 0.0, 0.2, 1.0 };

        glColor4fv(Blue);
        mFontRef->DrawString(txt, (int)tp.mPos.x, (int)tp.mPos.y);
        SetColorWhite();
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

void CG3DResourceTextfield::AddResRef(CG3DResource* res)
{
    switch (res->mType)
    {
        case ERTYPE_IMAGE:

            mImgRef = dynamic_cast<CG3DResourceImage*>(res);
            break;


        case ERTYPE_FONT:

            mFontRef = dynamic_cast<CG3DResourceFont*>(res);
            break;
    }
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTextfield
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DResourceTextfield::Inside(SG3DDrawParams* dp, CVector2<int>* Mouse)
{
    bool r = false;

    if (mImgRef->InsideImage(0, dp, Mouse))
    {
        throw(CInsideObject(this));
        r = true;
    }
    else
    {
        Reset(ERESET_ROLLOVER);
    }

    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DResourceTextfield
// Methode:		Inside
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

void CG3DResourceTextfield::SetTextParams(float x, float y, int tmax, int freq)
{
    mTextPos.x = x;
    mTextPos.y = y;
    mMaxSize = tmax;
    mFrequency = freq;
}
