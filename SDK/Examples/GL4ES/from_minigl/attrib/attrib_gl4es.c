/*
 *  gl4es adaptation by kas1e
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

#include <proto/exec.h>
#include <proto/intuition.h>

#include <proto/ogles2.h>

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

void myinit(void)
{
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
}

void drawPlane(void)
{
	glBegin (GL_QUADS);
		glNormal3f (0.0, 0.0, 1.0);
		glVertex3f (-1.0, -1.0, 0.0);
		glVertex3f (0.0, -1.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (-1.0, 0.0, 0.0);

		glNormal3f (0.0, 0.0, 1.0);
		glVertex3f (0.0, -1.0, 0.0);
		glVertex3f (1.0, -1.0, 0.0);
		glVertex3f (1.0, 0.0, 0.0);
		glVertex3f (0.0, 0.0, 0.0);

		glNormal3f (0.0, 0.0, 1.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (1.0, 0.0, 0.0);
		glVertex3f (1.0, 1.0, 0.0);
		glVertex3f (0.0, 1.0, 0.0);

		glNormal3f (0.0, 0.0, 1.0);
		glVertex3f (0.0, 0.0, 0.0);
		glVertex3f (0.0, 1.0, 0.0);
		glVertex3f (-1.0, 1.0, 0.0);
		glVertex3f (-1.0, 0.0, 0.0);
	glEnd();
}

void display (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix ();
	glTranslatef (-2.1, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	drawPlane ();
	glPopMatrix ();

	glPushAttrib(GL_CURRENT_BIT);

	glPushMatrix ();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef (0.0, 0.0, 0.0);
	drawPlane ();
	glPopMatrix ();

	glPopAttrib();

	glPushMatrix ();
	glTranslatef (2.1, 0.0, 0.0);
	drawPlane ();
	glPopMatrix ();

	glFlush ();
}

void myReshape(int w, int h)
{
	glViewport (0, 0, w, h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	if (w <= h) {
		glOrtho (-1.5, 1.5, -1.5*(GLdouble)h/(GLdouble)w,
			1.5*(GLdouble)h/(GLdouble)w, -10.0, 10.0);
	}
	else {
		glOrtho (-1.5*(GLdouble)w/(GLdouble)h,
			1.5*(GLdouble)w/(GLdouble)h, -1.5, 1.5, -10.0, 10.0);
	}
	glMatrixMode (GL_MODELVIEW);
}


/*  Main Loop
 *  Open window with initial window size, title bar,
 *  RGBA display mode, and handle input events.
 */
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
								WA_SizeGadget,			TRUE,
								WA_IDCMP,				IDCMP_REFRESHWINDOW | IDCMP_NEWSIZE | 
														IDCMP_CLOSEWINDOW | IDCMP_RAWKEY,
								WA_InnerWidth,			500,
								WA_InnerHeight,			200,
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

		myinit();
		myReshape(500,200);
		
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
