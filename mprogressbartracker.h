/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef MPROGRESSBARTRACKER_H
#define MPROGRESSBARTRACKER_H

#include <QProgressBar>

class MProgressBarTracker {

public:
	MProgressBarTracker();
	~MProgressBarTracker();
	
	inline QProgressBar *progressBar() { return m_progressBar; }
	void setValue(int value);
	void initiate();

private:
	QProgressBar *m_progressBar;
};

#endif
