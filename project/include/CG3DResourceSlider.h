class CG3DResourceSlider : public CG3DResourceButton
{
    public:

        void SetSliderLimits(float Slmin, float Slmax)
        {
            mSliderMin = Slmin;
            mSliderMax = Slmax;
        }


        EG3DSliderType mSliderType;
        float mSliderPos;
        float mSliderMin;
        float mSliderMax;
};
