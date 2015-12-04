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

#include "Core/Utility.h"
#include "Math/Color.h"

#include "IDevice.h"
#include "Graphics/Window/IWindow.h"

#include "SDL.h"
#include <Windows.h>

namespace Jade
{
	namespace Graphics
	{
		class GLDevice : public IDevice
		{
		private:

			// Window object contains some data on our window such as size, 
			// and the handle of it in memory which we need to create a device.
			std::shared_ptr<IWindow> window;

			SDL_Window* sdlWindow;
			SDL_GLContext sdlContext;

			bool Create() override;

			bool Release() override;

		public:

			// Empty Device.
			GLDevice() : window(nullptr) { }

			GLDevice(std::shared_ptr<IWindow> window)
			{
				this->window = window;

				// Create our device.
				Create();
			}

			~GLDevice()
			{
				// Cleanup resources.
				Release();
			}

			void Clear(Math::Color color) override;
			void Present() override;
		};
	}
}