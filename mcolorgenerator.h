/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

/*
 * This class contains list of common colors, and give a random color from the list.
 */

#ifndef MCOLORGENERATOR_H
#define MCOLORGENERATOR_H

#include <QColor>
#include <QVector>

class MColorGenerator
{
	public:
		MColorGenerator();
		~MColorGenerator();
		
		QColor getNewColor();	// use this metod to get a random color from predifined list
	
	private:
		QVector<QColor> m_colors;
};

#endif
