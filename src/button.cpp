#include "button.hpp"
#include "TextureManager.hpp"
#include "cursor.hpp"

Button::Button(const char* texture,const char* textureHover,SDL_Renderer* ren,int x,int y,int w,int h)
{
	textureButton = TextureManager::LoadTexture(texture,ren);
	textureHoverButton = TextureManager::LoadTexture(textureHover,ren);
	rectButton.w = w;
	rectButton.h = h;
	rectButton.x = x;
	rectButton.y = y;
}

void Button::update(Cursor cursor){
	collision = cursor.collision(&rectButton);
}

void Button::draw(SDL_Renderer* ren)
{
	if (collision)
		SDL_RenderCopy(ren,textureHoverButton,NULL,&rectButton);
	else
		SDL_RenderCopy(ren,textureButton,NULL,&rectButton);

}