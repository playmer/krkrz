/**
 * Offscreen クラス
 */

#ifndef OffscreenIntfH
#define OffscreenIntfH

#include "tjsNative.h"
#include "GLFrameBufferObject.h"
#include "GLVertexBufferObject.h"
#include "TextureInfo.h"

class tTJSNI_Offscreen : public tTJSNativeInstance, public iTVPTextureInfoIntrface
{
	GLFrameBufferObject	FrameBuffer;
	GLVertexBufferObject VertexBuffer;

public:
	tTJSNI_Offscreen();
	~tTJSNI_Offscreen() override;
	tjs_error TJS_INTF_METHOD Construct(tjs_int numparams, tTJSVariant **param, iTJSDispatch2 *tjs_obj) override;
	void TJS_INTF_METHOD Invalidate() override;

	// 引数が1つだけの時は、全体コピーでBitmap側のサイズをOffscreenに合わせる
	void CopyToBitmap( class tTJSNI_Bitmap* bmp );
	void CopyToBitmap( class tTJSNI_Bitmap* bmp, tjs_int sleft, tjs_int stop, tjs_int width, tjs_int height, tjs_int dleft, tjs_int dtop );
	void CopyFromBitmap( class tTJSNI_Bitmap* bmp, tjs_int sleft, tjs_int stop, tjs_int width, tjs_int height, tjs_int left, tjs_int top );
	void Update();

	tjs_uint GetWidth() const override;
	tjs_uint GetHeight() const override;
	tjs_int64 GetNativeHandle() const override;
	tjs_int64 GetVBOHandle() const override;

	/**
	 * 描画対象に設定する
	 */
	void BindFrameBuffer();
};


//---------------------------------------------------------------------------
// tTJSNC_Offscreen : TJS Offscreen class
//---------------------------------------------------------------------------
class tTJSNC_Offscreen : public tTJSNativeClass
{
public:
	tTJSNC_Offscreen();
	static tjs_uint32 ClassID;

protected:
	tTJSNativeInstance *CreateNativeInstance() override { return new tTJSNI_Offscreen(); }
};

//---------------------------------------------------------------------------
extern tTJSNativeClass * TVPCreateNativeClass_Offscreen();
#endif
