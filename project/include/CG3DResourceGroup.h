class CG3DGroup
{
    public:

        CG3DGroup()
        {
            mGroup = -1;
        }


        void CG3DResourceButton::DisableRadioGroup()
        {
            if (mGroup >= 0)
            {
                mGroupParent->DeactivateButtonGroup(this);
            }
        }


        void SetGroupParent(CG3DResource* gp) { mGroupParent = gp; }

        int mGroup;                     // Gruppe
        CG3DResource* mGroupParent;
};
