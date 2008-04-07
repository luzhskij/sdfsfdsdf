/*
 * This file is part of the Geoid and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#include "mprogressbartracker.h"

MProgressBarTracker::MProgressBarTracker()
{
	m_progressBar = new QProgressBar();
	m_progressBar->setMinimum(0);
	m_progressBar->setMaximum(100);
	m_progressBar->setValue(0);
}


MProgressBarTracker::~MProgressBarTracker()
{
	delete m_progressBar;
}

void MProgressBarTracker::setValue(int value)
{
	m_progressBar->setValue(value);
}

void MProgressBarTracker::initiate()
{
	m_progressBar->setValue(0);
}
