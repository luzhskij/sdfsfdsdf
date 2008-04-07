/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef RENDERER_H
#define RENDERER_H

class MObject;
class MGrid;
class MWell;
class MPoints;
class MModel;
class MModelSkeleton;
class MModelDiscreteProperty;

class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	
	virtual void draw(MObject*) = 0;

	virtual void render(MGrid*) = 0;
	virtual void render(MWell*) = 0;
	virtual void render(MPoints*) = 0;
	virtual void render(MModelSkeleton*) = 0;
	virtual void render(MModelDiscreteProperty*) = 0;

	virtual void render(MModel*) = 0;
};

#endif
