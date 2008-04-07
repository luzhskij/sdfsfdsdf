/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include "newgrid.h"

#include <math.h>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

NewGrid::NewGrid(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	QWidget::setWindowFlags(Qt::Dialog ); // | Qt::MSWindowsFixedSizeDialogHint
	validDataFile = false;
	xSizeEditing = false;
	ySizeEditing = false;
	
	QDoubleValidator * val = new QDoubleValidator( this );
	ui.minXEdit->setValidator(val);
	ui.maxXEdit->setValidator(val);
	ui.minYEdit->setValidator(val);
	ui.maxYEdit->setValidator(val);
	ui.xSizeEdit->setValidator(val);
	ui.ySizeEdit->setValidator(val);

	connect(this, SIGNAL(accepted()), this, SLOT(onAccepted()));
}

NewGrid::~NewGrid()
{

}


void NewGrid::on_radioImport_toggled(bool)
{
	ui.importEdit->setEnabled(true);
	ui.importButton->setEnabled(true);
	ui.createEdit->setEnabled(false);
	ui.createButton->setEnabled(false);
	ui.defineCooBox->setEnabled(false);
	ui.paramBox->setEnabled(false);
	ui.okButton->setEnabled(validGridFile);
}

void NewGrid::on_radioCreate_toggled(bool)
{
	ui.importEdit->setEnabled(false);
	ui.importButton->setEnabled(false);
	ui.defineCooBox->setEnabled(true);
	ui.paramBox->setEnabled(true);
	ui.createEdit->setEnabled(true);
	ui.createButton->setEnabled(true);
	ui.okButton->setEnabled(validDataFile);
}

void NewGrid::on_importButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open grid file"),
                                                 ui.importEdit->text(),
                                                 tr("Surfer grid (*.grd)"));
	if (!fileName.isEmpty())
	{
		validGridFile = true;
		ui.okButton->setEnabled(true);
		ui.importEdit->setText(fileName);
	}
}

void NewGrid::on_createButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open dat-file"),
                                                 ui.createEdit->text(),
                                                 tr("Data files (*.dat);;All files(*)"));
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	QString line = in.readLine();
	QStringList headlist = line.split("	");
	if (headlist.size()>1)
	{
		if (validDataFile)
		{
			disconnect(ui.xCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComboChanged()));
			disconnect(ui.yCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComboChanged()));
			disconnect(ui.minXEdit, SIGNAL(textEdited(QString)), this, SLOT(defineXCellSize()));
			disconnect(ui.maxXEdit, SIGNAL(textEdited(QString)), this, SLOT(defineXCellSize()));
			disconnect(ui.minYEdit, SIGNAL(textEdited(QString)), this, SLOT(defineYCellSize()));
			disconnect(ui.maxYEdit, SIGNAL(textEdited(QString)), this, SLOT(defineYCellSize()));
			disconnect(ui.xSizeEdit, SIGNAL(textEdited(QString)), this, SLOT(on_xSizeEdited()));
			disconnect(ui.ySizeEdit, SIGNAL(textEdited(QString)), this, SLOT(on_ySizeEdited()));
			disconnect(ui.xSizeEdit, SIGNAL(editingFinished()), this, SLOT(finishedXCellSize()));
			disconnect(ui.ySizeEdit, SIGNAL(editingFinished()), this, SLOT(finishedYCellSize()));
			disconnect(ui.numXSpin, SIGNAL(valueChanged(int)), this, SLOT(defineXCellSize()));
			disconnect(ui.numYSpin, SIGNAL(valueChanged(int)), this, SLOT(defineYCellSize()));
		}
		validDataFile = true;
		ui.createEdit->setText(fileName);
		limits.clear();
		dataTable.clear();
		vector <double> headline;
		QString letters[16]={"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P"} ;
		ui.xCombo->clear();
		ui.yCombo->clear();
		ui.zCombo->clear();
		ui.okButton->setEnabled(true);
		int isHeadline=0;
		minmax minmaxBuf;
		for (int i = 0; i < headlist.size(); ++i)
		{
			limits.push_back(minmaxBuf);
			headlist[i].replace(" ","");
			bool ok;
			headline.push_back(headlist.at(i).toDouble(&ok));
			if (!ok)
			{
				ui.xCombo->addItem(tr("Column ")+letters[i]+": "+headlist.at(i));
				ui.yCombo->addItem(tr("Column ")+letters[i]+": "+headlist.at(i));
				ui.zCombo->addItem(tr("Column ")+letters[i]+": "+headlist.at(i));
				isHeadline--;
			}
			else
			{
				ui.xCombo->addItem(tr("Column ")+letters[i]);
				ui.yCombo->addItem(tr("Column ")+letters[i]);
				ui.zCombo->addItem(tr("Column ")+letters[i]);
			}
		}
		int iterForMinMax = 0;
		if (isHeadline>=-2)
		{
			dataTable.push_back(headline);
			iterForMinMax++;
			for (int i = 0; i < headlist.size(); ++i)
			{
				limits[i].min = headline[i];
				limits[i].max = headline[i];
			}
		}
		while (!in.atEnd())
		{
			iterForMinMax++;
			line = in.readLine();
			QStringList list = line.split("	");
			vector <double> dataline;			  

			bool ok;

			while (limits.size()<list.size())
				limits.push_back(minmaxBuf);
			for (int i = 0; i < list.size(); i++)
			{
				list[i].replace(" ","");
				dataline.push_back(list.at(i).toDouble(&ok));
				if (iterForMinMax==1)
				{
					limits[i].min = dataline[i];
					limits[i].max = dataline[i];
				}
				else
					MinMax(dataline[i], limits[i].min, limits[i].max);
			}
			dataTable.push_back(dataline);
		}
		connect(ui.xCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComboChanged()));
		connect(ui.yCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ComboChanged()));
		connect(ui.minXEdit, SIGNAL(textEdited(QString)), this, SLOT(defineXCellSize()));
		connect(ui.maxXEdit, SIGNAL(textEdited(QString)), this, SLOT(defineXCellSize()));
		connect(ui.minYEdit, SIGNAL(textEdited(QString)), this, SLOT(defineYCellSize()));
		connect(ui.maxYEdit, SIGNAL(textEdited(QString)), this, SLOT(defineYCellSize()));
		connect(ui.xSizeEdit, SIGNAL(textEdited(QString)), this, SLOT(on_xSizeEdited()));
		connect(ui.ySizeEdit, SIGNAL(textEdited(QString)), this, SLOT(on_ySizeEdited()));
		connect(ui.xSizeEdit, SIGNAL(editingFinished()), this, SLOT(finishedXCellSize()));
		connect(ui.ySizeEdit, SIGNAL(editingFinished()), this, SLOT(finishedYCellSize()));
		connect(ui.numXSpin, SIGNAL(valueChanged(int)), this, SLOT(defineXCellSize()));
		connect(ui.numYSpin, SIGNAL(valueChanged(int)), this, SLOT(defineYCellSize()));
		ui.yCombo->setCurrentIndex(1);
		ui.zCombo->setCurrentIndex(2);
		ui.numXSpin->setMinimum(1);
		ui.numYSpin->setMinimum(1);
	}
	else
	{
		QMessageBox msgBox(QMessageBox::Warning, tr("Error"),
                       tr("Wrong file format!"), 0, this);
		msgBox.addButton(tr("Ok"), QMessageBox::AcceptRole);
		if (msgBox.exec() == QMessageBox::AcceptRole);
	}
}

void NewGrid::on_ComboChanged()
{
	int fieldX = ui.xCombo->currentIndex();
	int fieldY = ui.yCombo->currentIndex();
	minX = limits[fieldX].min;
	maxX = limits[fieldX].max;
	minY = limits[fieldY].min;
	maxY = limits[fieldY].max;

	QString str;
	str.setNum(minX);
	ui.minXEdit->setText(str);
	str.setNum(maxX);
	ui.maxXEdit->setText(str);
	str.setNum(minY);
	ui.minYEdit->setText(str);
	str.setNum(maxY);
	ui.maxYEdit->setText(str);

	if ((maxX-minX)>(maxY-minY))
	{
		ui.numXSpin->setValue(defaultCellNumber);
		ui.numYSpin->setValue(round((maxY-minY)/(maxX-minX)*defaultCellNumber));
	}
	else
	{
		ui.numXSpin->setValue(round((maxX-minX)/(maxY-minY)*defaultCellNumber));
		ui.numYSpin->setValue(defaultCellNumber);
	}
	defineXCellSize();
	defineYCellSize();
}

void NewGrid::defineXCellSize()
{
	if (!xSizeEditing)
	{
		minX = ui.minXEdit->text().toDouble();
		maxX = ui.maxXEdit->text().toDouble();
		numX = ui.numXSpin->value();
		QString str;
		str.setNum((maxX-minX)/(numX-1));
		ui.xSizeEdit->setText(str);
	}
}

void NewGrid::defineYCellSize()
{
	if (!ySizeEditing)
	{
		minY = ui.minYEdit->text().toDouble();
		maxY = ui.maxYEdit->text().toDouble();
		numY = ui.numYSpin->value();
		QString str;
		str.setNum((maxY-minY)/(numY-1));
		ui.ySizeEdit->setText(str);
	}
}

void NewGrid::on_xSizeEdited()
{
	xSizeEditing = true;
	xSize = ui.xSizeEdit->text().toDouble();
	minX = ui.minXEdit->text().toDouble();
	maxX = ui.maxXEdit->text().toDouble();
	ui.numXSpin->setValue(round((maxX-minX)/xSize)+1);
}


void NewGrid::on_ySizeEdited()
{
	ySizeEditing = true;
	ySize = ui.ySizeEdit->text().toDouble();
	minY = ui.minYEdit->text().toDouble();
	maxY = ui.maxYEdit->text().toDouble();
	ui.numYSpin->setValue(round((maxY-minY)/ySize)+1);
}

void NewGrid::finishedXCellSize()
{
	xSizeEditing = false;
	defineXCellSize();
}

void NewGrid::finishedYCellSize()
{
	ySizeEditing = false;
	defineYCellSize();
}

void NewGrid::onAccepted()
{
	import = ui.radioImport->isChecked();
	create = ui.radioCreate->isChecked();
	if (import)
	{
		fileName = ui.importEdit->text();
		invert = invertQuestion();
	}
	else if (create)
	{
		int fieldX = ui.xCombo->currentIndex();
		int fieldY = ui.yCombo->currentIndex();
		int fieldZ = ui.zCombo->currentIndex();
		finishedXCellSize();
		finishedYCellSize();
		createParams.objectName = ui.createEdit->text();
		createParams.xSize = ui.xSizeEdit->text().toDouble();
		createParams.ySize = ui.ySizeEdit->text().toDouble();
		createParams.minX = minX;
		createParams.minY = minY;
		createParams.numX = numX;
		createParams.numY = numY;
		int kInvert;
		if (invertQuestion())
			kInvert=-1;
		else
			kInvert=1;
		for (int i=0; i<dataTable.size(); i++)
			sourcePoints.push_back(MPoint(dataTable[i][fieldX], dataTable[i][fieldY], kInvert*dataTable[i][fieldZ]));
	}
}

bool NewGrid::invertQuestion()
{
	QString message= tr("<p>The file, probably, contains positive z coordinates" \
						 "<p>Do you wish to change the values to negatives?");
	QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                    message,
                                    QMessageBox::Yes | QMessageBox::No );
	if (reply == QMessageBox::Yes)
        return true;
    else
        return false;
}
