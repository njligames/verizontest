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

#include <bitset>

#include "btTransform.h"


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
        struct vec2
        {
            vec2(){}
            vec2(GLfptype x, GLfptype y)
            {
                v[0]=x;
                v[1] = y;
                v[2] = 0;
                v[3] = 0;
            }
            const vec2 &operator=(const vec2 &rhs)
            {
                if(this != &rhs)
                {
                    v[0] = rhs.v[0];
                    v[1] = rhs.v[1];
                    v[2] = rhs.v[2];
                    v[3] = rhs.v[3];
                }
                return *this;
            }
            GLfptype x()const{return v[0];}
            GLfptype y()const{return v[1];}
            void setX(GLfptype f){v[0] = f;}
            void setY(GLfptype f){v[1] = f;}
            GLfptype v[4];
        };
        struct vec3
        {
            vec3(){}
            vec3(GLfptype x, GLfptype y, GLfptype z)
            {
                v[0]=x;
                v[1] = y;
                v[2] = z;
                v[3] = 0;
            }
            const vec3 &operator=(const vec3 &rhs)
            {
                if(this != &rhs)
                {
                    v[0] = rhs.v[0];
                    v[1] = rhs.v[1];
                    v[2] = rhs.v[2];
                    v[3] = rhs.v[3];
                }
                return *this;
            }
            GLfptype x()const{return v[0];}
            GLfptype y()const{return v[1];}
            GLfptype z()const{return v[2];}
            void setX(GLfptype f){v[0] = f;}
            void setY(GLfptype f){v[1] = f;}
            void setZ(GLfptype f){v[2] = f;}
            GLfptype v[4];
        };
        struct vec4
        {
            vec4(){}
            vec4(GLfptype x, GLfptype y, GLfptype z, GLfptype w)
            {
                v[0]=x;
                v[1] = y;
                v[2] = z;
                v[3] = w;
            }
            const vec4 &operator=(const vec4 &rhs)
            {
                if(this != &rhs)
                {
                    v[0] = rhs.v[0];
                    v[1] = rhs.v[1];
                    v[2] = rhs.v[2];
                    v[3] = rhs.v[3];
                }
                return *this;
            }
            GLfptype x()const{return v[0];}
            GLfptype y()const{return v[1];}
            GLfptype z()const{return v[2];}
            GLfptype w()const{return v[3];}
            void setX(GLfptype f){v[0] = f;}
            void setY(GLfptype f){v[1] = f;}
            void setZ(GLfptype f){v[2] = f;}
            void setW(GLfptype f){v[3] = f;}
            GLfptype v[4];
        };
        TexturedColoredVertex()
        : vertex(0, 0, 0)
        , color(1, 1, 1, 1)
//        , texture(0, 0)
        , normal(1,1,1)
        , opacity(1.0f)
        , hidden(0.0f)
        {
        }
        TexturedColoredVertex(const vec3 vertex,
                              const vec4 color,
//                              const vec2 texture,
                              const vec3 normal,
                              const GLfptype opacity,
                              const GLfptype hidden)
        : vertex(vertex)
        , color(color)
//        , texture(texture)
        , normal(normal)
        , opacity(opacity)
        , hidden(hidden)
        {
        }
        vec3 vertex;
        vec4 color;
//        vec2 texture;
        vec3 normal;
        GLfptype opacity;
        GLfptype hidden;
        
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
        
        operator std::string() const
        {
            char buffer[2048];
//            sprintf(buffer, "{{%f, %f, %f}, {%f, %f}, {%f, %f, %f, %f}}", vertex.x(), vertex.y(), vertex.z(), texture.x(), texture.y(), color.x(), color.y(), color.z(), color.w());
            sprintf(buffer, "{{%f, %f, %f}, {%f, %f, %f, %f}, {%f, %f, %f}, %f, %f}",
                    vertex.x(), vertex.y(), vertex.z(),
                    color.x(), color.y(), color.z(), color.w(),
                    normal.x(), normal.y(), normal.z(),
                    opacity,
                    hidden);
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
        
        virtual void load(Shader *shader, const std::string &filecontent="", MeshType type = MeshType_Obj);
        void unLoad();
        bool isLoaded()const;
        
        Shader *const getShader();
        
        void setShader(Shader *const shader);
        
        void render(Camera *camera);
        
        static const GLsizei MAX_CUBES = 10000;
        
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
        virtual GLsizei maxNumberOfObjects()const;
        
        unsigned long getGeometryIndex(Node *const node)const;
        
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
        
        std::bitset<MAX_CUBES> m_References;
        
        Shader *m_Shader;
        
        bool m_OpacityModifyRGB;
        bool m_VertexBufferChanged;
        bool m_NormalMatrixBufferChanged;
        bool m_ModelViewBufferChanged;
        bool m_ShaderChanged;
        
        
    };
}

#endif /* Geometry_hpp */
