//------------------------------------------------------------------------------
//
// PROJECT : GLUT test application
//
// FILE    : COpenGL.cpp
//
// VERSION : 1.0
//
// AUTHOR  : Martin Steen
//
//
//
//
//
//---------------------------------------------------------------------------


#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

#include <GLinclude.h>
#include <COpenGL.h>

#include <CG3DGlobals.h>
#include "CGLApplication.h"

extern CGLApplication gApp;

// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : ViewportAction
//
//
//
// ---------------------------------------------------------------------------

void COpenGL::ViewportAction(int w, int h)
{
    //cout << "ViewportAction w=" << w << " h=" << h << endl;

    gApp.SetResolution(w, h);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : Action
//
//
//
// ---------------------------------------------------------------------------

void COpenGL::Action()
{
    GLfloat WhiteMaterial[] = { 1.0, 1.0, 1.0, 1.0 };

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    StartProjectionView();
    gApp.Draw2DObjects();

    glutSwapBuffers();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : MouseButtonAction
//
//
//
// ---------------------------------------------------------------------------

void COpenGL::MouseButtonAction()
{
    string ClickedName;

    //cout << "mMouse.mButton=" << mMouse.mButton << endl;

    switch (mMouse.mButton)
    {
        case GLUT_LEFT_BUTTON:

            if (mMouse.mState[GLUT_LEFT_BUTTON] == GLUT_DOWN)
            {
                gApp.LeftMouseButtonDown();
            }
            else
            {
                gApp.LeftMouseButtonUp();
            }
            break;

        case GLUT_RIGHT_BUTTON:

            if (mMouse.mState[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
            {
                gApp.RightMouseButtonDown();
            }
            else
            {
                gApp.RightMouseButtonUp();
            }
            break;

        case 3: // Mousewheel up

            if (mMouse.mState[3] == GLUT_DOWN)
            {
                gApp.MouseWheel(0);
            }
            break;

        case 4: // Mousewheel down

            if (mMouse.mState[4] == GLUT_DOWN)
            {
                gApp.MouseWheel(1);
            }
            break;
    }
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : MouseMotionAction
//
//
//
// ---------------------------------------------------------------------------

void COpenGL::MouseMotionAction()
{
    //gSlider.mPos.x = mMouse.x;
    //gSlider.mPos.y = mMouse.y;
    static CVector2<int> MousePos;

    int dx = MousePos.x - mMouse.x;
    int dy = MousePos.y - mMouse.y;

    MousePos.x = mMouse.x;
    MousePos.y = mMouse.y;

    gApp.MouseMotion(dx, dy);

/*
 *  if (mMouse.mState[GLUT_LEFT_BUTTON] == GLUT_DOWN)
 *  {
 *      gApp.MouseMotionLeft(dx, dy);
 *  }
 *  else
 *  if (mMouse.mState[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
 *  {
 *      gApp.MouseMotionRight(dx, dy);
 *  }
 *  else
 *  {
 *      gApp.MouseMotion(dx, dy);
 *  }
 */

    //cout << "MouseMotionAction X=" << mMouse.x << " Y=" << mMouse.y << endl;
}


void COpenGL::SpecialkeysAction(int key)
{
    gApp.SpecialkeysAction(key);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : KeyboardAction
//
//
//
// ---------------------------------------------------------------------------

void COpenGL::KeyboardAction(unsigned char key)
{
    gApp.KeyboardAction(key);
}
