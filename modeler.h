/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MODELER_H
#define MODELER_H

#include <QtGui/QMainWindow>
#include "ui_modeler.h"

class MObjectContainer;
class GLWidget3D;
class TreeModel;
class MView3D;
class Modeler : public QMainWindow
{
	Q_OBJECT

public:
	Modeler(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Modeler();

private:
	Ui::ModelerClass ui;
	
	GLWidget3D *glWidget3D;
	MObjectContainer *objects;
	TreeModel* objectsModel;
	MView3D *m_view3D;
	
	void createStatusBar();
	void drawScene();

private slots:
	void zScaleChanged(QString index);
	void on_newGridAction_triggered();
	void on_actionAdd_wells_triggered();
};

#endif // MODELER_H
