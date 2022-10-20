///*
//==================================================================================================================================================================
//|Copyright 2018 Oscar Lara	- scarlara@hotmail.com																												   |
//|																																								   |
//|See individual libraries separate legal notices																												   |
//|																																								   |
//|Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),				   |
//|to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,				   |
//|and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :					   |
//|																																								   |
//|The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.									   |
//|																																								   |
//|THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			   |
//|FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,	   |
//|WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.		   |
//==================================================================================================================================================================
//*/
//
#include "console.h"


Console::Console()
{
	mh_out = GetStdHandle(STD_OUTPUT_HANDLE);

	mcon_cur_inf.dwSize = 20;
	mcon_cur_inf.bVisible = FALSE;

	SetConsoleCursorInfo(mh_out, &mcon_cur_inf);

	mcurrent_row = 0;
	mstart_row = 0;
}

Console::~Console()
{
}

void Console::print(const char* data, ...)
{
	Sleep(10);

	mcurrent_row = (mstart_row != 0 && mcurrent_row < mstart_row) ? mstart_row : mcurrent_row;

	va_list argptr;
	va_start(argptr, data);

	//Clear row, before writing to it
	const char* empty_row = "\t\t\t\t";
	gotoxy(0, mcurrent_row);
	printf(empty_row);

	//Go back to cleared row and srite data
	gotoxy(0, mcurrent_row);
	vprintf(data, argptr);
	
	mcurrent_row++;

	va_end(argptr);	

}

void Console::lock_start_row()
{
	mstart_row = mcurrent_row;
}

void Console::reset_start_row()
{
	mstart_row = 0;
	mcurrent_row = 0;
}

void Console::refresh()
{
	mcurrent_row = mstart_row;
}

void Console::gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;	
	SetConsoleCursorPosition(mh_out, coord);
}
