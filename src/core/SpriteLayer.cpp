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

#include "SpriteLayer.h"

#include <QPainter>
#include <QPolygon>
#include <QRegion>
#include <QTransform>

#include "GraphicsBuffer.h"


SpriteLayer::SpriteLayer(GraphicsBuffer *graphics)
	: graphics(graphics), sprites(NULL), nsprites(0) {
}

void SpriteLayer::startRun() {
	nsprites = 0;
}

// SPRITEDIM
void SpriteLayer::dim(int n) {
	// deallocate existing sprites
	clear();
	// create new ones that are not visible, active, and are at origin
	if (n > 0) {
		sprites = new Sprite[n];
		nsprites = n;
		while (n>0) {
			n--;
			sprites[n].image = NULL;
			sprites[n].transformed_image = NULL;
			sprites[n].visible = false;
			sprites[n].x = 0;
			sprites[n].y = 0;
			sprites[n].r = 0;
			sprites[n].s = 1;
			sprites[n].position.setRect(0,0,0,0);
			sprites[n].changed=false;
			sprites[n].was_printed = false;
			sprites[n].last_position.setRect(0,0,0,0);
		}
	}
}

// SPRITEPLACE and SPRITEMOVE, with x, y, s, r and o already absolute
void SpriteLayer::place(int n, double x, double y, double s, double r, double o) {
	double img_w, img_h;

	if(sprites[n].s != s || sprites[n].r != r){
		//there is a transformation from the last time
		if (sprites[n].transformed_image) {
			delete sprites[n].transformed_image;
			sprites[n].transformed_image = NULL;
		}
		if(s!=1 || r!=0){
			QTransform transform = QTransform().translate(sprites[n].image->width()/2, sprites[n].image->height()/2).rotateRadians(r).scale(s,s);;
			sprites[n].transformed_image = new QImage(sprites[n].image->transformed(transform).convertToFormat(QImage::Format_ARGB32_Premultiplied));
			img_w=sprites[n].transformed_image->width();
			img_h=sprites[n].transformed_image->height();
			sprites[n].position.setRect(x-(img_w/2),y-(img_h/2),img_w,img_h);
		}else{
			img_w=sprites[n].image->width();
			img_h=sprites[n].image->height();
			sprites[n].position.setRect(x-(img_w/2),y-(img_h/2),img_w,img_h);
		}
		sprites[n].changed=true;
	}else if(sprites[n].x != x || sprites[n].y != y){
		//there is no transformation from last time but is just a movement
		if(s!=1 || r!=0){
			img_w=sprites[n].transformed_image->width();
			img_h=sprites[n].transformed_image->height();
		}else{
			img_w=sprites[n].image->width();
			img_h=sprites[n].image->height();
		}
		sprites[n].position.moveTo(x-(img_w/2),y-(img_h/2));
		sprites[n].changed=true;
	}
	if(sprites[n].o != o){
		if(o<0) o=0;
		if(o>1) o=1;
		if(sprites[n].o != o)
			sprites[n].changed=true;
	}

	sprites[n].x = x;
	sprites[n].y = y;
	sprites[n].s = s;
	sprites[n].r = r;
	sprites[n].o = o;
}

void SpriteLayer::clear() {
	// cleanup sprites - release images and deallocate the space
	graphics->spritesimage->fill(Qt::transparent);

	int i;
	if (nsprites>0) {
		for(i=0; i<nsprites; i++) {
			if (sprites[i].image) {
				delete sprites[i].image;
				sprites[i].image = NULL;
			}
			if (sprites[i].transformed_image) {
				delete sprites[i].transformed_image;
				sprites[i].transformed_image = NULL;
			}
		}
		delete[] sprites;
		sprites = NULL;
		nsprites = 0;
		graphics->draw_sprites_flag = false;
	}
}

void SpriteLayer::prepareForNewContent(int n) {
	if (sprites[n].image) {
		delete sprites[n].image;
		sprites[n].image = NULL;
	}
	if (sprites[n].transformed_image) {
		delete sprites[n].transformed_image;
		sprites[n].transformed_image = NULL;
	}
	sprites[n].x=0;
	sprites[n].y=0;
	sprites[n].r=0;	// rotate
	sprites[n].s=1;	// scale
	sprites[n].o=1;	// opacity
	sprites[n].visible=false;
	sprites[n].changed=true;
	sprites[n].position.setRect(0,0,0,0);
	//last_position and was_printed remains the same in case we need to clear last position
}

bool SpriteLayer::collide(int n1, int n2, bool deep) {
	QPolygon p1, p2, result;
	QPoint center;
	QRect rect;
	if (n1==n2) return true;											// cant collide with itself
	if (!sprites[n1].visible || !sprites[n2].visible) return false; 	// cant collide if invisible
	if(!sprites[n1].position.intersects(sprites[n2].position))
		return false;

	if(sprites[n1].r==0 && sprites[n2].r==0){
		if(!deep) return true;
		rect=sprites[n1].position.intersected(sprites[n2].position);
	}else{
		if(sprites[n1].r==0){
			p1=QPolygon(sprites[n1].position);
		}else{
			p1 = QTransform().translate(0,0).rotateRadians(sprites[n1].r).scale(sprites[n1].s,sprites[n1].s).mapToPolygon(QRect(0, 0, sprites[n1].image->width(), sprites[n1].image->height()));
			center = p1.boundingRect().center();
			p1.translate(sprites[n1].x-center.x(), sprites[n1].y-center.y());
		}
		if(sprites[n2].r==0){
			p2=QPolygon(sprites[n2].position);
		}else{
			p2 = QTransform().translate(0,0).rotateRadians(sprites[n2].r).scale(sprites[n2].s,sprites[n2].s).mapToPolygon(QRect(0, 0, sprites[n2].image->width(), sprites[n2].image->height()));
			center = p2.boundingRect().center();
			p2.translate(sprites[n2].x-center.x(), sprites[n2].y-center.y());
		}

		result=p1.intersected(p2);
		if(result.isEmpty()) return false;
		if(!deep) return true;
		//this line can look stupid, but if we use only rect = result.boundingRect(), then we got from time to time
		//bome black lines on the edge of intersected rectangle after we print the two images
		//draw contact zone
		//rect = result.boundingRect();
		rect = result.boundingRect().intersected(sprites[n1].position).intersected(sprites[n2].position);
	}
	if(rect.isEmpty()) return false;

	// Debug
	//	QPainter *ian2;
	//	ian2 = new QPainter(graphics->image);
	//	ian2->drawPolygon(p1);
	//	ian2->drawPolygon(p2);
	//	ian2->drawPolygon(result);
	//	ian2->drawRect(result.boundingRect());
	//	ian2->end();
	//	delete ian2;
	//////////////////////////////////


	QImage *scan = new QImage(rect.size(), QImage::Format_ARGB32_Premultiplied);
	scan->fill(Qt::transparent);
	QPainter *sprite_painter = new QPainter(scan);
	if(sprites[n1].r==0 && sprites[n1].s==1){
		sprite_painter->drawImage(sprites[n1].position.x()-rect.x(),sprites[n1].position.y()-rect.y(), *sprites[n1].image);
	}else{
		sprite_painter->drawImage(sprites[n1].position.x()-rect.x(),sprites[n1].position.y()-rect.y(), *sprites[n1].transformed_image);
	}
	sprite_painter->setCompositionMode(QPainter::CompositionMode_DestinationIn);
	if(sprites[n2].r==0 && sprites[n2].s==1){
		sprite_painter->drawImage(sprites[n2].position.x()-rect.x(),sprites[n2].position.y()-rect.y(), *sprites[n2].image);
	}else{
		sprite_painter->drawImage(sprites[n2].position.x()-rect.x(),sprites[n2].position.y()-rect.y(), *sprites[n2].transformed_image);
	}
	sprite_painter->end();
	delete sprite_painter;

	//check collision comparing only alpha channel
	const uchar* scanbits = scan->bits();
	bool flag=false;
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
	const int max = scan->sizeInBytes();
#else
	const int max = scan->byteCount();
#endif
	for(int f=3;f<max;f+=4){
		if(scanbits[f]){
			flag=true;
			break;
		}
	}

	//debug - print collision zone
	//	painter->drawImage(0,0,*scan);
	//	painter->end();

	delete scan;
	return flag;
}

void SpriteLayer::forceRedrawAll(){
	for(int n=0;n<nsprites;n++){
		sprites[n].was_printed=false;
	}
}

void SpriteLayer::updateScreen(){
	if(nsprites<=0){
		graphics->draw_sprites_flag = false;
		return;
	}

	QPainter *sprite_painter;
	QRegion region = QRegion(0,0,0,0);
	sprite_painter = new QPainter(graphics->spritesimage);
	bool flag=false;

	for(int n=0;n<nsprites;n++){
		if(sprites[n].was_printed){
			if(!sprites[n].visible){
				//clear old position if sprite is hidden now
				region+=sprites[n].last_position;
				sprites[n].was_printed=false;
			}else{
				if(sprites[n].changed){
					//prepare area for a moved sprite
					region+=sprites[n].last_position;
					region+=sprites[n].position;
				}
			}
		}else{
			//sprite become visible - clear area
			if(sprites[n].visible){
				region+=sprites[n].position; //Delete new - mark for first draw
			}
		}
	}

	graphics->sprites_clip_region = region;
	sprite_painter->setClipRegion(region);
	sprite_painter->setCompositionMode(QPainter::CompositionMode_Clear);
	sprite_painter->fillRect(region.boundingRect(),Qt::transparent);
	sprite_painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
	double lasto=1.0;
	for(int n=0;n<nsprites;n++){
		if(sprites[n].visible){
				if(lasto!=sprites[n].o){
					lasto=sprites[n].o;
					sprite_painter->setOpacity(lasto);
				}
				if(sprites[n].s==1 && sprites[n].r==0){
					if(sprites[n].image){
						if(graphics->sprites_clip_region.intersects(sprites[n].position)){
							sprite_painter->drawImage(sprites[n].position, *sprites[n].image);
							sprites[n].last_position=sprites[n].position;
							sprites[n].was_printed=true;
							sprites[n].changed=false;
						}
						graphics->sprites_clip_region+=sprites[n].position;
						flag = true;
					}
				}else{
					if(sprites[n].transformed_image){
						if(graphics->sprites_clip_region.intersects(sprites[n].position)){
							sprite_painter->drawImage(sprites[n].position, *sprites[n].transformed_image);
							sprites[n].last_position=sprites[n].position;
							sprites[n].was_printed=true;
							sprites[n].changed=false;
						}
						graphics->sprites_clip_region+=sprites[n].position;
						flag = true;
					}
				}

		}
	}
	sprite_painter->end();
	delete sprite_painter;
	graphics->draw_sprites_flag = flag;

}
