//
//  Shader.cpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "Shader.hpp"
#include <iostream>
#include <assert.h>

namespace njli
{   
    Shader::Shader():
    m_Program(0),
    m_mat4Buffer(new GLfloat[16])
    {
        
    }
    
    Shader::~Shader()
    {
        delete [] m_mat4Buffer;
        m_mat4Buffer = NULL;
        
        unLoad();
    }
    
    
    bool Shader::load(const std::string &vertexSource,
              const std::string &fragmentSource)//,
//              const std::vector<std::string> &attributes)
    {
        GLuint vertShader, fragShader;
        
        m_Program = glCreateProgram();
        
        if(!(vertShader = compileShader(vertexSource, GL_VERTEX_SHADER)))
            return false;
        
        if(!(fragShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER)))
            return false;
        
        glAttachShader(m_Program, vertShader);
        glAttachShader(m_Program, fragShader);
        
        if(!linkProgram(m_Program))
        {
            if (vertShader)
            {
                glDeleteShader(vertShader);
                vertShader = 0;
            }
            if (fragShader)
            {
                glDeleteShader(fragShader);
                fragShader = 0;
            }
            if (m_Program)
            {
                glDeleteProgram(m_Program);
                m_Program = 0;
            }
            return false;
        }
        
        if (vertShader)
        {
            glDetachShader(m_Program, vertShader);
            glDeleteShader(vertShader);
        }
        
        if (fragShader)
        {
            glDetachShader(m_Program, fragShader);
            glDeleteShader(fragShader);
        }
        
        return true;
    }
    
    void Shader::unLoad()
    {
        if(m_Program)
            glDeleteProgram(m_Program);
        m_Program = 0;
    }
    
    bool Shader::isLoaded()const
    {
        return (m_Program != 0);
    }
    
    bool Shader::use()const
    {
        if(m_Program)
        {
            GLint id;
            glGetIntegerv(GL_CURRENT_PROGRAM,&id);
            if(id != m_Program)
                glUseProgram(m_Program);
            return true;
        }
        return false;
    }
    
    int Shader::getAttributeLocation(const std::string &attributeName)const
    {
        int location = glGetAttribLocation(m_Program, attributeName.c_str());
        
#if defined(DEBUG)
        if(location == -1)
        {
            std::cout << "The named attribute variable " << attributeName << " is not an active attribute in the specified program object or if name starts with the reserved prefix \"gl_\"" << std::endl;
        }
#endif
        
        return location;
    }
    
    int Shader::getUniformLocation(const std::string &uniformName)
    {
        int location = -1;
        
        UniformMap::iterator iter = m_UniformMap.find(uniformName);
        if(iter != m_UniformMap.end())
        {
            location = iter->second;
        }
        else
        {
            location = glGetUniformLocation(m_Program, uniformName.c_str());
            m_UniformMap.insert(UniformPair(uniformName, location));
        }
        
#if defined(DEBUG)
        if(location == -1)
        {
            std::cout << "The named attribute variable " << uniformName << " is not an active attribute in the specified program object or if name starts with the reserved prefix \"gl_\"" << std::endl;
        }
#endif
        
        return location;
    }
    
    bool Shader::setUniformValue(const std::string &uniformName, const btTransform &value, bool transpose)
    {
        value.getOpenGLMatrix(m_mat4Buffer);
        return setUniformValue(uniformName, m_mat4Buffer, transpose);
    }
    
    bool Shader::setUniformValue(const std::string &uniformName, GLfloat *matrix4x4, bool transpose)
    {
        int location = getUniformLocation(uniformName);
        if(location != -1)
        {
            glUniformMatrix4fv(location,
                               1,
                               (transpose)?GL_TRUE:GL_FALSE,
                               matrix4x4);
            return true;
        }
        return false;
    }
    
    bool Shader::getUniformValue(const std::string &uniformName, btTransform &value)
    {
        int location = getUniformLocation(uniformName);
        if(location != -1)
        {
            glGetUniformfv(m_Program, location, m_mat4Buffer);
            value.setFromOpenGLMatrix(m_mat4Buffer);
            return true;
        }
        return false;
    }
    
    bool Shader::setUniformValue(const char *uniformName, int value)
    {
        int location = getUniformLocation(uniformName);
        if(location != -1)
        {
            glUniform1i(location, value);
            return true;
        }
        return false;
    }
    
    bool Shader::getUniformValue(const char *uniformName, int &value)
    {
        int location = getUniformLocation(uniformName);
        if(location != -1)
        {
            glGetUniformiv(m_Program, location, &value);
            return true;
        }
        return false;
    }
    
    GLuint Shader::compileShader(const std::string &source, GLenum type)
    {
        GLuint shader;
        
        GLint status;
        shader = glCreateShader(type);
        
        GLchar**str = new GLchar*[1];
        str[0] = new GLchar[source.length()];
        strcpy(str[0], source.c_str());
        
        glShaderSource(shader, 1, (const GLchar**)&(str[0]), NULL);
        glCompileShader(shader);
        
#if defined(DEBUG)
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            std::cout << "Shader compile log:" << std::endl << log;
            free(log);
        }
#endif
        
        delete [] str[0];str[0]=NULL;
        delete [] str;str=NULL;
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == 0) {
            glDeleteShader(shader);
            shader = 0;
        }
        
        return shader;
    }
    
    bool Shader::compileStatus(GLuint shader)
    {
        GLint compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        if(log_length > 0)
        {
            GLchar log_buffer[log_length];
            glGetShaderInfoLog(shader, log_length, NULL, log_buffer);
            std::cout << log_buffer << std::endl;
        }
        
        return (compileStatus == GL_TRUE);
    }
    
    bool Shader::linkProgram(GLuint program)
    {
        GLint status;
        glLinkProgram(program);
        
#if defined(DEBUG)
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = (GLchar *)malloc(logLength);
            glGetProgramInfoLog(program, logLength, &logLength, log);
            std::cout << "Program link log:\n" << log;
            free(log);
        }
#endif
        
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        return (status == GL_TRUE);
    }
}