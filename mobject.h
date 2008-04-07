/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MOBJECT_H
#define MOBJECT_H

#include <QString>
#include <GL/gl.h>

#include "treeitem.h"

class Renderer;

class MObject : public TreeItem
{
	public:
		enum ObjectType { Grid, Polygon, Folder, Model, ModelSkeleton, ModelSkeletonContainer, ModelPropertiesContainer, ModelDiscreteProperty, ModelFlowProperty, Well };
	
		MObject(QString name = QString());
		virtual ~MObject();

		double minX() const;
		double maxX() const;
		double minY() const;
		double maxY() const;
		double minZ() const;
		double maxZ() const;
		GLuint glDisplayList() const { return m_glDisplayList; }
		void newGlList();
		//void setGlDisplayList(GLuint glDisplaylist) { m_glDisplayList = glDisplayList; }
		void flushGlList();

		virtual void render(Renderer*) = 0;
		virtual void draw(Renderer *renderer) = 0;
		virtual ObjectType type() const = 0;
		virtual void calculateBorders(BorderStruct &borders);
		
		static QString getFileName(QString fileName);

	protected:
		double m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ;
		GLuint m_glDisplayList;
};

/*****************************************************************************
  MObject inline functions
 *****************************************************************************/

inline double MObject::minX() const
{ return m_minX; }

inline double MObject::maxX() const
{ return m_maxX; }

inline double MObject::minY() const
{ return m_minY; }

inline double MObject::maxY() const
{ return m_maxY; }

inline double MObject::minZ() const
{ return m_minZ; }

inline double MObject::maxZ() const
{ return m_maxZ; }

#endif
