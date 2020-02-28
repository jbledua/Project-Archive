//-----------------------------------------------------------------------------
// PROJECT      : INDEX
// FILE         : MENU.CPP
// FILE VERSION : 2.4
// PROGRAMMER   : JOSIAH LEDUA
//-----------------------------------------------------------------------------
// REVISION HISTORY
//-----------------------------------------------------------------------------
//
// 2.4, 2018-12-01, Josiah Ledua
//   - Working w/ all argument type except void types
//   - Function do not return values (must pass by ref)
//
//-----------------------------------------------------------------------------
// MODULE DESCRIPTION
//-----------------------------------------------------------------------------
//
// The main file is a the unviversal menu system to be use for other projects
//
//-----------------------------------------------------------------------------
// INCLUDE FILES
//-----------------------------------------------------------------------------

#include "menu.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// EXTERNALS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTION : Menu()
// PURPOSE  : 
//-----------------------------------------------------------------------------

Menu::Menu(std::string psTitle, bool pRepeating)
{
	this->Title = psTitle + "\n";
	this->Repeating = pRepeating;
}

//-----------------------------------------------------------------------------
// FUNCTION : Print()
// PURPOSE  : Prints the menu without the arrow
//-----------------------------------------------------------------------------

void Menu::Print(void)
{
	std::cout << this->Title;


	for (size_t i = 1; i < this->Line.size(); i++)
	{
		if (this->Line[i]->Key[0] != ESC)
		{
			std::cout << this->Line[i]->Key[0] << "   - " << ((this->Line[i]->Text.size() > 0) ? this->Line[i]->Text[0] : "(No text)") << std::endl;
		}

	}

	std::cout << "ESC - " << ((this->Line[0]->Text.size() > 0) ? this->Line[0]->Text[0] : "(No text)") << std::endl;

	return;
} // End Print()


//-----------------------------------------------------------------------------
// FUNCTION : Print(size_t)
// PURPOSE  : Prints the menu with the selected arrow
//-----------------------------------------------------------------------------

void Menu::Print(size_t puSelected)
{
	char tempChar = 0;

	std::cout << this->Title;

	for (size_t i = 1; i < this->Line.size(); i++)
	{

		if (this->Line[i]->Key[0] != ESC)
		{
			std::cout << ((puSelected == i) ? "->" : "  ") << this->Line[i]->Key[0] << "   - " << ((this->Line[i]->Text.size() > 0) ? this->Line[i]->Text[0] : "(No text)") << std::endl;

		}
	}

	std::cout << ((puSelected == 0) ? "->" : "  ")  << "ESC - " << ((this->Line[0]->Text.size() > 0) ? this->Line[0]->Text[0] : "(No text)") << std::endl;

	return;

} // End Print(size_t)

//-----------------------------------------------------------------------------
// FUNCTION : Show()
// PURPOSE  : Prints the menu with the selected arrow
//-----------------------------------------------------------------------------

bool Menu::Show(void)
{
	bool pbLoopMenu = true, pbInputInValid = true;

	char pcInput = ' ';

	CLEAR;

	// Print Menu
	this->Print(Cursor);

	// Gets 
	pcInput = _getch();

	// Check for option keys pressed
	for (size_t i = 1; i < this->Line.size(); i++)
	{
		for (size_t j = 0; j < this->Line[i]->Key.size(); j++)
		{
			if (pcInput == this->Line[i]->Key[j])
			{
				this->Line[i]->act();
				
				pbInputInValid = false;

				if (!Repeating) pbLoopMenu = false;

				break;
			}
		}

	}

	// Check for arrows & enter
	switch (pcInput)
	{
	case ESC:
		pbLoopMenu = false;
	case DOWN:
	case RIGHT:
		this->Cursor = (this->Cursor >= (this->Line.size() - 1)) ? 0 : (this->Cursor + 1);
		pbInputInValid = false;
		break;
	case UP:
	case LEFT:
		this->Cursor = (this->Cursor < 1) ? (this->Line.size() - 1) : (this->Cursor - 1);
		pbInputInValid = false;
		break;
	case -32: // Ignore arrow key release
		pbInputInValid = false;
		break;
	case ENTER:
		if (Cursor != 0)
		{
			this->Line[Cursor]->act();

			if (!Repeating) pbLoopMenu = false;
		}
		else
			pbLoopMenu = false;

		pbInputInValid = false;
		break;
	}

	// If input is invalid display error msg
	if (pbInputInValid)
	{
		CLEAR;
		printf("%c is invalid. Try again\n", pcInput);
		PAUSE;
	}

	return pbLoopMenu;
} // End Show()

//-----------------------------------------------------------------------------
// END MENU.CPP
//-----------------------------------------------------------------------------