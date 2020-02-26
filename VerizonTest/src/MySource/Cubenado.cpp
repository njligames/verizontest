#include "Cubenado.hpp"
#include <stddef.h>
#include <iostream>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include <stdio.h>
#include <assert.h>

#include "Shader.h"
//#include "CubeGeometry.hpp"
//#include "RectangleGeometry.hpp"
#include "MeshGeometry.h"
#include "Node.h"
#include "Scene.h"
#include "AbstractFrameBuffer.hpp"
#include "SceneFrameBuffer.hpp"

//#include "btQuaternion.h"

#define EDIT_SHADER

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
//    m_ToonShader(new Shader()),
//    m_RimLite(new Shader()),
//    m_Phong(new Shader()),
//    m_PixelLighting(new Shader()),
//    m_Gourand(new Shader()),
    m_Geometry(new MeshGeometry()),
    m_Camera(new Camera()),
    m_CameraNode(new Node()),
    m_Scene(new Scene()),
    m_Randomness(1.0f),
    m_NumberOfCubes(Cubenado::MAX_CUBES),
    m_Rotation(0.0f)
    {
        
#ifdef EDIT_SHADER
        m_CubeNodes.push_back(new Node());
        
//        unsigned int width = 1242;
//        unsigned int height = 2208;
//        
//        for (unsigned long i = 0; i < 12; i++)
//        {
//            m_FrameBuffer[i] = new SceneFrameBuffer();
//            m_FrameBuffer[i]->setScene(m_Scene);
//        }
        
//        m_FrameBuffer[0]->load                 (width, height, true);
//        m_FrameBuffer[1]->load                 (width, height, false);
//        m_FrameBuffer[2]->loadTexture2D        (NULL, NULL, width, height, true, false);
//        m_FrameBuffer[3]->loadTexture2D        (NULL, NULL, width, height, true, true);
//        m_FrameBuffer[4]->loadTextureCube      (NULL, NULL, width, height, true, false);
//        m_FrameBuffer[5]->loadTextureCube      (NULL, NULL, width, height, true, true);
//        m_FrameBuffer[6]->loadColorTexture2D   (NULL, width, height, true);
//        m_FrameBuffer[7]->loadDepthTexture2D   (NULL, width, height, true, false);
//        m_FrameBuffer[8]->loadDepthTexture2D   (NULL, width, height, true, true);
//        m_FrameBuffer[9]->loadColorTextureCube (NULL, width, height, true);
//        m_FrameBuffer[10]->loadDepthTextureCube (NULL, width, height, true, false);
//        m_FrameBuffer[11]->loadDepthTextureCube(NULL, width, height, true, true);
#else
        for (unsigned long i = 0; i < Cubenado::MAX_CUBES; i++)
            m_CubeNodes.push_back(new Node());
#endif
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
        
        delete m_Geometry;
        m_Geometry = NULL;
        
//        delete m_ToonShader;
//        m_ToonShader = NULL;
//        
//        delete m_Shader;
//        m_Shader = NULL;
//        
//        delete m_RimLite;
//        m_RimLite = NULL;
//        
//        delete m_Phong;
//        m_Phong = NULL;
//        
//        delete m_PixelLighting;
//        m_PixelLighting = NULL;
//        
//        delete m_Gourand;
//        m_Gourand = NULL;
    }
    
    void Cubenado::create(int x, int y, int width, int height)
    {
//        glClearColor(0.52, 0.86, 0.99, 1.0f);
//        glClearColor(0.7,0.7,0.7,1);
        glClearColor(0.7,0.7,0.7,1);
        
        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
//        glEnable(GL_CULL_FACE);
        
        srand((unsigned int)time(0));
        
        m_CameraNode->addCamera(m_Camera);
        m_CameraNode->setOrigin(glm::vec3(0.0f, 0.0f, 0.0f));
        
        m_Scene->addActiveNode(m_CameraNode);
        m_Scene->addActiveCamera(m_Camera);
//        m_Scene->getRootNode()->setOrigin(glm::vec3(-10.0f, -300.0f, 1800.0f));
        m_Scene->getRootNode()->setOrigin(glm::vec3(0.0f, 0.0f, 10.0f));
#ifdef EDIT_SHADER
        m_Scene->getRootNode()->setOrigin(glm::vec3(0.0f, 0.0f, 10.0f));
#endif
        
        assert(m_Shader->load(loadFile("shaders/StandardShader2.vert"), loadFile("shaders/StandardShader2.frag")));
//        assert(m_ToonShader->load(loadFile("shaders/Toon.vert"), loadFile("shaders/Toon.frag")));
//        assert(m_RimLite->load(loadFile("shaders/RimLite.vert"), loadFile("shaders/RimLite.frag")));
//        assert(m_Phong->load(loadFile("shaders/Phong.vert"), loadFile("shaders/Phong.frag")));
//        assert(m_PixelLighting->load(loadFile("shaders/PixelLighting.vert"), loadFile("shaders/PixelLighting.frag")));
//        assert(m_Gourand->load(loadFile("shaders/Gourand.vert"), loadFile("shaders/Gourand.frag")));
        
        
        m_ShaderMap.insert(ShaderPair("Default", m_Shader));
//        m_ShaderMap.insert(ShaderPair("Toon", m_ToonShader));
//        m_ShaderMap.insert(ShaderPair("Rim lite", m_RimLite));
//        m_ShaderMap.insert(ShaderPair("Phong", m_Phong));
//        m_ShaderMap.insert(ShaderPair("Pixel Lighting", m_PixelLighting));
//        m_ShaderMap.insert(ShaderPair("Gourand", m_Gourand));
        
#ifdef EDIT_SHADER
//        m_Geometry->load(m_Shader, loadFile("models/utah-teapot-EXPORT.obj"));
        m_Geometry->load(m_Shader, loadFile("models/sprite.obj"));
#else
        m_Geometry->load(m_Shader, loadFile("models/cube.obj"), m_NumberOfCubes);
#endif
        
//
        
        
        setStartPositions();
        
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            m_Scene->addActiveNode(node);
            m_Scene->getRootNode()->addChildNode(node);
            
            node->addGeometry(m_Geometry);
            
//            node->setColorBase(glm::vec4(randomFloat(0.0f, 1.0f),
//                                         randomFloat(0.0f, 1.0f),
//                                         randomFloat(0.0f, 1.0f), 1.0f));
            
//            node->setColorBase(glm::vec4(1.0f,
//                                         1.0f,
//                                         1.0f, 1.0f));
        }
        
    }
    
    void Cubenado::destroy()
    {
        m_Geometry->unLoad();
        m_Shader->unLoad();
    }
    
    void Cubenado::resize(int x, int y, int width, int height)
    {
//        glViewport(x, y, width, height);
    }
                
    void Cubenado::update(float step)
    {   
#ifdef EDIT_SHADER
        Node *node = m_CubeNodes.at(0);

        glm::mat3x3 normalMatrix = node->getTransform();
        glm::mat3x3 _inversed = glm::inverse(normalMatrix);
        
        glm::mat3x3 _transposed = glm::transpose(_inversed);
        
        
//        node->setNormalMatrix(node->getWorldTransform().getBasis().inverse().transpose());
        node->setNormalMatrix(_transposed);
        
//        node->setColorBase(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        
        
//        btQuaternion rot1(glm::vec3(1.0, 0.0, 0.0), m_Rotation);
//        btQuaternion rot2(glm::vec3(0.0, 1.0, 0.0), m_Rotation);
        glm::quat rot2;
        rot2 = glm::rotate(rot2, m_Rotation, glm::vec3(0.0, 1.0, 0.0));
//        btQuaternion rot3(glm::vec3(0.0, 0.0, 1.0), m_Rotation);
//        node->setRotation(rot1 * rot2 * rot3);
        node->setRotation(rot2);
        m_Rotation += step;
        
        node->setTransform(node->getTransform());
//
        node->setRotation(node->getRotation());
        
        node->setOrigin(node->getOrigin());
        
        return;
#endif
        long cubesToDraw = m_NumberOfCubes;
        
        //https://prezi.com/yxshhdjmxr9q/the-physics-of-tornadoes/
        for (std::vector<Node*>::reverse_iterator i = m_CubeNodes.rbegin();
             i != m_CubeNodes.rend();
             i++)
        {
            Node *node = *i;
            
            float rad(0.5);
            
            //            25800.13889
            float vertical_mat_from = 4000;
            float vertical_mat_to = (vertical_mat_from + (1000 * m_Randomness));
            
            float vertical_mag(randomFloat(vertical_mat_from, vertical_mat_to));
            
            float horizontal_mag_from = 2000;//25800.13889;
            float horizontal_mat_to = horizontal_mag_from + (500 * m_Randomness);
            
            float horizontal_mag(randomFloat(horizontal_mag_from, horizontal_mat_to));
            
//            glm::vec3 origin(node->getTransform().getOrigin());
//            glm::vec3 forward(origin.normalized());
//            forward.setY(0.0f);
            
//            glm::vec3 impulse(0,0,0);
//
//            if(origin.length2() > (rad * rad))
//            {
//                impulse += glm::vec3(-forward);
//            }
//            else
//            {
//                impulse += glm::vec3(forward.rotate(glm::vec3(0,1,0), btRadians(90 + 0)));
//
//            }
//            impulse.setY(0);
//            impulse *= horizontal_mag;
//
//            if(origin.y() <= 0)
//            {
//                impulse+=glm::vec3(0, vertical_mag, 0);
//            }
            
//            node->addImpulseForce(impulse);
            
//            node->setNormalMatrix(node->getTransform().getBasis().inverse().transpose());
//            btMatrix3x3 m(node->getTransform().getBasis().transpose().inverse());
            
//            node->setNormalMatrix(node->getTransform().getBasis().transpose().inverse());
            
            
//            glm::quat_cast(node->getTransform());
            
//            node->setNormalMatrix(glm::transpose(glm::inverse((glm::mat3_cast(glm::quat_cast(node->getTransform()))))));
            
            glm::mat3x3 normalMatrix = node->getTransform();
            glm::mat3x3 _transposed = glm::transpose(normalMatrix);
            glm::mat3x3 _inversed = glm::inverse(_transposed);

            node->setNormalMatrix(_inversed);
            
            node->enableHideGeometry(false);
            if(cubesToDraw < 0)
                node->enableHideGeometry();
            cubesToDraw--;
            
        }
        m_Scene->update(step);
    }
    
    void Cubenado::render()
    {
//        m_FrameBuffer[0]->load                 (width, height, true);
//        m_FrameBuffer[0]->render();
        
//        m_FrameBuffer[1]->load                 (width, height, false);
//        m_FrameBuffer[1]->render();
        
//        m_FrameBuffer[2]->loadTexture2D        (NULL, NULL, width, height, true, false);
//        m_FrameBuffer[2]->render();
        
//        m_FrameBuffer[3]->loadTexture2D        (NULL, NULL, width, height, true, true);
//        m_FrameBuffer[3]->render();
        
//        m_FrameBuffer[4]->loadTextureCube      (NULL, NULL, width, height, true, false);
//        m_FrameBuffer[4]->render();
        
//        m_FrameBuffer[5]->loadTextureCube      (NULL, NULL, width, height, true, true);
//        m_FrameBuffer[5]->render();
        
//        m_FrameBuffer[6]->loadColorTexture2D   (NULL, width, height, true);
//        m_FrameBuffer[6]->render();
        
//        m_FrameBuffer[7]->loadDepthTexture2D   (NULL, width, height, true, false);
//        m_FrameBuffer[7]->render();
        
//        m_FrameBuffer[8]->loadDepthTexture2D   (NULL, width, height, true, true);
//        m_FrameBuffer[8]->render();
        
//        m_FrameBuffer[9]->loadColorTextureCube (NULL, width, height, true);
//        m_FrameBuffer[9]->render();
        
//        m_FrameBuffer[10]->loadDepthTextureCube (NULL, width, height, true, false);
//        m_FrameBuffer[10]->render();
        
//        m_FrameBuffer[11]->loadDepthTextureCube(NULL, width, height, true, true);
//        m_FrameBuffer[11]->render();
        
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m_Scene->render();
    }
    
    Camera *const Cubenado::getCamera()const
    {
        return m_Camera;
    }
    
    void Cubenado::setNumberOfCubes(const unsigned int amount)
    {
        if (amount > Cubenado::MAX_CUBES)
        {
            m_NumberOfCubes = Cubenado::MAX_CUBES;
        }
        m_NumberOfCubes = amount;
    }
    
    unsigned long Cubenado::numberOfCubes()const
    {
        return m_NumberOfCubes;
    }
    
    void Cubenado::setRandomness(const float percent)
    {
        assert(percent >= 0.0f && percent <= 1.0f);
        m_Randomness = percent;
    }
    
    float Cubenado::getRandomness()const
    {
        return m_Randomness;
    }
    
    std::vector<std::string> Cubenado::getShaderNames()const
    {
        std::vector<std::string> shaderNames;
        
        for (ShaderMap::const_iterator i = m_ShaderMap.begin(); i != m_ShaderMap.end(); i++)
        {
            shaderNames.push_back((*i).first);
        }
        return shaderNames;
    }
    
    void Cubenado::setShader(const std::string &shader)
    {
        ShaderMap::iterator i = m_ShaderMap.find(shader);
        if(i != m_ShaderMap.end())
        {
            Shader *shader = (*i).second;
            
            m_Geometry->setShader(shader);
        }
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
#ifdef EDIT_SHADER
        Node *node = m_CubeNodes.at(0);
        node->setOrigin(glm::vec3(0,0,0));
        return;
#endif
        float startX = 0.0f;
        float startY = 10.0f;
        float startZ = 0.0f;
        
        float xinc = 2.0f;
        float xmargin = 1.0;
        
        float yinc = 2.0f;
        float ymargin = 30.0f;
        
        float zinc = 2.0f;
        float zmargin = 1.0;
        
        float currentX = startX;
        float currentY = startY;
        float currentZ = startZ;
        
        float dim = m_CubeNodes.size();
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
                        {
                            node->setOrigin(glm::vec3(currentX, currentY, currentZ));
//                            node->setGravity(glm::vec3(0,-9.81,0));
                        }
                    }
                    currentZ += (zinc + zmargin);
                }
                currentZ = startZ;
                currentY += (yinc + ymargin);
            }
            currentY = startY;
            currentX += (xinc + xmargin);
        }
    }
    
    glm::vec3 Cubenado::randomPosition(const glm::vec3 &min, const glm::vec3 &max)const
    {
        assert(min.x <= max.x);
        assert(min.y <= max.y);
        assert(min.z <= max.z);
        
        return glm::vec3(randomFloat(min.x, max.x),
                      randomFloat(min.y, max.y),
                      randomFloat(min.z, max.z));
    }
    
}
