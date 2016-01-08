/*
   Copyright (C) 2016 by Per Parker <wisalam@live.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/

#ifndef DISPLAY_RENDERER_INCLUDED
#define DISPLAY_RENDERER_INCLUDED

class surface;

#include "SDL.h"
#include <string>

class display_renderer {
public:

	struct text_params {
		const std::string &text;
		int size;
		int style;
		const SDL_Color &color;
	};

	virtual ~display_renderer() {}

	/**
	 * @brief Causes the display renderer to finish all drawing operations and display the results.
	 *
	 * Most rendering operations can be queued or performed on a temporary target. This method is
	 * required in order to flush any pending operations and to display the results on the rendering
	 * target.
	 */
	virtual void commit() = 0;

	/**
	 * @brief Render a given \ref surface at the specified location on the rendering target.
	 *
	 * This method allows for an arbitrary \ref surface to be drawn on the rendering target.
	 * @param[in] src The surface to be rendered.
	 * @param[in] src_rect The area of the surface which will be rendered, or null for the entire surface.
	 * @param[in] dst_rect The area of the rendering target which will be rendered on.
	 */
	virtual void render_surface(surface &src, SDL_Rect *src_rect, SDL_Rect *dst_rect) = 0;

	/**
	 * Render a rectangle on the rendering target.
	 * @param[in] dst_rect The area of the rendering target which will be filled with the given color.
	 * @param[in] color The color of the rectangle which will be rendered.
	 */
	virtual void fill_rect(SDL_Rect *dst_rect, const SDL_Color &color) = 0;

	/**
	 * Clear the rendering target using the given color.
	 * @param[in] color The color to which the rendering target will be set.
	 */
	virtual void clear(const SDL_Color &color) = 0;

	/**
	 * Render some text on the rendering target.
	 * @param[in] text_info Provides information on the text to be rendered.
	 * @param[in] area The area of the rendering target where the text will be rendered.
	 * @return A rectangle enclosing the actual text rendered, which may be smaller than the requested area.
	 */
	virtual SDL_Rect render_text(text_params &text_info, const SDL_Rect &area) = 0;
};

#endif
