#ifndef RTT_H
#define RTT_H

#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"

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

   // FBO and its buffers handle 
   GLuint    framebuffer;
   GLuint    depthRenderbuffer;
   GLuint    texture;

} UserData;

#endif
