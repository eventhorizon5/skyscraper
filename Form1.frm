VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SkyScraper"
   ClientHeight    =   7920
   ClientLeft      =   1080
   ClientTop       =   2775
   ClientWidth     =   10440
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7920
   ScaleWidth      =   10440
   Begin VB.Timer Timer10 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6600
      Top             =   2760
   End
   Begin VB.Timer Timer9 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   6000
      Top             =   2760
   End
   Begin VB.Timer Timer8 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   5400
      Top             =   2760
   End
   Begin VB.Timer Timer7 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4800
      Top             =   2760
   End
   Begin VB.Timer Timer6 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   4200
      Top             =   2760
   End
   Begin VB.Timer Timer5 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3600
      Top             =   2760
   End
   Begin VB.Timer Timer4 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   3000
      Top             =   2760
   End
   Begin VB.Timer Timer3 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   2400
      Top             =   2760
   End
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   5000
      Left            =   1800
      Top             =   2760
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
      Top             =   2760
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.91 Beta
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper
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
Call Elevator10Loop

End Sub

Private Sub Elevator1Timer_Timer()
'Sleep 30
Call Elevator1Loop
End Sub

Private Sub Elevator2Timer_Timer()
Call Elevator2Loop

End Sub

Private Sub Elevator3Timer_Timer()
Call Elevator3Loop

End Sub

Private Sub Elevator4Timer_Timer()
Call Elevator4Loop

End Sub

Private Sub Elevator5Timer_Timer()
Call Elevator5Loop

End Sub

Private Sub Elevator6Timer_Timer()
Call Elevator6Loop

End Sub

Private Sub Elevator7Timer_Timer()
Call Elevator7Loop

End Sub

Private Sub Elevator8Timer_Timer()
Call Elevator8Loop

End Sub

Private Sub Elevator9Timer_Timer()
Call Elevator9Loop

End Sub

Private Sub Form_GotFocus()
Focused = True
End Sub






Private Sub Form_Load()
ElevatorNumber = 1

FileName = App.Path + "\triton.dat"
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
  Next i
  
  For i = 1 To 10
    Plaque(i).Enable False
    FloorIndicator(i).Enable False
    'Elevator(i).Enable False
  Next i
  
  For i = -1 To 138
    ElevatorDoor1L(i).Enable False
    ElevatorDoor1R(i).Enable False
    ElevatorDoor2L(i).Enable False
    ElevatorDoor2R(i).Enable False
    ElevatorDoor3L(i).Enable False
    ElevatorDoor3R(i).Enable False
    ElevatorDoor4L(i).Enable False
    ElevatorDoor4R(i).Enable False
    ElevatorDoor5L(i).Enable False
    ElevatorDoor5R(i).Enable False
    ElevatorDoor6L(i).Enable False
    ElevatorDoor6R(i).Enable False
    ElevatorDoor7L(i).Enable False
    ElevatorDoor7R(i).Enable False
    ElevatorDoor8L(i).Enable False
    ElevatorDoor8R(i).Enable False
    ElevatorDoor9L(i).Enable False
    ElevatorDoor9R(i).Enable False
    ElevatorDoor10L(i).Enable False
    ElevatorDoor10R(i).Enable False
    Buttons1(i).Enable False
    Buttons2(i).Enable False
    Buttons3(i).Enable False
    Buttons4(i).Enable False
    Buttons5(i).Enable False
    Buttons6(i).Enable False
    Buttons7(i).Enable False
    Buttons8(i).Enable False
    Buttons9(i).Enable False
    Buttons10(i).Enable False
  Next i
  Room(1).Enable True
  'ElevatorDoorL(-1).Enable True
  'ElevatorDoorR(-1).Enable True
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
    For i = 1 To 10
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

Private Sub Timer2_Timer()
OpenElevator(2) = -1
Timer2.Enabled = False

End Sub

Private Sub Timer3_Timer()
OpenElevator(3) = -1
Timer3.Enabled = False

End Sub

Private Sub Timer4_Timer()
OpenElevator(4) = -1
Timer4.Enabled = False

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
