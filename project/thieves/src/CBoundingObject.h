#ifndef CBOUNDINGOBJECT_H
#define CBOUNDINGOBJECT_H

#include <vector>
#include "SShapeParams.h"
#include "CPolylineObject.h"

class CBoundingObject : public CPolylineObject
{
    public:

        void CreateBounds(const SShapeParams& sp);
        int Inside(const CBoundingObject& pb) const;
        bool Inside(float x, float y) const;
        void DrawBounds(const SShapeParams& sp);
        std::string Print() const;

    protected:

        std::vector<CVector2<float> > mBounds;
};

#endif
