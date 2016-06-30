//
//  Cube.cpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "CubeGeometry.hpp"
#include "Node.hpp"

namespace njli
{
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
    
    CubeGeometry::CubeGeometry():
    Geometry(),
    m_VertexData(NULL),
    m_IndiceData(NULL)
    {
        
    }
    
    
    CubeGeometry::~CubeGeometry()
    {
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = NULL;
    }
    
    void CubeGeometry::loadData()
    {
        Geometry::loadData();
        
        m_VertexData = new Cube[Geometry::MAX_CUBES];
        m_IndiceData = new GLushort[Geometry::MAX_CUBES * numberOfIndices()];
        
        assert(m_VertexData);
        assert(m_IndiceData);
        
        unsigned long i;
        
        for(i=0; i<Geometry::MAX_CUBES; i++)
        {
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
        
        for(i=0;i< Geometry::MAX_CUBES;i++)
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
    
    void CubeGeometry::unLoadData()
    {
        Geometry::unLoadData();
        
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = NULL;
    }
    
    const void *CubeGeometry::getVertexArrayBufferPtr()const
    {
        return (const void *)m_VertexData;
    }
    
    GLsizeiptr CubeGeometry::getVertexArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(Cube) * Geometry::MAX_CUBES;
        return size;
    }
    
    const void *CubeGeometry::getElementArrayBufferPtr()const
    {
        return m_IndiceData;
    }
    
    GLsizeiptr CubeGeometry::getElementArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLushort) * Geometry::MAX_CUBES * numberOfIndices();
        return size;
    }
    
    GLenum CubeGeometry::getElementIndexType()const
    {
        return GL_UNSIGNED_SHORT;
    }
    
    void CubeGeometry::setOpacity(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            float opacity = node->getOpacity();
            float o = (opacity > 1.0f)?1.0f:((opacity<0.0f)?0.0f:opacity);
            
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
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            bool hidden = node->isHiddenGeometry();
            
            float h = (hidden)?1.0f:0.0f;
            
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
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            
            btVector4 c(btFabs(node->getColorBase().x()),
                        btFabs(node->getColorBase().y()),
                        btFabs(node->getColorBase().z()),
                        btFabs(node->getColorBase().w()));
            
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
    
    GLsizei CubeGeometry::numberOfVertices()const
    {
        return Cube::NUMBER_OF_VERTICES;
    }
    
    GLsizei CubeGeometry::numberOfIndices()const
    {
        return Cube::NUMBER_OF_INDICES;
    }
}
