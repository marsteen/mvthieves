//---------------------------------------------------------------------------
//
// Klasse:    C3DGResourceLine
//
//---------------------------------------------------------------------------

class CG3DResourceBox : public CG3DResource
{
    public:

        CG3DResourceBox();

        int UnscaledWidth()  { return mBrect.Width(); }
        int UnscaledHeight() { return mBrect.Height(); }

        void Draw(SG3DDrawParams* dp);
        void DrawBox(int RectNr, SG3DDrawParams* dp);

        bool HasStatus(int i)
        {
            return (mBoxColor != NULL) || (mFrameColor != NULL);
        }


        CRectT<int> mBrect;
        float* mBoxColor[3];
        float* mFrameColor[3];
        int mStdColor;

    protected:
};
