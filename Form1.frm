VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper"
   ClientHeight    =   7920
   ClientLeft      =   1200
   ClientTop       =   2760
   ClientWidth     =   10440
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7920
   ScaleWidth      =   10440
   Begin VB.Timer Timer31 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1200
      Top             =   5400
   End
   Begin VB.Timer Timer32 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1800
      Top             =   5400
   End
   Begin VB.Timer Timer33 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2400
      Top             =   5400
   End
   Begin VB.Timer Timer34 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3000
      Top             =   5400
   End
   Begin VB.Timer Timer35 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3600
      Top             =   5400
   End
   Begin VB.Timer Timer36 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4200
      Top             =   5400
   End
   Begin VB.Timer Timer37 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4800
      Top             =   5400
   End
   Begin VB.Timer Timer38 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   5400
      Top             =   5400
   End
   Begin VB.Timer Timer39 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6000
      Top             =   5400
   End
   Begin VB.Timer Timer40 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   5400
   End
   Begin VB.Timer Timer21 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1200
      Top             =   4920
   End
   Begin VB.Timer Timer22 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1800
      Top             =   4920
   End
   Begin VB.Timer Timer23 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2400
      Top             =   4920
   End
   Begin VB.Timer Timer24 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3000
      Top             =   4920
   End
   Begin VB.Timer Timer25 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3600
      Top             =   4920
   End
   Begin VB.Timer Timer26 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4200
      Top             =   4920
   End
   Begin VB.Timer Timer27 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4800
      Top             =   4920
   End
   Begin VB.Timer Timer28 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   5400
      Top             =   4920
   End
   Begin VB.Timer Timer29 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6000
      Top             =   4920
   End
   Begin VB.Timer Timer30 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   4920
   End
   Begin VB.Timer Timer11 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1200
      Top             =   4440
   End
   Begin VB.Timer Timer12 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1800
      Top             =   4440
   End
   Begin VB.Timer Timer13 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2400
      Top             =   4440
   End
   Begin VB.Timer Timer14 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3000
      Top             =   4440
   End
   Begin VB.Timer Timer15 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3600
      Top             =   4440
   End
   Begin VB.Timer Timer16 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4200
      Top             =   4440
   End
   Begin VB.Timer Timer17 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4800
      Top             =   4440
   End
   Begin VB.Timer Timer18 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   5400
      Top             =   4440
   End
   Begin VB.Timer Timer19 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6000
      Top             =   4440
   End
   Begin VB.Timer Timer20 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   4440
   End
   Begin VB.Timer Elevator40Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6600
      Top             =   3240
   End
   Begin VB.Timer Elevator39Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6000
      Top             =   3240
   End
   Begin VB.Timer Elevator38Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   5400
      Top             =   3240
   End
   Begin VB.Timer Elevator37Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4800
      Top             =   3240
   End
   Begin VB.Timer Elevator36Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4200
      Top             =   3240
   End
   Begin VB.Timer Elevator35Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3600
      Top             =   3240
   End
   Begin VB.Timer Elevator34Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3000
      Top             =   3240
   End
   Begin VB.Timer Elevator33Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2400
      Top             =   3240
   End
   Begin VB.Timer Elevator32Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1800
      Top             =   3240
   End
   Begin VB.Timer Elevator31Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1200
      Top             =   3240
   End
   Begin VB.Timer Elevator30Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6600
      Top             =   2760
   End
   Begin VB.Timer Elevator29Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6000
      Top             =   2760
   End
   Begin VB.Timer Elevator28Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   5400
      Top             =   2760
   End
   Begin VB.Timer Elevator27Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4800
      Top             =   2760
   End
   Begin VB.Timer Elevator26Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4200
      Top             =   2760
   End
   Begin VB.Timer Elevator25Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3600
      Top             =   2760
   End
   Begin VB.Timer Elevator24Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3000
      Top             =   2760
   End
   Begin VB.Timer Elevator23Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2400
      Top             =   2760
   End
   Begin VB.Timer Elevator22Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1800
      Top             =   2760
   End
   Begin VB.Timer Elevator21Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1200
      Top             =   2760
   End
   Begin VB.Timer Elevator20Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6600
      Top             =   2280
   End
   Begin VB.Timer Elevator19Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6000
      Top             =   2280
   End
   Begin VB.Timer Elevator18Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   5400
      Top             =   2280
   End
   Begin VB.Timer Elevator17Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4800
      Top             =   2280
   End
   Begin VB.Timer Elevator16Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4200
      Top             =   2280
   End
   Begin VB.Timer Elevator15Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3600
      Top             =   2280
   End
   Begin VB.Timer Elevator14Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3000
      Top             =   2280
   End
   Begin VB.Timer Elevator13Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2400
      Top             =   2280
   End
   Begin VB.Timer Elevator12Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1800
      Top             =   2280
   End
   Begin VB.Timer Elevator11Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1200
      Top             =   2280
   End
   Begin VB.Timer Timer10 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   3960
   End
   Begin VB.Timer Timer9 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6000
      Top             =   3960
   End
   Begin VB.Timer Timer8 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   5400
      Top             =   3960
   End
   Begin VB.Timer Timer7 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4800
      Top             =   3960
   End
   Begin VB.Timer Timer6 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4200
      Top             =   3960
   End
   Begin VB.Timer Timer5 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3600
      Top             =   3960
   End
   Begin VB.Timer Timer4 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3000
      Top             =   3960
   End
   Begin VB.Timer Timer3 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2400
      Top             =   3960
   End
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1800
      Top             =   3960
   End
   Begin VB.Timer Elevator10Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6600
      Top             =   1800
   End
   Begin VB.Timer Elevator9Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   6000
      Top             =   1800
   End
   Begin VB.Timer Elevator8Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   5400
      Top             =   1800
   End
   Begin VB.Timer Elevator7Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4800
      Top             =   1800
   End
   Begin VB.Timer Elevator6Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   4200
      Top             =   1800
   End
   Begin VB.Timer Elevator5Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3600
      Top             =   1800
   End
   Begin VB.Timer Elevator4Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   3000
      Top             =   1800
   End
   Begin VB.Timer Elevator3Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2400
      Top             =   1800
   End
   Begin VB.Timer Elevator2Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1800
      Top             =   1800
   End
   Begin VB.Timer StairsTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2160
      Top             =   360
   End
   Begin VB.Timer Elevator1Timer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1200
      Top             =   1800
   End
   Begin VB.Timer MainTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   960
      Top             =   360
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1200
      Top             =   3960
   End
   Begin VB.Label Label2 
      Height          =   1215
      Left            =   120
      TabIndex        =   1
      Top             =   2040
      Width           =   6495
   End
   Begin VB.Label Label1 
      Height          =   1815
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   6495
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.92 Beta
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper
'http://sourceforge.net/projects/skyscraper
'Contact - ryan@tliquest.net

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





Private Sub Elevator10Timer_Timer()
Call ElevatorLoop(10)

End Sub

Private Sub Elevator11Timer_Timer()
Call ElevatorLoop(11)

End Sub

Private Sub Elevator12Timer_Timer()
Call ElevatorLoop(12)

End Sub

Private Sub Elevator13Timer_Timer()
Call ElevatorLoop(13)

End Sub

Private Sub Elevator14Timer_Timer()
Call ElevatorLoop(14)

End Sub

Private Sub Elevator15Timer_Timer()
Call ElevatorLoop(15)

End Sub

Private Sub Elevator16Timer_Timer()
Call ElevatorLoop(16)

End Sub

Private Sub Elevator17Timer_Timer()
Call ElevatorLoop(17)

End Sub

Private Sub Elevator18Timer_Timer()
Call ElevatorLoop(18)

End Sub

Private Sub Elevator19Timer_Timer()
Call ElevatorLoop(19)

End Sub

Private Sub Elevator1Timer_Timer()
Call ElevatorLoop(1)
End Sub

Private Sub Elevator20Timer_Timer()
Call ElevatorLoop(20)

End Sub

Private Sub Elevator21Timer_Timer()
Call ElevatorLoop(21)

End Sub

Private Sub Elevator22Timer_Timer()
Call ElevatorLoop(22)

End Sub

Private Sub Elevator23Timer_Timer()
Call ElevatorLoop(23)

End Sub

Private Sub Elevator24Timer_Timer()
Call ElevatorLoop(24)

End Sub

Private Sub Elevator25Timer_Timer()
Call ElevatorLoop(25)

End Sub

Private Sub Elevator26Timer_Timer()
Call ElevatorLoop(26)

End Sub

Private Sub Elevator27Timer_Timer()
Call ElevatorLoop(27)

End Sub

Private Sub Elevator28Timer_Timer()
Call ElevatorLoop(28)

End Sub

Private Sub Elevator29Timer_Timer()
Call ElevatorLoop(29)

End Sub

Private Sub Elevator2Timer_Timer()
Call ElevatorLoop(2)

End Sub

Private Sub Elevator30Timer_Timer()
Call ElevatorLoop(30)

End Sub

Private Sub Elevator31Timer_Timer()
Call ElevatorLoop(31)

End Sub

Private Sub Elevator32Timer_Timer()
Call ElevatorLoop(32)

End Sub

Private Sub Elevator33Timer_Timer()
Call ElevatorLoop(33)

End Sub

Private Sub Elevator34Timer_Timer()
Call ElevatorLoop(34)

End Sub

Private Sub Elevator35Timer_Timer()
Call ElevatorLoop(35)

End Sub

Private Sub Elevator36Timer_Timer()
Call ElevatorLoop(36)

End Sub

Private Sub Elevator37Timer_Timer()
Call ElevatorLoop(37)

End Sub

Private Sub Elevator38Timer_Timer()
Call ElevatorLoop(38)

End Sub

Private Sub Elevator39Timer_Timer()
Call ElevatorLoop(39)

End Sub

Private Sub Elevator3Timer_Timer()
Call ElevatorLoop(3)

End Sub

Private Sub Elevator40Timer_Timer()
Call ElevatorLoop(40)

End Sub

Private Sub Elevator4Timer_Timer()
Call ElevatorLoop(4)

End Sub

Private Sub Elevator5Timer_Timer()
Call ElevatorLoop(5)

End Sub

Private Sub Elevator6Timer_Timer()
Call ElevatorLoop(6)

End Sub

Private Sub Elevator7Timer_Timer()
Call ElevatorLoop(7)

End Sub

Private Sub Elevator8Timer_Timer()
Call ElevatorLoop(8)

End Sub

Private Sub Elevator9Timer_Timer()
Call ElevatorLoop(9)

End Sub

Private Sub Form_GotFocus()
Focused = True
End Sub






Private Sub Form_Load()
ElevatorNumber = 1
FloorHeight = 32
'With the next 2 variables, lower=faster
ElevatorSpeed = 8
ElevatorFineTuneSpeed = 0.15

'FileName = App.Path + "\triton.dat"
Call Init_Simulator
Call ProcessFloors
    
    '4 : Old movement system startposition stuff
    'Camera.SetCameraPosition 0, 10, 0
    Camera.SetPosition 0, 10, 130
    'Camera.SetPosition 0, 1000, 2000
    'Camera.SetCameraLookAt 0, 10, 1
    Camera.SetLookAt 0, 10, -90
    Camera.RotateY 3.15
    
'*** also part of third movement system
'    sngPositionX = 0
'    sngPositionY = 10
'    sngPositionZ = 90
'    snglookatX = 0
'    snglookatY = 0
'    snglookatZ = 0
'    sngAngleX = 0
'    sngAngleY = -1.6
    
    ' We set the initial values of movement
    sngWalk = 0
    sngStrafe = 0
  
  'Room.Enable False
  Buildings.Enable True
  Landscape.Enable True
  External.Enable False
  For i = 1 To 138
  Room(i).Enable False
  Stairs(i).Enable False
  ShaftsFloor(i).Enable False
  Shafts1(i).Enable False
  Shafts2(i).Enable False
  Shafts3(i).Enable False
  Shafts4(i).Enable False
  Next i
  
  For i = -1 To 138
  StairDoor(i).Enable False
  Next i
  
  For i = 1 To 40
    Plaque(i).Enable False
    FloorIndicator(i).Enable False
    'Elevator(i).Enable False
  Next i
  
  'For j = 1 To 40
  'ElevatorDoorL(j).Enable False
  'ElevatorDoorR(j).Enable False
  'Next j
  'For i = -1 To 144
  '  Buttons(i).Enable False
  'Next i
  Room(1).Enable True
  'ElevatorDoorL(1).Enable True
  'ElevatorDoorR(1).Enable True
  Stairs(1).Enable True
  ShaftsFloor(1).Enable True
  
'Lights
'MatFactory.CreateMaterialQuick 0.2, 0.2, 0.2, 1, "Mat"
'MatFactory.SetDiffuse GetMat("Mat"), 1, 1, 1, 1
'External.SetMaterial GetMat("Mat")
'Mesh.ComputeNormals
'LightD.Type = D3DLIGHT_POINT
'LightD.Position = Vector(0, 10, -130)
'LightD.Range = 20
'LightD.Ambient = DXColor(0, 0, 300, 1)
'LightD.diffuse = DXColor(0, 0, 300, 1)
'LightD.Attenuation0 = 1
'LightD.Attenuation1 = 1
'LightD.Attenuation2 = 1
'Light.CreateLight LightD

  'External.Enable False
  'Shafts.Enable False
  'Elevator1.Enable False
  

  'ElevatorMusic.Loop_ = True
  'Call ElevatorMusic.Play
  
  Atmos.SkyBox_SetTexture GetTex("SkyFront"), GetTex("SkyBack"), GetTex("SkyLeft"), GetTex("SkyRight"), GetTex("SkyTop"), GetTex("SkyBottom")
  Atmos.SkyBox_Enable True
    
MainTimer.Enabled = True
Elevator1Timer.Enabled = True
Elevator2Timer.Enabled = True
Elevator3Timer.Enabled = True
Elevator4Timer.Enabled = True
Elevator5Timer.Enabled = True
Elevator6Timer.Enabled = True
Elevator7Timer.Enabled = True
Elevator8Timer.Enabled = True
Elevator9Timer.Enabled = True
Elevator10Timer.Enabled = True
Elevator11Timer.Enabled = True
Elevator12Timer.Enabled = True
Elevator13Timer.Enabled = True
Elevator14Timer.Enabled = True
Elevator15Timer.Enabled = True
Elevator16Timer.Enabled = True
Elevator17Timer.Enabled = True
Elevator18Timer.Enabled = True
Elevator19Timer.Enabled = True
Elevator20Timer.Enabled = True
Elevator21Timer.Enabled = True
Elevator22Timer.Enabled = True
Elevator23Timer.Enabled = True
Elevator24Timer.Enabled = True
Elevator25Timer.Enabled = True
Elevator26Timer.Enabled = True
Elevator27Timer.Enabled = True
Elevator28Timer.Enabled = True
Elevator29Timer.Enabled = True
Elevator30Timer.Enabled = True
Elevator31Timer.Enabled = True
Elevator32Timer.Enabled = True
Elevator33Timer.Enabled = True
Elevator34Timer.Enabled = True
Elevator35Timer.Enabled = True
Elevator36Timer.Enabled = True
Elevator37Timer.Enabled = True
Elevator38Timer.Enabled = True
Elevator39Timer.Enabled = True
Elevator40Timer.Enabled = True
StairsTimer.Enabled = True
  'Do Until isRunning = False Or Inp.IsKeyPressed(TV_KEY_ESCAPE) = True
'Loop
  
  'Set TV = Nothing
  'End
  
ErrorHandler:
  Dim Msg
If Err.Number <> 0 Then
   Msg = "Error # " & Str(Err.Number) & " was generated by " _
         & Err.Source & Chr(13) & Err.Description
   MsgBox Msg, , "Error", Err.HelpFile, Err.HelpContext
   End
End If


End Sub

Private Sub Form_LostFocus()
Focused = False
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
isRunning = False
End Sub

Private Sub Form_Unload(Cancel As Integer)

    'Set Listener = Nothing
    Set SoundEngine = Nothing
    Set TV = Nothing
    Set Scene = Nothing
    Set External = Nothing
    'Set Shafts = Nothing
    For i = 1 To 40
    Set Elevator(i) = Nothing
    'Set ElevatorMusic(i) = Nothing
    Set ElevatorSounds(i) = Nothing
    Next i
    
    End
End Sub

Private Sub MainTimer_Timer()
Call MainLoop
End Sub

Sub StairsTimer_Timer()
Call StairsLoop
End Sub

Private Sub Timer1_Timer()
OpenElevator(1) = -1
Timer1.Enabled = False
End Sub

Private Sub Timer10_Timer()
OpenElevator(10) = -1
Timer10.Enabled = False

End Sub

Private Sub Timer11_Timer()
OpenElevator(11) = -1
Timer11.Enabled = False

End Sub

Private Sub Timer12_Timer()
OpenElevator(12) = -1
Timer12.Enabled = False

End Sub

Private Sub Timer13_Timer()
OpenElevator(13) = -1
Timer13.Enabled = False

End Sub

Private Sub Timer14_Timer()
OpenElevator(14) = -1
Timer14.Enabled = False

End Sub

Private Sub Timer15_Timer()
OpenElevator(15) = -1
Timer15.Enabled = False

End Sub

Private Sub Timer16_Timer()
OpenElevator(16) = -1
Timer16.Enabled = False

End Sub

Private Sub Timer17_Timer()
OpenElevator(17) = -1
Timer17.Enabled = False

End Sub

Private Sub Timer18_Timer()
OpenElevator(18) = -1
Timer18.Enabled = False

End Sub

Private Sub Timer19_Timer()
OpenElevator(19) = -1
Timer19.Enabled = False

End Sub

Private Sub Timer2_Timer()
OpenElevator(2) = -1
Timer2.Enabled = False

End Sub

Private Sub Timer20_Timer()
OpenElevator(20) = -1
Timer20.Enabled = False

End Sub

Private Sub Timer21_Timer()
OpenElevator(21) = -1
Timer21.Enabled = False

End Sub

Private Sub Timer22_Timer()
OpenElevator(22) = -1
Timer22.Enabled = False

End Sub

Private Sub Timer23_Timer()
OpenElevator(23) = -1
Timer23.Enabled = False

End Sub

Private Sub Timer24_Timer()
OpenElevator(24) = -1
Timer24.Enabled = False

End Sub

Private Sub Timer25_Timer()
OpenElevator(25) = -1
Timer25.Enabled = False

End Sub

Private Sub Timer26_Timer()
OpenElevator(26) = -1
Timer26.Enabled = False

End Sub

Private Sub Timer27_Timer()
OpenElevator(27) = -1
Timer27.Enabled = False

End Sub

Private Sub Timer28_Timer()
OpenElevator(28) = -1
Timer28.Enabled = False

End Sub

Private Sub Timer29_Timer()
OpenElevator(29) = -1
Timer29.Enabled = False

End Sub

Private Sub Timer3_Timer()
OpenElevator(3) = -1
Timer3.Enabled = False

End Sub

Private Sub Timer30_Timer()
OpenElevator(30) = -1
Timer30.Enabled = False

End Sub

Private Sub Timer31_Timer()
OpenElevator(31) = -1
Timer31.Enabled = False

End Sub

Private Sub Timer32_Timer()
OpenElevator(32) = -1
Timer32.Enabled = False

End Sub

Private Sub Timer33_Timer()
OpenElevator(33) = -1
Timer33.Enabled = False

End Sub

Private Sub Timer34_Timer()
OpenElevator(34) = -1
Timer34.Enabled = False

End Sub

Private Sub Timer35_Timer()
OpenElevator(35) = -1
Timer35.Enabled = False

End Sub

Private Sub Timer36_Timer()
OpenElevator(36) = -1
Timer36.Enabled = False

End Sub

Private Sub Timer37_Timer()
OpenElevator(37) = -1
Timer37.Enabled = False

End Sub

Private Sub Timer38_Timer()
OpenElevator(38) = -1
Timer38.Enabled = False

End Sub

Private Sub Timer39_Timer()
OpenElevator(39) = -1
Timer39.Enabled = False

End Sub

Private Sub Timer4_Timer()
OpenElevator(4) = -1
Timer4.Enabled = False

End Sub

Private Sub Timer40_Timer()
OpenElevator(40) = -1
Timer40.Enabled = False

End Sub

Private Sub Timer5_Timer()
OpenElevator(5) = -1
Timer5.Enabled = False

End Sub

Private Sub Timer6_Timer()
OpenElevator(6) = -1
Timer6.Enabled = False

End Sub

Private Sub Timer7_Timer()
OpenElevator(7) = -1
Timer7.Enabled = False

End Sub

Private Sub Timer8_Timer()
OpenElevator(8) = -1
Timer8.Enabled = False

End Sub

Private Sub Timer9_Timer()
OpenElevator(9) = -1
Timer9.Enabled = False

End Sub
