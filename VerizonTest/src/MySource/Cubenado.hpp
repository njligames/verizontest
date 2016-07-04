#ifndef Cubenado_hpp
#define Cubenado_hpp

#include <string>
#include <vector>
#include <map>

#include "Camera.hpp"

namespace njli
{
    class Shader;
    class Geometry;
    class RectangleGeometry;
    class Node;
    class Scene;
    class PhysicsBodyRigid;
    class PhysicsShapeCube;
    
    class Cubenado
    {
    public:
        /* static */
        static Cubenado *const getInstance();
        static void createInstance();
        static void destroyInstance();
        static void setBundlePath(const std::string &bundlePath);
        
        /* members */
        Cubenado();
        Cubenado(const Cubenado &rhs);
        const Cubenado &operator=(const Cubenado &rhs);
        ~Cubenado();
        
        void create(int x, int y, int width, int height);
        void destroy();
        
        void resize(int x, int y, int width, int height);
        void update(float step);
        void render();
        
        Camera *const getCamera()const;
        
        void setNumberOfCubes(const unsigned int amount);
        void setRandomness(const float percent);
        float getRandomness()const;
        std::vector<std::string> getShaderNames()const;
        void setShader(const std::string &shader);
        
        std::string loadFile(const std::string filepath);
    protected:
        void setupPhysics();
        void setStartPositions();
        btVector3 randomPosition(const btVector3 &min, const btVector3 &max)const;
        btQuaternion randomRotation(const btVector3 &axis, const float degreesMin, const float degreesMax)const;
        btQuaternion randomRotation(const float degreesMin, const float degreesMax)const;
        btQuaternion randomRotation()const;
    private:
        /* static */
        static Cubenado *s_Instance;
        static std::string *s_BundlePath;
        
        /* members */
        
        Shader *m_Shader;
        Shader *m_ToonShader;
        Shader *m_RimLite;
        Geometry *m_Geometry;
        Camera *m_Camera;
        Node *m_CameraNode;
        Scene *m_Scene;
        std::vector<Node*> m_CubeNodes;
        std::vector<PhysicsBodyRigid*> m_PhysicsBodies;
        PhysicsShapeCube *m_PhysicsShapeCube;
        float m_Randomness;
        unsigned long m_NumberOfCubes;
        float m_Rotation;
        
        typedef std::map<std::string, Shader*> ShaderMap;
        typedef std::pair<std::string, Shader*> ShaderPair;
        
        ShaderMap m_ShaderMap;
    };
}

#endif
