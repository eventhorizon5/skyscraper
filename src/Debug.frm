VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form DebugPanel 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper 1.1 Alpha - Simulator Control Panel"
   ClientHeight    =   5925
   ClientLeft      =   1770
   ClientTop       =   1710
   ClientWidth     =   6930
   ControlBox      =   0   'False
   Enabled         =   0   'False
   Icon            =   "Debug.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5925
   ScaleWidth      =   6930
   Begin VB.Frame Frame1 
      Caption         =   "Elevator"
      Height          =   2055
      Left            =   0
      TabIndex        =   64
      Top             =   3840
      Width           =   4815
      Begin VB.CommandButton Command3 
         Caption         =   "Close Manual"
         Height          =   255
         Left            =   3240
         TabIndex        =   73
         Top             =   480
         Width           =   1215
      End
      Begin VB.CommandButton Command12 
         Caption         =   "Open Manual"
         Height          =   255
         Left            =   2040
         TabIndex        =   74
         Top             =   480
         Width           =   1215
      End
      Begin VB.CommandButton Command11 
         Caption         =   "Enqueue Down"
         Height          =   255
         Left            =   3120
         TabIndex        =   65
         Top             =   1680
         Width           =   1335
      End
      Begin VB.CommandButton Command9 
         Caption         =   "Close"
         Height          =   255
         Left            =   3240
         TabIndex        =   68
         Top             =   240
         Width           =   1215
      End
      Begin VB.CommandButton Command1 
         Caption         =   "Go"
         Height          =   375
         Left            =   1800
         TabIndex        =   71
         Top             =   1560
         Width           =   855
      End
      Begin VB.CommandButton Command2 
         Caption         =   "Call Elevator"
         Height          =   375
         Left            =   360
         TabIndex        =   70
         Top             =   1560
         Width           =   1215
      End
      Begin VB.CommandButton Command8 
         Caption         =   "Open"
         Height          =   255
         Left            =   2040
         TabIndex        =   69
         Top             =   240
         Width           =   1215
      End
      Begin VB.CommandButton Command10 
         Caption         =   "Enqueue Up"
         Height          =   255
         Left            =   3120
         TabIndex        =   66
         Top             =   1440
         Width           =   1335
      End
      Begin MSComctlLib.Slider Slider2 
         Height          =   495
         Left            =   120
         TabIndex        =   67
         Top             =   480
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
      Begin MSComctlLib.Slider Slider1 
         Height          =   510
         Left            =   120
         TabIndex        =   72
         Top             =   960
         Width           =   4335
         _ExtentX        =   7646
         _ExtentY        =   900
         _Version        =   393216
         Min             =   -10
         Max             =   138
         TickFrequency   =   5
      End
      Begin VB.Label Label43 
         Alignment       =   2  'Center
         Caption         =   "Floor"
         Height          =   255
         Left            =   1800
         TabIndex        =   76
         Top             =   720
         Width           =   1455
      End
      Begin VB.Label Label42 
         Alignment       =   2  'Center
         Caption         =   "Number"
         Height          =   255
         Left            =   240
         TabIndex        =   75
         Top             =   240
         Width           =   1455
      End
   End
   Begin VB.CommandButton Command18 
      Caption         =   "Destroy Stairs"
      Height          =   255
      Left            =   5760
      TabIndex        =   59
      Top             =   840
      Width           =   1095
   End
   Begin VB.CheckBox Check14 
      Caption         =   "Buildings"
      Height          =   255
      Left            =   5040
      TabIndex        =   63
      Top             =   1155
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check13 
      Caption         =   "Landscape"
      Height          =   255
      Left            =   5040
      TabIndex        =   62
      Top             =   1395
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Shaft Front"
      Height          =   255
      Left            =   5040
      TabIndex        =   61
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
      TabIndex        =   60
      Top             =   840
      Width           =   1095
   End
   Begin VB.CheckBox Check12 
      Caption         =   "Automatic Shafts"
      Height          =   255
      Left            =   5040
      TabIndex        =   58
      Top             =   3555
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check11 
      Caption         =   "Main Sim Processing"
      Height          =   255
      Left            =   5040
      TabIndex        =   57
      Top             =   3315
      Value           =   1  'Checked
      Width           =   1815
   End
   Begin VB.CheckBox Check10 
      Caption         =   "Frame Limiter"
      Height          =   255
      Left            =   5040
      TabIndex        =   56
      Top             =   3075
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command17 
      Caption         =   "Add Object"
      Height          =   375
      Left            =   5160
      TabIndex        =   54
      Top             =   5400
      Width           =   1455
   End
   Begin VB.CommandButton Command16 
      Caption         =   "Init Realtime"
      Height          =   375
      Left            =   5400
      TabIndex        =   53
      Top             =   4800
      Width           =   1215
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Call Buttons"
      Height          =   255
      Left            =   5040
      TabIndex        =   22
      Top             =   1875
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox Check4 
      Caption         =   "Collision Detection"
      Height          =   255
      Left            =   5040
      TabIndex        =   21
      Top             =   2115
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CommandButton Command7 
      Caption         =   "All Shafts Off"
      Height          =   255
      Left            =   5760
      TabIndex        =   11
      Top             =   600
      Width           =   1095
   End
   Begin VB.CommandButton Command6 
      Caption         =   "All Shafts On"
      Height          =   255
      Left            =   4680
      TabIndex        =   10
      Top             =   600
      Width           =   1095
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Recreate Objects"
      Height          =   495
      Left            =   4920
      TabIndex        =   9
      Top             =   3840
      Width           =   855
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Remove Objects"
      Height          =   495
      Left            =   5880
      TabIndex        =   8
      Top             =   3840
      Width           =   855
   End
   Begin VB.CheckBox Check9 
      Caption         =   "Sync"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   4320
      Value           =   1  'Checked
      Width           =   975
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Sky"
      Height          =   255
      Left            =   5040
      TabIndex        =   5
      Top             =   2835
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   255
      Left            =   5040
      TabIndex        =   4
      Top             =   2595
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   255
      Left            =   5040
      TabIndex        =   3
      Top             =   2355
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check3 
      Caption         =   "Room (floor)"
      Height          =   255
      Left            =   5040
      TabIndex        =   2
      Top             =   315
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check2 
      Caption         =   "External"
      Height          =   375
      Left            =   5040
      TabIndex        =   1
      Top             =   0
      Value           =   1  'Checked
      Width           =   1575
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
      TabIndex        =   55
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
      TabIndex        =   52
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
      TabIndex        =   51
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
      TabIndex        =   50
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
      TabIndex        =   49
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
      TabIndex        =   48
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
      TabIndex        =   47
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
      TabIndex        =   46
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
      TabIndex        =   45
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
      TabIndex        =   44
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
      TabIndex        =   43
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
      TabIndex        =   42
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
      TabIndex        =   41
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
      TabIndex        =   40
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
      TabIndex        =   39
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
      TabIndex        =   38
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
      TabIndex        =   37
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
      TabIndex        =   36
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
      TabIndex        =   35
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
      TabIndex        =   34
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
      TabIndex        =   33
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
      TabIndex        =   32
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
      TabIndex        =   31
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
      TabIndex        =   30
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
      TabIndex        =   29
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
      TabIndex        =   28
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
      TabIndex        =   27
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
      TabIndex        =   26
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
      TabIndex        =   25
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
      TabIndex        =   24
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
      TabIndex        =   23
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
      TabIndex        =   20
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
      TabIndex        =   19
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
      TabIndex        =   18
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
      TabIndex        =   17
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
      TabIndex        =   16
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
      TabIndex        =   15
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
      TabIndex        =   14
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
      TabIndex        =   13
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
      TabIndex        =   12
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
      TabIndex        =   6
      Top             =   2280
      Width           =   495
   End
End
Attribute VB_Name = "DebugPanel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skyscraper 1.1 Alpha - Debug Panel
'Copyright ©2005 Ryan Thoryk
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

Private Sub Command1_Click()
Elevator(Slider2.Value).GotoFloor = Slider1.Value
Elevator(Slider2.Value).MoveElevator = True
End Sub

Private Sub Command12_Click()
Elevator(Slider2.Value).OpenDoor = 2
End Sub

Private Sub Command3_Click()
Elevator(Slider2.Value).OpenDoor = -2
End Sub

Private Sub Command8_Click()
Elevator(Slider2.Value).OpenDoor = 1
End Sub

Private Sub Command9_Click()
Elevator(Slider2.Value).OpenDoor = -1
End Sub
