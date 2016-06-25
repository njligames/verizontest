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

namespace njli
{
    class Node;
    
    class Camera
    {
    public:
        /* static */
        static btTransform makeFrustum(float *buffer, float fov, float aspect, float nearDist, float farDist, bool leftHanded = true );
        static btTransform makeLookAt(float *buffer,
                                      btScalar eyeX, btScalar eyeY, btScalar eyeZ,
                                      btScalar centerX, btScalar centerY, btScalar centerZ,
                                      btScalar upX, btScalar upY, btScalar upZ);
        
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
        
        btTransform getModelView()const;
        btTransform getProjectionMatrix()const;
        GLfloat *const getProjectionMatrixPtr()const;
        
        Node *const getNodeOwner()const;
        void setNodeOwner(Node *node);
        
        void lookAt(const btVector3& pos, const btVector3& up = btVector3(0, 1.0f, 0));
    protected:
        
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
        btTransform *m_ProjectionMatrix;
    };
}

#endif /* Camera_hpp */
