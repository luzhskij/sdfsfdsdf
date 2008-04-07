/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */
#include <math.h>

#include "camera.h"
#include "mobject.h"
#include "globals.h"

static int spin = 0;

Camera::Camera()
{
	u = 1;
	m_zScale = 1;
}

Camera::~Camera()
{
}

void Camera::rotate(int dx, int dy, int u2, bool mousePressed)
{
	cam_b += dy/80.f;
	if (mousePressed)
		m_rotateBehavior = u*u2;
	cam_a += m_rotateBehavior*dx/80.f;

	calculateCameraCoordinates();
}

void Camera::moveRadial(int dy)
{
	cam_r -= 0.5*cam_r*(dy)/40.f;
	
	calculateCameraCoordinates();
}

void Camera::move(const QPoint &lastPos, const QPoint &currentPos, int height)
{
	GLint    viewport[4];    // параметры viewport-a.
	GLdouble projection[16]; // матрица проекции.
	GLdouble modelview[16];  // видовая матрица.
	GLdouble vx, vy, vz;       // координаты курсора мыши в системе координат viewport-a.
	GLdouble wx, wy, wz, dx, dy, dz;       // возвращаемые мировые координаты.
	glGetIntegerv(GL_VIEWPORT, viewport);           // узнаём параметры viewport-a.
	glGetDoublev(GL_PROJECTION_MATRIX, projection); // узнаём матрицу проекции.
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // узнаём видовую матрицу.
		// переводим оконные координаты курсора в систему координат viewport-a.
	dx = 0;
	dy = 0;
	dz = 0;
		// вычисляем координату z рассматриваемого объекта в системе координат viewport.
	gluProject(x2, y2, z2/m_zScale, modelview, projection, viewport, &vx, &vy, &vz);

	vx = currentPos.x();
	vy = height - currentPos.y() - 1; // где height - текущая высота окна.
	gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
	dx += wx;
	dy += wy;
	dz += wz;

	vx = lastPos.x();
	vy = height - lastPos.y() - 1;
	gluUnProject(vx, vy, vz, modelview, projection, viewport, &wx, &wy, &wz);
	dx -= wx;
	dy -= wy;
	dz -= wz;

	x2 = x2-dx;
	y2 = y2-dy;
	z2 = z2-dz*m_zScale;
	
	calculateCameraCoordinates();
}

void Camera::calculateCameraCoordinates()
{
	if (cam_b < 0)
		cam_b = 2*pi + cam_b;
	if (cam_b > 2*pi)
		cam_b = cam_b - 2*pi;

	x1 = x2+cam_r*cos(cam_b)*cos(cam_a);
	y1 = y2-cam_r*cos(cam_b)*sin(cam_a);
	z1 = z2+cam_r*sin(cam_b);

	if ((cam_b > pi/2) && (cam_b < 3*pi/2))
		u = -1;
	else
		u = 1;
}

void Camera::prepareCamera()
{
	gluLookAt(x1, y1, z1, x2, y2, z2, 0, 0, u);
	glScalef(1.0, 1.0, GLfloat(m_zScale));
	
	GLfloat position[] = { 0, 0, 5000, 1.0 };
	glPushMatrix ();
		glRotated ((GLdouble) spin, 1.0, 0.0, 0.0);	
		glTranslated (x(), y(), z()/zScale());
		glLightfv (GL_LIGHT0, GL_POSITION, position);
/*		GLfloat x = position[0];
		GLfloat y = position[1];
		GLfloat z = position[2];
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBegin(GL_QUADS);
		glColor3f(0.2f, 0.8f, 1.f);
		glVertex3f(x, y, z);
		glVertex3f(x+10000, y, z);
		glVertex3f(x+10000, y+10000, z+10000);
		glVertex3f(x, y+1000, z+1000);
		glEnd();*/
	glPopMatrix();
}

void Camera::setCameraOnObject()
{
	x2 = (borders().minX+borders().maxX)/2;
	y2 = (borders().minY+borders().maxY)/2;
	z2 = (borders().minZ+borders().maxZ)/2*m_zScale;
	calculateCameraCoordinates();
}

void Camera::setDefaultCamera()
{
	cam_a = (GLdouble)pi/2;
	cam_b = (GLdouble)pi/4;
	cam_r = 1.5*(borders().maxX-borders().minX);
}

void Camera::setZScale(int value)
{
	m_zScale = value;
}
