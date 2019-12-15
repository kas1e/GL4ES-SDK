/* A simple demo that uses glutTimerFunc().
 *
 * Draws an animated triangle.
 *
 * by Hans de Ruiter
 *
 * gl4es adaptation by kas1e
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

#define WIDTH 640
#define HEIGHT 480

GLfloat rot = 0.0;


static void reshape(GLsizei w, GLsizei h);

	   
void idle(void)
{ 	
    rot += 0.1;
    if (rot > 360.0)
       rot = 0.0;
  
}
	   
static void init(int w, int h)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
	reshape(w, h);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


static void display()
{	
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-3.0f);
	glRotatef(rot, 0.0, 0.0, 1.0);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	
}


static void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
		
		init(WIDTH, HEIGHT);
		reshape(WIDTH,HEIGHT);
		
		BOOL bRun = TRUE;

		if (!IIntuition->ModifyIDCMP(win, IDCMP_VANILLAKEY|IDCMP_MOUSEMOVE|IDCMP_MOUSEBUTTONS|IDCMP_NEWSIZE|IDCMP_CLOSEWINDOW))
			bRun = FALSE;

		while (bRun)
		{
			struct IntuiMessage *imsg;

			display();
			
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
