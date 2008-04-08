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
}

void GLWidget3D::resizeGL(int width, int height)
{
	double dAspect = width<=height ? double(height)/width : double(width)/height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (45, dAspect, 100, 1000000.);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	this->width = width;
	this->height = height;
}

void GLWidget3D::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
	mousePressed = true;
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
