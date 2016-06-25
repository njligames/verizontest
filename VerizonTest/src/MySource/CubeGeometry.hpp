//
//  CubeGeometry.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef CubeGeometry_hpp
#define CubeGeometry_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include <bitset>

#include "btTransform.h"

namespace njli
{
    class Shader;
    class Camera;
    class Node;
    
    class CubeGeometry
    {
        friend class Node;
        friend class Scene;
        
    public:
        
        ATTRIBUTE_ALIGNED16(struct) VertexData
        {
            VertexData():
            m_Vertex(btVector3(0.0f, 0.0f, 0.0f)),
            m_Color(btVector4(1.0f, 1.0f, 1.0f, 1.0f)),
//            m_Normal(normal),
//            m_Texture(texture),
            m_Opacity(1.0f),
            m_Hidden(0.0f)
            {}
            
            VertexData(const btVector3 vertex,
                       const btVector4 color,
//                       const btVector3 normal,
//                       const btVector2 texture,
                       const GLfloat opacity,
                       const GLfloat hidden):
            m_Vertex(vertex),
            m_Color(color),
//            m_Normal(normal),
//            m_Texture(texture),
            m_Opacity(opacity),
            m_Hidden(hidden)
            {
            }
            
            btVector3 m_Vertex;
            btVector4 m_Color;
            GLfloat m_Opacity;
            GLfloat m_Hidden;
        };
        
        ATTRIBUTE_ALIGNED16(struct) CubeVerts
        {
            VertexData vertices[24];
        };
        
        /* members */
        CubeGeometry();
        CubeGeometry(const CubeGeometry &rhs);
        const CubeGeometry &operator=(const CubeGeometry &rhs);
        ~CubeGeometry();
        
        void load(Shader *shader);
        void unLoad();
        bool isLoaded()const;
        
        Shader *const getShader();
        
        void render(Camera *camera);
        
        void setOpacity(Node *node, GLfloat opacity);
        void setHidden(Node *node, bool hidden = true);
        bool isHidden(Node *node)const;
        
        static const unsigned int MAX_CUBES = 3;
        static const unsigned int NUMBER_OF_VERTICES = 24;
        static const unsigned int NUMBER_OF_INDICES = 36;
    protected:
        const void *getModelViewTransformArrayBufferPtr()const;
        GLsizeiptr getModelViewTransformArrayBufferSize()const;
        
        const void *getColorTransformArrayBufferPtr()const;
        GLsizeiptr getColorTransformArrayBufferSize()const;
        
        const void *getVertexArrayBufferPtr()const;
        GLsizeiptr getVertexArrayBufferSize()const;
        
        const void *getElementArrayBufferPtr()const;
        GLsizeiptr getElementArrayBufferSize()const;
        
        void addReference(Node *node);
        void removeReference(Node *node);
        void hideGeometry(Node *node);
        
        void setTransform(const unsigned long index, const btTransform &transform);
        btTransform getTransform(const unsigned long index);
        void setColorTransform(const unsigned long index, const btTransform &transform);
        btTransform getColorTransform(const unsigned long index);
        
    private:
        
        
        GLfloat *m_ModelViewTransformData;
        GLfloat *m_ColorTransformData;
        
        CubeVerts *m_CubeVertexData;
        GLushort *m_CubeIndiceData;
        
        GLuint m_VertexArray;
        GLuint m_ModelViewBuffer;
        GLuint m_ColorTransformBuffer;
        GLuint m_VerticesBuffer;
        GLuint m_IndexBuffer;
        
        std::bitset<MAX_CUBES> m_References;
        
        Shader *m_Shader;
        
        bool m_OpacityModifyRGB;
        
        GLfloat *m_MatrixBuffer;
        
    };
}

#endif /* CubeGeometry_hpp */
