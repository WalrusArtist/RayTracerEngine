#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include "qbImage.hpp"
#include "camera.hpp"
#include "objectsphere.hpp"
#include "pointlight.hpp"

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
        std::vector<std::shared_ptr<qbRT::ObjectBase>> m_objectList;
        std::vector<std::shared_ptr<qbRT::LightBase>> m_lightList;
         
        void renderChunk(int startX, int endX, int ySize, double xFact, double yFact, qbImage &outputImage);
    };
}
#endif