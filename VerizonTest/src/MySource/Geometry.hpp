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
//#include "glm::vec2.h"

#include "glm/glm.hpp"

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
                glm::vec3  v0 = (v + (i + 0))->vertex;
                glm::vec3  v1 = (v + (i + 1))->vertex;
                glm::vec3  v2 = (v + (i + 2))->vertex;
                
                // Shortcuts for UVs
                glm::vec2  uv0 = (v + (i + 0))->texture;
                glm::vec2  uv1 = (v + (i + 1))->texture;
                glm::vec2  uv2 = (v + (i + 2))->texture;
                
                // Edges of the triangle : postion delta
                glm::vec3 deltaPos1 = v1-v0;
                glm::vec3 deltaPos2 = v2-v0;
                
                // UV delta
                glm::vec2 deltaUV1 = uv1-uv0;
                glm::vec2 deltaUV2 = uv2-uv0;
                
                float d = (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
                
                float r = (d!=0.0f)?(1.0f / d):0.0f;
                glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
                glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
                
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
                glm::vec3 & n = (v + (i))->normal;
                glm::vec3 & t = (v + (i))->tangent;
                glm::vec3 & b = (v + (i))->bitangent;
                
                
                // Gram-Schmidt orthogonalize
//                t = (t - n * n.dot(t));
                t = (t - n * glm::dot(n,t));
                
                
                float l = t.length();
                if(l > 0.0f) {
//                    t.normalize();
                    t = glm::normalize(t);
                }
                
                // Calculate handedness
//                if (n.cross(t).dot(b) < 0.0f)
//                {
//                    t = t * -1.0f;
//                }
                glm::vec3 _cross = glm::cross(t, n);
                float _dot = glm::dot(_cross, b);
                
                if(_dot < 0.0f) {
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
        TexturedColoredVertex(const glm::vec3 vertex,
                              const glm::vec4 color,
                              const glm::vec2 texture,
                              const glm::vec3 normal,
                              const glm::vec3 tangent,
                              const glm::vec3 bitangent
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
        glm::vec3 vertex;
        glm::vec4 color;
        glm::vec2 texture;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        
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
                    vertex.x, vertex.y, vertex.z,
                    texture.x, texture.y,
//                    color.x(), color.y(), color.z(), color.w(),
                    normal.x, normal.y, normal.z//,
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
        
//        void setTransform(const unsigned long index, const btTransform &transform);
        void setTransform(const unsigned long index, const glm::mat4x4 &transform);
        
//        btTransform getTransform(const unsigned long index);
        glm::mat4x4 getTransform(const unsigned long index);
        
//        void setColorTransform(const unsigned long index, const btTransform &transform);
//        btTransform getColorTransform(const unsigned long index);
        
//        void setNormalMatrixTransform(const unsigned long index, const btTransform &transform);
        void setNormalMatrixTransform(const unsigned long index, const glm::mat4x4 &transform);
        
//        btTransform getNormalMatrixTransform(const unsigned long index);
        glm::mat4x4 getNormalMatrixTransform(const unsigned long index);
        
        virtual void setOpacity(Node *node) = 0;
        virtual void setHidden(Node *node) = 0;
        virtual void setColorBase(Node *node) = 0;
        
        virtual GLsizei numberOfVertices()const = 0;
        virtual GLsizei numberOfIndices()const = 0;
        virtual GLsizei numberOfInstances()const;
        
        unsigned long getGeometryIndex(Node *const node)const;
        
//        void computeTangentBasis(
//                                 // inputs
//                                 const std::vector<glm::vec3> & vertices,
//                                 const std::vector<glm::vec2> & uvs,
//                                 const std::vector<glm::vec3> & normals,
//                                 // outputs
//                                 std::vector<glm::vec3> & tangents,
//                                 std::vector<glm::vec3> & bitangents
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
