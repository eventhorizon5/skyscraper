VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form Form2 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper 0.9 Beta - Simulator Control Panel"
   ClientHeight    =   4410
   ClientLeft      =   1500
   ClientTop       =   1770
   ClientWidth     =   6720
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4410
   ScaleWidth      =   6720
   Begin VB.CommandButton Command15 
      Caption         =   "Bldgs Off"
      Height          =   255
      Left            =   5640
      TabIndex        =   33
      Top             =   1080
      Width           =   855
   End
   Begin VB.CommandButton Command14 
      Caption         =   "Bldgs On"
      Height          =   255
      Left            =   4680
      TabIndex        =   32
      Top             =   1080
      Width           =   855
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   495
      Left            =   0
      TabIndex        =   22
      Top             =   2520
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   873
      _Version        =   393216
      Min             =   1
      SelStart        =   1
      Value           =   1
   End
   Begin VB.CommandButton Command11 
      Caption         =   "Shaftf Off"
      Height          =   255
      Left            =   5640
      TabIndex        =   21
      Top             =   840
      Width           =   855
   End
   Begin VB.CommandButton Command10 
      Caption         =   "Shaftf On"
      Height          =   255
      Left            =   4680
      TabIndex        =   20
      Top             =   840
      Width           =   855
   End
   Begin VB.CommandButton Command9 
      Caption         =   "Close Elevator"
      Height          =   495
      Left            =   3960
      TabIndex        =   19
      Top             =   2760
      Width           =   855
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Open Elevator"
      Height          =   495
      Left            =   3120
      TabIndex        =   18
      Top             =   2760
      Width           =   855
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Shafts Off"
      Enabled         =   0   'False
      Height          =   255
      Left            =   5640
      TabIndex        =   17
      Top             =   600
      Width           =   855
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Shafts On"
      Enabled         =   0   'False
      Height          =   255
      Left            =   4680
      TabIndex        =   16
      Top             =   600
      Width           =   855
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Recreate Objects"
      Height          =   495
      Left            =   4800
      TabIndex        =   15
      Top             =   3600
      Width           =   855
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Remove Objects"
      Height          =   495
      Left            =   5760
      TabIndex        =   14
      Top             =   3600
      Width           =   855
   End
   Begin VB.CheckBox Check9 
      Caption         =   "Sync"
      Height          =   375
      Left            =   5040
      TabIndex        =   13
      Top             =   4080
      Value           =   1  'Checked
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Goto M"
      Height          =   375
      Left            =   3360
      TabIndex        =   11
      Top             =   3960
      Width           =   975
   End
   Begin VB.CheckBox Check1 
      Caption         =   "Sky"
      Height          =   375
      Left            =   5040
      TabIndex        =   10
      Top             =   3120
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Call Elevator"
      Height          =   375
      Left            =   1800
      TabIndex        =   9
      Top             =   2880
      Width           =   1215
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Stairs (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   8
      Top             =   2760
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   2400
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   6
      Top             =   2040
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Elevators"
      Enabled         =   0   'False
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   1680
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
      Top             =   3960
      Width           =   855
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   510
      Left            =   120
      TabIndex        =   1
      Top             =   3360
      Width           =   4335
      _ExtentX        =   7646
      _ExtentY        =   900
      _Version        =   393216
      Min             =   1
      Max             =   138
      SelStart        =   1
      TickFrequency   =   5
      Value           =   1
   End
   Begin VB.TextBox Text1 
      Height          =   1575
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   120
      Width           =   4335
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
      TabIndex        =   31
      Top             =   1920
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
      TabIndex        =   30
      Top             =   1920
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
      TabIndex        =   29
      Top             =   1920
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
      TabIndex        =   28
      Top             =   1920
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
      TabIndex        =   27
      Top             =   1920
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
      TabIndex        =   26
      Top             =   1920
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
      TabIndex        =   25
      Top             =   1920
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
      TabIndex        =   24
      Top             =   1920
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
      TabIndex        =   23
      Top             =   1920
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
      TabIndex        =   12
      Top             =   1920
      Width           =   495
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.9 Beta
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper

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


Private Sub Check5_Click()
For i = 1 To 10
If Check5.Value = 1 Then Elevator(i).Enable True
If Check5.Value = 0 Then Elevator(i).Enable False
Next i
End Sub

Private Sub Check6_Click()
For i = 1 To 10
If Check6.Value = 1 Then ElevatorInsDoorL(i).Enable True: ElevatorInsDoorR(i).Enable True
If Check6.Value = 0 Then ElevatorInsDoorL(i).Enable False: ElevatorInsDoorR(i).Enable False
Next i
End Sub

Private Sub Check7_Click()
i = CameraFloor
If i = 1 Then i = -1
If Check7.Value = 1 Then
ElevatorDoor1L(i).Enable True: ElevatorDoor1R(i).Enable True
ElevatorDoor2L(i).Enable True: ElevatorDoor2R(i).Enable True
ElevatorDoor3L(i).Enable True: ElevatorDoor3R(i).Enable True
ElevatorDoor4L(i).Enable True: ElevatorDoor4R(i).Enable True
ElevatorDoor5L(i).Enable True: ElevatorDoor5R(i).Enable True
ElevatorDoor6L(i).Enable True: ElevatorDoor6R(i).Enable True
ElevatorDoor7L(i).Enable True: ElevatorDoor7R(i).Enable True
ElevatorDoor8L(i).Enable True: ElevatorDoor8R(i).Enable True
ElevatorDoor9L(i).Enable True: ElevatorDoor9R(i).Enable True
ElevatorDoor10L(i).Enable True: ElevatorDoor10R(i).Enable True
End If

If Check7.Value = 0 Then ElevatorDoor1L(i).Enable False: ElevatorDoor1R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor2L(i).Enable False: ElevatorDoor2R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor3L(i).Enable False: ElevatorDoor3R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor4L(i).Enable False: ElevatorDoor4R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor5L(i).Enable False: ElevatorDoor5R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor6L(i).Enable False: ElevatorDoor6R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor7L(i).Enable False: ElevatorDoor7R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor8L(i).Enable False: ElevatorDoor8R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor9L(i).Enable False: ElevatorDoor9R(i).Enable False
If Check7.Value = 0 Then ElevatorDoor10L(i).Enable False: ElevatorDoor10R(i).Enable False

End Sub

Private Sub Check8_Click()
If Check8.Value = 1 Then
Stairs(CameraFloor).Enable True
If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
End If

If Check8.Value = 0 Then
Stairs(CameraFloor).Enable False
If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
End If
End Sub

Private Sub Command1_Click()
ElevatorNumber = Slider2.Value
If Check9.Value = 1 Then ElevatorSync(ElevatorNumber) = True Else ElevatorSync(ElevatorNumber) = False
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


Private Sub Command14_Click()
Buildings.Enable True
End Sub

Private Sub Command15_Click()
Buildings.Enable False
End Sub

Private Sub Command2_Click()
ElevatorNumber = Slider2.Value
ElevatorSync(ElevatorNumber) = False
OpenElevator(ElevatorNumber) = -1
GotoFloor(ElevatorNumber) = CameraFloor
If CameraFloor = 1 And Camera.GetPosition.Y > 27 Then GotoFloor(ElevatorNumber) = 0.1
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
InitObjectsForFloor (CameraFloor)

End Sub

Private Sub Command6_Click()
For i = 1 To 138
Shafts1(i).Enable True
Shafts2(i).Enable True
Next i
End Sub

Private Sub Command7_Click()
For i = 1 To 138
Shafts1(i).Enable False
Shafts2(i).Enable False
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

