ChangeLog
---------

========================
Version 1.2 - 2021/04/17
========================

General:

 - updated GL4ES Documentation
 - updated all libs, see below for changelog.
 - everything compiled with GCC 10.3.0

GL4ES (updated till commit done  Apr 15, 2021):

 - Improve glGetError() handling
 - Set default GL version to 2.1 (instead of 2.0) for GLES2.0 backend
 - GLSL version 1.2 is defaulit now, instead of GLSL 1.1
 - Added ARB_vertex_program and ARB_fragment_program, which is major work by itself and some of notable changes are:
	- Added support for Program Matrix from ARB_vertex_program extension
	- include support for GradARB variant for texture2D and textureCube built-in functions
	- Added ARL and relative addressing support
	- Added the ARB vertex shader attribute vertex.attrib
	- Added the ARB parameter `state.matrix.texture`
	- Added support for ARB_fog_linear
	- Added support of ARB_precision_hint_nicest option
	- Added 'OPTION ARB_position_invariant' to ARB_vertex_program
	- Added sign support for single-value params
	- Added support for direct constant in ARB language
 - Improved FPE with more compatibility fixes
 - Improved Handling of DXTc textures + added some support for DXTc with sRGB textures
 - Improve handling of partial mipmapped textures
 - Added support for Texture LOD access in Fragment Shaders
 - Increased maximum Texture Unit support to 16 (from 8)
 - Added support for a program without Vertex Shader
 - Some fixes for programs that switch GLX Context frequently.
 - Fixes and improvements in fpe_shaders (less array access)
 - Added support for Clipping Plane in custom shaders (help OpenMW water reflection)
 - Added support for multiple FBO attachment (if hardware support it)
 - More optimization on real VBO handling
 - Proper handling of gl_ClipVertex

 - AmigaOS4 specific changes (by ptitSeb and Daniel):
	- Fix: typo in pixel.c, pixel.g was not set
	- Cleaning up Logging
	- Introduced define GL4ES_COMPILE_FOR_USE_IN_SHARED_LIB so gl4es can be used inside of minigl.library (minigl4gl4es) amiga shared library.
	- Additionl extensions GL_MGL_packed_pixels (old Amiga flavour of GL_EXT_packed_pixels) and GL_EXT_compiled_vertex_arrays (note the s at the end, a common typo...)
	- build info can be disabled by silence-define
	- FPS drawing in its own function so that it can be called from outside in case aglSwapBuffers is not called by the client.
	- Improved env var handling.
	- Added a BigEndian only fastpath for GL_INT_8_8_8_8_REV <-> GL_UNSIGNED_BYTE conversion (not more slow-route)
	- Fixed some conversion involving GL_INT_8_8_8_8(_REV) and GL_BGRA that used 2 passes where 1 would be enough
	- Added a few more AmigaOS4 specifics texture format handling, so no conversion needs it at all. Thanks to Daniel for ogles2.library v3.1 where he added new, aos4 only, formats.
		GL_AOS4_texture_format_RGB332
		GL_AOS4_texture_format_RGB332REV
		GL_AOS4_texture_format_RGBA1555REV
		GL_AOS4_texture_format_RGBA8888
		GL_AOS4_texture_format_RGBA8888REV

 - And gazillion of improvements, fixes, dealing with regressions and new features. An actual changelog is _MUCH_ bigger, and there is the only a summary. For the full list go to https://github.com/ptitSeb/gl4es/commits/master

SDL1_gl4es (updated till commit has done Jan 3, 2021, all work done by Capehill):

 - Recompiled with GCC 10.3.0
 - Merged latest SDL1 official changes, many changes, but nextly count only amigaos4 specific fixes:
	- Enabled joysticks support (some missing fix from 2018)
	- Change initialization procedure: some apps do not call SDL_Init, but initialize some subsystem only
	- Fix issue with HWSURFACE blits

SDL2_gl4es (updated till commit done on Apr 10, 2021, all work done by Capehill):

 - Recompiled with GCC 10.3.0
 - Merged latest SDL2 official changes, a lot of changes, but nextly count only amigaos4 specific fixes:
	- Fixed fast calling of SDL_SetWindowSize + SDL_SetWindowPos combo
	- Implement SDL_OpenURL
	- Open screens with SA_LikeWorkbench + smart refresh mode
	- More controller mappings
	- Add Drag n Drop support
	- Avoid blitting over window borders 
	- Use vertex mode when doing Composite to avoid scaling factor issues
	- Do endian conversion for textures in order to fix the opengles2 render
	- Use BMA_ACTUALWIDTH instead of BMA_WIDTH to calculate the renderer output size
	- Use texture blending mode from command queue and simplify composite setup 
	- Change SYSREQ to PRINTSCREEN
	- Fix crash in thread creation
	- Fixed compiler warning 
	- Fix Calling SDL_GetTicks before SDL_Init crashes
	- Fixed SDL timer thread consuming CPU
	- Reset keyboard state during full screen toggle
	- Fix SDL_GetPrefPath 
	- Add native thread and timer support for AmigaOS 4 
	- Improve texture filter setup
	- Fix VSYNC handling during renderer creation


GLU_gl4es 

 - No functional changes, only recompiled with GCC 10.3.0.


========================
Version 1.1 - 2019/12/22
========================

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


========================
version 1.0 - 2019/09/11
========================

 - First release