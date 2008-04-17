/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MWELL_H
#define MWELL_H

#include <QVector>
#include "mfolder.h"

struct WellNode
{
	double x, y, z, h;
};

class MWell : public MObject
{
	public:
		MWell(QString name = QString());
		~MWell();
		
		void finalize();	//checks if well z-coordinates are not negative - if they are, inverts them

		virtual void render(Renderer*);
		virtual void draw(Renderer *renderer);
		virtual ObjectType type() const { return MObject::Well; }

		const QVector<WellNode> &nodes() const { return m_wellNodes; }
		void addNode(WellNode node);
		void reCalculateBorders();

	private:
		QVector<WellNode> m_wellNodes;
};

class MWellsDataContainer : public MFolder
{
	public:
		MWellsDataContainer(QString fileName);
		~MWellsDataContainer();
};

class MGlobalWellLogsFolder : public MFolder
{
	public:
		MGlobalWellLogsFolder();
		~MGlobalWellLogsFolder();
		
		virtual void render(Renderer *renderer) {}
		virtual void draw(Renderer *renderer) {}
		virtual void calculateBorders(BorderStruct &borders) {}
		
		virtual void setChecked(bool checkState, bool recursive);
};

#endif
