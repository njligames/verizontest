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

namespace NJLI
{
    Shader::Shader():
    m_Program(0)
    {
        
    }
    
    Shader::~Shader()
    {
        unLoad();
    }
    
    
    bool Shader::load(const std::string &vertexSource,
              const std::string &fragmentSource,
              const std::vector<std::string> &attributes)
    {
        GLuint vertShader, fragShader;
        
        // Create shader program.
        m_Program = glCreateProgram();
        
        if(!(vertShader = compileShader(vertexSource, GL_VERTEX_SHADER)))
            return false;
        
        if(!(fragShader = compileShader(fragmentSource, GL_FRAGMENT_SHADER)))
            return false;
        
        // Attach vertex shader to program.
        glAttachShader(m_Program, vertShader);
        
        // Attach fragment shader to program.
        glAttachShader(m_Program, fragShader);
        
        
        for (std::vector<std::string>::const_iterator i = attributes.begin(); i != attributes.end(); i++)
        {
            std::string attributeName = *i;
            int location = glGetAttribLocation(m_Program, attributeName.c_str());
            glBindAttribLocation(m_Program, location, attributeName.c_str());
        }
        
        // Bind attribute locations.
        // This needs to be done prior to linking.
//        glBindAttribLocation(_program, GLKVertexAttribPosition, "position");
//        glBindAttribLocation(_program, GLKVertexAttribNormal, "normal");
        
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
        
        
        
        // Get uniform locations.
//        uniforms[UNIFORM_MODELVIEWPROJECTION_MATRIX] = glGetUniformLocation(_program, "modelViewProjectionMatrix");
//        uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(_program, "normalMatrix");
        
        // Release vertex and fragment shaders.
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
    
    void Shader::use()
    {
        if(m_Program)
            glUseProgram(m_Program);
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