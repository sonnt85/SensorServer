/*
AWind.h - Arduino window library support for Color TFT LCD Boards
Copyright (C)2015 Andrei Degtiarev. All right reserved


You can always find the latest version of the library at
https://github.com/AndreiDegtiarev/AWind


This library is free software; you can redistribute it and/or
modify it under the terms of the CC BY-NC-SA 3.0 license.
Please see the included documents for further information.

Commercial use of this library requires you to buy a license that
will allow commercial use. This includes using the library,
modified or not, as a tool to sell products.

The license applies to all part of the library including the
examples and tools supplied with the library.
*/
#pragma once
#include "MainWindow.h"
#include "Button.h"

///TabControl. Control element wich allows intersactive switch between chidl windows
class TabControl : public Window, public ITouchEventReceiver
{
	LinkedList<Button> _listButtons; //list of buttons on the left scrren side
	LinkedList<Window> _listWindow;          //list of depended windows (
	static const int _szy = 30;
public:
	///Constructor
	/**
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	TabControl(const __FlashStringHelper * name, int left, int top, int width, int height):Window(name, left, top, width, height)
	{
		SetDecorators(Environment::Get()->FindDecorators(F("TabControl")));
		AddChild(new Window(F("Back tab buttons"), 0, 0, width, _szy));
		Children()[0]->SetDecorators(Environment::Get()->FindDecorators(F("BackTabButtons")));
	}
	///Adds pair: button + corresponding window. The size of added window is adjusted automatically
	void AddTab(const __FlashStringHelper *buttonName,Window *window)
	{
		int wnd_width=Width();
		int wnd_height=Height();
		int x=0;
		int y=0;
		Button * button=new Button(0,0,0,0,buttonName);
		button->SetMargins(5,7);
		button->RegisterTouchEventReceiver(this);
		Children()[0]->AddChild(button);
		AddChild(window);
		window->Move(1, _szy+1, wnd_width-2,wnd_height - _szy-2);
		_listButtons.Add(button);
        _listWindow.Add(window);
		if(_listWindow.Count()>1)
			window->SetVisible(false);
		else
			_listButtons[0]->SetDecorators(Environment::Get()->FindDecorators(F("ActiveTabButton")));
		UpdateTabs(0);
	}
	///If new tab is selected, the size of buttons and their appearance are adjusted
	void UpdateTabs(int sel_index)
	{
		int szx = Width() / _listButtons.Count();

		for (int i = 0;i < _listButtons.Count();i++)
		{
			int offset_y = i == sel_index ? 0 : 3;
			_listButtons[i]->Move(0 + szx*i, offset_y, szx - 1, _szy- offset_y);
		}
		Children()[0]->Invalidate();
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		int sel_index=-1;
		for(int i=0;i<_listButtons.Count();i++)
		{
			if(window == _listButtons[i])
			{
				sel_index=i;
				break;
			}
		}
		if(sel_index >=0)
		{
			UpdateTabs(sel_index);
			DecoratorList *buttonDeco=Environment::Get()->FindDecorators(F("Button"));
			for(int i=0;i<_listButtons.Count();i++)
			{
				_listWindow[i]->SetVisible(i==sel_index);
				if(i==sel_index)
				{
					_listWindow[i]->Invalidate();
					_listButtons[i]->SetDecorators(Environment::Get()->FindDecorators(F("ActiveTabButton")));
				}
				else
				{
					if(_listButtons[i]->GetDecorators()!=buttonDeco)
					{
						_listButtons[i]->SetDecorators(buttonDeco);
						//_listButtons[i]->Invalidate();
					}
				}
			}
		}
		//out<<F("Window selected: ")<<sel_index<<endl;
	}
};