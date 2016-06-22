//
//  Shader.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <vector>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

namespace NJLI
{
    class Shader
    {
    public:
        /* members */
        Shader();
        Shader(const Shader &rhs);
        const Shader &operator=(const Shader &rhs);
        ~Shader();

        bool load(const std::string &vertexSource,
                  const std::string &fragmentSource,
                  const std::vector<std::string> &attributes);
        
        void unLoad();
        bool isLoaded()const;
        
        void use();
    protected:
        GLuint compileShader(const std::string &source, GLenum type);
        bool compileStatus(GLuint shader);
        
        bool linkProgram(GLuint program);
        
    private:
        GLuint m_Program;
    };
}

#endif /* Shader_hpp */
