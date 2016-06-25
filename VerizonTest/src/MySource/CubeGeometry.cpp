//
//  CubeGeometry.cpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "CubeGeometry.hpp"

#include <assert.h>
#include <iostream>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Node.hpp"

#include "btVector3.h"
#include "btVector2.h"

namespace njli
{
    static const btVector4 WHITE_COLOR = {1.0f, 1.0f, 1.0f, 1.0f};
    static const btVector4 RED_COLOR   = {1.0f, 0.0f, 0.0f, 1.0f};
    
    static const CubeGeometry::VertexData CUBE_VERTEX_DATA[] =
    {
        // Front
        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f},
        // Back
        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f},
        // Left
        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f},
        // Right
        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f},
        // Top
        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f},
        // Bottom
        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, /*{1, 0},*/ 1.0f, 0.0f},
        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, /*{1, 1},*/ 1.0f, 0.0f},
        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, /*{0, 1},*/ 1.0f, 0.0f},
        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, /*{0, 0},*/ 1.0f, 0.0f}
    };
    
    const GLushort CUBE_INDICE_DATA[] =
    {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        6, 7, 4,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    };
    
//    static const VertexData CUBE_VERTEX_DATA[] =
//    {
//        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, {1.0f, 0.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        
//        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, {0.0f, 1.0f, 0.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        
//        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, {-1.0f, 0.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        
//        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, {0.0f, -1.0f, 0.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        
//        {{0.5f, 0.5f, 0.5f},    WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, -0.5f, 0.5f},   WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, 0.5f},   WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, 0.5f},  WHITE_COLOR, {0.0f, 0.0f, 1.0f},  /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        
//        {{0.5f, -0.5f, -0.5f},  WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{0.5f, 0.5f, -0.5f},   WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, -0.5f, -0.5f}, WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f},
//        {{-0.5f, 0.5f, -0.5f},  WHITE_COLOR, {0.0f, 0.0f, -1.0f}, /*{0.0f, 0.0f},*/ 1.0f, 0.0f}
//    };
    
    CubeGeometry::CubeGeometry():
    m_ModelViewTransformData(new GLfloat[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16]),
    m_ColorTransformData(new GLfloat[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16]),
    m_CubeVertexData(new CubeVerts[CubeGeometry::MAX_CUBES]),
    m_CubeIndiceData(new GLushort[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_INDICES]),
    m_VertexArray(0),
    m_ModelViewBuffer(0),
    m_ColorTransformBuffer(0),
    m_VerticesBuffer(0),
    m_IndexBuffer(0),
    m_Shader(NULL),
    m_OpacityModifyRGB(false),
    m_MatrixBuffer(new GLfloat[16])
    {
        assert(m_ModelViewTransformData);
        assert(m_ColorTransformData);
        assert(m_CubeVertexData);
        assert(m_CubeIndiceData);
        
//        const GLushort CUBE_INDICE_DATA[] =
//        {
//            0,  1,  2,  3,  4,  5,
//            6,  7,  8,  9, 10, 11,
//            12, 13, 14, 15, 16, 17,
//            18, 19, 20, 21, 22, 23,
//            24, 25, 26, 27, 28, 29,
//            30, 31, 32, 33, 34, 35
//        };
        
        static const GLfloat TRANSFORM_IDENTITY_MATRIX[] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
        
        static const GLfloat COLOR_IDENTITY_MATRIX[] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 0,
        };
        
        for (int i = 0;
             i < (CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16);
             i += 16)
            memcpy(m_ModelViewTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
        for (int i = 0;
             i < (CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16);
             i += 16)
            memcpy(m_ColorTransformData + i, COLOR_IDENTITY_MATRIX, sizeof(COLOR_IDENTITY_MATRIX));

        for (int i = 0;
             i < (CubeGeometry::MAX_CUBES * (sizeof(CUBE_VERTEX_DATA) / sizeof(GLfloat)));
             i += (sizeof(CUBE_VERTEX_DATA)))
            memcpy(m_CubeVertexData + i, CUBE_VERTEX_DATA, sizeof(CUBE_VERTEX_DATA));
        
        for (int i = 0;
             i < (CubeGeometry::MAX_CUBES * (sizeof(CUBE_INDICE_DATA) / sizeof(GLushort)));
             i += (sizeof(CUBE_INDICE_DATA) / sizeof(GLushort)))
            memcpy(m_CubeIndiceData + i, CUBE_INDICE_DATA, sizeof(CUBE_INDICE_DATA));
    }
    
    
    CubeGeometry::~CubeGeometry()
    {
                
        delete [] m_MatrixBuffer;
        m_MatrixBuffer = NULL;
        
        delete [] m_CubeIndiceData;
        m_CubeIndiceData = NULL;
        
        delete [] m_CubeVertexData;
        m_CubeVertexData = NULL;
        
        delete [] m_ColorTransformData;
        m_ColorTransformData = NULL;
        
        delete [] m_ModelViewTransformData;
        m_ModelViewTransformData = NULL;
    }
    
    void CubeGeometry::load(Shader *shader)
    {
        assert(shader);
        
        m_Shader = shader;
        
        int inTransformAttrib = getShader()->getAttributeLocation("inTransform");
        int inColorTransform = getShader()->getAttributeLocation("inColorTransform");
        
        int inPositionAttrib = getShader()->getAttributeLocation("inPosition");
        int inColorAttrib = getShader()->getAttributeLocation("inColor");
//        int inNormalAttrib = getShader()->getAttributeLocation("inNormal");
//        int inTexCoordAttrib = getShader()->getAttributeLocation("inTexCoord");
        int inOpacityAttrib = getShader()->getAttributeLocation("inOpacity");
        int inHiddenAttrib = getShader()->getAttributeLocation("inHidden");
        
        
        glGenVertexArraysOES(1, &m_VertexArray);
        glBindVertexArrayOES(m_VertexArray);
        {
            /*load the model view transforms for instancing...*/
            {
                glGenBuffers(1, &m_ModelViewBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferData(GL_ARRAY_BUFFER,
                             getModelViewTransformArrayBufferSize(),
                             getModelViewTransformArrayBufferPtr(),
                             GL_DYNAMIC_DRAW);
                
                assert(inTransformAttrib != -1);
                glEnableVertexAttribArray(inTransformAttrib + 0);
                glEnableVertexAttribArray(inTransformAttrib + 1);
                glEnableVertexAttribArray(inTransformAttrib + 2);
                glEnableVertexAttribArray(inTransformAttrib + 3);
                glVertexAttribPointer(inTransformAttrib + 0, 4, GL_FLOAT, 0, 64, (GLvoid*)0);
                glVertexAttribPointer(inTransformAttrib + 1, 4, GL_FLOAT, 0, 64, (GLvoid*)16);
                glVertexAttribPointer(inTransformAttrib + 2, 4, GL_FLOAT, 0, 64, (GLvoid*)32);
                glVertexAttribPointer(inTransformAttrib + 3, 4, GL_FLOAT, 0, 64, (GLvoid*)48);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            
            /*Load the color transforms for instancing...*/
            {
                glGenBuffers(1, &m_ColorTransformBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
                glBufferData(GL_ARRAY_BUFFER,
                             getColorTransformArrayBufferSize(),
                             getColorTransformArrayBufferPtr(),
                             GL_DYNAMIC_DRAW);
                
                assert(inColorTransform != -1);
                glEnableVertexAttribArray(inColorTransform + 0);
                glEnableVertexAttribArray(inColorTransform + 1);
                glEnableVertexAttribArray(inColorTransform + 2);
                glEnableVertexAttribArray(inColorTransform + 3);
                glVertexAttribPointer(inColorTransform + 0, 4, GL_FLOAT, 0, 64, (GLvoid*)0);
                glVertexAttribPointer(inColorTransform + 1, 4, GL_FLOAT, 0, 64, (GLvoid*)16);
                glVertexAttribPointer(inColorTransform + 2, 4, GL_FLOAT, 0, 64, (GLvoid*)32);
                glVertexAttribPointer(inColorTransform + 3, 4, GL_FLOAT, 0, 64, (GLvoid*)48);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            
            {
                glGenBuffers(1, &m_VerticesBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferData(GL_ARRAY_BUFFER,
                             getVertexArrayBufferSize(),
                             getVertexArrayBufferPtr(),
                             GL_DYNAMIC_DRAW);
                
                assert(inPositionAttrib != -1);
                glEnableVertexAttribArray(inPositionAttrib);
                glVertexAttribPointer(inPositionAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(VertexData),
                                      (const GLvoid*) offsetof(VertexData, m_Vertex));
                
                assert(inColorAttrib != -1);
                glEnableVertexAttribArray(inColorAttrib);
                glVertexAttribPointer(inColorAttrib,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(VertexData),
                                      (const GLvoid*) offsetof(VertexData, m_Color));
                
//                assert(inNormalAttrib != -1);
//                glEnableVertexAttribArray(inNormalAttrib);
//                glVertexAttribPointer(inNormalAttrib,
//                                      3,
//                                      GL_FLOAT,
//                                      GL_FALSE,
//                                      sizeof(VertexData),
//                                      (const GLvoid*) offsetof(VertexData, m_Normal));
//                assert(inTexCoordAttrib != -1);
//                glEnableVertexAttribArray(inTexCoordAttrib);
//                glVertexAttribPointer(inTexCoordAttrib,
//                                      2,
//                                      GL_FLOAT,
//                                      GL_FALSE,
//                                      sizeof(VertexData),
//                                      (const GLvoid*) offsetof(VertexData, m_Texture));
                
                assert(inOpacityAttrib != -1);
                glEnableVertexAttribArray(inOpacityAttrib);
                glVertexAttribPointer(inOpacityAttrib,
                                      1,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(VertexData),
                                      (const GLvoid*)offsetof(VertexData, m_Opacity));
                
                assert(inHiddenAttrib != -1);
                glEnableVertexAttribArray(inHiddenAttrib);
                glVertexAttribPointer(inHiddenAttrib,
                                      1,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(VertexData),
                                      (const GLvoid*)offsetof(VertexData, m_Hidden));
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            {
                glGenBuffers(1, &m_IndexBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             getElementArrayBufferSize(),
                             getElementArrayBufferPtr(),
                             GL_STATIC_DRAW);
                
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }
        glBindVertexArrayOES(0);
    }
    
    void CubeGeometry::unLoad()
    {
        if (m_IndexBuffer)
            glDeleteBuffers(1, &m_IndexBuffer);
        m_IndexBuffer = 0;
        
        if (m_VerticesBuffer)
            glDeleteBuffers(1, &m_VerticesBuffer);
        m_VerticesBuffer = 0;
        
        if (m_ColorTransformBuffer)
            glDeleteBuffers(1, &m_ColorTransformBuffer);
        m_ColorTransformBuffer = 0;
        
        if (m_ModelViewBuffer)
            glDeleteBuffers(1, &m_ModelViewBuffer);
        m_ModelViewBuffer = 0;
    }
    
    bool CubeGeometry::isLoaded()const
    {
        return false;
    }
    
    Shader *const CubeGeometry::getShader()
    {
        return m_Shader;
    }
    
    void CubeGeometry::render(Camera *camera)
    {
        Shader *shader = getShader();
        if(shader && camera)
        {
            assert(shader->use());
            
            assert(shader->setUniformValue("modelView", camera->getModelView()));
            assert(shader->setUniformValue("projection", camera->getProjectionMatrixPtr()));
            assert(shader->setUniformValue("opacityModifyRGB", m_OpacityModifyRGB));
            
            glBindVertexArrayOES(m_VertexArray);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, getModelViewTransformArrayBufferSize(), getModelViewTransformArrayBufferPtr());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexArrayBufferSize(), getVertexArrayBufferPtr());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
            
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((CubeGeometry::MAX_CUBES) * CubeGeometry::NUMBER_OF_INDICES), GL_UNSIGNED_SHORT, (const GLvoid*)0);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArrayOES(0);
        }
    }
    
    void CubeGeometry::setOpacity(Node *node, GLfloat opacity)
    {
        
    }
    
    void CubeGeometry::setHidden(Node *node, bool hidden)
    {
        
    }
    
    bool CubeGeometry::isHidden(Node *node)const
    {
        return false;
    }
    
    const void *CubeGeometry::getModelViewTransformArrayBufferPtr()const
    {
        return m_ModelViewTransformData;
    }
    
    GLsizeiptr CubeGeometry::getModelViewTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16;
        return size;
    }
    
    const void *CubeGeometry::getColorTransformArrayBufferPtr()const
    {
        return m_ColorTransformData;
    }
    
    GLsizeiptr CubeGeometry::getColorTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16;
        return size;
    }
    
    const void *CubeGeometry::getVertexArrayBufferPtr()const
    {
        return (const void *)m_CubeVertexData;
    }
    
    GLsizeiptr CubeGeometry::getVertexArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(CubeVerts) * CubeGeometry::MAX_CUBES;
        return size;
    }
    
    const void *CubeGeometry::getElementArrayBufferPtr()const
    {
        return m_CubeIndiceData;
    }
    
    GLsizeiptr CubeGeometry::getElementArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLushort) * CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_INDICES;
        return size;
    }
    
    void CubeGeometry::addReference(Node *node)
    {
        for (unsigned long i = 0; i < m_References.size(); ++i)
        {
            if (!m_References[i])
            {
                m_References[i] = 1;
                node->setGeometryIndex(i);
                return;
            }
        }
    }
    
    void CubeGeometry::removeReference(Node *node)
    {
        unsigned long index = node->getGeometryIndex();
        
        if(index < m_References.size())
        {
            m_References[index] = 0;
            
            hideGeometry(node);
        }
    }
    
    void CubeGeometry::hideGeometry(Node *node)
    {
        unsigned long i = node->getGeometryIndex();
        
        //!!!:TODO
    }
    
    void CubeGeometry::setTransform(const unsigned long index, const btTransform &transform)
    {
        assert(index < CubeGeometry::MAX_CUBES);
        
        const GLuint STRIDE = 64;
        
        transform.getOpenGLMatrix(m_MatrixBuffer);
        
        for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
        {
            unsigned long p = ((index * STRIDE) + (16 * currentVertex));
            int cmp = memcmp(m_ModelViewTransformData + p,
                             m_MatrixBuffer,
                             sizeof(GLfloat) * 16);
            
            if(0 != cmp)
            {
                memcpy(m_ModelViewTransformData + p, m_MatrixBuffer, sizeof(GLfloat) * 16);
            }
        }
    }
    
    btTransform CubeGeometry::getTransform(const unsigned long index)
    {
        assert(index < CubeGeometry::MAX_CUBES);
        
        btTransform transform(btTransform::getIdentity());
        const GLuint STRIDE = 64;
        
        for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
        {
            unsigned long p = ((index * STRIDE) + (16 * currentVertex));
            memcpy(m_MatrixBuffer,
                   m_ModelViewTransformData + p,
                   sizeof(GLfloat) * 16);
        }
        
        transform.setFromOpenGLMatrix(m_MatrixBuffer);
        return transform;
    }
    
    void CubeGeometry::setColorTransform(const unsigned long index, const btTransform &transform)
    {
        assert(index < CubeGeometry::MAX_CUBES);
        
        const GLuint STRIDE = 64;
        
        transform.getOpenGLMatrix(m_MatrixBuffer);
        
        for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
        {
            unsigned long p = ((index * STRIDE) + (16 * currentVertex));
            
            int cmp = memcmp(m_ColorTransformData + p,
                             m_MatrixBuffer,
                             sizeof(GLfloat) * 16);
            
            if(0 != cmp)
            {
                memcpy(m_ColorTransformData + p,
                       m_MatrixBuffer,
                       sizeof(GLfloat) * 16);
            }
        }
    }
    
    btTransform CubeGeometry::getColorTransform(const unsigned long index)
    {
        assert(index < CubeGeometry::MAX_CUBES);
        
        btTransform transform(btTransform::getIdentity());
        const GLuint STRIDE = 64;
        
        for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
        {
            unsigned long p = ((index * STRIDE) + (16 * currentVertex));
            memcpy(m_MatrixBuffer,
                   m_ColorTransformData + p,
                   sizeof(GLfloat) * 16);
        }
        
        transform.setFromOpenGLMatrix(m_MatrixBuffer);
        return transform;
    }
}
