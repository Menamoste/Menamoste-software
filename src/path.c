#include "path.h"

void clean_init()
{
    if (TTF_Init() == -1)
    {
        fprintf(stderr, "Couldn't initialize TTF_Init: %s", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void fill_text_box(SDL_Renderer *renderer, SDL_Rect bar)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &bar);
	SDL_RenderPresent(renderer);
}

void print_text(char *text, SDL_Renderer *renderer, TTF_Font *font, 
SDL_Rect bar)
{	
	SDL_Rect text_rect = bar;
    if (text[0] != 0)
    {
		SDL_Color color = {0, 0, 0, 255};
        SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color);
		if (text_surface->w > bar.w)
		{
			text_rect.x = text_surface->w - bar.w;
		}
		SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, 
		text_surface);
		SDL_FreeSurface(text_surface);

		fill_text_box(renderer, bar);

		SDL_QueryTexture(text_texture, NULL, NULL, &text_rect.w, &text_rect.h);
		SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
		SDL_RenderPresent(renderer);
    }
	else
		fill_text_box(renderer, bar);
}

char *text_handeling(SDL_Renderer *renderer, SDL_Rect bar)
{
	//Load the font
	char *font_path = "../res/Fonts/arial.ttf";	
   	TTF_Font *font = TTF_OpenFont(font_path, 50);
	//Limit of the length of the text.
	size_t len_max = 100;
	//Current length of the text.
	size_t len = 0;
	//Boolean to stop the loop.
	char running = 1;
	//To handle double delete issue.
	char backspace_pressed = 1;
	//The text that is being filled.
	char *text = malloc(sizeof(char) * len_max);
	//Event to catch the text and to stop the loop.
	SDL_Event event;
	//Fill the text with null characters.
	memset(text, 0, len_max);
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_RETURN)
				running = 0;	
			else
			{
				//If delete key is pressed.
				if (event.key.keysym.sym == SDLK_BACKSPACE)
				{
					if (backspace_pressed == 1)
					{	
						if (len != 0)
							len--;
						text[len] = '\0';
						print_text(text, renderer, font, bar);
						backspace_pressed = 0;
					}
					else
						backspace_pressed = 1;
				}
				//If another key is pressed.
				else if (event.type == SDL_TEXTINPUT)
				{
					//Length of the added text.
					size_t added_len = strlen(event.text.text);
					//Length to copy to the text.
					size_t len_to_copy = 0;
					//Overflow handeling
					if (added_len + len < len_max)
						len_to_copy = added_len;
					else
						len_to_copy = len_max - len;
					//Copy the text written to the string.
					strncpy(text + len, event.text.text, len_to_copy);
					//Refresh the length.
					len += len_to_copy;

					print_text(text, renderer, font, bar);
				}
			}
		}
	}
	return text;
}

char *text_box(SDL_Renderer *renderer, SDL_Rect bar)
{
	fill_text_box(renderer, bar);
	return text_handeling(renderer, bar);
}
