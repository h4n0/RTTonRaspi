#include <stdio.h>
#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Attribute locations
   GLint  positionLoc;
   GLint  texCoordLoc;

   // Sampler locations
   GLint baseMapLoc;
   GLint lightMapLoc;

   // Texture handle
   GLuint baseMapTexId;
   GLuint lightMapTexId;

} UserData;

int Init ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =  
      "attribute vec4 a_position;   \n"
      "attribute vec2 a_texCoord;   \n"
      "varying vec2 v_texCoord;     \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = a_position; \n"
      "   v_texCoord = a_texCoord;  \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
      "varying vec2 v_texCoord;                            \n"
      "uniform sampler2D s_baseMap;                        \n"
      "uniform sampler2D s_lightMap;                       \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  vec4 baseColor;                                   \n"
      "  vec4 lightColor;                                  \n"
      "                                                    \n"
      "  baseColor = texture2D( s_baseMap, v_texCoord );   \n"
      "  lightColor = texture2D( s_lightMap, v_texCoord ); \n"
      "  gl_FragColor = baseColor * (lightColor + 0.25);   \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObject, "a_position" );
   userData->texCoordLoc = glGetAttribLocation ( userData->programObject, "a_texCoord" );
   
   // Get the sampler location
   userData->baseMapLoc = glGetUniformLocation ( userData->programObject, "s_baseMap" );
   userData->lightMapLoc = glGetUniformLocation ( userData->programObject, "s_lightMap" );

   // Load the textures
   userData->baseMapTexId = LoadTexture ( "basemap.tga" );
   userData->lightMapTexId = LoadTexture ( "lightmap.tga" );

   if ( userData->baseMapTexId == 0 || userData->lightMapTexId == 0 )
      return FALSE;

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = { -0.5f,  0.5f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                           -0.5f, -0.5f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            0.5f, -0.5f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                            0.5f,  0.5f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
      
   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT, 
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( userData->texCoordLoc, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

   glEnableVertexAttribArray ( userData->positionLoc );
   glEnableVertexAttribArray ( userData->texCoordLoc );

   // Bind the base map
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->baseMapTexId );

   // Set the base map sampler to texture unit to 0
   glUniform1i ( userData->baseMapLoc, 0 );

   // Bind the light map
   glActiveTexture ( GL_TEXTURE1 );
   glBindTexture ( GL_TEXTURE_2D, userData->lightMapTexId );
   
   // Set the light map sampler to texture unit 1
   glUniform1i ( userData->lightMapLoc, 1 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
}
///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->baseMapTexId );
   glDeleteTextures ( 1, &userData->lightMapTexId );

   // Delete program object
   glDeleteProgram ( userData->programObject );
}

int main() {
    GLuint    framebuffer;
    GLuint    depthRenderbuffer;
    GLuint    texture;
    GLint     texWidth = 256, texHeight = 256;
    GLint     maxRenderbufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    // check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
    if((maxRenderbufferSize <= texWidth) || (maxRenderbufferSize <= texHeight))
    {
        // cannot use framebuffer objects as we need to create
        // a depth buffer as a renderbuffer object
        return -1; 
    }
    // generate the framebuffer, renderbuffer, and texture object names
    glGenFramebuffers(1, &framebuffer);
    glGenRenderbuffers(1, &depthRenderbuffer);
    glGenTextures(1, &texture);

    // bind texture and load the texture mip-level 0
    // texels are RGB565
    // no texels need to be specified as we are going to draw into
    // the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,
            0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // bind renderbuffer and create a 16-bit depth buffer
    // width and height of renderbuffer = width and height of
    // the texture
}
