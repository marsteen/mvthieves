#ifndef CVECTOROBJECT_H
#define CVECTOROBJECT_H

#include <CVector3T.h>

#ifndef DEG_TO_RAD_H
#define DEG_TO_RAD_H

#define DEG_TO_RAD(x)		((x) / 57.29577951)
#define RAD_TO_DEG(x)		((x) * 57.29577951)

#endif

#include "SShapeParams.h"

class CVectorObject
{
    public:

        CVectorObject();

        virtual void Draw() const = 0;

        void SetColor(float r, float g, float b, float a);

    protected:

        void TransformVertex(const CVector2<float>& v1, CVector2<float>& v2, const SShapeParams& sp);

        float mColor[4];
};

#endif
