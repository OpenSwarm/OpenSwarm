//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#include <string.h>

#pragma hdrstop

#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CGRID"
#pragma link "CGAUGES"
#pragma link "piereg"
#pragma resource "*.dfm"
#define SIZE_IMAGE 185  //size in pixel

TMainForm *MainForm;


//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
  : TForm(Owner)
{

  CommPort = new TCommPort();

  StopWaitingForCommands = false;

  // Put display handles into an array
  ImagePacket[0].Display = Display0;

  // Fill displays

    ImagePacket[0].Display->Canvas->Brush->Color = (TColor)0xFFFFFF;
    ImagePacket[0].Display->Canvas->Pen->Color = ImagePacket[0].Display->Canvas->Brush->Color;
    ImagePacket[0].Display->Canvas->Rectangle(0, 0, ImagePacket[0].Display->Width, ImagePacket[0].Display->Height);

}

__fastcall TMainForm::~TMainForm()
{

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ConnectClick(TObject *Sender)
{
  if (Connect->Caption == "Connect") {
    CommPort->SetCommPort(PortEdt->Text.c_str());
    CommPort->SetBaudRate(115200);
    CommPort->SetParity(NOPARITY);      // NOPARITY and friends are #defined in windows.h
    CommPort->SetByteSize(8);
    CommPort->SetStopBits(ONESTOPBIT);  // ONESTOPBIT is also from windows.h
    CommPort->OpenCommPort();
    Connect->Caption = "Disconnect";

    sprintf(TxBuffer,"\r");
    CommPort->WriteBuffer(TxBuffer, strlen(TxBuffer));
    CommPort->FlushCommPort();
    Sleep(100);
    CommPort->PurgeCommPort();
     CameraTimer->Enabled=false;
     LedBox0->Enabled=true;
     LedBox1->Enabled=true;
     LedBox2->Enabled=true;
     LedBox3->Enabled=true;
     LedBox4->Enabled=true;
     LedBox5->Enabled=true;
     LedBox6->Enabled=true;
     LedBox7->Enabled=true;
     LedBox0->Checked=false;
     LedBox1->Checked=false;
     LedBox2->Checked=false;
     LedBox3->Checked=false;
     LedBox4->Checked=false;
     LedBox5->Checked=false;
     LedBox6->Checked=false;
     LedBox7->Checked=false;
     FrontLed->Enabled=true;
     BodyLed->Enabled=true;
     FrontLed->Checked=false;
     BodyLed->Checked=false;
     continous->Enabled=true;
     GetImage->Enabled=true;
     Cam_para->Enabled=true;
     //GetSensor->Enabled=true;
     TimerSensor->Enabled=true;
       Shape2->Enabled=true;
       Shape3->Enabled=true;
       Shape4->Enabled=true;
       Label6->Enabled=true;
      Microtimer->Enabled=true;
       Button1->Enabled=true;
     TxBuffer[0]=-'L';
     TxBuffer[1]=8;
     TxBuffer[2]=0;
     CommPort->WriteBuffer(TxBuffer, 3);
     CommPort->FlushCommPort();
    //StopWaitingForCommands = false;
    //WaitForCommands();

  } else {
    //StopWaitingForCommands = true;
    TxBuffer[0]=-'D';   //stop robot
    TxBuffer[1]=0;
    TxBuffer[2]=0;
    TxBuffer[3]=0;
    TxBuffer[4]=0;
    CommPort->WriteBuffer(TxBuffer, 5);
    CommPort->FlushCommPort();
    TxBuffer[0]=0;          //exit  binary mode
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    sprintf(TxBuffer,"T,0\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->PurgeCommPort();
     TestTimer->Enabled=false;
    CameraTimer->Enabled=false;
    continous->Enabled=false;
    GetImage->Enabled=false;
     LedBox0->Enabled=false;
     LedBox1->Enabled=false;
     LedBox2->Enabled=false;
     LedBox3->Enabled=false;
     LedBox4->Enabled=false;
     LedBox5->Enabled=false;
     LedBox6->Enabled=false;
     LedBox7->Enabled=false;
     Cam_para->Enabled=false;
     FrontLed->Enabled=false;
     BodyLed->Enabled=false;
      TimerSensor->Enabled=false;
      Shape2->Enabled=false;
      Shape3->Enabled=false;
      Shape4->Enabled=false;
      Label6->Enabled=false;
      Microtimer->Enabled=false;
      Button1->Enabled=false;
     // GetSensor->Enabled=false;
    Connect->Caption = "Connect";
    CommPort->CloseCommPort();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::PauseClick(TObject *Sender)
{
  if (Pause->Caption == "Pause") {
    Pause->Caption = "Resume";
  } else {
    Pause->Caption = "Pause";
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WaitForCommands(void)
{
  while (!StopWaitingForCommands) {
    //ProcessCommand();
    Application->ProcessMessages();
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ProcessCommand(void)
{
;
}

void __fastcall TMainForm::RotateImage(unsigned int ID)
{
int i;
struct TImagePacket SourceImage;
  if(ImagePacket[ID].Type)
        for (int i=0; i<ImagePacket[ID].Width*2*ImagePacket[ID].Height; i++)             //copy image
                SourceImage.Pix[i]= ImagePacket[ID].Pix[i];
  else
        for (int i=0; i<ImagePacket[ID].Width*ImagePacket[ID].Height; i++)             //copy image
                SourceImage.Pix[i]= ImagePacket[ID].Pix[i];

  SourceImage.Width = ImagePacket[ID].Width;
  SourceImage.Height = ImagePacket[ID].Height;
  ImagePacket[ID].Height = SourceImage.Width;
  ImagePacket[ID].Width = SourceImage.Height;
  if(ImagePacket[ID].Type)
  for(int xSrc = 0; xSrc < SourceImage.Width; xSrc++)
  {
        for(int ySrc = 0; ySrc < SourceImage.Height; ySrc++)
        {
        //rotate -90
        // pixel de destination :
           int xDst, yDst;
           xDst = ySrc;
           yDst = SourceImage.Width-xSrc-1;
           ImagePacket[ID].Pix[(ImagePacket[ID].Width*2*yDst+2*xDst)]=SourceImage.Pix[SourceImage.Width*2*ySrc+2*xSrc];
           ImagePacket[ID].Pix[(ImagePacket[ID].Width*2*yDst+2*xDst)+1]=SourceImage.Pix[(SourceImage.Width*2*ySrc+2*xSrc)+1];
        }
  }
  else
  for(int xSrc = 0; xSrc < SourceImage.Width; xSrc++)
  {
        for(int ySrc = 0; ySrc < SourceImage.Height; ySrc++)
        {
        //rotate -90
        // pixel de destination :
           int xDst, yDst;
           xDst = ySrc;
           yDst = SourceImage.Width-xSrc-1;
           ImagePacket[ID].Pix[ImagePacket[ID].Width*yDst+xDst]=SourceImage.Pix[SourceImage.Width*ySrc+xSrc];
        }
  }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DisplayImage(unsigned int ID)
{
   
    if (Pause->Caption == "Pause") {
        unsigned int PixNum = ImagePacket[ID].PixNum;

        switch (ImagePacket[ID].Type) {
            case 0:  // Grey-level on 1 byte
                unsigned int Pix;
                signed int Pix_s;
                int line;
                int col;
                char byte;
                Height->Text = ImagePacket[ID].Height;
                Width->Text = ImagePacket[ID].Width;
                if(CheckBox1->Checked)
                    RotateImage(0);

                
                if(ImagePacket[ID].Height<ImagePacket[ID].Width)
                {
                        ImagePacket[ID].Display->Height=(SIZE_IMAGE* ImagePacket[ID].Height)/ImagePacket[ID].Width;
                        ImagePacket[ID].Display->Width=SIZE_IMAGE;
                }
                else{
                    ImagePacket[ID].Display->Width=(SIZE_IMAGE* ImagePacket[ID].Width)/ImagePacket[ID].Height;
                    ImagePacket[ID].Display->Height=SIZE_IMAGE;
                }
                for (int i=0; i<PixNum; i++)
                {
                    line=i/ImagePacket[ID].Width;
                    col=i-line*ImagePacket[ID].Width;

                    Pix= ((unsigned int)ImagePacket[ID].Pix[i])& 0xff;
                    ImagePacket[ID].Display->Canvas->Brush->Color = (TColor)(Pix*65536 + Pix*256 + Pix);
                    ImagePacket[ID].Display->Canvas->Pen->Color = ImagePacket[ID].Display->Canvas->Brush->Color;

                    ImagePacket[ID].Display->Canvas->Rectangle(col*ImagePacket[ID].Display->Width/ImagePacket[ID].Width,line*ImagePacket[ID].Display->Height/ImagePacket[ID].Height,
                                                (col+1)*ImagePacket[ID].Display->Width/ImagePacket[ID].Width,  (line+1)*ImagePacket[ID].Display->Height/ImagePacket[ID].Height );
                }
            break;
            case 1:   // RGB565 image
                unsigned char Pix_1,Pix_2;
                int index;
                int Red_pix, Green_pix, Blue_pix;

                Height->Text = ImagePacket[ID].Height;
                Width->Text = ImagePacket[ID].Width;
                if(CheckBox1->Checked)
                    RotateImage(0);
                if(ImagePacket[ID].Height<ImagePacket[ID].Width)
                {
                    ImagePacket[ID].Display->Height=(SIZE_IMAGE* ImagePacket[ID].Height)/ImagePacket[ID].Width;
                    ImagePacket[ID].Display->Width=SIZE_IMAGE;
                }
                else {
                    ImagePacket[ID].Display->Width=(SIZE_IMAGE* ImagePacket[ID].Width)/ImagePacket[ID].Height;
                    ImagePacket[ID].Display->Height=SIZE_IMAGE;
                }

                for (int line=0; line<ImagePacket[ID].Height; line++)
                 {
                    for (int col=0; col<ImagePacket[ID].Width; col++)
                    {

                        ImagePacket[ID].Display->Height=(ImagePacket[ID].Display->Width* ImagePacket[ID].Height)/ImagePacket[ID].Width;

                         index=line*2*ImagePacket[ID].Width+2*col;
                         Pix_1 =(unsigned char)ImagePacket[ID].Pix[index];
                         Pix_2 =(unsigned char)ImagePacket[ID].Pix[index+1];

                         Red_pix=(int)((Pix_1&0xF8));
                         Green_pix=(int)(((Pix_1 & 0x07)<<5)|((Pix_2 & 0xE0)>>3));
                         Blue_pix=(int)((Pix_2 & 0x1F)<<3);

                         if (Red_pix > 255) Red_pix = 255;
                         if (Green_pix > 255) Green_pix = 255;
                         if (Blue_pix > 255) Blue_pix = 255;
                         int xDst, yDst;

                         xDst = col;
                         yDst = line;

                        ImagePacket[ID].Display->Canvas->Brush->Color = (TColor)(Blue_pix*65536 + Green_pix*256 +Red_pix );
                        ImagePacket[ID].Display->Canvas->Pen->Color = ImagePacket[ID].Display->Canvas->Brush->Color;

                        ImagePacket[ID].Display->Canvas->Rectangle(xDst*ImagePacket[ID].Display->Width/ImagePacket[ID].Width,yDst*ImagePacket[ID].Display->Height/ImagePacket[ID].Height,
                                                    (xDst+1)*ImagePacket[ID].Display->Width/ImagePacket[ID].Width,  (yDst+1)*ImagePacket[ID].Display->Height/ImagePacket[ID].Height) ;
                    }
                 }
            break;
        }
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


void __fastcall TMainForm::CameraTimerTimer(TObject *Sender)
{
    int i;
    unsigned char ID=0;
    CommPort->PurgeCommPort();
    TxBuffer[0]='I';      //-I
    TxBuffer[0]= -TxBuffer[0];
    TxBuffer[1]=0; 
    CommPort->WriteBuffer(TxBuffer, 2);
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer,3);
    ImagePacket[ID].Type = (int) RxBuffer[0];
    ImagePacket[ID].Width = (int)RxBuffer[1];
    ImagePacket[ID].Height = (int)RxBuffer[2];
    if  (ImagePacket[ID].Type==0)
        ImagePacket[ID].PixNum = ImagePacket[ID].Height*ImagePacket[ID].Width;
    else
        ImagePacket[ID].PixNum = ImagePacket[ID].Height*ImagePacket[ID].Width*2;
    CommPort->ReadBytes(RxBuffer,ImagePacket[ID].PixNum);
    for(i=0;i<ImagePacket[ID].PixNum;i++)
         ImagePacket[ID].Pix[i]= RxBuffer[i] ;
    DisplayImage(ID);
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::Button2Click(TObject *Sender)
{
 int i;
    unsigned char ID=0;
     CommPort->PurgeCommPort();
    TxBuffer[0]='I';      //-I
    TxBuffer[0]= -TxBuffer[0];
    TxBuffer[1]= 0;              //end command
    CommPort->WriteBuffer(TxBuffer, 2);
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer,3);
    ImagePacket[ID].Type = (int) RxBuffer[0];
    ImagePacket[ID].Width = (int)RxBuffer[1];
    ImagePacket[ID].Height = (int)RxBuffer[2];
    if  (ImagePacket[ID].Type==0)
        ImagePacket[ID].PixNum = ImagePacket[ID].Height*ImagePacket[ID].Width;
    else
        ImagePacket[ID].PixNum = ImagePacket[ID].Height*ImagePacket[ID].Width*2;
    CommPort->ReadBytes(RxBuffer,ImagePacket[ID].PixNum);
    for(i=0;i<ImagePacket[ID].PixNum;i++)
         ImagePacket[ID].Pix[i]= RxBuffer[i] ;
    DisplayImage(ID);
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::LedBox0Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=0;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}

void __fastcall TMainForm::LedBox1Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=1;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}

void __fastcall TMainForm::LedBox2Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=2;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
void __fastcall TMainForm::LedBox3Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=3;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
void __fastcall TMainForm::LedBox4Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=4;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
void __fastcall TMainForm::LedBox5Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=5;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
void __fastcall TMainForm::LedBox6Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=6;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
void __fastcall TMainForm::LedBox7Click(TObject *Sender)
{
    TxBuffer[0]=-'L';
    TxBuffer[1]=7;
    TxBuffer[2]=2;
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::continousClick(TObject *Sender)
{
if (continous->Checked==false)
   { CameraTimer->Enabled=false;
    continous->Checked=false;  }
else
   { CameraTimer->Enabled=true;
    continous->Checked=true; }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TMainForm::Cam_paraClick(TObject *Sender)
{
    TxBuffer[0]=0;          //exit binary
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    if(ColorRadio->Checked)
        sprintf(TxBuffer,"J,1,%s,%s,%s\r",Width->Text,Height->Text,Zoom->Text);
    else if(BWRadio->Checked)
        sprintf(TxBuffer,"J,0,%s,%s,%s\r",Width->Text,Height->Text,Zoom->Text);
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TimerSensorTimer(TObject *Sender)
{
    float acc,orientation,inclination  ;

    CommPort->PurgeCommPort();
    TxBuffer[0]=-'N';
    TxBuffer[1]=-'A';
    TxBuffer[2]=0;//end command      //
    CommPort->WriteBuffer(TxBuffer, 3);
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer,16);
    ProxiBar0->Position=RxBuffer[0]+RxBuffer[1]*256;
    ProxiBar1->Position=RxBuffer[2]+RxBuffer[3]*256;
    ProxiBar2->Position=RxBuffer[4]+RxBuffer[5]*256;
    ProxiBar3->Position=RxBuffer[6]+RxBuffer[7]*256;
    ProxiBar4->Position=RxBuffer[8]+RxBuffer[9]*256;
    ProxiBar5->Position=RxBuffer[10]+RxBuffer[11]*256;
    ProxiBar6->Position=RxBuffer[12]+RxBuffer[13]*256;
    ProxiBar7->Position=RxBuffer[14]+RxBuffer[15]*256;

    CommPort->ReadBytes(RxBuffer,4);
    long  mantis = (RxBuffer[0] & 0xff) + ((RxBuffer[1] & 0xffl) << 8) + (((RxBuffer[2] &0x7fl) | 0x80) << 16);
    short  exp = (RxBuffer[3] & 0x7f) * 2 + ((RxBuffer[2] & 0x80) ? 1 : 0);
    if (RxBuffer[3] & 0x80)
        mantis = -mantis;
    float flt = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;
    acc=flt;

    CommPort->ReadBytes(RxBuffer,4);
      mantis = (RxBuffer[0] & 0xff) + ((RxBuffer[1] & 0xffl) << 8) + (((RxBuffer[2] &0x7fl) | 0x80) << 16);
    exp = (RxBuffer[3] & 0x7f) * 2 + ((RxBuffer[2] & 0x80) ? 1 : 0);
    if (RxBuffer[3] & 0x80)
        mantis = -mantis;
     flt = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;
     orientation=flt;

    CommPort->ReadBytes(RxBuffer,4);
    mantis = (RxBuffer[0] & 0xff) + ((RxBuffer[1] & 0xffl) << 8) + (((RxBuffer[2] &0x7fl) | 0x80) << 16);
    exp = (RxBuffer[3] & 0x7f) * 2 + ((RxBuffer[2] & 0x80) ? 1 : 0);
    if (RxBuffer[3] & 0x80)
        mantis = -mantis;
     flt = (mantis || exp) ? ((float) ldexp (mantis, (exp - 127 - 23))): 0;
      inclination=flt;
      if (orientation < 0 )
          orientation=0;
      if (orientation > 360 )
          orientation=360;
       if (inclination < 0 )
          inclination=0;
      if (inclination > 180 )
         inclination=180;
     Pie1->Angles->StartAngle=orientation-2+90;
     Pie1->Angles->EndAngle=orientation+2+90;
     CGauge1->Progress=  (int) inclination;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Shape2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{

    TxBuffer[0]=0;          //exit
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    if((X>30)&& (X<40)&&(Y>30)&& (Y<40)){
        sprintf(TxBuffer,"T,1\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        Sleep(500);
        CommPort->PurgeCommPort();
    }
    if((X>30+10)&& (X<40+10)&&(Y>30+10)&& (Y<40+10)){
        sprintf(TxBuffer,"T,2\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        Sleep(500);
        CommPort->PurgeCommPort();
    }
    if((X>30-10)&& (X<40-10)&&(Y>30-10)&& (Y<40-10)){
        sprintf(TxBuffer,"T,3\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        Sleep(500);
        CommPort->PurgeCommPort();
    }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------


void __fastcall TMainForm::BodyLedClick(TObject *Sender)
{
    TxBuffer[0]=0;          //exit binary
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();

    sprintf(TxBuffer,"B,2\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FrontLedClick(TObject *Sender)
{
    TxBuffer[0]=0;          //exit binary
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    sprintf(TxBuffer,"F,2\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::MicrotimerTimer(TObject *Sender)
{
    unsigned char i=0;
    int micro1,micro2,micro3,selector;
    int ir_check,ir_address,ir_data   ;
    TxBuffer[0]=0;          //exit binary
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    sprintf(TxBuffer,"U\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    do {
        CommPort->ReadBytes(&RxBuffer[i],1);
        i++;
    }
    while( RxBuffer[i-1] !=  '\n')  ;
    sscanf(RxBuffer,"u,%d,%d,%d\n\r",&micro1 ,&micro2,&micro3);
    Micro1->Position=micro1;
    Micro2->Position=micro2;
    Micro3->Position=micro3;
    CommPort->PurgeCommPort();
    sprintf(TxBuffer,"C\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    i=0;
    do {
        CommPort->ReadBytes(&RxBuffer[i],1);
        i++;
    }
    while( RxBuffer[i-1] !=  '\n')  ;
    sscanf(RxBuffer,"c,%d\n\r",&selector);
    Edit1->Text=selector;

    CommPort->PurgeCommPort();
    sprintf(TxBuffer,"G\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    i=0;
    do {
        CommPort->ReadBytes(&RxBuffer[i],1);
        i++;
    }
    while( RxBuffer[i-1] !=  '\n')  ;
    sscanf(RxBuffer,"g IR check : 0x%x, address : 0x%x, data : 0x%x\r\n",&ir_check,&ir_address,&ir_data);
    IRcheck->Text=ir_check;
    IRaddress->Text=ir_address;
    IRdata->Text=ir_data;
    CommPort->PurgeCommPort();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::Shape3MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int speedright,speedleft;
    int maxspeed=1000;
    //if(Button
        speedright=-(((float)Y/(float)Shape3->Width-0.5)+((float)X/(float)Shape3->Width-0.5))*maxspeed;
        speedleft=-(((float)Y/(float)Shape3->Width-0.5)-((float)X/(float)Shape3->Width-0.5))*maxspeed;
    TxBuffer[0]=-'D';
    TxBuffer[1]=speedleft&0xff;
    TxBuffer[2]=speedleft>>8;
    TxBuffer[3]=speedright&0xff;
    TxBuffer[4]=speedright>>8;
    CommPort->WriteBuffer(TxBuffer, 5);
    CommPort->FlushCommPort();
}
//---------------------------------------------------------------------------




void __fastcall TMainForm::Shape4MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TxBuffer[0]=-'D';
    TxBuffer[1]=0;
    TxBuffer[2]=0;
    TxBuffer[3]=0;
    TxBuffer[4]=0;
    CommPort->WriteBuffer(TxBuffer, 5);
    CommPort->FlushCommPort();
}
//---------------------------------------------------------------------------





void __fastcall TMainForm::Button1Click(TObject *Sender)
{
    if(TestTimer->Enabled==false)
         {
         TestTimer->Enabled=true;
         Button1->Caption="Stop testing" ;
         }

    else {
        TestTimer->Enabled=false ;
        Button1->Caption="Test all actuators";
         TxBuffer[0]=0;          //exit binary
        CommPort->WriteBuffer(TxBuffer, 1);
        CommPort->FlushCommPort();
         sprintf(TxBuffer,"B,0\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        CommPort->PurgeCommPort();
        sprintf(TxBuffer,"F,0\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        CommPort->PurgeCommPort();
        sprintf(TxBuffer,"T,0\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        CommPort->PurgeCommPort();
        sprintf(TxBuffer,"D,0,0\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        CommPort->PurgeCommPort();
        sprintf(TxBuffer,"L,8,0\r");
        CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
        CommPort->FlushCommPort();
        CommPort->ReadBytes(RxBuffer, 1);
        CommPort->PurgeCommPort();
         }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TestTimerTimer(TObject *Sender)
{
    static int testcounter=0;
    short int testspeed=300;
    CommPort->PurgeCommPort();
    if((testcounter&1)==0)
    {
        TxBuffer[0]=-'L';
        TxBuffer[1]=8;
        TxBuffer[2]=0;
        CommPort->WriteBuffer(TxBuffer, 3);
        CommPort->FlushCommPort();
    }
    else
    {
        TxBuffer[0]=-'L';
        TxBuffer[1]=8;
        TxBuffer[2]=1;
        CommPort->WriteBuffer(TxBuffer, 3);
        CommPort->FlushCommPort();
    }
    if(((testcounter/4)&1)==0)
    {
        TxBuffer[0]=-'D';
        TxBuffer[1]=(testspeed)&0xff;
        TxBuffer[2]=(testspeed)>>8;
        TxBuffer[3]=(-testspeed)&0xff;
        TxBuffer[4]=(-testspeed)>>8;
        CommPort->WriteBuffer(TxBuffer, 5);
        CommPort->FlushCommPort();
    }
    else
    {
        TxBuffer[0]=-'D';
        TxBuffer[1]=(-testspeed)&0xff;
        TxBuffer[2]=(-testspeed)>>8;
        TxBuffer[3]=(testspeed)&0xff;
        TxBuffer[4]=(testspeed)>>8;
        CommPort->WriteBuffer(TxBuffer, 5);
        CommPort->FlushCommPort();
    }
    TxBuffer[0]=0;          //exit binary
    CommPort->WriteBuffer(TxBuffer, 1);
    CommPort->FlushCommPort();
    sprintf(TxBuffer,"B,2\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
    sprintf(TxBuffer,"F,2\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
    if((testcounter&1)==0)
        sprintf(TxBuffer,"T,2\r");
    else
        sprintf(TxBuffer,"T,1\r");
    CommPort->WriteBuffer(TxBuffer,strlen(TxBuffer));
    CommPort->FlushCommPort();
    CommPort->ReadBytes(RxBuffer, 1);
    CommPort->PurgeCommPort();
    testcounter++;

}
//---------------------------------------------------------------------------


