#ifndef CVERTEXARRAYS
#define CVERTEXARRAYS

#include <CVector3T.h>

class CVertexArrays
{
    public:

        CVertexArrays()
        {
            mVertex = NULL;
            mNormal = NULL;
            mTexCoord = NULL;
            mIndex = NULL;
            mIndexInt = NULL;
        }


        ~CVertexArrays()
        {
            ClearArrays();
        }


        void ClearArrays()
        {
            delete[] mVertex;
            delete[] mNormal;
            delete[] mIndex;
            delete[] mTexCoord;
            delete[] mIndexInt;

            mVertex = NULL;
            mNormal = NULL;
            mIndex = NULL;
            mTexCoord = NULL;
            mIndexInt = NULL;
        }


        CVector3<float>*      MakeVertexArray(int e)    { return mVertex = new CVector3<float> [e]; }
        CVector3<float>*      MakeNormalArray(int e)    { return mNormal = new CVector3<float> [e]; }
        CVector2<float>*      MakeTexCoordArray(int e)  { return mTexCoord = new CVector2<float> [e]; }
        unsigned short*       MakeIndexArray(int e)     { return mIndex = new unsigned short[e]; }
        unsigned int*         MakeIndexArrayInt(int e)  { return mIndexInt = new unsigned int[e]; }

        CVector3<float>* mVertex;
        CVector3<float>* mNormal;
        CVector2<float>* mTexCoord;
        unsigned short* mIndex;
        unsigned int* mIndexInt;
};

#endif // CVERTEXARRAYS
