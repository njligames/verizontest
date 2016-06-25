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

using namespace std;

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
    m_CubeGeometry(new CubeGeometry()),
    m_Camera(new Camera()),
    m_CameraNode(new Node()),
    m_RootNode(new Node()),
    m_Scene(new Scene())
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
        
        delete m_RootNode;
        m_RootNode = NULL;
        
        delete m_CameraNode;
        m_CameraNode = NULL;
        
        delete m_Camera;
        m_Camera = NULL;
        
        delete m_CubeGeometry;
        m_CubeGeometry = NULL;
        
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
        
        assert(m_Shader->load(loadFile("shaders/Shader.vsh"), loadFile("shaders/Shader.fsh")));
        
        m_CubeGeometry->load(m_Shader);
        
        float z = 20.0f;
        float min_y = -7.0f;
        float max_y = 7.0f;
        float inc = 1.5;
        
        float yy = min_y;
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            m_Scene->addActiveNode(node);
            
            node->addGeometry(m_CubeGeometry);
            
            node->setOrigin(btVector3(0.0f, yy, z));
            
            yy+=inc;
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
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            node->setRotation(node->getRotation() * btQuaternion(btVector3(0.0f, 1.0f, 0.0f), step));
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
}