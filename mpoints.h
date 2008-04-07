/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MPOINTS_H
#define MPOINTS_H

#include <vector>
using std::vector;

#include "mobject.h"

class MPoint;

class MPoints : public MObject
{

public:
	MPoints(void);
	~MPoints(void);

private:
	vector <MPoint*> points;
	virtual void defineCoo();
};

#endif
