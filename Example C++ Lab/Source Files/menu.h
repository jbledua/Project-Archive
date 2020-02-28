//-----------------------------------------------------------------------------
// PROJECT      : INDEX
// FILE         : MENU.H
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
//-----------------------------------------------------------------------------
// INCLUTION LOCK
//-----------------------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------------------
// INCLUDE FILES
//-----------------------------------------------------------------------------

// Not including global.h so it cam be used in other projects
#include <conio.h>
#include <iostream>
#include <vector>
#include <string>

#include <tuple>
#include <functional>


//-----------------------------------------------------------------------------
// DEFINITIONS
//-----------------------------------------------------------------------------

#define ESC 27
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define ENTER '\r'

#define PAUSE system("pause")
#define CLEAR system("CLS")

typedef std::vector<char> KeyLst;
typedef std::vector<std::string> TextLst;

//-----------------------------------------------------------------------------
// Needed for the Action class 
//-----------------------------------------------------------------------------

namespace helper
{
	template <int... Is>
	struct index {};

	template <int N, int... Is>
	struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

	template <int... Is>
	struct gen_seq<0, Is...> : index<Is...> {};
}

//-----------------------------------------------------------------------------
// CLASS    : Option
// PURPOSE  : The base class representing each option of the menu
//-----------------------------------------------------------------------------

class Option
{
public:
	Option() { return; }
	Option(std::vector<char> *pKey, std::vector <std::string> *pText) : Key(*pKey), Text(*pText) {}
	std::vector<std::string> Text;
	std::vector<char> Key;

	virtual void act(void) {}
};

//-----------------------------------------------------------------------------
// CLASS    : Action
// PURPOSE  : The derived class that is stores a passed function w/ args to be called later 
//-----------------------------------------------------------------------------

template <typename returnType, typename... agrsType>
class Action : public Option
{
private:
	std::function<returnType(agrsType...)> Function;
	std::tuple<agrsType...> args;
public:
	template <typename F, typename... Args>
	Action(std::vector<char> * pKey, std::vector<std::string> * pText, F&& func, Args&&... args)
		: Function(std::forward<F>(func)), args(std::forward<Args>(args)...) 
	{
		this->Key = *pKey;
		this->Text = *pText;
	}

	template <typename... Args, int... Is>
	returnType func(std::tuple<Args...>& tup, helper::index<Is...>)
	{
		return Function(std::get<Is>(tup)...);
	}

	template <typename... Args>
	returnType func(std::tuple<Args...>& tup)
	{
		return func(tup, helper::gen_seq<sizeof...(Args)>{});
	}

	void act()
	{
		func(args);
		return;
	}
};

//-----------------------------------------------------------------------------
// CLASS    : Menu()
// PURPOSE  : The general menu class to that contains the option/actions
//-----------------------------------------------------------------------------

class Menu
{
public:
	Menu() { return; }
	Menu(std::string psTitle){ this->Title = psTitle + "\n"; }
	Menu(std::string, bool);

	bool Show(void);
	void AddLine(Option* pAction) { this->Line.push_back(pAction); }
	void AddLine(char pKey, std::string pText){ this->Line.push_back(new Option(new KeyLst{ pKey }, new TextLst{ pText })); }
	std::string * getTitlePtr() { return &this->Title; }
private:
	bool Repeating = true;
	void Print();
	void Print(size_t puSelected);

	size_t Cursor = 1;
	
	std::vector<Option*> Line;
	std::string Title = "";
};

//-----------------------------------------------------------------------------
// END MENU.H
//-----------------------------------------------------------------------------

