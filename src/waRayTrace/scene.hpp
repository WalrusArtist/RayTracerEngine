#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include "waImage.hpp"
#include "camera.hpp"
#include "./primitives/objectplane.hpp"
#include "./primitives/objectsphere.hpp"
#include "./lights/pointlight.hpp"

namespace waRT {
    class Scene {
    public:
        Scene();
        bool Render(waImage &outputImage);
    private:
    private:
        waRT::Camera m_camera;
        std::vector<std::shared_ptr<waRT::ObjectBase>> m_objectList;
        std::vector<std::shared_ptr<waRT::LightBase>> m_lightList;
        void renderChunk(int startX, int endX, int ySize, double xFact, double yFact, waImage &outputImage);
    };
}
#endif