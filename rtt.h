#ifndef RTT_H
#define RTT_H

#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
   // FBO and its buffers handle 
   GLuint    framebuffer;
   GLuint    depthbuffer;
   GLuint    texture;
} RenderFBO;

typedef struct
{
   // Handle to a render-to-texture program object
   GLuint programObjectRed;
   GLuint programObjectBlue;
   // Handle to a texture-render-to-screen program object
   GLuint programObjectScr;

   // Attribute locations
   GLint  positionLoc;
   GLint  texCoordLoc;

   // Sampler location
   GLint redSamplerLoc;
   GLint blueSamplerLoc;

   // FBOs for client side rendering
   RenderFBO redFBO;
   RenderFBO blueFBO;

} UserData;

#endif
