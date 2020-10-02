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
#include <fstream>

using namespace std;

extern "C"
{
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
};

#include <COpenGL.h>
#include <CFileIO.h>

COpenGL* sOpenGL;

// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       :
//
//
//
// ---------------------------------------------------------------------------

static void sReshapeWindow(int w, int h)
{
    sOpenGL->SetViewport(w, h);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sDisplayWindow
//
//
//
// ---------------------------------------------------------------------------

static void sDisplayWindow()
{
    sOpenGL->Action();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sMouseButton
//
// Mousebuttons:
//
// 0 - linker Button
// 1 - mittlerer Button
// 2 - rechter Button
// 3 - Mouserad nach oben
// 4 - Mouserad nach unten
//
// ---------------------------------------------------------------------------

static void sMouseButton(int Button, int State, int x, int y)
{
    //cout << "Mouse Bt=" << Button << " State=" << State << " x=" << x << " y=" << y << endl;

    sOpenGL->mMouse.mButton = Button;
    sOpenGL->mMouse.mState[Button] = State;
    sOpenGL->mMouse.x = x;
    sOpenGL->mMouse.y = y;

    sOpenGL->MouseButtonAction();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sMouseMotion
//
//
//
// ---------------------------------------------------------------------------

static void sMouseMotion(int x, int y)
{
    sOpenGL->mMouse.x = x;
    sOpenGL->mMouse.y = y;


    sOpenGL->MouseMotionAction();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sTimefunc
//
//
//
// ---------------------------------------------------------------------------

static void sTimefunc(int value)
{
    sOpenGL->Action();
    glutTimerFunc(sOpenGL->mActionFrequency, sTimefunc, 1);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sKeyboard
//
//
//
// ---------------------------------------------------------------------------

static void sKeyboard(unsigned char key, int x, int y)
{
    sOpenGL->KeyboardAction(key);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : static
// METHODE       : sSpecialKeys
//
//
//
// ---------------------------------------------------------------------------

static void sSpecialKeys(int key, int x, int y)
{
    sOpenGL->SpecialkeysAction(key);
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : InitGLUTCallback
//
//
// 22.2.2006 erstellt
//
// ---------------------------------------------------------------------------

void COpenGL::InitGLUTCallback(int Frequency)
{
    mActionFrequency = Frequency;

    sOpenGL = this;
    glutReshapeFunc(sReshapeWindow);
    glutDisplayFunc(sDisplayWindow);
    glutTimerFunc(Frequency, sTimefunc, 10);
    glutMouseFunc(sMouseButton);
    glutMotionFunc(sMouseMotion);
    glutPassiveMotionFunc(sMouseMotion);
    glutKeyboardFunc(sKeyboard);
    glutSpecialFunc(sSpecialKeys);
    glutMainLoop();
}


// ---------------------------------------------------------------------------
//
// KLASSE        : COpenGL
// METHODE       : SaveGLStrings
//
//
// 22.2.2006 erstellt
//
// ---------------------------------------------------------------------------

void COpenGL::SaveGLStrings(char* filename)
{
    int MaxTextureSize;

    ofstream fp(filename);

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);

    fp << "VENDOR:     " << glGetString(GL_VENDOR) << endl;
    fp << "RENDERER:   " << glGetString(GL_RENDERER) << endl;
    fp << "VERSION:    " << glGetString(GL_VERSION) << endl;
    fp << "MAXTEXTURE: " << MaxTextureSize << endl;

    fp.close();
}


// ---------------------------------------------------------------------------
//
// METHODE       : InitOpenGL
//
// KLASSE        : COpenGL
//
// RETURN        : -
//
// PARAMETER     : HWND handle - Handle des OpenGL-Window
//                 int width
//                 int height  - Breite und H�he des Fensters
//
// BESCHREIBUNG  : Initialisieren der Parameter
//
// HISTORY       :
//
// 1.11.1998 erstellt
//
// ---------------------------------------------------------------------------

bool COpenGL::InitWindowGLUT(int argc, char** argv, const char* WindowName, int Width, int Height, bool Fullscreen)
{
    glutInit(&argc, argv);
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    //cout << "glutInitDisplayMode OK" << endl;
    //
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    mWindowHandle = glutCreateWindow(WindowName);
    mAnaglyph = false;

    if (Fullscreen)
    {
        glutFullScreen();
    }

    //cout << "InitWindowGLUT SetViewport" << endl;

    //SetViewport(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    //cout << "InitWindowGLUT OK" << endl;

    return true;
}


// ---------------------------------------------------------------------------
//
// METHODE       : SetViewport
//
// KLASSE        : COpenGL
//
// RETURN        : -
//
// PARAMETER     : HWND handle - Handle des OpenGL-Window
//                 int width
//                 int height  - Breite und H�he des Fensters
//
// BESCHREIBUNG  : Initialisieren der Parameter
//
// HISTORY       :
//
// 1.11.1998 erstellt
//
// ---------------------------------------------------------------------------

void COpenGL::SetViewport(int w, int h)
{
    mWidth = w;
    mHeight = h;

    //glEnable(GL_CULL_FACE);
    //glDepthFunc(GL_LESS);		           // The Type Of Depth Test To Do
    //glDisable(GL_DEPTH_TEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    SetupLighting();

    glViewport(0, 0, mWidth, mHeight);
    mAspect = (float)mWidth / (mHeight);
    StartModelView();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    ViewportAction(w, h);
}


//---------------------------------------------------------------------------
//
// Klasse:    COpenGL
// Methode:   StartModelView
//
//
//---------------------------------------------------------------------------

void COpenGL::StartModelView()
{
    mAspect = (float)mWidth / (mHeight);
    glViewport(0, 0, mWidth, mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, mAspect, 0.1, 2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}


//---------------------------------------------------------------------------
//
// Klasse:    COpenGL
// Methode:   StartProjectionView
//
//
//---------------------------------------------------------------------------

void COpenGL::StartProjectionView()
{
    glViewport(0, 0, mWidth, mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, mWidth, 0.0, mHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
}


// ---------------------------------------------------------------------------
//
// METHODE       : SetupLighting
//
// KLASSE        : COpenGL
//
// RETURN        :
//
// PARAMETER     :
//
// BESCHREIBUNG  :
//
//  OpenGL Lichtquellen konfigurieren
//
// HISTORY       :
//
// 1.11.1998 erstellt
//
// ---------------------------------------------------------------------------

void COpenGL::SetupLighting()
{
    GLfloat Position1[] = { -3000.0, 0.0, 6000.0, 0, 0 };

    GLfloat matSpecular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat matShininess[] = { 30.0 };

/*
 *
 *  GLfloat BlueLight[]    = {0.0, 0.0, 1.0, 1.0};
 *  GLfloat OrangeLight[]  = {1.0, 0.5, 0.0, 1.0};
 *  GLfloat BlackLight[]   = {0,0,0, 1.0};
 *  GLfloat DarkLight[]    = { 0.5, 0.5, 0.5, 1.0 };
 */
    GLfloat WhiteLight[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat LightAmbient[] = { 0.2, 0.2, 0.2, 1.0 };

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);

    mLightPosition.Set(Position1[0], Position1[1], Position1[2]);

    glLightfv(GL_LIGHT0, GL_POSITION, Position1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, WhiteLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, WhiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
    glEnable(GL_NORMALIZE);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightAmbient);


    //glEnable(GL_DEPTH_TEST);

    // Rueckseiten der Polygone nicht zeichnen
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);


    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}
