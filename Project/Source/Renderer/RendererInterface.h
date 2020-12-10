/*
-------------------------------------------------------------------------------------------------------
	File: RendererInterface.h
	Author: Ozzie Mercado
	Created: December 9, 2020
	Modified: December 10, 2020
	Description: The interface that all renderer implementations share. Interface for creating the
		renderer and drawing to the screen.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#include "../Window/Window.h"

namespace OC
{
	class RendererInterface
	{
	private:
		// Description: Resize the the renderer.
		virtual void Resize() = 0;

	public:
		// Description: Constructs the renderer system and sets it up to output to the window.
		// Parameters: 
		//    const Window& _window, the window to render to.
		RendererInterface(const Window& _window) {};

		// Description: Renderer's cannot be created from other renderer's.
		RendererInterface(const RendererInterface& _renderer) = delete;

		// Description: Remove the renderer from the window and clean up this instance.
		virtual ~RendererInterface() = default;

		// Description: Renderer's cannot be assigned to other renderer's.
		virtual void operator=(const RendererInterface& _renderer) = delete;

		// Description: Renders to the window.
		virtual void Present() = 0;
	};
}
