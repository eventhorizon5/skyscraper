VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form Designer 
   Caption         =   "Building Designer"
   ClientHeight    =   6105
   ClientLeft      =   1650
   ClientTop       =   1830
   ClientWidth     =   9180
   Icon            =   "Designer.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6105
   ScaleWidth      =   9180
   Begin VB.CommandButton Command11 
      Caption         =   "Window"
      Height          =   375
      Left            =   120
      TabIndex        =   27
      Top             =   3840
      Width           =   735
   End
   Begin VB.CommandButton Command10 
      Caption         =   "Ladder"
      Height          =   375
      Left            =   120
      TabIndex        =   26
      Top             =   3480
      Width           =   735
   End
   Begin VB.CommandButton Command9 
      Caption         =   "Door"
      Height          =   375
      Left            =   120
      TabIndex        =   25
      Top             =   3120
      Width           =   735
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Sign"
      Height          =   375
      Left            =   120
      TabIndex        =   24
      Top             =   2760
      Width           =   735
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Call"
      Height          =   375
      Left            =   120
      TabIndex        =   23
      Top             =   2400
      Width           =   735
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Stair"
      Height          =   375
      Left            =   120
      TabIndex        =   22
      Top             =   2040
      Width           =   735
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Object"
      Height          =   375
      Left            =   120
      TabIndex        =   11
      Top             =   1680
      Width           =   735
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Elev"
      Height          =   375
      Left            =   120
      TabIndex        =   8
      Top             =   1320
      Width           =   735
   End
   Begin VB.Frame Frame3 
      Caption         =   "Area"
      Height          =   1335
      Left            =   1080
      TabIndex        =   18
      Top             =   2640
      Width           =   1455
      Begin VB.CheckBox Check8 
         Caption         =   "External"
         Height          =   255
         Left            =   120
         TabIndex        =   21
         Top             =   840
         Width           =   1215
      End
      Begin VB.CheckBox Check7 
         Caption         =   "Crawlspace"
         Height          =   255
         Left            =   120
         TabIndex        =   20
         Top             =   600
         Width           =   1215
      End
      Begin VB.CheckBox Check6 
         Caption         =   "Main Level"
         Height          =   255
         Left            =   120
         TabIndex        =   19
         Top             =   360
         Value           =   1  'Checked
         Width           =   1095
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Visibility"
      Height          =   2415
      Left            =   1080
      TabIndex        =   12
      Top             =   120
      Width           =   1335
      Begin VB.CheckBox Check5 
         Caption         =   "Objects"
         Height          =   255
         Left            =   120
         TabIndex        =   17
         Top             =   1200
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.CheckBox Check4 
         Caption         =   "Elevators"
         Height          =   255
         Left            =   120
         TabIndex        =   16
         Top             =   960
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.CheckBox Check3 
         Caption         =   "Ceilings"
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   720
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.CheckBox Check2 
         Caption         =   "Floors"
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   480
         Value           =   1  'Checked
         Width           =   1095
      End
      Begin VB.CheckBox Check1 
         Caption         =   "Walls"
         Height          =   255
         Left            =   120
         TabIndex        =   13
         Top             =   240
         Value           =   1  'Checked
         Width           =   1095
      End
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Ceiling"
      Height          =   375
      Left            =   120
      TabIndex        =   10
      Top             =   960
      Width           =   735
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Floor"
      Height          =   375
      Left            =   120
      TabIndex        =   9
      Top             =   600
      Width           =   735
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Wall"
      Height          =   375
      Left            =   120
      TabIndex        =   5
      Top             =   240
      Width           =   735
   End
   Begin VB.Frame Frame1 
      Caption         =   "Draw Lock"
      Height          =   735
      Left            =   2640
      TabIndex        =   2
      Top             =   0
      Width           =   1215
      Begin MSForms.ToggleButton ToggleButton2 
         Height          =   375
         Left            =   600
         TabIndex        =   4
         Top             =   240
         Width           =   375
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   6
         Size            =   "661;661"
         Value           =   "0"
         Caption         =   "Z"
         FontHeight      =   165
         FontCharSet     =   0
         FontPitchAndFamily=   2
         ParagraphAlign  =   3
      End
      Begin MSForms.ToggleButton ToggleButton1 
         Height          =   375
         Left            =   240
         TabIndex        =   3
         Top             =   240
         Width           =   375
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   6
         Size            =   "661;661"
         Value           =   "0"
         Caption         =   "X"
         FontHeight      =   165
         FontCharSet     =   0
         FontPitchAndFamily=   2
         ParagraphAlign  =   3
      End
   End
   Begin VB.PictureBox DataGrid 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   4695
      Left            =   2640
      ScaleHeight     =   4665
      ScaleWidth      =   6345
      TabIndex        =   1
      Top             =   960
      Width           =   6375
      Begin VB.HScrollBar HScroll1 
         Height          =   255
         Left            =   0
         TabIndex        =   7
         Top             =   4440
         Width           =   6135
      End
      Begin VB.VScrollBar VScroll1 
         Height          =   4575
         Left            =   6120
         TabIndex        =   6
         Top             =   0
         Width           =   255
      End
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   5850
      Width           =   9180
      _ExtentX        =   16193
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileNew 
         Caption         =   "New"
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "Open"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "Save"
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save As..."
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "Edit"
      Begin VB.Menu mnuEditCut 
         Caption         =   "Cut"
      End
      Begin VB.Menu mnuEditCopy 
         Caption         =   "Copy"
      End
      Begin VB.Menu mnuEditPaste 
         Caption         =   "Paste"
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "Options"
   End
   Begin VB.Menu mnuTools 
      Caption         =   "Tools"
      Begin VB.Menu mnuToolsCode 
         Caption         =   "View Code"
      End
      Begin VB.Menu mnuToolsEdit 
         Caption         =   "Level Editor"
      End
      Begin VB.Menu mnuToolsFull 
         Caption         =   "View 3D Fullscreen"
      End
      Begin VB.Menu mnuToolsWindowed 
         Caption         =   "View 3D Windowed"
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "Help"
      Begin VB.Menu mnuHelpDesigner 
         Caption         =   "Designer Manual"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "Designer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private DoubleClick As Boolean
Private SingleClick As Boolean
Private MouseX As Single
Private MouseY As Single

Public DesignLowerBound As Integer
Public DesignUpperBound As Integer

Private Sub DataGrid_Click()
SingleClick = True
End Sub

Private Sub DataGrid_DblClick()
DoubleClick = True
End Sub

Private Sub DataGrid_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
MouseX = X
MouseY = Y
End Sub

Private Sub mnuViewFull_Click()
Preview.Show
End Sub

Private Sub mnuViewWindowed_Click()
Preview.Show
End Sub

Private Sub mnuToolsEdit_Click()
LevelEdit.Show
End Sub
