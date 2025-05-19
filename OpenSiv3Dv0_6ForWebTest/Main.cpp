# include <Siv3D.hpp>
#include <emscripten.h>
#include <emscripten/val.h>

struct TouchInfo {
    int32 id;
    Vec2 pos;
};

// �^�b�`�����擾����֐�
Array<TouchInfo> GetMyTouches() {
    Array<TouchInfo> result = Array<TouchInfo>(EM_ASM_INT({ return window.myTouches.length; }));

    EM_ASM({
        const touches = window.myTouches;

        for (let i = 0; i < touches.length; i++) {
            const touch = touches[i];
            const touchPtr = $0 + i * 24; // TouchInfo �̃T�C�Y�ɉ����Ē���

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

EM_JS(void, setupMultiTouchHandler, (), {
    // �O���[�o���ϐ����`
    window.myTouches = [];

    // �^�b�`�C�x���g�̏�����ݒ�
    const canvas = Module['canvas'];

    function updateTouches(e) {
      window.myTouches = Array.from(e.touches);
      //e.preventDefault(); // �C�ӁF�X�N���[���h�~�Ȃ�
    }

    canvas.addEventListener("touchstart", updateTouches, false);
    canvas.addEventListener("touchmove", updateTouches, false);
    canvas.addEventListener("touchend", updateTouches, false);
    });


void Main()
{
    // JavaScript�̃^�b�`�C�x���g�n���h�����Z�b�g�A�b�v
    // setupMultiTouchHandler();
    
	//TextAreaEditState state;

    Window::Resize(800, 900);

	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetResizeMode(ResizeMode::Keep);

	bool pre_pressed = false;

	int32 changeNum = 0;
	int32 downNum = 0;
	int32 upNum = 0;
	
    while (System::Update())
    { 
        ClearPrint();


        //Scene::Resize(GetCanvasSize());

		Print << U"v34";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		if (MouseL.down())
		{
			downNum++;
		}

		if (MouseL.up())
		{
			upNum++;
		}

		if (pre_pressed != MouseL.pressed())
		{
			changeNum++;
		}

		Print << U"changeNum : " << changeNum;
		Print << U"downNum : " << downNum;
		Print << U"upNum : " << upNum;

		pre_pressed = MouseL.pressed();

        Print << U"myTouches :";
        // const auto myTouches = GetMyTouches();
        /*for (const auto& touch : myTouches)
        {
            Print << U"Touch ID: " << touch.id << U", pos: " << touch.pos;
        }

        for (const auto& touch : myTouches) {
            Circle(touch.pos, 30).drawFrame(1, Palette::Blue);
        }*/

		Circle(Cursor::Pos(), 10).drawFrame(1,Palette::Red);

        Scene::Rect().drawFrame(3, 0, Palette::Blue);
        Scene::Rect().drawFrame(0, 3, Palette::Red);

		//SimpleGUI::TextArea(state, { 300,100 });

    }
}
