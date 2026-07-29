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

#include "WasmMediaUnlock.h"

#include <QtGlobal>

#ifdef Q_OS_WASM

#include <emscripten.h>
#include <emscripten/em_js.h>

// Shares Module.__wasmAudio with WasmAudioSink -- same single AudioContext,
// created by whichever side gets there first. WasmAudioSink installs its
// startFrom/stopSource/elapsed helpers independently of context creation
// precisely so this file may win that race.
EM_JS(void, wasmMediaUnlockInstall, (), {
    if (typeof window === "undefined" || Module.__b256UnlockInstalled) return;
    Module.__b256UnlockInstalled = true;

    // touchend rather than touchstart: Safari counts the former as the
    // completed gesture. keydown covers the physical-keyboard case on iPad.
    var EVENTS = ["pointerdown", "touchend", "mousedown", "keydown"];

    var context = function() {
        if (!Module.__wasmAudio) Module.__wasmAudio = { ctx: null, nodes: new Map() };
        var w = Module.__wasmAudio;
        if (!w.ctx) {
            // webkitAudioContext is the only spelling on iOS Safari before 14.5.
            var AC = (typeof AudioContext !== "undefined") ? AudioContext :
                     ((typeof webkitAudioContext !== "undefined") ? webkitAudioContext : null);
            if (!AC) return null;
            w.ctx = new AC();
        }
        return w.ctx;
    };

    // On iOS an AudioContext is not really running until something has been
    // played through it from a gesture; resume() alone leaves it suspended and
    // every later sound is scheduled onto a clock that never advances.
    var primeAudio = function(ctx) {
        try {
            var src = ctx.createBufferSource();
            src.buffer = ctx.createBuffer(1, 1, ctx.sampleRate);
            src.connect(ctx.destination);
            src.start(0);
        } catch (e) {}
    };

    // 50 ms of 8 kHz 8-bit mono silence, built here rather than pasted in as a
    // base64 blob so the header is readable and verifiable.
    var silentWavUrl = function() {
        var n = 400, sr = 8000;
        var b = new Uint8Array(44 + n);
        var dv = new DataView(b.buffer);
        var ascii = function(off, s) { for (var i = 0; i < s.length; i++) b[off + i] = s.charCodeAt(i); };
        ascii(0, "RIFF");      dv.setUint32(4, 36 + n, true);
        ascii(8, "WAVEfmt ");  dv.setUint32(16, 16, true);
        dv.setUint16(20, 1, true);   // PCM
        dv.setUint16(22, 1, true);   // mono
        dv.setUint32(24, sr, true);  // sample rate
        dv.setUint32(28, sr, true);  // byte rate
        dv.setUint16(32, 1, true);   // block align
        dv.setUint16(34, 8, true);   // bits per sample
        ascii(36, "data");     dv.setUint32(40, n, true);
        b.fill(128, 44);             // 8-bit PCM silence is 0x80, not 0
        return URL.createObjectURL(new Blob([b], { type: "audio/wav" }));
    };

    // iOS gives Web Audio the "ambient" session, which the side switch and
    // Control Centre mute silence and which tracks ringer -- not media --
    // volume. That is the usual reason a WASM build is completely silent on an
    // iPad while working everywhere else. Playing a media element once in the
    // same gesture promotes the process to the "playback" session, after which
    // the mute switch no longer applies. The element is NOT muted: a muted one
    // does not promote. The file itself is silence, so nothing is audible.
    var primeSession = function() {
        try {
            if (!Module.__b256SilentEl) {
                var a = new Audio(silentWavUrl());
                a.setAttribute("playsinline", "");
                a.preload = "auto";
                Module.__b256SilentEl = a;
            }
            var p = Module.__b256SilentEl.play();
            if (p && p.catch) p.catch(function() {});
        } catch (e) {}
    };

    // Speaking one silent utterance inside the gesture marks the synthesizer
    // as user-activated, which is what lets the later fire-and-forget SAY calls
    // (queued slots, long after any gesture) actually produce onend/onerror
    // instead of vanishing. getVoices() is async on iOS and returns [] until
    // voiceschanged; touching it here starts that load early.
    var primeSpeech = function() {
        try {
            var synth = window.speechSynthesis;
            if (!synth || Module.__b256SpeechPrimed) return;
            Module.__b256SpeechPrimed = true;
            if (synth.getVoices) synth.getVoices();
            var u = new SpeechSynthesisUtterance(" ");
            u.volume = 0;
            synth.speak(u);
        } catch (e) {}
    };

    var unlock = function() {
        var ctx = context();
        if (ctx) {
            if (ctx.state !== "running") ctx.resume();
            primeAudio(ctx);
        }
        primeSession();
        primeSpeech();
        // Stay subscribed until the context is genuinely running -- iOS can
        // ignore a gesture that arrives while the page is still coming up, and
        // resume() is asynchronous, so the first attempt often is not the one
        // that takes. Once it is running, stop paying the per-event cost.
        if (ctx && ctx.state === "running") {
            for (var i = 0; i < EVENTS.length; i++) {
                window.removeEventListener(EVENTS[i], unlock, true);
            }
        }
    };

    // Capture phase, so Qt's own canvas handlers cannot stop the event before
    // it reaches us.
    for (var i = 0; i < EVENTS.length; i++) {
        window.addEventListener(EVENTS[i], unlock, true);
    }
});

#endif // Q_OS_WASM

namespace WasmMediaUnlock {

void install() {
#ifdef Q_OS_WASM
    wasmMediaUnlockInstall();
#endif
}

} // namespace WasmMediaUnlock
