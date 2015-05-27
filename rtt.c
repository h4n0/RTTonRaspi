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
   GLint samplerLoc;

   // FBO and its buffers handle 
   GLuint    framebuffer;
   GLuint    depthRenderbuffer;
   GLuint    texture;

} UserData;

int InitRedTriangle ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =  
      "attribute vec4 vposition;   \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vposition; \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);          \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObjectRed = esLoadProgram ( vShaderStr, fShaderStr );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( userData->programObject, 0, "vposition" );

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return TRUE;
}

int InitBlueTriangle ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   GLbyte vShaderStr[] =  
      "attribute vec4 vposition;   \n"
      "void main()                  \n"
      "{                            \n"
      "   gl_Position = vposition; \n"
      "}                            \n";
   
   GLbyte fShaderStr[] =  
      "precision mediump float;                            \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);          \n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObjectBlue = esLoadProgram ( vShaderStr, fShaderStr );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( userData->programObject, 0, "vposition" );

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return TRUE;
}

int Init ( ESContext *esContext )
{
    return InitRedTriangle ( esContext ) && InitBlueTriangle ( esContext );
}
 
///
// Draw the rendered texture back to screen
//
void RenderToScreen ( ESContext *esContext)
{
   UserData *userData = esContext->userData;
   GLfloat vVertices[] = { -0.5f, -0.5f, 0.0f,  // Position 0
                            0.0f,  0.0f,        // TexCoord 0 
                            0.5f, -0.5f, 0.0f,  // Position 1
                            0.0f,  1.0f,        // TexCoord 1
                            0.5f,  0.5f, 0.0f,  // Position 2
                            1.0f,  1.0f,        // TexCoord 2
                           -0.5f,  0.5f, 0.0f,  // Position 3
                            1.0f,  0.0f         // TexCoord 3
                         };
   GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

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
      "uniform sampler2D s_texture;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObjectScr = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObjectScr, "a_position" );
   userData->texCoordLoc = glGetAttribLocation ( userData->programObjectScr, "a_texCoord" );
   
   // Get the sampler location
   userData->samplerLoc = glGetUniformLocation ( userData->programObjectScr, "s_texture" );

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObjectScr );

   // Load the vertex position
   glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT, 
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
   // Load the texture coordinate
   glVertexAttribPointer ( userData->texCoordLoc, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

   glEnableVertexAttribArray ( userData->positionLoc );
   glEnableVertexAttribArray ( userData->texCoordLoc );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->texture );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->samplerLoc, 0 );

   glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
   // Change back to window system provided framebuffer
   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   RenderToScreen(esContext);
}
///
// Create Framebuffer object and a texture as its color buffer attachment
//
void InitFBO( ESContext *esContext)
{
    GLint     texWidth = 256, texHeight = 256;
    GLint     maxRenderbufferSize;
    UserData *userData = esContext->userData;

    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    
    // check if GL_MAX_RENDERBUFFER_SIZE is >= texWidth and texHeight
    if((maxRenderbufferSize <= texWidth) || (maxRenderbufferSize <= texHeight))
    {
        // cannot use framebuffer objects as we need to create
        // a depth buffer as a renderbuffer object
        printf("Cannot use framebuffer objects!\n");
        exit(EXIT_FAILURE);
    }
    // generate the framebuffer, renderbuffer, and texture object names
    glGenFramebuffers(1, &userData->framebuffer);
    glGenRenderbuffers(1, &userData->depthRenderbuffer);
    glGenTextures(1, &userData->texture);

    // bind texture and load the texture mip-level 0
    // texels are RGB565
    // no texels need to be specified as we are going to draw into
    // the texture
    glBindTexture(GL_TEXTURE_2D, userData->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,
            0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // bind renderbuffer and create a 16-bit depth buffer
    // width and height of renderbuffer = width and height of
    // the texture
    glBindRenderbuffer(GL_RENDERBUFFER, userData->depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, texWidth, texHeight);

    // bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, userData->framebuffer);
    // specify texture as color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, userData->texture, 0);

    // specify depth_renderbufer as depth attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, userData->depthRenderbuffer);

    // check for framebuffer complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Framebuffer object is not complete!\n");
        exit(EXIT_FAILURE);
    }

    // Draw a triangle to FBO
   GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
                           -0.5f, -0.5f, 0.0f,
                            0.5f, -0.5f, 0.0f };

   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear ( GL_COLOR_BUFFER_BIT );

   // Use the program object
   glUseProgram ( userData->programObject );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );

   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLES, 0, 3 );

}

///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->texture );

   // Delete program object
   glDeleteProgram ( userData->programObject );
   glDeleteProgram ( userData->programObjectScr );

   // Delete renderbuffer and framebuffer objects
   glDeleteRenderbuffers(1, &userData->depthRenderbuffer);
   glDeleteFramebuffers(1, &userData->framebuffer);
}

int main() 
{
   ESContext esContext;
   UserData  userData;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   InitFBO( &esContext);

   esRegisterDrawFunc ( &esContext, Draw );

   esMainLoop ( &esContext );

   return 0;
}
