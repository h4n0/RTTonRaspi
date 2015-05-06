# Straight forward Makefile to compile all examples in a row

# If cross-compiling, you may wish to set the following environment variable to the root location of 
# your 'sdk'
# SDKSTAGE=/home/foo/raspberrypi

INCDIR=-I./Common -I$(SDKSTAGE)/opt/vc/include -I$(SDKSTAGE)/opt/vc/include/interface/vcos/pthreads -I$(SDKSTAGE)/opt/vc/include/interface/vmcs_host/linux

LIBS=-lGLESv2 -lEGL -lm -lbcm_host -L$(SDKSTAGE)/opt/vc/lib

CFLAGS+=-DRPI_NO_X

# CFLAGS+=-DRPI_NO_X -DSTANDALONE -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DTARGET_POSIX -D_LINUX -fPIC -DPIC -D_REENTRANT -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -U_FORTIFY_SOURCE -Wall -g -DHAVE_LIBOPENMAX=2 -DOMX -DOMX_SKIP64BIT -ftree-vectorize -pipe -DUSE_EXTERNAL_OMX -DHAVE_LIBBCM_HOST -DUSE_EXTERNAL_LIBBCM_HOST -DUSE_VCHIQ_ARM -Wno-psabi -I$(SDKSTAGE)/opt/vc/include/ -I./ -I$(SDKSTAGE)/opt/vc/lib



COMMONSRC=./Common/esShader.c    \
          ./Common/esTransform.c \
          ./Common/esShapes.c    \
          ./Common/esUtil.c
COMMONHRD=esUtil.h

CH02SRC=./Chapter_2/Hello_Triangle/Hello_Triangle.c
CH08SRC=./Chapter_8/Simple_VertexShader/Simple_VertexShader.c
CH09SRC1=./Chapter_9/Simple_Texture2D/Simple_Texture2D.c
CH09SRC2=./Chapter_9/MipMap2D/MipMap2D.c
CH09SRC3=./Chapter_9/Simple_TextureCubemap/Simple_TextureCubemap.c
CH09SRC4=./Chapter_9/TextureWrap/TextureWrap.c
CH10SRC=./Chapter_10/MultiTexture/MultiTexture.c
CH11SRC=./Chapter_11/Multisample/Multisample.c
CH11SRC2=./Chapter_11/Stencil_Test/Stencil_Test.c
CH13SRC2=./Chapter_13/ParticleSystem/ParticleSystem.c

default: all

all: ./Chapter_2/Hello_Triangle/CH02_HelloTriangle \
     ./Chapter_8/Simple_VertexShader/CH08_SimpleVertexShader \
     ./Chapter_9/Simple_Texture2D/CH09_SimpleTexture2D \
     ./Chapter_9/MipMap2D/CH09_MipMap2D \
     ./Chapter_9/Simple_TextureCubemap/CH09_TextureCubemap \
     ./Chapter_9/TextureWrap/CH09_TextureWrap \
     ./Chapter_10/MultiTexture/CH10_MultiTexture \
     ./Chapter_11/Multisample/CH11_Multisample \
     ./Chapter_11/Stencil_Test/CH11_Stencil_Test \
     ./Chapter_13/ParticleSystem/CH13_ParticleSystem

clean:
	find . -name "CH??_*" | xargs rm -f

./Chapter_2/Hello_Triangle/CH02_HelloTriangle: ${COMMONSRC} ${COMMONHDR} ${CH02SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH02SRC} -o $@ ${INCDIR} ${LIBS}
./Chapter_8/Simple_VertexShader/CH08_SimpleVertexShader: ${COMMONSRC} ${COMMONHDR} ${CH08SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH08SRC} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_9/Simple_Texture2D/CH09_SimpleTexture2D: ${COMMONSRC} ${COMMONHDR} ${CH09SRC1}
	gcc $(CFLAGS) ${COMMONSRC} ${CH09SRC1} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_9/MipMap2D/CH09_MipMap2D: ${COMMONSRC} ${COMMONHDR} ${CH09SRC2}
	gcc $(CFLAGS) ${COMMONSRC} ${CH09SRC2} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_9/Simple_TextureCubemap/CH09_TextureCubemap: ${COMMONSRC} ${COMMONHDR} ${CH09SRC3}
	gcc $(CFLAGS) ${COMMONSRC} ${CH09SRC3} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_9/TextureWrap/CH09_TextureWrap: ${COMMONSRC} ${COMMONHDR} ${CH09SRC4}
	gcc $(CFLAGS) ${COMMONSRC} ${CH09SRC4} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_10/MultiTexture/CH10_MultiTexture: ${COMMONSRC} ${COMMONHDR} ${CH10SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH10SRC} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_11/Multisample/CH11_Multisample: ${COMMONSRC} ${COMMONHDR} ${CH11SRC}
	gcc $(CFLAGS) ${COMMONSRC} ${CH11SRC} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_11/Stencil_Test/CH11_Stencil_Test: ${COMMONSRC} ${COMMONHDR} ${CH11SRC2}
	gcc $(CFLAGS) ${COMMONSRC} ${CH11SRC2} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_13/Noise3D/CH13_Noise3D: ${COMMONSRC} ${COMMONHDR} ${CH13SRC1}
	gcc $(CFLAGS) ${COMMONSRC} ${CH13SRC1} -o ./$@ ${INCDIR} ${LIBS}
./Chapter_13/ParticleSystem/CH13_ParticleSystem: ${COMMONSRC} ${COMMONHDR} ${CH13SRC2}
	gcc $(CFLAGS) ${COMMONSRC} ${CH13SRC2} -o ./$@ ${INCDIR} ${LIBS}
