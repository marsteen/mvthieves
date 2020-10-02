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
using namespace std;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "CGL_Camera.h"

void CGL_Camera::LookAt()
{
    gluLookAt(mStandort.x, mStandort.y, mStandort.z,    // Standort der Kamera
        mVispoint.x, mVispoint.y, mVispoint.z,          // anvisierter Punkt
        mUpvector.x, mUpvector.y, mUpvector.z);         // Vektor, der "oben" angibt
}


//---------------------------------------------------------------------------
//
// Klasse:		CCamera
// Methode:
//
//
//---------------------------------------------------------------------------

void CGL_Camera::Reset()
{
    mStandort.Set(0, 0, -60.0);
    mVispoint.Set(0, 0, 0);
    mUpvector.Set(0, 1, 0);
}
