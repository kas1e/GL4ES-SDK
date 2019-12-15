/*  mipmap.c
 *  This program demonstrates using mipmaps for texture maps.
 *  To overtly show the effect of mipmaps, each mipmap reduction
 *  level has a solidly colored, contrasting texture image.
 *  Thus, the quadrilateral which is drawn is drawn with several
 *  different colors.
 *
 *  gl4es adaptation by kas1e
 *
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

/* Minimum stack space */
static USED const char *stack = "$STACK:65535"; 

GLubyte mipmapImage32[32][32][3];
GLubyte mipmapImage16[16][16][3];
GLubyte mipmapImage8[8][8][3];
GLubyte mipmapImage4[4][4][3];
GLubyte mipmapImage2[2][2][3];
GLubyte mipmapImage1[1][1][3];

void makeImages(void)
{
    int i, j;

    for (i = 0; i < 32; i++) {
	for (j = 0; j < 32; j++) {
	    mipmapImage32[i][j][0] = 255;
	    mipmapImage32[i][j][1] = 255;
	    mipmapImage32[i][j][2] = 0;
	}
    }
    for (i = 0; i < 16; i++) {
	for (j = 0; j < 16; j++) {
	    mipmapImage16[i][j][0] = 255;
	    mipmapImage16[i][j][1] = 0;
	    mipmapImage16[i][j][2] = 255;
	}
    }
    for (i = 0; i < 8; i++) {
	for (j = 0; j < 8; j++) {
	    mipmapImage8[i][j][0] = 255;
	    mipmapImage8[i][j][1] = 0;
	    mipmapImage8[i][j][2] = 0;
	}
    }
    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    mipmapImage4[i][j][0] = 0;
	    mipmapImage4[i][j][1] = 255;
	    mipmapImage4[i][j][2] = 0;
	}
    }
    for (i = 0; i < 2; i++) {
	for (j = 0; j < 2; j++) {
	    mipmapImage2[i][j][0] = 0;
	    mipmapImage2[i][j][1] = 0;
	    mipmapImage2[i][j][2] = 255;
	}
    }
    mipmapImage1[0][0][0] = 255;
    mipmapImage1[0][0][1] = 255;
    mipmapImage1[0][0][2] = 255;
}

void myinit(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_FLAT);

    glTranslatef(0.0, 0.0, -3.6);
    makeImages();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage32[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 1, 3, 16, 16, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage16[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 2, 3, 8, 8, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage8[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 3, 3, 4, 4, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage4[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 4, 3, 2, 2, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage2[0][0][0]);
    glTexImage2D(GL_TEXTURE_2D, 5, 3, 1, 1, 0,
		 GL_RGB, GL_UNSIGNED_BYTE, &mipmapImage1[0][0][0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEnable(GL_TEXTURE_2D);
}

GLfloat amount = -2.0f;

void display(void)
{
	#if 0
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
    glTexCoord2f(0.0, 8.0); glVertex3f(-2.0, 1.0, 0.0);
    glTexCoord2f(8.0, 8.0); glVertex3f(2000.0, 1.0, -6000.0);
    glTexCoord2f(8.0, 0.0); glVertex3f(2000.0, -1.0, -6000.0);
    glEnd();
    glFlush();
    #else
    glPushMatrix();
    
    glTranslatef(0.0f, 0.0f, -6.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, -amount-2.0f);
    glTexCoord2f(0.0, 8.0); glVertex3f(-1.0f, 1.0f, -amount-2.0f);
    glTexCoord2f(8.0, 8.0); glVertex3f(1.0f, 1.0f, -amount);
    glTexCoord2f(8.0, 0.0); glVertex3f(1.0f, -1.0f, -amount);
    glEnd();
    glPopMatrix();
    glFlush();
	#endif
}

void myReshape(int w, int h)
{
	#if 0 
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0*(GLfloat)w/(GLfloat)h, 1.0, 30000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    #else
    const float ar = (float) w / (float) h;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    #endif
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
								WA_SizeGadget,			TRUE, 
								WA_IDCMP,				IDCMP_REFRESHWINDOW | IDCMP_NEWSIZE | 
														IDCMP_CLOSEWINDOW | IDCMP_RAWKEY,
								WA_InnerWidth,			500,
								WA_InnerHeight,			500,
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

		glViewport(0,0, 500, 500);

		myinit();

		myReshape(500,500);

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
							case '+':
								amount += 0.1f;
								break;
							case '-':
								amount -= 0.1f;
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