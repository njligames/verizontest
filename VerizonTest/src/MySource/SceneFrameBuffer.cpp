//
//  SceneFrameBuffer.cpp
//  VerizonTest
//
//  Created by James Folk on 8/18/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "SceneFrameBuffer.hpp"
#include "Scene.h"
#include "Shader.h"

namespace njli
{
    SceneFrameBuffer::SceneFrameBuffer():
    m_Scene(0)
    {
        
    }
    
    SceneFrameBuffer::~SceneFrameBuffer()
    {
        
    }
    
    void SceneFrameBuffer::setScene(Scene *scene)
    {
        m_Scene = scene;
    }
    
    void SceneFrameBuffer::renderToBuffer(Shader *shader)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        m_Scene->render();
    }
}
