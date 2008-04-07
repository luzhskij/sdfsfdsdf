/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

//
// C++ Implementation: mmodelproperty
//
// Description:
//
//
// Author: Eldar <eldarik@eldarik-desktop>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mmodelproperty.h"
#include "renderer.h"

MModelProperty::MModelProperty(MModel *model, QString name) :
		m_model(model),
		m_numX(model->numX()-1),
		m_numY(model->numY()-1),
		m_numZ(model->numZ()-1)
{
	setObjectName(name);
}


MModelProperty::~MModelProperty()
{
}

MModelDiscreteProperty::MModelDiscreteProperty(MModel *model, QString name) :
		MModelProperty(model, name)
{
	m_cells = new unsigned char [numX()*numY()*numZ()];
	
	for(int k=0; k<numZ(); k++)
		for(int j=0; j<numY(); j++)
			for(int i=0; i<numX(); i++)
				if( model->getZ(i, j, k) == undef || model->getZ(i+1, j, k) == undef || model->getZ(i+1, j+1, k) == undef || model->getZ(i, j+1, k) == undef || model->getZ(i, j, k+1) == undef || model->getZ(i+1, j, k+1) == undef || model->getZ(i+1, j+1, k+1) == undef || model->getZ(i, j+1, k+1) == undef)
					m_cells[numY()*numX()*k+numX()*j+i] = 0;
				else
					m_cells[numY()*numX()*k+numX()*j+i] = 1;

	srand(time(NULL));
	long size = numX()*numY()*numZ();
	for(long i=0; i<size; i++)
		if (m_cells[i] != 0)
//			m_cells[i] = rand()%3;
			m_cells[i] = 1+rand()%2;
}


MModelDiscreteProperty::~MModelDiscreteProperty()
{
	delete[] m_cells;
}

void MModelDiscreteProperty::render( Renderer* renderer )
{
	renderer->render( this );
}

void MModelDiscreteProperty::draw( Renderer* renderer )
{
	renderer->draw( this );
}
