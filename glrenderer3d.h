/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef GLRENDERER3D_H
#define GLRENDERER3D_H

#include "renderer.h"

class GLRenderer3D : public Renderer
{
	public:
		GLRenderer3D();
		virtual ~GLRenderer3D();
		
		virtual void draw(MObject*);

		virtual void render(MGrid*);
		virtual void render(MWell*);
		virtual void render(MPoints*);
		virtual void render(MModelSkeleton*);
		virtual void render(MModelDiscreteProperty*);

		virtual void render(MModel*);
};

#endif
