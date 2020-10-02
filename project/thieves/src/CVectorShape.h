#ifndef CVECTORSHAPE_H
#define CVECTORSHAPE_H

#include <string>
#include <vector>
#include "SShapeParams.h"
#include "CPolylineObject.h"
#include "CBoundingObject.h"


class CVectorShape
{
    public:

        CVectorShape();

        void SetBound(CBoundingObject& bound);
        void AddPoly(CPolylineObject& poly);
        void Draw(const SShapeParams& shape);
        void DrawPoints(const SShapeParams& shape);
        bool Inside(const CVectorShape& Other) const;
        bool Inside(float x, float y) const;
        void SetColor(float r, float g, float b, float a);

        //void TransformBounds();
        void Read(const char* Filename);
        void SetName(const std::string& name);
        void CenterShape();
        void Scale(float f);

        const float* Color() const { return mColor; }
        std::string PrintBounds() const;



        std::string mName;
        CVector2<float> mOrigin;
        const float* mColorPtr;
        float mPointStrength;

    protected:

        CBoundingObject mBound;
        std::vector<CPolylineObject> mPolylist;
        float mColor[4];


        float mRotation;
        unsigned int mDisplayListHandle;
        bool mDisplayListValid;
};

#endif
