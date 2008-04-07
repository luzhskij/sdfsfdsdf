/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <QtGui>
#include <QtOpenGL/QtOpenGL>

#include "glwidget3d.h"
#include "globals.h"
#include "mobject.h"

QColor backGround;

GLWidget3D::GLWidget3D(QWidget *parent)
	: QGLWidget(parent)
{
	backGround = QColor::fromRgb(0.0, 0.0, 0.0);
}

GLWidget3D::~GLWidget3D()
{
	makeCurrent();
	glDeleteLists(1, 1);
}

QSize GLWidget3D::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget3D::sizeHint() const
{
    return QSize(600, 600);
}

void GLWidget3D::initializeGL()
{
	qglClearColor(backGround);
	glList = glGenLists(1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
}

void GLWidget3D::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	m_camera.prepareCamera();
	

	glCallList(glList);

	glFlush();


/*	GLfloat position[] = { 0.0, 0.0, 10000, 1.0 };

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const GLfloat movx = 148000.0;
	const GLfloat movy = 88000.0;
	const GLfloat movz = -2400.0;
	const GLfloat size = 10000;
	const GLfloat movx = 0;
	const GLfloat movy = 0;
	const GLfloat movz = 0;
	const GLfloat size = 3;
//	gluLookAt (movx, movy, movz, movx, movy, movz+10000.0, 0.0, 1.0, 0.0);
	
	glLoadIdentity();
//	glTranslatef (-movx, -movy, -movz);
	
	glRotatef ((GLfloat) spin, 1.0, 0.0, 0.0);
	glTranslatef (0, 0, -size*3);

	
	glPushMatrix ();
	glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);
//	glTranslatef (m_camera.x(), m_camera.y(), m_camera.z()/m_camera.zScale());
	glTranslatef (-movx, -movy, -movz);
	glLightfv (GL_LIGHT0, GL_POSITION, position);

	glPopMatrix ();
	

	GLfloat x1 = movx-size;
	GLfloat y1 = movy-size;
	GLfloat x2 = movx+size;
	GLfloat y2 = movy+size;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glColor3f(0.2f, 0.8f, 1.f);
	glVertex3f(x1, y1, movz);
	glVertex3f(x2, y1, movz);
	glVertex3f(x2, y2, movz);
	glVertex3f(x1, y2, movz);
	glEnd();
//	glCallList(glList);	

	glFlush ();*/

}

void GLWidget3D::resizeGL(int width, int height)
{
	double dAspect = width<=height ? double(height)/width : double(width)/height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (45, dAspect, 100, 1000000.);
//	gluPerspective(40.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	this->width = width;
	this->height = height;
}

void GLWidget3D::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
	mousePressed = true;
/*	if (event->buttons() & Qt::MidButton) //Rotate object
		spin = (spin + 30) % 360;
	updateGL();*/
}

void GLWidget3D::mouseMoveEvent(QMouseEvent *event)
{
	int m_dx = (event->x() - lastPos.x());
    int m_dy = (event->y() - lastPos.y());

	if (event->buttons() & Qt::LeftButton) //Rotate object
	{
		int u2 = 1;
		if (event->y() < height/2)
			u2 = -1;
		m_camera.rotate(m_dx, m_dy, u2, mousePressed);
		mousePressed = false;
	}
    else if (event->buttons() & Qt::RightButton) //Zoom in/out object
		m_camera.moveRadial(m_dy);
    else if (event->buttons() & Qt::MidButton)  // Move object
		m_camera.move(lastPos, event->pos(), height);
// 		int yyy=8;

	lastPos = event->pos();
	updateGL();
}

void GLWidget3D::setZScale(int value)
{
	m_camera.setZScale(value);
	m_camera.setCameraOnObject();
	updateGL();
}

GLuint GLWidget3D::getglList()
{
	return glList;
}
