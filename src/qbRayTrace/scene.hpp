#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <SDL2/SDL.h>
#include "qbImage.hpp"
#include "camera.hpp"
#include "objectsphere.hpp"

namespace qbRT
{
    class Scene
    {
    public:
        Scene();

        bool Render(qbImage &outputImage);

    private:
    private:
        qbRT::Camera m_camera;
        qbRT::ObjSphere m_testSphere;
    };
}
#endif