//
//  Rectangle.hpp
//  VerizonTest
//
//  Created by James Folk on 6/27/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef Rectangle_hpp
#define Rectangle_hpp

#include "Geometry.hpp"

namespace njli
{
    class RectangleGeometry : public Geometry
    {
        class Rectangle
        {
        public:
            TexturedColoredVertex bl;
            TexturedColoredVertex br;
            TexturedColoredVertex tl;
            TexturedColoredVertex tr;
            
            static const unsigned int NUMBER_OF_VERTICES = 4;
            static const unsigned int NUMBER_OF_INDICES = 6;
            
            Rectangle &operator=(const Rectangle &rhs)
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
        
        friend class Node;
        friend class Scene;
        
    public:
        
        /* members */
        RectangleGeometry();
        RectangleGeometry(const RectangleGeometry &rhs);
        const RectangleGeometry &operator=(const RectangleGeometry &rhs);
        ~RectangleGeometry();
        
    protected:
        
        virtual void loadData();
        virtual void unLoadData();
        
        virtual const void *getVertexArrayBufferPtr()const;
        virtual GLsizeiptr getVertexArrayBufferSize()const;
        
        virtual const void *getElementArrayBufferPtr()const;
        virtual GLsizeiptr getElementArrayBufferSize()const;
        
        virtual void setOpacity(Node *node);
        virtual void setHidden(Node *node);
        virtual void setColorBase(Node *node);
        
        virtual GLsizei numberOfVertices()const;
        virtual GLsizei numberOfIndices()const;
        
    private:
        Rectangle *m_VertexData;
        GLushort *m_IndiceData;
        
    };
}

#endif /* Rectangle_hpp */
