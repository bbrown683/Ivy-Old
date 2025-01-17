#pragma once

#include "Core/Include.h"
#include "Graphics/Widget/Widget.h"

namespace Ivy
{
	namespace Graphics
	{
		//! Represents a surface that can contain other sub-widgets such as buttons.
		class Canvas : public Widget
		{
		public:

			Math::Point<float> GetPosition() override;
			void SetPosition(int x, int y) override;
			void Show() override;
			void Hide() override;
			void Draw() override;
		};
	}
}