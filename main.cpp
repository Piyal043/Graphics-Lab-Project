
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<math.h>
#include<stdio.h>

static int slices = 16;
static int stacks = 16;
GLdouble tx=0,ty=0,tz=-10,rx=0,ry=0,rz=0,sx=1,sy=1,sz=1;
GLdouble ex=0,ey=0,ez=0,cx=0,cy=0,cz=0,hx=0,hy=1,hz=0;
GLdouble dt=0.5,dr=5,ds=1.5,de=5,theta,radius=15,roll=0;

static GLdouble cube_vertices[8*3] = {
    -1, -1, 1,
    1, -1, 1,
    1, 1, 1,
    -1, 1, 1,
    -1, -1, -1,
    1, -1, -1,
    1, 1, -1,
    -1, 1, -1
};
static GLdouble cube_colors[8*3] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
    1, 1, 0,
    1, 0, 1,
    0, 1, 1,
    0, 0, 0,
    0.5, 0.5, 0.5
};
static GLubyte cube_indices[24] = {
    0, 1, 2, 3,
    1, 5, 6, 2,
    7, 6, 5, 4,
    0, 3, 7, 4,
    3, 2, 6, 7,
    4, 5, 1, 0
};
static void cube()
{
    glEnableClientState( GL_VERTEX_ARRAY );
    //glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_vertices);
    //glColorPointer(3, GL_DOUBLE, 3 * sizeof(GLdouble), cube_colors);
    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cube_indices);
    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

static enum{
    scaling,
    rotation,
    TRANSLATION,
    eye
}task=TRANSLATION;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
void cube2()
{
    //front
    glBegin(GL_QUADS);
    //glColor3f(1,1,0);
    glVertex3f(-1,1,1);
    glVertex3f(-1,-1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,1,1);

    //right
    //glColor3f(0,0,1);
    glVertex3f(1,1,1);
    glVertex3f(1,-1,1);
    glVertex3f(1,-1,-1);
    glVertex3f(1,1,-1);

    //back
    //glColor3f(1,0,1);
    glVertex3d(1,1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,1,-1);

    //left
    //glColor3f(1,0,0);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,-1,1);
    glVertex3d(-1,1,1);

    //up
    //glColor3f(0,0,0);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,1,1);
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);

    //down
    //glColor3f(.5,0,1);
    glVertex3d(-1,-1,1);
    glVertex3d(-1,-1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,-1,1);

    glEnd();

}
void cylinder()
{
    GLdouble x,y,i;
    //lower_circle
    glBegin(GL_POLYGON);
        for(i=360;i>=0;i-=5)
        {
            x=cos(i*3.1416/180);
            y=sin(i*3.1416/180);
            glVertex3f(x,y,-1);
        }
    glEnd();
    //upper_circle
    glBegin(GL_POLYGON);
        for(i=0;i<=360;i+=5)
        {
            x=cos(i*3.1416/180);
            y=sin(i*3.1416/180);
            glVertex3f(x,y,1);
        }
    glEnd();

    //body
    glBegin(GL_QUAD_STRIP);
        for(i=0;i<=180;i+=5)
        {
            x=cos(i*3.1416/180);
            y=sin(i*3.1416/180);
            glVertex3f(x,y,-1);
            glVertex3f(x,y,1);
        }
        for(i=360;i>=0;i-=5)
        {
            x=cos(i*3.1416/180);
            y=sin(i*3.1416/180);
            glVertex3f(x,y,-1);
            glVertex3f(x,y,1);
        }
    glEnd();
}
void stand(GLdouble transx,GLdouble transy,GLdouble transz,GLdouble scalex,GLdouble scaley,GLdouble scalez,GLdouble angle,GLdouble resx,GLdouble resy,GLdouble resz)
{
    glPushMatrix();
        glRotated(angle,resx,resy,resz);
        glScaled(scalex,scaley,scalez);
        glTranslated(transx,transy,transz);
        cube();
//        glTranslated(-transx,-transy,-transz);
//        glScaled(1/scalex,1/scaley,1/scalez);
//        glRotated(-angle,resx,resy,resz);
    glPopMatrix();
}
void ground()
{
    glPushMatrix();
        glColor3d(.06,.277,.16);
        stand(0,-39,0,5,.1,5,0,0,0,0);
    glPopMatrix();
}
void base()
{
    //four_stands
    glColor3f(.75,.60,.41);
    stand(0,-1,0,0.1,2,0.1,-20,1,0,1);
    stand(0,-1,0,0.1,2,0.1,20,1,0,1);
    stand(0,-1,0,0.1,2,0.1,-20,1,0,-1);
    stand(0,-1,0,0.1,2,0.1,20,1,0,-1);

    //stands middle joint
    glColor3f(.54,.27,0.07);
    stand(-20,0,4,0.1,.5,0.1,90,0,0,1);
    stand(-20,0,-4,0.1,.5,0.1,90,0,0,1);
    stand(4,0,20,0.1,.5,0.1,90,1,0,0);
    stand(-4,0,20,0.1,.5,0.1,90,1,0,0);

    //stands lower joint
    glColor3f(.54,.27,0.07);
    stand(-32.5,-0,7,0.1,.75,0.1,90,0,0,1);
    stand(-32.5,-0,-7,0.1,.75,0.1,90,0,0,1);
    stand(7,0,32,0.1,.75,0.1,90,1,0,0);
    stand(-7,0,32,0.1,.75,0.1,90,1,0,0);
}
void fan_base(GLdouble transx,GLdouble transy,GLdouble transz,GLdouble scalex,GLdouble scaley,GLdouble scalez)
{
    glPushMatrix();
        glScaled(scalex,scaley,scalez);
        glTranslated(transx,transy,transz);
        cylinder();
    glPopMatrix();
}
void joint()
{
    glColor3f(.54,.27,0.07);
    stand(0,0,.125,.5,.5,.75,0,0,0,0);
    glColor3f(.734,.55,.55);
    fan_base(0,0,2.5,.25,.25,.4);
    glColor3f(0,0,0);
    fan_base(0,0,3.9,.15,.15,.35);
}
void fan()
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double sp = 200.0;
    glColor3f(.69,.74,.70);
    stand(-.75,0,15.5,1,.2,.1,-20+t*sp,0,0,1);
    stand(-.75,0,15.5,1,.2,.1,-140+t*sp,0,0,1);
    stand(-.75,0,15.5,1,.2,.1,-260+t*sp,0,0,1);
}
void windmil()
{
    ground();
    base();
    joint();
    fan();
}
static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    double b = t*50.0,xx=0,zz=0;
    theta=t*30;
    //printf("%d\n",aaa);
    //printf("%lf %lf %lf\n\n",t,a,b);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);
    glTranslated(0, 0, 10);
    glPushMatrix();
        glTranslated(tx,ty,tz);
        glScaled(sx,sy,sz);
        glRotated(rx,1,0,0);
        glRotated(ry,0,1,0);
        glRotated(rz,0,0,1);
        windmil();
    glPopMatrix();



    glLoadIdentity();
    xx=radius*sin(theta*3.1416/180);
    zz=radius*cos(theta*3.1416/180);
    //printf("%f %f %f\n",xx,zz,ey);
    gluLookAt(xx,ey,zz,cx,cy,cz,hx,hy,hz);


    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 't':task=TRANSLATION;break;
        case 'r':task=rotation;break;
        case 's':task=scaling;break;
        case 'e':task=eye;break;

        case 'x':
            switch(task)
            {
                case TRANSLATION:tx+=dt;break;
                case rotation:rx+=dr;break;
                case scaling:sx*=ds;break;
                case eye:ex+=de;break;
            }
            break;
        case 'X':
            switch(task)
            {
                case TRANSLATION:tx-=dt;break;
                case rotation:rx-=dr;break;
                case scaling:sx/=ds;break;
                case eye:ex-=de;break;
            }
            break;
        case 'y':
            switch(task)
            {
                case TRANSLATION:ty+=dt;break;
                case rotation:ry+=dr;break;
                case scaling:sy*=ds;break;
                case eye:ey+=de;break;

            }
            break;
        case 'Y':
            switch(task)
            {
                case TRANSLATION:ty-=dt;break;
                case rotation:ry-=dr;break;
                case scaling:sy/=ds;break;
                case eye:ey-=de;break;
            }
            break;
        case 'z':
            switch(task)
            {
                case TRANSLATION:tz+=dt;break;
                case rotation:rz+=dr;break;
                case scaling:sz*=ds;break;
                case eye:ez+=de;break;
            }
            break;
        case 'Z':
            switch(task)
            {
                case TRANSLATION:tz-=dt;break;
                case rotation:rz-=dr;break;
                case scaling:sz/=ds;break;
                case eye:ez-=de;break;
            }
            break;
        case 'a':
                radius+=1;break;
        case 'A':
                radius-=1;break;
        case 'b':
                ey+=1;break;
        case 'B':
                ey-=1;break;
        case 'n':
                roll+=5;
                hx=cos(roll*3.1416/180);
                hy=sin(roll*3.1416/180);
                break;
        case 'N':
                roll-=5;
                hx=cos(roll*3.1416/180);
                hy=sin(roll*3.1416/180);
                break;
        case 'w':
                cy+=1;break;
        case 'W':
                cy-=1;break;
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Windmill");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glutMainLoop();

    return EXIT_SUCCESS;
}
