ChangeLog
---------


Version 1.1 - 2019/12/22

General:

 - updated GL4ES Documentation
 - updated SDL1, SDL2 and GL4ES libs, see below for changelog.
 - fixed typos and grammars in the readme
 
GL4ES (updated till commit done Dec 19, 2019):

 - use -gstabs for AmigaOS4 debug builds
 - Using real VBO now by default for glList() & glBindBuffer() (help a lot to games which use those functions, sometimes speed gain is very high).
   Also added LIBGL_USEVBO environment so it can be disabled/enabled. See usage.md for more details.
 - Exposed GL_ARB_map_buffer_range extension, and fixed its behavior (help Arx Libertatis, using real VBO)
 - Don't try to clean resources if GLES lib is already unloaded (fixed crashes with games/apps which didn't clear/free textures on exit)
 - Fixed a nasty bugs with texture handling (when going gl(Sub)TexImage2D on other than GL_TEXTURE0 (helps eDuke32)
 - Some FPE Fragment shader optimization on (multi)texturing
 - Force to recalculate mipmap if needed after a glCopyTexSubImage2D
 - Added support for GL_NV_fog_distance extension
 - Fixed an issue with two-sided lightning (fixed ManiaDrive)
 - Optimized Instanced drawing (should help FEZ)
 - Fixed LIBGL_RECYCLEFBO 1 option (when that environment set to 1, then if FBO is deleted, it's then not really deleted but goes on a "pool" of unused FBO.
   Then if an FBO is recreated and the pool is not empty, that "unused" FBO is taken from the pool and used).
 - Added internally some betters ways to hack shaders on the fly when that really need it.
 - Added support for GL_R8 as internal format (helps a lot video in Desperado)
 - FOG is now calculated per pixel (instead of per-vertex) shader (help Night Of The Zombies)
 - Force 24/8 for LIBGL_FB=2, also always report 24/8 even if depth is 16 when stencil is 8
 - Fixed an issue when using GL_UNPACK_BUFFER with glSubTexImage2D and some shrink mode (or automatic NPOT->POT conversion)
 - Redone a bit a shaderconv to avoid memory overwriting

SDL1_gl4es (updated till commit done Sep 21, 2019): 

 - A window wasn't active by default after uniconification.
  
SDL2_gl4es (updated till commit done Oct 19, 2019):

 - Fix mouse wrapping (help Irrlicht Engine when used with SDL2)
 - Some debug output was left unnoticed, removed.
 - implemented SetWindowResizable()





version 1.0 - 2019/09/11

 - First release
