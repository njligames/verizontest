//
//  SceneFrameBuffer.hpp
//  VerizonTest
//
//  Created by James Folk on 8/18/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef SceneFrameBuffer_hpp
#define SceneFrameBuffer_hpp

#include "AbstractFrameBuffer.hpp"

namespace njli
{
    class Scene;
    
    class SceneFrameBuffer: public AbstractFrameBuffer
    {
    public:
        SceneFrameBuffer();
        virtual ~SceneFrameBuffer();
        
        void setScene(Scene *scene);
    protected:
        virtual void renderToBuffer(Shader *shader = NULL);
    private:
        Scene *m_Scene;
    };
}

#endif /* SceneFrameBuffer_hpp */
