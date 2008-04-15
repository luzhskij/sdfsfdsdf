/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef NEWGRID_H
#define NEWGRID_H

#include <QDialog>

#include <vector>
using std::vector;

#include "mgrid.h"
#include "ui_newgrid.h"
#include "globals.h"
#include "mpoint.h"

struct minmax
{
	double min, max;
};

const double defaultCellNumber = 100;

class NewGrid : public QDialog
{
	Q_OBJECT

public:
	NewGrid(QWidget *parent = 0);
	~NewGrid();

private:
	Ui::NewGridClass ui;

	double minX, maxX, minY, maxY, xSize, ySize;
	int numX, numY;
	bool validDataFile, validGridFile;
	bool xSizeEditing, ySizeEditing;
	vector< vector<double> > dataTable;
	vector<minmax> limits;

private:
	bool invertQuestion();

public:
	GridParams createParams;
	QString fileName;
	bool invert, import, create;
	vector<MPoint> sourcePoints;

private slots:
	void on_createButton_clicked();
	void on_importButton_clicked();
	void on_radioImport_toggled(bool);
	void on_radioCreate_toggled(bool);
	void onComboChanged();
	void defineXCellSize();
	void defineYCellSize();
	void onXSizeEdited();
	void onYSizeEdited();
	void finishedXCellSize();
	void finishedYCellSize();
	void onAccepted();
};

#endif // NEWGRID_H
