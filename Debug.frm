VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form DebugPanel 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper 0.97 Beta - Simulator Control Panel"
   ClientHeight    =   5355
   ClientLeft      =   1770
   ClientTop       =   1710
   ClientWidth     =   6930
   ControlBox      =   0   'False
   Enabled         =   0   'False
   Icon            =   "Debug.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5355
   ScaleWidth      =   6930
   Begin VB.CommandButton Command11 
      Caption         =   "Enqueue Down"
      Height          =   255
      Left            =   3720
      TabIndex        =   71
      Top             =   5040
      Width           =   1335
   End
   Begin VB.CommandButton Command10 
      Caption         =   "Enqueue Up"
      Height          =   255
      Left            =   3720
      TabIndex        =   70
      Top             =   4800
      Width           =   1335
   End
   Begin VB.CommandButton Command18 
      Caption         =   "Destroy Stairs"
      Height          =   255
      Left            =   5760
      TabIndex        =   65
      Top             =   840
      Width           =   1095
   End
   Begin VB.CheckBox Check14 
      Caption         =   "Buildings"
      Height          =   255
      Left            =   5040
      TabIndex        =   69
      Top             =   1155
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check13 
      Caption         =   "Landscape"
      Height          =   255
      Left            =   5040
      TabIndex        =   68
      Top             =   1395
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Shaft Front"
      Height          =   255
      Left            =   5040
      TabIndex        =   67
      Top             =   1635
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   0
      Top             =   0
   End
   Begin VB.CommandButton Command19 
      Caption         =   "Create Stairs"
      Height          =   255
      Left            =   4680
      TabIndex        =   66
      Top             =   840
      Width           =   1095
   End
   Begin VB.CheckBox Check12 
      Caption         =   "Automatic Shafts"
      Height          =   255
      Left            =   5040
      TabIndex        =   64
      Top             =   3555
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check11 
      Caption         =   "Main Sim Processing"
      Height          =   255
      Left            =   5040
      TabIndex        =   63
      Top             =   3315
      Value           =   1  'Checked
      Width           =   1815
   End
   Begin VB.CheckBox Check10 
      Caption         =   "Frame Limiter"
      Height          =   255
      Left            =   5040
      TabIndex        =   62
      Top             =   3075
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command17 
      Caption         =   "Add Object"
      Height          =   375
      Left            =   120
      TabIndex        =   60
      Top             =   4845
      Width           =   1455
   End
   Begin VB.CommandButton Command16 
      Caption         =   "Init Realtime"
      Height          =   375
      Left            =   5400
      TabIndex        =   59
      Top             =   4800
      Width           =   1215
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Call Buttons"
      Height          =   255
      Left            =   5040
      TabIndex        =   28
      Top             =   1875
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check4 
      Caption         =   "Collision Detection"
      Height          =   255
      Left            =   5040
      TabIndex        =   27
      Top             =   2115
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   495
      Left            =   0
      TabIndex        =   17
      Top             =   3840
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
   Begin VB.CommandButton Command9 
      Caption         =   "Close Elevator"
      Height          =   495
      Left            =   3960
      TabIndex        =   16
      Top             =   3840
      Width           =   855
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Open Elevator"
      Height          =   495
      Left            =   3120
      TabIndex        =   15
      Top             =   3840
      Width           =   855
   End
   Begin VB.CommandButton Command7 
      Caption         =   "All Shafts Off"
      Height          =   255
      Left            =   5760
      TabIndex        =   14
      Top             =   600
      Width           =   1095
   End
   Begin VB.CommandButton Command6 
      Caption         =   "All Shafts On"
      Height          =   255
      Left            =   4680
      TabIndex        =   13
      Top             =   600
      Width           =   1095
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Recreate Objects"
      Height          =   495
      Left            =   4920
      TabIndex        =   12
      Top             =   3840
      Width           =   855
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Remove Objects"
      Height          =   495
      Left            =   5880
      TabIndex        =   11
      Top             =   3840
      Width           =   855
   End
   Begin VB.CheckBox Check9 
      Caption         =   "Sync"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5040
      TabIndex        =   10
      Top             =   4320
      Value           =   1  'Checked
      Width           =   975
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Sky"
      Height          =   255
      Left            =   5040
      TabIndex        =   8
      Top             =   2835
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Call Elevator"
      Height          =   375
      Left            =   1800
      TabIndex        =   7
      Top             =   3840
      Width           =   1215
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   255
      Left            =   5040
      TabIndex        =   6
      Top             =   2595
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   255
      Left            =   5040
      TabIndex        =   5
      Top             =   2355
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check3 
      Caption         =   "Room (floor)"
      Height          =   255
      Left            =   5040
      TabIndex        =   4
      Top             =   315
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
      Left            =   1680
      TabIndex        =   2
      Top             =   4845
      Width           =   855
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   510
      Left            =   120
      TabIndex        =   1
      Top             =   4320
      Width           =   4335
      _ExtentX        =   7646
      _ExtentY        =   900
      _Version        =   393216
      Min             =   -10
      Max             =   138
      TickFrequency   =   5
   End
   Begin VB.TextBox Text1 
      Height          =   1815
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      TabIndex        =   0
      Top             =   120
      Width           =   4335
   End
   Begin VB.Label Label41 
      Alignment       =   2  'Center
      Caption         =   "Elevator floor indicators - click for queue information"
      Height          =   255
      Left            =   105
      TabIndex        =   61
      Top             =   1995
      Width           =   4695
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
      TabIndex        =   58
      Top             =   3360
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
      TabIndex        =   57
      Top             =   3360
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
      TabIndex        =   56
      Top             =   3360
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
      TabIndex        =   55
      Top             =   3360
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
      TabIndex        =   54
      Top             =   3360
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
      TabIndex        =   53
      Top             =   3360
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
      TabIndex        =   52
      Top             =   3360
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
      TabIndex        =   51
      Top             =   3360
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
      TabIndex        =   50
      Top             =   3360
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
      TabIndex        =   49
      Top             =   3360
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
      TabIndex        =   48
      Top             =   3000
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
      TabIndex        =   47
      Top             =   3000
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
      TabIndex        =   46
      Top             =   3000
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
      TabIndex        =   45
      Top             =   3000
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
      TabIndex        =   44
      Top             =   3000
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
      TabIndex        =   43
      Top             =   3000
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
      TabIndex        =   42
      Top             =   3000
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
      TabIndex        =   41
      Top             =   3000
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
      TabIndex        =   40
      Top             =   3000
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
      TabIndex        =   39
      Top             =   3000
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
      TabIndex        =   38
      Top             =   2640
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
      TabIndex        =   37
      Top             =   2640
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
      TabIndex        =   36
      Top             =   2640
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
      TabIndex        =   35
      Top             =   2640
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
      TabIndex        =   34
      Top             =   2640
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
      TabIndex        =   33
      Top             =   2640
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
      TabIndex        =   32
      Top             =   2640
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
      TabIndex        =   31
      Top             =   2640
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
      TabIndex        =   30
      Top             =   2640
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
      TabIndex        =   29
      Top             =   2640
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
      TabIndex        =   26
      Top             =   2280
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
      TabIndex        =   25
      Top             =   2280
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
      TabIndex        =   24
      Top             =   2280
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
      TabIndex        =   23
      Top             =   2280
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
      TabIndex        =   22
      Top             =   2280
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
      TabIndex        =   21
      Top             =   2280
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
      TabIndex        =   20
      Top             =   2280
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
      TabIndex        =   19
      Top             =   2280
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
      TabIndex        =   18
      Top             =   2280
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
      TabIndex        =   9
      Top             =   2280
      Width           =   495
   End
End
Attribute VB_Name = "DebugPanel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.97 Beta - Debug Panel
'Copyright (C) 2004 Ryan Thoryk
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

Public Sub ShowQueues(Number As Integer)
MsgBox ("Elevator " + LTrim(Str$(Number)) + "'s queues:" + vbCrLf + "Up: " + UpQueue(Number) + vbCrLf + "Down: " + DownQueue(Number))
End Sub

Private Sub Check1_Click()
If Check1.Value = 1 Then Atmos.SkyBox_Enable True
If Check1.Value = 0 Then Atmos.SkyBox_Enable False

End Sub



Private Sub Check13_Click()
If Check13.Value = 1 Then Landscape.Enable True
If Check13.Value = 0 Then Landscape.Enable False
End Sub

Private Sub Check14_Click()
If Check14.Value = 1 Then Buildings.Enable True
If Check14.Value = 0 Then Buildings.Enable False
End Sub

Private Sub Check2_Click()
If Check2.Value = 1 Then External.Enable True
If Check2.Value = 0 Then External.Enable False

End Sub

Private Sub Check3_Click()
If Check3.Value = 1 Then Room(CameraFloor).Enable True: CrawlSpace(CameraFloor).Enable True
If Check3.Value = 0 Then Room(CameraFloor).Enable False: CrawlSpace(CameraFloor).Enable False
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
If Check8.Value = 1 Then ShaftsFloor(CameraFloor).Enable True
If Check8.Value = 0 Then ShaftsFloor(CameraFloor).Enable False
End Sub

Private Sub Command1_Click()
ElevatorNumber = Slider2.Value
'If Check9.Value = 1 Then ElevatorSync(ElevatorNumber) = True Else ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = Slider1.Value
If GotoFloor(ElevatorNumber) = 0 Then GotoFloor(ElevatorNumber) = 0.001
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

Private Sub Command10_Click()
Call AddRoute(Slider1.Value, Slider2.Value, 1)
End Sub

Private Sub Command11_Click()
Call AddRoute(Slider1.Value, Slider2.Value, -1)
End Sub

Private Sub Command16_Click()
Call InitRealtime(CameraFloor)
End Sub

Private Sub Command17_Click()
Object.Visible = True
End Sub


Private Sub Command18_Click()
If StairDataTable(CameraFloor) = True Then DeleteStairs (CameraFloor)
If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = True Then DeleteStairs (CameraFloor + 1)
If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = True Then DeleteStairs (CameraFloor - 1)
End Sub

Private Sub Command19_Click()
If StairDataTable(CameraFloor) = False Then CreateStairs (CameraFloor)
If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = False Then CreateStairs (CameraFloor + 1)
If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = False Then CreateStairs (CameraFloor - 1)
End Sub

Private Sub Command2_Click()
ElevatorNumber = Slider2.Value
ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = CameraFloor
If CameraFloor = 1 Then GotoFloor(ElevatorNumber) = 0.01
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
For i = BottomFloor To TopFloor
Shafts1(i).Enable True
Shafts2(i).Enable True
Shafts3(i).Enable True
Shafts4(i).Enable True
Next i
PipeShaft(1).Enable True
PipeShaft(2).Enable True
PipeShaft(3).Enable True
PipeShaft(4).Enable True
End Sub

Private Sub Command7_Click()
For i = BottomFloor To TopFloor
Shafts1(i).Enable False
Shafts2(i).Enable False
Shafts3(i).Enable False
Shafts4(i).Enable False
Next i
PipeShaft(1).Enable False
PipeShaft(2).Enable False
PipeShaft(3).Enable False
PipeShaft(4).Enable False
End Sub

Private Sub Command8_Click()
ElevatorNumber = Slider2.Value
OpenElevator(ElevatorNumber) = 1
End Sub

Private Sub Command9_Click()
ElevatorNumber = Slider2.Value
OpenElevator(ElevatorNumber) = -1
End Sub

Private Sub Form_Load()
Me.Left = 0
Me.Top = 0
End Sub

Private Sub Label1_Click()
ShowQueues (1)
End Sub

Private Sub Label2_Click()
ShowQueues (2)
End Sub

Private Sub Label3_Click()
ShowQueues (3)
End Sub

Private Sub Label4_Click()
ShowQueues (4)
End Sub

Private Sub Label5_Click()
ShowQueues (5)
End Sub

Private Sub Label6_Click()
ShowQueues (6)
End Sub

Private Sub Label7_Click()
ShowQueues (7)
End Sub

Private Sub Label8_Click()
ShowQueues (8)
End Sub

Private Sub Label9_Click()
ShowQueues (9)
End Sub

Private Sub Label10_Click()
ShowQueues (10)
End Sub

Private Sub Label11_Click()
ShowQueues (11)
End Sub

Private Sub Label12_Click()
ShowQueues (12)
End Sub

Private Sub Label13_Click()
ShowQueues (13)
End Sub

Private Sub Label14_Click()
ShowQueues (14)
End Sub

Private Sub Label15_Click()
ShowQueues (15)
End Sub

Private Sub Label16_Click()
ShowQueues (16)
End Sub

Private Sub Label17_Click()
ShowQueues (17)
End Sub

Private Sub Label18_Click()
ShowQueues (18)
End Sub

Private Sub Label19_Click()
ShowQueues (19)
End Sub

Private Sub Label20_Click()
ShowQueues (20)
End Sub

Private Sub Label21_Click()
ShowQueues (21)
End Sub

Private Sub Label22_Click()
ShowQueues (22)
End Sub

Private Sub Label23_Click()
ShowQueues (23)
End Sub

Private Sub Label24_Click()
ShowQueues (24)
End Sub

Private Sub Label25_Click()
ShowQueues (25)
End Sub

Private Sub Label26_Click()
ShowQueues (26)
End Sub

Private Sub Label27_Click()
ShowQueues (27)
End Sub

Private Sub Label28_Click()
ShowQueues (28)
End Sub

Private Sub Label29_Click()
ShowQueues (29)
End Sub

Private Sub Label30_Click()
ShowQueues (30)
End Sub

Private Sub Label31_Click()
ShowQueues (31)
End Sub

Private Sub Label32_Click()
ShowQueues (32)
End Sub

Private Sub Label33_Click()
ShowQueues (33)
End Sub

Private Sub Label34_Click()
ShowQueues (34)
End Sub

Private Sub Label35_Click()
ShowQueues (35)
End Sub

Private Sub Label36_Click()
ShowQueues (36)
End Sub

Private Sub Label37_Click()
ShowQueues (37)
End Sub

Private Sub Label38_Click()
ShowQueues (38)
End Sub

Private Sub Label39_Click()
ShowQueues (39)
End Sub

Private Sub Label40_Click()
ShowQueues (40)
End Sub

Private Sub Timer1_Timer()
If External.IsMeshEnabled = True Then Check2.Value = 1
If External.IsMeshEnabled = False Then Check2.Value = 0
If Room(CameraFloor).IsMeshEnabled = True Then Check3.Value = 1
If Room(CameraFloor).IsMeshEnabled = False Then Check3.Value = 0
If CallButtonsDown(1).IsMeshEnabled = True Then Check5.Value = 1
If CallButtonsDown(1).IsMeshEnabled = False Then Check5.Value = 0
If ElevatorInsDoorL(1).IsMeshEnabled = True Then Check6.Value = 1
If ElevatorInsDoorL(1).IsMeshEnabled = False Then Check6.Value = 0
If ElevatorDoorL(1).IsMeshEnabled = True Then Check7.Value = 1
If ElevatorDoorL(1).IsMeshEnabled = False Then Check7.Value = 0
If ShaftsFloor(CameraFloor).IsMeshEnabled = True Then Check8.Value = 1
If ShaftsFloor(CameraFloor).IsMeshEnabled = False Then Check8.Value = 0
If Landscape.IsMeshEnabled = True Then Check13.Value = 1
If Landscape.IsMeshEnabled = False Then Check13.Value = 0
If Buildings.IsMeshEnabled = True Then Check13.Value = 1
If Buildings.IsMeshEnabled = False Then Check13.Value = 0
End Sub
