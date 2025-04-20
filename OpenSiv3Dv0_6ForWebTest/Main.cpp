# include <Siv3D.hpp>
#include <emscripten.h>

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

void Main()
{
    
	TextAreaEditState state;

    while (System::Update())
    { 
        ClearPrint();

        Scene::Resize(GetCanvasSize());

		Print << U"v9";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"MouseR.pressed() : " << MouseR.pressed();
		Print << U"MouseR.down() : " << MouseR.down();
		Print << U"MouseR.up() : " << MouseR.up();

		Print << U"GetViewportSize() : " << GetViewportSize();


        //Print << GetCanvasSize();




		Circle(Cursor::Pos(), 10).draw(Palette::Red);

		SimpleGUI::TextArea(state, { 300,100 });

    }
}
