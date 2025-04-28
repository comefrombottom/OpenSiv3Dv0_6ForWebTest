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
        // Module.canvas �����݂��Ȃ��ꍇ�Aid="canvas" �̗v�f���Q��
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
//


int getTouchesLength()
{
	int length = EM_ASM_INT({
		return siv3dActiveTouches.length;
		});
	return length;
}

// �^�b�`�����擾����֐�
Array<TouchInfo> GetTouchesFromBrowser() {
    Array<TouchInfo> result = Array<TouchInfo>(getTouchesLength());

    EM_ASM({
        const touches = siv3dActiveTouches;

        for (let i = 0; i < touches.length; i++) {
            const touch = touches[i];
            const touchPtr = $0 + i * 16; // TouchInfo �̃T�C�Y�ɉ����Ē���

            setValue(touchPtr, touch.identifier, 'i32');
            setValue(touchPtr + 4, touch.pageX, 'double');
            setValue(touchPtr + 12, touch.pageY, 'double');
        }
        }, result.data());

    return result;
}


/*
// JavaScript�R�[�h��C++���ɖ��ߍ���
EM_JS(void, setupMultiTouchHandler, (), {
    const canvas = Module.canvas || document.getElementById('canvas');

    if (!canvas) {
        console.error("Canvas not found");
        return;
    }

    // �^�b�`�J�n��
    canvas.addEventListener('touchstart', (event) = > {
        for (const touch of event.touches) {
            ccall('onTouchStart', null,['number', 'number', 'number'],[touch.identifier, touch.clientX, touch.clientY]);
        }
        event.preventDefault();
    });

    // �^�b�`�ړ���
    canvas.addEventListener('touchmove', (event) = > {
        for (const touch of event.touches) {
            ccall('onTouchMove', null,['number', 'number', 'number'],[touch.identifier, touch.clientX, touch.clientY]);
        }
        event.preventDefault();
    });

    // �^�b�`�I����
    canvas.addEventListener('touchend', (event) = > {
        for (const touch of event.changedTouches) {
            ccall('onTouchEnd', null,['number', 'number', 'number'],[touch.identifier, -1, -1]); // -1, -1�Ń^�b�`�I��������
        }
        event.preventDefault();
    });
});

// �^�b�`�J�n���̏���
extern "C" void onTouchStart(int id, int x, int y) {
    Console << U"Touch Start - ID: " << id << U", X: " << x << U", Y: " << y;
}

// �^�b�`�ړ����̏���
extern "C" void onTouchMove(int id, int x, int y) {
    Console << U"Touch Move - ID: " << id << U", X: " << x << U", Y: " << y;
}

// �^�b�`�I�����̏���
extern "C" void onTouchEnd(int id, int x, int y) {
    Console << U"Touch End - ID: " << id << U", X: " << x << U", Y: " << y;
}

*/

void Main()
{
    // JavaScript�̃^�b�`�C�x���g�n���h�����Z�b�g�A�b�v
    //setupMultiTouchHandler();
    
	TextAreaEditState state;

	Window::SetStyle(WindowStyle::Sizable);

	Scene::SetResizeMode(ResizeMode::Keep);
    
    while (System::Update())
    { 
        ClearPrint();

        //Scene::Resize(GetCanvasSize());

		Print << U"v20";

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

        const auto touches = GetTouchesFromBrowser();

		for (const auto& touch : touches)
		{
			Print << U"Touch ID: " << touch.id << U", X: " << touch.x << U", Y: " << touch.y;
		}


        Scene::Rect().drawFrame(3, 0, Palette::Blue);
        Scene::Rect().drawFrame(0, 3, Palette::Red);




		Circle(Cursor::Pos(), 10).drawFrame(1,Palette::Red);

		SimpleGUI::TextArea(state, { 300,100 });

    }
}
