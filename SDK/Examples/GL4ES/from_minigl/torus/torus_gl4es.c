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

/*
 *  torus.c
 *  This program demonstrates the creation of a display list.
 *
 *   gl4es adaptation by kas1e
 */

#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/ogles2.h>

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

/* Some <math.h> files do not define M_PI... */
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLuint theTorus;


/* Draw a torus */
static void torus(int numc, int numt)
{
   int i, j, k;
   double s, t, x, y, z, twopi;

   twopi = 2 * (double)M_PI;
   for (i = 0; i < numc; i++) {
      glBegin(GL_QUAD_STRIP);
      for (j = 0; j <= numt; j++) {
         for (k = 1; k >= 0; k--) {
            s = (i + k) % numc + 0.5;
            t = j % numt;

            x = (1+.1*cos(s*twopi/numc))*cos(t*twopi/numt);
            y = (1+.1*cos(s*twopi/numc))*sin(t*twopi/numt);
            z = .1 * sin(s * twopi / numc);
            glVertex3f(x, y, z);
         }
      }
      glEnd();
   }
}

/* Create display list with Torus and initialize state */
static void init(void)
{
   theTorus = glGenLists (1);
   glNewList(theTorus, GL_COMPILE);
   torus(8, 25);
   glEndList();

   glShadeModel(GL_FLAT);
   glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Clear window and draw torus */
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);
   glCallList(theTorus);
   glFlush();
}

/* Handle window resize */
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}

/* Rotate about x-axis when "x" typed; rotate about y-axis
   when "y" typed; "i" returns torus to original view */
/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
   case 'x':
   case 'X':
      glRotatef(30.,1.0,0.0,0.0);
      break;
   case 'y':
   case 'Y':
      glRotatef(30.,0.0,1.0,0.0);
      break;
   case 'i':
   case 'I':
      glLoadIdentity();
      gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
      break;
   case 27:
      exit(0);
      break;
   }
}

int main(int argc, char **argv)
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
		
		reshape(640,480);
 
		BOOL bRun = TRUE;
		
		if (!IIntuition->ModifyIDCMP(win, IDCMP_VANILLAKEY|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW))
			bRun = FALSE;

		while (bRun)
		{
			struct IntuiMessage *imsg;

			display();
									
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
							case 'x':
							case 'X':
								glRotatef(30.,1.0,0.0,0.0);
								display();
								aglSwapBuffers(); 
								break;
							case 'y':
							case 'Y':
								glRotatef(30.,0.0,1.0,0.0);
								display();
								aglSwapBuffers(); 
								break;
							case 'i':
							case 'I':
								glLoadIdentity();
								gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
								display();
								aglSwapBuffers(); 
								break;
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