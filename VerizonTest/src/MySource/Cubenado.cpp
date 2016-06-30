#include "Cubenado.hpp"
#include <stddef.h>
#include <iostream>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include <stdio.h>
#include <assert.h>

#include "Shader.hpp"
#include "CubeGeometry.hpp"
#include "RectangleGeometry.hpp"
#include "MeshGeometry.hpp"
#include "Node.hpp"
#include "Scene.hpp"
#include "TornadoData.hpp"

#include "btQuaternion.h"

using namespace std;

static float randomFloat(float min, float max)
{
    float r = (float)rand() / (float)RAND_MAX;
    return min + r * (max - min);
}

namespace njli
{
    Cubenado *Cubenado::s_Instance = NULL;
    std::string *Cubenado::s_BundlePath = NULL;
    
    Cubenado *const Cubenado::getInstance()
    {
        return s_Instance;
    }
    
    void Cubenado::createInstance()
    {
        if(NULL == s_Instance)
        {
            s_Instance = new Cubenado();
            s_BundlePath = new std::string();
        }
    }
    
    void Cubenado::destroyInstance()
    {
        if(s_Instance)
        {
            delete s_BundlePath;
            delete s_Instance;
        }
        s_Instance = NULL;
    }
    
    void Cubenado::setBundlePath(const std::string &bundlePath)
    {
        *s_BundlePath = bundlePath;
    }
    
    Cubenado::Cubenado():
    m_Shader(new Shader()),
    m_ToonShader(new Shader()),
    m_CubeGeometry(new CubeGeometry()),
    m_RectangleGeometry(new RectangleGeometry()),
    m_MeshGeometry(new MeshGeometry()),
    m_Camera(new Camera()),
    m_CameraNode(new Node()),
    m_Scene(new Scene()),
    m_Randomness(0.0f),
    m_NumberOfCubes(Geometry::MAX_CUBES),
    m_Rotation(0.0f)
    {
        for (unsigned long i = 0; i < Geometry::MAX_CUBES; i++)
            m_CubeNodes.push_back(new Node());
    }
    
    Cubenado::~Cubenado()
    {
        while (!m_CubeNodes.empty())
        {
            Node *node = m_CubeNodes.back();
            m_CubeNodes.pop_back();
            delete node;
        }
        
        delete m_Scene;
        m_Scene = NULL;
        
        delete m_CameraNode;
        m_CameraNode = NULL;
        
        delete m_Camera;
        m_Camera = NULL;
        
        delete m_MeshGeometry;
        m_MeshGeometry = NULL;
        
        delete m_RectangleGeometry;
        m_RectangleGeometry = NULL;
        
        delete m_CubeGeometry;
        m_CubeGeometry = NULL;
        
        delete m_ToonShader;
        m_ToonShader = NULL;
        
        delete m_Shader;
        m_Shader = NULL;
    }
    
    void Cubenado::create(int x, int y, int width, int height)
    {
        glEnable(GL_DEPTH_TEST);
        
        srand((unsigned int)time(0));
        
        m_CameraNode->addCamera(m_Camera);
        m_CameraNode->setOrigin(btVector3(0.0f, 0.0f, 0.0f));
        
        m_Scene->addActiveNode(m_CameraNode);
        m_Scene->addActiveCamera(m_Camera);
        m_Scene->getRootNode()->setOrigin(btVector3(-10.0f, -10.0f, 130.0f));
        
        assert(m_Shader->load(loadFile("shaders/Shader.vsh"), loadFile("shaders/Shader.fsh")));
        
        assert(m_ToonShader->load(loadFile("shaders/Toon.vsh"), loadFile("shaders/Toon.fsh")));
        
        m_MeshGeometry->load(m_Shader, loadFile("models/cube.obj"));
//        m_RectangleGeometry->load(m_Shader);
        m_CubeGeometry->load(m_Shader);
        
        setStartPositions();
        
        
        btTransform baseTransform(btTransform::getIdentity());
        baseTransform.setOrigin(btVector3(0.0f, 0.0f, 2.0f));
        
        btQuaternion rot(btVector3(0.0f, 1.0f, 0.0f), m_Rotation);
        
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            m_Scene->addActiveNode(node);
            m_Scene->getRootNode()->addChildNode(node);
            
            node->addGeometry(m_MeshGeometry);
            
//            node->getTornadoData()->setBaseDegreesPerTimeStep(randomFloat(1.0f, 90.0f));
//            
//            node->getTornadoData()->setMaxDegreesPerTimestep(1.0f);
//            node->getTornadoData()->setMaxDegreesPerTimestep(randomFloat(1.0f, 45.0f) + (randomFloat(1.0f, 45.0f) * m_Randomness));
            
            
            node->setColorBase(btVector4(randomFloat(0.0f, 1.0f),
                                         randomFloat(0.0f, 1.0f),
                                         randomFloat(0.0f, 1.0f), 1.0f));
            
//            node->setScale(randomFloat(0.8f, 1.1f));
        }
        
    }
    
    void Cubenado::destroy()
    {
        m_MeshGeometry->unLoad();
        m_RectangleGeometry->unLoad();
        m_CubeGeometry->unLoad();
        m_Shader->unLoad();
    }
    
    void Cubenado::resize(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
                
    void Cubenado::update(float step)
    {
//        long cubesToDraw = m_NumberOfCubes;
//        
//        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
//             i != m_CubeNodes.end();
//             i++)
//        {
//            Node *node = *i;
//            
////            node->getTornadoData()->setMaxDegreesPerTimestep(randomFloat(1.0f, 45.0f) + (randomFloat(1.0f, 45.0f) * m_Randomness));
//            
//            node->getTornadoData()->update(step);
//            
//            
//            btVector3 axis = randomPosition(btVector3(0.0f, 0.0f, 0.0f), btVector3(1.0f, 1.0f, 1.0f)).normalized();
////            node->setRotation(node->getRotation() * btQuaternion(axis, step + (step * m_Randomness)));
//            
//            node->setTransform(node->getTornadoData()->getBaseTransform() * node->getTransform());
//            
//            
//            node->setNormalMatrix(node->getTransform().getBasis().inverse().transpose());
//            
//            node->enableHideGeometry(false);
//            if(cubesToDraw < 0)
//                node->enableHideGeometry();
//            cubesToDraw--;
//                
//        }
        
        m_Scene->update(step);
    }
    
    void Cubenado::render()
    {
        glClearColor(0.52, 0.86, 0.99, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m_Scene->render();
    }
    
    Camera *const Cubenado::getCamera()const
    {
        return m_Camera;
    }
    
    void Cubenado::setNumberOfCubes(const unsigned int amount)
    {
        if (amount > Geometry::MAX_CUBES)
        {
            m_NumberOfCubes = Geometry::MAX_CUBES;
        }
        m_NumberOfCubes = amount;
    }
    
    void Cubenado::setRandomness(const float percent)
    {
        assert(percent >= 0.0f && percent <= 1.0f);
        m_Randomness = percent;
        
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            node->getTornadoData()->setMaxDegreesPerTimestep(node->getTornadoData()->getBaseDegreesPerTimeStep() + (randomFloat(1.0f, 10.0f) * m_Randomness));
            
        }
    }
    
    float Cubenado::getRandomness()const
    {
        return m_Randomness;
    }
    
    std::string Cubenado::loadFile(const std::string filepath)
    {
        std::string filedata("");
        
        std::string fullPath = *s_BundlePath + filepath;
        
        FILE *file = fopen(fullPath.c_str(), "rb");
        
        if(file)
        {
            fseek(file, 0, SEEK_END);
            long fileSize = ftell(file);
            fseek(file, 0, SEEK_SET);
            
            char* buffer = (char*)malloc(fileSize);
            buffer[fileSize] = '\0';
            
            fread(buffer, 1, fileSize, file);
            filedata = std::string(buffer);
            
            fclose(file);
            free(buffer);
        }
        
        return filedata;
    }
    
    void Cubenado::setStartPositions()
    {
        float startX = 0.0f;
        float startY = 0.0f;
        float startZ = 0.0f;
        
        float xinc = 2.0f;
        float xmargin = 0.1;
        
        float yinc = 2.0f;
        float ymargin = 0.1f;
        
        float zinc = 2.0f;
        float zmargin = 0.1;
        
        
//        int currentIndex = 1;
//        int amountHigh = sqrt(Geometry::MAX_CUBES);
//        int currentIndexIncrement = 1;
        
        float currentX = startX;
        float currentY = startY;
        float currentZ = startZ;
        
//        float currentBaseX = 0.0f;
//        float currentBaseZ = 0.0f;
        
        float dim = m_CubeNodes.size();
//        dim = sqrt(dim);
        dim = pow(dim, 1.0/3.0);
        unsigned long idx = 0;
        
        for (int x = 0; x < dim; x++)
        {
            for (int y = 0; y < dim; y++)
            {
                for (int z = 0; z < dim; z++)
                {
                    if(idx < m_CubeNodes.size())
                    {
                        Node *node = m_CubeNodes.at(idx++);
                        if(node)
                            node->setOrigin(btVector3(currentX, currentY, currentZ));
                    }
                    currentZ += (zinc + zmargin);
                }
                currentZ = startZ;
                currentY += (yinc + ymargin);
            }
            currentY = startY;
            currentX += (xinc + xmargin);
        }
//        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
//             i != m_CubeNodes.end();
//             i++)
//        {
//            Node *node = *i;
//            
//            node->setOrigin(btVector3(0.0f, currentY, 0.0f));
////            while(currentIndex <= (currentIndexIncrement * currentIndexIncrement))
////            {
////                while(currentX <  )
////                node->setOrigin(btVector3(0.0f, currentY, 0.0f));
////                node->getTornadoData()->setTranslationOffset(btVector3(currentX, 0.0f, currentZ));
////                currentIndex++;
////            }
////            
////            currentIndexIncrement++;
////            
////            currentBaseX -= (xinc + xmargin);
////            currentBaseZ -= (zinc + zmargin);
////            
////            currentX = currentBaseX;
//            currentY += (yinc + ymargin);
////            currentZ = currentBaseZ;
//            
//            
//        }
    }
    
    btVector3 Cubenado::randomPosition(const btVector3 &min, const btVector3 &max)const
    {
        assert(min.x() <= max.x());
        assert(min.y() <= max.y());
        assert(min.z() <= max.z());
        
        return btVector3(randomFloat(min.x(), max.x()),
                      randomFloat(min.y(), max.y()),
                      randomFloat(min.z(), max.z()));
    }
    
    btQuaternion Cubenado::randomRotation(const btVector3 &axis, const float degreesMin, const float degreesMax)const
    {
        assert(degreesMin <= degreesMax);
        
        return btQuaternion(axis, randomFloat(degreesMin, degreesMax)).normalized();
    }
    
    btQuaternion Cubenado::randomRotation(const float degreesMin, const float degreesMax)const
    {
        return randomRotation(randomPosition(btVector3(0,0,0), btVector3(1,1,1)).normalized(), degreesMin, degreesMax);
    }
    
    btQuaternion Cubenado::randomRotation()const
    {
        return randomRotation(randomPosition(btVector3(0,0,0), btVector3(1,1,1)).normalized(), btRadians(0), btRadians(360));
    }
}
