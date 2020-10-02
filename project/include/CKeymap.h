#ifndef CKeymap_H
#define CKeymap_H

class CKeymap
{
    public:

        CKeymap();

        static bool ReadMapFile(const char* filename);
        static int GetMappedKey(int key);

    protected:
};

#endif
