VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ElevatorObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'Skyscraper 1.1 Alpha - Elevator Subsystem Class
'Copyright ©2004 Ryan Thoryk
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

Public Mesh As TVMesh
Public FloorIndicator As TVMesh
Public InsDoorL As TVMesh
Public InsDoorR As TVMesh
Public DoorL As TVMesh
Public DoorR As TVMesh
Public Plaque As TVMesh
Public Music As TVSoundWave3D
Public Sounds As TVSoundWave3D
Public Sync As Boolean
Private elevatorstart As D3DVECTOR
Public Enable As Single
Public Direction As Integer
Public OpenLoc As Single
Public Check As Integer
Public Check2 As Integer
Public Check3 As Integer
Public Check4 As Integer
Public GotoFloor As Single
Public CurrentFloor As Integer
Public CurrentFloorExact As Single
Public DistanceToTravel As Single
Public Destination As Single
Public OriginalLocation As Single
Public StoppingDistance As Single
Public FineTune As Boolean
Public OpenDoors As Integer
Public ElevatorFloor As Integer
Public FloorIndicatorText As String
Public FloorIndicatorPic As String
Public FloorIndicatorPicOld As String
Public ElevTemp As Single
Public QueuePositionDirection As Integer
Public PauseQueueSearch As Boolean
Public UpQueue As String
Public DownQueue As String
Public Number As Integer

Public Sub ElevatorLoop()
Dim FloorHeight As Single
Dim FloorAltitude As Single
FloorHeight = Floor(Number).GetHeight
FloorAltitude = Floor(Number).GetAltitude

On Error GoTo ErrorHandler
If DebugPanel.Check11.Value = 0 Then Exit Sub

Call ProcessCallQueue

If Enable = 0 And GotoFloor = 0 And OpenElevator = 0 And FineTune = False Then Exit Sub

elevatorstart = Mesh.GetPosition

'Find the floor that the elevator's on
ElevatorFloor = GetFloor
      
      If GotoFloor = ElevatorFloor - 1 Then CurrentFloor = ElevatorFloor

      If GotoFloor <> 0 And GotoFloor > CurrentFloor And Direction = 0 And DoorL.GetPosition.z <= 0 Then
      Direction = 1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = GetFloorAltitude(Int(GotoFloor)) - GetFloorAltitude(Int(CurrentFloorExact))
      If Sync = True Then
      Call DeleteStairDoors
      Room(CameraFloor).Enable False
      CrawlSpace(CameraFloor).Enable False
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable False
      For ElevTemp = 1 To 40
      CallButtonsUp(ElevTemp).Enable False
      CallButtonsDown(ElevTemp).Enable False
      Next ElevTemp
      For ElevTemp = 1 To 40
      DoorL(ElevTemp).Enable False
      DoorR(ElevTemp).Enable False
      Next ElevTemp
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Buildings.Enable False
      Landscape.Enable False
      If StairDataTable(CameraFloor) = True Then DeleteStairs (CameraFloor)
      If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = True Then DeleteStairs (CameraFloor + 1)
      If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = True Then DeleteStairs (CameraFloor - 1)
      End If
      End If
      If GotoFloor <> 0 And GotoFloor < CurrentFloor And Direction = 0 And DoorL.GetPosition.z <= 0 Then
      Mesh.Enable True
      Direction = -1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = GetFloorAltitude(Int(CurrentFloorExact)) - GetFloorAltitude(Int(GotoFloor))
      If Sync = True Then
      Call DeleteStairDoors
      Room(CameraFloor).Enable False
      CrawlSpace(CameraFloor).Enable False
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable False
      For ElevTemp = 1 To 40
      CallButtonsUp(ElevTemp).Enable False
      CallButtonsDown(ElevTemp).Enable False
      Next ElevTemp
      For ElevTemp = 1 To 40
      DoorL(ElevTemp).Enable False
      DoorR(ElevTemp).Enable False
      Next ElevTemp
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Buildings.Enable False
      Landscape.Enable False
      If StairDataTable(CameraFloor) = True Then DeleteStairs (CameraFloor)
      If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = True Then DeleteStairs (CameraFloor + 1)
      If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = True Then DeleteStairs (CameraFloor - 1)
      End If
      End If
      
       CurrentFloorExact = GetFloorExact(elevatorstart.Y)
       CurrentFloor = Int(CurrentFloorExact)
        
        If Enable >= 0 And Direction = 1 Then
        'sound
        If Sounds.PlayState <> TV_PLAYSTATE_PLAYING And Check = 0 And GotoFloor <> ElevatorFloor Then
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevstart.wav"
        Sounds.Play
        Check = 1
        End If
        If Sounds.PlayState = TV_PLAYSTATE_ENDED And Check = 1 Then
        Sounds.Load App.Path + "\data\elevmove.wav"
        Sounds.Loop_ = True
        Sounds.Play
        End If
        'movement
        Mesh.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorL.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorR.MoveRelative 0, (Enable / ElevatorSpeed), 0
        FloorIndicator.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Plaque.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, (Enable / ElevatorSpeed), 0
        Next ElevTemp
        If Sync = True Then Camera.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Enable = Enable + 0.25
        If Enable <= 15 Then StoppingDistance = CurrentFloorExact - OriginalLocation + 0.4
        If Enable > 15 Then Enable = 15
        Destination = GetFloorExact(OriginalLocation) + DistanceToTravel - 40
        If GotoFloor <> 0 And elevatorstart.Y >= Destination - GetFloorExact(StoppingDistance) Then Direction = -1: Check = 0
        End If
      
        If Enable > 0 And Direction = -1 Then
        'Sounds
        If Sounds.PlayState = TV_PLAYSTATE_PLAYING And Check2 = 0 And FineTune = False Then
        Sounds.Loop_ = False
        Sounds.Stop_
        End If
        If Sounds.PlayState <> TV_PLAYSTATE_PLAYING And Check2 = 0 And FineTune = False Then
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevstop.wav"
        Sounds.Play
        Check2 = 1
        End If
        'Movement
        Mesh.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorL.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorR.MoveRelative 0, (Enable / ElevatorSpeed), 0
        FloorIndicator.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Plaque.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, (Enable / ElevatorSpeed), 0
        Next ElevTemp
        If Sync = True Then Camera.MoveRelative 0, (Enable / ElevatorSpeed), 0
        
        Enable = Enable - 0.25
        If Enable < 0 Then Enable = 0
        If Enable = 0 Then Direction = 0
        If GotoFloor <> 0 Then Check = 0: FineTune = True
        End If
      
        If Enable <= 0 And Direction = -1 Then
        If Sounds.PlayState <> TV_PLAYSTATE_PLAYING And Check = 0 Then
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevstart.wav"
        Sounds.Play
        Check = 1
        End If
        If Sounds.PlayState = TV_PLAYSTATE_ENDED And Check = 1 Then
        Sounds.Load App.Path + "\data\elevmove.wav"
        Sounds.Loop_ = True
        Sounds.Play
        End If
        Mesh.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorL.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorR.MoveRelative 0, (Enable / ElevatorSpeed), 0
        FloorIndicator.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Plaque.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, (Enable / ElevatorSpeed), 0
        Next ElevTemp
        If Sync = True Then Camera.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Enable = Enable - 0.25
        If Enable >= -15 Then StoppingDistance = OriginalLocation - CurrentFloorExact
        If Enable < -15 Then Enable = -15
        Destination = GetFloorExact(OriginalLocation) - DistanceToTravel - 15
        If GotoFloor <> 0 And elevatorstart.Y <= Destination + GetFloorExact(StoppingDistance) Then Direction = 1: Check = 0
        End If
      
        If Enable < 0 And Direction = 1 Then
        If Sounds.PlayState = TV_PLAYSTATE_PLAYING And Check2 = 0 And FineTune = False Then
        Sounds.Loop_ = False
        Sounds.Stop_
        End If
        If Sounds.PlayState <> TV_PLAYSTATE_PLAYING And Check2 = 0 And FineTune = False Then
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevstop.wav"
        Sounds.Play
        Check2 = 1
        End If
        Mesh.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorL.MoveRelative 0, (Enable / ElevatorSpeed), 0
        InsDoorR.MoveRelative 0, (Enable / ElevatorSpeed), 0
        FloorIndicator.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Plaque.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, (Enable / ElevatorSpeed), 0
        Next ElevTemp
        If Sync = True Then Camera.MoveRelative 0, (Enable / ElevatorSpeed), 0
        Enable = Enable + 0.25
        If Enable > 0 Then Enable = 0
        If Enable = 0 Then Direction = 0
        If GotoFloor <> 0 Then Check = 0: FineTune = True
        End If
      
      If FineTune = True And Enable = 0 And elevatorstart.Y > GetFloorExact(GotoFloor) + -0.3 And elevatorstart.Y < (GotoFloor * FloorHeight) + FloorHeight + 0.3 Then
      FineTune = False
      Room(CameraFloor).Enable True
      CrawlSpace(CameraFloor).Enable True
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable True
      If Sync = True Then
      For ElevTemp = 1 To 40
      DoorL(ElevTemp).Enable True
      DoorR(ElevTemp).Enable True
      Next ElevTemp
      For ElevTemp = 1 To 40
      CallButtonsUp(ElevTemp).Enable True
      CallButtonsDown(ElevTemp).Enable True
      Next ElevTemp
      ShaftsFloor(CameraFloor).Enable True
      'If StairDataTable(CameraFloor) = False Then CreateStairs (CameraFloor)
      Atmos.SkyBox_Enable True
      Buildings.Enable True
      Landscape.Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      Call InitRealtime(CameraFloor)
      InitObjectsForFloor (CameraFloor)
      'If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = False Then CreateStairs (CameraFloor + 1)
      'If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = False Then CreateStairs (CameraFloor - 1)
      End If
      GotoFloor = 0
      OpenElevator = 1
      Check = 0
      Check2 = 0
      Check3 = 0
      Check4 = 0
      If CameraFloor > BottomFloor And Sync = True Then Camera.SetPosition Camera.GetPosition.X, GetFloorAltitude(CameraFloor) + 10, Camera.GetPosition.z
      End If
      
      If FineTune = True Then
      If Check3 = 0 Then
      Sounds.Load App.Path + "\data\ding1.wav"
      Sounds.Play
      Check3 = 1
        'For ElevTemp = -11 To 144
        'If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).SetColor RGBA(1, 1, 1, 1)
        'Next ElevTemp
        If ButtonsEnabled = True And Sync = True Then Buttons(GotoFloor).SetColor RGBA(1, 1, 1, 1)
      End If
      If elevatorstart.Y < GetFloorExact(GotoFloor) Then
      Mesh.MoveRelative 0, ElevatorFineTuneSpeed, 0
      InsDoorL.MoveRelative 0, ElevatorFineTuneSpeed, 0
      InsDoorR.MoveRelative 0, ElevatorFineTuneSpeed, 0
      Plaque.MoveRelative 0, ElevatorFineTuneSpeed, 0
      Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, ElevatorFineTuneSpeed, 0
        Next ElevTemp
        FloorIndicator.MoveRelative 0, ElevatorFineTuneSpeed, 0
        If Sync = True Then Camera.MoveRelative 0, ElevatorFineTuneSpeed, 0
      End If
      If elevatorstart.Y > GetFloorExact(GotoFloor) Then
      Mesh.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      InsDoorL.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      InsDoorR.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      Plaque.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        For ElevTemp = -11 To 144
        If ButtonsEnabled = True And Sync = True Then Buttons(ElevTemp).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        Next ElevTemp
        FloorIndicator.MoveRelative 0, -ElevatorFineTuneSpeed, 0
        If Sync = True Then Camera.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      End If
      End If
      
      If OpenElevator = 1 Then
      
      Dim jxx As Integer
      Dim jyy As Integer
      jxx = CameraFloor
      jyy = ElevatorFloor
      If jxx <> jyy Then OpenElevator = 0: Exit Sub
      
      If DoorL.GetPosition.z >= 4 Then OpenElevator = 0: GoTo OpenElevator1
      If Check4 = 0 Then
        If Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Sounds.Stop_
        End If
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevatoropen.wav"
        Sounds.SetPosition Mesh.GetPosition.X / SoundDivisor, Mesh.GetPosition.Y / SoundDivisor, Mesh.GetPosition.z / SoundDivisor
        Sounds.maxDistance = SoundMaxDistance
        Sounds.Play
        Check4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If DoorL.GetPosition.z > 1 Then OpenElevator = 2
OpenElevator1:
      End If
      
      If OpenElevator = 2 Then
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If DoorL.GetPosition.z > 3 Then OpenElevator = 3
      End If
      
      If OpenElevator = 3 Then
      Check4 = 0
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      If DoorL.GetPosition.z < 7 And OpenElevatorLoc = 0 Then OpenElevatorLoc = 0.02
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc <= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      If Number = 1 Then Sim.Timer1.Enabled = True
      If Number = 2 Then Sim.Timer2.Enabled = True
      If Number = 3 Then Sim.Timer3.Enabled = True
      If Number = 4 Then Sim.Timer4.Enabled = True
      If Number = 5 Then Sim.Timer5.Enabled = True
      If Number = 6 Then Sim.Timer6.Enabled = True
      If Number = 7 Then Sim.Timer7.Enabled = True
      If Number = 8 Then Sim.Timer8.Enabled = True
      If Number = 9 Then Sim.Timer9.Enabled = True
      If Number = 10 Then Sim.Timer10.Enabled = True
      If Number = 11 Then Sim.Timer11.Enabled = True
      If Number = 12 Then Sim.Timer12.Enabled = True
      If Number = 13 Then Sim.Timer13.Enabled = True
      If Number = 14 Then Sim.Timer14.Enabled = True
      If Number = 15 Then Sim.Timer15.Enabled = True
      If Number = 16 Then Sim.Timer16.Enabled = True
      If Number = 17 Then Sim.Timer17.Enabled = True
      If Number = 18 Then Sim.Timer18.Enabled = True
      If Number = 19 Then Sim.Timer19.Enabled = True
      If Number = 20 Then Sim.Timer20.Enabled = True
      If Number = 21 Then Sim.Timer21.Enabled = True
      If Number = 22 Then Sim.Timer22.Enabled = True
      If Number = 23 Then Sim.Timer23.Enabled = True
      If Number = 24 Then Sim.Timer24.Enabled = True
      If Number = 25 Then Sim.Timer25.Enabled = True
      If Number = 26 Then Sim.Timer26.Enabled = True
      If Number = 27 Then Sim.Timer27.Enabled = True
      If Number = 28 Then Sim.Timer28.Enabled = True
      If Number = 29 Then Sim.Timer29.Enabled = True
      If Number = 30 Then Sim.Timer30.Enabled = True
      If Number = 31 Then Sim.Timer31.Enabled = True
      If Number = 32 Then Sim.Timer32.Enabled = True
      If Number = 33 Then Sim.Timer33.Enabled = True
      If Number = 34 Then Sim.Timer34.Enabled = True
      If Number = 35 Then Sim.Timer35.Enabled = True
      If Number = 36 Then Sim.Timer36.Enabled = True
      If Number = 37 Then Sim.Timer37.Enabled = True
      If Number = 38 Then Sim.Timer38.Enabled = True
      If Number = 39 Then Sim.Timer39.Enabled = True
      If Number = 40 Then Sim.Timer40.Enabled = True
      End If
      End If
      
      If OpenElevator = -1 Then
      
      If GotoFloor = 0 Then PauseQueueSearch = False
      
      If DoorL.GetPosition.z <= 0 Then OpenElevator = 0: GoTo OpenElevator2
      If Check4 = 0 Then
        If Sounds.PlayState = TV_PLAYSTATE_PLAYING Then
        Sounds.Stop_
        End If
        Sounds.Loop_ = False
        Sounds.Load App.Path + "\data\elevatorclose.wav"
        Sounds.Play
        Check4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If DoorL.GetPosition.z < 3 Then OpenElevator = -2
OpenElevator2:
      End If
      
      If OpenElevator = -2 Then
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If DoorL.GetPosition.z < 1 Then OpenElevator = -3
      End If
      
      If OpenElevator = -3 Then
      Check4 = 0
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      If DoorL.GetPosition.z > 0 And OpenElevatorLoc >= 0 Then OpenElevatorLoc = -0.02
      DoorL.MoveRelative OpenElevatorLoc, 0, 0
      DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      InsDoorL.MoveRelative OpenElevatorLoc, 0, 0
      InsDoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc >= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      InsDoorL.SetPosition InsDoorL.GetPosition.X, InsDoorL.GetPosition.Y, 0
      InsDoorR.SetPosition InsDoorR.GetPosition.X, InsDoorR.GetPosition.Y, 0
      DoorL.SetPosition DoorL.GetPosition.X, DoorL.GetPosition.Y, 0
      DoorR.SetPosition DoorR.GetPosition.X, DoorR.GetPosition.Y, 0
      End If
      End If
Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End

End Sub

Public Sub AddRoute(Floor As Integer, Direction As Integer)
'Add call route to elevator routing table, in sorted order

Dim Position As Long
Dim Position2 As Long
Dim TempString As String
Dim TempString2 As String

If Direction = 1 Then
    Position = 1
    If UpQueue = "" Then UpQueue = LTrim(Str$(Floor)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, UpQueue, ",")
    If Position2 = 0 Then Position2 = Len(UpQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(UpQueue, Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(UpQueue, Position, Position2 - 1)) < Floor And Mid$(UpQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt
    If Val(Mid$(UpQueue, Position, Position2 - 1)) > Floor Or Mid$(UpQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(UpQueue, Position - 1)
        TempString2 = Mid$(UpQueue, Position)
        'Insert new number between 2 strings
        UpQueue = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch = False
        Exit Sub
    End If
SkipIt:
    Loop
End If

If Direction = -1 Then
    Position = 1
    If DownQueue = "" Then DownQueue = LTrim(Str$(Floor)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, DownQueue, ",")
    If Position2 = 0 Then Position2 = Len(DownQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(DownQueue, Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(DownQueue, Position, Position2 - 1)) < Floor And Mid$(DownQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt2
    If Val(Mid$(DownQueue, Position, Position2 - 1)) > Floor Or Mid$(DownQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(DownQueue, Position - 1)
        TempString2 = Mid$(DownQueue, Position)
        'Insert new number between 2 strings
        DownQueue = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch = False
        Exit Sub
    End If
SkipIt2:
    Loop
End If

End Sub

Public Sub Alarm()

End Sub


Public Sub DeleteRoute(Floor As Integer, Direction As Integer)
'Delete call route from elevator routing table

Dim Position As Long
Dim Position2 As Long

If Direction = 1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, UpQueue, LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, UpQueue, ",")
    'join 2 strings, eliminating old number
    UpQueue = Left$(UpQueue, Position - 1) + Mid$(UpQueue, Position2 + 1)
End If

If Direction = -1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, DownQueue, LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, DownQueue, ",")
    'join 2 strings, eliminating old number
    DownQueue = Left$(DownQueue, Position - 1) + Mid$(DownQueue, Position2 + 1)
End If

End Sub

Public Sub DrawElevatorButtons()
ButtonsEnabled = True

'New button handling code
For i54 = -11 To 144
Buttons(i54).ResetMesh
Buttons(i54).SetPosition 0, 0, 0
Buttons(i54).SetRotation 0, 0, 0
Next i54

'General Buttons
Call ElevatorButton(139, "Open", Number, 56)
Call ElevatorButton(140, "Close", Number, 57)
Call ElevatorButton(141, "Cancel", Number, 58)
Call ElevatorButton(142, "Stop", Number, 59)
Call ElevatorButton(143, "Alarm", Number, 60)
    
'Elevator 1
If Number = 1 Then

    Call ElevatorButton(134, "134", Number, 6)
    Call ElevatorButton(135, "135", Number, 7)
    Call ElevatorButton(136, "136", Number, 8)
    Call ElevatorButton(137, "137", Number, 9)
    Call ElevatorButton(138, "R", Number, 10)
    
    Call ElevatorButton(129, "129", Number, 11)
    Call ElevatorButton(130, "130", Number, 12)
    Call ElevatorButton(131, "131", Number, 13)
    Call ElevatorButton(132, "132", Number, 14)
    Call ElevatorButton(133, "133", Number, 15)
    
    Call ElevatorButton(124, "124", Number, 16)
    Call ElevatorButton(125, "125", Number, 17)
    Call ElevatorButton(126, "126", Number, 18)
    Call ElevatorButton(127, "127", Number, 19)
    Call ElevatorButton(128, "128", Number, 20)
    
    Call ElevatorButton(119, "119", Number, 21)
    Call ElevatorButton(120, "120", Number, 22)
    Call ElevatorButton(121, "121", Number, 23)
    Call ElevatorButton(122, "122", Number, 24)
    Call ElevatorButton(123, "123", Number, 25)
    
    Call ElevatorButton(114, "114", Number, 26)
    Call ElevatorButton(115, "115", Number, 27)
    Call ElevatorButton(116, "116", Number, 28)
    Call ElevatorButton(117, "117", Number, 29)
    Call ElevatorButton(118, "118", Number, 30)
    
    Call ElevatorButton(109, "109", Number, 31)
    Call ElevatorButton(110, "110", Number, 32)
    Call ElevatorButton(111, "111", Number, 33)
    Call ElevatorButton(112, "112", Number, 34)
    Call ElevatorButton(113, "113", Number, 35)
    
    Call ElevatorButton(104, "104", Number, 36)
    Call ElevatorButton(105, "105", Number, 37)
    Call ElevatorButton(106, "106", Number, 38)
    Call ElevatorButton(107, "107", Number, 39)
    Call ElevatorButton(108, "108", Number, 40)
    
    Call ElevatorButton(79, "79", Number, 41)
    Call ElevatorButton(80, "80", Number, 42)
    Call ElevatorButton(101, "101", Number, 43)
    Call ElevatorButton(102, "102", Number, 44)
    Call ElevatorButton(103, "103", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 46)
    Call ElevatorButton(1, "M", Number, 47)
    Call ElevatorButton(2, "2", Number, 48)
    Call ElevatorButton(39, "39", Number, 49)
    Call ElevatorButton(40, "40", Number, 50)
    
End If

'Elevator 2
If Number >= 2 And Number <= 4 Then

    Call ElevatorButton(136, "136", Number, 13)
    Call ElevatorButton(135, "135", Number, 18)
    Call ElevatorButton(134, "134", Number, 23)
    Call ElevatorButton(132, "132", Number, 28)
    Call ElevatorButton(80, "80", Number, 33)
    Call ElevatorButton(1, "M", Number, 38)
    Call ElevatorButton(0, "L", Number, 43)

End If

If Number >= 5 And Number <= 10 Then

    Call ElevatorButton(127, "127", Number, 7)
    Call ElevatorButton(128, "128", Number, 8)
    Call ElevatorButton(129, "129", Number, 9)
    
    Call ElevatorButton(124, "124", Number, 12)
    Call ElevatorButton(125, "125", Number, 13)
    Call ElevatorButton(126, "126", Number, 14)
    
    Call ElevatorButton(121, "121", Number, 17)
    Call ElevatorButton(122, "122", Number, 18)
    Call ElevatorButton(123, "123", Number, 19)
    
    Call ElevatorButton(118, "118", Number, 22)
    Call ElevatorButton(119, "119", Number, 23)
    Call ElevatorButton(120, "120", Number, 24)
    
    Call ElevatorButton(112, "112", Number, 27)
    Call ElevatorButton(113, "113", Number, 28)
    Call ElevatorButton(114, "114", Number, 29)
    
    Call ElevatorButton(109, "109", Number, 32)
    Call ElevatorButton(110, "110", Number, 33)
    Call ElevatorButton(111, "111", Number, 34)
    
    Call ElevatorButton(106, "106", Number, 37)
    Call ElevatorButton(107, "107", Number, 38)
    Call ElevatorButton(108, "108", Number, 39)
    
    Call ElevatorButton(103, "103", Number, 42)
    Call ElevatorButton(104, "104", Number, 43)
    Call ElevatorButton(105, "105", Number, 44)
    
    Call ElevatorButton(100, "100", Number, 47)
    Call ElevatorButton(101, "101", Number, 48)
    Call ElevatorButton(102, "102", Number, 49)
    
    Call ElevatorButton(80, "80", Number, 53)
    
End If

    
If Number = 11 Then

    Call ElevatorButton(51, "51", Number, 1)
    Call ElevatorButton(80, "80", Number, 2)
    Call ElevatorButton(115, "115", Number, 3)
    Call ElevatorButton(116, "116", Number, 4)
    
    Call ElevatorButton(46, "46", Number, 6)
    Call ElevatorButton(47, "47", Number, 7)
    Call ElevatorButton(48, "48", Number, 8)
    Call ElevatorButton(49, "49", Number, 9)
    Call ElevatorButton(50, "50", Number, 10)
    
    Call ElevatorButton(41, "41", Number, 11)
    Call ElevatorButton(42, "42", Number, 12)
    Call ElevatorButton(43, "43", Number, 13)
    Call ElevatorButton(44, "44", Number, 14)
    Call ElevatorButton(45, "45", Number, 15)
    
    Call ElevatorButton(36, "36", Number, 16)
    Call ElevatorButton(37, "37", Number, 17)
    Call ElevatorButton(38, "38", Number, 18)
    Call ElevatorButton(39, "39", Number, 19)
    Call ElevatorButton(40, "40", Number, 20)
    
    Call ElevatorButton(31, "31", Number, 21)
    Call ElevatorButton(32, "32", Number, 22)
    Call ElevatorButton(33, "33", Number, 23)
    Call ElevatorButton(34, "34", Number, 24)
    Call ElevatorButton(35, "35", Number, 25)
    
    Call ElevatorButton(26, "26", Number, 26)
    Call ElevatorButton(27, "27", Number, 27)
    Call ElevatorButton(28, "28", Number, 28)
    Call ElevatorButton(29, "29", Number, 29)
    Call ElevatorButton(30, "30", Number, 30)
    
    Call ElevatorButton(21, "21", Number, 31)
    Call ElevatorButton(22, "22", Number, 32)
    Call ElevatorButton(23, "23", Number, 33)
    Call ElevatorButton(24, "24", Number, 34)
    Call ElevatorButton(25, "25", Number, 35)
    
    Call ElevatorButton(16, "16", Number, 36)
    Call ElevatorButton(17, "17", Number, 37)
    Call ElevatorButton(18, "18", Number, 38)
    Call ElevatorButton(19, "19", Number, 39)
    Call ElevatorButton(20, "20", Number, 40)
    
    Call ElevatorButton(11, "11", Number, 41)
    Call ElevatorButton(12, "12", Number, 42)
    Call ElevatorButton(13, "13", Number, 43)
    Call ElevatorButton(14, "14", Number, 44)
    Call ElevatorButton(15, "15", Number, 45)
    
    Call ElevatorButton(6, "6", Number, 46)
    Call ElevatorButton(7, "7", Number, 47)
    Call ElevatorButton(8, "8", Number, 48)
    Call ElevatorButton(9, "9", Number, 49)
    Call ElevatorButton(10, "10", Number, 50)
    
    Call ElevatorButton(0, "L", Number, 51)
    Call ElevatorButton(2, "2", Number, 52)
    Call ElevatorButton(3, "3", Number, 53)
    Call ElevatorButton(4, "4", Number, 54)
    Call ElevatorButton(5, "5", Number, 55)
    
End If

If Number = 12 Then

    Call ElevatorButton(100, "100", Number, 1)
    Call ElevatorButton(101, "101", Number, 2)
    Call ElevatorButton(115, "115", Number, 3)
    Call ElevatorButton(116, "116", Number, 4)
    
    Call ElevatorButton(95, "95", Number, 6)
    Call ElevatorButton(96, "96", Number, 7)
    Call ElevatorButton(97, "97", Number, 8)
    Call ElevatorButton(98, "98", Number, 9)
    Call ElevatorButton(99, "99", Number, 10)
    
    Call ElevatorButton(90, "90", Number, 11)
    Call ElevatorButton(91, "91", Number, 12)
    Call ElevatorButton(92, "92", Number, 13)
    Call ElevatorButton(93, "93", Number, 14)
    Call ElevatorButton(94, "94", Number, 15)
    
    Call ElevatorButton(85, "85", Number, 16)
    Call ElevatorButton(86, "86", Number, 17)
    Call ElevatorButton(87, "87", Number, 18)
    Call ElevatorButton(88, "88", Number, 19)
    Call ElevatorButton(89, "89", Number, 20)
    
    Call ElevatorButton(80, "80", Number, 21)
    Call ElevatorButton(81, "81", Number, 22)
    Call ElevatorButton(82, "82", Number, 23)
    Call ElevatorButton(83, "83", Number, 24)
    Call ElevatorButton(84, "84", Number, 25)
    
    Call ElevatorButton(75, "75", Number, 26)
    Call ElevatorButton(76, "76", Number, 27)
    Call ElevatorButton(77, "77", Number, 28)
    Call ElevatorButton(78, "78", Number, 29)
    Call ElevatorButton(79, "79", Number, 30)
    
    Call ElevatorButton(70, "70", Number, 31)
    Call ElevatorButton(71, "71", Number, 32)
    Call ElevatorButton(72, "72", Number, 33)
    Call ElevatorButton(73, "73", Number, 34)
    Call ElevatorButton(74, "74", Number, 35)
    
    Call ElevatorButton(65, "65", Number, 36)
    Call ElevatorButton(66, "66", Number, 37)
    Call ElevatorButton(67, "67", Number, 38)
    Call ElevatorButton(68, "68", Number, 39)
    Call ElevatorButton(69, "69", Number, 40)
    
    Call ElevatorButton(60, "60", Number, 41)
    Call ElevatorButton(61, "61", Number, 42)
    Call ElevatorButton(62, "62", Number, 43)
    Call ElevatorButton(63, "63", Number, 44)
    Call ElevatorButton(64, "64", Number, 45)
    
    Call ElevatorButton(55, "55", Number, 46)
    Call ElevatorButton(56, "56", Number, 47)
    Call ElevatorButton(57, "57", Number, 48)
    Call ElevatorButton(58, "58", Number, 49)
    Call ElevatorButton(59, "59", Number, 50)
    
    Call ElevatorButton(0, "L", Number, 51)
    Call ElevatorButton(51, "51", Number, 52)
    Call ElevatorButton(52, "52", Number, 53)
    Call ElevatorButton(53, "53", Number, 54)
    Call ElevatorButton(54, "54", Number, 55)
    
End If

If Number = 13 Or Number = 14 Then

    Call ElevatorButton(80, "80", Number, 28)
    Call ElevatorButton(0, "L", Number, 33)
    
End If

If Number >= 15 And Number <= 20 Then

    Call ElevatorButton(98, "98", Number, 7)
    Call ElevatorButton(99, "99", Number, 9)
    
    Call ElevatorButton(96, "96", Number, 12)
    Call ElevatorButton(97, "97", Number, 14)
    
    Call ElevatorButton(94, "94", Number, 17)
    Call ElevatorButton(95, "95", Number, 19)
    
    Call ElevatorButton(92, "92", Number, 22)
    Call ElevatorButton(93, "93", Number, 24)
    
    Call ElevatorButton(90, "90", Number, 27)
    Call ElevatorButton(91, "91", Number, 29)
    
    Call ElevatorButton(88, "88", Number, 32)
    Call ElevatorButton(89, "89", Number, 34)
    
    Call ElevatorButton(86, "86", Number, 37)
    Call ElevatorButton(87, "87", Number, 39)
    
    Call ElevatorButton(84, "84", Number, 42)
    Call ElevatorButton(85, "85", Number, 44)
    
    Call ElevatorButton(82, "82", Number, 47)
    Call ElevatorButton(83, "83", Number, 49)
    
    Call ElevatorButton(80, "80", Number, 52)
    Call ElevatorButton(81, "81", Number, 54)
    
End If
    
If Number >= 21 And Number <= 30 Then

    Call ElevatorButton(75, "75", Number, 6)
    Call ElevatorButton(76, "76", Number, 7)
    Call ElevatorButton(77, "77", Number, 8)
    Call ElevatorButton(78, "78", Number, 9)
    Call ElevatorButton(79, "79", Number, 10)
    
    Call ElevatorButton(70, "70", Number, 11)
    Call ElevatorButton(71, "71", Number, 12)
    Call ElevatorButton(72, "72", Number, 13)
    Call ElevatorButton(73, "73", Number, 14)
    Call ElevatorButton(74, "74", Number, 15)
    
    Call ElevatorButton(65, "65", Number, 16)
    Call ElevatorButton(66, "66", Number, 17)
    Call ElevatorButton(67, "67", Number, 18)
    Call ElevatorButton(68, "68", Number, 19)
    Call ElevatorButton(69, "69", Number, 20)
    
    Call ElevatorButton(60, "60", Number, 21)
    Call ElevatorButton(61, "61", Number, 22)
    Call ElevatorButton(62, "62", Number, 23)
    Call ElevatorButton(63, "63", Number, 24)
    Call ElevatorButton(64, "64", Number, 25)
    
    Call ElevatorButton(55, "55", Number, 26)
    Call ElevatorButton(56, "56", Number, 27)
    Call ElevatorButton(57, "57", Number, 28)
    Call ElevatorButton(58, "58", Number, 29)
    Call ElevatorButton(59, "59", Number, 30)
    
    Call ElevatorButton(50, "50", Number, 31)
    Call ElevatorButton(51, "51", Number, 32)
    Call ElevatorButton(52, "52", Number, 33)
    Call ElevatorButton(53, "53", Number, 34)
    Call ElevatorButton(54, "54", Number, 35)
    
    Call ElevatorButton(45, "45", Number, 36)
    Call ElevatorButton(46, "46", Number, 37)
    Call ElevatorButton(47, "47", Number, 38)
    Call ElevatorButton(48, "48", Number, 39)
    Call ElevatorButton(49, "49", Number, 40)
    
    Call ElevatorButton(40, "40", Number, 41)
    Call ElevatorButton(41, "41", Number, 42)
    Call ElevatorButton(42, "42", Number, 43)
    Call ElevatorButton(43, "43", Number, 44)
    Call ElevatorButton(44, "44", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 48)
    
End If

If Number >= 31 And Number <= 40 Then

    Call ElevatorButton(37, "37", Number, 7)
    Call ElevatorButton(38, "38", Number, 8)
    Call ElevatorButton(39, "39", Number, 9)
    
    Call ElevatorButton(32, "32", Number, 11)
    Call ElevatorButton(33, "33", Number, 12)
    Call ElevatorButton(34, "34", Number, 13)
    Call ElevatorButton(35, "35", Number, 14)
    Call ElevatorButton(36, "36", Number, 15)
    
    Call ElevatorButton(27, "27", Number, 16)
    Call ElevatorButton(28, "28", Number, 17)
    Call ElevatorButton(29, "29", Number, 18)
    Call ElevatorButton(30, "30", Number, 19)
    Call ElevatorButton(31, "31", Number, 20)
    
    Call ElevatorButton(22, "22", Number, 21)
    Call ElevatorButton(23, "23", Number, 22)
    Call ElevatorButton(24, "24", Number, 23)
    Call ElevatorButton(25, "25", Number, 24)
    Call ElevatorButton(26, "26", Number, 25)
    
    Call ElevatorButton(17, "17", Number, 26)
    Call ElevatorButton(18, "18", Number, 27)
    Call ElevatorButton(19, "19", Number, 28)
    Call ElevatorButton(20, "20", Number, 29)
    Call ElevatorButton(21, "21", Number, 30)
    
    Call ElevatorButton(12, "12", Number, 31)
    Call ElevatorButton(13, "13", Number, 32)
    Call ElevatorButton(14, "14", Number, 33)
    Call ElevatorButton(15, "15", Number, 34)
    Call ElevatorButton(16, "16", Number, 35)
    
    Call ElevatorButton(7, "7", Number, 36)
    Call ElevatorButton(8, "8", Number, 37)
    Call ElevatorButton(9, "9", Number, 38)
    Call ElevatorButton(10, "10", Number, 39)
    Call ElevatorButton(11, "11", Number, 40)
    
    Call ElevatorButton(2, "2", Number, 41)
    Call ElevatorButton(3, "3", Number, 42)
    Call ElevatorButton(4, "4", Number, 43)
    Call ElevatorButton(5, "5", Number, 44)
    Call ElevatorButton(6, "6", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 48)
    
End If
End Sub

Public Sub ElevatorButton(FloorID As Integer, ButtonLabel As String, ButtonIndex As Integer)
    
    'This code segment places all of the actual elevator button processing code into a single algorithm
    'FloorID - Exact floor that button calls
    'ButtonLabel - Simplified label for button
    'ButtonLabel2 - Texture name of button derived from ButtonLabel
    'ButtonIndex - Position of button on table below
    'ShaftLeft/Right - the left and right x coordinate boundaries of the elevator's shaft
    'Side - -1 if elevator is on left side (elevator number is even), and 1 if it is on the right
    'Side2 - -1 if elevator is in an odd-numbered shaft, and 1 if the shaft is even
    'ElevIndex - Elevator's group number (0-4); 1 or 2=group0, 3 or 4=group1, etc
    'Offset - Initial Z offset of buttons
    'IndexH - Horizontal index of button
    'IndexV - Vertical index of button
    
    'ButtonIndex values (based on visual look of panel)
    '1  2  3  4  5    0
    '6  7  8  9  10   1
    '11 12 13 14 15   2
    '16 17 18 19 20   3
    '21 22 23 24 25   4
    '26 27 28 29 30   5
    '31 32 33 34 35   6
    '36 37 38 39 40   7
    '41 42 43 44 45   8
    '46 47 48 49 50   9
    '51 52 53 54 55   10
    '56 57 58 59 60   11
                      'V
  'H 0  1  2  3  4
    'So button 54 would have an IndexH of 3 and an IndexV of 10, while button 6 would have an IndexH of 0 and an IndexV of 1
    
    Dim ButtonLabel2 As String
    Dim ShaftLeft As Single
    Dim Side As Integer
    Dim Side2 As Integer
    Dim ElevIndex As Integer
    Dim Offset As Single
    Dim IndexH As Integer
    Dim IndexV As Integer
    
    ButtonLabel = LCase(ButtonLabel)
    
    If IsNumeric(ButtonLabel) Then ButtonLabel2 = "Button" + ButtonLabel
    If ButtonLabel = "l" Then ButtonLabel2 = "ButtonL"
    If ButtonLabel = "m" Then ButtonLabel2 = "ButtonM"
    If ButtonLabel = "r" Then ButtonLabel2 = "ButtonR"
    If ButtonLabel = "open" Then ButtonLabel2 = "ButtonOpen"
    If ButtonLabel = "close" Then ButtonLabel2 = "ButtonClose"
    If ButtonLabel = "stop" Then ButtonLabel2 = "ButtonStop"
    If ButtonLabel = "alarm" Then ButtonLabel2 = "ButtonAlarm"
    If ButtonLabel = "cancel" Then ButtonLabel2 = "ButtonCancel"
    
    ElevIndex = Number
    If IsEven(ElevIndex) = True Then ElevIndex = ElevIndex - 1
    If Number <= 10 Then ShaftLeft = 12.5: Side2 = 1: ElevIndex = Int(ElevIndex / 2)
    If Number > 10 And Number <= 20 Then ShaftLeft = 52.5: Side2 = -1: ElevIndex = Int((ElevIndex - 10) / 2)
    If Number > 20 And Number <= 30 Then ShaftLeft = 90.5: Side2 = 1: ElevIndex = Int((ElevIndex - 20) / 2)
    If Number > 30 And Number <= 40 Then ShaftLeft = 130.5: Side2 = -1: ElevIndex = Int((ElevIndex - 30) / 2)
    
    If IsEven(Number) = False Then
        Side = -1
        If Side2 = 1 Then Offset = -27.85
        If Side2 = -1 Then Offset = -18.45
    Else
        Side = 1
        If Side2 = -1 Then Offset = -27.85
        If Side2 = 1 Then Offset = -18.45
    End If
            
    If ButtonIndex <= 5 Then IndexH = ButtonIndex - 1: IndexV = 0
    If ButtonIndex > 5 And ButtonIndex <= 10 Then IndexH = ButtonIndex - 6: IndexV = 1
    If ButtonIndex > 10 And ButtonIndex <= 15 Then IndexH = ButtonIndex - 11: IndexV = 2
    If ButtonIndex > 15 And ButtonIndex <= 20 Then IndexH = ButtonIndex - 16: IndexV = 3
    If ButtonIndex > 20 And ButtonIndex <= 25 Then IndexH = ButtonIndex - 21: IndexV = 4
    If ButtonIndex > 25 And ButtonIndex <= 30 Then IndexH = ButtonIndex - 26: IndexV = 5
    If ButtonIndex > 30 And ButtonIndex <= 35 Then IndexH = ButtonIndex - 31: IndexV = 6
    If ButtonIndex > 35 And ButtonIndex <= 40 Then IndexH = ButtonIndex - 36: IndexV = 7
    If ButtonIndex > 40 And ButtonIndex <= 45 Then IndexH = ButtonIndex - 41: IndexV = 8
    If ButtonIndex > 45 And ButtonIndex <= 50 Then IndexH = ButtonIndex - 46: IndexV = 9
    If ButtonIndex > 50 And ButtonIndex <= 55 Then IndexH = ButtonIndex - 51: IndexV = 10
    If ButtonIndex > 55 And ButtonIndex <= 60 Then IndexH = ButtonIndex - 56: IndexV = 11
    
    Buttons(FloorID).AddWall GetTex(ButtonLabel2), Side * (ShaftLeft + (0.17 * Side2)), Offset + (15 * ElevIndex) + (0.4 * IndexH * Side * Side2), Side * (ShaftLeft + (0.17 * Side2)), (Offset + 0.3) + (15 * ElevIndex) + (0.4 * IndexH * Side * Side2), 0.3, (Elevator(Number).GetPosition.Y + 12) - (0.5 * IndexV), Side * Side2, 1
    
End Sub

Sub ProcessCallQueue()
'new calling queue - old one was slow and inefficient

Dim Position As Long
Dim Position2 As Long
Dim Floor As Integer

If PauseQueueSearch(Number) = True Then Exit Sub

If QueuePositionDirection(Number) = 0 Then
    If UpQueue(Number) <> "" Then QueuePositionDirection(Number) = 1
    If DownQueue(Number) <> "" Then QueuePositionDirection(Number) = -1
End If
If UpQueue(Number) = "" And DownQueue(Number) = "" Then
    QueuePositionDirection(Number) = 0
    PauseQueueSearch(Number) = True
    Exit Sub
End If
If QueuePositionDirection(Number) = 1 And UpQueue(Number) = "" Then QueuePositionDirection(Number) = 0
If QueuePositionDirection(Number) = -1 And DownQueue(Number) = "" Then QueuePositionDirection(Number) = 0

If QueuePositionDirection(Number) = 1 Then
    Position = 1
    Do
    Position2 = InStr(Position, UpQueue(Number), ",")
    'Search through queue list and find next valid floor call (direction-wise)
    If Val(Mid$(UpQueue(Number), Position, Position2 - 1)) < Floor Then
        Position = Position2 + 1
        Exit Sub
    Else
        PauseQueueSearch(Number) = True
        Floor = Val(Mid$(UpQueue(Number), Position, Position2 - 1))
        Call DeleteRoute(Floor, Number, 1)
        OpenElevator(Number) = -1
        If InElevator = True Then Sync(Number) = True
        If Floor <> 0 Then GotoFloor(Number) = Floor
        If Floor = 0 Then GotoFloor(Number) = 0.001
        Exit Sub
    End If
    Loop
End If

If QueuePositionDirection(Number) = -1 Then
    Position = 1
    Do
    Position2 = InStr(Position, DownQueue(Number), ",")
    'Search through queue list and find next valid floor call (direction-wise)
    If Val(Mid$(DownQueue(Number), Position, Position2 - 1)) < Floor Then
        Position = Position2 + 1
        Exit Sub
    Else
        PauseQueueSearch(Number) = True
        Floor = Val(Mid$(DownQueue(Number), Position, Position2 - 1))
        Call DeleteRoute(Floor, Number, -1)
        OpenElevator(Number) = -1
        If InElevator = True Then Sync(Number) = True
        If Floor <> 0 Then GotoFloor(Number) = Floor
        If Floor = 0 Then GotoFloor(Number) = 0.001
        Exit Sub
    End If
    Loop
End If

End Sub

Public Sub StopElevator()
'If GotoFloor(ElevatorNumber) < ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
    'Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    'GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) - 1
    'FineTune(ElevatorNumber) = True
'End If
'If GotoFloor(ElevatorNumber) > ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
    'Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    'GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) + 1
    'FineTune(ElevatorNumber) = True
'End If

Direction(Number) = -Direction(Number)

End Sub

Public Function GetFloor() As Integer
'Determine what floor the specified elevator is on
Dim curfloor As Integer

'Find the upper and lower bounds of each floor, then see if the
'elevator altitude is within the bounds
For curfloor = BottomFloor To TopFloor
DoEvents
If Mesh(Number).GetPosition.Y >= GetFloorAltitude(curfloor) And Mesh(Number).GetPosition.Y < GetFloorAltitude(curfloor) + GetFloorHeight(curfloor) Then Exit For
Next curfloor

GetFloor = curfloor

End Function

Public Sub Init(Num As Integer)
'Set elevator's object number
Number = Num

'Create and name new meshes
Set Mesh = Scene.CreateMeshBuilder("Elevator" + Str$(Number))
Set FloorIndicator = Scene.CreateMeshBuilder("FloorIndicator" + Str$(Number))
Set InsDoorL = Scene.CreateMeshBuilder("InsDoorL" + Str$(Number))
Set InsDoorR = Scene.CreateMeshBuilder("InsDoorR" + Str$(Number))
Set Plaque = Scene.CreateMeshBuilder("Plaque" + Str$(Number))
Set CallButtonsUp = Scene.CreateMeshBuilder("CallButtonsUp" + Str$(Number))
Set CallButtonsDown = Scene.CreateMeshBuilder("CallButtonsDown" + Str$(Number))

'Init sound objects
'Set Music = New TV3DMedia.TVSoundWave3D
Set Sounds = New TV3DMedia.TVSoundWave3D
End Sub
