/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include "mwell.h"
#include "renderer.h"

MWell::MWell(QString name) :
		MObject(name)
{

}


MWell::~MWell()
{

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
