/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MABSTRACTVIEW_H
#define MABSTRACTVIEW_H

class Renderer;
class QWidget;
class TreeModel;

class MAbstractView
{
	public:
		MAbstractView(TreeModel *treeModel);
		virtual ~MAbstractView();
		
		virtual void draw() = 0;
		virtual Renderer *renderer() const = 0;
		virtual QWidget *widget() const = 0;
		TreeModel *treeModel() const { return m_treeModel; }

	private:
		TreeModel *m_treeModel;
};

#endif
