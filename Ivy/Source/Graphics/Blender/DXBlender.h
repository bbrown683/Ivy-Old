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

#include "Graphics/Blender/IBlender.h"
#include "Graphics/Device/DXDevice.h"

namespace Ivy
{
	namespace Graphics
	{
		class DXBlender : public IBlender
		{
			std::shared_ptr<DXDevice> device;
			unsigned int mask;

			ComPtr<ID3D11BlendState> m_pBlendState;

            bool enabled;

		public:

			DXBlender(std::shared_ptr<DXDevice> device)
			{
				this->device = device;
			}

			//! \brief Sets the blend state for the specified mask value in hexadecimal.
			//! Example: 0xffffffff is the mask for enabling alpha blending.
			bool SetBlendState(unsigned int mask) override;

            bool DisableBlendState() override;

            bool Enabled() override;
		};
	}
}
