/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <cassert>
#include <QFile>
#include <math.h>

#include "mgrid.h"
#include "mrgb.h"
#include "renderer.h"
#include "globals.h"
#include "mprogressbartracker.h"

MGrid::MGrid (QString fileName, bool invert)
{
	setObjectName( getFileName(fileName) );
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);
	in.setByteOrder(QDataStream::LittleEndian);

	qint32 header, nX, nY;
	in >> header;
	in.skipRawData(4*4);
	in >> nY >> nX;
	m_numX = nX;
	m_numY = nY;
	in >> m_gridMinX >> m_gridMinY >> m_xSize >> m_ySize >> m_minZ >> m_maxZ;

	int kInv;
	if (invert)
	{
		kInv = -1;
		swap(m_minZ, m_maxZ);
		m_minZ = -m_minZ;
		m_maxZ = -m_maxZ;
	}
	else
		kInv = 1;

	m_gridMaxX = m_gridMinX+(m_numX-1)*m_xSize;
	m_gridMaxY = m_gridMinY+(m_numY-1)*m_ySize;

	in.skipRawData(24);
	grid = new double[m_numX*m_numY];
	for (int i = 0; i < m_numX*m_numY; i++)
	{
		in >> grid[i];
		if (grid[i]!=undef)
			grid[i] = kInv*grid[i];
	}
	
	m_minX = m_gridMinX;
	m_minY = m_gridMinY;
	m_maxX = m_gridMaxX;
	m_maxY = m_gridMaxY;
}

MGrid::MGrid (GridParams& params)
{
	setObjectName( getFileName( params.objectName ) );
	m_numX = params.numX;
	m_numY = params.numY;
	m_gridMinX = params.minX;
	m_gridMinY = params.minY;
	m_xSize = params.xSize;
	m_ySize = params.ySize;
	m_gridMaxX = m_gridMinX+(m_numX-1)*m_xSize;
	m_gridMaxY = m_gridMinY+(m_numY-1)*m_ySize;
	grid = new double[m_numX*m_numY];
	m_minX = m_gridMinX;
	m_minY = m_gridMinY;
	m_maxX = m_gridMaxX;
	m_maxY = m_gridMaxY;
}

MGrid::~MGrid()
{
	delete[] grid;
}

//Структуры для расчета параметров вариограммы

double MGrid::calc(double src, double a, int approx, double n)
{
	double res;
	switch (approx)
	{
		case 1:
			res = a*src;
			break;
		case 2:
			res = a*pow(src,n);
			break;
	}
	return res;
}

double MGrid::distance(double x1, double y1, double x2, double y2)
{
	return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
}

struct tab1
{
	double dist, semi_v;
	tab1 () { dist = semi_v = 0; }
};

struct tab2
{
	double semi_v;
	int n;
	tab2 () { semi_v = 0.f; n = 0; }
};

//Вариант без поиска - all data
void MGrid::calculate(vector<MPoint> &data, int approx, double n, MProgressBarTracker &progressTracker)
{
	double	co = 25;
	int		lags = 14;
	int		num_close_wells = 10;
	int		i,j,k;

    //Определение экспериментальной вариограммы
	int		size_s;
	for (j = 1, size_s = 0; j < data.size(); j++)
		size_s+=j;

	vector <tab1> table1(size_s);

	double maxD = 0;
	for (j = 0, k = 0; j < data.size(); j++)
	{
		for(i = j+1; i < data.size(); i++, k++)
		{
			table1[k].dist = distance(data[i].x, data[i].y, data[j].x, data[j].y);
			table1[k].semi_v = 0.5*pow((data[j].z-data[i].z), 2);
			if (maxD < table1[k].dist)
				maxD = table1[k].dist;
		}
	}

	double lag = maxD/(lags-1);	//определяем величину лага

	vector <tab2> table2(lags);	//таблица дисперсий по лагам

	for(i = 0; i < table1.size(); i++)
	{
		int iter = floor(table1[i].dist/lag);
		table2[iter].n++;
		table2[iter].semi_v += table1[i].semi_v;
	}
	for(i = 0; i<table2.size(); i++)
	{
		if (table2[i].n != 0)
			table2[i].semi_v = table2[i].semi_v/table2[i].n;
	}

	//Определение параметров вариограммы МНК
	double a;
	if (approx == 1)
	{
			double
				S_yx = 0,
				S_x2 = 0;
			for(i = 0; i < table2.size(); i++)
				if (table2[i].n != 0)
				{
					S_yx += (i+lag/2)*table2[i].semi_v;
					S_x2 += (i+lag/2)*(i+lag/2);
				}
			a = S_yx/S_x2;
	}
	if(approx == 2)
	{
			double
				S_yxn = 0,
				S_x2n = 0;
			for(i = 0; i < table2.size(); i++)
				if (table2[i].n != 0)
				{
					S_yxn += pow((i+lag/2),n)*table2[i].semi_v;
					S_x2n += pow((i+lag/2),2*n);
				}
			a = S_yxn/S_x2n;
	}

	//Расчет сетки
	int size = (UINT) data.size()+1; //размер с учетом добавления строки и столбца с единицами
	double **matrix = new double*[size];
	for (i = 0; i < size; i++)
		matrix[i] = new double[2*size];
	double **G = new double*[size];
	for (i = 0; i < size; i++)
		G[i] = new double[2*size];

	//Заполнение элементами матрицы
	for (i = 0; i < size-1; i++)
		for (j = 0; j < size-1; j++)
			G[i][j] = calc(distance(data[i].x, data[i].y, data[j].x, data[j].y), a, approx, n);
	for (i = 0; i < size-1; i++)
		G[size-1][i] = 1;
	for (i = 0; i < size-1; i++)
		G[i][size-1] = 1;
	G[size-1][size-1] = 0;

	//Добавляем единичную матрицу
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
		{
			if (i==j) G[i][j+size] = 1;
			else G[i][j+size] = 0;
		}

	//Переставляем строки
	for (i = 0; i < size; i++)
		if (i == size-1)
			for (j = 0; j < 2*size; j++)
				matrix[0][j] = G[size-1][j];
		else
			for (j = 0; j < 2*size; j++)
				matrix[i+1][j] = G[i][j];
	
	// Вычисления
	double coef;
	for (i = 0; i < size-1; i++)
		for (j = i+1; j < size; j++)
		{
			coef = matrix[j][i]/matrix[i][i];
			for (k = i+1; k < 2*size; k++)
				matrix[j][k] = matrix[j][k]-coef*matrix[i][k];
		}
	for (i = size-1; i > 0; i--)
		for (j = i-1; j >= 0; j--)
		{
			coef = matrix[j][i]/matrix[i][i];
			for (k = size; k < 2*size; k++)
				matrix[j][k] = matrix[j][k]-coef*matrix[i][k];
		}
	for (i = 0; i < size; i++)
		if (matrix[i][i] != 1)
			for (j = size; j < 2*size; j++)
				matrix[i][j] /= matrix[i][i];

	//Перенос в другой массив
	for (i = 0; i < size; i++)
		for (j = 0; j<size; j++)
			G[i][j] = matrix[i][j+size];

	m_minZ = m_maxZ = data[0].z;
	int currentProcent = 0;

	double *vec  = new double[size];
	double *vesa = new double[size];
	double x = 0,
		   y = 0,
		   z = 0;
	for (int i2 = 0; i2 < m_numX; i2++)
	{
		x = getX(i2);
		for (int j2 = 0; j2 < m_numY; j2++)
		{
			y = getY(j2);
			z = 0;

			//Вектор точки
			for (i = 0; i< size-1; i++)
				vec[i] = calc(distance(data[i].x, data[i].y, x, y), a, approx, n);
			vec[size-1] = 1;

			//Вектор весов
			for (i = 0; i < size; i++)
				vesa[i] = 0;
			for (i = 0; i < size; i++)
				for (j = 0; j < size; j++)
					vesa[i] += vec[j]*G[j][i];

			//z точки
			for (i = 0; i < size-1; i++)
				z += vesa[i]*data[i].z;

			MinMax(z, m_minZ, m_maxZ);
			grid[j2*m_numX+i2] = z;

			if (((i2*m_numY+j2)*100/((m_numX-1)*(m_numY-1))) > currentProcent)
				progressTracker.setValue(++currentProcent);
//				emit progressBarChanged(++currentProcent);
		}
	}

	delete[] vec;
	delete[] vesa;

	for (i=0; i<size; i++)
		delete matrix[i];
	delete[] matrix;
	for (i=0; i<size; i++)
		delete G[i];
	delete[] G;
}

void MGrid::render(Renderer* renderer)
{
	renderer->render(this);
}

void MGrid::draw(Renderer* renderer)
{
	renderer->draw(this);
}

QDataStream &operator>>(QDataStream &in, MGrid &grid)
{
	qint32 header, m_numX, m_numY;
	double m_gridMinX, m_gridMinY, m_xSize, m_ySize, m_minZ, m_maxZ;
	in >> header;
	in.skipRawData(4*4);
	in >> m_numY >> m_numX >> m_gridMinX >> m_gridMinY >> m_xSize >> m_ySize >> m_minZ >> m_maxZ;
	in.skipRawData(24);
	double *grd;
	grd = new double[m_numX*m_numY];
	for (int i=0; i<m_numX*m_numY; i++)
		in >> grd[0];
//	grid = MGrid(m_numX, m_numY, m_gridMinX, m_gridMinY, m_xSize, m_ySize, m_minZ, m_maxZ, grd);

	return in;
}

struct YrLine3D
{
	double k1, b1, k2, b2;
};

void Line3Dparams(MPoint &p1, MPoint &p2, YrLine3D &params)
{
	params.k1 = (p2.y-p1.y)/(p2.x-p1.x);
	params.b1 = p1.y-params.k1*p1.x;
	params.k2 = (p2.z-p1.z)/(p2.x-p1.x);
	params.b2 = p1.z-params.k2*p1.x;
}

void getZfromTriangle(MPoint &p1, MPoint &p2, MPoint &p3, MPoint &p0)
{
	double k1 = (p1.y-p0.y)/(p1.x-p0.x);
	double b1 = p1.y-k1*p1.x;
	double k2 = (p3.y-p2.y)/(p3.x-p2.x);
	double b2 = p3.y-k2*p3.x;
	MPoint p_;
	p_.x = (b2-b1)/(k1-k2);
	p_.y = k1*p_.x+b1;
	YrLine3D params;
	Line3Dparams(p2, p3, params);
	p_.z = params.k2*p_.x+params.b2;
	Line3Dparams(p_, p1, params);
	p0.z = params.k2*p0.x+params.b2;
}

double MGrid::operator ()(double _x, double _y) const
{
	assert((_x>m_gridMinX)&&(_x<m_gridMaxX)&&(_y>m_gridMinY)&&(_y<m_gridMaxY));
	int i = floor((_x-m_gridMinX)/m_xSize);
	int j = floor((_y-m_gridMinY)/m_ySize);
	double dx = _x - (m_gridMinX+i*m_xSize);
	double dy = _y - (m_gridMinY+j*m_ySize);
	if (dx==0)
		return getZ(i,j)+(getZ(i,j+1)-getZ(i,j))/(getY(j+1)-getY(j))*dy;
	if (dy==0)
		return getZ(i,j)+(getZ(i+1,j)-getZ(i,j))/(getX(i+1)-getX(i))*dx;
	MPoint p1(getX(i), getY(j), getZ(i, j));
	MPoint p2(getX(i), getY(j+1), getZ(i, j+1));
	MPoint p3(getX(i+1), getY(j), getZ(i+1, j));
	MPoint p4(getX(i+1), getY(j+1), getZ(i+1, j+1));
	MPoint p0(_x, _y, 0);
	if (dy<m_numY-m_numY/m_numX*dx)
		getZfromTriangle(p1, p2, p3, p0);
	else
		getZfromTriangle(p4, p2, p3, p0);
	return p0.z;
}
