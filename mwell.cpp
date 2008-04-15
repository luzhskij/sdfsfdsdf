/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <QTextStream>
#include <QFile>

#include "mwell.h"
#include "renderer.h"
#include "mcolorgenerator.h"

MWell::MWell(QString name) :
		MObject(name)
{

}

MWell::~MWell()
{

}

void MWell::finalize()
{
	if (m_wellNodes.last().z > 0)
		for (int i = 0; i<m_wellNodes.size(); i++)
			m_wellNodes[i].z = -m_wellNodes[i].z;
	reCalculateBorders();
}

void MWell::render( Renderer* renderer )
{
	renderer->render( this );
}

void MWell::draw( Renderer* renderer )
{
	renderer->draw( this );
}

void MWell::reCalculateBorders()
{
	m_minX = m_wellNodes[0].x;
	m_maxX = m_wellNodes[0].x;
	m_minY = m_wellNodes[0].y;
	m_maxY = m_wellNodes[0].y;
	m_minZ = m_wellNodes[0].z;
	m_maxZ = m_wellNodes[0].z;
	for (int i = 1; i<m_wellNodes.size(); i++)
	{
		m_minX = m_minX < m_wellNodes[i].x ? m_minX : m_wellNodes[i].x;
		m_maxX = m_maxX > m_wellNodes[i].x ? m_maxX : m_wellNodes[i].x;
		m_minY = m_minY < m_wellNodes[i].y ? m_minY : m_wellNodes[i].y;
		m_maxY = m_maxY > m_wellNodes[i].y ? m_maxY : m_wellNodes[i].y;
		m_minZ = m_minZ < m_wellNodes[i].z ? m_minZ : m_wellNodes[i].z;
		m_maxZ = m_maxZ > m_wellNodes[i].z ? m_maxZ : m_wellNodes[i].z;
	}
}

void MWell::addNode(WellNode node)
{
	m_wellNodes.append(node);
	if ( m_wellNodes.size() == 1 )
	{
		m_minX = m_wellNodes[0].x;
		m_maxX = m_wellNodes[0].x;
		m_minY = m_wellNodes[0].y;
		m_maxY = m_wellNodes[0].y;
		m_minZ = m_wellNodes[0].z;
		m_maxZ = m_wellNodes[0].z;
	}
	else
	{
		m_minX = m_minX < m_wellNodes.last().x ? m_minX : m_wellNodes.last().x;
		m_maxX = m_maxX > m_wellNodes.last().x ? m_maxX : m_wellNodes.last().x;
		m_minY = m_minY < m_wellNodes.last().y ? m_minY : m_wellNodes.last().y;
		m_maxY = m_maxY > m_wellNodes.last().y ? m_maxY : m_wellNodes.last().y;
		m_minZ = m_minZ < m_wellNodes.last().z ? m_minZ : m_wellNodes.last().z;
		m_maxZ = m_maxZ > m_wellNodes.last().z ? m_maxZ : m_wellNodes.last().z;
	}
}

MFolder *MWell::readFromXYZ(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return 0;

	QTextStream in(&file);
	in.setCodec("CP-1251");
	
	MColorGenerator colorGenerator;
	
	MFolder *wellFolder = new MFolder(MObject::getFileName(fileName));
	MWell *newWell = NULL;
	QString line = in.readLine();
	while (!in.atEnd())
	{
		bool deleted = false;
		if (line.at(0) == QChar('*')) // checks whether first char at the line in is "*" which is a marker of a new well
		{
			newWell = new MWell(line.mid(1, line.length()-2));
			newWell->setColor(colorGenerator.getNewColor());
			line = in.readLine();
		}
		QStringList list = line.split(";");
		if (list.at(4) == "")	//checking whether altitude, i.e. z coordinate is empty - in this case we are not able to deal with the well, so skipping such wells
		{
			delete newWell;
			deleted = true;
			line = in.readLine();
			while (!in.atEnd() && line.at(0) != QChar('*'))	//checking for the new well
				line = in.readLine();
		}
		else	//filling well-object with geometry information
		{
			WellNode wellNode;
			wellNode.h = list.at(1).toDouble();
			wellNode.x = list.at(2).toDouble();
			wellNode.y = list.at(3).toDouble();
			wellNode.z = list.at(4).toDouble();
			newWell->addNode(wellNode);
			line = in.readLine();
		}
		
		if (in.atEnd())
		{
			if (!deleted)
			{
				newWell->finalize();
				wellFolder->appendChild(newWell);
			}
		}
		else
		{
			if ( line.at(0) == QChar('*') && !deleted )  // new well found - adding previous well to the list	
			{
				newWell->finalize();
				wellFolder->appendChild(newWell);
			}
		}
	}
	return wellFolder;
}
