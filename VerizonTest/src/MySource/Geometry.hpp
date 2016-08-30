//
//  Geometry.hpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

//#include <bitset>
#include <vector>
#include <string>

#include "btTransform.h"
#include "btVector2.h"

//#define USE_HALF_FLOAT


typedef unsigned short hfloat;

#ifdef USE_HALF_FLOAT
typedef hfloat GLfptype;
#define GL_INDEX_TYPE (GL_HALF_FLOAT_OES)

#else
typedef GLfloat GLfptype;
#define GL_INDEX_TYPE (GL_FLOAT)
#endif

namespace njli
{
    ATTRIBUTE_ALIGNED16(struct)
    TexturedColoredVertex
    {
        static void computeTangentBasis(TexturedColoredVertex *v, unsigned int numVerts)
        {
            
            for (unsigned int i=0; i<numVerts; i+=3 )
            {
                // Shortcuts for vertices
                btVector3  v0 = (v + (i + 0))->vertex;
                btVector3  v1 = (v + (i + 1))->vertex;
                btVector3  v2 = (v + (i + 2))->vertex;
                
                // Shortcuts for UVs
                btVector2  uv0 = (v + (i + 0))->texture;
                btVector2  uv1 = (v + (i + 1))->texture;
                btVector2  uv2 = (v + (i + 2))->texture;
                
                // Edges of the triangle : postion delta
                btVector3 deltaPos1 = v1-v0;
                btVector3 deltaPos2 = v2-v0;
                
                // UV delta
                btVector2 deltaUV1 = uv1-uv0;
                btVector2 deltaUV2 = uv2-uv0;
                
                float d = (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
                
                float r = (d!=0.0f)?(1.0f / d):0.0f;
                btVector3 tangent = (deltaPos1 * deltaUV2.y()   - deltaPos2 * deltaUV1.y())*r;
                btVector3 bitangent = (deltaPos2 * deltaUV1.x()   - deltaPos1 * deltaUV2.x())*r;
                
                // Set the same tangent for all three vertices of the triangle.
                // They will be merged later, in vboindexer.cpp
                (v + (i + 0))->tangent = tangent;
                (v + (i + 1))->tangent = tangent;
                (v + (i + 2))->tangent = tangent;
                
                // Same thing for binormals
                (v + (i + 0))->bitangent = bitangent;
                (v + (i + 1))->bitangent = bitangent;
                (v + (i + 2))->bitangent = bitangent;
                
            }
            
            // See "Going Further"
            for (unsigned int i=0; i<numVerts; i+=1 )
            {
                btVector3 & n = (v + (i))->normal;
                btVector3 & t = (v + (i))->tangent;
                btVector3 & b = (v + (i))->bitangent;
                
                
                // Gram-Schmidt orthogonalize
                t = (t - n * n.dot(t));
                
                if(t.length() > 0.0f)
                    t.normalize();
                
                // Calculate handedness
                if (n.cross(t).dot(b) < 0.0f)
                {
                    t = t * -1.0f;
                }
            }
        }
        
        TexturedColoredVertex()
        : vertex(0, 0, 0)
        , color(1, 1, 1, 1)
        , texture(0, 0)
        , normal(1,1,1)
        , tangent(0, 0, 0)
        , bitangent(0, 0, 0)
//        , opacity(1.0f)
//        , hidden(0.0f)
        {
        }
        TexturedColoredVertex(const btVector3 vertex,
                              const btVector4 color,
                              const btVector2 texture,
                              const btVector3 normal,
                              const btVector3 tangent,
                              const btVector3 bitangent
//                              const GLfptype opacity,
//                              const GLfptype hidden
                              )
        : vertex(vertex)
        , color(color)
        , texture(texture)
        , normal(normal)
        , tangent(tangent)
        , bitangent(bitangent)
//        , opacity(opacity)
//        , hidden(hidden)
        {
        }
        btVector3 vertex;
        btVector4 color;
        btVector2 texture;
        btVector3 normal;
        btVector3 tangent;
        btVector3 bitangent;
        
//        GLfptype opacity;
//        GLfptype hidden;
        
        TexturedColoredVertex& operator=(const TexturedColoredVertex& rhs)
        {
            if (this != &rhs) {
                vertex = rhs.vertex;
                color = rhs.color;
                texture = rhs.texture;
                normal = rhs.normal;
                tangent = rhs.tangent;
                bitangent = rhs.bitangent;
                
//                opacity = rhs.opacity;
//                hidden = rhs.hidden;
            }
            return *this;
        }
        
        operator std::string() const
        {
            char buffer[4098];
            
//            sprintf(buffer, "{{%f, %f, %f}, {%f, %f}, {%f, %f, %f, %f}, {%f, %f, %f}, %f, %f}",
//            sprintf(buffer, "{{%f, %f, %f}, {%f, %f}, {%f, %f, %f}, %f, %f}",
            sprintf(buffer, "{{%f, %f, %f}, {%f, %f}, {%f, %f, %f}}",
                    vertex.x(), vertex.y(), vertex.z(),
                    texture.x(), texture.y(),
//                    color.x(), color.y(), color.z(), color.w(),
                    normal.x(), normal.y(), normal.z()//,
//                    opacity,
//                    hidden
                    );
            
            return std::string(buffer);
        }
        
    };
    
    class Shader;
    class Camera;
    class Node;
    
    class Geometry
    {
        friend class Node;
        friend class Scene;
        
    public:
        enum MeshType
        {
            MeshType_Obj
        };
        
        /* members */
        Geometry();
        Geometry(const Geometry &rhs);
        const Geometry &operator=(const Geometry &rhs);
        virtual ~Geometry();
        
        virtual void load(Shader *shader, const std::string &filecontent="", unsigned int numInstances = 1, MeshType type = MeshType_Obj);
        void unLoad();
        bool isLoaded()const;
        
        Shader *const getShader();
        
        void setShader(Shader *const shader);
        
        void render(Camera *camera);
        
//        static const GLsizei MAX_CUBES = 10000;
        
    protected:
        const void *getModelViewTransformArrayBufferPtr()const;
        GLsizeiptr getModelViewTransformArrayBufferSize()const;
        bool isModelViewBufferChanged()const;
        void enableModelViewBufferChanged(bool changed = true);
        
//        const void *getColorTransformArrayBufferPtr()const;
//        GLsizeiptr getColorTransformArrayBufferSize()const;
        
        const void *getNormalMatrixTransformArrayBufferPtr()const;
        GLsizeiptr getNormalMatrixTransformArrayBufferSize()const;
        bool isNormalMatrixBufferChanged()const;
        void enableNormalMatrixBufferChanged(bool changed = true);
        
        virtual void loadData();
        virtual void unLoadData();
        
        virtual const void *getVertexArrayBufferPtr()const = 0;
        virtual GLsizeiptr getVertexArrayBufferSize()const = 0;
        bool isVertexArrayBufferChanged()const;
        void enableVertexArrayBufferChanged(bool changed = true);
        
        virtual const void *getElementArrayBufferPtr()const = 0;
        virtual GLsizeiptr getElementArrayBufferSize()const = 0;
        
        virtual GLenum getElementIndexType()const = 0;
        
        void addReference(Node *node);
        void removeReference(Node *node);
        
        void setTransform(const unsigned long index, const btTransform &transform);
        btTransform getTransform(const unsigned long index);
        
//        void setColorTransform(const unsigned long index, const btTransform &transform);
//        btTransform getColorTransform(const unsigned long index);
        
        void setNormalMatrixTransform(const unsigned long index, const btTransform &transform);
        btTransform getNormalMatrixTransform(const unsigned long index);
        
        virtual void setOpacity(Node *node) = 0;
        virtual void setHidden(Node *node) = 0;
        virtual void setColorBase(Node *node) = 0;
        
        virtual GLsizei numberOfVertices()const = 0;
        virtual GLsizei numberOfIndices()const = 0;
        virtual GLsizei numberOfInstances()const;
        
        unsigned long getGeometryIndex(Node *const node)const;
        
//        void computeTangentBasis(
//                                 // inputs
//                                 const std::vector<btVector3> & vertices,
//                                 const std::vector<btVector2> & uvs,
//                                 const std::vector<btVector3> & normals,
//                                 // outputs
//                                 std::vector<btVector3> & tangents,
//                                 std::vector<btVector3> & bitangents
//                                 );

        
        
        GLfptype *m_MatrixBuffer;
        btScalar *m_MatrixBufferFullSize;
        
    private:
        
        GLfptype *m_ModelViewTransformData;
//        GLfptype *m_ColorTransformData;
        GLfptype *m_NormalMatrixTransformData;
        
        GLuint m_VertexArray;
        GLuint m_ModelViewBuffer;
//        GLuint m_ColorTransformBuffer;
        GLuint m_NormalMatrixTransformBuffer;
        GLuint m_VerticesBuffer;
        GLuint m_IndexBuffer;
        
//        std::bitset<MAX_CUBES> m_References;
        std::vector<bool> m_References;
        unsigned int m_NumberInstances;
        
        Shader *m_Shader;
        
        bool m_OpacityModifyRGB;
        bool m_VertexBufferChanged;
        bool m_NormalMatrixBufferChanged;
        bool m_ModelViewBufferChanged;
        bool m_ShaderChanged;
    };
}

#endif /* Geometry_hpp */
