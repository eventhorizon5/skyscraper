VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form Form2 
   Caption         =   "SkyScraper 0.6 Alpha - Simulator Control Panel"
   ClientHeight    =   4410
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6720
   Icon            =   "Form2.frx":0000
   LinkTopic       =   "Form2"
   ScaleHeight     =   4410
   ScaleWidth      =   6720
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command2 
      Caption         =   "Call Elevator"
      Height          =   375
      Left            =   1680
      TabIndex        =   10
      Top             =   2760
      Width           =   1215
   End
   Begin VB.CheckBox Check8 
      Caption         =   "Stairs (floor)"
      Height          =   375
      Left            =   5040
      TabIndex        =   9
      Top             =   2400
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check7 
      Caption         =   "Shaft Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   8
      Top             =   2040
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check6 
      Caption         =   "Elevator Doors"
      Height          =   375
      Left            =   5040
      TabIndex        =   7
      Top             =   1680
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check5 
      Caption         =   "Elevator1"
      Height          =   375
      Left            =   5040
      TabIndex        =   6
      Top             =   1320
      Value           =   1  'Checked
      Width           =   1575
   End
   Begin VB.CheckBox Check4 
      Caption         =   "Shafts"
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   960
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
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False


Private Sub Check2_Click()
If Check2.Value = 1 Then External.Enable True
If Check2.Value = 0 Then External.Enable False

End Sub

Private Sub Check3_Click()
If Check3.Value = 1 Then
For i = 1 To 138
Room(i).Enable True
Next i
End If

If Check3.Value = 0 Then
For i = 1 To 138
Room(i).Enable False
Next i
End If
End Sub

Private Sub Check4_Click()
If Check4.Value = 1 Then Shafts.Enable True
If Check4.Value = 0 Then Shafts.Enable False
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
For i = 1 To 138
ElevatorDoorL(i).Enable True: ElevatorDoorR(i).Enable True
Next i
End If

If Check7.Value = 0 Then
For i = 1 To 138
ElevatorDoorL(i).Enable False: ElevatorDoorR(i).Enable False
Next i
End If
End Sub

Private Sub Check8_Click()
If Check8.Value = 1 Then
For i = 1 To 138
Stairs(i).Enable True
Next i
End If

If Check8.Value = 0 Then
For i = 1 To 138
Stairs(i).Enable False
Next i
End If
End Sub

Private Sub Command1_Click()
ElevatorSync = True
OpenElevator = -1
GotoFloor = Slider1.Value
If GotoFloor = 1 Then GotoFloor = -1
End Sub

Private Sub Command2_Click()
ElevatorSync = False
OpenElevator = -1
GotoFloor = CameraFloor
If GotoFloor = 1 Then GotoFloor = -1
End Sub

