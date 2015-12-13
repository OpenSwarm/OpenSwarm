object MainForm: TMainForm
  Left = 302
  Top = 197
  Width = 776
  Height = 678
  Caption = 'e-puck monitor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object Display0: TImage
    Left = 48
    Top = 168
    Width = 185
    Height = 185
  end
  object Label26: TLabel
    Left = 48
    Top = 20
    Width = 19
    Height = 13
    Caption = 'Port'
  end
  object Label1: TLabel
    Left = 251
    Top = 204
    Width = 31
    Height = 13
    Caption = 'Height'
  end
  object Label2: TLabel
    Left = 254
    Top = 227
    Width = 28
    Height = 13
    Caption = 'Width'
  end
  object Label3: TLabel
    Left = 254
    Top = 251
    Width = 27
    Height = 13
    Caption = 'Zoom'
  end
  object Shape1: TShape
    Left = 504
    Top = 112
    Width = 177
    Height = 177
    Brush.Color = clLime
    ParentShowHint = False
    Shape = stCircle
    ShowHint = True
  end
  object Shape2: TShape
    Left = 560
    Top = 200
    Width = 65
    Height = 65
    Brush.Color = clBlack
    Enabled = False
    Shape = stCircle
    OnMouseMove = Shape2MouseMove
  end
  object Label4: TLabel
    Left = 611
    Top = 396
    Width = 75
    Height = 13
    Caption = 'Micro Amplitude'
  end
  object CGauge1: TCGauge
    Left = 336
    Top = 416
    Width = 25
    Height = 185
    Kind = gkVerticalBar
    ShowText = False
    BackColor = clBlue
    MaxValue = 180
    Progress = 160
  end
  object Pie1: TPie
    Left = 368
    Top = 416
    Width = 185
    Height = 185
    Angles.StartAngle = 88
    Angles.EndAngle = 92
    Brush.Color = clRed
    Pen.Color = clRed
  end
  object Label5: TLabel
    Left = 379
    Top = 388
    Width = 120
    Height = 13
    Caption = 'Accelerometer orientation'
  end
  object Shape3: TShape
    Left = 48
    Top = 392
    Width = 209
    Height = 209
    Brush.Color = clNavy
    Enabled = False
    OnMouseDown = Shape3MouseDown
  end
  object Shape4: TShape
    Left = 136
    Top = 480
    Width = 41
    Height = 41
    Brush.Color = clRed
    Enabled = False
    OnMouseDown = Shape4MouseDown
  end
  object Label6: TLabel
    Left = 147
    Top = 492
    Width = 22
    Height = 13
    Caption = 'Stop'
    Color = clRed
    Enabled = False
    ParentColor = False
    OnMouseDown = Shape4MouseDown
  end
  object Label7: TLabel
    Left = 123
    Top = 372
    Width = 64
    Height = 13
    Caption = 'Click to move'
  end
  object Label8: TLabel
    Left = 459
    Top = 308
    Width = 39
    Height = 13
    Caption = 'Selector'
  end
  object Label9: TLabel
    Left = 667
    Top = 308
    Width = 44
    Height = 13
    Caption = 'IR check'
  end
  object Label10: TLabel
    Left = 667
    Top = 324
    Width = 51
    Height = 13
    Caption = 'IR address'
  end
  object Label11: TLabel
    Left = 667
    Top = 340
    Width = 35
    Height = 13
    Caption = 'IR data'
  end
  object Connect: TButton
    Left = 40
    Top = 48
    Width = 121
    Height = 33
    Caption = 'Connect'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = ConnectClick
  end
  object Pause: TButton
    Left = 40
    Top = 88
    Width = 121
    Height = 33
    Caption = 'Pause'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = PauseClick
  end
  object PortEdt: TEdit
    Left = 80
    Top = 16
    Width = 81
    Height = 21
    TabOrder = 2
    Text = 'COM6'
  end
  object Height: TEdit
    Left = 296
    Top = 200
    Width = 41
    Height = 21
    TabOrder = 3
    Text = '40'
  end
  object Width: TEdit
    Left = 296
    Top = 224
    Width = 41
    Height = 21
    TabOrder = 4
    Text = '40'
  end
  object CheckBox1: TCheckBox
    Left = 243
    Top = 167
    Width = 129
    Height = 25
    Caption = 'Rotate -90°'
    Checked = True
    State = cbChecked
    TabOrder = 5
  end
  object GetImage: TButton
    Left = 256
    Top = 280
    Width = 73
    Height = 33
    Caption = 'Get image'
    Enabled = False
    TabOrder = 6
    OnClick = Button2Click
  end
  object Zoom: TEdit
    Left = 296
    Top = 248
    Width = 41
    Height = 21
    TabOrder = 7
    Text = '8'
  end
  object continous: TCheckBox
    Left = 248
    Top = 328
    Width = 73
    Height = 17
    Caption = 'continous'
    Enabled = False
    TabOrder = 8
    OnClick = continousClick
  end
  object ProxiBar0: TProgressBar
    Left = 600
    Top = 32
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 9
  end
  object LedBox0: TCheckBox
    Left = 584
    Top = 96
    Width = 17
    Height = 25
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 10
    OnClick = LedBox0Click
  end
  object LedBox1: TCheckBox
    Left = 648
    Top = 120
    Width = 17
    Height = 25
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 11
    OnClick = LedBox1Click
  end
  object LedBox2: TCheckBox
    Left = 672
    Top = 184
    Width = 17
    Height = 25
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 12
    OnClick = LedBox2Click
  end
  object LedBox3: TCheckBox
    Left = 648
    Top = 248
    Width = 17
    Height = 25
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 13
    OnClick = LedBox3Click
  end
  object LedBox4: TCheckBox
    Left = 584
    Top = 280
    Width = 17
    Height = 25
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 14
    OnClick = LedBox4Click
  end
  object LedBox5: TCheckBox
    Left = 520
    Top = 248
    Width = 17
    Height = 25
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 15
    OnClick = LedBox5Click
  end
  object LedBox6: TCheckBox
    Left = 496
    Top = 184
    Width = 17
    Height = 25
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 16
    OnClick = LedBox6Click
  end
  object LedBox7: TCheckBox
    Left = 520
    Top = 128
    Width = 17
    Height = 25
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 17
    OnClick = LedBox7Click
  end
  object ProxiBar1: TProgressBar
    Left = 640
    Top = 48
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 18
  end
  object ProxiBar3: TProgressBar
    Left = 616
    Top = 288
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 19
  end
  object ProxiBar4: TProgressBar
    Left = 560
    Top = 288
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 20
  end
  object ProxiBar6: TProgressBar
    Left = 536
    Top = 48
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 21
  end
  object ProxiBar7: TProgressBar
    Left = 576
    Top = 32
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 22
  end
  object ProxiBar2: TProgressBar
    Left = 696
    Top = 128
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 23
  end
  object ProxiBar5: TProgressBar
    Left = 480
    Top = 128
    Width = 8
    Height = 80
    Min = 0
    Max = 4095
    Orientation = pbVertical
    ParentShowHint = False
    Position = 500
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 24
  end
  object Cam_para: TButton
    Left = 344
    Top = 280
    Width = 73
    Height = 33
    Caption = 'Send para'
    Enabled = False
    TabOrder = 25
    OnClick = Cam_paraClick
  end
  object ColorRadio: TRadioButton
    Left = 352
    Top = 232
    Width = 65
    Height = 17
    Caption = 'Color'
    Checked = True
    TabOrder = 26
    TabStop = True
  end
  object BWRadio: TRadioButton
    Left = 352
    Top = 248
    Width = 65
    Height = 17
    Caption = 'Grayscale'
    TabOrder = 27
  end
  object BodyLed: TCheckBox
    Left = 584
    Top = 168
    Width = 17
    Height = 17
    Caption = 'BodyLed'
    Color = clLime
    Enabled = False
    ParentColor = False
    TabOrder = 28
    OnClick = BodyLedClick
  end
  object FrontLed: TCheckBox
    Left = 616
    Top = 112
    Width = 17
    Height = 33
    BiDiMode = bdLeftToRight
    Color = clRed
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentBiDiMode = False
    ParentColor = False
    ParentFont = False
    TabOrder = 29
    OnClick = FrontLedClick
  end
  object Micro2: TProgressBar
    Left = 616
    Top = 416
    Width = 17
    Height = 105
    Min = 0
    Max = 500
    Orientation = pbVertical
    ParentShowHint = False
    Position = 23
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 30
  end
  object Micro1: TProgressBar
    Left = 664
    Top = 416
    Width = 17
    Height = 105
    Min = 0
    Max = 500
    Orientation = pbVertical
    ParentShowHint = False
    Position = 23
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 31
  end
  object Micro3: TProgressBar
    Left = 640
    Top = 440
    Width = 17
    Height = 105
    Min = 0
    Max = 500
    Orientation = pbVertical
    ParentShowHint = False
    Position = 23
    Smooth = True
    Step = 1000
    ShowHint = True
    TabOrder = 32
  end
  object Edit1: TEdit
    Left = 504
    Top = 304
    Width = 25
    Height = 21
    TabOrder = 33
    Text = '0'
  end
  object Button1: TButton
    Left = 264
    Top = 48
    Width = 97
    Height = 33
    Caption = 'Test all actuators'
    Enabled = False
    TabOrder = 34
    OnClick = Button1Click
  end
  object IRcheck: TEdit
    Left = 720
    Top = 304
    Width = 25
    Height = 21
    TabOrder = 35
    Text = '0'
  end
  object IRaddress: TEdit
    Left = 720
    Top = 320
    Width = 25
    Height = 21
    TabOrder = 36
    Text = '0'
  end
  object IRdata: TEdit
    Left = 720
    Top = 336
    Width = 25
    Height = 21
    TabOrder = 37
    Text = '0'
  end
  object CameraTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = CameraTimerTimer
    Left = 182
    Top = 120
  end
  object TimerSensor: TTimer
    Enabled = False
    Interval = 50
    OnTimer = TimerSensorTimer
    Left = 688
    Top = 248
  end
  object Microtimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = MicrotimerTimer
    Left = 688
    Top = 480
  end
  object TestTimer: TTimer
    Enabled = False
    OnTimer = TestTimerTimer
    Left = 376
    Top = 48
  end
end
