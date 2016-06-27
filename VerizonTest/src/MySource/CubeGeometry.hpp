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
#include "btVector2.h"

namespace njli
{
    ATTRIBUTE_ALIGNED16(struct)
    TexturedColoredVertex
    {
        TexturedColoredVertex()
        : vertex(0, 0, 0)
        , color(1, 1, 1, 1)
//        , texture(0, 0)
        , normal(1,1,1)
        , opacity(1.0f)
        , hidden(0.0f)
        {
        }
        TexturedColoredVertex(const btVector3 vertex,
                              const btVector4 color,
//                              const btVector2 texture,
                              const btVector3 normal,
                              const GLfloat opacity,
                              const GLfloat hidden)
        : vertex(vertex)
        , color(color)
//        , texture(texture)
        , normal(normal)
        , opacity(opacity)
        , hidden(hidden)
        {
        }
        btVector3 vertex;
        btVector4 color;
//        btVector2 texture;
        btVector3 normal;
        GLfloat opacity;
        GLfloat hidden;
        
        TexturedColoredVertex& operator=(const TexturedColoredVertex& rhs)
        {
            if (this != &rhs) {
                vertex = rhs.vertex;
                color = rhs.color;
//                texture = rhs.texture;
                normal = rhs.normal;
                opacity = rhs.opacity;
                hidden = rhs.hidden;
            }
            return *this;
        }
    };
    
    class Sprite
    {
    public:
        TexturedColoredVertex bl;
        TexturedColoredVertex br;
        TexturedColoredVertex tl;
        TexturedColoredVertex tr;
        
        static const unsigned int NUMBER_OF_VERTICES = 4;
        static const unsigned int NUMBER_OF_INDICES = 6;
        
        Sprite &operator=(const Sprite &rhs)
        {
            if(this != &rhs)
            {
                bl = rhs.bl;
                br = rhs.br;
                tl = rhs.tl;
                tr = rhs.tr;
            }
            return *this;
        }
    };
    
    class Cube
    {
    public:
        TexturedColoredVertex blf; //bottom, left, front
        TexturedColoredVertex brf; //bottom, right, front
        TexturedColoredVertex tlf; //top, left, front
        TexturedColoredVertex trf; //top, right, front
        
        TexturedColoredVertex blb; //bottom, left, back
        TexturedColoredVertex brb; //bottom, right, back
        TexturedColoredVertex tlb; //bottom, left, back
        TexturedColoredVertex trb; //bottom, right, back
        
        static const unsigned int NUMBER_OF_VERTICES = 8;
        static const unsigned int NUMBER_OF_INDICES = 36;
        
        Cube &operator=(const Cube &rhs)
        {
            if(this != &rhs)
            {
                blf = rhs.blf;
                brf = rhs.brf;
                tlf = rhs.tlf;
                trf = rhs.trf;
                
                blb = rhs.blb;
                brb = rhs.brb;
                tlb = rhs.tlb;
                trb = rhs.trb;

            }
            return *this;
        }
    };
    
    
    class Shader;
    class Camera;
    class Node;

    class CubeGeometry
    {
        friend class Node;
        friend class Scene;
        
    public:
        
        
        
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
        
        static const unsigned int MAX_CUBES = 10000;
        
    protected:
        const void *getModelViewTransformArrayBufferPtr()const;
        GLsizeiptr getModelViewTransformArrayBufferSize()const;
        
        const void *getColorTransformArrayBufferPtr()const;
        GLsizeiptr getColorTransformArrayBufferSize()const;
        
        const void *getNormalMatrixTransformArrayBufferPtr()const;
        GLsizeiptr getNormalMatrixTransformArrayBufferSize()const;
        
        const void *getVertexArrayBufferPtr()const;
        GLsizeiptr getVertexArrayBufferSize()const;
        
        const void *getElementArrayBufferPtr()const;
        GLsizeiptr getElementArrayBufferSize()const;
        
        void addReference(Node *node);
        void removeReference(Node *node);
        
        void setTransform(const unsigned long index, const btTransform &transform);
        btTransform getTransform(const unsigned long index);
        
        void setColorTransform(const unsigned long index, const btTransform &transform);
        btTransform getColorTransform(const unsigned long index);
        
        void setNormalMatrixTransform(const unsigned long index, const btTransform &transform);
        btTransform getNormalMatrixTransform(const unsigned long index);
        
        void setOpacity(Node *node);
        void setHidden(Node *node);
        void setColorBase(Node *node);
        
        unsigned long numberOfVertices()const;
        unsigned long numberOfIndices()const;
        unsigned long maxNumberOfObjects()const;
    private:
        
        
        GLfloat *m_ModelViewTransformData;
        GLfloat *m_ColorTransformData;
        GLfloat *m_NormalMatrixTransformData;
        
//        Sprite *m_VertexData;
        Cube *m_VertexData;
        GLushort *m_IndiceData;
        
        GLuint m_VertexArray;
        GLuint m_ModelViewBuffer;
        GLuint m_ColorTransformBuffer;
        GLuint m_NormalMatrixTransformBuffer;
        GLuint m_VerticesBuffer;
        GLuint m_IndexBuffer;
        
        std::bitset<MAX_CUBES> m_References;
        
        Shader *m_Shader;
        
        bool m_OpacityModifyRGB;
        
        GLfloat *m_MatrixBuffer;
        
    };
}

#endif /* CubeGeometry_hpp */
