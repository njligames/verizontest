#include <string>

namespace NJLI
{
    class Shader;
    
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
        
        void setNumberOfCubes(const unsigned int amount);
        void setRandomness(const float percent);
        
        std::string loadFile(const std::string filepath);
    protected:
    private:
        /* static */
        static Cubenado *s_Instance;
        static std::string *s_BundlePath;
        
        /* members */
        
        Shader *m_Shader;
    };
}
