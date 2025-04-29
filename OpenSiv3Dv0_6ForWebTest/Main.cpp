# include <Siv3D.hpp>
#include <emscripten.h>
#include <emscripten/val.h>

struct TouchInfo {
    int32 id;
    Vec2 pos;
};

// タッチ情報を取得する関数
Array<TouchInfo> GetTouchesFromBrowser() {
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

// タッチ情報を取得する関数
Array<TouchInfo> GetMyTouches() {
    Array<TouchInfo> result = Array<TouchInfo>(EM_ASM_INT({ return window.myTouches.length; }));

    EM_ASM({
        const touches = window.myTouches;

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

EM_JS(void, setup_touch_handlers, (), {
    // グローバル変数を定義
    window.myTouches = [];

// タッチイベントの処理を設定
const canvas = Module['canvas'];

function updateTouches(e) {
  window.myTouches = Array.from(e.touches);
  //e.preventDefault(); // 任意：スクロール防止など
}

canvas.addEventListener("touchstart", updateTouches, false);
canvas.addEventListener("touchmove", updateTouches, false);
canvas.addEventListener("touchend", updateTouches, false);
    });


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

	setup_touch_handlers();
    
    while (System::Update())
    { 
        ClearPrint();


        //Scene::Resize(GetCanvasSize());

		Print << U"v28";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"getMousePos() : " << getMousePos();


        const auto adjTouches = GetTouchesFromBrowser();
        for (const auto& touch : adjTouches)
        {
            Print << U"Touch ID: " << touch.id << U", pos: " << touch.pos;
        }

        Print << U"myTouches :";
        const auto myTouches = GetTouchesFromBrowser();
        for (const auto& touch : myTouches)
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
