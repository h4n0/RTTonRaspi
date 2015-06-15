#include "triangle_draw.h"

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
   glBindAttribLocation ( userData->programObjectRed, 0, "vposition" );

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
   glBindAttribLocation ( userData->programObjectBlue, 0, "vposition" );

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
   return TRUE;
}

///
// Create Framebuffer object and a texture as its color buffer attachment
//
void InitFBO( ESContext *esContext, RenderFBO *renderFBO, GLuint programObj)
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
    glGenFramebuffers(1, &renderFBO->framebuffer);
    glGenRenderbuffers(1, &renderFBO->depthRenderbuffer);
    glGenTextures(1, &renderFBO->texture);

    // bind texture and load the texture mip-level 0
    // texels are RGB565
    // no texels need to be specified as we are going to draw into
    // the texture
    glBindTexture(GL_TEXTURE_2D, renderFBO->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,
            0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // bind renderbuffer and create a 16-bit depth buffer
    // width and height of renderbuffer = width and height of
    // the texture
    glBindRenderbuffer(GL_RENDERBUFFER, renderFBO->depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, texWidth, texHeight);

    // bind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, renderFBO->framebuffer);
    // specify texture as color attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderFBO->texture, 0);

    // specify depth_renderbufer as depth attachment
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderFBO->depthRenderbuffer);

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
   glUseProgram ( programObj );

   // Load the vertex position
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );

   glEnableVertexAttribArray ( 0 );

   glDrawArrays ( GL_TRIANGLES, 0, 3 );

}
