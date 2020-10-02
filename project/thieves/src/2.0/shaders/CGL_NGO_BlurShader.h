#ifndef CGL_NGO_BLURSHADER_H
#define CGL_NGO_BLURSHADER_H

#include <CGL_NGO_Shader.h>


class CGL_NGO_BlurShader : public CGL_NGO_Shader
{
    public:


        CGL_NGO_BlurShader()
        {
        }


    protected:

        void SetUniformShaders();
};


#endif
