#ifndef CGL_DISK_H
#define CGL_DISK_H


class CGL_Disk : public CGL_Object
{
    public:

        void MakeObject();

        CVector3<float> mOrigin;
        int mSegments;
        float mRadius;
};

#endif
