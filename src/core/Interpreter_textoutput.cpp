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

// Text output opcodes.
//
// Interpreter::execByteCode() in Interpreter.cpp decodes each opcode and
// hands these ones to execTextOutputOp().  The cases are exactly as they were
// there: a break still ends the opcode, and the checks execByteCode()
// makes after every opcode still run when this returns.

#include "InterpreterPrivate.h"

void Interpreter::execTextOutputOp(int opcode) {
	switch(opcode) {

		case OP_CLS: {
			mymutex->lock();
			emit(outputClear());
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_INPUT: {
			if (guiState == GUISTATESILENT) {
				// --silent is non-interactive: never block waiting for
				// input the user has no way to provide. Exit the whole
				// process immediately (not a catchable BASIC error) so
				// automated test runners get a clear non-zero status.
				std::cerr << "INPUT not supported in --silent mode." << std::endl;
				std::exit(1);
			}
			inputType = stack->popInt();
			inputString.clear();
			QString prompt = stack->popQString();
			if (prompt.length()>0) {
				mymutex->lock();
				emit(outputReady(prompt));
				waitCond->wait(mymutex);
				mymutex->unlock();
			}
			// 1) Signal the outwin to start input
			// 2) when return is pressed  outwin signals runcontroller with the string
			// 3) runcontroller puts it in the variable inputString and releases the wait condition
			mymutex->lock();
			emit(getInput());
			waitCond->wait(mymutex);
			mymutex->unlock();

			// now push value to stack
			switch (inputType) {
				case T_INT:
					{
						bool ok;
						qint64 i=0;
						i = inputString.toLongLong(&ok);
						if (!ok) {
							error->q(ERROR_NUMBERCONV);
						}
						stack->pushLong(i);
					}
					break;
				case T_FLOAT:
					{
						bool ok;
						double d=0.0;
						// match Convert::getFloat()'s handling: only use the
						// locale's own decimal point if the user opted in via
						// the "use locale decimal point" setting; otherwise
						// always expect "." regardless of system/app locale.
						if (convert->useLocaleDecimalPoint()) {
							d = locale->toDouble(inputString,&ok);
						} else {
							d = inputString.toDouble(&ok);
						}
						if (!ok) {
							error->q(ERROR_NUMBERCONV);
						}
						stack->pushDouble(d);
					}
					break;
				case T_STRING:
					{
						stack->pushQString(inputString);
					}
					break;
				default:
					{
						// standard input should try to convert string to a number if it can.
						bool ok;
						qint64 i;
						i = inputString.toLongLong(&ok);
						if (ok) {
							stack->pushLong(i);
						} else {
							double d;
							if (convert->useLocaleDecimalPoint()) {
								d = locale->toDouble(inputString,&ok);
							} else {
								d = inputString.toDouble(&ok);
							}
							if (ok) {
								stack->pushDouble(d);
							} else {
								stack->pushQString(inputString);
							}
						}
					}
			}
		}
		break;

		case OP_KEY: {
			int getUNICODE = stack->popInt();
			mymutex->lock();
			stack->pushInt(basicKeyboard->getLastKey(getUNICODE));
			mymutex->unlock();
		}
		break;

		case OP_KEYPRESSED: {
			mymutex->lock();
			int keyCode = stack->popInt();
			if (keyCode==0) {
				stack->pushInt(basicKeyboard->count());
			} else {
				if (basicKeyboard->isPressed(keyCode)) {
					stack->pushInt(1);
				} else {
					stack->pushInt(0);
				}
			}
			mymutex->unlock();
		}
		break;

		case OP_PRINT:
		{
			// arguments are in reverse order off the stack
			bool nl = stack->popBool();
			int n = stack->popInt();
			QString p = "";
			for (int i =0; i<n; i++) {
				QString s = stack->popQString();
				if (i>0) {
					// add blanks to 14 character tab stop
					while(s.length()%14!=0) {
						s.append(" ");
					}
				}
				p.prepend(s);
			}
			if (nl) {
				p += "\n";
			}
			mymutex->lock();
			emit(outputReady(p));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_LOCATE: {
			// LOCATE column, row - zero based, matching the graphics pane
			int row = stack->popInt();
			int col = stack->popInt();
			mymutex->lock();
			emit(outputLocate(col, row));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_TEXTCOLOR: {
			QColor bg = stack->popQColor();
			QColor fg = stack->popQColor();
			mymutex->lock();
			emit(outputColor((int) fg.rgba(), (int) bg.rgba()));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_TEXTFONT: {
			bool italic = stack->popBool();
			int weight = stack->popInt();
			int size = stack->popInt();
			QString family = stack->popQString().trimmed();
			mymutex->lock();
			emit(outputFont(family, size, weight, italic));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_TEXTCOL: {
			mymutex->lock();
			emit(getTextCol());
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushInt(returnInt);
		}
		break;

		case OP_TEXTROW: {
			mymutex->lock();
			emit(getTextRow());
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushInt(returnInt);
		}
		break;

		case OP_TEXTBACKGROUND: {
			QColor bg = stack->popQColor();
			mymutex->lock();
			emit(outputBackground((int) bg.rgba()));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_TEXTSCREEN: {
			// TEXTSCREEN columns, rows - the same order as LOCATE. Zero
			// for either drops back to the flowing pane. The third
			// argument, when it is there, asks for square cells.
			int square = stack->popInt();
			int rows = stack->popInt();
			int cols = stack->popInt();
			mymutex->lock();
			emit(outputScreen(cols, rows, square != 0));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_TEXTCHAR: {
			// TEXTCHAR(column, row) reads a character back off the
			// screen, the way SCREEN$ did on the Spectrum.
			int row = stack->popInt();
			int col = stack->popInt();
			mymutex->lock();
			emit(getTextChar(col, row));
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushQString(returnString);
		}
		break;

	}
}
