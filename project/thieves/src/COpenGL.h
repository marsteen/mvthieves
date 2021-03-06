#ifndef COPENGL_H
#define COPENGL_H

class COpenGL
{
    public:

        void SetViewport(int w, int h);
        void StartModelView();
        void StartProjectionView();
        void StartProjectionView(int Width, int Height) const;
        void SetupLighting();

    protected:

        int mWidth;
        int mHeight;
        float mAspect;
};

#endif
