#ifndef CSDLAPP_H
#define CSDLAPP_H

#include <SDL/SDL.h>

#include <iostream>
#include <vector>
#include <string>
#include <GSystemFunctions.h>
#include "COpenGL.h"


// ---------------------------------------------------------------------------
//
// KLASSE        : CSdlApp
//
// ---------------------------------------------------------------------------

class CSdlApp
{
    public:

        CSdlApp();


        bool Init();
        void ParseArgs(int argc, char* argv[]);
        void ParseWinArgs(const char* Commandline);
        bool InitScreen();
        void EventLoop();
        void MainLoop();
        void Finish();
        virtual void Timer();
        virtual void GameLoop();
        virtual void InitGame();
        virtual void FinishGame();

    protected:

        void ParseArgVec(const std::vector<std::string>& ArgStr);
        void DisableKeyRepeat();
        bool GetVideoModes(int* FirstW, int* FirstH, int n);
        virtual bool ParseKeys(SDLKey key, bool down);
        virtual void ParseMouseRel(int xrel, int yrel);
        virtual void SetResolution(int w, int h);

        void InitOpenGL(int w, int h);


        void HandleUserEvents(SDL_Event* event);

        SDL_Surface* mDrawContext;
        //SDL_TimerID  mTimer;
        int mXres;
        int mYres;
        float mMouseFaktor;
        COpenGL mOpenGL;
        bool mFullscreen;
};

#endif
