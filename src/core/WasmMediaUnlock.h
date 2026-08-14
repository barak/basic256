/** Copyright (C) 2026, BASIC256 contributors
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

#pragma once

// Browser audio and speech both refuse to start unless they are started from
// inside a user-gesture handler. Desktop Chrome is lenient -- it treats any
// past interaction with the page as sticky, which is why SOUND and SAY worked
// in the browser tests -- but iOS/iPadOS WebKit is not: an AudioContext
// created or resumed anywhere other than *during* a gesture stays suspended,
// and speechSynthesis.speak() called without a live activation drops the
// utterance and fires no event at all.
//
// Neither condition can be met from where BASIC256 actually plays sound: the
// interpreter runs on a worker thread and reaches audio through queued slots,
// so by the time SOUND or SAY executes the gesture that started the program is
// long over. install() therefore hooks the *first* interaction with the page,
// before any program runs, and does the three things that must happen inside
// that handler: start the shared AudioContext, promote the iOS audio session
// out of "ambient" (so the iPad's side switch stops muting Web Audio), and
// prime speechSynthesis.
//
// Call this once at startup, on the main thread, alongside MediaPath::init().
// It is a no-op off WASM.

namespace WasmMediaUnlock {

void install();

}
