#ifndef TRIANGLE_DRAW_H
#define TRIANGLE_DRAW_H

#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"
#include "rtt.h"

int InitRedTriangle ( ESContext *esContext );
int InitBlueTriangle ( ESContext *esContext );
void InitFBO( ESContext *esContext, RenderFBO *renderFBO, GLuint programObj);

#endif
