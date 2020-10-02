/**********************************************************************
*
*
*  Simple Demo for GLSL 2.0
*
*  www.lighthouse3d.com
*
**********************************************************************/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cmath>


#include <GL/glew.h>
#include <CGL_NGO_BlurShader.h>

extern void GlobalShowOpenGLError(const char* mstr);

using namespace std;


//---------------------------------------------------------------------------
//
// Klasse:    CGL_NGO_BlurShader
// Methode:   SetUniformShaders
//
//
//---------------------------------------------------------------------------


void CGL_NGO_BlurShader::SetUniformShaders()
{
    //glActiveTexture (GL_TEXTURE0);
    SetUniformInt("uTexture0", 0);
}
