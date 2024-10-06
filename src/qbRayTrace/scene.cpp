#include "scene.hpp"
#include <thread>
#include <vector>

waRT::Scene::Scene() {
	 
	m_camera.SetPosition(	qbVector<double>{std::vector<double> {0.0, -10.0, 0.0}} );
	m_camera.SetLookAt	( qbVector<double>{std::vector<double> {0.0, 0.0, 0.0}} );
	m_camera.SetUp			( qbVector<double>{std::vector<double> {0.0, 0.0, 1.0}} );
	m_camera.SetHorzSize(0.25);
	m_camera.SetAspect(16.0 / 9.0);
	m_camera.UpdateCameraGeometry();
	 
	m_objectList.push_back(std::make_shared<waRT::ObjSphere> (waRT::ObjSphere()));
	m_objectList.push_back(std::make_shared<waRT::ObjSphere> (waRT::ObjSphere()));
	m_objectList.push_back(std::make_shared<waRT::ObjSphere> (waRT::ObjSphere()));
	 
	waRT::GTform testMatrix1, testMatrix2, testMatrix3;
	testMatrix1.SetTransform(	qbVector<double>{std::vector<double>{-1.5, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});
														
	testMatrix2.SetTransform(	qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.75, 0.5, 0.5}});
														
	testMatrix3.SetTransform(	qbVector<double>{std::vector<double>{1.5, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
					qbVector<double>{std::vector<double>{0.75, 0.75, 0.75}});
														
	m_objectList.at(0) -> SetTransformMatrix(testMatrix1);
	m_objectList.at(1) -> SetTransformMatrix(testMatrix2);
	m_objectList.at(2) -> SetTransformMatrix(testMatrix3);
	
	m_objectList.at(0) -> m_baseColor = qbVector<double>{std::vector<double>{64.0, 128.0, 200.0}};
	m_objectList.at(1) -> m_baseColor = qbVector<double>{std::vector<double>{255.0, 128.0, 0.0}};
	m_objectList.at(2) -> m_baseColor = qbVector<double>{std::vector<double>{255.0, 200.0, 0.0}};
	
	m_lightList.push_back(std::make_shared<waRT::PointLight> (waRT::PointLight()));
	m_lightList.at(0) -> m_location = qbVector<double> {std::vector<double> {5.0, -10.0, -5.0}};
	m_lightList.at(0) -> m_color = qbVector<double> {std::vector<double> {255.0, 255.0, 255.0}};
}

bool waRT::Scene::Render(waImage &outputImage) {
    int xSize = outputImage.GetXSize();
    int ySize = outputImage.GetYSize();
    
    int numThreads = std::thread::hardware_concurrency();   
    std::vector<std::thread> threads;

    double xFact = 1.0 / (static_cast<double>(xSize) / 2.0);
    double yFact = 1.0 / (static_cast<double>(ySize) / 2.0);

     
    auto renderChunk = [&](int startX, int endX) {
        waRT::Ray cameraRay;
        qbVector<double> intPoint(3);
        qbVector<double> localNormal(3);
        qbVector<double> localColor(3);

        double minDist = 1e6;
        double maxDist = 0.0;

        for (int x = startX; x < endX; x++) {
            for (int y = 0; y < ySize; y++) {
                double normX = (static_cast<double>(x) * xFact) - 1.0;
                double normY = (static_cast<double>(y) * yFact) - 1.0;

                 
                m_camera.GenerateRay(normX, normY, cameraRay);

                bool hitObject = false;     
                qbVector<double> closestIntPoint(3);    
                qbVector<double> closestNormal(3);      
                qbVector<double> closestColor(3);       
                double closestDist = 1e6;               

                 
                for (auto currentObject : m_objectList) {
                    qbVector<double> tempIntPoint(3);
                    qbVector<double> tempNormal(3);
                    qbVector<double> tempColor(3);
                    
                    bool validInt = currentObject->TestIntersection(cameraRay, tempIntPoint, tempNormal, tempColor);
                    if (validInt) {
                        hitObject = true;

                         
                        double dist = (tempIntPoint - cameraRay.m_point1).norm();

                         
                        if (dist < closestDist) {
                            closestDist = dist;
                            closestIntPoint = tempIntPoint;
                            closestNormal = tempNormal;
                            closestColor = tempColor;
                        }
                    }
                }

                 
                if (hitObject) {
                    double intensity = 0.0;
                    bool validIllum = false;
                    qbVector<double> finalColor(3);

                    for (auto currentLight : m_lightList) {
                        validIllum = currentLight->ComputeIllumination(closestIntPoint, closestNormal, m_objectList, nullptr, finalColor, intensity);
                    }

                     
                    if (validIllum) {
                        outputImage.SetPixel(x, y, 
                                            closestColor.GetElement(0) * intensity,
                                            closestColor.GetElement(1) * intensity,
                                            closestColor.GetElement(2) * intensity);
                    } else {
                        outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);
                    }
                } else {
                    outputImage.SetPixel(x, y, 0.0, 0.0, 0.0);
                }
            }
        }
        std::cout << "Thread completed rendering from X=" << startX << " to X=" << endX << std::endl;
    };

    int chunkSize = xSize / numThreads;   

    for (int i = 0; i < numThreads; ++i) {
        int startX = i * chunkSize;
        int endX = (i == numThreads - 1) ? xSize : (i + 1) * chunkSize;   
        threads.emplace_back(renderChunk, startX, endX);   
    }

    for (auto &t : threads) {
        t.join();
    }

    return true;
}
