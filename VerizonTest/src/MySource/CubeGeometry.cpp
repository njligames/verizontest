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
    static const float N0 = 0.408248f;
    static const float N1 = 0.816497f;
    
    static const btVector3 BL_NORMAL = { -N0, -N0, N1 };
    static const btVector3 BR_NORMAL = {  N0, -N0, N1 };
    static const btVector3 TL_NORMAL = { -N0,  N0, N1 };
    static const btVector3 TR_NORMAL = {  N0,  N0, N1 };
    
    static const btVector3 BL_VERTEX = { -0.5f, -0.5f, 0.0f };
    static const btVector3 BR_VERTEX = {  0.5f, -0.5f, 0.0f };
    static const btVector3 TL_VERTEX = { -0.5f,  0.5f, 0.0f };
    static const btVector3 TR_VERTEX = {  0.5f,  0.5f, 0.0f };
    
    static const btVector2 BL_TEXTURECOORD = { 0.0f, 0.0f };
    static const btVector2 BR_TEXTURECOORD = { 1.0f, 0.0f };
    static const btVector2 TL_TEXTURECOORD = { 0.0f, 1.0f };
    static const btVector2 TR_TEXTURECOORD = { 1.0f, 1.0f };
    
    static const btVector4 DEFAULTCOLOR = {1.0f, 1.0f, 1.0f, 1.0f};
    
    
    
    static const float N3 = 0.57735f;
    
    static const btVector3 BLF_NORMAL = { -N3,  -N3,  -N3 };
    static const btVector3 BRF_NORMAL = {  N3,  -N3,  -N3 };
    static const btVector3 TLF_NORMAL = { -N3,   N3,  -N3 };
    static const btVector3 TRF_NORMAL = {  N3,   N3,  -N3 };
    static const btVector3 BLB_NORMAL = { -N3,  -N3,   N3 };
    static const btVector3 BRB_NORMAL = {  N3,  -N3,   N3 };
    static const btVector3 TLB_NORMAL = { -N3,   N3,   N3 };
    static const btVector3 TRB_NORMAL = {  N3,   N3,   N3 };
    
    static const btVector3 BLF_VERTEX = { -0.5f, -0.5f, -0.5f };
    static const btVector3 BRF_VERTEX = {  0.5f, -0.5f, -0.5f };
    static const btVector3 TLF_VERTEX = { -0.5f,  0.5f, -0.5f };
    static const btVector3 TRF_VERTEX = {  0.5f,  0.5f, -0.5f };
    static const btVector3 BLB_VERTEX = { -0.5f, -0.5f, 0.5f };
    static const btVector3 BRB_VERTEX = {  0.5f, -0.5f, 0.5f };
    static const btVector3 TLB_VERTEX = { -0.5f,  0.5f, 0.5f };
    static const btVector3 TRB_VERTEX = {  0.5f,  0.5f, 0.5f };
    
    
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
    m_ModelViewTransformData(new GLfloat[CubeGeometry::MAX_CUBES * numberOfVertices() * 16]),
    m_ColorTransformData(new GLfloat[CubeGeometry::MAX_CUBES * numberOfVertices() * 16]),
    m_NormalMatrixTransformData(new GLfloat[CubeGeometry::MAX_CUBES * numberOfVertices() * 16]),
//    m_VertexData(new Sprite[CubeGeometry::MAX_CUBES]),
    m_VertexData(new Cube[CubeGeometry::MAX_CUBES]),
    m_IndiceData(new GLushort[CubeGeometry::MAX_CUBES * numberOfIndices()]),
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
        assert(m_NormalMatrixTransformData);
        assert(m_VertexData);
        assert(m_IndiceData);
        
        unsigned long i;
        
        
        for (i = 0;
             i < (CubeGeometry::MAX_CUBES * numberOfVertices() * 16);
             i += 16)
            memcpy(m_ModelViewTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
        for (i = 0;
             i < (CubeGeometry::MAX_CUBES * numberOfVertices() * 16);
             i += 16)
            memcpy(m_ColorTransformData + i, COLOR_IDENTITY_MATRIX, sizeof(COLOR_IDENTITY_MATRIX));

        for (i = 0;
             i < (CubeGeometry::MAX_CUBES * numberOfVertices() * 16);
             i += 16)
            memcpy(m_NormalMatrixTransformData + i, TRANSFORM_IDENTITY_MATRIX, sizeof(TRANSFORM_IDENTITY_MATRIX));
        
        for(i=0; i<CubeGeometry::MAX_CUBES; i++)
        {
         
            /*m_VertexData[i].bl.vertex = BLF_VERTEX;
            m_VertexData[i].br.vertex = BRF_VERTEX;
            m_VertexData[i].tl.vertex = TLF_VERTEX;
            m_VertexData[i].tr.vertex = TRF_VERTEX;
            
//            m_VertexData[i].bl.texture = BLF_TEXTURECOORD;
//            m_VertexData[i].br.texture = BRF_TEXTURECOORD;
//            m_VertexData[i].tl.texture = TLF_TEXTURECOORD;
//            m_VertexData[i].tr.texture = TRF_TEXTURECOORD;
            
            m_VertexData[i].bl.normal = BLF_NORMAL;
            m_VertexData[i].br.normal = BRF_NORMAL;
            m_VertexData[i].tl.normal = TLF_NORMAL;
            m_VertexData[i].tr.normal = TRF_NORMAL;
            
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
            m_VertexData[i].tr.hidden = 0;*/
            
            
            
            
            
            
            
            
            m_VertexData[i].blf.vertex = BLF_VERTEX;
            m_VertexData[i].brf.vertex = BRF_VERTEX;
            m_VertexData[i].tlf.vertex = TLF_VERTEX;
            m_VertexData[i].trf.vertex = TRF_VERTEX;
            m_VertexData[i].blb.vertex = BLB_VERTEX;
            m_VertexData[i].brb.vertex = BRB_VERTEX;
            m_VertexData[i].tlb.vertex = TLB_VERTEX;
            m_VertexData[i].trb.vertex = TRB_VERTEX;
            
//            m_VertexData[i].blf.texture = BLF_TEXTURECOORD;
//            m_VertexData[i].brf.texture = BRF_TEXTURECOORD;
//            m_VertexData[i].tlf.texture = TLF_TEXTURECOORD;
//            m_VertexData[i].trf.texture = TRF_TEXTURECOORD;
//            m_VertexData[i].blb.texture = BLB_TEXTURECOORD;
//            m_VertexData[i].brb.texture = BRB_TEXTURECOORD;
//            m_VertexData[i].tlb.texture = TLB_TEXTURECOORD;
//            m_VertexData[i].trb.texture = TRB_TEXTURECOORD;

            m_VertexData[i].blf.normal = BLF_NORMAL;
            m_VertexData[i].brf.normal = BRF_NORMAL;
            m_VertexData[i].tlf.normal = TLF_NORMAL;
            m_VertexData[i].trf.normal = TRF_NORMAL;
            m_VertexData[i].blb.normal = BLB_NORMAL;
            m_VertexData[i].brb.normal = BRB_NORMAL;
            m_VertexData[i].tlb.normal = TLB_NORMAL;
            m_VertexData[i].trb.normal = TRB_NORMAL;
            
            m_VertexData[i].blf.color = DEFAULTCOLOR;
            m_VertexData[i].brf.color = DEFAULTCOLOR;
            m_VertexData[i].tlf.color = DEFAULTCOLOR;
            m_VertexData[i].trf.color = DEFAULTCOLOR;
            m_VertexData[i].blb.color = DEFAULTCOLOR;
            m_VertexData[i].brb.color = DEFAULTCOLOR;
            m_VertexData[i].tlb.color = DEFAULTCOLOR;
            m_VertexData[i].trb.color = DEFAULTCOLOR;
            
            m_VertexData[i].blf.opacity = 1.0f;
            m_VertexData[i].brf.opacity = 1.0f;
            m_VertexData[i].tlf.opacity = 1.0f;
            m_VertexData[i].trf.opacity = 1.0f;
            m_VertexData[i].blb.opacity = 1.0f;
            m_VertexData[i].brb.opacity = 1.0f;
            m_VertexData[i].tlb.opacity = 1.0f;
            m_VertexData[i].trb.opacity = 1.0f;
            
            m_VertexData[i].blf.hidden = 0;
            m_VertexData[i].brf.hidden = 0;
            m_VertexData[i].tlf.hidden = 0;
            m_VertexData[i].trf.hidden = 0;
            m_VertexData[i].blb.hidden = 0;
            m_VertexData[i].brb.hidden = 0;
            m_VertexData[i].tlb.hidden = 0;
            m_VertexData[i].trb.hidden = 0;
        }
        
        /*
        for(i=0;i< CubeGeometry::MAX_CUBES;i++)
        {
            m_IndiceData[i*numberOfIndices()+0] = i*4+0;
            m_IndiceData[i*numberOfIndices()+1] = i*4+1;
            m_IndiceData[i*numberOfIndices()+2] = i*4+2;
            m_IndiceData[i*numberOfIndices()+3] = i*4+1;
            m_IndiceData[i*numberOfIndices()+4] = i*4+3;
            m_IndiceData[i*numberOfIndices()+5] = i*4+2;
        }*/
        
        for(i=0;i< CubeGeometry::MAX_CUBES;i++)
        {
//            0, 1, 2, 1, 3, 2
            m_IndiceData[i*numberOfIndices()+0] = i*8+0;
            m_IndiceData[i*numberOfIndices()+1] = i*8+1;
            m_IndiceData[i*numberOfIndices()+2] = i*8+2;
            m_IndiceData[i*numberOfIndices()+3] = i*8+1;
            m_IndiceData[i*numberOfIndices()+4] = i*8+3;
            m_IndiceData[i*numberOfIndices()+5] = i*8+2;
            
//            4, 5, 6, 5, 7, 6
            m_IndiceData[i*numberOfIndices()+6]  = i*8+4;
            m_IndiceData[i*numberOfIndices()+7]  = i*8+5;
            m_IndiceData[i*numberOfIndices()+8]  = i*8+6;
            m_IndiceData[i*numberOfIndices()+9]  = i*8+5;
            m_IndiceData[i*numberOfIndices()+10] = i*8+7;
            m_IndiceData[i*numberOfIndices()+11] = i*8+6;
            
//            1, 5, 3, 5, 7, 3
            m_IndiceData[i*numberOfIndices()+12]  = i*8+1;
            m_IndiceData[i*numberOfIndices()+13]  = i*8+5;
            m_IndiceData[i*numberOfIndices()+14]  = i*8+3;
            m_IndiceData[i*numberOfIndices()+15]  = i*8+5;
            m_IndiceData[i*numberOfIndices()+16]  = i*8+7;
            m_IndiceData[i*numberOfIndices()+17]  = i*8+3;
            
//            0, 4, 1, 4, 5, 1
            m_IndiceData[i*numberOfIndices()+18]  = i*8+0;
            m_IndiceData[i*numberOfIndices()+19]  = i*8+4;
            m_IndiceData[i*numberOfIndices()+20]  = i*8+1;
            m_IndiceData[i*numberOfIndices()+21]  = i*8+4;
            m_IndiceData[i*numberOfIndices()+22]  = i*8+5;
            m_IndiceData[i*numberOfIndices()+23]  = i*8+1;
            
//            4, 0, 6, 0, 2, 6
            m_IndiceData[i*numberOfIndices()+24]  = i*8+4;
            m_IndiceData[i*numberOfIndices()+25]  = i*8+0;
            m_IndiceData[i*numberOfIndices()+26]  = i*8+6;
            m_IndiceData[i*numberOfIndices()+27]  = i*8+0;
            m_IndiceData[i*numberOfIndices()+28]  = i*8+2;
            m_IndiceData[i*numberOfIndices()+29]  = i*8+6;
            
//            6, 2, 7, 2, 3, 7
            m_IndiceData[i*numberOfIndices()+30]  = i*8+6;
            m_IndiceData[i*numberOfIndices()+31]  = i*8+2;
            m_IndiceData[i*numberOfIndices()+32]  = i*8+7;
            m_IndiceData[i*numberOfIndices()+33]  = i*8+2;
            m_IndiceData[i*numberOfIndices()+34]  = i*8+3;
            m_IndiceData[i*numberOfIndices()+35]  = i*8+7;
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
        
        delete [] m_NormalMatrixTransformData;
        m_NormalMatrixTransformData = NULL;
        
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
            
//            {
//                glGenBuffers(1, &m_ColorTransformBuffer);
//                glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
//                glBufferData(GL_ARRAY_BUFFER, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr(), GL_DYNAMIC_DRAW);
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
                glGenBuffers(1, &m_NormalMatrixTransformBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
                glBufferData(GL_ARRAY_BUFFER, getNormalMatrixTransformArrayBufferSize(), getNormalMatrixTransformArrayBufferPtr(), GL_DYNAMIC_DRAW);
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
                glGenBuffers(1, &m_VerticesBuffer);
                glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
                glBufferData(GL_ARRAY_BUFFER, getVertexArrayBufferSize(), getVertexArrayBufferPtr(), GL_DYNAMIC_DRAW);
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
            
//            glBindBuffer(GL_ARRAY_BUFFER, m_ColorTransformBuffer);
//            glBufferSubData(GL_ARRAY_BUFFER, 0, getColorTransformArrayBufferSize(), getColorTransformArrayBufferPtr());
//            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_NormalMatrixTransformBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, getNormalMatrixTransformArrayBufferSize(), getNormalMatrixTransformArrayBufferPtr());
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_VerticesBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexArrayBufferSize(), getVertexArrayBufferPtr());
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

            glDrawElements(GL_TRIANGLES, CubeGeometry::MAX_CUBES * numberOfIndices(), GL_UNSIGNED_SHORT, (const GLvoid*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArrayOES(0);
        }
    }
    
    void CubeGeometry::setOpacity(Node *node)
    {
        unsigned long index = node->getGeometryIndex();
        
        if(m_VertexData)
        {
            float opacity = node->getOpacity();
            float o = (opacity > 1.0f)?1.0f:((opacity<0.0f)?0.0f:opacity);
            
            /*m_VertexData[index].bl.opacity = o;
            m_VertexData[index].br.opacity = o;
            m_VertexData[index].tl.opacity = o;
            m_VertexData[index].tr.opacity = o;*/
            
            m_VertexData[index].blf.opacity = o;
            m_VertexData[index].brf.opacity = o;
            m_VertexData[index].tlf.opacity = o;
            m_VertexData[index].trf.opacity = o;
            
            m_VertexData[index].blb.opacity = o;
            m_VertexData[index].brb.opacity = o;
            m_VertexData[index].tlb.opacity = o;
            m_VertexData[index].trb.opacity = o;
        }
    }
    
    void CubeGeometry::setHidden(Node *node)
    {
        unsigned long index = node->getGeometryIndex();
        
        if(m_VertexData)
        {
            bool hidden = node->isHiddenGeometry();
            
            float h = (hidden)?1.0f:0.0f;
            
            /*if(m_VertexData[index].bl.hidden != (h) ||
               m_VertexData[index].br.hidden != (h) ||
               m_VertexData[index].tl.hidden != (h) ||
               m_VertexData[index].tr.hidden != (h))
            {
                m_VertexData[index].bl.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].br.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tl.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tr.hidden = (hidden)?1.0f:0.0f;
            }*/
            
            if(m_VertexData[index].blf.hidden != (h) ||
               m_VertexData[index].brf.hidden != (h) ||
               m_VertexData[index].tlf.hidden != (h) ||
               m_VertexData[index].trf.hidden != (h) ||
               m_VertexData[index].blb.hidden != (h) ||
               m_VertexData[index].brb.hidden != (h) ||
               m_VertexData[index].tlb.hidden != (h) ||
               m_VertexData[index].trb.hidden != (h))
            {
                m_VertexData[index].blf.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].brf.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tlf.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].trf.hidden = (hidden)?1.0f:0.0f;
                
                m_VertexData[index].blb.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].brb.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tlb.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].trb.hidden = (hidden)?1.0f:0.0f;
            }
        }
    }
    
    
    void CubeGeometry::setColorBase(Node *node)
    {
        unsigned long index = node->getGeometryIndex();
        
        if(m_VertexData)
        {
            
            btVector4 c(btFabs(node->getColorBase().x()),
                        btFabs(node->getColorBase().y()),
                        btFabs(node->getColorBase().z()),
                        btFabs(node->getColorBase().w()));
            
            /*m_VertexData[index].bl.color = c;
            m_VertexData[index].br.color = c;
            m_VertexData[index].tl.color = c;
            m_VertexData[index].tr.color = c;*/
            
            m_VertexData[index].blf.color = c;
            m_VertexData[index].brf.color = c;
            m_VertexData[index].tlf.color = c;
            m_VertexData[index].trf.color = c;
            m_VertexData[index].blb.color = c;
            m_VertexData[index].brb.color = c;
            m_VertexData[index].tlb.color = c;
            m_VertexData[index].trb.color = c;
        }
    }
    
    unsigned long CubeGeometry::numberOfVertices()const
    {
//        return Sprite::NUMBER_OF_VERTICES;
        return Cube::NUMBER_OF_VERTICES;
    }
    
    unsigned long CubeGeometry::numberOfIndices()const
    {
//        return Sprite::NUMBER_OF_INDICES;
        return Cube::NUMBER_OF_INDICES;
    }
    
    unsigned long CubeGeometry::maxNumberOfObjects()const
    {
        return MAX_CUBES;
    }
    
    const void *CubeGeometry::getModelViewTransformArrayBufferPtr()const
    {
        return m_ModelViewTransformData;
    }
    
    GLsizeiptr CubeGeometry::getModelViewTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * CubeGeometry::MAX_CUBES * numberOfVertices() * 16;
        return size;
    }
    
    const void *CubeGeometry::getColorTransformArrayBufferPtr()const
    {
        return m_ColorTransformData;
    }
    
    GLsizeiptr CubeGeometry::getColorTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * CubeGeometry::MAX_CUBES * numberOfVertices() * 16;
        return size;
    }
    
    const void *CubeGeometry::getNormalMatrixTransformArrayBufferPtr()const
    {
        return m_NormalMatrixTransformData;
    }
    
    GLsizeiptr CubeGeometry::getNormalMatrixTransformArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLfloat) * CubeGeometry::MAX_CUBES * numberOfVertices() * 16;
        return size;
    }
    
    const void *CubeGeometry::getVertexArrayBufferPtr()const
    {
        return (const void *)m_VertexData;
    }
    
    GLsizeiptr CubeGeometry::getVertexArrayBufferSize()const
    {
//        GLsizeiptr size = sizeof(Sprite) * CubeGeometry::MAX_CUBES;
        GLsizeiptr size = sizeof(Cube) * CubeGeometry::MAX_CUBES;
        return size;
    }
    
    const void *CubeGeometry::getElementArrayBufferPtr()const
    {
        return m_IndiceData;
    }
    
    GLsizeiptr CubeGeometry::getElementArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLushort) * CubeGeometry::MAX_CUBES * numberOfIndices();
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
            
            setHidden(node);
        }
    }
    
    void CubeGeometry::setTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < CubeGeometry::MAX_CUBES)
        {
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
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
        }
    }
    
    btTransform CubeGeometry::getTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < CubeGeometry::MAX_CUBES)
        {
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
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
    
    void CubeGeometry::setColorTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < CubeGeometry::MAX_CUBES)
        {
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
            transform.getOpenGLMatrix(m_MatrixBuffer);
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
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
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
            for (int currentVertex = 0; currentVertex < numberOfVertices(); currentVertex++)
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
    
    void CubeGeometry::setNormalMatrixTransform(const unsigned long index, const btTransform &transform)
    {
        if (index < CubeGeometry::MAX_CUBES)
        {
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
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
        }
    }
    
    btTransform CubeGeometry::getNormalMatrixTransform(const unsigned long index)
    {
        btTransform transform(btTransform::getIdentity());
        if (index < CubeGeometry::MAX_CUBES)
        {
//            const GLuint STRIDE = 64;
            const GLuint STRIDE = 16 * numberOfVertices();
            
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
