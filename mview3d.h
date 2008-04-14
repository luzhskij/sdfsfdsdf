/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MVIEW3D_H
#define MVIEW3D_H

#include "mabstractview.h"
#include "borderstruct.h"

class GLRenderer3D;
class GLWidget3D;

class MView3D : public MAbstractView
{
	public:
		MView3D(TreeModel *treeModel);
		~MView3D();

		virtual void draw();
		virtual Renderer *renderer() const;
		virtual QWidget *widget() const;
		virtual void calculateBorders();
		
	private:
		GLRenderer3D *m_glRenderer3D;
		GLWidget3D *m_glWidget3D;
};

#endif
