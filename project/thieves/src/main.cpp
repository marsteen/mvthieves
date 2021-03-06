#include <cstdlib>



#include <SDL/SDL.h>
#include <iostream>
#include <GlobalDebug.h>
#include "CSdlThiefs.h"

using namespace std;

// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

#ifdef _WIN32

// ---------------------------------------------------------------------------
//
// WINDOWS
//
// ---------------------------------------------------------------------------


#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int iCmdShow)
{
    CSdlThiefs SdlThiefs;

    GlobalDebugInit();

    if (SdlThiefs.Init())
    {
        SdlThiefs.ParseWinArgs(lpCmdLine);
        if (SdlThiefs.InitScreen())
        {
            SdlThiefs.MainLoop();
        }
    }
    return 0;
}


#else

// ---------------------------------------------------------------------------
//
// UNIX
//
// ---------------------------------------------------------------------------


int main(int argc, char* argv[])
{
    CSdlThiefs SdlThiefs;

    GlobalDebugInit();

    if (SdlThiefs.Init())
    {
        SdlThiefs.ParseArgs(argc, argv);
        if (SdlThiefs.InitScreen())
        {
            SdlThiefs.MainLoop();
        }
    }
    return 0;
}


#endif
