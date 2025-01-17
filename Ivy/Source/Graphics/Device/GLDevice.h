#pragma once

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

#include "glad/gl/include/glad/glad.h"

#ifdef IVY_PLATFORM_WINDOWS
// Includes WGL extensions for creating a context.
#include "glad/wgl/include/glad/glad_wgl.h"
#elif IVY_PLATFORM_UNIX

#endif

#include "Core/Exception/Exception.h"
#include "Core/Include.h"
#include "Graphics/Device/IDevice.h"
#include "Graphics/Device/Specification.h"
#include "Math/Color.h"
#include "System/Window/Window.h"

namespace Ivy
{
    namespace Graphics
    {
        class GLDevice : public IDevice
        {
#ifdef IVY_PLATFORM_WINDOWS
            HDC dc;
            HGLRC context;
#endif

            // Window object contains some data on our window such as size, 
            // and the handle of it in memory which we need to create a device.
            System::Window window;
            Specification specification;
            DrawType drawType;

            bool Create() override;
            bool Release() override;

        public:

            GLDevice(System::Window window, Specification specification)
            {
                this->window = window;
                this->specification = specification;

                // Create our device.
                if (!GLDevice::Create())
                {
                    throw Core::DeviceCreationException();
                }
            }

            ~GLDevice()
            {
                // Cleanup resources.
                GLDevice::Release();
            }

            void AdjustViewport() override;
            void Clear(Math::Color color) override;
            void Present() override;
            char* DeviceInformation() override;
            DrawType GetDrawType() override;
            void SetDrawType(DrawType type) override;
            void TakeScreenshot() override;
        };
    }
}
