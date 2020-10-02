#define CPLAYFIELDSEGMEMT_H
#define CPLAYFIELDSEGMEMT_H


class CPlayfieldSegmemt
{

    void Insert(const CVectorUnit* vu);
    bool Test(const CVectorUnit* vu);


  public:

    set<CVectorUnit*> mUnitSet;
};


void CPlayfieldSegmemt::Insert(const CVectorUnit* vu)
{
  mUnitSet.insert(vu);
}

bool Test(const CVectorUnit* vu)
{
  if (
}


#endif
