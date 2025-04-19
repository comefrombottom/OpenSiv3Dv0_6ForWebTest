
# include <Siv3D.hpp>

void Main()
{
    
	TextAreaEditState state;

    while (System::Update())
    { 
        ClearPrint();

		Print << U"v7";

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"MouseR.pressed() : " << MouseR.pressed();
		Print << U"MouseR.down() : " << MouseR.down();
		Print << U"MouseR.up() : " << MouseR.up();

		Circle(Cursor::Pos(), 10).draw(Palette::Red);

		SimpleGUI::TextArea(state, { 300,100 });

    }
}
