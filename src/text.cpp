#include "text.hpp"

Text::Text(const std::string &font_path, int font_size, const char *message_text, const SDL_Color &color,SDL_Renderer *ren)
{	
	Text::renderer = ren;
	SDL_SetRenderDrawBlendMode(Text::renderer,SDL_BLENDMODE_BLEND);
	setFont(font_path,font_size);
	setMessage(message_text,color);
	//SDL_QueryTexture(_text_texture,nullptr,nullptr, &_text_rect.w,&_text_rect.h);
	TTF_SizeText(font,message_text, &_text_rect.w, &_text_rect.h);
}




void Text::display(int x,int y, SDL_Renderer *ren) {
	_text_rect.x = x;
	_text_rect.y = y;
	SDL_RenderCopy(ren, _text_texture, nullptr, &_text_rect);
}

void Text::setMessage(const char *message_text, const SDL_Color &color)
{
	SDL_Surface *text_surface = TTF_RenderText_Blended(font, message_text, color);
	if(!text_surface){
		std::cout<<"Faild to create text surface"<<std::endl;
	}
	auto text_texture = SDL_CreateTextureFromSurface(renderer,text_surface);
	if(!text_surface){
		std::cout<<"Failed tp create text texture"<<std::endl;
	}
	SDL_FreeSurface(text_surface);
	_text_texture = text_texture;
	TTF_SizeText(font,message_text, &_text_rect.w, &_text_rect.h);
}

void Text::setFont(const std::string &font_path, int font_size){
	font = TTF_OpenFont(font_path.c_str(), font_size);
	if(!font){
		std::cout<<"Failed to load font"<<std::endl;
	}
}

// SDL_Texture *Text::loadFont(const std::string &font_path, int font_size, const std::string &message_text, const SDL_Color &color,SDL_Renderer *ren){
// 	//TTF_Font *font = TTF_OpenFont(font_path.c_str(), font_size);
// 	// if(!font){
// 	// 	std::cout<<"Failed to load font"<<std::endl;
// 	// }

// 	// SDL_Surface *text_surface = TTF_RenderText_Solid(font, message_text.c_str(), color);
// 	// if(!text_surface){
// 	// 	std::cout<<"Faild to create text surface"<<std::endl;
// 	// }
// 	// auto text_texture = SDL_CreateTextureFromSurface(ren,text_surface);
// 	// if(!text_surface){
// 	// 	std::cout<<"Failed tp create text texture"<<std::endl;
// 	// }
// 	// SDL_FreeSurface(text_surface);
// 	// return text_texture;
// }