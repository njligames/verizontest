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
#include "PhysicsBodyRigid.hpp"
#include "PhysicsShapeCube.hpp"
#include "PhysicsWorld.hpp"
#include "AbstractFrameBuffer.hpp"
#include "SceneFrameBuffer.hpp"

#include "btQuaternion.h"

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
    m_Rotation(0.0f),
    m_PhysicsShapeCube(new PhysicsShapeCube())
    {
        for (unsigned long i = 0; i < Cubenado::MAX_CUBES; i++)
            m_PhysicsBodies.push_back(new PhysicsBodyRigid());
        
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
        delete m_PhysicsShapeCube;
        m_PhysicsShapeCube = NULL;
        
        while (!m_CubeNodes.empty())
        {
            Node *node = m_CubeNodes.back();
            m_CubeNodes.pop_back();
            delete node;
        }
        
        while (!m_PhysicsBodies.empty())
        {
            PhysicsBodyRigid *rigidBody = m_PhysicsBodies.back();
            m_PhysicsBodies.pop_back();
            delete rigidBody;
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
        glEnable(GL_CULL_FACE);
        
        srand((unsigned int)time(0));
        
        m_CameraNode->addCamera(m_Camera);
        m_CameraNode->setOrigin(btVector3(0.0f, 0.0f, 0.0f));
        
        m_Scene->addActiveNode(m_CameraNode);
        m_Scene->addActiveCamera(m_Camera);
        m_Scene->getRootNode()->setOrigin(btVector3(-10.0f, -300.0f, 1800.0f));
#ifdef EDIT_SHADER
        m_Scene->getRootNode()->setOrigin(btVector3(0.0f, 0.0f, 10.0f));
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
        m_Geometry->load(m_Shader, loadFile("models/suzanne.obj"));
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
            
//            node->setColorBase(btVector4(randomFloat(0.0f, 1.0f),
//                                         randomFloat(0.0f, 1.0f),
//                                         randomFloat(0.0f, 1.0f), 1.0f));
            
//            node->setColorBase(btVector4(1.0f,
//                                         1.0f,
//                                         1.0f, 1.0f));
        }
        
//        setupPhysics();
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
        
        node->setNormalMatrix(node->getWorldTransform().getBasis().inverse().transpose());
//        node->setColorBase(btVector4(1.0f, 0.0f, 0.0f, 1.0f));
        
        
//        btQuaternion rot1(btVector3(1.0, 0.0, 0.0), m_Rotation);
        btQuaternion rot2(btVector3(0.0, 1.0, 0.0), m_Rotation);
//        btQuaternion rot3(btVector3(0.0, 0.0, 1.0), m_Rotation);
//        node->setRotation(rot1 * rot2 * rot3);
        node->setRotation(rot2);
        m_Rotation += step;
        return;
#endif
        long cubesToDraw = m_NumberOfCubes;
        
        //https://prezi.com/yxshhdjmxr9q/the-physics-of-tornadoes/
        for (std::vector<Node*>::reverse_iterator i = m_CubeNodes.rbegin();
             i != m_CubeNodes.rend();
             i++)
        {
            Node *node = *i;
            
            btScalar rad(0.5);
            
            //            25800.13889
            btScalar vertical_mat_from = 4000;
            btScalar vertical_mat_to = (vertical_mat_from + (1000 * m_Randomness));
            
            btScalar vertical_mag(randomFloat(vertical_mat_from, vertical_mat_to));
            
            btScalar horizontal_mag_from = 2000;//25800.13889;
            btScalar horizontal_mat_to = horizontal_mag_from + (500 * m_Randomness);
            
            btScalar horizontal_mag(randomFloat(horizontal_mag_from, horizontal_mat_to));
            
            btVector3 origin(node->getTransform().getOrigin());
            btVector3 forward(origin.normalized());
            forward.setY(0.0f);
            
            btVector3 impulse(0,0,0);
            
            if(origin.length2() > (rad * rad))
            {
                impulse += btVector3(-forward);
            }
            else
            {
                impulse += btVector3(forward.rotate(btVector3(0,1,0), btRadians(90 + 0)));
                
            }
            impulse.setY(0);
            impulse *= horizontal_mag;
            
            if(origin.y() <= 0)
            {
                impulse+=btVector3(0, vertical_mag, 0);
            }
            
            node->addImpulseForce(impulse);
            
//            node->setNormalMatrix(node->getTransform().getBasis().inverse().transpose());
            btMatrix3x3 m(node->getTransform().getBasis().transpose().inverse());
            
            node->setNormalMatrix(node->getTransform().getBasis().transpose().inverse());
            
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
    
    void Cubenado::setupPhysics()
    {
        m_Scene->getPhysicsWorld()->setGravity(btVector3(0,-9.81,0));
        m_PhysicsShapeCube->setHalfExtends(btVector3(1.0f, 1.0f, 1.0f));
        
        unsigned long idx = 0;
        for (std::vector<Node*>::iterator i = m_CubeNodes.begin();
             i != m_CubeNodes.end();
             i++)
        {
            Node *node = *i;
            
            assert(idx < m_PhysicsBodies.size());
            
            PhysicsBodyRigid *rigidBody = m_PhysicsBodies.at(idx++);
            
            rigidBody->addPhysicsShape(m_PhysicsShapeCube);
            rigidBody->setKinematicPhysics();
            rigidBody->setMass(1.0f);
            rigidBody->setScene(m_Scene);
            
            assert(node->addPhysicsBody(rigidBody));
            
            assert(m_Scene);
            assert(m_Scene->getPhysicsWorld());
            assert(m_Scene->getPhysicsWorld()->addRigidBody(rigidBody));
        }
    }
    
    void Cubenado::setStartPositions()
    {
#ifdef EDIT_SHADER
        Node *node = m_CubeNodes.at(0);
        node->setOrigin(btVector3(0,0,0));
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
                            node->setOrigin(btVector3(currentX, currentY, currentZ));
                            node->setGravity(btVector3(0,-9.81,0));
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
