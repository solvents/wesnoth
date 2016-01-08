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

#ifndef SRC_SDL_SDL_DISPLAY_RENDERER_HPP_
#define SRC_SDL_SDL_DISPLAY_RENDERER_HPP_

#include "display_renderer.hpp"

namespace sdl {

class sdl_display_renderer: public display_renderer {
public:
	sdl_display_renderer(SDL_Renderer *renderer);
	virtual ~sdl_display_renderer();

	void render_surface(surface &src, SDL_Rect *src_rect, SDL_Rect *dst_rect);
	void fill_rect(SDL_Rect *dst_rect, const SDL_Color &color);
	void clear(const SDL_Color &color);
	void commit();
	SDL_Rect render_text(text_params &text_info, const SDL_Rect &area);

private:
	SDL_Renderer *renderer_;
	bool hint_software_renderer_;
};

} /* namespace sdl */

#endif /* SRC_SDL_SDL_DISPLAY_RENDERER_HPP_ */
