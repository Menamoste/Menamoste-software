#include "component.h"
#include <stdlib.h>

UIComponent* component_create(int x, int y, int w, int h)
{
    UIComponent* comp = (UIComponent*)malloc(sizeof(UIComponent));
    if (!comp) return NULL;

    comp->bounds.x = x;
    comp->bounds.y = y;
    comp->bounds.w = w;
    comp->bounds.h = h;
    comp->visible = 1;
    comp->render = NULL;
    comp->destroy = NULL;
    comp->user_data = NULL;

    return comp;
}

void component_destroy(UIComponent* component)
{
    if (!component) return;
    if (component->destroy) {
        component->destroy(component);
    }
    free(component);
}

void component_render(UIComponent* component, SDL_Renderer* renderer)
{
    if (!component || !component->visible) return;
    if (component->render) {
        component->render(component, renderer);
    }
}

void component_set_position(UIComponent* component, int x, int y)
{
    if (!component) return;
    component->bounds.x = x;
    component->bounds.y = y;
}

void component_set_size(UIComponent* component, int w, int h)
{
    if (!component) return;
    component->bounds.w = w;
    component->bounds.h = h;
}
