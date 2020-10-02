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
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <GlobalDebug.h>
#include <CFileIO.h>


#include <GL/glew.h>


#include <CGL_NGO_Shader.h>

extern void GlobalShowOpenGLError(const char* mstr);
extern std::string GlobFile(const char* fname);

static const char* DebugFolder;

using namespace std;



// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

bool CGL_NGO_Shader::CompileShader(const char* LogFileName, GLuint Shader)
{
    bool r;
    GLint Success;

    glCompileShader(Shader);
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);

    if (Success == GL_FALSE)
    {
        if (LogFileName != NULL)
        {
            int OutLenght;
            char* LogFileBuffer = new char[16000];
            int OutLength;
            glGetShaderInfoLog(Shader, 16000, &OutLenght, LogFileBuffer);

            //GlobalDebug(LogFileBuffer, DBG_MAPS);

            cout << "**********************************************************" << endl;
            cout << "LogFileName=" << LogFileName << endl;
            cout << LogFileBuffer << endl;
            cout << "**********************************************************" << endl;
        }
    }
    else
    {
        //cout << "+++++ Compile Shader OK +++++ " << LogFileName << endl;
        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::ShowInfoLog(ofstream& InfoLogStream)
{
    int Length;
    char* InfoLog = new char[16000];

    glGetProgramInfoLog(mShaderProgram, 16000, &Length, InfoLog);

    if (strlen(InfoLog) > 0)
    {
        cout << "-------------------------------------------------" << endl;
        cout << InfoLog << endl;
        cout << "-------------------------------------------------\n" << endl;
    }
    delete[] InfoLog;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::BindShaderAtrribs()
{
    vector<SShaderAttrib>::iterator it;

    for (it = mShaderAtrribVec.begin(); it != mShaderAtrribVec.end(); ++it)
    {
        glBindAttribLocation(mShaderProgram, it->mAttIndex, it->mAttName.c_str());
    }
    mShaderAtrribVec.clear();
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::AddShaderAttrib(const char* Name, int Index)
{
    SShaderAttrib satt;

    satt.mAttName = Name;
    satt.mAttIndex = Index;

    mShaderAtrribVec.push_back(satt);
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------



bool CGL_NGO_Shader::InitShaders(const char* VertProg, const char* FragProg, const char* DebugFolder)
{
    cout << "InitShaders START" << endl;

    bool r;

    ofstream InfoLog;
    CFileIO fio;
    int FileSize;
    struct stat statBlock;
    const char* FragShaderLog;
    const char* VertShaderLog;

    string VertProgPath = VertProg;
    string FragProgPath = FragProg;

    if (DebugFolder != NULL)
    {
        string filename = string(DebugFolder) + "/ShaderInfoLog.txt";
        InfoLog.open(filename.c_str());

        VertShaderLog = (string(DebugFolder) +  "/VertShaderLog.txt").c_str();
        FragShaderLog = (string(DebugFolder) +  "/FragShaderLog.txt").c_str();
    }

    if (stat(VertProgPath.c_str(), &statBlock) != 0)
    {
        cout << "***** Vert program file error:" << VertProg << endl;
    }
    else
    if (stat(FragProgPath.c_str(), &statBlock) != 0)
    {
        cout << "***** Frag program file error:" << FragProg << endl;
    }
    else
    {
        mVertShader = glCreateShader(GL_VERTEX_SHADER);
        mFragShader = glCreateShader(GL_FRAGMENT_SHADER);

        const char* vs = (const char*)fio.ReadFile(VertProgPath.c_str(), &FileSize);
        const char* fs = (const char*)fio.ReadFile(FragProgPath.c_str(), &FileSize);

        if (vs == NULL)
        {
            cout << "***** Vertex Shader file failed:" << VertProg << endl;
        }

        if (fs == NULL)
        {
            cout << "***** Fragment Shader file failed!" << FragProg << endl;
        }



        const char* ff = fs;
        const char* vv = vs;

        glShaderSource(mVertShader, 1, &vv, NULL);
        glShaderSource(mFragShader, 1, &ff, NULL);

        cout << "-------------------------------------------------" << endl;
        cout << "Vert Shader=" << vs << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Frag Shader=" << fs << endl;
        cout << "-------------------------------------------------" << endl;


        CompileShader("Vert Shader", mVertShader);
        CompileShader("Frag Shader", mFragShader);



        mShaderProgram = glCreateProgram();
        ShowInfoLog(InfoLog);

        cout << "mVertShader    = " << mVertShader << endl;
        cout << "mFragShader    = " << mFragShader << endl;
        cout << "mShaderProgram = " << mShaderProgram << endl;


        glAttachShader(mShaderProgram, mVertShader);

        glAttachShader(mShaderProgram, mFragShader);

        BindShaderAtrribs();

        glLinkProgram(mShaderProgram);
        ShowInfoLog(InfoLog);

        if (glIsProgram(mShaderProgram))
        {
            cout << "Program OK" << endl;
            r = true;
        }
        else
        {
            cout << "Program FAILED" << endl;
            r = false;
        }

        {
            int prog;
            glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
            cout << "Prog=" << prog << endl;
        }



        glValidateProgram(mShaderProgram);
        GlobalShowOpenGLError("glValidateProgram");
        ShowInfoLog(InfoLog);


        {
            GLint params[8];
            glGetProgramiv(mShaderProgram, GL_VALIDATE_STATUS, params);

            if (params[0] == GL_FALSE)
            {
                cout << "***** GL_VALIDATE_STATUS failed" << endl;
            }


            glGetProgramiv(mShaderProgram, GL_ATTACHED_SHADERS, params);
            cout << "Attached Shaders:" << params[0] << endl;

            glGetProgramiv(mShaderProgram, GL_LINK_STATUS, params);
            cout << "Link status=" << params[0] << endl;
        }

        glUseProgram(mShaderProgram);
        SetUniformShaders();

        glUseProgram(0);

        delete[] vs;
        delete[] fs;

        InfoLog.close();
    }


    //DeleteShaders();



    return r;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::DeleteShaders()
{
    glUseProgram(0);

    glDetachShader(mShaderProgram, mVertShader);
    glDetachShader(mShaderProgram, mFragShader);

    glDeleteShader(mVertShader);
    glDeleteShader(mFragShader);

    glDeleteProgram(mShaderProgram);
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::UseShader(bool t)
{
    if (t)
    {
        // Use shaders

        glUseProgram(mShaderProgram);
    }
    else
    {
        // Use fixed

        glUseProgram(0);
    }
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

bool CGL_NGO_Shader::SetUniform(const char* UniName, float f1, float f2)
{
    bool r;
    GLint val = glGetUniformLocation(mShaderProgram, UniName);

    if (val == -1)
    {
        cout << "***** uniform not found:" << UniName << endl;
        r = false;
    }
    else
    {
        glUniform2f(val, f1, f2);
        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

bool CGL_NGO_Shader::SetUniform(const char* UniName, float f1)
{
    bool r;
    GLint val = glGetUniformLocation(mShaderProgram, UniName);

    if (val == -1)
    {
        cout << "***** uniform not found:" << UniName << endl;
        r = false;
    }
    else
    {
        glUniform1f(val, f1);
        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

void CGL_NGO_Shader::BindAttrib(const char* AttName, int AttIndex)
{
    glBindAttribLocation(mShaderProgram, AttIndex, AttName);
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

bool CGL_NGO_Shader::SetUniform(const char* UniName, float f1, float f2, float f3)
{
    bool r;
    GLint val = glGetUniformLocation(mShaderProgram, UniName);

    if (val == -1)
    {
        cout << "***** uniform not found:" << UniName << endl;
        r = false;
    }
    else
    {
        glUniform3f(val, f1, f2, f3);
        r = true;
    }
    return r;
}


/*
 * // ---------------------------------------------------------------------------
 * //
 * //
 * //
 * //
 * // ---------------------------------------------------------------------------
 *
 * bool CGL_NGO_Shader::SetUniformInt(const char* UniName, int i1)
 * {
 *  bool r;
 *  GLint val = glGetUniformLocation(mShaderProgram, UniName);
 *  if (val == -1)
 *  {
 *      cout << "***** uniform not found:" << UniName << endl;
 *      r = false;
 *  }
 *  else
 *  {
 *      glUniform1i(val, i1);
 *      r = true;
 *  }
 *  return r;
 * }
 */

// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------


bool CGL_NGO_Shader::SetUniformInt(const char* UniName, int i1)
{
    bool r;
    GLint val = glGetUniformLocation(mShaderProgram, UniName);

    if (val == -1)
    {
        cout << "***** uniform not found:" << UniName << endl;
        r = false;
    }
    else
    {
        glUniform1i(val, i1);
        r = true;
    }
    return r;
}


// ---------------------------------------------------------------------------
//
//
//
//
// ---------------------------------------------------------------------------

bool CGL_NGO_Shader::SetAttrib(const char* AttName, float ax, float ay, float az)
{
    bool r;
    GLint val = glGetAttribLocation(mShaderProgram, AttName);

    if (val == -1)
    {
        cout << "***** attrib not found:" << AttName << endl;
        r = false;
    }
    else
    {
        glVertexAttrib3f(val, ax, ay, az);
        r = true;
    }
    return r;
}
