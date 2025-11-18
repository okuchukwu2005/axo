#ifndef IMAGE_H
#define IMAGE_H

#include "../core/parent.h"
#include "../core/backend_interface.h"



typedef struct{
   axParent*      parent;
    int          x, y, w, h;          /* logical coordinates */
    const char*  file_path;
    ImageHandle* handle;              /* opaque backend handle */
}axImage;

axImage axCreateImage(axParent * parent, int x, int y, const char * file_path, int w, int h  );

void axRenderImage(axImage * image);

void axUpdateImage(axImage *image, axEvent *event);
void axFreeImage(axImage *image);


// Registration of widgets for rendering
#define MAX_IMAGES 100
extern axImage * image_widgets[MAX_IMAGES];

extern int images_count;

void axRegisterImage(axImage* image);

void axRenderAllRegisteredImages(void);

void axUpdateAllRegisteredImages(axEvent* event);

void axFreeAllRegisteredImages(void);

#endif // IMAGE_H
