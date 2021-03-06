//------------------------------------------------------------------------------
//
// PROJECT : OpenGL
//
// FILE    : COpenGL.h
//
// VERSION : 1.2
//
//
//------------------------------------------------------------------------------
//
// Inhalt: Deklaration der Klasse COpenGL
//
//------------------------------------------------------------------------------
//
//

#ifndef COPENGL_H
#define COPENGL_H

#include <CVector3T.h>
#include <CGL_Camera.h>

class CGL_Mouse : public CVector3<int>
{
    public:

        CGL_Mouse()
        {
            /*
             * mState[0] = GLUT_UP;
             * mState[1] = GLUT_UP;
             * mState[2] = GLUT_UP;
             * mState[3] = GLUT_UP;
             */
        }


        int mButton;
        int mState[4];
};


class COpenGL
{
    public:

        bool InitWindowGLUT(int argc, char** argv, const char* WindowName, int width, int height, bool Fullscreen);
        void Action();
        void MouseButtonAction();
        void MouseMotionAction();
        void KeyboardAction(unsigned char key);
        void SpecialkeysAction(int key);
        void StartModelView();
        void StartProjectionView();
        void SaveGLStrings(char* filename);
        void InitGLUTCallback(int fq);
        unsigned int* CreateSplitTextures(std::string& TextureFile, int XPatches, int YPatches);
        void SetViewport(int w, int h);
        void ViewportAction(int w, int h);

        int mActionFrequency;
        CGL_Mouse mMouse;
        CVector3<float> mLightPosition;
        CGL_Camera mCamera;
        bool mAnaglyph;

    protected:

        int mWindowHandle;
        int mWidth;
        int mHeight;
        float mAspect;



        void SetupLighting();
        void SetPixelFormatDescriptorBitmap();
};

#endif
