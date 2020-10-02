#ifndef CGL_MATOBJECT_H
#define CGL_MATOBJECT_H

#define DEG_TO_RAD(x)		((x) / 57.29577951)
#define RAD_TO_DEG(x)		((x) * 57.29577951)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CGL_MatObject
{
    public:

        GLfloat mMat[16];

        void MatInit();
        void MatCopy(GLfloat* SrcMat);
        void MatCopy(GLfloat* SrcMat, GLfloat* DstMat);
        void MatApply(GLfloat* mx);

        void MatSetIdentity(GLfloat* mx);
        void MatSetIdentity();

        void MatTransform();

        void MatRotateX(float angle);
        void MatRotateY(float angle);
        void MatRotateZ(float angle);

        void MatRotateX(GLfloat* mat, float angle);
        void MatRotateY(GLfloat* mat, float angle);
        void MatRotateZ(GLfloat* mat, float angle);

        void CalcRotationMatrix(float mx[16], CVector3<float>* v, float angle);
        void MatRotateVector(CVector3<float>* v, float angle);
        void MatRotateVector(CVector3<float>* v1, CVector3<float>* v2, float angle, int Count);
        void MatTransformVertex(float Mat[16], CVector3<float>* sv, CVector3<float>* dv);

        void MatTranslate(float x, float y, float z);



        void MatScale(GLfloat s);
        void MatScale(GLfloat x, GLfloat y, GLfloat z);
};

#endif // CGL_MATOBJECT
