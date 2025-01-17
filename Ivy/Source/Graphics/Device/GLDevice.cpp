/*
The MIT License (MIT)

Copyright (c) 2015 Ben Brown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "GLDevice.h"

#ifdef IVY_PLATFORM_WINDOWS
bool Ivy::Graphics::GLDevice::Create()
{   
    std::cout << "[Device Context]" << std::endl;

    // Apparently the class has to be titled "STATIC" for this to be a valid window handle.
    HWND dummyWND = CreateWindowA("STATIC", "", WS_DISABLED, 0, 0, 0, 0, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    HDC dummyDC = GetDC(dummyWND);

    PIXELFORMATDESCRIPTOR dummyPixelFormatDescriptor =
    {
        sizeof dummyPixelFormatDescriptor, 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA, static_cast<BYTE>(specification.colorBits), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        24, 8, 0, PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    int dummyPixelFormat = ChoosePixelFormat(dummyDC, &dummyPixelFormatDescriptor);
    
    if (!SetPixelFormat(dummyDC, dummyPixelFormat, &dummyPixelFormatDescriptor))
        return false;

    // Create dummy context
    HGLRC dummyGLRC = wglCreateContext(dummyDC);
    wglMakeCurrent(dummyDC, dummyGLRC);

    // Initialize OpenGL entry points for dummy context.
    if (!gladLoadGL())
        return false;

    // Highest supported OpenGL version.
    // Used to generate a context.
    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    // Anything below 3.2 is unsupported.
    if (major < 3 && minor < 2)
        return false;

    // Required extensions.
#if defined(WGL_ARB_pixel_format) && defined(WGL_ARB_create_context) \
    && defined(WGL_ARB_multisample) && defined(WGL_EXT_swap_control)

    // Initialize WGL extensions.
    if (!gladLoadWGL(dc))
        return false;

    // Choose final pixel format
    const int pixelAttribs[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, specification.colorBits,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_SAMPLE_BUFFERS_ARB, (specification.samples > 1) ? GL_TRUE : GL_FALSE,
        WGL_SAMPLES_ARB, (specification.samples > 1) ? specification.samples : 0,
        0
    };

    // Get the device context for the window.
    dc = window.GetPlatformDisplay();

    int pixelFormat;
    unsigned int formatCount;
    wglChoosePixelFormatARB(dc, pixelAttribs, nullptr, 1, &pixelFormat, &formatCount);

    if (!formatCount)
        return false;

    SetPixelFormat(dc, pixelFormat, &dummyPixelFormatDescriptor);

    int contextAttribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 2,
        0, 0
    };
    
    // Create the OpenGL context		
    context = wglCreateContextAttribsARB(dc, nullptr, contextAttribs);

    // Failed to be created.
    if (!context)
        return false;

    // Unbind dummy context and delete.
    wglMakeCurrent(dc, nullptr);
    wglDeleteContext(dummyGLRC);
    ReleaseDC(dummyWND, dummyDC);

    // Bind main context and destroy dummy window.
    wglMakeCurrent(dc, context);
    DestroyWindow(dummyWND);

    // Initialize OpenGL pointers for new context.
    // While the methods would still "seemingly"
    // function correctly, this could lead to 
    // unusual and unintended behavior.
    if (!gladLoadGL())
        return false;

    // MakeActive Refresh rate.
    wglSwapIntervalEXT(specification.vsync);

#else
    // Unsupported extensions.
    return false;
#endif // Extensions 

    std::cout << "Renderer: OpenGL " << glGetString(GL_VERSION) << std::endl;
    return true;
}
#endif

bool Ivy::Graphics::GLDevice::Release()
{
#ifdef IVY_PLATFORM_WINDOWS
    // Unbind OpenGL context.
    wglMakeCurrent(dc, nullptr);
    wglDeleteContext(context);
    ReleaseDC(window.GetPlatformWindow(), dc);
#elif IVY_PLATFORM_UNIX

#endif
    return context == nullptr ? true : false;
}

void Ivy::Graphics::GLDevice::AdjustViewport()
{
    glViewport(0, 0, window.GetWidth(), window.GetHeight());
}

void Ivy::Graphics::GLDevice::Clear(Math::Color color)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
}

void Ivy::Graphics::GLDevice::Present()
{
#ifdef IVY_PLATFORM_WINDOWS
    SwapBuffers(dc);
#elif IVY_PLATFORM_LINUX

#endif
}

char* Ivy::Graphics::GLDevice::DeviceInformation()
{
    return nullptr;
}

Ivy::Graphics::DrawType Ivy::Graphics::GLDevice::GetDrawType()
{
    return drawType;
}

void Ivy::Graphics::GLDevice::SetDrawType(DrawType type)
{
    drawType = type;
}

void Ivy::Graphics::GLDevice::TakeScreenshot()
{

}
