/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MGRID_H
#define MGRID_H

#include <QDataStream>
#include <GL/gl.h>

#include <vector>
using std::vector;

#include "mobject.h"
#include "mpoint.h"
//#include <GL/glext.h>

struct GridParams
{
	int numX, numY;
	double minX, minY, xSize, ySize;
	QString objectName;
};

class Renderer;
class MProgressBarTracker;

class MGrid	: public MObject
{

public:
	//====== Конструктор экспорта
	MGrid (QString fileName, bool invert);
	//====== Конструктор для расчета
	MGrid (GridParams& params);
	//Деструктор
	~MGrid();

	double getX(int i) const;
	double getY(int j) const;
	double getZ(int i, int j) const;
	int numX() const;
	int numY() const;
	double xSize() const;
	double ySize() const;
	double gMinX() const;
	double gMinY() const;
	double gMaxX() const;
	double gMaxY() const;
	double operator() (double _x, double _y) const;
	double *operator[] ( size_t i ) const;

	virtual void render(Renderer *renderer);
	virtual void draw(Renderer *renderer);
	virtual ObjectType type() const {return MObject::Grid;}

	static const int M_LINEAR = 1,
					 M_POWER = 2;

	void calculate(vector<MPoint> &data, int approx, double n, MProgressBarTracker &progressTracker);
	friend QDataStream &operator>>(QDataStream &in, MGrid &grid);

private:
	double calc(double src, double a, int approx, double n);
	double distance(double x1, double y1, double x2, double y2);
	
	double *grid;
	double m_xSize, m_ySize;
	long m_numX, m_numY;
	double m_gridMinX, m_gridMinY, m_gridMaxX, m_gridMaxY;
};

/*****************************************************************************
  MGrid inline functions
 *****************************************************************************/

inline int MGrid::numX() const
{ return m_numX; }

inline int MGrid::numY() const
{ return m_numY; }

inline double MGrid::xSize() const
{ return m_xSize; }

inline double MGrid::ySize() const
{ return m_ySize; }

inline double MGrid::gMinX() const
{ return m_gridMinX; }

inline double MGrid::gMinY() const
{ return m_gridMinY; }

inline double MGrid::gMaxX() const
{ return m_gridMaxX; }

inline double MGrid::gMaxY() const
{ return m_gridMaxY; }

inline double MGrid::getX(int i) const
{ return m_gridMinX+i*m_xSize; }

inline double MGrid::getY(int j) const
{ return m_gridMinY+j*m_ySize; }

inline double MGrid::getZ(int i, int j) const
{ return grid[j*m_numX+i]; }

inline double *MGrid::operator[] ( size_t i ) const
{ return &grid[i*m_numY]; }

#endif //MGRID_H
