# include <Siv3D.hpp>
#include <emscripten.h>
#include <emscripten/val.h>

Size GetViewportSize()
{
    int width = EM_ASM_INT({
        return window.innerWidth;
		});

    int height = EM_ASM_INT({
        return window.innerHeight;
        });

	return Size(width, height);
}

Size GetCanvasSize()
{
    int width = EM_ASM_INT({
        // Module.canvas が存在しない場合、id="canvas" の要素を参照
        var canvas = Module.canvas || document.getElementById('canvas');
        return canvas ? canvas.getBoundingClientRect().width : 0;
        });

    int height = EM_ASM_INT({
        var canvas = Module.canvas || document.getElementById('canvas');
        return canvas ? canvas.getBoundingClientRect().height : 0;
        });

    return Size(width, height);
}

struct TouchInfo {
    int32 id;
    double x;
    double y;
};


int32 getTouchesLength()
{
	int32 length = EM_ASM_INT({
		return siv3dActiveTouches.length;
		});
	return length;
}

// タッチ情報を取得する関数
Array<TouchInfo> GetTouchesFromBrowser() {
    Array<TouchInfo> result = Array<TouchInfo>(getTouchesLength());

    EM_ASM({
        const touches = siv3dActiveTouches;

        for (let i = 0; i < touches.length; i++) {
            const touch = touches[i];
            const touchPtr = $0 + i * 24; // TouchInfo のサイズに応じて調整

            setValue(touchPtr, touch.identifier, 'i32');
            setValue(touchPtr + 8, touch.pageX, 'double');
            setValue(touchPtr + 16, touch.pageY, 'double');
        }
        }, result.data());

    return result;
}

// タッチ情報を取得する関数
Array<TouchInfo> GetAdjustedTouchesFromBrowser() {
    Array<TouchInfo> result = Array<TouchInfo>(getTouchesLength());

    EM_ASM({
        const touches = siv3dActiveTouches;

        for (let i = 0; i < touches.length; i++) {
            const touch = touches[i];
            const touchPtr = $0 + i * 24; // TouchInfo のサイズに応じて調整

            const adjusted = siv3dAdjustPoint(touch.pageX, touch.pageY);

            setValue(touchPtr, touch.identifier, 'i32');
            setValue(touchPtr + 8, adjusted.x, 'double');
            setValue(touchPtr + 16, adjusted.y, 'double');
        }
        }, result.data());

    return result;
}

int32 getTouchesLength2()
{
    int32 length = EM_ASM_INT({
		return Object.keys(Browser.touches).length;
        });
    return length;
}

Vec2 getMousePos()
{
    Vec2 v{};
	
	v.x = EM_ASM_DOUBLE({
		return Browser.mouseX;
		});
	v.y = EM_ASM_DOUBLE({
		return Browser.mouseY;
		});
	return v;
}

bool getPointerLock() {
	int32 r = EM_ASM_INT({
		return Browser.pointerLock;
		});
	return r != 0;
}





void Main()
{
    // JavaScriptのタッチイベントハンドラをセットアップ
    //setupMultiTouchHandler();
    
	//TextAreaEditState state;

    Window::Resize(800, 900);

	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetResizeMode(ResizeMode::Keep);
    /*
    function siv3dOnTouchMove(e) {
        siv3dActiveTouches = Array.from(e.touches);
        // e.stopPropagation();
    }

    function siv3dOnTouchStart(e) {
        siv3dActiveTouches = Array.from(e.touches);
        // e.preventDefault()
    }
    function _siv3dRegisterTouchCallback() {
        Module["canvas"].addEventListener("touchstart", siv3dOnTouchStart);
        Module["canvas"].addEventListener("touchmove", siv3dOnTouchMove);
    }
    にならってtouchendの処理をC++側で追記
    */

	
    EM_ASM({
    Module["canvas"].addEventListener("touchend", function(e) {
        siv3dActiveTouches = Array.from(e.touches);
    });
        });
    
    while (System::Update())
    { 
        ClearPrint();

        //Scene::Resize(GetCanvasSize());

		Print << U"v26";

        Print << U"Cursor::Pos() : " << Cursor::Pos();
		Print << U"Cursor::ScreenPos() : " << Cursor::ScreenPos();
		Print << U"Cursor::PosRaw() : " << Cursor::PosRaw();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"MouseR.pressed() : " << MouseR.pressed();
		Print << U"MouseR.down() : " << MouseR.down();
		Print << U"MouseR.up() : " << MouseR.up();

		Print << U"GetViewportSize() : " << GetViewportSize();

		Print << U"GetCanvasSize() : " << GetCanvasSize();

		Print << U"Scene::Size() : " << Scene::Size();

		Print << U"Window::GetState().bounds : " << Window::GetState().bounds;

        Print << U"Window::GetState().frameBufferSize : " << Window::GetState().frameBufferSize;
        Print << U"Window::GetState().frameSize : " << Window::GetState().frameSize;



        Print << U"Window::GetState().virtualSize : " << Window::GetState().virtualSize;

        //Print << GetCanvasSize();
		Print << U"getMousePos() : " << getMousePos();

        Print << U"getTouchesLength() : " << getTouchesLength();
        Print << U"getTouchesLength2() : " << getTouchesLength2();
		Print << U"getPointerLock() : " << getPointerLock();

        

        const auto touches = GetTouchesFromBrowser();

		for (const auto& touch : touches)
		{
			Print << U"Touch ID: " << touch.id << U", X: " << touch.x << U", Y: " << touch.y;
		}

        const auto adjTouches = GetAdjustedTouchesFromBrowser();
		Print << U"Adjusted Touches:";
        for (const auto& touch : adjTouches)
        {
            Print << U"Touch ID: " << touch.id << U", X: " << touch.x << U", Y: " << touch.y;
        }

        for (const auto& touch : adjTouches) {
            Vec2 pos = Scene::ClientToScene({ touch.x,touch.y });
            Circle(pos, 15).drawFrame(1, Palette::Blue);
        }


        Scene::Rect().drawFrame(3, 0, Palette::Blue);
        Scene::Rect().drawFrame(0, 3, Palette::Red);




		Circle(Cursor::Pos(), 10).drawFrame(1,Palette::Red);

		//SimpleGUI::TextArea(state, { 300,100 });

    }
}
