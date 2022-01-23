
#define _USE_MATH_DEFINES
#include "FreeTypeFontRasterizer.h"
#include "LayerBitmapIntf.h"
#include "FreeType.h"
#include <math.h>
#include "MsgIntf.h"
#include "FontSystem.h"
#include "StringUtil.h"
#include <cmath>
#include <algorithm>
#ifdef _WIN32
#include "TVPSysFont.h"
#endif

extern void TVPUninitializeFreeFont();
extern FontSystem* TVPFontSystem;

FreeTypeFontRasterizer::FreeTypeFontRasterizer() : RefCount(0), Face(NULL), LastBitmap(NULL) {
	AddRef();
}
FreeTypeFontRasterizer::~FreeTypeFontRasterizer() {
	if( Face ) delete Face;
	Face = NULL;
	TVPUninitializeFreeFont();
}
void FreeTypeFontRasterizer::AddRef() {
	RefCount++;
}
//---------------------------------------------------------------------------
void FreeTypeFontRasterizer::Release() {
	RefCount--;
	LastBitmap = NULL;
	if( RefCount == 0 ) {
		if( Face ) delete Face;
		Face = NULL;

		delete this;
	}
}
//---------------------------------------------------------------------------
void FreeTypeFontRasterizer::ApplyFont( class tTVPNativeBaseBitmap *bmp, bool force ) {
	if( bmp != LastBitmap || force ) {
		ApplyFont( bmp->GetFont() );
		LastBitmap = bmp;
	}
}
//---------------------------------------------------------------------------
void FreeTypeFontRasterizer::ApplyFont( const tTVPFont& font ) {
	CurrentFont = font;
	std::vector<tjs_string> faces;
	tjs_string face = font.Face.AsStdString();
	// TODO 最初に@があった場合にすべて縦書きとして処理する処理は入っていない、縦書き対応するのなら必要。
	if( face[0] == TJS_W(',') ) {
		tjs_string stdname = TVPFontSystem->GetBeingFont(face);
		faces.push_back( stdname );
	} else {
		split( face, tjs_string(TJS_W(",")), faces );
		for( auto i = faces.begin(); i != faces.end(); ) {
			tjs_string& x = *i;
			x = Trim(x);
			if( TVPFontSystem->FontExists( x ) == false ) {
				i = faces.erase( i );
			} else {
				i++;
			}
		}
		if( faces.empty() ) {
			faces.push_back( tjs_string(TVPFontSystem->GetDefaultFontName()) );
		}
	}

	// TVP_FACE_OPTIONS_NO_ANTIALIASING
	// TVP_FACE_OPTIONS_NO_HINTING
	// TVP_FACE_OPTIONS_FORCE_AUTO_HINTING
	tjs_uint32 opt = 0;
	opt |= (font.Flags & TVP_TF_ITALIC) ? TVP_TF_ITALIC : 0;
	opt |= (font.Flags & TVP_TF_BOLD) ? TVP_TF_BOLD : 0;
	opt |= (font.Flags & TVP_TF_UNDERLINE) ? TVP_TF_UNDERLINE : 0;
	opt |= (font.Flags & TVP_TF_STRIKEOUT) ? TVP_TF_STRIKEOUT : 0;
	opt |= (font.Flags & TVP_TF_FONTFILE) ? TVP_FACE_OPTIONS_FILE : 0;
	bool recreate = false;
	if( Face ) {
		if( Face->GetFontName() != faces[0] || Face->GetOption(TVP_TF_ITALIC) != !!(font.Flags & TVP_TF_ITALIC) || Face->GetOption(TVP_TF_BOLD) != !!(font.Flags & TVP_TF_BOLD)) {
			delete Face;
			Face = NULL;
			Face = new tFreeTypeFace( faces, opt );
			recreate = true;
		}
	} else {
		Face = new tFreeTypeFace( faces, opt );
		recreate = true;
	}
	Face->SetHeight( font.Height < 0 ? -font.Height : font.Height );
	if( recreate == false ) {
		if( font.Flags & TVP_TF_ITALIC ) {
			Face->SetOption(TVP_TF_ITALIC);
		} else {
			Face->ClearOption(TVP_TF_ITALIC);
		}
		if( font.Flags & TVP_TF_BOLD ) {
			Face->SetOption(TVP_TF_BOLD);
		} else {
			Face->ClearOption(TVP_TF_BOLD);
		}
		if( font.Flags & TVP_TF_UNDERLINE ) {
			Face->SetOption(TVP_TF_UNDERLINE);
		} else {
			Face->ClearOption(TVP_TF_UNDERLINE);
		}
		if( font.Flags & TVP_TF_STRIKEOUT ) {
			Face->SetOption(TVP_TF_STRIKEOUT);
		} else {
			Face->ClearOption(TVP_TF_STRIKEOUT);
		}
	}
	LastBitmap = NULL;
}
//---------------------------------------------------------------------------
void FreeTypeFontRasterizer::GetTextExtent(tjs_char ch, tjs_int &w, tjs_int &h) {
	if( Face ) {
		tGlyphMetrics metrics;
		if( Face->GetGlyphSizeFromCharcode( ch, metrics) ) {
			w = metrics.CellIncX;
			h = metrics.CellIncY;
		}
		else
		{
			w = Face->GetHeight();
			h = w;
		}
	}
}
//---------------------------------------------------------------------------
tjs_int FreeTypeFontRasterizer::GetAscentHeight() {
	if( Face ) return Face->GetAscent();
	return 0;
}
//---------------------------------------------------------------------------
tTVPCharacterData* FreeTypeFontRasterizer::GetBitmap( const tTVPFontAndCharacterData & font, tjs_int aofsx, tjs_int aofsy ) {
	if( font.Antialiased ) {
		Face->ClearOption( TVP_FACE_OPTIONS_NO_ANTIALIASING );
	} else {
		Face->SetOption( TVP_FACE_OPTIONS_NO_ANTIALIASING );
	}
	if( font.Hinting ) {
		Face->ClearOption( TVP_FACE_OPTIONS_NO_HINTING );
		//Face->SetOption( TVP_FACE_OPTIONS_FORCE_AUTO_HINTING );
	} else {
		Face->SetOption( TVP_FACE_OPTIONS_NO_HINTING );
		//Face->ClearOption( TVP_FACE_OPTIONS_FORCE_AUTO_HINTING );
	}
	tTVPCharacterData* data = Face->GetGlyphFromCharcode(font.Character);
	if( data == NULL ) {
		data = Face->GetGlyphFromCharcode( Face->GetDefaultChar() );
	}
	if( data == NULL ) {
		data = Face->GetGlyphFromCharcode( Face->GetFirstChar() );
	}
	if( data == NULL ) {
		TVPThrowExceptionMessage( TVPFontRasterizeError );
	}

	int cx = data->Metrics.CellIncX;
	int cy = data->Metrics.CellIncY;
	if( font.Font.Angle == 0 ) {
		data->Metrics.CellIncX = cx;
		data->Metrics.CellIncY = 0;
	} else if(font.Font.Angle == 2700) {
		data->Metrics.CellIncX = 0;
		data->Metrics.CellIncY = cx;
	} else {
		double angle = font.Font.Angle * (M_PI/1800);
		data->Metrics.CellIncX = static_cast<tjs_int>(  std::cos(angle) * cx);
		data->Metrics.CellIncY = static_cast<tjs_int>(- std::sin(angle) * cx);
	}

	data->Antialiased = font.Antialiased;
	data->FullColored = false;
	data->Blured = font.Blured;
	data->BlurWidth = font.BlurWidth;
	data->BlurLevel = font.BlurLevel;
	data->OriginX += aofsx;

	// apply blur
	if(font.Blured) data->Blur(); // nasty ...
	return data;
}
//---------------------------------------------------------------------------
void FreeTypeFontRasterizer::GetGlyphDrawRect( const ttstr & text, tTVPRect& area ) {
	// アンチエイリアスとヒンティングは有効にする
	Face->ClearOption( TVP_FACE_OPTIONS_NO_ANTIALIASING );
	Face->ClearOption( TVP_FACE_OPTIONS_NO_HINTING );

	area.left = area.top = area.right = area.bottom = 0;
	tjs_int offsetx = 0;
	tjs_int offsety = 0;
	tjs_uint len = text.length();
	for( tjs_uint i = 0; i < len; i++ ) {
		tjs_char ch = text[i];
		tjs_int ax, ay;
		tTVPRect rt(0,0,0,0);
		bool result = Face->GetGlyphRectFromCharcode(rt,ch,ax,ay);
		if( result == false ) result = Face->GetGlyphRectFromCharcode(rt,Face->GetDefaultChar(),ax,ay);
		if( result == false ) result = Face->GetGlyphRectFromCharcode(rt,Face->GetFirstChar(),ax,ay);
		if( result ) {
			rt.add_offsets( offsetx, offsety );
			if( i != 0 ) {
				area.do_union( rt );
			} else {
				area = rt;
			}
		}
		offsetx += ax;
		offsety = 0;
	}
}
//---------------------------------------------------------------------------
extern bool TVPAddFontToFreeType( const ttstr& storage, std::vector<tjs_string>* faces );
bool FreeTypeFontRasterizer::AddFont( const ttstr& storage, std::vector<tjs_string>* faces ) {
	return TVPAddFontToFreeType( storage, faces );
}
//---------------------------------------------------------------------------
extern void TVPGetFontListFromFreeType(std::vector<ttstr> & list, tjs_uint32 flags, const tTVPFont & font );
void FreeTypeFontRasterizer::GetFontList(std::vector<ttstr> & list, tjs_uint32 flags, const struct tTVPFont & font ) {
//#ifdef _WIN32
//	TVPGetFontList( list, flags, font );
//#endif
	TVPGetFontListFromFreeType( list, flags, font );
}
//---------------------------------------------------------------------------

