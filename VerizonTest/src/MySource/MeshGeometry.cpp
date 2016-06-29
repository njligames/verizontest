//
//  MeshGeometry.cpp
//  VerizonTest
//
//  Created by James Folk on 6/28/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "MeshGeometry.hpp"

#include "Node.hpp"
#include <string>
#include <iostream>
#include <sstream>

namespace njli
{
    MeshGeometry::MeshGeometry():
    Geometry(),
    m_VertexData(NULL),
    m_IndiceData(NULL),
    m_Filedata(""),
    m_NumberOfVertices(0),
    m_NumberOfIndices(0)
    {
    }
    
    
    MeshGeometry::~MeshGeometry()
    {
    }
    
    void MeshGeometry::load(Shader *shader, const std::string &filedata, MeshType type)
    {
        m_Filedata = filedata;
        
        Geometry::load(shader);
    }
    
    void MeshGeometry::loadData()
    {
        
        
        std::vector<btVector3> vertices;
        std::vector<btVector3> normals;
        std::vector<btVector2> texture;
        std::vector<std::string> faces;
        
        std::stringstream ss_line(m_Filedata);
        std::string line;
        
        enum parsemode{none,v,vn,vt,f};
        parsemode mode = none;
        btVector3 vec3;
        btVector2 vec2;
        
        while(std::getline(ss_line, line, '\n'))
        {
            std::stringstream ss_token(line);
            std::string token;
            int tokencount = 0;
            
            while(std::getline(ss_token, token, ' '))
            {
                if(tokencount == 0)
                {
                    if(token == "v")
                        mode = v;
                    else if(token == "vt")
                        mode = vt;
                    else if(token == "vn")
                        mode = vn;
                    else if (token == "f")
                        mode = f;
                }
                else
                {
                    switch (mode)
                    {
                        case v:
                        case vn:
                        {
                            switch (tokencount)
                            {
                                case 1:vec3.setX(atof(token.c_str()));break;
                                case 2:vec3.setY(atof(token.c_str()));break;
                                case 3:vec3.setZ(atof(token.c_str()));break;
                                default:
                                    assert(false);
                            }
                        }
                            break;
                        case vt:
                        {
                            switch (tokencount)
                            {
                                case 1:vec2.setX(atof(token.c_str()));break;
                                case 2:vec2.setY(atof(token.c_str()));break;
                                default:
                                    assert(false);
                            }
                        }
                            break;
                        case f:
                        {
                            faces.push_back(token);
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
                tokencount++;
            }
            
            
            
            switch (mode)
            {
                case v:
                    vertices.push_back(vec3);
                    break;
                case vn:
                    normals.push_back(vec3);
                    break;
                case vt:
                    texture.push_back(vec2);
                    break;
                case f:
                {
//                        for (std::vector<std::string>::iterator i = faces.begin();
//                             i != faces.end();
//                             i++)
//                        {
//                            std::cout << *i << std::endl;
//                        }
                }
                    break;
                    
                default:
                    break;
            }
            mode = none;
        }
        
        m_NumberOfIndices = (GLsizei)faces.size();
        m_NumberOfVertices = (GLsizei)faces.size();
        
        unsigned long vertexIndex = 0;
        TexturedColoredVertex *vertexData = new TexturedColoredVertex[numberOfVertices()];
        for (std::vector<std::string>::iterator i = faces.begin();
             i != faces.end();
             i++)
        {
            std::stringstream ss_faceData(*i);
            std::string faceData;
            int ii = 0;
            
            while(std::getline(ss_faceData, faceData, '/'))
            {
                unsigned long idx = atoi(faceData.c_str()) - 1;
                std::cout << idx << std::endl;
                
                switch (ii)
                {
                    case 0:
                        //vertex idx
                        assert(idx < vertices.size());
                        vertexData[vertexIndex].vertex = vertices.at(idx);
                        break;
                    case 1:
                        assert(idx < texture.size());
                        //texture idx
//                            vertexData[vertexIndex].texture = texture.at(idx);
                        break;
                    case 2:
                        assert(idx < normals.size());
                        //normal idx
                        vertexData[vertexIndex].normal = normals.at(idx);
                        break;
                        
                    default:
                        assert(false);
                        break;
                }
                ii++;
            }
            vertexData[vertexIndex].hidden = 0.0f;
            vertexData[vertexIndex].opacity = 1.0f;
            vertexData[vertexIndex].color = btVector4(1.0f, 1.0f, 1.0f, 1.0f);
            vertexIndex++;
        }
        
        GLushort *indiceData = new GLushort[numberOfIndices()];
        for (unsigned long indiceIndex = 0; indiceIndex < numberOfIndices(); indiceIndex++)
        {
            indiceData[indiceIndex] = indiceIndex;
        }
        
        Geometry::loadData();
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = new TexturedColoredVertex[numberOfVertices() * maxNumberOfObjects()];
        
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = new GLushort[numberOfIndices() * maxNumberOfObjects()];
        
        for (unsigned long meshIndex = 0; meshIndex < maxNumberOfObjects(); meshIndex++)
        {
            unsigned long offset = (meshIndex * numberOfVertices());
            
            for (unsigned long vertexIndex = 0; vertexIndex < numberOfVertices(); vertexIndex++)
            {
                m_VertexData[meshIndex + offset] = vertexData[vertexIndex];
            }
            
            offset = (meshIndex * numberOfIndices());
            for (unsigned long indiceIndex = 0; indiceIndex < numberOfIndices(); indiceIndex++)
            {
                m_IndiceData[indiceIndex + offset] = indiceData[indiceIndex];
            }
        }
        
        delete [] indiceData;
        indiceData = NULL;
        
        delete [] vertexData;
        vertexData = NULL;
    }
    
    void MeshGeometry::unLoadData()
    {
        Geometry::unLoadData();
        
        if(m_IndiceData)
            delete [] m_IndiceData;
        m_IndiceData = NULL;
        
        if(m_VertexData)
            delete [] m_VertexData;
        m_VertexData = NULL;
    }
    
    const void *MeshGeometry::getVertexArrayBufferPtr()const
    {
        return (const void *)m_VertexData;
    }
    
    GLsizeiptr MeshGeometry::getVertexArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(TexturedColoredVertex) * numberOfVertices() * maxNumberOfObjects();
        return size;
    }
    
    const void *MeshGeometry::getElementArrayBufferPtr()const
    {
        return m_IndiceData;
    }
    
    GLsizeiptr MeshGeometry::getElementArrayBufferSize()const
    {
        GLsizeiptr size = sizeof(GLushort) * numberOfIndices() * maxNumberOfObjects();
        return size;
    }
    
    void MeshGeometry::setOpacity(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
//        if(m_VertexData)
//        {
//            float opacity = node->getOpacity();
//            float o = (opacity > 1.0f)?1.0f:((opacity<0.0f)?0.0f:opacity);
//            
//            m_VertexData[index].bl.opacity = o;
//            m_VertexData[index].br.opacity = o;
//            m_VertexData[index].tl.opacity = o;
//            m_VertexData[index].tr.opacity = o;
//        }
    }
    
    void MeshGeometry::setHidden(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            bool hidden = node->isHiddenGeometry();
            
            float h = (hidden)?1.0f:0.0f;
            
//            if(m_VertexData[index].bl.hidden != (h) ||
//               m_VertexData[index].br.hidden != (h) ||
//               m_VertexData[index].tl.hidden != (h) ||
//               m_VertexData[index].tr.hidden != (h))
//            {
//                m_VertexData[index].bl.hidden = (hidden)?1.0f:0.0f;
//                m_VertexData[index].br.hidden = (hidden)?1.0f:0.0f;
//                m_VertexData[index].tl.hidden = (hidden)?1.0f:0.0f;
//                m_VertexData[index].tr.hidden = (hidden)?1.0f:0.0f;
//            }
        }
    }
    
    
    void MeshGeometry::setColorBase(Node *node)
    {
        unsigned long index = getGeometryIndex(node);
        
        if(m_VertexData)
        {
            
            btVector4 c(btFabs(node->getColorBase().x()),
                        btFabs(node->getColorBase().y()),
                        btFabs(node->getColorBase().z()),
                        btFabs(node->getColorBase().w()));
            
//            m_VertexData[index].bl.color = c;
//            m_VertexData[index].br.color = c;
//            m_VertexData[index].tl.color = c;
//            m_VertexData[index].tr.color = c;
        }
    }
    
    GLsizei MeshGeometry::numberOfVertices()const
    {
        return m_NumberOfVertices;
    }
    
    GLsizei MeshGeometry::numberOfIndices()const
    {
        return m_NumberOfIndices;
    }
}