/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef GLWIDGET3D_H
#define GLWIDGET3D_H

#include <QtOpenGL/QGLWidget>
#include <QTimer>

#include "camera.h"

class GLWidget3D : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget3D(QWidget *parent = 0);
	~GLWidget3D();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void  setZScale(int value);
	GLuint getglList();
	Camera &camera() { return m_camera; };

public:
	GLenum	m_FillMode;

public slots:
	void resizeGL(int width, int height);

signals:
	void reDraw();
	void FPS(int frames);

private:
	QPoint lastPos;
	int width, height;
	GLuint glList;
	bool mousePressed;	// remembers the mousepress event end

	Camera m_camera;

protected:
	void initializeGL();
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
};

#endif // GLWIDGET3D_H
