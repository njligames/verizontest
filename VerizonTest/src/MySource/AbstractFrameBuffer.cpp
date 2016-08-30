//
//  AbstractFrameBuffer.cpp
//  VerizonTest
//
//  Created by James Folk on 8/16/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#include "AbstractFrameBuffer.hpp"
#include <string>
#include <assert.h>

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>




static GLboolean QueryExtension(const std::string &extName)
{
    /*
     ** Search for extName in the extensions string. Use of strstr()
     ** is not sufficient because extension names can be prefixes of
     ** other extension names. Could use strtok() but the constant
     ** string returned by glGetString might be in read-only memory.
     */
    char *p;
    char *end;
    unsigned long extNameLen;
    
    extNameLen = strlen(extName.c_str());
               
    const char *the_extensions = (const char *) glGetString(GL_EXTENSIONS);

    
    p = (char *)glGetString(GL_EXTENSIONS);
    if (NULL == p) {
        return GL_FALSE;
    }
    
    end = p + strlen(p);
    
    while (p < end) {
        unsigned long n = strcspn(p, " ");
        if ((extNameLen == n) && (strncmp(extName.c_str(), p, n) == 0)) {
            return GL_TRUE;
        }
        p += (n + 1);
    }
    return GL_FALSE;
}
namespace njli
{
AbstractFrameBuffer::AbstractFrameBuffer():
m_AbstractFrameBufferID(0),
m_RenderBufferColorID(0),
m_RenderBufferDepthID(0),
m_Width(0),
m_Height(0),
m_UseDepth(GL_FALSE),
m_UseStencil(GL_FALSE),
m_IsCubemap(GL_FALSE)
{
    
}

AbstractFrameBuffer::~AbstractFrameBuffer()
{
    unLoad();
}

bool AbstractFrameBuffer::load(unsigned int width, unsigned int height, bool useStencil)
{
    GLuint framebuffer = 0;
    GLuint renderbufferColor = 0;
    GLuint renderbufferDepth = 0;
    
    if(render_to_buffer(width, height, &framebuffer, &renderbufferColor, &renderbufferDepth))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = renderbufferColor;
        m_RenderBufferDepthID = renderbufferDepth;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_FALSE;
        m_UseStencil = GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadColorTexture2D(MaterialProperty *colorMaterial,
                                     unsigned int width,
                                     unsigned int height,
                                     bool generateMipmaps)
{
    GLuint texture;
    GLuint framebuffer;
    GLuint renderbuffer;
    
    if(render_to_texture(width, height, &texture, &framebuffer, &renderbuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = renderbuffer;
        m_RenderBufferDepthID = 0;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_FALSE;
        m_UseStencil = GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadColorTextureCube(MaterialProperty *colorMaterial,
                                       unsigned int width,
                                       unsigned int height,
                                       bool generateMipmaps)
{
    GLuint texture = 0;
    GLuint framebuffer = 0;
    GLuint renderbuffer = 0;
    
    if(render_to_cubemap(width, height, &texture, &framebuffer, &renderbuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = renderbuffer;
        m_RenderBufferDepthID = 0;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_FALSE;
        m_UseStencil = GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadDepthTexture2D(MaterialProperty *depthMaterial,
                                     unsigned int width,
                                     unsigned int height,
                                     bool generateMipmaps,
                                     bool useStencil)
{
    GLuint texture = 0;
    GLuint framebuffer = 0;
    GLuint renderbuffer = 0;
    
    if((useStencil)?
       render_to_texture_depth_stencil(width, height, &texture, &framebuffer, &renderbuffer, generateMipmaps) :
       render_to_texture_depth_only(width, height, &texture, &framebuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = 0;
        m_RenderBufferDepthID = 0;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_TRUE;
        m_UseStencil = (useStencil)?GL_TRUE:GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadDepthTextureCube(MaterialProperty *depthMaterial,
                                       unsigned int width,
                                       unsigned int height,
                                       bool generateMipmaps,
                                       bool useStencil)
{
    GLuint texture = 0;
    GLuint framebuffer = 0;
    GLuint renderbuffer = 0;
    
    if((useStencil)?
       render_to_cubemap_depth_stencil(width, height, &texture, &framebuffer, &renderbuffer, generateMipmaps) :
       render_to_cubemap_depth_only(width, height, &texture, &framebuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = 0;
        m_RenderBufferDepthID = 0;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_TRUE;
        m_UseStencil = (useStencil)?GL_TRUE:GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadTexture2D(MaterialProperty *colorMaterial,
                                MaterialProperty *depthMaterial,
                                unsigned int width,
                                unsigned int height,
                                bool generateMipmaps,
                                bool useStencil)
{
    const int COLOR_TEXTURE = 0;
    const int DEPTH_TEXTURE = 1;
    GLuint textures[2] = {0, 0};
    GLuint framebuffer = 0;
    GLuint renderbufferColor = 0;
    GLuint renderbufferDepth = 0;
    
    if(render_to_depth_texture(width, height, textures, &framebuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = renderbufferColor;
        m_RenderBufferDepthID = renderbufferDepth;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_TRUE;
        m_UseStencil = (useStencil)?GL_TRUE:GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

bool AbstractFrameBuffer::loadTextureCube(MaterialProperty *colorMaterial,
                                  MaterialProperty *depthMaterial,
                                  unsigned int width,
                                  unsigned int height,
                                  bool generateMipmaps,
                                  bool useStencil)
{
    const int COLOR_TEXTURE = 0;
    const int DEPTH_TEXTURE = 1;
    GLuint textures[2] = {0, 0};
    GLuint framebuffer = 0;
    GLuint renderbufferColor = 0;
    GLuint renderbufferDepth = 0;
    
    if(render_to_depth_cubemap(width, height, textures, &framebuffer, generateMipmaps))
    {
        unLoad();
        
        m_AbstractFrameBufferID = framebuffer;
        m_RenderBufferColorID = renderbufferColor;
        m_RenderBufferDepthID = renderbufferDepth;
        m_Width = width;
        m_Height = height;
        m_UseDepth = GL_TRUE;
        m_UseStencil = (useStencil)?GL_TRUE:GL_FALSE;
        m_IsCubemap = GL_FALSE;
        
        return true;
    }
    return false;
}

//bool AbstractFrameBuffer::load(unsigned int width, unsigned int height, bool mipmaps, bool useStencil)
//{
//    unLoad();
//
//    GLint maxRenderbufferSize;
//    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
//    
//    // check if GL_MAX_RENDERBUFFER_SIZE is >= width and height
//    if((maxRenderbufferSize <= width) ||
//       maxRenderbufferSize <= height)
//    {
//        // cannot use framebuffer objects as we need to create a
//        // depth buffer as a renderbuffer object
//        // return with appropriate error
//        
//        return false;
//    }
//    
//    if(useStencil && !QueryExtension(std::string("GL_EXT_packed_depth_stencil")))
//       return false;
//    
//    //RGBA8 2D texture, D24S8 depth/stencil texture, 256x256
//    glGenTextures(1, &m_ColorTextureID);
//    glBindTexture(GL_TEXTURE_2D, m_ColorTextureID);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    //NULL means reserve texture memory, but texels are undefined
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//    
//    //You must reserve memory for other mipmaps levels as well either by making a series of calls to
//    //glTexImage2D or use glGenerateMipmap(GL_TEXTURE_2D).
//    //Here, we'll use :
//    if(mipmaps)
//        glGenerateMipmap(GL_TEXTURE_2D);
//    //-------------------------
//    glGenFramebuffers(1, &m_AbstractFrameBufferID);
//    glBindFramebuffer(GL_FRAMEBUFFER, m_AbstractFrameBufferID);
//    //Attach 2D texture to this FBO
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTextureID, 0);
//    
//    //-------------------------
//    glGenRenderbuffers(1, &m_BufferID);
//    glBindRenderbuffer(GL_RENDERBUFFER, m_BufferID);
//    
//    {
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
//        if(useStencil)
//            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
//        else
//            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, width, height);
//    }
//    
//    //-------------------------
//    //Attach depth buffer to FBO
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_BufferID);
//    //Also attach as a stencil
//    if(useStencil)
//        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_BufferID);
//    //-------------------------
//    //Does the GPU support current FBO configuration?
//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//        return false;
//    
//    m_UseStencil = useStencil;
//    m_Width = width;
//    m_Height = height;
//    
//    return true;
//}

void AbstractFrameBuffer::unLoad()
{
    //Delete resources
    if(m_RenderBufferColorID != 0)
        glDeleteRenderbuffers(1, &m_RenderBufferColorID);
    m_RenderBufferColorID = 0;
    
    if(m_RenderBufferDepthID != 0)
        glDeleteRenderbuffers(1, &m_RenderBufferDepthID);
    m_RenderBufferDepthID = 0;
    
    //Bind 0, which means render to back buffer, as a result, m_AbstractFrameBufferID is unbound
    if(m_AbstractFrameBufferID != 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_AbstractFrameBufferID);
    }
    m_AbstractFrameBufferID = 0;
}

bool AbstractFrameBuffer::isLoaded()const
{
    return (m_AbstractFrameBufferID != 0);
}

void AbstractFrameBuffer::render(Shader *shader)
{
    //and now you can render to GL_TEXTURE_2D
    glBindFramebuffer(GL_FRAMEBUFFER, m_AbstractFrameBufferID);
    glClearColor(0.0, 0.0, 0.0, 0.0);
//
//    GLboolean initialDepth = glIsEnabled(GL_DEPTH_TEST);
//    GLboolean initialStencil = glIsEnabled(GL_STENCIL_TEST);
//    
//    //It's always a good idea to clear the stencil at the same time as the depth when the format is D24S8.
    if(m_UseStencil)
    {
        assert(m_UseDepth);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    }
    else if(m_UseDepth)
    {
        glEnable(GL_DEPTH_TEST);
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    
    //-------------------------
    
    if(m_IsCubemap)
    {
        for (unsigned long i = 0; i < 6; i++)
        {
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, *texture, 0);
            assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            
            renderToBuffer(shader);
        }
    }
    else
    {
        
        renderToBuffer(shader);
    }
    
    
    //cleanup...
//    if(glIsEnabled(GL_DEPTH_TEST) != initialDepth)
//    {
//        if(initialDepth)
//            glEnable(GL_DEPTH_TEST);
//        else
//            glDisable(GL_DEPTH_TEST);
//    }
//    
//    if(glIsEnabled(GL_STENCIL_TEST) != initialStencil)
//    {
//        if(initialStencil)
//            glEnable(GL_STENCIL_TEST);
//        else
//            glDisable(GL_STENCIL_TEST);
//    }
    
//    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    
    // render to the window system provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}











bool AbstractFrameBuffer::is_valid_dimension(unsigned int width, unsigned int height)
{
    if(QueryExtension("GL_EXT_framebuffer_object"))
    {
        return true;
    }
    GLint maxRenderbufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);assert(glGetError() == GL_NO_ERROR);
    
    // check if GL_MAX_RENDERBUFFER_SIZE is >= width and height
    if((maxRenderbufferSize <= width) ||
       maxRenderbufferSize <= height)
    {
        // cannot use framebuffer objects as we need to create a
        // depth buffer as a renderbuffer object
        // return with appropriate error
        
        return false;
    }
    return true;
}

bool AbstractFrameBuffer::render_to_texture(unsigned int width, unsigned int height,
                                    GLuint *_texture,
                                    GLuint *_framebuffer,
                                    GLuint *_renderbuffer,
                                    GLboolean generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        GLuint tex;
        GLuint fb;
        GLuint rb;
        
        //RGBA8 2D texture, 24 bit depth texture, 256x256
        glGenTextures(1, &tex);assert(glGetError() == GL_NO_ERROR);
        glBindTexture(GL_TEXTURE_2D, tex);assert(glGetError() == GL_NO_ERROR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);assert(glGetError() == GL_NO_ERROR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);assert(glGetError() == GL_NO_ERROR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);assert(glGetError() == GL_NO_ERROR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);assert(glGetError() == GL_NO_ERROR);
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);assert(glGetError() == GL_NO_ERROR);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);assert(glGetError() == GL_NO_ERROR);
        
        //-------------------------
        glGenFramebuffers(1, &fb);assert(glGetError() == GL_NO_ERROR);
        glBindFramebuffer(GL_FRAMEBUFFER, fb);assert(glGetError() == GL_NO_ERROR);
        //Attach 2D texture to this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);assert(glGetError() == GL_NO_ERROR);
        //-------------------------
        glGenRenderbuffers(1, &rb);assert(glGetError() == GL_NO_ERROR);
        glBindRenderbuffer(GL_RENDERBUFFER, rb);assert(glGetError() == GL_NO_ERROR);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, width, height);assert(glGetError() == GL_NO_ERROR);
        
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);assert(glGetError() == GL_NO_ERROR);
        //-------------------------
        //Does the GPU support current FBO configuration?
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);assert(glGetError() == GL_NO_ERROR);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                return true;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                break;
            default:
                return true;
        }
    }
    return false;
    
    
    //-------------------------
    //and now you can render to GL_TEXTURE_2D
//    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glClearDepthf(1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //-------------------------
//    glViewport(0, 0, 256, 256);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0.0, 256.0, 0.0, 256.0, -1.0, 1.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
    //-------------------------
//    glDisable(GL_TEXTURE_2D);
//    glDisable(GL_BLEND);
//    glEnable(GL_DEPTH_TEST);
    //-------------------------
    //**************************
    //RenderATriangle, {0.0, 0.0}, {256.0, 0.0}, {256.0, 256.0}
    //Read http://www.opengl.org/wiki/VBO_-_just_examples
//    RenderATriangle();
    //-------------------------
//    GLubyte pixels[4*4*4];
//    glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    //pixels 0, 1, 2 should be white
    //pixel 4 should be black
    //----------------
    //Bind 0, which means render to back buffer
//    glBindFramebuffer(GL_FRAM EBUFFER, 0);
}

bool AbstractFrameBuffer::render_to_cubemap(unsigned int width, unsigned int height,
                                    GLuint *texture,
                                    GLuint *framebuffer,
                                    GLuint *renderbuffer,
                                    GLboolean generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        //RGBA8 Cubemap texture, 24 bit depth texture, 256x256
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        //-------------------------
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Attach one of the faces of the Cubemap texture to this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, *texture, 0);
        //-------------------------
        glGenRenderbuffers(1, renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, width, height);
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
        //-------------------------
        //Does the GPU support current FBO configuration?
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
    
    
    
    
    //-------------------------
    //and now you can render to GL_TEXTURE_CUBE_MAP_POSITIVE_X
    //In order to render to the other faces, do this :
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, *texture, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, *texture, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, *texture, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, *texture, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, *texture, 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, *texture, 0);
    //... now render
}

bool AbstractFrameBuffer::render_to_texture_depth_only(unsigned int width, unsigned int height,
                                               GLuint *texture,
                                               GLuint *framebuffer,
                                               GLboolean generateMipmaps)
{
//    if(is_valid_dimension(width, height))
    {
        //32 bit depth texture, 256x256
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
        
        //-------------------------
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Attach
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *texture, 0);
        //-------------------------
        //Does the GPU support current FBO configuration?
        //Before checking the configuration, you should call these 2 according to the spec.
        //At the very least, you need to call glDrawBuffer(GL_NONE)
        //    glDrawBuffer(GL_NONE);
        //    glReadBuffer(GL_NONE);
//        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);assert(glGetError() == GL_NO_ERROR);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                return true;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                break;
            default:
                return true;
        }
    }
    return false;
    
    
    
    
//    //-------------------------
//    //----and to render to it, don't forget to call
//    //At the very least, you need to call glDrawBuffer(GL_NONE)
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    //-------------------------
//    //If you want to render to the back buffer again, you must bind 0 AND THEN CALL glDrawBuffer(GL_BACK)
//    //else GL_INVALID_OPERATION will be raised
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glDrawBuffer(GL_BACK);
//    glReadBuffer(GL_BACK);
}

bool AbstractFrameBuffer::render_to_cubemap_depth_only(unsigned int width, unsigned int height,
                                               GLuint *texture,
                                               GLuint *framebuffer,
                                               GLboolean generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        //RGBA8 Cubemap texture, 24 bit depth texture, 256x256
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        //-------------------------
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Attach
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, *texture, 0);
        //-------------------------
        //Does the GPU support current FBO configuration?
        //Before checking the configuration, you should call these 2 according to the spec.
        //At the very least, you need to call glDrawBuffer(GL_NONE)
        //    glDrawBuffer(GL_NONE);
        //    glReadBuffer(GL_NONE);
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
    
}

bool AbstractFrameBuffer::render_to_texture_depth_stencil(unsigned int width, unsigned int height,
                                                  GLuint *texture,
                                                  GLuint *framebuffer,
                                                  GLuint *renderbuffer,
                                                  GLboolean generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        //RGBA8 2D texture, D24S8 depth/stencil texture, 256x256
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_2D, *texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        //You must reserve memory for other mipmaps levels as well either by making a series of calls to
        //glTexImage2D or use glGenerateMipmap(GL_TEXTURE_2D).
        //Here, we'll use :
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
        
        //-------------------------
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Attach 2D texture to this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);
        //-------------------------
        glGenRenderbuffers(1, renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
        //Also attach as a stencil
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
        //-------------------------
        //Does the GPU support current FBO configuration?
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
    
    
    
    
    //    //-------------------------
    //    //and now you can render to GL_TEXTURE_2D
    //    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
    //    glClearColor(0.0, 0.0, 0.0, 0.0);
    //    //It's always a good idea to clear the stencil at the same time as the depth when the format is D24S8.
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);
    //    //-------------------------
    //    glViewport(0, 0, 256, 256);
    //    glMatrixMode(GL_PROJECTION);
    //    glLoadIdentity();
    //    glOrtho(0.0, 256.0, 0.0, 256.0, -1.0, 1.0);
    //    glMatrixMode(GL_MODELVIEW);
    //    glLoadIdentity();
    //    //-------------------------
    //    glDisable(GL_TEXTURE_2D);
    //    glDisable(GL_BLEND);
    //    glEnable(GL_DEPTH_TEST);
    //    //-------------------------
    //    //**************************
    //    //RenderATriangle, {0.0, 0.0}, {256.0, 0.0}, {256.0, 256.0}
    //    //Read http://www.opengl.org/wiki/VBO_-_just_examples
    //    RenderATriangle();
    //    //-------------------------
    //    GLubyte pixels[4*4*4];
    //    glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    //    //pixels 0, 1, 2 should be white
    //    //pixel 4 should be black
    //    //----------------
    //    //Bind 0, which means render to back buffer
    //    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool AbstractFrameBuffer::render_to_cubemap_depth_stencil(unsigned int width, unsigned int height,
                                                  GLuint *texture,
                                                  GLuint *framebuffer,
                                                  GLuint *renderbuffer,
                                                  GLboolean generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        //RGBA8 Cubemap texture, 24 bit depth texture, 256x256
        glGenTextures(1, texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        //-------------------------
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Attach 2D texture to this FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);
        //-------------------------
        glGenRenderbuffers(1, renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, width, height);
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
        //Also attach as a stencil
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer);
        //-------------------------
        //Does the GPU support current FBO configuration?
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
    
}

bool AbstractFrameBuffer::render_to_buffer(unsigned int width, unsigned int height,
                                   GLuint *framebuffer,
                                   GLuint *renderbuffer_color,
                                   GLuint *renderbuffer_depth)
{
    if(is_valid_dimension(width, height))
    {
        //RGBA8 RenderBuffer, 24 bit depth RenderBuffer, 256x256
        glGenFramebuffers(1, framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        //Create and attach a color buffer
        glGenRenderbuffers(1, renderbuffer_color);
        //We must bind renderbuffer_color before we call glRenderbufferStorage
        glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer_color);
        //The storage format is RGBA8
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
        //Attach color buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, *renderbuffer_color);
        //-------------------------
        glGenRenderbuffers(1, renderbuffer_depth);
        glBindRenderbuffer(GL_RENDERBUFFER, *renderbuffer_depth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24_OES, width, height);
        //-------------------------
        //Attach depth buffer to FBO
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *renderbuffer_depth);
        //-------------------------
        //Does the GPU support current FBO configuration?
        
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        switch (status) {
            case GL_FRAMEBUFFER_COMPLETE:
                return true;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                break;
            default:
                break;
        }
    }
    return false;
    
    
    
    
    
    
    //-------------------------
//    //and now you can render to the FBO (also called RenderBuffer)
//    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
//    glClearColor(0.0, 0.0, 0.0, 0.0);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    //-------------------------
////    glViewport(0, 0, 256, 256);
////    glMatrixMode(GL_PROJECTION);
////    glLoadIdentity();
////    glOrtho(0.0, 256.0, 0.0, 256.0, -1.0, 1.0);
////    glMatrixMode(GL_MODELVIEW);
////    glLoadIdentity();
//    //-------------------------
//    glDisable(GL_TEXTURE_2D);
//    glDisable(GL_BLEND);
//    glEnable(GL_DEPTH_TEST);
//    //-------------------------
//    //**************************
//    //RenderATriangle, {0.0, 0.0}, {256.0, 0.0}, {256.0, 256.0}
//    //Read http://www.opengl.org/wiki/VBO_-_just_examples
//    RenderATriangle();
//    //-------------------------
//    GLubyte pixels[4*4*4];
//    glReadPixels(0, 0, 4, 4, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
//    //pixels 0, 1, 2 should be white
//    //pixel 4 should be black
//    //----------------
//    //Bind 0, which means render to back buffer
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool AbstractFrameBuffer::render_to_depth_texture(unsigned int width, unsigned int height,
                                          GLuint *textures,
                                          GLuint *framebuffer,
                                          bool generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        const int COLOR_TEXTURE = 0;
        const int DEPTH_TEXTURE = 1;
        
        glGenTextures(2, textures);
        glGenFramebuffers(1, framebuffer);
        
        glBindTexture(GL_TEXTURE_2D, textures[COLOR_TEXTURE]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
        
        glBindTexture(GL_TEXTURE_2D, textures[DEPTH_TEXTURE]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_2D);
        
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[COLOR_TEXTURE], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[DEPTH_TEXTURE], 0);
        
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
}

bool AbstractFrameBuffer::render_to_depth_cubemap(unsigned int width, unsigned int height,
                                          GLuint *textures,
                                          GLuint *framebuffer,
                                          bool generateMipmaps)
{
    if(is_valid_dimension(width, height))
    {
        const int COLOR_TEXTURE = 0;
        const int DEPTH_TEXTURE = 1;
        
        glGenTextures(2, textures);
        glGenFramebuffers(1, framebuffer);
        
        //RGBA8 Cubemap texture, 24 bit depth texture, 256x256
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures[COLOR_TEXTURE]);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        //RGBA8 Cubemap texture, 24 bit depth texture, 256x256
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures[DEPTH_TEXTURE]);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        //NULL means reserve texture memory, but texels are undefined
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+0, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        if(generateMipmaps)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[COLOR_TEXTURE], 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[DEPTH_TEXTURE], 0);
        
        return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }
    return false;
}
}
