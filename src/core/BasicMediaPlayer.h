/** Copyright (C) 2014, James Reneau.
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/


#ifndef __BASICMEDIAPLAYER_H
#define __BASICMEDIAPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFileInfo>
#include <QEventLoop>
#include <QTimer>

#include "Sleeper.h"

class BasicMediaPlayer : private QMediaPlayer
{
    Q_OBJECT
public:

	BasicMediaPlayer();
    void loadFile(QString file);
	int state();
    void stop();
    void wait();
    void play();
    void pause();
    bool seek(double);
	double position();
	double length();
	int error();
	
public slots:

private:
	Sleeper *mediasleeper;
	QAudioOutput *audioOutput;
    void waitForSeekable(int);
    void waitForState(QMediaPlayer::PlaybackState, int);

};

#endif
