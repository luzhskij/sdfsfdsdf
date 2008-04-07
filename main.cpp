/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include <QtGui/QApplication>
#include <QTranslator>

#include "modeler.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Modeler w;
	w.show();
/*
	QTranslator * qt_translator = new QTranslator; 
	if ( !qt_translator->load( "qt_ru.qm" ) ) 
	{
		delete qt_translator; 
		return false; 
	}
	a.installTranslator( qt_translator );
*/
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}
