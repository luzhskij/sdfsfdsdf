/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MFOLDER_H
#define MFOLDER_H

#include "mobject.h"

class MFolder : public MObject
{
	public:
		MFolder(QString name = QString());
		virtual ~MFolder();

		virtual void render(Renderer *renderer);
		virtual void draw(Renderer *renderer);
		virtual void calculateBorders(BorderStruct &borders);
		virtual ObjectType type() const { return MObject::Folder; }
		
		virtual void setChecked(bool checkState, bool recursive);
		virtual QIcon icon() const;
};

#endif
