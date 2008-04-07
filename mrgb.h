/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef M_RGB
#define M_RGB

class MRGB	
{
public:
	float r, g, b;	// Координаты точки

	//====== Конструктор по умолчанию
	MRGB () { r = g = b = 0; }
	//====== Конструктор с параметрами
	MRGB (float prop, float max, float min)
	{
		float p_r=(prop-min)/(max-min)*5;
		if (p_r<1) {
			r=1;
			g=p_r;
			b=0;}
		else
		if (p_r<2) {
			r=2-p_r;
			g=1;
			b=0;}
		else
		if (p_r<3) {
			r=0;
			g=1;
			b=p_r-2;}
		else
		if (p_r<4) {
			r=0;
			g=4-p_r;
			b=1;}
		else
		if (p_r<5) {
			r=p_r-4;
			g=0;
			b=1;}
	}


	//====== Операция присвоения
	MRGB& operator=(const MRGB& pt)
	{
		r = pt.r;
		g = pt.g;
		b = pt.b;
		return *this;
	}

	//====== Конструктор копирования
	MRGB (const MRGB& pt)
	{
		*this = pt;
	}

};

#endif // M_RGB
