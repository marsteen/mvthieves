class CG3DColor
{
    public:



        CG3DColor()
        {
            mRGB[0] = 1.0;
            mRGB[1] = 1.0;
            mRGB[2] = 1.0;
            mRGB[3] = 1.0;
        }


        float mRGB[4];
        string mName;
};


class CG3DColorList : public vector<CG3DColor>
{
    public:

        float* FindColor(string& Colorname)
        {
            for (vector<CG3DColor>::iterator col = begin();
                col != end();
                col++)
            {
                if (col->mName == Colorname)
                {
                    return col->mRGB;
                }
            }
            return NULL;
        }
};
