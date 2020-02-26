//
//  Camera.hpp
//  VerizonTest
//
//  Created by James Folk on 6/21/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include "btTransform.h"
#include "glm/glm.hpp"

namespace njli
{
    class Node;
    class Shader;
    
    class Camera
    {
        friend class Geometry;
        
    public:
        /* static */
        static glm::mat4x4 makeFrustum(float *buffer, float fov, float aspect, float nearDist, float farDist, bool leftHanded = true );
        static glm::mat4x4 makeLookAt(float *buffer,
                                      float eyeX, float eyeY, float eyeZ,
                                      float centerX, float centerY, float centerZ,
                                      float upX, float upY, float upZ);
        
        /* members */
        Camera();
        Camera(const Camera &rhs);
        const Camera &operator=(const Camera &rhs);
        ~Camera();
        
        void setZNear(const float val);
        float getZNear() const;
        
        void setZFar(const float val);
        float getZFar() const;
        
        void setFov(const float val);
        float getFov() const;
        
        void setAspectRatio(const float val);
        float getAspectRatio()const;
        
//        btTransform getModelView()const;
        glm::mat4x4 getModelView()const;
        
//        btTransform getProjectionMatrix()const;
        glm::mat4x4 getProjectionMatrix()const;
        
        Node *const getNodeOwner()const;
        void setNodeOwner(Node *const node);
        
        void lookAt(const btVector3& pos, const btVector3& up = btVector3(0, 1.0f, 0));
    protected:
        void render(Shader *const shader, bool shouldRedraw = false);
    private:
        /* static */
        float *m_MatrixBuffer;
        GLfloat *m_ProjectionMatrixBuffer;
        GLfloat *m_ModelViewMatrixBuffer;
        
        /* members */
        Node *m_NodeOwner;
        float m_Near;
        float m_Far;
        float m_Fov;
        float m_AspectRatio;
//        btTransform *m_ProjectionMatrix;
        glm::mat4x4 *mProjectionMatrix;
        bool m_ModelViewDirty;
        bool m_ProjectionDirty;
    };
}

#endif /* Camera_hpp */
