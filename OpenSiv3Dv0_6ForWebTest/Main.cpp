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

// JavaScript から取得したタッチ情報を C++ で扱うための構造体
//struct TouchInfo {
//    int32 id;
//    double x;
//    double y;
//};
////
//// タッチ情報を取得する関数
//Array<TouchInfo> GetTouchesFromBrowser() {
//    Array<TouchInfo> result;
//
//    EM_ASM({
//        const touches = siv3dActiveTouches;
//
//        for (let i = 0; i < touches.length; i++) {
//            const touch = touches[i];
//            const touchPtr = $0 + i * 16; // TouchInfo のサイズに応じて調整
//
//            setValue(touchPtr, touch.id, 'i32');
//            setValue(touchPtr + 4, touch.x, 'double');
//            setValue(touchPtr + 12, touch.y, 'double');
//        }
//        }, result.data());
//
//    return result;
//}

int getTouchesLength()
{
	int length = EM_ASM_INT({
		return siv3dActiveTouches.length;
		});
	return length;
}

/*
// JavaScriptコードをC++内に埋め込む
EM_JS(void, setupMultiTouchHandler, (), {
    const canvas = Module.canvas || document.getElementById('canvas');

    if (!canvas) {
        console.error("Canvas not found");
        return;
    }

    // タッチ開始時
    canvas.addEventListener('touchstart', (event) = > {
        for (const touch of event.touches) {
            ccall('onTouchStart', null,['number', 'number', 'number'],[touch.identifier, touch.clientX, touch.clientY]);
        }
        event.preventDefault();
    });

    // タッチ移動時
    canvas.addEventListener('touchmove', (event) = > {
        for (const touch of event.touches) {
            ccall('onTouchMove', null,['number', 'number', 'number'],[touch.identifier, touch.clientX, touch.clientY]);
        }
        event.preventDefault();
    });

    // タッチ終了時
    canvas.addEventListener('touchend', (event) = > {
        for (const touch of event.changedTouches) {
            ccall('onTouchEnd', null,['number', 'number', 'number'],[touch.identifier, -1, -1]); // -1, -1でタッチ終了を示す
        }
        event.preventDefault();
    });
});

// タッチ開始時の処理
extern "C" void onTouchStart(int id, int x, int y) {
    Console << U"Touch Start - ID: " << id << U", X: " << x << U", Y: " << y;
}

// タッチ移動時の処理
extern "C" void onTouchMove(int id, int x, int y) {
    Console << U"Touch Move - ID: " << id << U", X: " << x << U", Y: " << y;
}

// タッチ終了時の処理
extern "C" void onTouchEnd(int id, int x, int y) {
    Console << U"Touch End - ID: " << id << U", X: " << x << U", Y: " << y;
}

*/

void Main()
{
    // JavaScriptのタッチイベントハンドラをセットアップ
    //setupMultiTouchHandler();
    
	TextAreaEditState state;

	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetResizeMode(ResizeMode::Keep);
    
    while (System::Update())
    { 
        ClearPrint();

        //Scene::Resize(GetCanvasSize());

		Print << U"v19";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

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

		Print << U"getTouchesLength() : " << getTouchesLength();

        //const auto touches = GetTouchesFromBrowser();



        // タッチ情報に基づいて円を更新または追加
		//Array<Circle> circles;

  //      for (const auto& touch : touches) {
  //          circles.push_back(Circle(touch.x, touch.y, 30));
  //      }

		//Print << circles;

  //      // 円を描画
  //      for (const auto& circle : circles) {
  //          circle.draw(ColorF(1.0, 0.5, 0.0, 0.8));
  //      }


        Scene::Rect().drawFrame(3, 0, Palette::Blue);
        Scene::Rect().drawFrame(0, 3, Palette::Red);




		Circle(Cursor::Pos(), 10).drawFrame(1,Palette::Red);

		SimpleGUI::TextArea(state, { 300,100 });

    }
}
