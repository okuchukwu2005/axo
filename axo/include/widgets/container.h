#ifndef CONTAINER_H
#define CONTAINER_H

#include "../core/parent.h"
#include "../core/backend_interface.h"


#include<SDL2/SDL.h>
#include<stdbool.h>

axParent axCreateContainer(axParent* root, int x, int y, int w, int h);
void axSetContainerProperties(axParent* container, bool moveable, const char* title, bool has_title_bar, bool closeable/*,bool resizeable*/);

void draw_title_bar_(axParent* container);
void axRenderContainer(axParent* container);

void axUpdateContainer(axParent* container, axEvent* event);
// registering stuffs

#define MAX_CONTAINERS 100

extern axParent* container_widgets[MAX_CONTAINERS];
extern int containers_count;

void axRegisterContainer(axParent* container);

void axRenderAllRegisteredContainers(void);
void axUpdateAllRegisteredContainers(axEvent* event);

void axFreeContainer(axParent* parent);

void axFreeAllRegisteredContainers(void);

#endif /* CONTAINER_H */
