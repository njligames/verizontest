#include "Cubenado.hpp"
#include <stddef.h>
#include <iostream>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include <stdio.h>
#include <assert.h>

#include "Shader.hpp"

using namespace std;

namespace NJLI
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
    m_Shader(new Shader())
    {
        
    }
    
    Cubenado::~Cubenado()
    {
        delete m_Shader;
    }
    
    void Cubenado::create(int x, int y, int width, int height)
    {
        glEnable(GL_BLEND);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_DEPTH_TEST);
        
        glClearColor(0.52, 0.86, 0.99, 1.0f);
        
        glViewport(x, y, width, height);
        
        std::vector<std::string> attributes;
        attributes.push_back("inPosition");
        attributes.push_back("inTexCoord");
        attributes.push_back("inColor");
        attributes.push_back("inOpacity");
        attributes.push_back("inHidden");
        attributes.push_back("inTransform");
        attributes.push_back("inColorTransform");
        
        assert(m_Shader->load(loadFile("shaders/Shader.vsh"), loadFile("shaders/Shader.fsh"), attributes));
        
    }
    
    void Cubenado::destroy()
    {
        
    }
    
    void Cubenado::resize(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
                
    void Cubenado::update(float step)
    {
        
    }
    
    void Cubenado::render()
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
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