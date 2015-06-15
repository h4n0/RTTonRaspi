#include "rtt.h"
#include "triangle_draw.h"

///
// Init shaders of drawing triangles 
//
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
      "uniform sampler2D s_triangle_red;                        \n"
      "uniform sampler2D s_triangle_blue;                        \n"
      "uniform sampler2D depth_red;                        \n"
      "uniform sampler2D depth_blue;                        \n"
      "void main()                                         \n"
      "{                                                   \n"
      "  vec4 redTriangle;                                 \n"
      "  vec4 blueTriangle;                                 \n"
      "                                                   \n"
      "  redTriangle = texture2D( s_triangle_red, v_texCoord );\n"
      "  blueTriangle = texture2D( s_triangle_blue, v_texCoord );\n"
      "                                                   \n"
      "//  gl_FragColor = redTriangle + blueTriangle;\n"
      "  if(depth_red.z>depth_blue.z)                  \n"
      "    gl_FragColor = redTriangle;\n"
      "  else                               \n"
      "    gl_FragColor = blueTriangle;\n"
      "}                                                   \n";

   // Load the shaders and get a linked program object
   userData->programObjectScr = esLoadProgram ( vShaderStr, fShaderStr );

   // Get the attribute locations
   userData->positionLoc = glGetAttribLocation ( userData->programObjectScr, "a_position" );
   userData->texCoordLoc = glGetAttribLocation ( userData->programObjectScr, "a_texCoord" );
   
   // Get the sampler location
   userData->redSamplerLoc = glGetUniformLocation ( userData->programObjectScr, "s_triangle_red" );
   userData->blueSamplerLoc = glGetUniformLocation ( userData->programObjectScr, "s_triangle_blue" );

   //FIXME
   GLint redDepth;
   GLint blueDepth;
   redDepth = glGetUniformLocation ( userData->programObjectScr, "depth_red" );
   blueDepth = glGetUniformLocation ( userData->programObjectScr, "depth_blue" );

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

   /// FIXME Here can only with 2 triangles situation work.
   // Bind the texture
   glActiveTexture ( GL_TEXTURE0 );
   glBindTexture ( GL_TEXTURE_2D, userData->redFBO.texture );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->redSamplerLoc, 0 );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE1 );
   glBindTexture ( GL_TEXTURE_2D, userData->blueFBO.texture );

   // Set the sampler texture unit to 0
   glUniform1i ( userData->blueSamplerLoc, 1 );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE2 );
   glBindTexture ( GL_TEXTURE_2D, userData->redFBO.depthbuffer );

   // Set the sampler texture unit to 0
   glUniform1i ( redDepth, 2 );

   // Bind the texture
   glActiveTexture ( GL_TEXTURE3 );
   glBindTexture ( GL_TEXTURE_2D, userData->blueFBO.depthbuffer );

   // Set the sampler texture unit to 0
   glUniform1i ( blueDepth, 3 );
   /// FIXEND

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
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = esContext->userData;

   // Delete texture object
   glDeleteTextures ( 1, &userData->redFBO.texture );
   glDeleteTextures ( 1, &userData->blueFBO.texture );

   // Delete program object
   glDeleteProgram ( userData->programObjectRed );
   glDeleteProgram ( userData->programObjectBlue );
   glDeleteProgram ( userData->programObjectScr );

   // Delete renderbuffer and framebuffer objects
   glDeleteTextures(1, &userData->redFBO.depthbuffer);
   glDeleteFramebuffers(1, &userData->redFBO.framebuffer);
   glDeleteTextures(1, &userData->blueFBO.depthbuffer);
   glDeleteFramebuffers(1, &userData->blueFBO.framebuffer);
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

   InitFBO( &esContext, &userData.redFBO, userData.programObjectRed);
   InitFBO( &esContext, &userData.blueFBO, userData.programObjectBlue);

   esRegisterDrawFunc ( &esContext, Draw );

   esMainLoop ( &esContext );

   return 0;
}
