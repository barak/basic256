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

#ifndef MEDIALOADER_H
#define MEDIALOADER_H

#include <functional>
#include <QByteArray>
#include <QSet>
#include <QString>

class BasicDownloader;

/*
 * MediaLoader - getting the bytes of a sound or picture a program names.
 *
 * What it does
 * ------------
 * A program names media three ways: a local file, a URL, or - in the browser -
 * a path relative to the page (MediaPath).  This class turns that name into
 * bytes, and for sounds hands the bytes to the sound system as a "sound:"
 * resource.  Playing, stopping and mixing are not here: they belong to
 * SoundSystem, on the main thread, and the opcodes reach it the way they
 * always have.
 *
 * What the interpreter asks
 * -------------------------
 *   fetch(path)                 bytes of a path that is not a local file,
 *                               downloaded (IMGLOAD and IMAGELOAD decode them)
 *   loadSound(path)             SOUNDLOAD: register the file or URL as a sound
 *                               resource and return its "sound:" id, or "" with
 *                               ERROR_SOUNDFILE queued
 *   startRun()                  once per run, before the first opcode
 * and, in the browser build only,
 *   registerPlaySource(source)  SOUND, SOUNDPLAY and SOUNDPLAYER given a file
 *                               or URL: make sure it is registered as
 *                               "sound:" + source, once per run; false with an
 *                               error queued when it cannot be read
 *   forgetSound(id)             UNLOAD of a sound, so the next play of the same
 *                               file registers it afresh
 *
 * Why the browser registers play sources
 * --------------------------------------
 * A file or URL handed straight to SoundSystem::playSound() would construct a
 * QAudioOutput, which resolves the default audio device, which never returns
 * on WASM - and playSound() is a queued slot, so that spin is on the main
 * thread and the whole module dies with no output and no working Stop.
 * Registering the bytes as a resource here, on the interpreter thread where
 * blocking is legal, sends playback through WasmAudioSink like every other
 * sound.  The ids registered this way are remembered for the run so a sound
 * replayed in a loop is not downloaded again.
 *
 * What it needs from the interpreter
 * ----------------------------------
 * Two things, both handed in at construction:
 *   - the interpreter's downloader.  The interpreter creates it at the start of
 *     every run and stops and deletes it when a run is halted; MediaLoader
 *     borrows it by reference, so it always uses whichever one is current.
 *   - a way to hand bytes to the sound system.  That is a blocking round trip
 *     to the main thread, and the interpreter is the one place that talks to
 *     it, so the interpreter supplies it.
 * The only other dependency is the global error queue (Error.h).
 *
 * Threading
 * ---------
 * Everything here runs on the interpreter thread.
 *
 * Deliberately not here
 * ---------------------
 * SPRITELOAD fetches its picture the same way and could call fetch() too; it
 * sits in the sprite code, which is being kept as it is for now.  Loading a
 * local picture stays with the opcode (QImage reads the file itself).
 */
class MediaLoader {
	public:
		// Registers bytes with the sound system under a "sound:" id.
		typedef std::function<void(const QString &id, QByteArray *bytes)> SoundRegistrar;

		MediaLoader(BasicDownloader *&downloader, SoundRegistrar registrar);

		void startRun();
		QByteArray fetch(const QString &path);
		QString loadSound(const QString &path);
#ifdef Q_OS_WASM
		bool registerPlaySource(const QString &source);
		void forgetSound(const QString &id);
#endif

	private:
		BasicDownloader *&downloader;	// the interpreter's, replaced every run
		SoundRegistrar registerSound;
#ifdef Q_OS_WASM
		// "sound:" resource ids this run has already registered for a file or
		// URL passed straight to SOUND/SOUNDPLAY/SOUNDPLAYER, so replaying the
		// same track in a loop does not re-download it. Interpreter-thread only
		// -- deliberately not a peek at SoundSystem::loadedsounds, which lives
		// on the main thread. Cleared at the start of every run().
		QSet<QString> wasmSoundResources;
#endif
};

#endif
