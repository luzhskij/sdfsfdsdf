/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <math.h>

#include "mgrid.h"
#include "mmodel.h"
#include "renderer.h"
#include "globals.h"
#include "math.h"
#include "mmodelproperty.h"

MModel::MModel ( MGrid *surface, GeometryType type, int numZ, double layerPower ) :
		m_numX ( surface->numX() ),
		m_numY ( surface->numY() ),
		m_numZ ( numZ ),
		m_xSize ( surface->xSize() ),
		m_ySize ( surface->ySize() ),
		m_modelMinX ( surface->gMinX() ),
		m_modelMinY ( surface->gMinY() )
{
	m_skeleton = new double [m_numX*m_numY*m_numZ];
	for ( int k=0; k<m_numZ; k++ )
		for ( int j=0; j<m_numY; j++ )
			for ( int i=0; i<m_numX; i++ )
				if ( surface->getZ ( i, j ) != undef )
					m_skeleton[m_numY*m_numX*k+m_numX*j+i] = surface->getZ ( i, j )-layerPower*k;
				else
					m_skeleton[m_numY*m_numX*k+m_numX*j+i] = undef;

	m_minX = m_modelMinX;
	m_minY = m_modelMinY;
	m_maxX = surface->gMaxX();
	m_maxY = surface->gMaxY();
	m_minZ = surface->minZ()-layerPower* ( m_numZ-1 );
	m_maxZ = surface->maxZ();
	
	construct();
}

MModel::MModel ( MGrid *top, MGrid *bot, int numZ, double layerPower )
{

}

void MModel::construct()
{
	setObjectName(QObject::tr("Model"));
	appendChild( new MModelSkeletonContainer(this) );
	appendChild( new MModelPropertiesContainer(this) );
}

MModel::~MModel()
{
	delete[] m_skeleton;
}

void MModel::calculateBorders ( BorderStruct &borders )
{
	MObject::calculateBorders ( borders );
}

void MModel::render(Renderer *renderer)
{
//	renderer->render( this );
	MFolder::render(renderer);
}
void MModel::draw(Renderer *renderer)
{
//	renderer->draw( this );
	MFolder::draw(renderer);
}

/* --------------------------------------------------------------------------------------
						MModelSkeletonContainer
   --------------------------------------------------------------------------------------*/

MModelSkeletonContainer::MModelSkeletonContainer ( MModel *model )
{
	appendChild ( new MModelSkeleton ( model, MModelSkeleton::Top ) );
	if (model->numZ()>2)
		appendChild ( new MModelSkeleton ( model, MModelSkeleton::Middle ) );
	appendChild ( new MModelSkeleton ( model, MModelSkeleton::Bottom ) );

	setObjectName( QObject::tr("Skeleton") );
}

MModelSkeletonContainer::~MModelSkeletonContainer()
{

}

/* --------------------------------------------------------------------------------------
						MModelSkeleton
   --------------------------------------------------------------------------------------*/

MModelSkeleton::MModelSkeleton ( MModel *model, SkeletonType skeletonType ) :
	m_skeletonType(skeletonType), m_model(model)
{
	switch ( skeletonType )
	{
		case Top:
			m_layer = 0;
			setObjectName( QObject::tr("Top") );
			setColor( QColor(30, 144, 255) );
			break;
		case Middle:
			m_layer = (int) round ((m_model->numZ()-1)/2);
			setObjectName( QObject::tr("Middle") );
			setColor( QColor(0, 255, 127) );
			break;
		case Bottom:
			m_layer = m_model->numZ()-1;
			setObjectName( QObject::tr("Bottom") );
			setColor( QColor(204, 255, 0) );
			break;
	}
}

MModelSkeleton::~MModelSkeleton()
{

}

void MModelSkeleton::render( Renderer* renderer )
{
	renderer->render( this );
}

void MModelSkeleton::draw( Renderer* renderer )
{
	renderer->draw( this );
}

/* --------------------------------------------------------------------------------------
						MModelPropertiesContainer
   --------------------------------------------------------------------------------------*/

MModelPropertiesContainer::MModelPropertiesContainer(MModel *model) :
		m_model(model)
{
	appendChild( new MModelDiscreteProperty(model, QObject::tr("facies") ) );
	setObjectName( QObject::tr("Properties") );
}

MModelPropertiesContainer::~MModelPropertiesContainer()
{

}

/*	switch(type){
              case followTop:

              	break;
              case followBottom:
              	break;
              }
              */
              /*
              {
              		minX = top_.minX>=bot_.minX ? top_.minX : bot_.minX;  // Определение параметров сетки
              		maxX = top_.maxX<=bot_.maxX ? top_.maxX : bot_.maxX;
              		minY = top_.minY>=bot_.minY ? top_.minY : bot_.minY;
              		maxX = top_.maxY<=bot_.maxY ? top_.maxY : bot_.maxY;
              		bool scaleX = (top_.minX-bot_.minX)/top_.xSize==floor((top_.minX-bot_.minX)/top_.xSize);
              		bool scaleY = (top_.minY-bot_.minY)/top_.ySize==floor((top_.minY-bot_.minY)/top_.ySize);
              		if ((top_.xSize==bot_.xSize) && (top_.ySize==bot_.ySize) && scaleX && scaleY) // проверка равентства геометрии сетки
              {
              			xSize = top_.xSize;
              			ySize = top_.ySize;
              			numX = (maxX-minX)/xSize;
              			numY = (maxY-minY)/ySize;
              			numZ = _numZ;

              			skeleton = new double [numX*numY*numZ];
              			top = new double[(numX+1)*(numY+1)];
              			bot = new double[(numX+1)*(numY+1)];

              			int ditop, djtop, dibot, djbot;
              			if (minX == top_.minX)
              {
              				ditop = 0;
              				dibot = (top_.minX-bot_.minX)/xSize;
              }
              			else
              {
              				dibot = 0;
              				ditop = (bot_.minX-top_.minX)/xSize;
              }
              			if (minY == top_.minY)
              {
              				djtop = 0;
              				djbot = (top_.minY-bot_.minY)/ySize;
              }
              			else
              {
              				djbot = 0;
              				djtop = (bot_.minY-top_.minY)/ySize;
              }
              			for (int i=0; i<numX+1; i++)
              				for (int j=0; j<numY+1; j++)
              {
              					top[(numX+1)*j+i]=top_.getZ(i + ditop, j + djtop);
              					bot[(numX+1)*j+i]=bot_.getZ(i + dibot, j + djbot);
              }
              }
              }
              */
