/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MMODEL_H
#define MMODEL_H

#include "mfolder.h"

class MGrid;

class MModel : public MFolder
{

public:
	enum GeometryType { followTop, followBottom };
	MModel(MGrid *surface, GeometryType type, int numZ, double layerPower);
	MModel(MGrid *top, MGrid *bot, int numZ, double layerPower);
	~MModel();

	double getX(int i) const;
	double getY(int j) const;
	double getZ(int i, int j, int k) const;
	int numX() const;
	int numY() const;
	int numZ() const;

	virtual ObjectType type() const { return MObject::Model; }
	virtual void calculateBorders(BorderStruct &borders);
	
	virtual void render(Renderer *renderer);
	virtual void draw(Renderer *renderer);
	
private:
	void construct();

private:
	double *m_skeleton;
	double m_xSize, m_ySize;
	double m_modelMinX, m_modelMinY;
	int m_numX, m_numY, m_numZ;
};

class MModelSkeletonContainer : public MFolder
{
	public:
		MModelSkeletonContainer(MModel *model);
		~MModelSkeletonContainer();

		ObjectType type() const { return MObject::ModelSkeletonContainer; }
};

class MModelSkeleton : public MObject
{
	public:
		enum SkeletonType { Top, Middle, Bottom };

	public:
		MModelSkeleton(MModel *model, SkeletonType skeletonType);
		~MModelSkeleton();

		ObjectType type() const { return MObject::ModelSkeleton; }
		int layer() const { return m_layer; }
		MModel *model() const { return m_model; }

		virtual void render(Renderer*);
		virtual void draw(Renderer *renderer);

	private:
		int m_layer;
		SkeletonType m_skeletonType;
		MModel *m_model;
};

class MModelPropertiesContainer : public MFolder
{
	public:
		MModelPropertiesContainer(MModel *model);
		~MModelPropertiesContainer();

		ObjectType type() const { return MObject::ModelPropertiesContainer; }

	private:
		MModel *m_model;
};

/*****************************************************************************
  MModel inline functions
 *****************************************************************************/

inline int MModel::numX() const
{ return m_numX; }

inline int MModel::numY() const
{ return m_numY; }

inline int MModel::numZ() const
{ return m_numZ; }

inline double MModel::getX(int i) const
{ return m_modelMinX+i*m_xSize; }

inline double MModel::getY(int j) const
{ return m_modelMinY+j*m_ySize; }

inline double MModel::getZ(int i, int j, int k) const
{ return m_skeleton[m_numY*m_numX*k+m_numX*j+i]; }

#endif // MMODEL_H
