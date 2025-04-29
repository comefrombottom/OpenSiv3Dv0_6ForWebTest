# include <Siv3D.hpp>
#include <emscripten.h>
#include <emscripten/val.h>

struct TouchInfo {
    int32 id;
    Vec2 pos;
};

int32 getTouchesLength()
{
	int32 length = EM_ASM_INT({
		return siv3dActiveTouches.length;
		});
	return length;
}


// タッチ情報を取得する関数
Array<TouchInfo> GetAdjustedTouchesFromBrowser() {
    Array<TouchInfo> result = Array<TouchInfo>(EM_ASM_INT({return siv3dActiveTouches.length;}));

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

	for (auto& touch : result)
	{
		touch.pos = Scene::ClientToScene(touch.pos);
	}

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

bool getSDLisDefined()
{
	return EM_ASM_INT({
        if (typeof SDL != "undefined") {
            return 1;
        }
        else {
            return 0;
        }
	});
}



void Main()
{
    // JavaScriptのタッチイベントハンドラをセットアップ
    //setupMultiTouchHandler();
    
	//TextAreaEditState state;

    Window::Resize(800, 900);

	Window::SetStyle(WindowStyle::Sizable);


	Scene::SetResizeMode(ResizeMode::Keep);
	
    EM_ASM({
    Module["canvas"].addEventListener("touchend", function(e) {
        siv3dActiveTouches = Array.from(e.touches);
    });
        });
    
    while (System::Update())
    { 
        ClearPrint();


        //Scene::Resize(GetCanvasSize());

		Print << U"v27";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"getMousePos() : " << getMousePos();

        Print << U"getTouchesLength() : " << getTouchesLength();
        Print << U"getTouchesLength2() : " << getTouchesLength2();
		Print << U"getSDLisDefined() : " << getSDLisDefined();

        

        const auto adjTouches = GetAdjustedTouchesFromBrowser();
        for (const auto& touch : adjTouches)
        {
            Print << U"Touch ID: " << touch.id << U", pos: " << touch.pos;
        }

        for (const auto& touch : adjTouches) {
            Circle(touch.pos, 15).drawFrame(1, Palette::Blue);
        }

		Circle(Cursor::Pos(), 10).drawFrame(1,Palette::Red);

        Scene::Rect().drawFrame(3, 0, Palette::Blue);
        Scene::Rect().drawFrame(0, 3, Palette::Red);

		//SimpleGUI::TextArea(state, { 300,100 });

    }
}
