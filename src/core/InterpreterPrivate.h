/** Copyright (C) 2006, Ian Paul Larsen.
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

// What Interpreter.cpp and the Interpreter_<group>.cpp files share: the
// includes, the globals the interpreter reaches that are defined elsewhere,
// and the opcode macros.  Only for those files - nothing else includes it.

#ifndef INTERPRETERPRIVATE_H
#define INTERPRETERPRIVATE_H

#include <iostream>
#include <cstdlib>
#include <climits>
#include <math.h>
#include <string>

#ifdef BASIC256_ENABLE_TCP
#include <QTcpSocket>
#include <QTcpServer>
#include <QNetworkInterface>
#endif
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QDebug>
#include <QTimer>
#include <QRegularExpression>

#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QPen>
#include <QBrush>
#include <QTime>
#include <QMutex>
#include <QWaitCondition>
#include <QDeadlineTimer>
#include <QCoreApplication>
#include <QDir>


#include "basicParse.tab.h"
#include "WordCodes.h"
#include "CompileErrors.h"
#include "Interpreter.h"
#include "MediaPath.h"
#include "md5.h"
#include "opensimplex.h"
#include "Settings.h"
#include "Sound.h"
#include "Constants.h"
#include "BasicKeyboard.h"
#include "WasmSettings.h"


extern SoundSystem *sound;
extern QMutex* mymutex;
extern QMutex* mydebugmutex;
extern QWaitCondition* waitCond;
extern QWaitCondition* waitDebugCond;

extern int guiState;

extern "C" {
//extern int yydebug;
	extern int basicParse(char *);
	extern char* include_filenames[];   // filenames being LEXd
	extern char* include_exec_path;		//path to executable
	extern int linenumber;			  // linenumber being LEXd
	extern int column;				  // column on line being LEXd
	extern char* lexingfilename;		// current included file name being LEXd

	extern int numparsewarnings;
	extern int initializeBasicParse();
	extern void freeBasicParse();
	extern int bytesToFullWords(int size);
	extern int *wordCode;
	extern unsigned int wordOffset;
	extern unsigned int maxwordoffset;

	extern char **symtable;		// table of variables and labels (strings)
	extern int *symtableaddress;	// associated label address
	extern int *symtableaddresstype;	// associated address type
	extern int *symtableaddressargs;	// number of arguments expected by function/subroutine
	extern int numsyms;				// number of symbols

	// arrays to return warnings from compiler
	// defined in basicParse.y
	extern int parsewarningtable[];
	extern int parsewarningtablelinenumber[];
	extern int parsewarningtablecolumn[];
	extern int parsewarningtablelexingfilenumber[];
}

#define optype(op) (OPTYPE_MASK & op) //faster
// the optype as a small dense number 0..7 rather than 0x00000000..0x07000000
// - the values run consecutively so the dispatch switch in execByteCode
// compiles to a single jump table instead of a chain of comparisons
#define optypeindex(op) (((unsigned int)(op)) >> 24)

#endif
