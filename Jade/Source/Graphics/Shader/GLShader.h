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
#include "Graphics/Device/GLDevice.h"
#include "Graphics/Shader/IShader.h"
#include "Graphics/Shader/ShaderType.h"

namespace Jade
{
	namespace Graphics
	{
		class GLShader : public IShader
		{
			std::map<std::string, ShaderType> shaders;
			std::vector<GLuint> shaderIDs;
			GLuint programID;

			bool Create(std::string filename, ShaderType type) override;
			bool Compile(std::string filename, ShaderType type) override;
			bool Release() override;
			bool Check(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

		public:

			GLShader(std::map<std::string, ShaderType> shaders)
			{
				this->shaders = shaders;
				
				for(auto iterator = shaders.begin(); iterator != shaders.end(); ++iterator)
				{
					std::string iString = iterator->first;
					ShaderType iType = iterator->second;

					GLShader::Create(iString, iType);
				}
			}

			void MakeActive() override;
			bool CreateProgram();
			GLuint GetProgramID();

			~GLShader()
			{
				GLShader::Release();
			}
		};
	}
}
