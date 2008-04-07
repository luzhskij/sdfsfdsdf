/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glu.h>
#include <QPoint>

#include "borderstruct.h"

class Camera
{
public:
	Camera();
	~Camera();
	
	void rotate(int dx, int dy, int u2, bool mousePressed);
	void moveRadial(int dy);
	void move(const QPoint &lastPos, const QPoint &currentPos, int height);
	void calculateCameraCoordinates();
	
	void prepareCamera();
	
	void setCameraOnObject();
	void setDefaultCamera();
	
	void setZScale(int value);
	int zScale() const { return m_zScale; }
	
	BorderStruct &borders() { return m_borders; }
	
	GLdouble eyex() const { return x1; }
	GLdouble eyey() const { return y1; }
	GLdouble eyez() const { return z1; }
	GLdouble x() const { return x2; }
	GLdouble y() const { return y2; }
	GLdouble z() const { return z2; }
	GLdouble vx() const { return x1-x2; }
	GLdouble vy() const { return y1-y2; }
	GLdouble vz() const { return z1-z2; }
	
private:
	int u;  // upVector
	GLdouble	x1,y1,z1,x2,y2,z2;
	GLdouble	cam_r,cam_a,cam_b;
	int m_zScale;
	BorderStruct m_borders;
	int m_rotateBehavior;
};

#endif
