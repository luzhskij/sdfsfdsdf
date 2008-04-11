/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <GL/gl.h>
#include <math.h>
#include <QColor>

#include "glrenderer3d.h"
#include "mgrid.h"
#include "mwell.h"
#include "mmodel.h"
#include "mmodelproperty.h"
#include "mrgb.h"
#include "globals.h"

GLRenderer3D::GLRenderer3D()
{
}

GLRenderer3D::~GLRenderer3D()
{
}

void GLRenderer3D::render(MGrid* grid)
{
	grid->newGlList();

	if (grid->glDisplayList() != 0)
	{
		glNewList(grid->glDisplayList(), GL_COMPILE);
	
		GLfloat *vertices;
		GLfloat *colors;
		GLuint *indices;
		GLuint numOfQuads;
		UINT numX = grid->numX();
		UINT numY = grid->numY();

		vertices = new GLfloat[numX*numY*3];
		colors = new GLfloat[numX*numY*3];
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX; i++)
			{
				vertices[j*numX*3+i*3+0] = (GLfloat) grid->getX(i);
				vertices[j*numX*3+i*3+1] = (GLfloat) grid->getY(j);
				vertices[j*numX*3+i*3+2] = (GLfloat) grid->getZ(i, j);
			}
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX; i++)
			{
				MRGB color(grid->getZ(i, j), grid->minZ(), grid->maxZ());
				colors[j*numX*3+i*3+0] = color.r;
				colors[j*numX*3+i*3+1] = color.g;
				colors[j*numX*3+i*3+2] = color.b;
			}
		indices = new GLuint [(numX-1)*(numY-1)*4];
		numOfQuads = 0;
		for (UINT j = 0; j < numY-1; j++)
			for (UINT i = 0; i < numX-1; i++)
				if (!(grid->getZ(i, j)==undef || grid->getZ(i, j+1)==undef ||
						grid->getZ(i+1, j+1)==undef || grid->getZ(i+1, j)==undef))
				{
					indices[numOfQuads*4+0] = j*numX+i;
					indices[numOfQuads*4+1] = (j+1)*numX+i;
					indices[numOfQuads*4+2] = (j+1)*numX+(i+1);
					indices[numOfQuads*4+3] = j*numX+(i+1);
					numOfQuads++;
				}
		
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colors);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawElements(GL_QUADS, numOfQuads*4, GL_UNSIGNED_INT, indices);
			
		delete[] vertices;
		delete[] colors;
		delete[] indices;
		
		glEndList();
	}
}

void GLRenderer3D::render(MModelSkeleton *skeleton)
{
	skeleton->newGlList();

	if (skeleton->glDisplayList() != 0)
	{
		glNewList(skeleton->glDisplayList(), GL_COMPILE);
		GLfloat *vertices;
		GLuint *indices;
		GLuint numOfLines;
		MModel *model = skeleton->model();
		UINT numX = model->numX();
		UINT numY = model->numY();
		
		GLuint index = skeleton->glDisplayList();
		
		vertices = new GLfloat[numX*numY*3];
		UINT k = skeleton->layer();
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX; i++)
			{
				vertices[(j*numX+i)*3+0] = (GLfloat) model->getX(i);
				vertices[(j*numX+i)*3+1] = (GLfloat) model->getY(j);
				vertices[(j*numX+i)*3+2] = (GLfloat) model->getZ(i, j, k);
			}
	
		indices = new GLuint [2*((numX-1)*numY+(numY-1)*numX)];
		numOfLines = 0;
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX-1; i++)
				if (!(fabs(model->getZ(i, j, k))==undef || fabs(model->getZ(i+1, j, k))==undef))
				{
					indices[numOfLines*2+0] = j*numX+i;
					indices[numOfLines*2+1] = j*numX+(i+1);
					numOfLines++;
				}
		for (UINT j = 0; j < numY-1; j++)
			for (UINT i = 0; i < numX; i++)
				if (!(fabs(model->getZ(i, j, k))==undef || fabs(model->getZ(i, j+1, k))==undef))
				{
					indices[numOfLines*2+0] = j*numX+i;
					indices[numOfLines*2+1] = (j+1)*numX+i;
					numOfLines++;
				}
		GLubyte red = skeleton->color().red();
		GLubyte green = skeleton->color().green();
		GLubyte blue = skeleton->color().blue();
		glColor3ub(red, green, blue);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawElements(GL_LINES, numOfLines*2, GL_UNSIGNED_INT, indices);

		delete[] vertices;
		delete[] indices;

		glEndList();
	}
}

void GLRenderer3D::render(MWell* well)
{
	well->newGlList();

	if (well->glDisplayList() != 0)
	{
		glNewList(well->glDisplayList(), GL_COMPILE);
		
		GLuint index = well->glDisplayList();
		
		GLuint wellSize = well->nodes().size();
		GLfloat *vertices = new GLfloat[wellSize*3];
		GLuint *indices = new GLuint[wellSize];
		
		for (GLuint i = 0; i < wellSize; i++)
		{
			vertices[i*3+0] = (GLfloat) well->nodes().at(i).x;
			vertices[i*3+1] = (GLfloat) well->nodes().at(i).y;
			vertices[i*3+2] = (GLfloat) well->nodes().at(i).z;
			indices[i] = i;
		}
		GLubyte red = well->color().red();
		GLubyte green = well->color().green();
		GLubyte blue = well->color().blue();
		glColor3ub(red, green, blue);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawElements(GL_LINE_STRIP, wellSize, GL_UNSIGNED_INT, indices);
		
		delete[] vertices;
		delete[] indices;
		
		glEndList();
	}
}
void GLRenderer3D::render(MModelDiscreteProperty* property)
{
	property->newGlList();

	if (property->glDisplayList() != 0)
	{
		glNewList(property->glDisplayList(), GL_COMPILE);

		GLuint numOfQuads = 0;
		GLuint numOfVertices = 0;
		MModel *model = property->model();
		UINT numX = model->numX();
		UINT numY = model->numY();
		UINT numZ = model->numZ();

		
		GLfloat *vertices = new GLfloat[numZ*numX*numY*6*3];
		GLubyte *colors = new GLubyte[numZ*numX*numY*6*3];
		GLuint *indices = new GLuint[numZ*numX*numY*6*3];
		
		QColor colorArray[3];
		colorArray[1]= QColor::fromRgb(255, 219, 88);
		colorArray[2]= QColor::fromRgb(128, 128, 128);
		for (UINT k = 0; k < numZ-1; k++)
			for (UINT j = 0; j < numY-1; j++)
				for (UINT i = 0; i < numX-1; i++)
					if (property->cellNoCheck(i, j, k) != 0)
					{
						unsigned char cell = 0;
						GLuint begVert = numOfVertices;
						cell = property->cellNoCheck(i, j, k);
						bool 	isUp = false,
								isDown = false,
								isLeft = false,
								isRight = false;

						if (property->cell(i, j, k-1) == 0) // up side
						{
							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k);

							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;

							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k);

							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;

							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k);

							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;

							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k);
							
							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;

							indices[numOfQuads*4+0] = numOfVertices-4;
							indices[numOfQuads*4+1] = numOfVertices-3;
							indices[numOfQuads*4+2] = numOfVertices-2;
							indices[numOfQuads*4+3] = numOfVertices-1;
							numOfQuads++;
							
							isUp = true;
						}

						if (property->cell(i, j, k+1) == 0) // down side
						{
							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k+1);
							
							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;
							
							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k+1);
							
							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;
							
							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k+1);
							
							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;
							
							vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
							vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
							vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k+1);
							
							colors[numOfVertices*3+0] = colorArray[cell].red();
							colors[numOfVertices*3+1] = colorArray[cell].green();
							colors[numOfVertices*3+2] = colorArray[cell].blue();
							numOfVertices++;

							indices[numOfQuads*4+0] = numOfVertices-4;
							indices[numOfQuads*4+1] = numOfVertices-3;
							indices[numOfQuads*4+2] = numOfVertices-2;
							indices[numOfQuads*4+3] = numOfVertices-1;
							numOfQuads++;
							
							isDown = true;
						}

						if (property->cell(i-1, j, k) == 0) // left side
						{
							if (isUp)
							{
								indices[numOfQuads*4+0] = begVert+0;
								indices[numOfQuads*4+1] = begVert+1;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+5;
									indices[numOfQuads*4+3] = begVert+4;
								}
							}
							else
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
							
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								
								indices[numOfQuads*4+0] = numOfVertices-2;
								indices[numOfQuads*4+1] = numOfVertices-1;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+1;
									indices[numOfQuads*4+3] = begVert+0;
								}
							}

							if (!isDown)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k+1);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k+1);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								
								indices[numOfQuads*4+2] = numOfVertices-2;
								indices[numOfQuads*4+3] = numOfVertices-1;
							}
							numOfQuads++;
							
							isLeft = true;
						}

						if (property->cell(i+1, j, k) == 0) // right side
						{
							if (isUp)
							{
								indices[numOfQuads*4+0] = begVert+3;
								indices[numOfQuads*4+1] = begVert+2;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+6;
									indices[numOfQuads*4+3] = begVert+7;
								}
							}
							else
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
							
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k);
							
								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;

								indices[numOfQuads*4+0] = numOfVertices-2;
								indices[numOfQuads*4+1] = numOfVertices-1;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+2;
									indices[numOfQuads*4+3] = begVert+3;
								}
							}

							if (!isDown)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;

								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;

								indices[numOfQuads*4+2] = numOfVertices-2;
								indices[numOfQuads*4+3] = numOfVertices-1;
							}
							numOfQuads++;
							
							isRight = true;
						}

						if (property->cell(i, j-1, k) == 0) // front side
						{
							if (isUp)
							{
								indices[numOfQuads*4+0] = begVert+0;
								indices[numOfQuads*4+1] = begVert+3;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+7;
									indices[numOfQuads*4+3] = begVert+4;
								}
								else //--!isDown
								{
									if (isLeft)
									{
										indices[numOfQuads*4+3] = begVert+5;
										if (isRight)
											indices[numOfQuads*4+2] = begVert+7;
									}
									else //--!isLeft
										if (isRight)
											indices[numOfQuads*4+2] = begVert+5;
								}
							}
							else //---!isUp
							{
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+3;
									indices[numOfQuads*4+3] = begVert+0;
									if(isLeft)
									{
										indices[numOfQuads*4+0] = begVert+4;
										if(isRight)
											indices[numOfQuads*4+1] = begVert+6;
									}
									else
										if(isRight)
											indices[numOfQuads*4+1] = begVert+4;
								}
								else //--!isDown
								{
									if (isLeft)
									{
										indices[numOfQuads*4+0] = begVert+0;
										indices[numOfQuads*4+3] = begVert+3;
										if (isRight)
										{
											indices[numOfQuads*4+1] = begVert+4;
											indices[numOfQuads*4+2] = begVert+7;
										}
									}
									else //--!isLeft
										if (isRight)
										{
											indices[numOfQuads*4+1] = begVert+0;
											indices[numOfQuads*4+2] = begVert+3;
										}
								}
							}
							//check conditions - if no condition satisfyable for the node - then add vertex
							if (!isUp && !isLeft)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+0] = numOfVertices-1;
							}
							if (!isUp && !isRight)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+1] = numOfVertices-1;
							}
							if (!isDown && !isRight)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+2] = numOfVertices-1;
							}
							if (!isDown && !isLeft)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+3] = numOfVertices-1;
							}
							numOfQuads++;
						}

						if (property->cell(i, j+1, k) == 0) // back side
						{
							if (isUp)
							{
								indices[numOfQuads*4+0] = begVert+1;
								indices[numOfQuads*4+1] = begVert+2;
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+6;
									indices[numOfQuads*4+3] = begVert+5;
								}
								else //--!isDown
								{
									if (isLeft)
									{
										indices[numOfQuads*4+3] = begVert+4;
										if (isRight)
											indices[numOfQuads*4+2] = begVert+6;
									}
									else //--!isLeft
										if (isRight)
											indices[numOfQuads*4+2] = begVert+4;
								}
							}
							else //---!isUp
							{
								if (isDown)
								{
									indices[numOfQuads*4+2] = begVert+2;
									indices[numOfQuads*4+3] = begVert+1;
									if(isLeft)
									{
										indices[numOfQuads*4+0] = begVert+5;
										if(isRight)
											indices[numOfQuads*4+1] = begVert+7;
									}
									else
										if(isRight)
											indices[numOfQuads*4+1] = begVert+5;
								}
								else //--!isDown
								{
									if (isLeft)
									{
										indices[numOfQuads*4+0] = begVert+1;
										indices[numOfQuads*4+3] = begVert+2;
										if (isRight)
										{
											indices[numOfQuads*4+1] = begVert+5;
											indices[numOfQuads*4+2] = begVert+6;
										}
									}
									else //--!isLeft
										if (isRight)
										{
											indices[numOfQuads*4+1] = begVert+1;
											indices[numOfQuads*4+2] = begVert+2;
										}
								}
							}
							//check conditions - if no condition satisfyable for the node - then add vertex
							if (!isUp && !isLeft)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+0] = numOfVertices-1;
							}
							if (!isUp && !isRight)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+1] = numOfVertices-1;
							}
							if (!isDown && !isRight)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i+1);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i+1, j+1, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+2] = numOfVertices-1;
							}
							if (!isDown && !isLeft)
							{
								vertices[numOfVertices*3+0] = (GLfloat) model->getX(i);
								vertices[numOfVertices*3+1] = (GLfloat) model->getY(j+1);
								vertices[numOfVertices*3+2] = (GLfloat) model->getZ(i, j+1, k+1);

								colors[numOfVertices*3+0] = colorArray[cell].red();
								colors[numOfVertices*3+1] = colorArray[cell].green();
								colors[numOfVertices*3+2] = colorArray[cell].blue();
								numOfVertices++;
								indices[numOfQuads*4+3] = numOfVertices-1;
							}
							numOfQuads++;
						}
					}

		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawElements(GL_QUADS, numOfQuads*4, GL_UNSIGNED_INT, indices);

		delete[] vertices;
		delete[] colors;
		delete[] indices;

		glEndList();
	}
}

void GLRenderer3D::render(MPoints* points)
{

}

void GLRenderer3D::draw(MObject* object)
{
	if (object->glDisplayList() != 0)
		glCallList(object->glDisplayList());
}

void GLRenderer3D::render(MModel* model)
{
	model->newGlList();

	if (model->glDisplayList() != 0)
	{
		glNewList(model->glDisplayList(), GL_COMPILE);	
	
	GLfloat *vertices;
	GLuint *indices;
	GLuint numOfLines;
	UINT numX = model->numX();
	UINT numY = model->numY();
	UINT numZ = model->numZ();
	
	vertices = new GLfloat[numZ*numX*numY*3];
	for (UINT k = 0; k < numZ; k++)
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX; i++)
	{
		vertices[(k*numY*numX+j*numX+i)*3+0] = (GLfloat) model->getX(i);
		vertices[(k*numY*numX+j*numX+i)*3+1] = (GLfloat) model->getY(j);
		vertices[(k*numY*numX+j*numX+i)*3+2] = (GLfloat) model->getZ(i, j, k);
	}

	indices = new GLuint [2*numZ*((numX-1)*numY+(numY-1)*numX)];
	numOfLines = 0;
	for (UINT k = 0; k < numZ; k++)
	{
		for (UINT j = 0; j < numY; j++)
			for (UINT i = 0; i < numX-1; i++)
				if (!(fabs(model->getZ(i, j, k))==undef || fabs(model->getZ(i+1, j, k))==undef))
		{
			indices[numOfLines*2+0] = k*numY*numX+j*numX+i;
			indices[numOfLines*2+1] = k*numY*numX+j*numX+(i+1);
			numOfLines++;
		}
		for (UINT j = 0; j < numY-1; j++)
			for (UINT i = 0; i < numX; i++)
				if (!(fabs(model->getZ(i, j, k))==undef || fabs(model->getZ(i, j+1, k))==undef))
		{
			indices[numOfLines*2+0] = k*numY*numX+j*numX+i;
			indices[numOfLines*2+1] = k*numY*numX+(j+1)*numX+i;
			numOfLines++;
		}
	}
	glColor3f(1.0, 1.0, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_LINES, numOfLines*2, GL_UNSIGNED_INT, indices);
		
	delete[] vertices;
	delete[] indices;
	
		glEndList();
	}
}
