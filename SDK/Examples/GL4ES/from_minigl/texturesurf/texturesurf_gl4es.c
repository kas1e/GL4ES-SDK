/*
 * License Applicability. Except to the extent portions of this file are
 * made subject to an alternative license as permitted in the SGI Free
 * Software License B, Version 1.1 (the "License"), the contents of this
 * file are subject only to the provisions of the License. You may not use
 * this file except in compliance with the License. You may obtain a copy
 * of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
 * Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
 * 
 * http://oss.sgi.com/projects/FreeB
 * 
 * Note that, as provided in the License, the Software is distributed on an
 * "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
 * DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
 * CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
 * 
 * Original Code. The Original Code is: OpenGL Sample Implementation,
 * Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
 * Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
 * Copyright in any portions created by third parties is as indicated
 * elsewhere herein. All Rights Reserved.
 * 
 * Additional Notice Provisions: The application programming interfaces
 * established by SGI in conjunction with the Original Code are The
 * OpenGL(R) Graphics System: A Specification (Version 1.2.1), released
 * April 1, 1999; The OpenGL(R) Graphics System Utility Library (Version
 * 1.3), released November 4, 1998; and OpenGL(R) Graphics with the X
 * Window System(R) (Version 1.3), released October 19, 1998. This software
 * was created using the OpenGL(R) version 1.2.1 Sample Implementation
 * published by SGI, but has not been independently verified as being
 * compliant with the OpenGL(R) version 1.2.1 Specification.
 *
 */

/*  texturesurf.c
 *  This program uses evaluators to generate a curved
 *  surface and automatically generated texture coordinates.
 */
 
/*
 * gl4es adaptation by kas1e
 */

#include <GL/gl.h>
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

/* Apps that use the GLU NURBS functions often need a bit more stack space. 
 * Allocate a bit more stack. */
static USED const char *stack = "$STACK:65535"; 

GLfloat ctrlpoints[4][4][3] = {
   {{ -1.5, -1.5, 4.0}, { -0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{ -1.5, -0.5, 1.0}, { -0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{ -1.5, 0.5, 4.0}, { -0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{ -1.5, 1.5, -2.0}, { -0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, 
			{{1.0, 0.0}, {1.0, 1.0}}};

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glEvalMesh2(GL_FILL, 0, 20, 0, 20);
   glFlush();
}

#define	imageWidth 64
#define	imageHeight 64
GLubyte image[3*imageWidth*imageHeight];

void makeImage(void)
{
   int i, j;
   float ti, tj;
   
   for (i = 0; i < imageWidth; i++) {
      ti = 2.0*3.14159265*i/imageWidth;
      for (j = 0; j < imageHeight; j++) {
         tj = 2.0*3.14159265*j/imageHeight;

         image[3*(imageHeight*i+j)] = (GLubyte) (127.0*(1.0+sin(ti)));
         image[3*(imageHeight*i+j)+1] = (GLubyte) (127.0*(1.0+cos(2*tj)));
         image[3*(imageHeight*i+j)+2] = (GLubyte) (127.0*(1.0+cos(ti+tj)));
      }
   }
}

void init(void)
{
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0][0]);
   glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 
           0, 1, 4, 2, &texpts[0][0][0]);
   glEnable(GL_MAP2_TEXTURE_COORD_2);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   makeImage();

   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_FLAT);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w, 
              4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
   else
      glOrtho(-4.0*(GLfloat)w/(GLfloat)h, 
              4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(85.0, 1.0, 1.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
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

		BOOL bRun = TRUE;

		if (!IIntuition->ModifyIDCMP(win, IDCMP_VANILLAKEY|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW))
			bRun = FALSE;

		while (bRun)
		{
			struct IntuiMessage *imsg;

			display();
			
			reshape(640,480);
			
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
							case 27:
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
		aglDestroyContext(ogles_context);
		IIntuition->CloseWindow(win);
	}
	return 0; 
}
