class CG3DResourceTooltipp : public CG3DResourceBox
{
    public:

        CG3DResourceTooltipp();

        int UnscaledWidth()
        {
            return mContent->UnscaledWidth();
        }


        int UnscaledHeight()
        {
            return mContent->UnscaledHeight();
        }


        void AddResRef(CG3DResource* res);
        void Draw(SG3DDrawParams* dp);


    protected:

        CG3DResourceString* mContent;
};
