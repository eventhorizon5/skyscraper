VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "SkyScraper"
   ClientHeight    =   7920
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10440
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   7920
   ScaleWidth      =   10440
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer StairsTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2160
      Top             =   360
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   1560
      Top             =   360
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
      Left            =   240
      Top             =   360
   End
End
Attribute VB_Name = "Form1"
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



Private Sub ElevatorTimer_Timer()
ElevatorFloor = (Elevator1.GetPosition.Y - 25) / 25
      If ElevatorFloor < 1 Then ElevatorFloor = 1
      
      If InStairwell = False Then CameraFloor = (Camera.GetPosition.Y - 25 - 10) / 25
      If CameraFloor < 1 Then CameraFloor = 1
      
      If GotoFloor = ElevatorFloor - 1 Then CurrentFloor = ElevatorFloor

      If GotoFloor <> 0 And GotoFloor > CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then
      ElevatorDirection = 1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((GotoFloor * 25) + 25) - ((CurrentFloorExact * 25) + 25)
      If ElevatorSync = True Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      If GotoFloor <> 0 And GotoFloor < CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then
      ElevatorDirection = -1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((CurrentFloorExact * 25) + 25) - ((GotoFloor * 25) + 25)
      If ElevatorSync = True Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor = Int((elevator1start.Y - 25) / 25)
       CurrentFloorExact = (elevator1start.Y - 25) / 25
       'CurrentFloor = (Elevator1.GetPosition.y / 25) - 1
       'CurrentFloorExact = Int((Elevator1.GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable / 5)
        
        If ElevatorEnable >= 0 And ElevatorDirection = 1 Then
        'sound
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck = 0 And GotoFloor <> ElevatorFloor Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstart.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 100
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y + 20, -23)
        Elevator1Sounds.Play
        ElevatorCheck = 1
        End If
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck = 1 Then
        Elevator1Sounds.Load App.Path + "\elevmove.wav"
        Elevator1Sounds.Loop_ = True
        Elevator1Sounds.Play
        End If
        'movement
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable <= 15 Then StoppingDistance = CurrentFloorExact - OriginalLocation + 0.4
        If ElevatorEnable > 15 Then ElevatorEnable = 15
        Destination = ((OriginalLocation * 25) + 25) + DistanceToTravel - 35
        If GotoFloor <> 0 And elevator1start.Y >= (Destination - (StoppingDistance * 25) + 25) Then ElevatorDirection = -1: ElevatorCheck = 0
        End If
      
        If ElevatorEnable > 0 And ElevatorDirection = -1 Then
        'Sounds
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Stop_
        End If
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstop.wav"
        Elevator1Sounds.Play
        ElevatorCheck2 = 1
        End If
        'Movement
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable < 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then ElevatorCheck = 0: FineTune = True
        End If
      
        If ElevatorEnable <= 0 And ElevatorDirection = -1 Then
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck = 0 Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstart.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 100
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y + 20, -23)
        Elevator1Sounds.Play
        ElevatorCheck = 1
        End If
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck = 1 Then
        Elevator1Sounds.Load App.Path + "\elevmove.wav"
        Elevator1Sounds.Loop_ = True
        Elevator1Sounds.Play
        End If
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable >= -15 Then StoppingDistance = OriginalLocation - CurrentFloorExact
        If ElevatorEnable < -15 Then ElevatorEnable = -15
        Destination = ((OriginalLocation * 25) + 25) - DistanceToTravel
        If GotoFloor <> 0 And elevator1start.Y <= (Destination + (StoppingDistance * 25) + 25) Then ElevatorDirection = 1: ElevatorCheck = 0
        End If
      
        If ElevatorEnable < 0 And ElevatorDirection = 1 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Stop_
        End If
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstop.wav"
        Elevator1Sounds.Play
        ElevatorCheck2 = 1
        End If
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable > 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then ElevatorCheck = 0: FineTune = True
        End If
      
      If FineTune = True And ElevatorEnable = 0 And elevator1start.Y > (GotoFloor * 25) + 25 + -0.3 And elevator1start.Y < (GotoFloor * 25) + 25 + 0.3 Then
      FineTune = False
      Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      GotoFloor = 0
      OpenElevator = 1
      ElevatorTimer.Enabled = False
      ElevatorCheck = 0
      ElevatorCheck2 = 0
      ElevatorCheck3 = 0
      ElevatorCheck4 = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicator <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune = True Then
      If ElevatorCheck3 = 0 Then
      Elevator1Sounds.Load App.Path + "\ding1.wav"
      Elevator1Sounds.Play
      ElevatorCheck3 = 1
      End If
      If elevator1start.Y < (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, 0.2, 0
      Elevator1DoorL.MoveRelative 0, 0.2, 0
      Elevator1DoorR.MoveRelative 0, 0.2, 0
      If ElevatorSync = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevator1start.Y > (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, -0.2, 0
      Elevator1DoorL.MoveRelative 0, -0.2, 0
      Elevator1DoorR.MoveRelative 0, -0.2, 0
      If ElevatorSync = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator = 1 Then
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z >= 4 Then OpenElevator = 0: GoTo OpenElevator1
      If ElevatorCheck4 = 0 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Elevator1Sounds.Stop_
        End If
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevatoropen.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 1000
        Call Elevator1Sounds.SetConeOrientation(0, -5, 0)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 1 Then OpenElevator = 2
OpenElevator1:
      End If
      
      If OpenElevator = 2 Then
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 3 Then OpenElevator = 3
      End If
      
      If OpenElevator = 3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 7 And OpenElevatorLoc = 0 Then OpenElevatorLoc = 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc <= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Timer1.Enabled = True
      End If
      End If
      
      If OpenElevator = -1 Then
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then OpenElevator = 0: GoTo OpenElevator2
      If ElevatorCheck4 = 0 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Elevator1Sounds.Stop_
        End If
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevatorclose.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 1000
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 3 Then OpenElevator = -2
OpenElevator2:
      End If
      
      If OpenElevator = -2 Then
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 1 Then OpenElevator = -3
      End If
      
      If OpenElevator = -3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 0 And OpenElevatorLoc >= 0 Then OpenElevatorLoc = -0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc >= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Elevator1DoorL.SetPosition Elevator1DoorL.GetPosition.X, Elevator1DoorL.GetPosition.Y, 0
      Elevator1DoorR.SetPosition Elevator1DoorR.GetPosition.X, Elevator1DoorR.GetPosition.Y, 0
      ElevatorDoorL(ElevatorFloor2).SetPosition ElevatorDoorL(ElevatorFloor2).GetPosition.X, ElevatorDoorL(ElevatorFloor2).GetPosition.Y, 0
      ElevatorDoorR(ElevatorFloor2).SetPosition ElevatorDoorR(ElevatorFloor2).GetPosition.X, ElevatorDoorR(ElevatorFloor2).GetPosition.Y, 0
      End If
      End If
      
      lineend = Camera.GetPosition
    
End Sub

Private Sub Form_GotFocus()
Focused = True
End Sub





Sub Init_Simulator()
'On Error GoTo ErrorHandler
isRunning = True
Form2.Show
Me.Show
Set TV = New TVEngine
Set Scene = New TVScene
Set Mesh = New TVMesh
Set External = New TVMesh
For i = 1 To 138
Set Room(i) = New TVMesh
Set Stairs(i) = New TVMesh
Set ShaftsFloor(i) = New TVMesh
Next i
For i = -1 To 138
Set ElevatorDoorL(i) = New TVMesh
Set ElevatorDoorR(i) = New TVMesh
Next i
Set Shafts = New TVMesh
Set Elevator1 = New TVMesh
Set Elevator1DoorL = New TVMesh
Set Elevator1DoorR = New TVMesh

Set Camera = New TVCamera

Set TextureFactory = New TVTextureFactory
Set SoundEngine = New TV3DMedia.TVSoundEngine
'Set ElevatorMusic = New TV3DMedia.TVSoundWave3D
Set Elevator1Sounds = New TV3DMedia.TVSoundWave3D
Set Light = New TVLightEngine


   
Print " "
Print "Skyscraper 0.7b Alpha"
Print "©2003 Ryan Thoryk"
Print " "
Print "Skyscraper comes with ABSOLUTELY NO WARRANTY. This is free"
Print "software, and you are welcome to redistribute it under certain"
Print "conditions. For details, see the file gpl.txt"
Print " "

  If TV.ShowDriverDialog = False Then End
   DoEvents
   '2. Initialize the engine with the selected mode
   TV.SetSearchDirectory App.Path
Print "Initializing TrueVision3D..."
   'TV.Initialize Form1.hWnd
    TV.Init3DWindowedMode Form1.hWnd
    'TV.Init3DFullscreen 640, 480, 16

Print "TrueVision3D Initialized"
  Set Inp = New TVInputEngine
  TV.SetSearchDirectory App.Path
  TV.DisplayFPS = True
  Set Mesh = Scene.CreateMeshBuilder("Mesh")
  Set External = Scene.CreateMeshBuilder("External")
  For i = 1 To 138
  Set Room(i) = Scene.CreateMeshBuilder("Room " + Str$(i))
  Set Stairs(i) = Scene.CreateMeshBuilder("Stairs " + Str$(i))
  Set ShaftsFloor(i) = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(i))
  Next i
  For i = -1 To 138
  Set ElevatorDoorL(i) = Scene.CreateMeshBuilder("ElevatorDoorL " + Str$(i))
  Set ElevatorDoorR(i) = Scene.CreateMeshBuilder("ElevatorDoorR " + Str$(i))
  Next i
  Set Shafts = Scene.CreateMeshBuilder("Shafts")
  Set Elevator1 = Scene.CreateMeshBuilder("Elevator1")
  Set Elevator1DoorL = Scene.CreateMeshBuilder("Elevator1DoorL")
  Set Elevator1DoorR = Scene.CreateMeshBuilder("Elevator1DoorR")
  
  
Print "Mesh Areas Created"
  Scene.SetViewFrustum 90, 10000
  'TextureFactory.LoadTexture "..\..\..\media\stone_wall.bmp", "Floor"
  
  TextureFactory.LoadTexture "brick1.bmp", "BrickTexture"
  TextureFactory.LoadTexture "LobbyFront.jpg", "LobbyFront"
  TextureFactory.LoadTexture "windows11c.jpg", "MainWindows"
  TextureFactory.LoadTexture "granite.bmp", "Granite"
  'TextureFactory.LoadTexture "marbl3.bmp", "Marble3"
  TextureFactory.LoadTexture "text12.bmp", "Marble3"
  TextureFactory.LoadTexture "marb047.bmp", "Marble4"
  TextureFactory.LoadTexture "elev1.bmp", "Elev1"
  TextureFactory.LoadTexture "textur15.bmp", "Wood1"
  TextureFactory.LoadTexture "text16.bmp", "Wood2"
  'TextureFactory.LoadTexture "text12.bmp", "Wall1"
  TextureFactory.LoadTexture "marbl3.bmp", "Wall1"
  TextureFactory.LoadTexture "marb123.jpg", "Wall2"
  TextureFactory.LoadTexture "cutston.bmp", "Ceiling1"
  TextureFactory.LoadTexture "symb5.bmp", "Wall3"
  TextureFactory.LoadTexture "text16.bmp", "ElevDoors"
  TextureFactory.LoadTexture "marb148.jpg", "ElevExtPanels"
  
  TextureFactory.LoadTexture "top.bmp", "SkyTop"
  TextureFactory.LoadTexture "bottom.bmp", "SkyBottom"
  TextureFactory.LoadTexture "left.bmp", "SkyLeft"
  TextureFactory.LoadTexture "right.bmp", "SkyRight"
  TextureFactory.LoadTexture "front.bmp", "SkyFront"
  TextureFactory.LoadTexture "back.bmp", "SkyBack"
  TextureFactory.LoadTexture App.Path + "\objects\benedeti.jpg", "ColumnTex", , , TV_COLORKEY_NO

   
Print "Textures Loaded"
      
  'Sound System
    
    Call SoundEngine.Init(Form1.hWnd)
    
Print "Sound Initialized"

    'Load the file into the classes.
    'ElevatorMusic.Load App.Path + "\elevmusic3.wav"
    'ElevatorMusic.Load App.Path + "\elevmusic2.wav"
'Print "Music Loaded"
    'Set sound properties.
    'ElevatorMusic.Volume = -300
    'ElevatorMusic.maxDistance = 1000
    'Call ElevatorMusic.SetConeOrientation(0, 0, 90)
    'ElevatorMusic.ConeOutsideVolume = -300
    'Call ElevatorMusic.SetPosition(-20.25, 20, -23)
    'Call ElevatorMusic.SetPosition(0, 10, 0)
    'Setup the 3D listener.
    'Set Listener = SoundEngine.Get3DListener
    'Call Listener.SetPosition(picDraw.ScaleWidth / 2, 0, picDraw.ScaleHeight / 2)
    'Call Listener.SetPosition(0, 0, 0)
    'Listener.rolloffFactor = 0.1
    'Listener.distanceFactor = 50
Print "Loading 3D Objects..."
End Sub

Sub ProcessFloors()
    'Elevator Mesh are 19 feet in from the north, 51 feet in from the west and are 8 feet wide
    'Lobby
Call ProcessLobby
'GoTo 2
Print "Processing Floors 2 to 39...";
Call Process2to39
'GoTo 2
Print "Processing Floors 40 to 79";
Call Process40to79
Print "Processing Floors 80 to 117";
Call Process80to117
Print "Processing Floors 118 to 134";
Call Process118to132
Call ProcessOtherFloors


2

    Shafts.AddWall GetTex("BrickTexture"), -12.5, -30.85, -32.5, -30.85, (25 * 139) + 25, 0, 1, 139
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -30.85, 32.5, -30.85, (25 * 137) + 25, 0, 1, 139
    
    'Shafts.AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, (25 * 138) + 25, 0, 2, 139 * 2
        
    Stairs(1).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 22, 0, 2, 2
        
    Stairs(138).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12 - 20, -30.85, (138 * 25) + 25, 1, 1
    
    'Shafts.AddFloor GetTex("BrickTexture"), -60, -150, 60, 150, (138 * 25) + 25, 10, 10

    'Elevator1
    'Old one Elevator1.AddFloor GetTex("BrickTexture"), -32, -30, -12, -16, 1
    Elevator1.AddFloor GetTex("Wood2"), -28, -30, -12.5, -16, 0.1, 1, 1
    Elevator1.AddFloor GetTex("Elev1"), -28, -30, -12.5, -16, 19.5, 2, 2
    Elevator1.AddWall GetTex("Wood1"), -28, -30, -12.5, -30, 19.5, 0.1, 2, 2
    'Elevator1.AddWall GetTex("BrickTexture"), -12, -30, -12, -16, 19.5
    Elevator1.AddWall GetTex("Wood1"), -28, -16, -12.5, -16, 19.5, 0.1, 2, 2
    Elevator1.AddWall GetTex("Wood1"), -28, -16, -28, -30, 19.5, 0.1, 2, 2
    'Interior Panels
    Elevator1.AddWall GetTex("Wall3"), -12.65, -16, -12.65, -19, 19.5, 0.1, 1, 1
    Elevator1.AddWall GetTex("Wall3"), -12.65, -30, -12.65, -27, 19.5, 0.1, 1, 1
    'Interior Doors
    Elevator1DoorL.AddWall GetTex("ElevDoors"), -12.6, -19.05, -12.6, -22.95, 19.5, 0.1, 1, 1
    Elevator1DoorR.AddWall GetTex("ElevDoors"), -12.6, -23.05, -12.6, -27.05, 19.5, 0.1, 1, 1
    'Exterior Doors
    ElevatorDoorL(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05, -12.55, -22.95, 19.6, 0, 1, 1
    ElevatorDoorR(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05, -12.55, -27.05, 19.6, 0, 1, 1
    ElevatorDoorL(0).AddWall GetTex("ElevDoors"), -12.55, -19.05, -12.55, -22.95, 19.6, 27, 1, 1
    ElevatorDoorR(0).AddWall GetTex("ElevDoors"), -12.55, -23.05, -12.55, -27.05, 19.6, 27, 1, 1
    For i = 2 To 138
    ElevatorDoorL(i).AddWall GetTex("ElevDoors"), -12.55, -19, -12.55, -23, 19.6, (i * 25) + 25, 1, 1
    ElevatorDoorR(i).AddWall GetTex("ElevDoors"), -12.55, -23, -12.55, -27, 19.6, (i * 25) + 25, 1, 1
    'Exterior Panels
    Room(i).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, (i * 25) + 25, 0.5, 1
    Room(i).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, (i * 25) + 25, 0.5, 1
    
    Next i
    
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, 0, 0.5, 1
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, 0, 0.5, 1
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, 27, 0.5, 1
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, 27, 0.5, 1
    
    Print "Processing Stairs...";
    
i = 1
'Stairs on the first floor, section 1
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 - 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 - 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 - 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 - 25
    
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 - 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 - 25

'Stairs on the first floor, section 2
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14
    
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12

'Stairs on the first floor, section 3
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 + 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 + 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 + 25
    
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 + 25


    For i = 2 To 137
    Print ".";
    'Stairs
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 + 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 + 25
    
    Stairs(i).AddFloor GetTex("Wood2"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 + 25
    Stairs(i).AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 + 25
    
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 + 25
    Stairs(i).AddWall GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 + 25
    
    Next i
    Print "done"

End Sub
Private Sub Form_Load()
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
  For i = 1 To 138
  Room(i).Enable False
  Stairs(i).Enable False
  ShaftsFloor(i).Enable False
  Next i
  For i = -1 To 138
  ElevatorDoorL(i).Enable False
  ElevatorDoorR(i).Enable False
  Next i
  Room(1).Enable True
  ElevatorDoorL(-1).Enable True
  ElevatorDoorR(-1).Enable True
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
  Print "Optimizing Meshes.";
  For i = 1 To 138
  Room(i).Optimize
  Stairs(i).Optimize
  Next i
  Print ".";
  For i = -1 To 138
  ElevatorDoorL(i).Optimize
  ElevatorDoorR(i).Optimize
  Next i
  Print ".";
  External.Optimize
  Print ".";
  Shafts.Optimize
  Print ".";
  Elevator1.Optimize
  Print ".";
  Elevator1DoorL.Optimize
  Print ".";
  Elevator1DoorR.Optimize
  Print "Done"
  
  'ElevatorMusic.Loop_ = True
  'Call ElevatorMusic.Play
  
  Atmos.SkyBox_SetTexture GetTex("SkyFront"), GetTex("SkyBack"), GetTex("SkyLeft"), GetTex("SkyRight"), GetTex("SkyTop"), GetTex("SkyBottom")
  Atmos.SkyBox_Enable True
    
MainTimer.Enabled = True
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
'Set ElevatorMusic = Nothing
Set Elevator1Sounds = Nothing

    'Set Listener = Nothing
    Set SoundEngine = Nothing
    End
End Sub

Private Sub MainTimer_Timer()

'Simple Frame Limiter

'If TV.GetFPS > 20 And GotoFloor = 0 Then
'MainTimer.Interval = (TV.GetFPS - 20) * 1.5
'End If
If GotoFloor <> 0 Then
MainTimer.Interval = 45
Else
'End If
'If TV.GetFPS < 20 Then
MainTimer.Interval = 1
End If


Dim a As Single
''update lights
'a = a + TV.TimeElapsed * 0.001
'      LightD.Position = Vector(0, 10, Sin(a) * 50 + 50)
'      Light.UpdateLight 1, LightD
      
      
'Update the floor indicator
If FloorIndicator <> "M" Then FloorIndicator = CameraFloor
Form2.Label1.Caption = FloorIndicator
If Camera.GetPosition.Y > 27 And CameraFloor < 2 Then
FloorIndicator = "M"
Else
FloorIndicator = 1
End If

ElevatorFloor2 = ((Elevator1.GetPosition.Y - 10) / 25) - 1
CameraFloor2 = ((Camera.GetPosition.Y - 10) / 25) - 1

'this determines if the person is inside the stairwell shaft or not, and sets a variable accordingly.
If Camera.GetPosition.X < -12.5 And Camera.GetPosition.X > -32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30 Then
InStairwell = True
Else
InStairwell = False
End If

'floors 135 and 136 are combined. this simply makes sure that they are off when not in use :)
If CameraFloor <> 136 And CameraFloor <> 135 Then
Room(136).Enable False
ElevatorDoorL(136).Enable False
ElevatorDoorR(136).Enable False
Room(135).Enable False
ElevatorDoorL(135).Enable False
ElevatorDoorR(135).Enable False
ShaftsFloor(135).Enable False
ShaftsFloor(136).Enable False
Else
If CameraFloor = 136 Or CameraFloor = 135 Then
    If GotoFloor = 0 And InStairwell = False Then
    Room(136).Enable True
    ElevatorDoorL(136).Enable True
    ElevatorDoorR(136).Enable True
    Room(135).Enable True
    ElevatorDoorL(135).Enable True
    ElevatorDoorR(135).Enable True
    ShaftsFloor(135).Enable True
    ShaftsFloor(136).Enable True
    End If
End If
End If
'This section turns on and off the external mesh (outside windows, not inside windows), based on the camera's current location
If CameraFloor >= 1 And CameraFloor <= 39 Then
    If Camera.GetPosition.X < -160 Or Camera.GetPosition.X > 160 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    External.Enable True
    Else
    External.Enable False
    End If
End If
If CameraFloor >= 40 And CameraFloor <= 79 Then
    If Camera.GetPosition.X < -135 Or Camera.GetPosition.X > 135 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    External.Enable True
    Else
    External.Enable False
    End If
End If
If CameraFloor >= 80 And CameraFloor <= 117 Then
    If Camera.GetPosition.X < -110 Or Camera.GetPosition.X > 110 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    External.Enable True
    Else
    External.Enable False
    End If
End If
If CameraFloor >= 118 And CameraFloor <= 134 Then
    If Camera.GetPosition.X < -85 Or Camera.GetPosition.X > 85 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    External.Enable True
    Else
    External.Enable False
    End If
End If
If CameraFloor >= 135 And CameraFloor <= 138 Then
    If Camera.GetPosition.X < -60 Or Camera.GetPosition.X > 60 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    External.Enable True
    Else
    External.Enable False
    End If
End If

'This section turns on and off the Shafts mesh (inside the elevator and pipe shafts) when the camera is located inside them.
If CameraFloor = ElevatorFloor And Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < -16 And CameraFloor <> 137 Then
Shafts.Enable False
'Atmos.SkyBox_Enable False
GoTo EndShafts
Else
'Atmos.SkyBox_Enable True
End If

If CameraFloor = 137 Then GoTo EndShafts
If GotoFloor <> 0 Then GoTo EndShafts

If CameraFloor >= 1 And CameraFloor <= 39 Then
    'right shaft (the one with the stairs)
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 46.25 Then
    Shafts.Enable True
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 46.25 Then
    Shafts.Enable True
    Else
    Shafts.Enable False
    End If
    End If
    
End If
If CameraFloor >= 40 And CameraFloor <= 79 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 30.83 Then
    Shafts.Enable True
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 30.83 Then
    Shafts.Enable True
    Else
    Shafts.Enable False
    End If
    End If
End If
If CameraFloor >= 80 And CameraFloor <= 117 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 15.41 Then
    Shafts.Enable True
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 15.41 Then
    Shafts.Enable True
    Else
    Shafts.Enable False
    End If
    End If
End If
If CameraFloor >= 118 And CameraFloor <= 134 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 0 Then
    Shafts.Enable True
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 0 Then
    Shafts.Enable True
    Else
    Shafts.Enable False
    End If
    End If
End If
If CameraFloor >= 135 And CameraFloor <= 138 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < -15.42 Then
    Shafts.Enable True
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -15.42 Then
    Shafts.Enable True
    Else
    Shafts.Enable False
    End If
    End If
End If
EndShafts:

    linestart = Camera.GetPosition
    elevator1start = Elevator1.GetPosition

    TV.Clear
    'Call Listener.SetPosition(Camera.GetPosition.X, Camera.GetPosition.Y, Camera.GetPosition.z)
    'Call ElevatorMusic.GetPosition(ListenerDirection.X, ListenerDirection.Y, ListenerDirection.z)
    'Call Camera.GetRotation(ListenerDirection.x, ListenerDirection.y, ListenerDirection.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    
    
'*** First movement system


      If Inp.IsKeyPressed(TV_KEY_UP) = True Then
      KeepAltitude = Camera.GetPosition.Y
      Camera.MoveRelative 0.7, 0, 0
      If Camera.GetPosition.Y <> KeepAltitude Then Camera.SetPosition Camera.GetPosition.X, KeepAltitude, Camera.GetPosition.z
      End If
      
      If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then
      KeepAltitude = Camera.GetPosition.Y
      Camera.MoveRelative -0.7, 0, 0
      If Camera.GetPosition.Y <> KeepAltitude Then Camera.SetPosition Camera.GetPosition.X, KeepAltitude, Camera.GetPosition.z
      End If
      
      If Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then Camera.RotateY 0.05
      If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then Camera.RotateY -0.05
      
'*** Second movement system (has horrible problems)
      'If Inp.IsKeyPressed(TV_KEY_UP) = True Then Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z - (Camera.GetLookAt.z / 100), Camera.GetLookAt.x, Camera.GetLookAt.y, Camera.GetLookAt.z - 0.7
      'If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x, Camera.GetLookAt.y, Camera.GetLookAt.z + 0.7
      'If Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then Camera.SetCamera Camera.GetPosition.x - 0.7, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x - 0.7, Camera.GetLookAt.y, Camera.GetLookAt.z
      'If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then Camera.SetCamera Camera.GetPosition.x + 0.7, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x + 0.7, Camera.GetLookAt.y, Camera.GetLookAt.z
            
'*** Third (new) Movement System, should be released with version 0.6 (I had problems with it)
'       If Inp.IsKeyPressed(TV_KEY_UP) = True Then
'            sngWalk = 1
'
'        ' If we are not walking forward, maybe we are walking backward
'        ' by using the DOWN arrow? If so, set walk speed to negative.
'        ElseIf Inp.IsKeyPressed(TV_KEY_DOWN) = True Then
'           sngWalk = -1
'        End If
'        ' Check if we pressed the LEFT arrow key, if so, then strafe
'        ' on the left.
'        If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then
'            sngStrafe = 1
'        ' If we are not strafing left, maybe we want to strafe to the
'        ' right, using the RIGHT arrow? If so, set strafe to negative.
'        ElseIf Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then
'            sngStrafe = -1
'        End If
'
'       ' Now, for the mouse input...
'        Dim tmpMouseX As Long, tmpMouseY As Long
'        Dim tmpMouseB1 As Integer, tmpMouseB2 As Integer, tmpMouseB3 As Integer
'        Dim tmpMouseScrollOld As Long, tmpMouseScrollNew As Long
'
'        ' We pass the actual value of the mouse scroller to the variable
'        ' that holds the old mouse scroller value.
'        tmpMouseScrollOld = tmpMouseScrollNew
'
'        ' By using GetMouseState, we get the movement of the mouse
'        ' with the speed of the movement. The fastest the mouse movement
'        ' will be, the higher will be the return.
'        If Focused = True Then Inp.GetMouseState tmpMouseX, tmpMouseY, tmpMouseB1, tmpMouseB2, tmpMouseB3, tmpMouseScrollNew
'
'        ' From the mouse return values, we update the camera angles
'        ' by adding or substracting the mouse return value.
'        sngAngleX = sngAngleX - (tmpMouseY / 100)
'        sngAngleY = sngAngleY - (tmpMouseX / 100) '
'
'        ' We will add a simple check, so we can't look up at more
'        ' than 80 degrees nor down than -80 degrees.
'        If sngAngleX > 1.3 Then sngAngleX = 1.3
'        If sngAngleX < -1.3 Then sngAngleX = -1.3
'
' ' Okay, now for the smothing of the movement... We checked
'        ' above if we were pressing a key. If so, then we updated the
'        ' movement variable to 1 (positive or negative). Here, we
'        ' lower this value until it get to 0. This method give us a
'        ' smoother camera movement. We start by updating the forward
'        ' and backward (walk) movement.
'
'        Select Case sngWalk
'        Case Is > 0
'            sngWalk = sngWalk - 0.15
'            If sngWalk < 0 Then sngWalk = 0
'        Case Is < 0
'            sngWalk = sngWalk + 0.15
'            If sngWalk > 0 Then sngWalk = 0
'        End Select
'
'        ' Now, we update the left and right (strafe) movement.
'        Select Case sngStrafe
'        Case Is > 0
'            sngStrafe = sngStrafe - 0.15
'            If sngStrafe < 0 Then sngStrafe = 0
'        Case Is < 0
'            sngStrafe = sngStrafe + 0.15
'            If sngStrafe > 0 Then sngStrafe = 0
'        End Select
'
'        ' Update the vectors using the angles and positions.
'        sngPositionX = sngPositionX + (Cos(sngAngleY) * sngWalk * (TV.TimeElapsed / 70)) + (Cos(sngAngleY + 3.141596 / 2) * sngStrafe * (TV.TimeElapsed / 70))
'        sngPositionZ = sngPositionZ + (Sin(sngAngleY) * sngWalk * (TV.TimeElapsed / 70)) + (Sin(sngAngleY + 3.141596 / 2) * sngStrafe * (TV.TimeElapsed / 70))
'
'        snglookatX = sngPositionX + Cos(sngAngleY)
'        snglookatY = sngPositionY + Tan(sngAngleX)
'        snglookatZ = sngPositionZ + Sin(sngAngleY)

'        ' With the new values of the camera vectors (position and
'        ' look at), we update the scene's camera.
'        Scene.SetCamera sngPositionX, sngPositionY, sngPositionZ, snglookatX, snglookatY, snglookatZ
        
      If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True Then Camera.RotateX -0.006
      If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True Then Camera.RotateX 0.006
      
      If Inp.IsKeyPressed(TV_KEY_HOME) = True Then Camera.MoveRelative 0, 1, 0
      If Inp.IsKeyPressed(TV_KEY_END) = True Then Camera.MoveRelative 0, -1, 0
      'If Inp.IsKeyPressed(TV_KEY_1) = True Then ElevatorDirection = 1
      'If Inp.IsKeyPressed(TV_KEY_2) = True Then ElevatorDirection = -1
      If Inp.IsKeyPressed(TV_KEY_3) = True Then OpenElevator = 1
      If Inp.IsKeyPressed(TV_KEY_4) = True Then OpenElevator = -1
      'If Inp.IsKeyPressed(TV_KEY_5) = True Then Call ElevatorMusic.Play
      'If Inp.IsKeyPressed(TV_KEY_6) = True Then Call ElevatorMusic.Stop_
      'If Inp.IsKeyPressed(TV_KEY_8) = True Then External.SetBlendingMode (TV_BLEND_ALPHA): External.SetColor RGBA(1, 1, 1, 0.1)
      'If Inp.IsKeyPressed(TV_KEY_G) = True Then Objects(55 + (100 * (2 - 1))).SetRotation 0, Objects(55 + (100 * (2 - 1))).GetRotation.Y + 0.01, 0
      'If Inp.IsKeyPressed(TV_KEY_H) = True Then Objects(55 + (100 * (2 - 1))).SetRotation 0, Objects(55 + (100 * (2 - 1))).GetRotation.Y - 0.01, 0
      'If Inp.IsKeyPressed(TV_KEY_J) = True Then MsgBox (Objects(55 + (100 * (2 - 1))).GetRotation.Y)
      
        'Inp.GetMouseState MousePositionX, MousePositionY
      'MsgBox (Str$(MousePositionX) + "," + Str$(MousePositionY))
      'Camera.SetLookAt -50, -50, -50
      'Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x - (MousePositionX / 100), Camera.GetLookAt.y - (MousePositionY / 100), Camera.GetLookAt.z
      
      'Orientation: 0-1 pan to center, -1 left 1 right, ?, same as 1st, same as 2nd, ?
      'If Inp.IsKeyPressed(TV_KEY_7) = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
      'If Inp.IsKeyPressed(TV_KEY_8) = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
        
      If Inp.IsKeyPressed(TV_KEY_F1) = True Then TV.ScreenShot ("c:\shot.bmp")
      
      ElevatorFloor = (Elevator1.GetPosition.Y - 25) / 25
      If ElevatorFloor < 1 Then ElevatorFloor = 1
      
      If InStairwell = False Then CameraFloor = (Camera.GetPosition.Y - 25 - 10) / 25
      If CameraFloor < 1 Then CameraFloor = 1
      
      If GotoFloor <> 0 And GotoFloor > CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then
      ElevatorDirection = 1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((GotoFloor * 25) + 25) - ((CurrentFloorExact * 25) + 25)
      If ElevatorSync = True Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      Stairs(CameraFloor).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      DestroyObjects (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      If GotoFloor <> 0 And GotoFloor < CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then
      ElevatorDirection = -1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((CurrentFloorExact * 25) + 25) - ((GotoFloor * 25) + 25)
      If ElevatorSync = True Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      DestroyObjects (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor = Int((elevator1start.Y - 25) / 25)
       CurrentFloorExact = (elevator1start.Y - 25) / 25
       'CurrentFloor = (Elevator1.GetPosition.y / 25) - 1
       'CurrentFloorExact = Int((Elevator1.GetPosition.y / 25) - 1)

       Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.X)) + "," + Str$(Int(Camera.GetPosition.Y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "GotoFloor=" + Str$(GotoFloor) + vbCrLf + "DistancetoDest=" + Str$(Abs(GotoFloor - CurrentFloor)) + vbCrLf + "Rate=" + Str$(ElevatorEnable / 5)
        
        If ElevatorEnable >= 0 And ElevatorDirection = 1 Then
        'sound
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck = 0 And GotoFloor <> ElevatorFloor Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstart.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 100
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y + 20, -23)
        Elevator1Sounds.Play
        ElevatorCheck = 1
        End If
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck = 1 Then
        Elevator1Sounds.Load App.Path + "\elevmove.wav"
        Elevator1Sounds.Loop_ = True
        Elevator1Sounds.Play
        End If
        'movement
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable <= 15 Then StoppingDistance = CurrentFloorExact - OriginalLocation + 0.4
        If ElevatorEnable > 15 Then ElevatorEnable = 15
        Destination = ((OriginalLocation * 25) + 25) + DistanceToTravel - 35
        If GotoFloor <> 0 And elevator1start.Y >= (Destination - (StoppingDistance * 25) + 25) Then ElevatorDirection = -1: ElevatorCheck = 0
        End If
      
        If ElevatorEnable > 0 And ElevatorDirection = -1 Then
        'Sounds
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Stop_
        End If
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstop.wav"
        Elevator1Sounds.Play
        ElevatorCheck2 = 1
        End If
        'Movement
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable < 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then ElevatorCheck = 0: FineTune = True
        End If
      
        If ElevatorEnable <= 0 And ElevatorDirection = -1 Then
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck = 0 Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstart.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 100
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y + 20, -23)
        Elevator1Sounds.Play
        ElevatorCheck = 1
        End If
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck = 1 Then
        Elevator1Sounds.Load App.Path + "\elevmove.wav"
        Elevator1Sounds.Loop_ = True
        Elevator1Sounds.Play
        End If
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable >= -15 Then StoppingDistance = OriginalLocation - CurrentFloorExact
        If ElevatorEnable < -15 Then ElevatorEnable = -15
        Destination = ((OriginalLocation * 25) + 25) - DistanceToTravel
        If GotoFloor <> 0 And elevator1start.Y <= (Destination + (StoppingDistance * 25) + 25) Then ElevatorDirection = 1: ElevatorCheck = 0
        End If
      
        If ElevatorEnable < 0 And ElevatorDirection = 1 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Stop_
        End If
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2 = 0 And FineTune = False Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstop.wav"
        Elevator1Sounds.Play
        ElevatorCheck2 = 1
        End If
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorL.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1DoorR.MoveRelative 0, (ElevatorEnable / 5), 0
        If ElevatorSync = True Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        'ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.Y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable > 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then ElevatorCheck = 0: FineTune = True
        End If
      
      If FineTune = True And ElevatorEnable = 0 And elevator1start.Y > (GotoFloor * 25) + 25 + -0.3 And elevator1start.Y < (GotoFloor * 25) + 25 + 0.3 Then
      FineTune = False
      Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor = 137 Then Shafts.Enable True
      
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      GotoFloor = 0
      OpenElevator = 1
      ElevatorCheck = 0
      ElevatorCheck2 = 0
      ElevatorCheck3 = 0
      ElevatorCheck4 = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicator <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune = True Then
      If ElevatorCheck3 = 0 Then
      Elevator1Sounds.Load App.Path + "\ding1.wav"
      Elevator1Sounds.Play
      ElevatorCheck3 = 1
      End If
      If elevator1start.Y < (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, 0.2, 0
      Elevator1DoorL.MoveRelative 0, 0.2, 0
      Elevator1DoorR.MoveRelative 0, 0.2, 0
      If ElevatorSync = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevator1start.Y > (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, -0.2, 0
      Elevator1DoorL.MoveRelative 0, -0.2, 0
      Elevator1DoorR.MoveRelative 0, -0.2, 0
      If ElevatorSync = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator = 1 Then
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z >= 4 Then OpenElevator = 0: GoTo OpenElevator1
      If ElevatorCheck4 = 0 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Elevator1Sounds.Stop_
        End If
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevatoropen.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 1000
        Call Elevator1Sounds.SetConeOrientation(0, -5, 0)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 1 Then OpenElevator = 2
OpenElevator1:
      End If
      
      If OpenElevator = 2 Then
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 3 Then OpenElevator = 3
      End If
      
      If OpenElevator = 3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 7 And OpenElevatorLoc = 0 Then OpenElevatorLoc = 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc <= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Timer1.Enabled = True
      End If
      End If
      
      If OpenElevator = -1 Then
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z <= 0 Then OpenElevator = 0: GoTo OpenElevator2
      If ElevatorCheck4 = 0 Then
        If Elevator1Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Elevator1Sounds.Stop_
        End If
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevatorclose.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 1000
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.Y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 3 Then OpenElevator = -2
OpenElevator2:
      End If
      
      If OpenElevator = -2 Then
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z < 1 Then OpenElevator = -3
      End If
      
      If OpenElevator = -3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      If ElevatorDoorL(ElevatorFloor2).GetPosition.z > 0 And OpenElevatorLoc >= 0 Then OpenElevatorLoc = -0.02
      ElevatorDoorL(ElevatorFloor2).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor2).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc >= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Elevator1DoorL.SetPosition Elevator1DoorL.GetPosition.X, Elevator1DoorL.GetPosition.Y, 0
      Elevator1DoorR.SetPosition Elevator1DoorR.GetPosition.X, Elevator1DoorR.GetPosition.Y, 0
      ElevatorDoorL(ElevatorFloor2).SetPosition ElevatorDoorL(ElevatorFloor2).GetPosition.X, ElevatorDoorL(ElevatorFloor2).GetPosition.Y, 0
      ElevatorDoorR(ElevatorFloor2).SetPosition ElevatorDoorR(ElevatorFloor2).GetPosition.X, ElevatorDoorR(ElevatorFloor2).GetPosition.Y, 0
      End If
      End If
      
      lineend = Camera.GetPosition
      
       'Stairs Movement
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      DestroyObjects (CameraFloor)
      'Stairs(CameraFloor).Enable False
      
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
    If CameraFloor >= 2 Then CameraFloor = CameraFloor + 1
    If CameraFloor = 1 Then PartialFloor = PartialFloor + 25
    If CameraFloor = 1 And PartialFloor >= 25 Then PartialFloor = 0: CameraFloor = 2
      Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor = 137 Then Shafts.Enable True
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
    Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 2 + PartialFloor Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
   
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      DestroyObjects (CameraFloor)
      
      'Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
    If CameraFloor = 1 Then PartialFloor = PartialFloor - 25
    If CameraFloor = 2 Then PartialFloor = 0: CameraFloor = 1
    If CameraFloor = 1 And PartialFloor <= -50 Then PartialFloor = -50
    If CameraFloor > 2 Then CameraFloor = CameraFloor - 1
     Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor = 137 Then Shafts.Enable True
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
    Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.Y <> 10 Then
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 20 + PartialFloor Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 8 And Camera.GetPosition.X > -12.5 - 10 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 20 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 10 And Camera.GetPosition.X > -12.5 - 12 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 18 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 12 And Camera.GetPosition.X > -12.5 - 14 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 16 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 14 And Camera.GetPosition.X > -12.5 - 16 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 14 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.Y = 10 And PartialFloor = -50 Then PartialFloor = 0
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 And PartialFloor = 0 And Camera.GetPosition.Y = 10 Then PartialFloor = -50
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 2 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 8 And Camera.GetPosition.X > -12.5 - 10 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 4 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 10 And Camera.GetPosition.X > -12.5 - 12 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 6 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 12 And Camera.GetPosition.X > -12.5 - 14 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 8 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 14 And Camera.GetPosition.X > -12.5 - 16 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 10 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 16 And Camera.GetPosition.X > -12.5 - 20 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 12 + PartialFloor, Camera.GetPosition.z
    
      
GoTo CollisionEnd

    'Elevator1 Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If Elevator1.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If Elevator1.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
        
 'ElevatorDoorL Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If ElevatorDoorL(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If ElevatorDoorL(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If ElevatorDoorL(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If ElevatorDoorL(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
       
'ElevatorDoorR Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If ElevatorDoorR(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If ElevatorDoorR(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If ElevatorDoorR(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If ElevatorDoorR(CameraFloor2).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
        
'Room Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Room(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Room(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If Room(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If Room(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
            
'Shafts Collision
    
    LineTest.X = Camera.GetPosition.X + 2
    LineTest.Y = Camera.GetPosition.Y
    LineTest.z = Camera.GetPosition.z + 2
    LineTest2.X = Camera.GetPosition.X - 2
    LineTest2.Y = Camera.GetPosition.Y
    LineTest2.z = Camera.GetPosition.z + 2
    If Shafts.Collision(LineTest, LineTest2, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = Camera.GetPosition.X + 2
    LineTest.Y = Camera.GetPosition.Y
    LineTest.z = Camera.GetPosition.z - 2
    LineTest2.X = Camera.GetPosition.X - 2
    LineTest2.Y = Camera.GetPosition.Y
    LineTest2.z = Camera.GetPosition.z - 2
    'If Inp.IsKeyPressed(TV_KEY_7) = True Then Shafts.AddWall GetTex("Wood2"), LineTest.x, LineTest.z, LineTest2.x, LineTest2.z, 25
    If Shafts.Collision(LineTest, LineTest2, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = Camera.GetPosition.X + 2
    LineTest.Y = Camera.GetPosition.Y
    LineTest.z = Camera.GetPosition.z + 2
    LineTest2.X = Camera.GetPosition.X + 2
    LineTest2.Y = Camera.GetPosition.Y
    LineTest2.z = Camera.GetPosition.z - 2
    If Shafts.Collision(LineTest, LineTest2, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = Camera.GetPosition.X - 2
    LineTest.Y = Camera.GetPosition.Y
    LineTest.z = Camera.GetPosition.z + 2
    LineTest2.X = Camera.GetPosition.X - 2
    LineTest2.Y = Camera.GetPosition.Y
    LineTest2.z = Camera.GetPosition.z - 2
    If Shafts.Collision(LineTest, LineTest2, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    
       
'Elevator1DoorL Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1DoorL.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1DoorL.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If Elevator1DoorL.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If Elevator1DoorL.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
        
'Elevator1DoorR Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1DoorR.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If Elevator1DoorR.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If Elevator1DoorR.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If Elevator1DoorR.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
        
'External Collision
    
    LineTest.X = lineend.X + 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If External.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X - 2
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z
    If External.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, Camera.GetPosition.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z + 2
    If External.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    LineTest.X = lineend.X
    LineTest.Y = lineend.Y
    LineTest.z = lineend.z - 2
    If External.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
            
    'If External.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
    '    Camera.SetPosition linestart.x, linestart.y, linestart.z
    '    GoTo CollisionEnd
    'Else
    '    Camera.SetPosition lineend.x, lineend.y, lineend.z
    'End If
    
    'If Mesh.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
    '    Camera.SetPosition linestart.x, linestart.y, linestart.z
    '    GoTo CollisionEnd
    'Else
    '    Camera.SetPosition lineend.x, lineend.y, lineend.z
    'End If
CollisionEnd:
        
    Atmos.Atmosphere_Render
    Scene.RenderAllMeshes
    TV.RenderToScreen
    DoEvents
  
End Sub

Sub StairsTimer_Timer()
 'Stairs Movement
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      DestroyObjects (CameraFloor)
      
      'Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
    If CameraFloor >= 2 Then CameraFloor = CameraFloor + 1
    If CameraFloor = 1 Then PartialFloor = PartialFloor + 25
    If CameraFloor = 1 And PartialFloor >= 25 Then PartialFloor = 0: CameraFloor = 2
      Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor = 137 Then Shafts.Enable True
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
    Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 2 + PartialFloor Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
   
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor2).Enable False
      ElevatorDoorR(CameraFloor2).Enable False
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      DestroyObjects (CameraFloor)
      
      'Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
    If CameraFloor = 1 Then PartialFloor = PartialFloor - 25
    If CameraFloor = 2 Then PartialFloor = 0: CameraFloor = 1
    If CameraFloor = 1 And PartialFloor <= -50 Then PartialFloor = -50
    If CameraFloor > 2 Then CameraFloor = CameraFloor - 1
     Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor2).Enable True
      ElevatorDoorR(CameraFloor2).Enable True
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor = 137 Then Shafts.Enable True
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
    Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.Y <> 10 Then
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 20 + PartialFloor Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 8 And Camera.GetPosition.X > -12.5 - 10 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 20 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 10 And Camera.GetPosition.X > -12.5 - 12 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 18 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 12 And Camera.GetPosition.X > -12.5 - 14 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 16 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 14 And Camera.GetPosition.X > -12.5 - 16 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 14 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.Y = 10 And PartialFloor = -50 Then PartialFloor = 0
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 And PartialFloor = 0 And Camera.GetPosition.Y = 10 Then PartialFloor = -50
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 2 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 8 And Camera.GetPosition.X > -12.5 - 10 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 4 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 10 And Camera.GetPosition.X > -12.5 - 12 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 6 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 12 And Camera.GetPosition.X > -12.5 - 14 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 8 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 14 And Camera.GetPosition.X > -12.5 - 16 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 10 + PartialFloor, Camera.GetPosition.z
    If Camera.GetPosition.X <= -12.5 - 16 And Camera.GetPosition.X > -12.5 - 20 And Camera.GetPosition.z > -46.25 + 7.71 And Camera.GetPosition.z < -30.85 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + 12 + PartialFloor, Camera.GetPosition.z
    
 End Sub

Private Sub Timer1_Timer()
OpenElevator = -1
Timer1.Enabled = False
End Sub
