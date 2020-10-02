#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include <SDL/SDL.h>
#include <CStringTool.h>
#include <GSystemFunctions.h>
#include <CKeymap.h>
#include "CSdlApp.h"



extern float gGlobalLineWidth;
extern float gGlobalScale;
extern float gGlobalScorePos;

using namespace std;

// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : CSdlApp
//
//
//
// ---------------------------------------------------------------------------

CSdlApp::CSdlApp()
{
    mFullscreen = false;
    mMouseFaktor = 10.0;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseArgVec
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseArgVec(const vector<string>& ArgStr)
{
    CStringTool st;

    for (int i = 0; i < ArgStr.size(); i++)
    {
        if (ArgStr[i] == "-w480x300")
        {
            mFullscreen = false;
            mXres = 480;
            mYres = 300;
        }
        else
        if (ArgStr[i] == "-w800x600")
        {
            mFullscreen = false;
            mXres = 800;
            mYres = 600;
        }
        else
        if (ArgStr[i] == "-w1024x768")
        {
            mFullscreen = false;
            mXres = 1024;
            mYres = 768;
        }
        else
        if (ArgStr[i] == "-w1280x1024")
        {
            mFullscreen = false;
            mXres = 1280;
            mYres = 1024;
        }
        else
        if (ArgStr[i] == "-w1440x900")
        {
            mFullscreen = false;
            mXres = 1440;
            mYres = 900;
        }
        else
        if (ArgStr[i] == "-w1920x1080")
        {
            mFullscreen = true;
            mXres = 1920;
            mYres = 1080;
        }
        else
        if (ArgStr[i] == "-w1920x1200")
        {
            mFullscreen = true;
            mXres = 1920;
            mYres = 1200;
        }
        else
        if (ArgStr[i] == "-scale2")
        {
            gGlobalScale = 0.5;
            gGlobalLineWidth = 1.0;
        }
        else
        if (ArgStr[i] == "-uhd")
        {
            gGlobalScale = 1.5f;
            gGlobalLineWidth = 2.0f;
        }
        else
        if (ArgStr[i] == "-linewidth")
        {
            gGlobalLineWidth = st.StringTo<float>(ArgStr[i+1]);
        }
        else
        if (ArgStr[i] == "-scale")
        {
            gGlobalScale = st.StringTo<float>(ArgStr[i+1]);
        }
        else
        if (ArgStr[i] == "-mousespeed")
        {
            mMouseFaktor = 100.0 / st.StringTo<float>(ArgStr[i+1]);
        }
        else
        if (ArgStr[i] == "-scorepos")
        {
            gGlobalScorePos = st.StringTo<int>(ArgStr[i+1]);
        }
        if (ArgStr[i] == "-version")
        {
            cout << "Version: 2.0" << endl;
            exit(0);
        }
        else
        if (ArgStr[i] == "-help")
        {
            cout << "********************************************************" << endl;
            cout << "THIEFS Version 2.1" << endl;
            cout << "written 2009 by Martin Steen" << endl;
            cout << "commandline options:" << endl;
            cout << "  -linewidth <float>: change linewidth" << endl;
            cout << "  -scale <float>: change scale" << endl;
            cout << "  -w800x600: windowmode 800x600" << endl;
            cout << "  -w1024x768: windowmode 1024x768" << endl;
            cout << "  -w1280x1024: windowmode 1280x1024" << endl;
            cout << "  -w1440x900: windowmode 1440x900" << endl;
            cout << "  -mousespeed <float>: change mousespeed (default=10.0)" << endl;
            cout << "Version: 1.0" << endl;
            cout << "********************************************************" << endl;
            exit(0);
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseArgs
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseArgs(int argc, char* argv[])
{
    char AppPath[256];

    mFullscreen = true;
    vector<string> ArgVec;

    strcpy(AppPath, argv[0]);
    char* Path = strrchr(AppPath, GsysPathDiv());

    if (Path != NULL)
    {
        *Path = 0;
        GsysSetAppPath(AppPath);
        //cout << "mAppPath=" << mAppPath << endl;
    }


    for (int i = 1; i < argc; i++)
    {
        string ArgStr = string(argv[i]);
        ArgVec.push_back(ArgStr);
    }
    ParseArgVec(ArgVec);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseWinArgs
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseWinArgs(const char* Commandline)
{
    CStringTool st;
    vector<string> SplitResult;

    mFullscreen = true;

    st.SplitString(Commandline, &SplitResult, ' ');


    ParseArgVec(SplitResult);

    /*
     * for (int i = 0; i < SplitResult.size(); i++)
     * {
     *  ParseArg(SplitResult[i]);
     * }
     */
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : GameLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::GameLoop()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

static Uint32 sTimerCallback(Uint32 interval, void* param)
{
    CSdlApp* SdlApp = (CSdlApp*)param;

    SdlApp->Timer();

    return interval;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : Timer
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::Timer()
{
/*
 * static int t;
 * cout << "timer:"  << t << endl;
 * t++;
 */

    GameLoop();
    SDL_GL_SwapBuffers();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : SetResolution
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::SetResolution(int w, int h)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::InitGame()
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : GetVideoModes
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::GetVideoModes(int* FirstW, int* FirstH, int n)
{
    SDL_Rect** modes;
    int i;
    bool r = false;

    /* Get available fullscreen/hardware modes */
    modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);
    /* Check if there are any modes available */
    if (modes == NULL)
    {
        cout << "No modes available!" << endl;
    }
    else
    /* Check if our resolution is restricted */
    if (modes == (SDL_Rect**)-1)
    {
        //cout << "All resolutions available." << endl;
    }
    else
    {
#if 0
        /* Print valid modes */
        cout << "Available Modes:" << endl;
        for (i = 0; modes[i]; ++i)
        {
            cout	<< " " << modes[i]->w
                    << "x" << modes[i]->h << endl;
        }
#endif
        *FirstW = modes[n]->w;
        *FirstH = modes[n]->h;
        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : FinishGame
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::FinishGame()
{
    SDL_ShowCursor(true);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::Init()
{
    bool r = true;

    //cout << "CSdlApp::Init START" << endl;
    //cout << "SDL_GL_SetAttribute ok" << endl;
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        cout << "SDL konnte nicht initialisiert werden: " << SDL_GetError() << endl;
        r = false;
    }
    // cout << "CSdlApp::Init OK" << endl;
    SDL_ShowCursor(false); // Mousecursor verstecken

    CKeymap::ReadMapFile("files/keymap.txt");

    //cout << "Joysticks:" << SDL_NumJoysticks() << endl;



    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlThiefs
// METHODE       : InitOpenGL
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::InitOpenGL(int w, int h)
{
    mOpenGL.SetViewport(w, h);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::InitScreen() //int xres, int yres, int Bits)
{
    bool r = true;
    unsigned int flags;
    const SDL_VideoInfo* VideoInfo = NULL;

    VideoInfo = SDL_GetVideoInfo();
    int bpp = VideoInfo->vfmt->BitsPerPixel;

    //cout << "bits per pixel=" << bpp << endl;

    //SDL_Surface* Surface = SDL_GetVideoSurface();
    //cout << "XRES=" << Surface->w << " YRES=" << Surface->h << endl;

    if (mFullscreen)
    {
        flags = SDL_OPENGL | SDL_FULLSCREEN;
    }
    else
    {
        flags = SDL_OPENGL;
    }
    //flags = SDL_OPENGL | SDL_NOFRAME | SDL_DOUBLEBUF | SDL_FULLSCREEN;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, bpp);

    if (mFullscreen)
    {
        int FirstW;
        int FirstH;
        if ((GetVideoModes(&FirstW, &FirstH, 0) && mFullscreen))
        {
            mXres = FirstW;
            mYres = FirstH;


            cout << "Setting Mode to " << FirstW << "x" << FirstH << " Bits:" << bpp << endl;
        }
    }
    if (mXres < 1280.0)
    {
        gGlobalScale = float(mXres) / 1280.0;
    }

    mDrawContext = SDL_SetVideoMode(mXres, mYres, 0, flags);

    if (mDrawContext == NULL)
    {
    }


    //SDL_Delay(1000);
    SetResolution(mXres, mYres);
    //cout << "InitOpenGL OK" << endl;
    //SDL_Delay(1000);
    InitOpenGL(mXres, mYres);
    //cout << "InitOpenGL OK" << endl;

    if (mDrawContext == NULL)
    {
        cout << "Fehler beim Umschalten der Auflösung " << mXres << "*" << mYres << ":"  <<  SDL_GetError() << endl;
        r = false;
    }
    InitGame();
    //mTimer = SDL_AddTimer(100, sTimerCallback, this);

    return r;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : DisableKeyRepeat
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::DisableKeyRepeat()
{
    SDL_EnableKeyRepeat(0, 0);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseKeys
//
//
//
// ---------------------------------------------------------------------------

bool CSdlApp::ParseKeys(SDLKey key, bool down)
{
    if (CKeymap::GetMappedKey(key) == SDLK_ESCAPE)
    {
        return false;
    }

    return true;
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : MainLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::MainLoop()
{
    while (1)
    {
        Uint32 StartTime = SDL_GetTicks();
        EventLoop();
        GameLoop();

        Uint32 EndTime = SDL_GetTicks();
        int DelayTime = 20 - (EndTime - StartTime);
        if (DelayTime > 0)
        {
            //cout << "DelayTime=" << DelayTime << endl;
            SDL_Delay(DelayTime);
        }
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : ParseMouseRel
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::ParseMouseRel(int xrel, int yrel)
{
}


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
// METHODE       : EventLoop
//
//
//
// ---------------------------------------------------------------------------

void CSdlApp::EventLoop()
{
    SDL_Event event;


    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_USEREVENT:

                //HandleUserEvents(&event);
                break;

            case SDL_KEYDOWN:

                if (CKeymap::GetMappedKey(event.key.keysym.sym) == SDLK_ESCAPE)
                {
                    if (!ParseKeys(SDLK_ESCAPE, true))
                    {
                        FinishGame();
                        SDL_Quit();
                        exit(0);
                    }
                }
                else
                {
                    ParseKeys(event.key.keysym.sym, true);
                }
                break;

            case SDL_KEYUP:

                ParseKeys(event.key.keysym.sym, false);
                break;

            case SDL_MOUSEMOTION:
            {
                int xrel = event.motion.xrel;
                int yrel = event.motion.yrel;
                ParseMouseRel(xrel, yrel);
            }
            break;


            case SDL_MOUSEBUTTONDOWN:

                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    ParseKeys(SDLK_DELETE, true);
                }
                else
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    ParseKeys(SDLK_UP, true);
                }
                else
                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    ParseKeys(SDLK_DOWN, true);
                }


                // Handle mouse clicks here.
                break;

            case SDL_MOUSEBUTTONUP:

                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    ParseKeys(SDLK_UP, false);
                }
                else
                if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    ParseKeys(SDLK_DOWN, false);
                }
                break;

            case SDL_QUIT:

                FinishGame();
                SDL_Quit();
                exit(0);
                //done = true;
                break;

            default:

                break;
        }   // End switch
    }       // End while
}
