/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <QFileInfo>

#include "mobject.h"
#include "borderstruct.h"

MObject::MObject(QString name) :
	m_glDisplayList(0)
{
	setObjectName(name);
}

MObject::~MObject()
{
	flushGlList();
}

void MObject::flushGlList()
{
	if (m_glDisplayList)
	{
		glDeleteLists(m_glDisplayList, 1);
		m_glDisplayList = 0;
	}
}

void MObject::newGlList()
{
	flushGlList();
	m_glDisplayList = glGenLists(1);
}

QString MObject::getFileName(QString fileName)
{
	QFileInfo fileInfo(fileName);
	return fileInfo.fileName();
}

void MObject::calculateBorders(BorderStruct &borders)
{
	if (borders.atLeastOne)
	{
		borders.minX = minX() < borders.minX ? minX() : borders.minX;
		borders.maxX = maxX() > borders.maxX ? maxX() : borders.maxX;
		borders.minY = minY() < borders.minY ? minY() : borders.minY;
		borders.maxY = maxY() > borders.maxY ? maxY() : borders.maxY;
		borders.minZ = minZ() < borders.minZ ? minZ() : borders.minZ;
		borders.maxZ = maxZ() > borders.maxZ ? maxZ() : borders.maxZ;
	}
	else
	{
		borders.minX = minX();
		borders.maxX = maxX();
		borders.minY = minY();
		borders.maxY = maxY();
		borders.minZ = minZ();
		borders.maxZ = maxZ();
		borders.atLeastOne = true;
	}
}
