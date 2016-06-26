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
    static const btVector3 BL_VERTEX = { -0.5f, -0.5f, 0.0f };
    static const btVector3 BR_VERTEX = {  0.5f, -0.5f, 0.0f };
    static const btVector3 TL_VERTEX = { -0.5f,  0.5f, 0.0f };
    static const btVector3 TR_VERTEX = {  0.5f,  0.5f, 0.0f };
    
    static const btVector2 BL_TEXTURECOORD = { 0.0f, 0.0f };
    static const btVector2 BR_TEXTURECOORD = { 1.0f, 0.0f };
    static const btVector2 TL_TEXTURECOORD = { 0.0f, 1.0f };
    static const btVector2 TR_TEXTURECOORD = { 1.0f, 1.0f };
    
    static const btVector4 DEFAULTCOLOR = {1.0f, 1.0f, 1.0f, 1.0f};
    
    static const btVector2 DEFAULTPIVOT = btVector2(0.5f, 0.5f);
    
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
    
    
    CubeGeometry::CubeGeometry():
    m_ModelViewTransformData(new GLfloat[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16]),
    m_ColorTransformData(new GLfloat[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16]),
    m_VertexData(new SpriteQuad[CubeGeometry::MAX_CUBES]),
    m_IndiceData(new GLushort[CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_INDICES]),
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
        assert(m_VertexData);
        assert(m_IndiceData);
        
        unsigned long i;
        
        
        for (i = 0;
             i < (CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16);
             i += 16)
            memcpy(m_ModelViewTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
        for (i = 0;
             i < (CubeGeometry::MAX_CUBES * CubeGeometry::NUMBER_OF_VERTICES * 16);
             i += 16)
            memcpy(m_ColorTransformData + i, COLOR_IDENTITY_MATRIX, sizeof(COLOR_IDENTITY_MATRIX));

        for(i=0; i<CubeGeometry::MAX_CUBES; i++)
        {
            m_VertexData[i].bl.vertex = BL_VERTEX;
            m_VertexData[i].br.vertex = BR_VERTEX;
            m_VertexData[i].tl.vertex = TL_VERTEX;
            m_VertexData[i].tr.vertex = TR_VERTEX;
            
//            m_VertexData[i].bl.texture = BL_TEXTURECOORD;
//            m_VertexData[i].br.texture = BR_TEXTURECOORD;
//            m_VertexData[i].tl.texture = TL_TEXTURECOORD;
//            m_VertexData[i].tr.texture = TR_TEXTURECOORD;
            
            m_VertexData[i].bl.color = DEFAULTCOLOR;
            m_VertexData[i].br.color = DEFAULTCOLOR;
            m_VertexData[i].tl.color = DEFAULTCOLOR;
            m_VertexData[i].tr.color = DEFAULTCOLOR;
            
            m_VertexData[i].bl.opacity = 1.0f;
            m_VertexData[i].br.opacity = 1.0f;
            m_VertexData[i].tl.opacity = 1.0f;
            m_VertexData[i].tr.opacity = 1.0f;
            
            m_VertexData[i].bl.hidden = 0;
            m_VertexData[i].br.hidden = 0;
            m_VertexData[i].tl.hidden = 0;
            m_VertexData[i].tr.hidden = 0;
        }
        
        for(i=0;i< CubeGeometry::MAX_CUBES;i++)
        {
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+0] = i*4+0;
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+1] = i*4+1;
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+2] = i*4+2;
            
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+5] = i*4+2;
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+4] = i*4+3;
            m_IndiceData[i*CubeGeometry::NUMBER_OF_INDICES+3] = i*4+1;
        }

    }
    
    
    CubeGeometry::~CubeGeometry()
    {
                
        delete [] m_MatrixBuffer;
        m_MatrixBuffer = NULL;
        
        delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        delete [] m_VertexData;
        m_VertexData = NULL;
        
        delete [] m_ColorTransformData;
        m_ColorTransformData = NULL;
        
        delete [] m_ModelViewTransformData;
        m_ModelViewTransformData = NULL;
    }
    
    void CubeGeometry::load(Shader *shader)
    {
        assert(shader);
        
        m_Shader = shader;
        
        glGenVertexArraysOES(1, &m_VertexArray);
        glBindVertexArrayOES(m_VertexArray);
        {
            {
                glGenBuffers(1, &m_ModelViewBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferData(GL_ARRAY_BUFFER, getModelViewTransformArrayBufferSize(), getModelViewTransformArrayBufferPtr(), GL_DYNAMIC_DRAW);
                int inTransformAttrib = getShader()->getAttributeLocation("inTransform");
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
            
            {
                glGenBuffers(1, &m_ColorTransformBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
                glBufferData(GL_ARRAY_BUFFER, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr(), GL_DYNAMIC_DRAW);
                int inColorTransform = getShader()->getAttributeLocation("inColorTransform");
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
                glBufferData(GL_ARRAY_BUFFER, getVertexArrayBufferSize(), getVertexArrayBufferPtr(), GL_DYNAMIC_DRAW);
                int inPositionAttrib = getShader()->getAttributeLocation("inPosition");
                int inColorAttrib = getShader()->getAttributeLocation("inColor");
                int inOpacityAttrib = getShader()->getAttributeLocation("inOpacity");
                int inHiddenAttrib = getShader()->getAttributeLocation("inHidden");
//                int inTexCoordAttrib = getShader()->getAttributeLocation("inTexCoord");
                glEnableVertexAttribArray(inPositionAttrib);
                glVertexAttribPointer(inPositionAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(TexturedColoredVertex),
                                      (const GLvoid*) offsetof(TexturedColoredVertex, vertex));
                
//                glEnableVertexAttribArray(inTexCoordAttrib);
//                glVertexAttribPointer(inTexCoordAttrib,
//                                      2,
//                                      GL_FLOAT,
//                                      GL_FALSE,
//                                      sizeof(TexturedColoredVertex),
//                                      (const GLvoid*) offsetof(TexturedColoredVertex, texture));
                
                glEnableVertexAttribArray(inColorAttrib);
                glVertexAttribPointer(inColorAttrib,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(TexturedColoredVertex),
                                      (const GLvoid*) offsetof(TexturedColoredVertex, color));
                
                glEnableVertexAttribArray(inOpacityAttrib);
                glVertexAttribPointer(inOpacityAttrib,
                                      1,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(TexturedColoredVertex),
                                      (const GLvoid*)offsetof(TexturedColoredVertex, opacity));
                
                glEnableVertexAttribArray(inHiddenAttrib);
                glVertexAttribPointer(inHiddenAttrib,
                                      1,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(TexturedColoredVertex),
                                      (const GLvoid*)offsetof(TexturedColoredVertex, hidden));
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            
            {
                glGenBuffers(1, &m_IndexBuffer);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, getElementArrayBufferSize(), getElementArrayBufferPtr(), GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
//
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
//
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>((CubeGeometry::MAX_CUBES) * CubeGeometry::NUMBER_OF_INDICES), GL_UNSIGNED_SHORT, (const GLvoid*)0);
//
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
        return (const void *)m_VertexData;
    }
    
    GLsizeiptr CubeGeometry::getVertexArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(SpriteQuad) * CubeGeometry::MAX_CUBES;
        return size;
    }
    
    const void *CubeGeometry::getElementArrayBufferPtr()const
    {
        return m_IndiceData;
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
        if (index < CubeGeometry::MAX_CUBES)
        {
            const GLuint STRIDE = 64;
            
            transform.getOpenGLMatrix(m_MatrixBuffer);
            
            for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                int cmp = memcmp(m_ModelViewTransformData + p,
                                 m_MatrixBuffer,
                                 sizeof(float) * 16);
                
                if(0 != cmp)
                {
                    memcpy(m_ModelViewTransformData + p, m_MatrixBuffer, sizeof(float) * 16);
                }
            }
        }
    }
    
    btTransform CubeGeometry::getTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < CubeGeometry::MAX_CUBES)
        {
            const GLuint STRIDE = 64;
            
            for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                memcpy(m_MatrixBuffer,
                       m_ModelViewTransformData + p,
                       sizeof(float) * 16);
            }
            
            transform.setFromOpenGLMatrix(m_MatrixBuffer);
        }
        return transform;
    }
    
    void CubeGeometry::setColorTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < CubeGeometry::MAX_CUBES)
        {
            const GLuint STRIDE = 64;
            
            transform.getOpenGLMatrix(m_MatrixBuffer);
            
            for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                
                int cmp = memcmp(m_ColorTransformData + p,
                                 m_MatrixBuffer,
                                 sizeof(float) * 16);
                
                if(0 != cmp)
                {
                    memcpy(m_ColorTransformData + p,
                           m_MatrixBuffer,
                           sizeof(float) * 16);
                }
            }
        }
    }
    
    btTransform CubeGeometry::getColorTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < CubeGeometry::MAX_CUBES)
        {
            const GLuint STRIDE = 64;
            
            for (int currentVertex = 0; currentVertex < CubeGeometry::NUMBER_OF_VERTICES; currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                memcpy(m_MatrixBuffer,
                       m_ColorTransformData + p,
                       sizeof(float) * 16);
            }
            
            transform.setFromOpenGLMatrix(m_MatrixBuffer);
        }
        return transform;
    }
}
