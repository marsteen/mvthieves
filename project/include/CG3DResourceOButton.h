//---------------------------------------------------------------------------
//
// Klasse:    C3DGResourceOButton
//
//---------------------------------------------------------------------------

class CG3DResourceOButton : public CG3DResourceButton
{
    public:

        CG3DResourceOButton();

        void Draw(SG3DDrawParams* dp);
        void Reset(int mode);
        bool Inside(SG3DDrawParams* dp, CVector2<int>* Mouse);
        void AddResRef(CG3DResource* res);
        void SetOptionParams(float x, float y, float sx, float sy);


    protected:

        void GetDropImagePos(SG3DDrawParams* dp, SG3DDrawParams* cdp);

        CG3DResourceImage* mDropImageRef; // Image reference
        CG3DResChild mContChild;
};
