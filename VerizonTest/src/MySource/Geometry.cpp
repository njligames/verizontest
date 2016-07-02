//
//  Geometry.cpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "Geometry.hpp"

#include <assert.h>
#include <iostream>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Node.hpp"

#include "btVector3.h"
#include "btVector2.h"

namespace njli
{
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
    
    
    Geometry::Geometry():
    m_MatrixBuffer(new GLfloat[16]),
    m_ModelViewTransformData(NULL),
//    m_ColorTransformData(NULL),
    m_NormalMatrixTransformData(NULL),
    m_VertexArray(0),
    m_ModelViewBuffer(0),
//    m_ColorTransformBuffer(0),
    m_NormalMatrixTransformBuffer(0),
    m_VerticesBuffer(0),
    m_IndexBuffer(0),
    m_Shader(NULL),
    m_OpacityModifyRGB(false),
    m_VertexBufferChanged(true),
    m_NormalMatrixBufferChanged(true),
    m_ModelViewBufferChanged(true),
    m_ShaderChanged(true)
    {
        assert(m_MatrixBuffer);
    }
    
    
    Geometry::~Geometry()
    {
        if(m_NormalMatrixTransformData)
            delete [] m_NormalMatrixTransformData;
        m_NormalMatrixTransformData = NULL;
        
//        if(m_ColorTransformData)
//            delete [] m_ColorTransformData;
//        m_ColorTransformData = NULL;
        
        if(m_ModelViewTransformData)
            delete [] m_ModelViewTransformData;
        m_ModelViewTransformData = NULL;
        
        delete [] m_MatrixBuffer;
        m_MatrixBuffer = NULL;
    }
    
    void Geometry::load(Shader *shader, const std::string &filecontent, MeshType type)
    {
        assert(shader);
        
        setShader(shader);
        
        loadData();
        
        assert(m_VertexArray == 0);
        glGenVertexArraysOES(1, &m_VertexArray);
        glBindVertexArrayOES(m_VertexArray);
        {
            {
                assert(m_ModelViewBuffer == 0);
                glGenBuffers(1, &m_ModelViewBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferData(GL_ARRAY_BUFFER, getModelViewTransformArrayBufferSize(), getModelViewTransformArrayBufferPtr(), GL_STREAM_DRAW);
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
            
//            {
//            assert(m_ColorTransformBuffer == 0);
//                glGenBuffers(1, &m_ColorTransformBuffer);
//                glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
//                glBufferData(GL_ARRAY_BUFFER, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr(), GL_STREAM_DRAW);
//                int inColorTransform = getShader()->getAttributeLocation("inColorTransform");
//                glEnableVertexAttribArray(inColorTransform + 0);
//                glEnableVertexAttribArray(inColorTransform + 1);
//                glEnableVertexAttribArray(inColorTransform + 2);
//                glEnableVertexAttribArray(inColorTransform + 3);
//                glVertexAttribPointer(inColorTransform + 0, 4, GL_FLOAT, 0, 64, (GLvoid*)0);
//                glVertexAttribPointer(inColorTransform + 1, 4, GL_FLOAT, 0, 64, (GLvoid*)16);
//                glVertexAttribPointer(inColorTransform + 2, 4, GL_FLOAT, 0, 64, (GLvoid*)32);
//                glVertexAttribPointer(inColorTransform + 3, 4, GL_FLOAT, 0, 64, (GLvoid*)48);
//                glBindBuffer(GL_ARRAY_BUFFER, 0);
//            }
            
            {
                assert(m_NormalMatrixTransformBuffer == 0);
                glGenBuffers(1, &m_NormalMatrixTransformBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
                glBufferData(GL_ARRAY_BUFFER, getNormalMatrixTransformArrayBufferSize(), getNormalMatrixTransformArrayBufferPtr(), GL_STREAM_DRAW);
                int inNormalMatrixAttrib = getShader()->getAttributeLocation("inNormalMatrix");
                glEnableVertexAttribArray(inNormalMatrixAttrib + 0);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 1);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 2);
                glEnableVertexAttribArray(inNormalMatrixAttrib + 3);
                glVertexAttribPointer(inNormalMatrixAttrib + 0, 4, GL_FLOAT, 0, 64, (GLvoid*)0);
                glVertexAttribPointer(inNormalMatrixAttrib + 1, 4, GL_FLOAT, 0, 64, (GLvoid*)16);
                glVertexAttribPointer(inNormalMatrixAttrib + 2, 4, GL_FLOAT, 0, 64, (GLvoid*)32);
                glVertexAttribPointer(inNormalMatrixAttrib + 3, 4, GL_FLOAT, 0, 64, (GLvoid*)48);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            {
                assert(m_VerticesBuffer == 0);
                glGenBuffers(1, &m_VerticesBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferData(GL_ARRAY_BUFFER, getVertexArrayBufferSize(), getVertexArrayBufferPtr(), GL_STREAM_DRAW);
                int inPositionAttrib = getShader()->getAttributeLocation("inPosition");
                int inColorAttrib = getShader()->getAttributeLocation("inColor");
                int inNormalAttrib = getShader()->getAttributeLocation("inNormal");
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
                
                glEnableVertexAttribArray(inNormalAttrib);
                glVertexAttribPointer(inNormalAttrib,
                                      3,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(TexturedColoredVertex),
                                      (const GLvoid*) offsetof(TexturedColoredVertex, normal));
                
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
    
    void Geometry::unLoad()
    {
        if (m_IndexBuffer)
            glDeleteBuffers(1, &m_IndexBuffer);
        m_IndexBuffer = 0;
        
        if (m_VerticesBuffer)
            glDeleteBuffers(1, &m_VerticesBuffer);
        m_VerticesBuffer = 0;
        
        if (m_NormalMatrixTransformBuffer)
            glDeleteBuffers(1, &m_NormalMatrixTransformBuffer);
        m_NormalMatrixTransformBuffer = 0;
        
//        if (m_ColorTransformBuffer)
//            glDeleteBuffers(1, &m_ColorTransformBuffer);
//        m_ColorTransformBuffer = 0;
        
        if (m_ModelViewBuffer)
            glDeleteBuffers(1, &m_ModelViewBuffer);
        m_ModelViewBuffer = 0;
        
        if (m_VertexArray)
            glDeleteVertexArraysOES(1, &m_VertexArray);
        m_VertexArray = 0;
    }
    
    bool Geometry::isLoaded()const
    {
        return false;
    }
    
    Shader *const Geometry::getShader()
    {
        return m_Shader;
    }
    
    void Geometry::setShader(Shader *const shader)
    {
        m_Shader = shader;
        m_ShaderChanged = true;
    }
    
    void Geometry::render(Camera *camera)
    {
        Shader *shader = getShader();
        if(shader && camera)
        {
            assert(shader->use());
            
            camera->render(shader, m_ShaderChanged);
            m_ShaderChanged = false;
            
            glBindVertexArrayOES(m_VertexArray);
            
            if(isModelViewBufferChanged())
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_ModelViewBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getModelViewTransformArrayBufferSize(), getModelViewTransformArrayBufferPtr());
                enableModelViewBufferChanged(false);
            }
            
//            glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
//            glBufferSubData(GL_ARRAY_BUFFER, 0, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr());
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            if(isNormalMatrixBufferChanged())
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getNormalMatrixTransformArrayBufferSize(), getNormalMatrixTransformArrayBufferPtr());
                enableNormalMatrixBufferChanged(false);
            }
            
            if(isVertexArrayBufferChanged())
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexArrayBufferSize(), getVertexArrayBufferPtr());
                enableVertexArrayBufferChanged(false);
            }
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
            
            glDrawElements(GL_TRIANGLES, maxNumberOfObjects() * numberOfIndices(), getElementIndexType(), (const GLvoid*)0);
            
//            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//            glBindVertexArrayOES(0);
        }
    }
    
    GLsizei Geometry::maxNumberOfObjects()const
    {
        return MAX_CUBES;
    }
    
    unsigned long Geometry::getGeometryIndex(Node *const node)const
    {
        return node->getGeometryIndex();
    }
    
    const void *Geometry::getModelViewTransformArrayBufferPtr()const
    {
        assert(m_ModelViewTransformData);
        
        return m_ModelViewTransformData;
    }
    
    GLsizeiptr Geometry::getModelViewTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * maxNumberOfObjects() * numberOfVertices() * 16;
        return size;
    }
    
    bool Geometry::isModelViewBufferChanged()const
    {
        return m_ModelViewBufferChanged;
    }
    
    void Geometry::enableModelViewBufferChanged(bool changed)
    {
        m_ModelViewBufferChanged = changed;
    }
    
//    const void *Geometry::getColorTransformArrayBufferPtr()const
//    {
//        assert(m_ColorTransformData);
//        
//        return m_ColorTransformData;
//    }
//    
//    GLsizeiptr Geometry::getColorTransformArrayBufferSize()const
//    {
//        GLsizeiptr size = sizeof(GLfloat) * maxNumberOfObjects() * numberOfVertices() * 16;
//        return size;
//    }
    
    const void *Geometry::getNormalMatrixTransformArrayBufferPtr()const
    {
        assert(m_NormalMatrixTransformData);
        
        return m_NormalMatrixTransformData;
    }
    
    GLsizeiptr Geometry::getNormalMatrixTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * maxNumberOfObjects() * numberOfVertices() * 16;
        return size;
    }
    
    bool Geometry::isNormalMatrixBufferChanged()const
    {
        return m_NormalMatrixBufferChanged;
    }
    
    void Geometry::enableNormalMatrixBufferChanged(bool changed)
    {
        m_NormalMatrixBufferChanged = changed;
    }
    
    void Geometry::loadData()
    {
        unLoadData();
        
        m_ModelViewTransformData = new GLfloat[maxNumberOfObjects() * numberOfVertices() * 16];
//        m_ColorTransformData = new GLfloat[maxNumberOfObjects() * numberOfVertices() * 16];
        m_NormalMatrixTransformData = new GLfloat[maxNumberOfObjects() * numberOfVertices() * 16];
        enableNormalMatrixBufferChanged(true);
        
        assert(m_ModelViewTransformData);
//        assert(m_ColorTransformData);
        assert(m_NormalMatrixTransformData);
        
        unsigned long i;
        
        for (i = 0;
             i < (maxNumberOfObjects() * numberOfVertices() * 16);
             i += 16)
            memcpy(m_ModelViewTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
//        for (i = 0;
//             i < (maxNumberOfObjects() * numberOfVertices() * 16);
//             i += 16)
//            memcpy(m_ColorTransformData + i, COLOR_IDENTITY_MATRIX, sizeof(COLOR_IDENTITY_MATRIX));
        
        for (i = 0;
             i < (maxNumberOfObjects() * numberOfVertices() * 16);
             i += 16)
            memcpy(m_NormalMatrixTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
        enableModelViewBufferChanged(true);
        enableNormalMatrixBufferChanged(true);
    }
    
    void Geometry::unLoadData()
    {
        if(m_NormalMatrixTransformData)
            delete [] m_NormalMatrixTransformData;
        m_NormalMatrixTransformData = NULL;
        
//        if(m_ColorTransformData)
//            delete [] m_ColorTransformData;
//        m_ColorTransformData = NULL;
        
        if(m_ModelViewTransformData)
            delete [] m_ModelViewTransformData;
        m_ModelViewTransformData = NULL;
    }
    
    bool Geometry::isVertexArrayBufferChanged()const
    {
        return m_VertexBufferChanged;
    }
    
    void Geometry::enableVertexArrayBufferChanged(bool changed)
    {
        m_VertexBufferChanged = changed;
    }
    
    void Geometry::addReference(Node *node)
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
    
    void Geometry::removeReference(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(index < m_References.size())
        {
            m_References[index] = 0;
            
            setHidden(node);
        }
    }
    
    void Geometry::setTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < maxNumberOfObjects())
        {
            const unsigned long STRIDE = 16 * numberOfVertices();
            
            transform.getOpenGLMatrix(m_MatrixBuffer);
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
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
            enableModelViewBufferChanged(true);
        }
    }
    
    btTransform Geometry::getTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < maxNumberOfObjects())
        {
            const unsigned long STRIDE = 16 * numberOfVertices();
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
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
    
//    void Geometry::setColorTransform(const unsigned long index, const btTransform &transform)
//    {
//        if (index < maxNumberOfObjects())
//        {
//            const unsigned long STRIDE = 16 * numberOfVertices();
//            
//            transform.getOpenGLMatrix(m_MatrixBuffer);
//            
//            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
//            {
//                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
//                
//                int cmp = memcmp(m_ColorTransformData + p,
//                                 m_MatrixBuffer,
//                                 sizeof(float) * 16);
//                
//                if(0 != cmp)
//                {
//                    memcpy(m_ColorTransformData + p,
//                           m_MatrixBuffer,
//                           sizeof(float) * 16);
//                }
//            }
//        }
//    }
//    
//    btTransform Geometry::getColorTransform(const unsigned long index)
//    {
//        btTransform transform(btTransform::getIdentity());
//        if (index < maxNumberOfObjects())
//        {
//            const unsigned long STRIDE = 16 * numberOfVertices();
//            
//            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
//            {
//                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
//                memcpy(m_MatrixBuffer,
//                       m_ColorTransformData + p,
//                       sizeof(float) * 16);
//            }
//            
//            transform.setFromOpenGLMatrix(m_MatrixBuffer);
//        }
//        return transform;
//    }
    
    void Geometry::setNormalMatrixTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < maxNumberOfObjects())
        {
            const unsigned long STRIDE = 16 * numberOfVertices();
            
            transform.getOpenGLMatrix(m_MatrixBuffer);
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                
                int cmp = memcmp(m_NormalMatrixTransformData + p,
                                 m_MatrixBuffer,
                                 sizeof(float) * 16);
                
                if(0 != cmp)
                {
                    memcpy(m_NormalMatrixTransformData + p,
                           m_MatrixBuffer,
                           sizeof(float) * 16);
                }
            }
            enableNormalMatrixBufferChanged(true);
        }
    }
    
    btTransform Geometry::getNormalMatrixTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < maxNumberOfObjects())
        {
            const unsigned long STRIDE = 16 * numberOfVertices();
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
            {
                unsigned long p = ((index * STRIDE) + (16 * currentVertex));
                memcpy(m_MatrixBuffer,
                       m_NormalMatrixTransformData + p,
                       sizeof(float) * 16);
            }
            
            transform.setFromOpenGLMatrix(m_MatrixBuffer);
        }
        return transform;
    }
}
