VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form Form2 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper 0.8 Alpha - Simulator Control Panel"
   ClientHeight    =   4410
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6720
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4410
   ScaleWidth      =   6720
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command9 
      Caption         =   "Close Elevator"
      Enabled         =   0   'False
      Height          =   495
      Left            =   3240
      TabIndex        =   19
      Top             =   1920
      Width           =   855
   End
   Begin VB.CommandButton Command8 
      Caption         =   "Open Elevator"
      Enabled         =   0   'False
      Height          =   495
      Left            =   2280
      TabIndex        =   18
      Top             =   1920
      Width           =   855
   End
   Begin VB.CommandButton Command7 
      Caption         =   "Shafts Off"
      Height          =   375
      Left            =   5640
      TabIndex        =   17
      Top             =   960
      Width           =   855
   End
   Begin VB.CommandButton Command6 
      Caption         =   "Shafts On"
      Height          =   375
      Left            =   4680
      TabIndex        =   16
      Top             =   960
      Width           =   855
   End
   Begin VB.CommandButton Command5 
      Caption         =   "Recreate Objects"
      Height          =   495
      Left            =   4800
      TabIndex        =   15
      Top             =   3240
      Width           =   855
   End
   Begin VB.CommandButton Command4 
      Caption         =   "Remove Objects"
      Height          =   495
      Left            =   5760
      TabIndex        =   14
      Top             =   3240
      Width           =   855
   End
   Begin VB.CheckBox Check9 
      Caption         =   "Sync"
      Height          =   375
      Left            =   5040
      TabIndex        =   13
      Top             =   3840
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
      Top             =   2760
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Call Elevator"
      Enabled         =   0   'False
      Height          =   375
      Left            =   1680
      TabIndex        =   9
      Top             =   2760
      Width           =   1215
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Stairs (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   8
      Top             =   2400
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   2040
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   6
      Top             =   1680
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Elevator1"
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   1320
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check3 
      Caption         =   "Room (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   4
      Top             =   600
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check2 
      Caption         =   "External"
      Height          =   375
      Left            =   5040
      TabIndex        =   3
      Top             =   240
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
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   240
      TabIndex        =   12
      Top             =   1920
      Width           =   975
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.7 Alpha
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
If Check5.Value = 1 Then Elevator1.Enable True
If Check5.Value = 0 Then Elevator1.Enable False
End Sub

Private Sub Check6_Click()
If Check6.Value = 1 Then Elevator1DoorL.Enable True: Elevator1DoorR.Enable True
If Check6.Value = 0 Then Elevator1DoorL.Enable False: Elevator1DoorR.Enable False
End Sub

Private Sub Check7_Click()
If Check7.Value = 1 Then
ElevatorDoorL(CameraFloor).Enable True: ElevatorDoorR(CameraFloor).Enable True
End If

If Check7.Value = 0 Then ElevatorDoorL(CameraFloor).Enable False: ElevatorDoorR(CameraFloor).Enable False
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
If Check9.Value = 1 Then ElevatorSync = True Else ElevatorSync = False
OpenElevator = -1
GotoFloor = Slider1.Value
If GotoFloor = 1 Then GotoFloor = -1
End Sub

Private Sub Command2_Click()
ElevatorSync = False
OpenElevator = -1
GotoFloor = CameraFloor
If FloorIndicator = "M" Then GotoFloor = 0.1
If GotoFloor = 1 Then GotoFloor = -1
End Sub

Private Sub Command3_Click()
If Check9.Value = 1 Then ElevatorSync = True Else ElevatorSync = False
OpenElevator = -1
GotoFloor = 0.1
End Sub

Private Sub Command4_Click()
DestroyObjects (CameraFloor)

End Sub

Private Sub Command5_Click()
InitObjectsForFloor (CameraFloor)

End Sub

Private Sub Command6_Click()
Shafts.Enable True

End Sub

Private Sub Command7_Click()
Shafts.Enable False

End Sub

Private Sub Command8_Click()
OpenElevator = 1
End Sub

Private Sub Command9_Click()
OpenElevator = -1
End Sub
