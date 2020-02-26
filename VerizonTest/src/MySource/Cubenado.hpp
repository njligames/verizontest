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
    class SceneFrameBuffer;
    
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
        unsigned long numberOfCubes()const;
        void setRandomness(const float percent);
        float getRandomness()const;
        std::vector<std::string> getShaderNames()const;
        void setShader(const std::string &shader);
        
        std::string loadFile(const std::string filepath);
    protected:
        void setStartPositions();
        glm::vec3 randomPosition(const glm::vec3 &min, const glm::vec3 &max)const;
//        btQuaternion randomRotation(const glm::vec3 &axis, const float degreesMin, const float degreesMax)const;
//        btQuaternion randomRotation(const float degreesMin, const float degreesMax)const;
//        btQuaternion randomRotation()const;
    private:
        /* static */
        static Cubenado *s_Instance;
        static std::string *s_BundlePath;
        static const GLsizei MAX_CUBES = 10000;
        
        /* members */
        
        Shader *m_Shader;
//        Shader *m_ToonShader;
//        Shader *m_RimLite;
//        Shader *m_Phong;
//        Shader * m_PixelLighting;
//        Shader *m_Gourand;
        
        Geometry *m_Geometry;
        Camera *m_Camera;
        Node *m_CameraNode;
        Scene *m_Scene;
        std::vector<Node*> m_CubeNodes;
        float m_Randomness;
        unsigned long m_NumberOfCubes;
        float m_Rotation;
        
        typedef std::map<std::string, Shader*> ShaderMap;
        typedef std::pair<std::string, Shader*> ShaderPair;
        
        ShaderMap m_ShaderMap;
        
        SceneFrameBuffer *m_FrameBuffer[12];
    };
}

#endif
