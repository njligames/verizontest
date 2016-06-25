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

#include "btTransform.h"

namespace njli
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
                  const std::string &fragmentSource);
        
        void unLoad();
        bool isLoaded()const;
        
        bool use()const;
        
        int getAttributeLocation(const std::string &attributeName)const;
        
        int getUniformLocation(const std::string &uniformName)const;
        
        bool setUniformValue(const std::string &uniformName, const btTransform &value, bool transpose = false);
        bool setUniformValue(const std::string &uniformName, GLfloat *matrix4x4, bool transpose = false);
        bool getUniformValue(const std::string &uniformName, btTransform &value)const;
        
        bool setUniformValue(const char *uniformName, int value);
        bool getUniformValue(const char *uniformName, int &value);
    protected:
        GLuint compileShader(const std::string &source, GLenum type);
        bool compileStatus(GLuint shader);
        
        bool linkProgram(GLuint program);
        
    private:
        GLuint m_Program;
        
        GLfloat *m_mat4Buffer;
        
    };
}

#endif /* Shader_hpp */
