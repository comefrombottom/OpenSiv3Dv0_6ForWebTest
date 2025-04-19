
# include <Siv3D.hpp>

void Main()
{
    

    while (System::Update())
    { 
        ClearPrint();

        Print << U"Cursor::Pos() : " << Cursor::Pos();

		Print << U"MouseL.pressed() : " << MouseL.pressed();
		Print << U"MouseL.down() : " << MouseL.down();
		Print << U"MouseL.up() : " << MouseL.up();

		Print << U"MouseR.pressed() : " << MouseR.pressed();
		Print << U"MouseR.down() : " << MouseR.down();
		Print << U"MouseR.up() : " << MouseR.up();

		Circle(Cursor::Pos(), 10).draw(Palette::Red);

    }
}
