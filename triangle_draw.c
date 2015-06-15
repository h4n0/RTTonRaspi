#include triangle_draw.h

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
