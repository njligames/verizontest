//
//  Cube.hpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

#include "Geometry.hpp"

namespace njli
{
    class CubeGeometry : public Geometry
    {
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
        
        friend class Node;
        friend class Scene;
        
    public:
        
        /* members */
        CubeGeometry();
        CubeGeometry(const CubeGeometry &rhs);
        const CubeGeometry &operator=(const CubeGeometry &rhs);
        ~CubeGeometry();
        
    protected:
        
        virtual void loadData();
        virtual void unLoadData();
        
        virtual const void *getVertexArrayBufferPtr()const;
        virtual GLsizeiptr getVertexArrayBufferSize()const;
        
        virtual const void *getElementArrayBufferPtr()const;
        virtual GLsizeiptr getElementArrayBufferSize()const;
        
        virtual GLenum getElementIndexType()const;
        
        virtual void setOpacity(Node *node);
        virtual void setHidden(Node *node);
        virtual void setColorBase(Node *node);
        
        virtual GLsizei numberOfVertices()const;
        virtual GLsizei numberOfIndices()const;
        
    private:
        Cube *m_VertexData;
        GLushort *m_IndiceData;
        
    };
}


#endif /* Cube_hpp */
