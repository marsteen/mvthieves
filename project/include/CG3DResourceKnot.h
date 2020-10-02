class CG3DResourceKnot : public CG3DResource, public CG3DGroup
{
    public:

        void DisableRadioGroup()
        {
            if (mGroup >= 0)
            {
                mParent->DeactivateKnotGroup(this);
            }
        }


        int UnscaledWidth()  { return mParent->UnscaledWidth(); }
        int UnscaledHeight() { return mParent->UnscaledHeight(); }
};
