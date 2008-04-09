/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include "mcolorgenerator.h"

MColorGenerator::MColorGenerator()
{
	//Predefined list of colors
	m_colors.append( QColor(3, 192, 60) );	// dark pastel green
	m_colors.append( QColor(204, 255, 0) );	// lime
	m_colors.append( QColor(255, 0, 0) );	// red
	m_colors.append( QColor(199, 21, 133) );	// red violet
	m_colors.append( QColor(205, 0, 205) );	// blue violet
	m_colors.append( QColor(0, 149, 182) );	//bondi blue
	m_colors.append( QColor(0, 0, 255) );	//blue
	m_colors.append( QColor(100, 255, 200) );	// modified aquamarine
	m_colors.append( QColor(204, 119, 34) );	// ochre
	m_colors.append( QColor(230, 230, 210) );	// dark cream

	srand(time(NULL));
}

MColorGenerator::~MColorGenerator()
{
}

QColor MColorGenerator::getNewColor()
{
	int vectorSize = m_colors.size();
	return m_colors.at(rand()%vectorSize);
}