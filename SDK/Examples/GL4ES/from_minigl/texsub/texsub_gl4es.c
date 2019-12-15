/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  texsub.c
 *  This program texture maps a checkerboard image onto
 *  two rectangles.  This program clamps the texture, if
 *  the texture coordinates fall outside 0.0 and 1.0.
 *  If the s key is pressed, a texture subimage is used to
 *  alter the original texture.  If the r key is pressed, 
 *  the original texture is restored.
 */

/*
 * MiniGL adaption by Hans-Joerg Frieden
 * December 1999
 *
 * gl4es adaptation by kas1e, 2019
 *
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <proto/ogles2.h>
#include <proto/exec.h>
#include <stdlib.h>
#include <proto/intuition.h>
#include <stdlib.h>
#include <math.h> 

/* We use GL4ES wrapper functions which have same name as OGLES2 ones (they do some internal GL4ES things) */
void* aglCreateContext2(ULONG * errcode, struct TagItem * tags);
void aglDestroyContext(void* context);
void aglMakeCurrent(void* context);
void aglSwapBuffers();
void aglSetBitmap(struct BitMap *bitmap);
void aglSetParams2(struct TagItem * tags);
void* aglGetProcAddress(const char* name);

/* Minimum stack space */
static USED const char *stack = "$STACK:65535"; 

/*  Create checkerboard textures  */
#define checkImageWidth 64
#define checkImageHeight 64
#define subImageWidth 16
#define subImageHeight 16
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte subImage[subImageHeight][subImageWidth][4];

static GLuint texName;

void makeCheckImages(void)
{
   int i, j, c;
	
   for (i = 0; i < checkImageHeight; i++) {
	  for (j = 0; j < checkImageWidth; j++) {
	 c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
	 checkImage[i][j][0] = (GLubyte) c;
	 checkImage[i][j][1] = (GLubyte) c;
	 checkImage[i][j][2] = (GLubyte) c;
	 checkImage[i][j][3] = (GLubyte) 255;
	  }
   }
   for (i = 0; i < subImageHeight; i++) {
	  for (j = 0; j < subImageWidth; j++) {
	 c = ((((i&0x4)==0)^((j&0x4)==0)))*255;
	 subImage[i][j][0] = (GLubyte) c;
	 subImage[i][j][1] = (GLubyte) 0;
	 subImage[i][j][2] = (GLubyte) 0;
	 subImage[i][j][3] = (GLubyte) 255;
	  }
   }
}

void init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   glEnable(GL_DEPTH_TEST);

   makeCheckImages();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   glGenTextures(1, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
}

void display(void)
{
	float array[] = 
	{
		0.0, 0.0, -2.0, -1.0, 0.0,
		0.0, 1.0, -2.0, 1.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 0.0,
		1.0, 0.0, 0.0, -1.0, 0.0,

		0.0, 0.0, 1.0, -1.0, 0.0,
		0.0, 1.0, 1.0, 1.0, 0.0,
		1.0, 1.0, 2.41421, 1.0, -1.41421,
		1.0, 0.0, 2.41421, -1.0, -1.41421
	};
	
	GLubyte index[] =
	{
		4, 5, 6, 7, 0, 1, 2, 3
	};
	
			
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_TEXTURE_2D);

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glBindTexture(GL_TEXTURE_2D, texName);

   glInterleavedArrays(GL_T2F_V3F, 0, array);
//   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//   glEnableClientState(GL_VERTEX_ARRAY);

//   glDrawArrays(GL_QUADS, 0, 8);
   glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, index);

#if 0
   glBegin(GL_QUADS);
 	glArrayElement(0); 
 	glArrayElement(1); 
 	glArrayElement(2); 
 	glArrayElement(3); 

 	glArrayElement(4); 
 	glArrayElement(5); 
 	glArrayElement(6); 
 	glArrayElement(7); 
  
   glEnd();
#endif   
   
   glFlush();
   glDisable(GL_TEXTURE_2D);
//   mglUnlockDisplay();
   //mglSwitchDisplay();
   aglSwapBuffers(); 
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -3.6);
}

static void
idle(void)
{
	display();
}


int main(int argc, char *argv[])
{
	GLint width=640; GLint height=480;
	int i;

	for (i=1; i<argc; i++)
	{
	if (0 == strcasecmp(argv[i], "-width"))
	{
		i++;
		width = atoi(argv[i]);
	}
	if (0 == strcasecmp(argv[i], "-height"))
	{
		i++;
		height = atoi(argv[i]);
	}
	}

		ULONG errCode = 0;
		const char titleStrBase[] = "Hello OpenGLES2";
		struct Window *win=IIntuition->OpenWindowTags(NULL,
								WA_Title,				titleStrBase,
								WA_Activate,			TRUE,
								WA_RMBTrap,				TRUE,
								WA_DragBar,				TRUE,
								WA_DepthGadget,			TRUE,
								WA_SimpleRefresh,		TRUE,
								WA_CloseGadget,			TRUE,
								WA_SizeGadget,			TRUE, 
								WA_IDCMP,				IDCMP_REFRESHWINDOW | IDCMP_NEWSIZE | 
														IDCMP_CLOSEWINDOW | IDCMP_RAWKEY,
								WA_InnerWidth,			640,
								WA_InnerHeight,			480,
								WA_MinWidth,			100,
								WA_MinHeight,			100,
								WA_MaxWidth,			2048,
								WA_MaxHeight,			2048,
								WA_BackFill, 			LAYERS_NOBACKFILL,
								TAG_DONE);
		
	struct TagItem contextparams[] =
	{
			{OGLES2_CCT_WINDOW,(ULONG)win},
			{OGLES2_CCT_DEPTH,16},
			{OGLES2_CCT_STENCIL,8},
			{OGLES2_CCT_VSYNC,0},
			{OGLES2_CCT_SINGLE_GET_ERROR_MODE,1},
			{OGLES2_CCT_RESIZE_VIEWPORT, TRUE},
			{TAG_DONE,0}
	};
	
	void *ogles_context = aglCreateContext2(&errCode,contextparams);

  	if (ogles_context)
	{
		aglMakeCurrent(ogles_context);
		
		glViewport(0,0, 640, 480);

		init();

		glDisable(GL_BLEND);
		glClearColor(0.2, 0.2, 0.2, 1.0);

		reshape(width,height);

		BOOL bRun = TRUE;
		
		if (!IIntuition->ModifyIDCMP(win, IDCMP_VANILLAKEY|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW))
			bRun = FALSE;

		while (bRun)
		{
			struct IntuiMessage *imsg;

			idle();
			
			glFlush();
			
			aglSwapBuffers();

			while ((imsg = (struct IntuiMessage *)IExec->GetMsg(win->UserPort)))
			{
				switch (imsg->Class)
				{
					case IDCMP_CLOSEWINDOW:
						bRun = FALSE;
						break;
					case IDCMP_VANILLAKEY:
						switch (imsg->Code)
						{
							case 's':
							case 'S':
								glBindTexture(GL_TEXTURE_2D, texName);
								glTexSubImage2D(GL_TEXTURE_2D, 0, 12, 44, subImageWidth,subImageHeight, GL_RGBA,
								GL_UNSIGNED_BYTE, subImage);
								break;
							case 'r':
							case 'R':
								glBindTexture(GL_TEXTURE_2D, texName);
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
								checkImageHeight, 0, GL_RGBA,
								GL_UNSIGNED_BYTE, checkImage);
								break;
							case 27:
								glDeleteTextures(1, &texName);
								aglDestroyContext(ogles_context);
								IIntuition->CloseWindow(win); 
								exit(0);
								break;
						}
					default:
						break;
				}
				IExec->ReplyMsg((struct Message *)imsg);
				}
		}
		glDeleteTextures(1, &texName);
		aglDestroyContext(ogles_context);
		IIntuition->CloseWindow(win);
	}
	return 0;
}