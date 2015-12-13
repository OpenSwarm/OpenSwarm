//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("EpuckMonitor.res");
USEFORM("main.cpp", MainForm);
USEUNIT("comm.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->Title = "e-puck monitor";
         Application->CreateForm(__classid(TMainForm), &MainForm);
         Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
