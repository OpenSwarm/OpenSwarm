//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "comm.h"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "CGRID.h"
#include "CGAUGES.h"
#include <AppEvnts.hpp>
#include "piereg.h"

#include <fstream>

#define NB_DISPLAYS 1
#define NB_NUMBERS 8
#define MAX_PIX 80*40

  struct TImagePacket {
    bool DisplayID;
    unsigned char Type;
    unsigned int PixNum;
    unsigned int Height;
    unsigned int Width;
    byte Pix[2*MAX_PIX];
    TImage *Display;
    TTrackBar *Scaling;
    TEdit *EdScaling;
    TEdit* Label;
  };

  struct TNumberPacket {
    bool ReadFlag;  // true if the number is read from the robot
    bool Signed;
    unsigned char Value;
    TTrackBar *TB;
    TEdit *Ed;
    TEdit* Label;
  };

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
  TButton *Connect;
  TImage *Display0;
  TButton *Pause;

        TEdit *PortEdt;
        TLabel *Label26;
    TEdit *Height;
    TEdit *Width;
        TLabel *Label1;
        TLabel *Label2;
        TCheckBox *CheckBox1;
    TButton *GetImage;
    TTimer *CameraTimer;
    TLabel *Label3;
    TEdit *Zoom;
    TCheckBox *continous;
    TProgressBar *ProxiBar0;
    TShape *Shape1;
    TShape *Shape2;
    TCheckBox *LedBox0;
    TCheckBox *LedBox1;
    TCheckBox *LedBox2;
    TCheckBox *LedBox3;
    TCheckBox *LedBox4;
    TCheckBox *LedBox5;
    TCheckBox *LedBox6;
    TCheckBox *LedBox7;
    TProgressBar *ProxiBar1;
    TProgressBar *ProxiBar3;
    TProgressBar *ProxiBar4;
    TProgressBar *ProxiBar6;
    TProgressBar *ProxiBar7;
    TProgressBar *ProxiBar2;
    TProgressBar *ProxiBar5;
    TButton *Cam_para;
    TTimer *TimerSensor;
    TRadioButton *ColorRadio;
    TRadioButton *BWRadio;
    TCheckBox *BodyLed;
    TCheckBox *FrontLed;
    TProgressBar *Micro2;
    TProgressBar *Micro1;
    TProgressBar *Micro3;
    TLabel *Label4;
    TTimer *Microtimer;
    TCGauge *CGauge1;
    TPie *Pie1;
    TLabel *Label5;
    TShape *Shape3;
    TShape *Shape4;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *Edit1;
    TButton *Button1;
    TTimer *TestTimer;
    TLabel *Label9;
    TEdit *IRcheck;
    TLabel *Label10;
    TEdit *IRaddress;
    TLabel *Label11;
    TEdit *IRdata;
  void __fastcall ConnectClick(TObject *Sender);
  void __fastcall PauseClick(TObject *Sender);
    void __fastcall CameraTimerTimer(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall LedBox0Click(TObject *Sender);
    void __fastcall LedBox1Click(TObject *Sender);
    void __fastcall LedBox2Click(TObject *Sender);
    void __fastcall LedBox3Click(TObject *Sender);
    void __fastcall LedBox4Click(TObject *Sender);
    void __fastcall LedBox5Click(TObject *Sender);
    void __fastcall LedBox6Click(TObject *Sender);
    void __fastcall LedBox7Click(TObject *Sender);
    void __fastcall continousClick(TObject *Sender);
    void __fastcall Cam_paraClick(TObject *Sender);
    void __fastcall TimerSensorTimer(TObject *Sender);
    void __fastcall Shape2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall BodyLedClick(TObject *Sender);
    void __fastcall FrontLedClick(TObject *Sender);
    void __fastcall MicrotimerTimer(TObject *Sender);
    void __fastcall Shape3MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Shape4MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall Button1Click(TObject *Sender);
    void __fastcall TestTimerTimer(TObject *Sender);


       // void __fastcall Label5Click(TObject *Sender);
      //  void __fastcall Edit1Change(TObject *Sender);

private:	// User declarations
  bool StopWaitingForCommands;

  TCommPort *CommPort;
  char TxBuffer[100];
  unsigned char RxBuffer[8000];

  signed short Encoders[2];
  signed char MotorCommands[2];

  TImagePacket ImagePacket[NB_DISPLAYS];

  TNumberPacket NumberPacket[NB_NUMBERS];

public:		// User declarations
  __fastcall TMainForm(TComponent* Owner);
  __fastcall virtual ~TMainForm();
  void __fastcall WaitForCommands(void);
  void __fastcall ProcessCommand(void);
  void __fastcall DisplayImage(unsigned int ID);
  void __fastcall RotateImage(unsigned int ID);
  void __fastcall DisplayNumber(unsigned char ID);

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
