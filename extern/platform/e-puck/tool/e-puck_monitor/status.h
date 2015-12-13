#ifndef _STATUS_H
#define _STATUS_H

#include <vcl.h>
#include "main.h"

void StatusLineErr(const AnsiString &str, int err);
void StatusLine(const AnsiString &str);

void StatusLineErr(const AnsiString &str, int err)
{
	//Application->MessageBox(str, "Error", IDOK);
    dynamic_cast<TMainForm*>(Application->MainForm)->AppendStatus(str + "(error = " + AnsiString(err) + ")");
}

void StatusLine(const AnsiString &str)
{
	//Application->MessageBox(str, "Error", IDOK);
    dynamic_cast<TMainForm*>(Application->MainForm)->AppendStatus(str);
}

#endif
