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

#ifndef SPRITELAYER_H
#define SPRITELAYER_H

#include <QImage>
#include <QRect>

class GraphicsBuffer;

/*
 * SpriteLayer - the sprites a program has made with SPRITEDIM, and the layer
 * they are drawn on.
 *
 * What it does
 * ------------
 * Sprites are not drawn by the painter the rest of the graphics statements use.
 * They are kept here as a numbered array, each with its picture, a copy of the
 * picture scaled and rotated, where it sits and whether it is shown, and are
 * painted onto their own image, GraphicsBuffer::spritesimage.  GraphicsBuffer
 * lays that image over the drawing when the screen is updated, clipped to
 * sprites_clip_region, and only while draw_sprites_flag is set.  So a sprite
 * never marks the drawing beneath it, and moving one needs no redraw of the
 * program's picture - only of the rectangles sprites have left or entered.
 *
 * What the interpreter asks
 * -------------------------
 *   startRun()               once per run, before the first opcode: no sprites
 *   dim(n)                   SPRITEDIM: drop every sprite and make n empty ones
 *   clear()                  drop every sprite and empty the layer (the end of
 *                            a run, and SPRITEDIM before it makes new ones)
 *   count(), exists(n)       how many there are; whether n is one of them
 *   sprites[n]               the Sprite itself - the opcodes read its fields,
 *                            and SPRITEPOLY/TEXT/LOAD/SLICE give it an image
 *   prepareForNewContent(n)  before a sprite is given a new image: free the old
 *                            ones and put it back at 0,0, unscaled, unrotated,
 *                            opaque and hidden
 *   place(n, x, y, s, r, o)  SPRITEPLACE and SPRITEMOVE, once the arguments
 *                            are resolved to absolute values: rebuild the
 *                            transformed image if s or r changed, move, clamp
 *                            the opacity, and mark the sprite changed
 *   collide(n1, n2, deep)    SPRITECOLLIDE: do the two overlap - by bounding
 *                            shape, or with deep set by opaque pixels
 *   forceRedrawAll()         GRAPHSIZE on screen: the layer image was made anew,
 *                            so every shown sprite is painted again next update
 *   updateScreen()           before every screen update: repaint the changed
 *                            rectangles of the layer and set the clip region and
 *                            flag GraphicsBuffer composites with
 *
 * The opcodes themselves - popping arguments, range and "not assigned" errors,
 * and making a sprite's image - are Interpreter::execSpriteOp() in
 * Interpreter_sprites.cpp.  Making an image stays there because it needs the
 * interpreter's pen, brush, font, painter, image list and media loader; this
 * class is handed the finished QImage by assignment to Sprite::image.
 *
 * Coordinates
 * -----------
 * Everything here is in pixels of the graphics output, and a sprite's x,y is
 * its centre.  WINDOW does not reach this layer: SPRITEPLACE stores x,y as
 * given and updateScreen() sets no world transform, so under a WINDOW a sprite
 * and a PLOT at the same numbers land in different places.  That is known and
 * left as it is - the layer is composited, not painted through the drawing
 * transform, and changing that is a design decision of its own.
 *
 * Redraw bookkeeping
 * ------------------
 * position is the rectangle the sprite covers now (of the transformed image
 * when there is one), last_position the one it covered when last painted,
 * was_printed whether it has been painted since it was last hidden or the
 * layer was remade, and changed whether position, picture or opacity moved on
 * since.  updateScreen() clears the union of the old and new rectangles of
 * every sprite that moved, hid or appeared, then repaints each shown sprite
 * that meets that region, in number order, so a higher number is on top.
 *
 * What it needs from the interpreter
 * ----------------------------------
 * Only the GraphicsBuffer, handed in at construction and not owned - the same
 * one the interpreter draws on.  No errors are raised here: the opcode checks
 * the sprite number and that it has an image before calling in.
 *
 * Threading
 * ---------
 * Everything here runs on the interpreter thread.  updateScreen() writes the
 * layer image and clip region that the main thread reads when it composites;
 * the interpreter only calls it from waitForGraphics(), before handing over
 * with the goutputReady() round trip, and the two never overlap.
 *
 * Kept as it was
 * --------------
 * This class came out of Interpreter.cpp as a behaviour-neutral move, so a few
 * oddities came with it:
 *   - dim() does not set o, so SPRITEO of a sprite that has never had an image
 *     reports whatever the allocation held.  prepareForNewContent() sets it to
 *     1, so every sprite that can be shown has a real opacity.
 *   - startRun() only forgets the count; the array is freed by the clear() at
 *     the end of the run before.  Nothing frees it when the interpreter is
 *     destroyed, which only matters if that happens mid-run.
 *   - collide() and updateScreen() pick the plain image when s is 1 and r is 0
 *     and the transformed one otherwise; place() keeps the two in step, so the
 *     transformed image is there whenever it is picked.
 */

struct Sprite {
	bool visible;
	double x;
	double y;
	double r;	// rotate
	double s;	// scale
	double o;	// opacity
	QImage *image;
	QImage *transformed_image;
	QRect position;
	bool changed;
	bool was_printed;
	QRect last_position;
};

class SpriteLayer {
	public:
		SpriteLayer(GraphicsBuffer *graphics);

		void startRun();
		void dim(int n);
		void clear();
		int count() const { return nsprites; }
		bool exists(int n) const { return n >= 0 && n < nsprites; }
		Sprite &operator[](int n) { return sprites[n]; }

		void prepareForNewContent(int n);
		void place(int n, double x, double y, double s, double r, double o);
		bool collide(int n1, int n2, bool deep);
		void forceRedrawAll();
		void updateScreen();

	private:
		GraphicsBuffer *graphics;	// not owned
		Sprite *sprites;
		int nsprites;
};

#endif
