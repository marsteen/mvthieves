#ifndef CPOLYLINEOBJECT_H
#define CPOLYLINEOBJECT_H

#include "CVectorObject.h"

class CPolylineObject : public CVectorObject
{
    public:

        int Inside(CPolylineObject& pb);

        void AddVertex(float x, float y);
        void Draw() const;
        void DrawPoints() const;

        void Clear() { mVerts.clear(); }
        void GetMinMax(CVector2<float>* Min, CVector2<float>* Max) const;
        void Translate(float xa, float ya);
        void Scale(float f);

    protected:

        std::vector<CVector2<float> > mVerts;
};

#endif
