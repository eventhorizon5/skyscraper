VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "Mscomctl.ocx"
Begin VB.Form DebugPanel 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper 0.95 Beta - Simulator Control Panel"
   ClientHeight    =   5280
   ClientLeft      =   1830
   ClientTop       =   1710
   ClientWidth     =   6870
   Icon            =   "Debug.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5280
   ScaleWidth      =   6870
   Begin VB.CommandButton Command17 
      Caption         =   "Add Object"
      Height          =   375
      Left            =   120
      TabIndex        =   68
      Top             =   4800
      Width           =   1455
   End
   Begin VB.CommandButton Command16 
      Caption         =   "InitRealtime"
      Height          =   255
      Left            =   5160
      TabIndex        =   67
      Top             =   4920
      Width           =   975
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Call Buttons"
      Height          =   255
      Left            =   5040
      TabIndex        =   36
      Top             =   1800
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton Command13 
      Caption         =   "Land Off"
      Height          =   255
      Left            =   5640
      TabIndex        =   35
      Top             =   1440
      Width           =   855
   End
   Begin VB.CommandButton Command12 
      Caption         =   "Land On"
      Height          =   255
      Left            =   4680
      TabIndex        =   34
      Top             =   1440
      Width           =   855
   End
   Begin VB.CheckBox Check4 
      Caption         =   "Collision Detection"
      Height          =   255
      Left            =   5040
      TabIndex        =   33
      Top             =   2160
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton Command15 
      Caption         =   "Bldgs Off"
      Height          =   255
      Left            =   5640
      TabIndex        =   32
      Top             =   1200
      Width           =   855
   End
   Begin VB.CommandButton Command14 
      Caption         =   "Bldgs On"
      Height          =   255
      Left            =   4680
      TabIndex        =   31
      Top             =   1200
      Width           =   855
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   495
      Left            =   0
      TabIndex        =   21
      Top             =   3600
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   873
      _Version        =   393216
      Min             =   1
      Max             =   40
      SelStart        =   1
      TickFrequency   =   3
      Value           =   1
   End
   Begin VB.CommandButton Command11 
      Caption         =   "Shaftf Off"
      Height          =   255
      Left            =   5640
      TabIndex        =   20
      Top             =   960
      Width           =   855
   End
   Begin VB.CommandButton Command10 
      Caption         =   "Shaftf On"
      Height          =   255
      Left            =   4680
      TabIndex        =   19
      Top             =   960
      Width           =   855
   End
   Begin VB.CommandButton Command9 
      Caption         =   "Close Elevator"
      Height          =   495
      Left            =   3960
      TabIndex        =   18
      Top             =   3600
      Width           =   855
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Open Elevator"
      Height          =   495
      Left            =   3120
      TabIndex        =   17
      Top             =   3600
      Width           =   855
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Shafts Off"
      Enabled         =   0   'False
      Height          =   255
      Left            =   5640
      TabIndex        =   16
      Top             =   720
      Width           =   855
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Shafts On"
      Enabled         =   0   'False
      Height          =   255
      Left            =   4680
      TabIndex        =   15
      Top             =   720
      Width           =   855
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Recreate Objects"
      Height          =   495
      Left            =   4920
      TabIndex        =   14
      Top             =   3840
      Width           =   855
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Remove Objects"
      Height          =   495
      Left            =   5880
      TabIndex        =   13
      Top             =   3840
      Width           =   855
   End
   Begin VB.CheckBox Check9 
      Caption         =   "Sync"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5040
      TabIndex        =   12
      Top             =   4320
      Value           =   1  'Checked
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Goto M"
      Height          =   375
      Left            =   3360
      TabIndex        =   10
      Top             =   4680
      Width           =   975
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Sky"
      Height          =   375
      Left            =   5040
      TabIndex        =   9
      Top             =   3480
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Call Elevator"
      Height          =   375
      Left            =   1800
      TabIndex        =   8
      Top             =   3720
      Width           =   1215
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Stairs (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   3120
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   6
      Top             =   2760
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   2400
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check3 
      Caption         =   "Room (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   4
      Top             =   240
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check2 
      Caption         =   "External"
      Height          =   375
      Left            =   5040
      TabIndex        =   3
      Top             =   0
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Go"
      Height          =   375
      Left            =   1920
      TabIndex        =   2
      Top             =   4680
      Width           =   855
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   510
      Left            =   120
      TabIndex        =   1
      Top             =   4200
      Width           =   4335
      _ExtentX        =   7646
      _ExtentY        =   900
      _Version        =   393216
      Min             =   -10
      Max             =   138
      SelStart        =   1
      TickFrequency   =   5
      Value           =   1
   End
   Begin VB.TextBox Text1 
      Height          =   1815
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   120
      Width           =   4335
   End
   Begin VB.Label Label40 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4320
      TabIndex        =   66
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label39 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3840
      TabIndex        =   65
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label38 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3360
      TabIndex        =   64
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label37 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2880
      TabIndex        =   63
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label36 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2400
      TabIndex        =   62
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label35 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1920
      TabIndex        =   61
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label34 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1440
      TabIndex        =   60
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label33 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   59
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label32 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   58
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label31 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   0
      TabIndex        =   57
      Top             =   3120
      Width           =   495
   End
   Begin VB.Label Label30 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4320
      TabIndex        =   56
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label29 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3840
      TabIndex        =   55
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label28 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3360
      TabIndex        =   54
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label27 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2880
      TabIndex        =   53
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label26 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2400
      TabIndex        =   52
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label25 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1920
      TabIndex        =   51
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label24 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1440
      TabIndex        =   50
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label23 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   49
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label22 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   48
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label21 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   0
      TabIndex        =   47
      Top             =   2760
      Width           =   495
   End
   Begin VB.Label Label20 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4320
      TabIndex        =   46
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label19 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3840
      TabIndex        =   45
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label18 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3360
      TabIndex        =   44
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label17 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2880
      TabIndex        =   43
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label16 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2400
      TabIndex        =   42
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label15 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1920
      TabIndex        =   41
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label14 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1440
      TabIndex        =   40
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label13 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   39
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label12 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   38
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label11 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   0
      TabIndex        =   37
      Top             =   2400
      Width           =   495
   End
   Begin VB.Label Label10 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   4320
      TabIndex        =   30
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label9 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3840
      TabIndex        =   29
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label8 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3360
      TabIndex        =   28
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label7 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2880
      TabIndex        =   27
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   2400
      TabIndex        =   26
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1920
      TabIndex        =   25
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1440
      TabIndex        =   24
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   960
      TabIndex        =   23
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   480
      TabIndex        =   22
      Top             =   2040
      Width           =   495
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   0
      TabIndex        =   11
      Top             =   2040
      Width           =   495
   End
End
Attribute VB_Name = "DebugPanel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.95 Beta
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper
'http://sourceforge.net/projects/skyscraper

'This program is free software; you can redistribute it and/or
'modify it under the terms of the GNU General Public License
'as published by the Free Software Foundation; either version 2
'of the License, or (at your option) any later version.

'This program is distributed in the hope that it will be useful,
'but WITHOUT ANY WARRANTY; without even the implied warranty of
'MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'GNU General Public License for more details.

'You should have received a copy of the GNU General Public License
'along with this program; if not, write to the Free Software
'Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

Option Explicit


Private Sub Check1_Click()
If Check1.Value = 1 Then Atmos.SkyBox_Enable True
If Check1.Value = 0 Then Atmos.SkyBox_Enable False

End Sub



Private Sub Check2_Click()
If Check2.Value = 1 Then External.Enable True
If Check2.Value = 0 Then External.Enable False

End Sub

Private Sub Check3_Click()
If Check3.Value = 1 Then Room(CameraFloor).Enable True
If Check3.Value = 0 Then Room(CameraFloor).Enable False
End Sub


Private Sub Check4_Click()
If Check4.Value = 1 Then EnableCollisions = True
If Check4.Value = 0 Then EnableCollisions = False: FallRate = 0
End Sub

Private Sub Check5_Click()
For i = 1 To 40
If Check5.Value = 1 Then CallButtonsDown(i).Enable True: CallButtonsUp(i).Enable True
If Check5.Value = 0 Then CallButtonsDown(i).Enable False: CallButtonsUp(i).Enable False
Next i
End Sub

Private Sub Check6_Click()
For i = 1 To 40
If Check6.Value = 1 Then ElevatorInsDoorL(i).Enable True: ElevatorInsDoorR(i).Enable True
If Check6.Value = 0 Then ElevatorInsDoorL(i).Enable False: ElevatorInsDoorR(i).Enable False
Next i
End Sub

Private Sub Check7_Click()
On Error Resume Next
'i = CameraFloor
'If i = 1 Then i = -1
If Check7.Value = 1 Then
For i = 1 To 40
ElevatorDoorL(i).Enable True
ElevatorDoorR(i).Enable True
Next i
End If
If Check7.Value = 0 Then
For i = 1 To 40
ElevatorDoorL(i).Enable False
ElevatorDoorR(i).Enable False
Next i
End If
End Sub

Private Sub Check8_Click()
If Check8.Value = 1 Then
Stairs(CameraFloor).Enable True
If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
If CameraFloor > -10 Then Stairs(CameraFloor - 1).Enable True
End If

If Check8.Value = 0 Then
Stairs(CameraFloor).Enable False
If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
If CameraFloor > -10 Then Stairs(CameraFloor - 1).Enable False
End If
End Sub

Private Sub Command1_Click()
ElevatorNumber = Slider2.Value
'If Check9.Value = 1 Then ElevatorSync(ElevatorNumber) = True Else ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = Slider1.Value
If GotoFloor(ElevatorNumber) = 1 Then GotoFloor(ElevatorNumber) = -1
End Sub

Private Sub Command10_Click()
ShaftsFloor(CameraFloor).Enable True
End Sub

Private Sub Command11_Click()
ShaftsFloor(CameraFloor).Enable False
End Sub


Private Sub Command12_Click()
Landscape.Enable True
End Sub

Private Sub Command13_Click()
Landscape.Enable False
End Sub

Private Sub Command14_Click()
Buildings.Enable True
End Sub

Private Sub Command15_Click()
Buildings.Enable False
End Sub

Private Sub Command16_Click()
Call InitRealtime(CameraFloor)
End Sub

Private Sub Command17_Click()
Object.Visible = True
End Sub


Private Sub Command2_Click()
ElevatorNumber = Slider2.Value
ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = CameraFloor
If CameraFloor = 1 And Camera.GetPosition.Y > FloorHeight Then GotoFloor(ElevatorNumber) = 0.1
If GotoFloor(ElevatorNumber) = 1 Then GotoFloor(ElevatorNumber) = -1
End Sub

Private Sub Command3_Click()
ElevatorNumber = Slider2.Value
If Check9.Value = 1 Then ElevatorSync(ElevatorNumber) = True Else ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = 0.1
End Sub

Private Sub Command4_Click()
DestroyObjects (CameraFloor)

End Sub

Private Sub Command5_Click()
Test1 = True
Call InitRealtime(CameraFloor)
InitObjectsForFloor (CameraFloor)

End Sub

Private Sub Command6_Click()
For i = -10 To 138
Shafts1(i).Enable True
Shafts2(i).Enable True
Shafts3(i).Enable True
Shafts4(i).Enable True
Next i
End Sub

Private Sub Command7_Click()
For i = -10 To 138
Shafts1(i).Enable False
Shafts2(i).Enable False
Shafts3(i).Enable False
Shafts4(i).Enable False
Next i
End Sub

Private Sub Command8_Click()
ElevatorNumber = Slider2.Value
OpenElevator(ElevatorNumber) = 1
End Sub

Private Sub Command9_Click()
ElevatorNumber = Slider2.Value
OpenElevator(ElevatorNumber) = -1
End Sub

