#ifndef __RENDER_PIPELINE_HPP__
#define __RENDER_PIPELINE_HPP__

#include <project/common.hpp>
#include <project/texture.hpp>
#include <project/timerstack.hpp>
#include <project/resources.hpp>

#include <project/objects/glfullscreenquad.hpp>

class RenderPipeline
{
    const Resources& resources;

    GLuint width;
    GLuint height;

    std::shared_ptr<Texture> gBufferDepthStencil;
    std::shared_ptr<Texture> gBufferDiffuseSpecular;
    std::shared_ptr<Texture> gBufferNormals;
    std::shared_ptr<Texture> gBufferPosition;
    std::shared_ptr<Texture> gBufferLightAccumulation;

    std::shared_ptr<Texture> sceneLuminosity;

    GLuint gBufferFramebuffer;
    GLuint LightFramebuffer;

    GLFullscreenQuad fullscreenQuad;
    GLTimerStack     timerStack;

public:
    RenderPipeline(const Resources& resources);
    ~RenderPipeline();

    void BeginGBufferPass();
    void EndGBufferPass();
    void BeginLightPass();
    void EndLightPass();
    void BeginUnlitPass();
    void EndUnlitPass();

    // Point lights, spot lights, etc.
    void BeginLocalLightMask();
    void EndLocalLightMask();
    void BeginLocalLightRender();
    void EndLocalLightRender();

    // Directional lights
    void BeginGlobalLightRender();
    void EndGlobalLightRender();

    void BeginRendering();
    void EndRendering();

    void PushTimer(const std::string& name);
    void PopTimer();

    void ResizeRenderPipeline(GLuint width, GLuint height);
};

#endif
