# GL4ES-SDK
GL4ES SDK for AmigaOS4

# What is GL4ES 

GL4ES is an OpenGL 2.1/1.5 to GL ES 2.0/1.1 translation library, with support for Pandora, ODroid, OrangePI, CHIP, Raspberry PI, Android and AmigaOS4.

For AmigaOS4 translation done via ogles2.library (written by Daniel "Daytona" Muessener), and which in turn works on top of warp3dnova.library (written by Hans de Ruiter).

Minimum requirements for ogles2.library are 2.8 and for warp3dnova.library it should be 1.65

With GL4ES we have not only OpenGL1.x as with MiniGL, but OpenGL 1.x, OpenGL2.x and even some limited OpenGL3.x support.

Also it is faster than MiniGL and bug-less.

Written by Sebastian "ptitSeb" Chevalier. AmigaOS4 version handled by kas1e, but all actual AmigaOS4 related code still done by ptitSeb.

Actual code: https://github.com/ptitSeb/gl4es

Main site: http://ptitseb.github.io/gl4es/



# What is GL4ES SDK for AmigaOS4

GL4ES SDK for AmigaOS4 come not only with gl4es link library, but also with set of other link libraries which help to use it more widely, such as SDL1, SDL2 and GLU.

Currently in the first release of SDK it contains those libraries:

libgl4es.a      : gl4es (r1.974, Sep 7, 2019) : https://github.com/ptitSeb/gl4es 

libgGLU_gl4es   : GLU  (9.0.0,  Apr 15, 2018) : https://github.com/ptitSeb/GLU

libSDL_gl4es.a  : SDL1 (1.2.15, Sep 1, 2019)  : https://github.com/AmigaPorts/SDL/tree/SDL-1.2

libSDL2_gl4es.a : SDL2 (2.0.8, Apr 9, 2019)   : https://github.com/AmigaPorts/SDL/tree/master

While actual AmigaOS4 changes are in gl4es & glu repos, SDL1/2 changes related to handling of GL4ES still not on github, as at the moment it's still unclear how to handle that all better:
 - or to put them to original repos and made a way to choose with what support to compile (MiniGL or GL4ES).
 - or to just make separate branches, where remove MiniGL and keep only GL4ES, so that there is no chaos in the repositories.
 - or to find some other clean-to-handle way.
In other words at the moment in terms of SDL only ready to use static link libs, without actual code changes (in which probably no one interested if everything works)



# How to install

1. Be sure that you have installed original SDL1, SDL2 and OpenGLES2 SDK

	SDL1: http://os4depot.net/share/library/misc/sdl.lha
	
	SDL2: http://os4depot.net/share/library/misc/sdl2.lha
	
	OGLES2 SDK : that one can be taken together with ogles2.library itself from Enhancer pack.

2. Put gl4es_sdk's libraries to SDK/local/newlib/lib/

3. Put gl4es_sdk's GL includes to the place where you have old MiniGL ones (don't forget to save them, to something like GL_save_minigl). It can be in SDK/local/common/include/GL or in SDK/local/newlib/include/GL. Just be sure nowhere else old MiniGL includes will be taken.
   
4. For SDL1, in SDK/local/newlib/include/SDL/SDL_opengl.h comment out include of <mgl/gl.h> (so to avoid name conflicts and co).
   For SDL2 no such include used, so no conflicts there.

One may ask, why remove MiniGL includes, and why not put GL4ES includes to some directory like GL4ES instead of GL (so to not touch MiniGL ones) : yes, you can do that. Just when you will later work with GL4ES, then everywhere and every time you will be in needs to change include of opengl includes from GL to GL4ES directory. And when sometime in your sources you will forget or miss to do so, then you will have name conflicts and clashes. But you can choose the way you want, of course. I just find it easier to have it in GL.



# How to use:

If you want to use pure AmigaOS API + GL (without SDL) , you need to use ogles2 context instead of minigl one. You shouldn't use directly any agl* functions from ogles2.library, but instead their alternatives from gl4es: they have the same name, just before calling original agl* ogles2 function, in wrapper ones done some internal for gl4es things. For developer it is a matter of only adding necessary defines. I rewrote few random minigl examples from minigl distributive to make them works over gl4es and put them to Examples/from_minigl, so you can see how to work with pure AmigaOS4 API + GL4ES.

If you want to use SDL1/2, then there nothing need to be changed, only compiling lines changes from -lSDL -lGL -lGLU , to -lSDL_gl4es -lglu_gl4es -lgl4es , or to -lSDL2_gl4es if SDL2 in use.

Examples directory also comes with some SDL1 examples (just a few randomly taken nehe's tutorials, with amigaos4 makefile), as well as with with some random SDL2 examples which I take from the google, adapt a bit, and made a makefiles for.

Remember, that GL4ES can be also controlled via environments a lot (that, of course, includes AmigaOS4 version too, where you just control envs via "setenv LIBGL_xxx xx"). Full list of environments and their description can be found in Documentation/GL4ES/USAGE.md 



# How be up to date:

I hope to update SDK from time to time with newer version of gl4es, SDL1 and SDL2 libraries, but you always can go to https://github.com/ptitSeb/gl4es , and build up2date version yourself. For myself, I do it all on crosscompiler (cygwin) and to build it I just do:

cd gl4es-master

mkdir build

cd build

cmake \
-DCMAKE_SYSTEM_NAME=Generic \
-DAMIGAOS4=1 \
-DSTATICLIB=ON \
-DCMAKE_SYSTEM_VERSION=1 \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_C_COMPILER="/usr/local/amiga/bin/ppc-amigaos-gcc" \
-DCMAKE_CXX_COMPILER="/usr/local/amiga/bin/ppc-amigaos-g++" \
-DCMAKE_LINKER="/usr/local/amiga/bin/ppc-amigaos-ld" \
-DCMAKE_AR="/usr/local/amiga/bin/ppc-amigaos-ar" \
-DCMAKE_RANLIB="/usr/local/amiga/bin/ppc-amigaos-ranlib" \
-DCMAKE_FIND_ROOT_PATH="/usr/local/amiga/ppc-amigaos/" \
..

make -j4

And in build/lib directory you have libGL.a, which you just rename after to libgl4es.a



=== Thanks to:

to ptitSeb for gl4es and all his help with everything : https://github.com/ptitSeb/gl4es/

to Daniel for OpenGL ES2 and all his help with everything : http://www.goldencode.de/

to Hans for Warp3DNova and all his help with everything : https://keasigmadelta.com/

to Capehill for actual SDL1 and SDL2 ports and all his help with everything: https://github.com/AmigaPorts/SDL

to others for help and tests 

(c) kas1e, 10 sep 2019
 
