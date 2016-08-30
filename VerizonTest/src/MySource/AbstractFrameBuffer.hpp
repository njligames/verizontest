//
//  AbstractFrameBuffer.hpp
//  VerizonTest
//
//  Created by James Folk on 8/16/16.
//  Copyright © 2016 NJLIGames Ltd. All rights reserved.
//

#ifndef AbstractFrameBuffer_hpp
#define AbstractFrameBuffer_hpp

#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES2/gl.h>

class MaterialProperty;

#include "Shader.hpp"

//https://www.opengl.org/wiki/Framebuffer_Object_Examples
//http://gamedev.stackexchange.com/questions/19461/opengl-glsl-render-to-cube-map
namespace njli
{
class AbstractFrameBuffer
{
public:
    AbstractFrameBuffer();
    virtual ~AbstractFrameBuffer();
    
    bool load(unsigned int width,
              unsigned int height,
              bool useStencil = false);
    
    bool loadColorTexture2D(MaterialProperty *colorMaterial,
                             unsigned int width,
                             unsigned int height,
                             bool generateMipmaps = false);
    
    bool loadColorTextureCube(MaterialProperty *colorMaterial,
                              unsigned int width,
                              unsigned int height,
                              bool generateMipmaps = false);
    
    bool loadDepthTexture2D(MaterialProperty *depthMaterial,
                            unsigned int width,
                            unsigned int height,
                            bool generateMipmaps = false,
                            bool useStencil = false);
    
    bool loadDepthTextureCube(MaterialProperty *depthMaterial,
                              unsigned int width,
                              unsigned int height,
                              bool generateMipmaps = false,
                              bool useStencil = false);
    
    bool loadTexture2D(MaterialProperty *colorMaterial,
                       MaterialProperty *depthMaterial,
                       unsigned int width,
                       unsigned int height,
                       bool generateMipmaps = false,
                       bool useStencil = false);
    
    bool loadTextureCube(MaterialProperty *colorMaterial,
                         MaterialProperty *depthMaterial,
                         unsigned int width,
                         unsigned int height,
                         bool generateMipmaps = false,
                         bool useStencil = false);
    
    void unLoad();
    bool isLoaded()const;
    
    void render(Shader *shader = NULL);
    
protected:
    //set_fbo_texture_shader_and_uniforms(), then draw to the framebuffer object...
    virtual void renderToBuffer(Shader *shader = NULL) = 0;
private:
    
    bool is_valid_dimension(unsigned int width, unsigned int height);
    
    bool render_to_texture(unsigned int width, unsigned int height,
                           GLuint *texture,
                           GLuint *framebuffer,
                           GLuint *renderbuffer,
                           GLboolean generateMipmaps);
    
    bool render_to_cubemap(unsigned int width, unsigned int height,
                           GLuint *texture,
                           GLuint *framebuffer,
                           GLuint *renderbuffer,
                           GLboolean generateMipmaps);
    
    bool render_to_texture_depth_only(unsigned int width, unsigned int height,
                                      GLuint *texture,
                                      GLuint *framebuffer,
                                      GLboolean generateMipmaps);
    
    bool render_to_cubemap_depth_only(unsigned int width, unsigned int height,
                                      GLuint *texture,
                                      GLuint *framebuffer,
                                      GLboolean generateMipmaps);
    
    bool render_to_texture_depth_stencil(unsigned int width, unsigned int height,
                                         GLuint *texture,
                                         GLuint *framebuffer,
                                         GLuint *renderbuffer,
                                         GLboolean generateMipmaps);
    
    bool render_to_cubemap_depth_stencil(unsigned int width, unsigned int height,
                                         GLuint *texture,
                                         GLuint *framebuffer,
                                         GLuint *renderbuffer,
                                         GLboolean generateMipmaps);
    
    bool render_to_buffer(unsigned int width, unsigned int height,
                          GLuint *framebuffer,
                          GLuint *renderbuffer_color,
                          GLuint *renderbuffer_depth);
    
    bool render_to_depth_texture(unsigned int width, unsigned int height,
                                 GLuint *textures,
                                 GLuint *framebuffer,
                                 bool generateMipmaps);
    
    bool render_to_depth_cubemap(unsigned int width, unsigned int height,
                                 GLuint *textures,
                                 GLuint *framebuffer,
                                 bool generateMipmaps);
    
    GLuint m_AbstractFrameBufferID;
    GLuint m_RenderBufferColorID;
    GLuint m_RenderBufferDepthID;
    
    GLsizei m_Width;
    GLsizei m_Height;
    
    GLboolean m_UseDepth;
    GLboolean m_UseStencil;
    GLboolean m_IsCubemap;
};
}
#endif /* AbstractFrameBuffer_hpp */
