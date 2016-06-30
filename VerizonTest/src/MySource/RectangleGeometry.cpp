//
//  Rectangle.cpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "RectangleGeometry.hpp"
#include "Node.hpp"

namespace njli
{
    static const btVector4 DEFAULTCOLOR = {1.0f, 1.0f, 1.0f, 1.0f};
    
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
    
    RectangleGeometry::RectangleGeometry():
    Geometry(),
    m_VertexData(NULL),
    m_IndiceData(NULL)
    {
        
    }
    
    
    RectangleGeometry::~RectangleGeometry()
    {
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = NULL;
    }
    
    void RectangleGeometry::loadData()
    {
        Geometry::loadData();
        
        m_VertexData = new Rectangle[RectangleGeometry::MAX_CUBES];
        m_IndiceData = new GLushort[RectangleGeometry::MAX_CUBES * numberOfIndices()];
        
        assert(m_VertexData);
        assert(m_IndiceData);
        
        unsigned long i;
        
        for(i=0; i<RectangleGeometry::MAX_CUBES; i++)
        {
            m_VertexData[i].bl.vertex = BL_VERTEX;
            m_VertexData[i].br.vertex = BR_VERTEX;
            m_VertexData[i].tl.vertex = TL_VERTEX;
            m_VertexData[i].tr.vertex = TR_VERTEX;
            
//            m_VertexData[i].bl.texture = BL_TEXTURECOORD;
//            m_VertexData[i].br.texture = BR_TEXTURECOORD;
//            m_VertexData[i].tl.texture = TL_TEXTURECOORD;
//            m_VertexData[i].tr.texture = TR_TEXTURECOORD;
            
            m_VertexData[i].bl.normal = BL_NORMAL;
            m_VertexData[i].br.normal = BR_NORMAL;
            m_VertexData[i].tl.normal = TL_NORMAL;
            m_VertexData[i].tr.normal = TR_NORMAL;
            
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
        
        for(i=0;i< Geometry::MAX_CUBES;i++)
        {
            m_IndiceData[i*numberOfIndices()+0] = i*4+0;
            m_IndiceData[i*numberOfIndices()+1] = i*4+1;
            m_IndiceData[i*numberOfIndices()+2] = i*4+2;
            m_IndiceData[i*numberOfIndices()+3] = i*4+1;
            m_IndiceData[i*numberOfIndices()+4] = i*4+3;
            m_IndiceData[i*numberOfIndices()+5] = i*4+2;
        }
    }
    
    void RectangleGeometry::unLoadData()
    {
        Geometry::unLoadData();
        
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = NULL;
    }
    
    const void *RectangleGeometry::getVertexArrayBufferPtr()const
    {
        return (const void *)m_VertexData;
    }
    
    GLsizeiptr RectangleGeometry::getVertexArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(Rectangle) * RectangleGeometry::MAX_CUBES;
        return size;
    }
    
    const void *RectangleGeometry::getElementArrayBufferPtr()const
    {
        return m_IndiceData;
    }
    
    GLsizeiptr RectangleGeometry::getElementArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLushort) * RectangleGeometry::MAX_CUBES * numberOfIndices();
        return size;
    }
    
    GLenum RectangleGeometry::getElementIndexType()const
    {
        return GL_UNSIGNED_SHORT;
    }
    
    void RectangleGeometry::setOpacity(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            float opacity = node->getOpacity();
            float o = (opacity > 1.0f)?1.0f:((opacity<0.0f)?0.0f:opacity);
            
            m_VertexData[index].bl.opacity = o;
            m_VertexData[index].br.opacity = o;
            m_VertexData[index].tl.opacity = o;
            m_VertexData[index].tr.opacity = o;
        }
    }
    
    void RectangleGeometry::setHidden(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            bool hidden = node->isHiddenGeometry();
            
            float h = (hidden)?1.0f:0.0f;
            
            if(m_VertexData[index].bl.hidden != (h) ||
               m_VertexData[index].br.hidden != (h) ||
               m_VertexData[index].tl.hidden != (h) ||
               m_VertexData[index].tr.hidden != (h))
            {
                m_VertexData[index].bl.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].br.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tl.hidden = (hidden)?1.0f:0.0f;
                m_VertexData[index].tr.hidden = (hidden)?1.0f:0.0f;
            }
        }
    }
    
    
    void RectangleGeometry::setColorBase(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            
            btVector4 c(btFabs(node->getColorBase().x()),
                        btFabs(node->getColorBase().y()),
                        btFabs(node->getColorBase().z()),
                        btFabs(node->getColorBase().w()));
            
            m_VertexData[index].bl.color = c;
            m_VertexData[index].br.color = c;
            m_VertexData[index].tl.color = c;
            m_VertexData[index].tr.color = c;
        }
    }
    
    GLsizei RectangleGeometry::numberOfVertices()const
    {
        return Rectangle::NUMBER_OF_VERTICES;
    }
    
    GLsizei RectangleGeometry::numberOfIndices()const
    {
        return Rectangle::NUMBER_OF_INDICES;
    }
}