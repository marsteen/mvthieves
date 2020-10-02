#ifndef CRANDOM_H
#define CRANDOM_H

class CRandom
{
    public:

        static float Get01();
        static float Get(float Low, float High);
        static int GetInt(int Low, int High);

        // Liefert 1 oder -1
        static int Sign()
        {
            return (GetInt(0, 1) == 0) ? 1 : -1;
        }
};

#endif
