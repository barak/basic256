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

#include "MediaLoader.h"

#include <QFile>
#include <QFileInfo>

#include "BasicDownloader.h"
#include "Error.h"
#include "ErrorCodes.h"
#include "MediaPath.h"


MediaLoader::MediaLoader(BasicDownloader *&downloader, SoundRegistrar registrar)
	: downloader(downloader), registerSound(registrar) {
}

void
MediaLoader::startRun() {
#ifdef Q_OS_WASM
	wasmSoundResources.clear();
#endif
}

// The bytes of a path that is not a local file.  On wasm a relative path is
// fetched from beside the page (MediaPath); on the desktop only a URL is.
QByteArray
MediaLoader::fetch(const QString &path) {
	downloader->download(MediaPath::downloadUrl(path));
	return downloader->data();
}

// SOUNDLOAD: register a file or URL as a sound resource and return its id.
QString
MediaLoader::loadSound(const QString &s) {
	if(QFileInfo(s).exists()){
		QFile file(s);
		file.open(QIODevice::ReadOnly);
		QByteArray arr = file.readAll();
		file.close();
		QString id = QString("sound:") + s;
		registerSound(id, &arr);
		return id;
	}else if (MediaPath::isFetchable(s)){
		// On wasm a relative path ("./sounds/bounce.mp3") is fetched
		// from beside the page -- there is no local file to find.
		downloader->download(MediaPath::downloadUrl(s));
		QByteArray arr = downloader->data();
		QString id = QString("sound:") + s;
		registerSound(id, &arr);
		return id;
	}else{
		error->q(ERROR_SOUNDFILE);
		return QString("");
	}
}

#ifdef Q_OS_WASM
// SOUND, SOUNDPLAY and SOUNDPLAYER given a file or URL: make sure it is
// registered as "sound:" + source, once per run (see MediaLoader.h for why).
bool
MediaLoader::registerPlaySource(const QString &playsource) {
	QString id = QString("sound:") + playsource;
	if(!wasmSoundResources.contains(id)){
		QByteArray arr;
		bool got = false;
		if(QFileInfo(playsource).exists()){
			QFile file(playsource);
			if(file.open(QIODevice::ReadOnly)){
				arr = file.readAll();
				file.close();
				got = true;
			}
		}else if(MediaPath::isFetchable(playsource)){
			downloader->download(MediaPath::downloadUrl(playsource));
			arr = downloader->data();
			got = !arr.isEmpty();
		}
		if(!got){
			// BasicDownloader raises its own ERROR_DOWNLOAD on a
			// failed fetch -- don't stack a second error on top.
			if(!error->pending()) error->q(ERROR_SOUNDFILE);
			return false;
		}
		registerSound(id, &arr);
		wasmSoundResources.insert(id);
	}
	return true;
}

// Forget any auto-registration of this id, so a later SOUND/SOUNDPLAY of the
// same file or URL fetches and re-registers it instead of playing a resource
// that is no longer loaded.
void
MediaLoader::forgetSound(const QString &id) {
	wasmSoundResources.remove(id);
}
#endif
