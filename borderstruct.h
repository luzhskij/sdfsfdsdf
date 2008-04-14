/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef BORDERSTRUCT_H
#define BORDERSTRUCT_H
struct BorderStruct
{
	double minX, maxX, minY, maxY, minZ, maxZ;
	bool atLeastOne;
	
	void flush()
	{
		atLeastOne = false;
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
		minZ = 0;
		maxZ = 0;
	}
};

#endif
