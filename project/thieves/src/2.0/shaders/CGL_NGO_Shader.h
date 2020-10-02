#ifndef CGL_NGO_SHADER_H
#define CGL_NGO_SHADER_H

#include <fstream>
#include <string>
#include <vector>

struct SShaderAttrib
{
    std::string mAttName;
    int			mAttIndex;
};


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

class CGL_NGO_Shader
{
    public:

        bool InitShaders(const char* VertProg, const char* FragProg, const char* DebugFolder);
        bool SetUniform(const char* UniName, float f1);
        bool SetUniformInt(const char* UniName, int i1);
        bool SetUniform(const char* UniName, float f1, float f2, float f3);
        bool SetUniform(const char* UniName, float f1, float f2);

        bool SetAttrib(const char* AttName, float ax, float ay, float az);
        void BindAttrib(const char* AttName, int AttIndex);
        void AddShaderAttrib(const char* AttName, int AttIndex);


        void UseShader(bool t);
        void DeleteShaders();

        std::string mShaderMessage;

    protected:

        virtual void SetUniformShaders() = 0;

        bool CompileShader(const char* LogFile, GLuint Shader);
        void ShowInfoLog(std::ofstream& InfoLog);
        void BindShaderAtrribs();

        std::vector<SShaderAttrib> mShaderAtrribVec;

        GLuint mShaderProgram;
        GLuint mVertShader;
        GLuint mFragShader;
};



#endif
