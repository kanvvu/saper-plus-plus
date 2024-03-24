#pragma once

#include "game.hpp"
#include <string>
#include "SDL_ttf.h"


class Text{
public:
	Text(const std::string &font_path, int font_size, const char *message_text, const SDL_Color &color,SDL_Renderer *ren);

	void display(int x,int y, SDL_Renderer *ren);
	void setMessage(const char *message_text, const SDL_Color &color);
	void setFont(const std::string &font_path, int font_size);
	SDL_Rect getTextRect(){ return _text_rect; }

	//static SDL_Texture *loadFont(const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color, SDL_Renderer *ren);
private:
	SDL_Texture *_text_texture = nullptr;
	SDL_Rect _text_rect;
	TTF_Font *font;
	SDL_Renderer *renderer;
};