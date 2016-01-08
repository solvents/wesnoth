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

#include "sdl_display_renderer.hpp"

#include "utils.hpp"
#include "sdl/exception.hpp"
#include "font.hpp"
#include "marked-up_text.hpp"
#include "serialization/string_utils.hpp"
#include "tooltips.hpp"

#include <cassert>

namespace sdl {

sdl_display_renderer::sdl_display_renderer(SDL_Renderer *renderer):
		renderer_(renderer),
		hint_software_renderer_(true)
{
	assert(renderer_);

	SDL_RendererInfo info;
	if (SDL_GetRendererInfo(renderer, &info) == 0) {
		hint_software_renderer_ = info.flags & SDL_RENDERER_SOFTWARE;
	}
}

sdl_display_renderer::~sdl_display_renderer()
{
	SDL_DestroyRenderer(renderer_);
}

void sdl_display_renderer::render_surface(surface &src, SDL_Rect *src_rect, SDL_Rect *dst_rect)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer_, src);
	/*if (SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND) ||
	    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND)) {
		throw texception("Failed to set SDL blending mode.", true);
	}*/
	// TODO Figure out why SDL_ALPHA_OPAQUE doesn't work on the software renderer
	if (hint_software_renderer_)
		SDL_SetTextureAlphaMod(tex, 254);
	sdl_render(tex, src_rect, renderer_, dst_rect);
	SDL_DestroyTexture(tex);
}

void sdl_display_renderer::fill_rect(SDL_Rect *dst_rect, const SDL_Color &color)
{
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer_, dst_rect);
}

void sdl_display_renderer::clear(const SDL_Color &color)
{
	// TODO Document or remove exception
	SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
	if(SDL_RenderClear(renderer_) != 0) {
		throw texception("Failed to clear the SDL_Renderer object.",
						 true);
	}
}

void sdl_display_renderer::commit()
{
	SDL_RenderPresent(renderer_);
}

SDL_Rect sdl_display_renderer::render_text(text_params &text_info, const SDL_Rect &area)
{
	// TODO this code is duplicated from marked-up_text, need to remove from marked-up_text
	// Make sure there's always at least a space,
	// so we can ensure that we can return a rectangle for height
	static const std::string blank_text(" ");
	const std::string& text = text_info.text.empty() ? blank_text : text_info.text;

	SDL_Rect res;
	res.x = area.x;
	res.y = area.y;
	res.w = 0;
	res.h = 0;

	int x = res.x;
	int y = res.y;

	std::string::const_iterator i1 = text.begin();
	std::string::const_iterator i2 = std::find(i1,text.end(),'\n');
	for(;;) {
		SDL_Color col = text_info.color;
		int sz = text_info.size;
		int text_style = text_info.style;

		i1 = font::parse_markup(i1,i2,&sz,&col,&text_style);

		if(i1 != i2) {
			std::string new_string = utils::unescape(std::string(i1, i2));
			std::string etext = font::make_text_ellipsis(new_string, sz, area.w);

			surface line_surface = font::get_rendered_text(etext, sz, col, text_style);

			SDL_Rect dest;
			if (x != -1) {
				dest.x = x;
#ifdef HAVE_FRIDIBI
				// Oron -- Conditional, until all draw_text_line calls have fixed area parameter
				if(getenv("NO_RTL") == NULL) {
					bool is_rtl = text_cache::find(text_surface(text, size, color, style)).is_rtl();
					if(is_rtl)
						dest.x = area.x + area.w - line_surface->w - (x - area.x);
				}
#endif
			}
			else {
				dest.x = (area.w/2) - (line_surface->w/2);
			}
			if (y != -1) {
				dest.y = y;
			}
			else {
				dest.y = (area.h/2) - (line_surface->h/2);
			}
			dest.w = line_surface->w;
			dest.h = line_surface->h;

			if(font::line_width(new_string, sz) > area.w) {
				tooltips::add_tooltip(dest,new_string);
			}

			if(dest.x + dest.w > area.x + area.w) {
				dest.w = area.x + area.w - dest.x;
			}

			if(dest.y + dest.h > area.y + area.h) {
				dest.h = area.y + area.h - dest.y;
			}
			if (line_surface != NULL) {
				SDL_Rect src = dest;
				src.x = 0;
				src.y = 0;
				render_surface(line_surface, &src, &dest);
			}

			if(dest.w > res.w) {
				res.w = dest.w;
			}

			res.h += dest.h;
			y += dest.h;
		}

		if(i2 == text.end()) {
			break;
		}

		i1 = i2+1;
		i2 = std::find(i1,text.end(),'\n');
	}

	return res;
}

} /* namespace sdl */
