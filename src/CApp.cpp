#include "CApp.h"
#include "./qbRayTrace/qbLinAlg/qbVector.h"

// const (default)
CApp::CApp()
{
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
}

bool CApp::OnInit()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return false;
    }

    pWindow = SDL_CreateWindow("YADAYAD", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);

    if (pWindow != NULL)
    {
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
        // Init the image
        m_image.Initialize(1280, 720, pRenderer);

        qbRT::Camera testCamera;
        testCamera.SetPosition(qbVector<double>(std::vector<double>{0.0, 0.0, 0.0}));
        testCamera.SetLookAt(qbVector<double>(std::vector<double>{0.0, 2.0, 0.0}));
        testCamera.SetUp(qbVector<double>(std::vector<double>{0.0, 0.0, 1.0}));
        testCamera.SetLength(1.0);
        testCamera.SetHorzSize(1.0);
        testCamera.SetAspect(1.0);
        testCamera.UpdateCameraGeometry();

        // test printouts
        auto screenCentre = testCamera.GetScreenCenter();
        auto screenU = testCamera.GetU();
        auto screenV = testCamera.GetV();

        std::cout << "Camera screen cetre:" << std::endl;
        PrintVector(screenCentre);
        std::cout << "\nCamera U VEC:" << std::endl;
        PrintVector(screenU);
        std::cout << "\nCamera V VEC:" << std::endl;
        PrintVector(screenV);
    }
    else
    {
        return false;
    }
    return true;
}

int CApp::OnExecute()
{
    SDL_Event event;

    if (OnInit() == false)
    {
        return -1;
    }

    while (isRunning)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            OnEvent(&event);
        }
        OnLoop();
        OnRender();
    }
}

void CApp::OnEvent(SDL_Event *event)
{
    if (event->type == SDL_QUIT)
    {
        isRunning = false;
    }
}

void CApp::OnLoop()
{
}

void CApp::OnRender()
{
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);

    // render scene
    m_scene.Render(m_image);

    // display image
    m_image.Display();

    // result
    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit()
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

// private funks

void CApp::PrintVector(const qbVector<double> &inputVector)
{
    int nRows = inputVector.GetNumDims();
    for (int row = 0; row < nRows; ++row)
    {
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}