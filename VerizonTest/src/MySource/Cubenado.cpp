#include "Cubenado.hpp"
#include <stddef.h>
#include <iostream>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include <stdio.h>
#include <assert.h>

#include "Shader.hpp"
#include "CubeGeometry.hpp"
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
    m_Camera(new Camera()),
    m_CameraNode(new Node()),
    m_Scene(new Scene()),
    m_Randomness(0.0f),
    m_NumberOfCubes(CubeGeometry::MAX_CUBES),
    m_Rotation(0.0f)
    {
        for (unsigned long i = 0; i < CubeGeometry::MAX_CUBES; i++)
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
        
        m_CameraNode->addCamera(m_Camera);
        m_CameraNode->setOrigin(btVector3(0.0f, 0.0f, 0.0f));
        
        m_Scene->addActiveNode(m_CameraNode);
        m_Scene->addActiveCamera(m_Camera);
        m_Scene->getRootNode()->setOrigin(btVector3(0.0f, 0.0f, 40.0f));
        
        assert(m_Shader->load(loadFile("shaders/Shader.vsh"), loadFile("shaders/Shader.fsh")));
        
        assert(m_ToonShader->load(loadFile("shaders/Toon.vsh"), loadFile("shaders/Toon.fsh")));
        
        m_CubeGeometry->load(m_Shader);
        
        float min_y = -7.0f;
        float xinc = 0.00001f;
        float yinc = 0.0014f;
        
        btTransform baseTransform(btTransform::getIdentity());
        baseTransform.setOrigin(btVector3(0.0f, 0.0f, 2.0f));
        
        btQuaternion rot(btVector3(0.0f, 1.0f, 0.0f), m_Rotation);
        
        float xx = 0.0f;
        float yy = min_y;
        
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            m_Scene->addActiveNode(node);
            m_Scene->getRootNode()->addChildNode(node);
            
            node->addGeometry(m_CubeGeometry);
            
            node->setOrigin(btVector3(0.0f, yy, 0.0f));
            
            node->getTornadoData()->setTranslationOffset(btVector3(xx, 0.0f, 0.0f));
            
            node->getTornadoData()->setBaseDegreesPerTimeStep(randomFloat(1.0f, 90.0f));
            
            node->getTornadoData()->setMaxDegreesPerTimestep(randomFloat(1.0f, 45.0f) + (randomFloat(1.0f, 45.0f) * m_Randomness));
            
            
            node->setColorBase(btVector4(randomFloat(0.0f, 1.0f),
                                         randomFloat(0.0f, 1.0f),
                                         randomFloat(0.0f, 1.0f), 1.0f));
            
            node->setScale(randomFloat(0.1f, 1.1f));
            
            xx+=xinc;
            yy+=yinc;
        }
        
    }
    
    void Cubenado::destroy()
    {
        m_CubeGeometry->unLoad();
        m_Shader->unLoad();
    }
    
    void Cubenado::resize(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
                
    void Cubenado::update(float step)
    {
        long cubesToDraw = m_NumberOfCubes;
        
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
//            node->getTornadoData()->setMaxDegreesPerTimestep(randomFloat(1.0f, 45.0f) + (randomFloat(1.0f, 45.0f) * m_Randomness));
            
            node->getTornadoData()->update(step);
            
            
            btVector3 axis = randomPosition(btVector3(0.0f, 0.0f, 0.0f), btVector3(1.0f, 1.0f, 1.0f)).normalized();
            node->setRotation(node->getRotation() * btQuaternion(axis, step + (step * m_Randomness)));
            
            node->setTransform(node->getTornadoData()->getBaseTransform() * node->getTransform());
            
            
            node->setNormalMatrix(node->getTransform().getBasis().inverse().transpose());
            
            node->enableHideGeometry(false);
            if(cubesToDraw < 0)
                node->enableHideGeometry();
            cubesToDraw--;
                
        }
        
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
        if (amount > CubeGeometry::MAX_CUBES)
        {
            m_NumberOfCubes = CubeGeometry::MAX_CUBES;
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
