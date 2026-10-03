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

// System and IDE opcodes.
//
// Interpreter::execByteCode() in Interpreter.cpp decodes each opcode and
// hands these ones to execSystemOp().  The cases are exactly as they were
// there: a break still ends the opcode, and the checks execByteCode()
// makes after every opcode still run when this returns.

#include "InterpreterPrivate.h"

void Interpreter::execSystemOp(int opcode) {
	switch(opcode) {

		case OP_SYSTEM: {
			QString temp = stack->popQString();
#ifdef BASIC256_ENABLE_PROCESS
			int doit = settingsAllowSystem;
			if(doit==SETTINGSALLOWASK){
				if (guiState == GUISTATESILENT) {
					// --silent: no one to ask, so fail closed (same
					// outcome as the user clicking "No") rather than
					// hanging on a dialog that will never appear.
					doit = SETTINGSALLOWNO;
				} else {
					mymutex->lock();
					emit(dialogAllowSystem(temp));
					waitCond->wait(mymutex);
					mymutex->unlock();
					doit = returnInt;
				}
			}
			if(doit==SETTINGSALLOWNO){
				error->q(ERROR_PERMISSION);
			}else if(doit==SETTINGSALLOWYES) {
				sys = new QProcess();
				QStringList args = QProcess::splitCommand(temp);
				if (!args.isEmpty()) {
				    QString program = args.takeFirst();
				    sys->start(program, args);
				}
				if (sys->waitForStarted(-1)) {
					if (!sys->waitForFinished(-1)) {
						//QByteArray result = sy.readAll();
					}
				}
				delete sys;
				sys=NULL;
			}
#else
			(void)temp;
			error->q(ERROR_NOTAVAILABLE);
#endif
		}
		break;

		case OP_SETSETTING: {
			QString stuff = stack->popQString();
			QString key = stack->popQString().trimmed();
			QString app = stack->popQString().trimmed();

			if(app.isEmpty() || app.contains(QChar('\\')) || app.contains(QChar('/')) || app.size()>255 || QString::compare(app, "SYSTEM", Qt::CaseInsensitive)==0){
				error->q(ERROR_INVALIDPROGNAME);
				break;
			}
			if(key.isEmpty() || key.contains(QChar('\\')) || key.contains(QChar('/')) || key.size()>255){
				error->q(ERROR_INVALIDKEYNAME);
				break;
			}
			if(stuff.size()>16383){
				error->q(ERROR_SETTINGMAXLEN);
				break;
			}

			if(settingsSettingsAccess!=2){
				if(programName.isEmpty()){
					programName=app;
				}else if(programName!=app){
					error->q(ERROR_SETTINGSSETACCESS);
					break;
				}
			}
			if(settingsAllowSetting) {
				SETTINGS;
				settings.beginGroup(SETTINGSGROUPUSER);
				settings.beginGroup(app);
				if(stuff.isEmpty()){
					settings.remove(key);
				}else{
					if(settingsSettingsMax>0){
						QStringList list=settings.childKeys();
						int s=list.size();
						if(!(s<settingsSettingsMax || list.contains(key))){
							error->q(ERROR_SETTINGMAXKEYS);
							break;
						}
					}
					settings.setValue(key, stuff);
				}
				settings.endGroup();
				settings.endGroup();
#ifdef Q_OS_WASM
				// Flush to the settings file and schedule a debounced
				// persist of the /persist IDBFS mount to IndexedDB
				// (coalesces SETSETTING in a loop into one sync). Runs on
				// the interpreter thread; persistSoon() marshals to main.
				settings.sync();
				WasmSettings::persistSoon();
#endif
			} else {
				if(stuff.isEmpty()){
					fakeSettings[app].remove(key);
				}else{
					if(settingsSettingsMax>0){
						int s=fakeSettings[app].size();
						if(!(s<settingsSettingsMax || fakeSettings[app].contains(key))){
							error->q(ERROR_SETTINGMAXKEYS);
							break;
						}
					}
					fakeSettings[app][key]=stuff;
				}
			}
		}
		break;

		case OP_GETSETTING: {
			QString key = stack->popQString().trimmed();
			QString app = stack->popQString().trimmed();
			if(QString::compare(app, "SYSTEM", Qt::CaseInsensitive)==0) {
				SETTINGS;
				QString v = settings.value(key, "").toString();
				if(v.length()!=32){
					//not MD5, not the password
					stack->pushQString(v);
				}else{
					//check if user try to get the password
					//user can use different keys to get the password such "////Pref//Password/" or "PREF/password"
					//the safest way is to compare the value with saved password to avoid user roundabouts
					QString p = settings.value(SETTINGSPREFPASSWORD, "").toString();
					if(QString::compare(v,p)==0){
						stack->pushQString("****");
					}else{
						stack->pushQString(v);
					}
				}
				break;
			}
			if(app.isEmpty() || app.contains(QChar('\\')) || app.contains(QChar('/')) || app.size()>255){
				error->q(ERROR_INVALIDPROGNAME);
				stack->pushQString("");
				break;
			}
			if(key.isEmpty() || key.contains(QChar('\\')) || key.contains(QChar('/')) || key.size()>255){
				error->q(ERROR_INVALIDKEYNAME);
				stack->pushQString("");
				break;
			}

			if(settingsSettingsAccess==0){
				if(programName.isEmpty()){
					programName=app;
				}else if(programName!=app){
					error->q(ERROR_SETTINGSGETACCESS);
					stack->pushQString("");
					break;
				}
			}

			if(settingsAllowSetting){
				SETTINGS;
				settings.beginGroup(SETTINGSGROUPUSER);
				settings.beginGroup(app);
				stack->pushQString(settings.value(key, "").toString());
				settings.endGroup();
				settings.endGroup();
			} else {
				QString s("");
				if(fakeSettings.contains(app)){
					if(fakeSettings[app].contains(key)) s=fakeSettings[app][key];
				}
				stack->pushQString(s);
			}
		}
		break;

		case OP_PORTOUT: {
			int data = stack->popInt();
			int port = stack->popInt();
#ifdef WIN32PORTIO
			int doit = settingsAllowPort;
			if(doit==SETTINGSALLOWASK){
				if (guiState == GUISTATESILENT) {
					// --silent: no one to ask, so fail closed rather than
					// hanging on a dialog that will never appear.
					doit = SETTINGSALLOWNO;
				} else {
					mymutex->lock();
					emit(dialogAllowPortInOut(QString("PORTOUT ") + QString::number(port) + ", " + QString::number(data)));
					waitCond->wait(mymutex);
					mymutex->unlock();
					doit = returnInt;
				}
			}
			if(doit>0) {
				if (Out32==NULL) {
					error->q(ERROR_NOTIMPLEMENTED);
				} else {
					Out32(port, data);
				}
			} else if(doit==0){
				error->q(ERROR_PERMISSION);
			}
# else
				error->q(ERROR_NOTIMPLEMENTED);
#endif
		}
		break;

		case OP_PORTIN: {
			int data=0;
			int port = stack->popInt();
#ifdef WIN32PORTIO
			int doit = settingsAllowPort;
			if(doit==SETTINGSALLOWASK){
				if (guiState == GUISTATESILENT) {
					// --silent: no one to ask, so fail closed rather than
					// hanging on a dialog that will never appear.
					doit = SETTINGSALLOWNO;
				} else {
					mymutex->lock();
					emit(dialogAllowPortInOut(QString("PORTIN ") + QString::number(port)));
					waitCond->wait(mymutex);
					mymutex->unlock();
					doit = returnInt;
				}
			}

			if(doit==SETTINGSALLOWNO){
				error->q(ERROR_PERMISSION);
			}else if(doit==SETTINGSALLOWYES) {
				if (Inp32==NULL) {
					error->q(ERROR_NOTIMPLEMENTED);
				} else {
					data = Inp32(port);
				}
			}
#else
				error->q(ERROR_NOTIMPLEMENTED);
#endif
			stack->pushInt(data);
		}
		break;

		case OP_OSTYPE: {
			// Return type of OS this compile was for
			int os = -1;
#ifdef WIN32
			os = OSTYPE_WINDOWS;
#endif
#ifdef LINUX
			os = OSTYPE_LINUX;
#endif
#ifdef MACX
			os = OSTYPE_MACINTOSH;
#endif
			// No Android target. OSTYPE_ANDROID remains a language constant so
			// existing programs that compare against it still compile; OS()
			// simply never returns it.
			stack->pushInt(os);
		}
		break;

		case OP_EDITVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(0,show!=0));
		}
		break;

		case OP_GRAPHVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(1,show!=0));
		}
		break;

		case OP_OUTPUTVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(2,show!=0));
		}
		break;

		case OP_MAINTOOLBARVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(3,show!=0));
		}
		break;

		case OP_GRAPHTOOLBARVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(4,show!=0));
		}
		break;

		case OP_OUTPUTTOOLBARVISIBLE:
		{
			int show = stack->popInt();
			emit(mainWindowsVisible(5,show!=0));
		}
		break;

		case OP_MAXIMIZE:
		{
			// Not a window of its own but the state of the IDE window
			// that holds them all, so it rides the same signal with a
			// selector of its own rather than getting a second one.
			int maximize = stack->popInt();
			emit(mainWindowsVisible(6,maximize!=0));
		}
		break;

		case OP_ALERT: {
			if (guiState == GUISTATESILENT) {
				std::cerr << "ALERT not supported in --silent mode." << std::endl;
				std::exit(1);
			}
			QString temp = stack->popQString();
			mymutex->lock();
			emit(dialogAlert(temp));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

		case OP_CONFIRM: {
			if (guiState == GUISTATESILENT) {
				std::cerr << "CONFIRM not supported in --silent mode." << std::endl;
				std::exit(1);
			}
			int dflt = stack->popInt();
			QString temp = stack->popQString();
			mymutex->lock();
			emit(dialogConfirm(temp,dflt));
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushInt(returnInt);
		}
		break;

		case OP_PROMPT: {
			if (guiState == GUISTATESILENT) {
				std::cerr << "PROMPT not supported in --silent mode." << std::endl;
				std::exit(1);
			}
			QString dflt = stack->popQString();
			QString msg = stack->popQString();
			mymutex->lock();
			emit(dialogPrompt(msg,dflt));
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushQString(inputString);
		}
		break;

		case OP_GETCLIPBOARDIMAGE: {
			mymutex->lock();
			emit(getClipboardImage());
			waitCond->wait(mymutex);
			mymutex->unlock();
			//
			lastImageId++;
			QString id = QString("image:") + QString::number(lastImageId) + QStringLiteral(":clipboard");
			images[id] = new QImage(returnImage.convertToFormat(QImage::Format_ARGB32));
			stack->pushQString(id);
		}
		break;

		case OP_GETCLIPBOARDSTRING: {
			mymutex->lock();
			emit(getClipboardString());
			waitCond->wait(mymutex);
			mymutex->unlock();
			stack->pushQString(inputString);
		}
		break;

		case OP_SETCLIPBOARDIMAGE: {
			QString id = stack->popQString();
			if (images.contains(id)){
				mymutex->lock();
				emit(setClipboardImage(*images[id]));
				waitCond->wait(mymutex);
				mymutex->unlock();
			} else {
				error->q(ERROR_IMAGERESOURCE);
			}
		}
		break;

		case OP_SETCLIPBOARDSTRING: {
			QString s = stack->popQString();
			mymutex->lock();
			emit(setClipboardString(s));
			waitCond->wait(mymutex);
			mymutex->unlock();
		}
		break;

	}
}
