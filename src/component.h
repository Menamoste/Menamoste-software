#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>

// Base component structure for reusable UI elements
typedef struct UIComponent UIComponent;

// Function pointers for component behavior
typedef void (*ComponentRender)(UIComponent* self, SDL_Renderer* renderer);
typedef void (*ComponentDestroy)(UIComponent* self);

struct UIComponent {
    SDL_Rect bounds;           // Position and size (x, y, w, h)
    int visible;               // Whether to render this component

    ComponentRender render;    // Rendering function
    ComponentDestroy destroy;  // Cleanup function

    void* user_data;          // Optional data specific to component type
};

// Component lifecycle
UIComponent* component_create(int x, int y, int w, int h);
void component_destroy(UIComponent* component);
void component_render(UIComponent* component, SDL_Renderer* renderer);
void component_set_position(UIComponent* component, int x, int y);
void component_set_size(UIComponent* component, int w, int h);

#endif
