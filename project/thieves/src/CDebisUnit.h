#ifndef CDEBRISUNIT_H
#define CDEBRISUNIT_H

class CDebris
{
    CVector2<float> mPos;
    CVector2<float> mDir;
    int mLife;
};


class CDebrisUnit
{
    public:

        void Init(float xpos, int ypos);

    protected:


        void Draw();

        CDebris* mDebris;
        int mDebrisCount;
};


void CDebrisUnit::Init(float xpos, int ypos)
{
    CRandom Random;

    int mDebrisCount = Random.GetInt(100, 200);

    mDebris = new CDebris[mDebrisCount];

    for (int i = 0; i < mDebrisCount; i++)
    {
        CDebris* db = mDebris + i;

        db->mPos.x = xpos;
        db->mPos.y = ypos;
        db->mDir.x = Random.Get(1, 8);
        db->mDir.y = Random.Get(1, 8);
    }
}


#endif
