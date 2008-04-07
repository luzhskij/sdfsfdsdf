/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MPOINT_H
#define MPOINT_H

class MPoint	
{
public:
	double x, y, z;	// Координаты точки

	//====== Конструктор по умолчанию
	MPoint () { x = y = z = 0; }
	//====== Конструктор с параметрами
	MPoint (double c1, double c2, double c3)
	{
		x = float(c1);
		y = float(c2);
		z = float(c3);
	}

	//====== Операция присвоения
	MPoint& operator=(const MPoint& pt)
	{
		x = pt.x;
		y = pt.y;
		z = pt.z;
		return *this;
	}
	
	//====== Операция сдвига в пространстве
	MPoint& operator+=(const MPoint& pt)
	{
		x += pt.x;
		y += pt.y;
		z += pt.z;
		return *this;
	}	

	//====== Конструктор копирования
	MPoint (const MPoint& pt)
	{
		*this = pt;
	}
};

#endif //MPOINT_H
