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

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include <CVector3T.h>
#include <CFileIO.h>
#include "CG3DGlobals.h"
#include "CG3DReslistInterface.h"

class CG3DInterfaceError
{
    public:

        CG3DInterfaceError(char* err)
        {
            mErrMsg = err;
        }


        char* mErrMsg;
};


//---------------------------------------------------------------------------
//
// Klasse:    CG3DReslistInterface
// Methode:		Init
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------


#ifdef _XXXWIN32

bool CG3DReslistInterface::Init(char* ResFileName, CG3DGlobals* gbl)
{
    HINSTANCE hinst;
    string MsgBuff;
    bool ok;

    char* DllName = "G3DResourceList.dll";

    try
    {
        hinst = LoadLibrary(DllName);
        if (hinst == NULL)
        {
            throw(CG3DInterfaceError(DllName));
        }
    }
    catch (CG3DInterfaceError& ierr)
    {
        string MsgBuff = string(ierr.mErrMsg) + " DLL nicht gefunden";
        MessageBox(0, MsgBuff.c_str(), "CG3DReslistInterface.cpp", MB_OK);
    }

    mSendCommand = (DLLFUNC_intvoidP)GetProcAddress(hinst, "SendCommand");

    try
    {
        if (mSendCommand == NULL)
        {
            throw(CG3DInterfaceError("InitG3DResource()"));
        }

        // Kein Fehler aufgetreten
        (*mSendCommand)(EG3DcomSetGlobals, gbl);
        (*mSendCommand)(EG3DcomInit, ResFileName);

        ok = true;
    }
    catch (CG3DInterfaceError& ierr)
    {
        string ErrMsg = string(ierr.mErrMsg) + " nicht gefunden";
        MessageBox(0, MsgBuff.c_str(), "CG3DReslistInterface.cpp", MB_OK);
        ok = false;
    }

    return ok;
}


int CDReslistInterface::SendCommand(EG3DInterfaceCommand cmd, void* val = NULL)
{
    (*mSendCommand)(cmd, val);
}


#else

//---------------------------------------------------------------------------
//
// Klasse:    CG3DReslistInterface
// Methode:		SendCommand
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

int gSendCommand(int a, void* b);

int CG3DReslistInterface::SendCommand(EG3DInterfaceCommand cmd, void* val)
{
    return gSendCommand(cmd, val);
}


//---------------------------------------------------------------------------
//
// Klasse:    CG3DReslistInterface
// Methode:		Init
//
// Parameter:
//
// Return:
//
//---------------------------------------------------------------------------

bool CG3DReslistInterface::Init(const char* ResFileName, CG3DGlobals* gbl)
{
    CFileIO fio;
    char* ResTmpName = fio.NewString(ResFileName);

    gSendCommand(EG3DcomSetGlobals, gbl);
    gSendCommand(EG3DcomInit, ResTmpName);
    delete[] ResTmpName;

    return true;
}


#endif
