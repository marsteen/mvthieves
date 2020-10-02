#ifndef COPENAL_H
#define COPENAL_H

#include <al.h>
#include <alut.h>

class COpenALSound
{
    public:

        ~COpenALSound()
        {
        }


        void LoadSound(const char* fname, bool looping);
        void SetSourceRelative();
        void PlaySound();
        void StopSound();
        void DestroySound();
        void SetSoundPosition(float x, float y, float z);
        void SetSoundVelocity(float vx, float vy, float vz);

    private:

        char* alBuffer;
        ALenum alFormatBuffer;
        ALsizei alFreqBuffer;
        long alBufferLen;
        ALboolean alLoop;
        unsigned int alSource;
        unsigned int alSampleSet;
};

class COpenAL
{
    public:

        ~COpenAL();

        void Init();
        void DestroyAll();

        void SetListenerPosition(float x, float y, float z);
        void SetListenerOrientation(float fx, float fy, float fz, float ux, float uy, float uz);

        COpenALSound* operator[](int i)
        {
            return mSounds[i];
        }


        unsigned int AddSound(const char* fname, bool looping);

    protected:

        unsigned int mSoundNr;
        vector<COpenALSound*> mSounds;
};

#endif
