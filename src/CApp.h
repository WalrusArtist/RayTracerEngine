#ifndef CAPP_H
#define CAPP_H

#include <SDL2/SDL.h>
#include "./waRayTrace/waImage.hpp"
#include "./waRayTrace/scene.hpp"
#include "./waRayTrace/camera.hpp"
class CApp {
    public:
        CApp();
        int OnExecute();
        bool OnInit();
        void OnEvent(SDL_Event *event);
        void OnLoop();
        void OnRender();
        void OnExit();
    private:
        void PrintVector(const qbVector<double> &inputVector);
    private:
        waImage m_image;
        waRT::Scene m_scene;
        // SDL2 STUFF
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
};

#endif