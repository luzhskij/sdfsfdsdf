/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <QMessageBox>
#include <QTextCodec>
#include <QFileDialog>
#include <QTextStream>

#include "treemodel.h"
#include "modeler.h"
#include "newgrid.h"
#include "glwidget3d.h"
#include "mgrid.h"
#include "mmodel.h"
#include "mfolder.h"
#include "globals.h"
#include "mprogressbartracker.h"
#include "mview3d.h"
#include "mwell.h"
#include "treeitem.h"

#include "mcolorgenerator.h"

Modeler::Modeler(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	
	objectsModel = new TreeModel(this);
	m_view3D = new MView3D(objectsModel);
	objectsModel->setCurrentView(m_view3D);

	glWidget3D = dynamic_cast<GLWidget3D*> (m_view3D->widget());
	QHBoxLayout *glLayout = new QHBoxLayout(ui.tab);
	glLayout->addWidget(glWidget3D);
	glLayout->setMargin(0);
	
	ui.objectList->setModel(objectsModel);

	createStatusBar();

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(codec);

	QString str[] = {"1", "2", "5", "10", "20", "25", "50", "100", "200"};
	for(int i = 0; i < 9; i++)
		ui.zScaleBox->addItem(str[i]);
	glWidget3D->setZScale(ui.zScaleBox->currentText().toInt());
	connect(ui.zScaleBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(zScaleChanged(QString)));
}

Modeler::~Modeler()
{

}

void Modeler::on_newGridAction_triggered()
{
	NewGrid dialog(this);
	
	if (dialog.exec())
	{
		if (dialog.import)
		{
			MGrid *grid = new MGrid(dialog.fileName, dialog.invert);
			MModel *model = new MModel(grid, MModel::followTop, 20, 1);
			objectsModel->add(model);
			objectsModel->add(grid);
		}
		else if (dialog.create)
		{
			MProgressBarTracker progressTracker;
			statusBar()->addWidget(progressTracker.progressBar());
			MGrid *grid = new MGrid(dialog.createParams);
			grid->calculate(dialog.sourcePoints, MGrid::M_POWER, 2.5, progressTracker);
			objectsModel->add(grid);
		}

		drawScene();
	}
}

void Modeler::on_actionAdd_wells_triggered()
{
	// Reading XYZ file format which contains data about well geometry
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open well coordinates file"),
			"",
			tr("XYZ files (*.xyz)"));

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	in.setCodec("CP-1251");
	
	MColorGenerator colorGenerator;
	
	MFolder *wellFolder = new MFolder(MObject::getFileName(fileName));
	MWell *newWell = NULL;
	QString line = in.readLine();
	while (!in.atEnd())
	{
		bool deleted = false;
		if (line.at(0) == QChar('*')) // checks whether first char at the line in is "*" which is a marker of a new well
		{
			newWell = new MWell(line.mid(1, line.length()-2));
			newWell->setColor(colorGenerator.getNewColor());
			line = in.readLine();
		}
		QStringList list = line.split(";");
		if (list.at(4) == "")	//checking whether altitude, i.e. z coordinate is empty - in this case we are not able to deal with the well, so skipping such wells
		{
			delete newWell;
			deleted = true;
			line = in.readLine();
			while (!in.atEnd() && line.at(0) != QChar('*'))	//checking for the new well
				line = in.readLine();
		}
		else	//filling well-object with geometry information
		{
			WellNode wellNode;
			wellNode.h = list.at(1).toDouble();
			wellNode.x = list.at(2).toDouble();
			wellNode.y = list.at(3).toDouble();
			wellNode.z = list.at(4).toDouble();
			newWell->addNode(wellNode);
			line = in.readLine();
		}
		
		if (in.atEnd())
		{
			if (!deleted)
			{
				newWell->finalize();
				wellFolder->appendChild(newWell);
			}
		}
		else
		{
			if ( line.at(0) == QChar('*') && !deleted )  // new well found - adding previous well to the list	
			{
				newWell->finalize();
				wellFolder->appendChild(newWell);
			}
		}
	}
	
	objectsModel->add(wellFolder);
	
	drawScene();
}

void Modeler::zScaleChanged(QString index)
{
	glWidget3D->setZScale(index.toInt());
}

void Modeler::drawScene()
{
	m_view3D->calculateBorders();
	glWidget3D->camera().setDefaultCamera();
	glWidget3D->camera().setCameraOnObject();
	m_view3D->draw();
}

void Modeler::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}
