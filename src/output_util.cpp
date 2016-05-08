#include "output_util.h"

#include <iostream>
#include <string>

using namespace std;

void display_progress(float completion)
{
	const int progressbar_width = 20;
	const char progress_symbol = '#';
	string bar_filled(completion * progressbar_width, progress_symbol);
	string bar_blank(20 - bar_filled.length(), ' ');

	cout << "\r[" << bar_filled << bar_blank << "]" << (int)(completion * 100) << "%";
}