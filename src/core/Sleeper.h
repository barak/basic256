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


#ifndef __SLEEPER_H
#define __SLEEPER_H

#ifdef WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <time.h>
#endif
#include <atomic>   // new

class Sleeper
{
public:
	Sleeper();
	bool sleepMS(long int ms);
	void sleepRQM(long int ms);
	void sleepSeconds(double s);
	void wake();
	
private:
	std::atomic<bool> wakesleeper;

};

#endif
