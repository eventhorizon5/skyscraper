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

Public Number As Integer 'elevator number
Public ElevatorMesh As TVMesh 'elevator mesh object
Public FloorIndicator As TVMesh 'floor indicator object
Public ElevatorInsDoorL As TVMesh 'left inside door
Public ElevatorInsDoorR As TVMesh 'right inside door
Public Plaque As TVMesh 'plaque
Public CallButtonsUp As TVMesh 'up call button
Public CallButtonsDown As TVMesh 'down call button
Public UpQueue As String 'up call queue
Public DownQueue As String 'down call queue
Public QueuePositionDirection As Integer 'queue processing direction
Public PauseQueueSearch As Boolean 'pause queue processor
Public ElevatorSpeed As Single 'maximum elevator speed
Public ElevatorFloor As Integer 'current elevator floor
Public ElevatorSync As Boolean 'true if user should move with elevator
Public ElevatorMusic As TVSoundWave3D
Public ElevatorSounds As TVSoundWave3D
Public MoveElevator As Boolean 'Tells elevator to start going to specified floor
Public GotoFloor As Integer 'floor to go to
Public OpenDoor As Integer '1=open doors, -1=close doors
Public Acceleration As Single 'percentage of speed increase
Private ElevWait As Boolean

'Internal elevator simulation data
Private ElevatorStart As Single 'elevator vertical starting location
Private DoorsOpen As Boolean 'elevator door state
Private ElevatorDirection As Integer '-1=down, 1=up, 0=stopped
Private DistanceToTravel As Single 'distance in Y to travel
Private Destination As Single 'elevator destination Y value
Private ElevatorRate As Single
Private StoppingDistance As Single
Private CalculateStoppingDistance As Boolean
Private Brakes As Boolean

Option Explicit


Public Sub Initialize(Num As Integer)
Number = Num

'create object meshes
Set ElevatorMesh = Scene.CreateMeshBuilder("Elevator" + Str$(Number))
Set FloorIndicator = Scene.CreateMeshBuilder("FloorIndicator" + Str$(Number))
Set ElevatorInsDoorL = Scene.CreateMeshBuilder("ElevatorInsDoorL" + Str$(Number))
Set ElevatorInsDoorR = Scene.CreateMeshBuilder("ElevatorInsDoorR" + Str$(Number))
Set Plaque = Scene.CreateMeshBuilder("Plaque" + Str$(Number))
Set CallButtonsUp = Scene.CreateMeshBuilder("CallButtonsUp" + Str$(Number))
Set CallButtonsDown = Scene.CreateMeshBuilder("CallButtonsDown" + Str$(Number))
'Set ElevatorMusic = New TV3DMedia.TVSoundWave3D
Set ElevatorSounds = New TV3DMedia.TVSoundWave3D

End Sub

Public Sub CreateElevator(X As Single, Y As Single, FloorID As Integer, Direction As Integer)
'Creates elevator at specified location and floor
'x is the horiz position where the door is
'y is the northmost position

'Direction: 0=x is left side, 1=x is right side

Dim V As Integer
ElevatorMesh.SetPosition X, Floor(FloorID).FloorAltitude, Y
FloorIndicator.SetPosition X, Floor(FloorID).FloorAltitude, Y
Plaque.SetPosition X, Floor(FloorID).FloorAltitude, Y
ElevatorInsDoorL.SetPosition X, Floor(FloorID).FloorAltitude, Y
ElevatorInsDoorR.SetPosition X, Floor(FloorID).FloorAltitude, Y

If Direction = 0 Then V = 1
If Direction = 1 Then V = -1

    'Elevator
    ElevatorMesh.AddFloor GetTex("Wood2"), V * 15.5, 0, 0, -13.9, 0.1, 1, 1
    ElevatorMesh.AddFloor GetTex("Elev1"), V * 15.5, 0, 0, -13.9, 19.5, 2, 2
    ElevatorMesh.AddWall GetTex("Wood1"), V * 15.5, 0, 0, 0, 19.5, 0.1, 2, 2
    ElevatorMesh.AddWall GetTex("Wood1"), V * 15.5, -13.9, 0, -13.9, 19.5, 0.1, 2, 2
    ElevatorMesh.AddWall GetTex("Wood1"), V * 15.5, -13.9, V * 15.5, 0, 19.5, 0.1, 2, 2

    'Floor Indicator
    If Direction = 0 Then FloorIndicator.AddWall GetTex("ButtonL"), V * 0.16, -0.5, V * 0.16, -2.5, 1.5, 16, -1, 1
    If Direction = 1 Then FloorIndicator.AddWall GetTex("ButtonL"), V * 0.16, -13.9 + 0.5, V * 0.16, -13.9 + 2.5, 1.5, 16, -1, 1
    
    'Button Panel
    If Direction = 0 Then ElevatorMesh.AddWall GetTex("ElevExtPanels"), V * 0.16, -0.3, V * 0.16, -2.7, 7, 6, 1, 1
    If Direction = 1 Then ElevatorMesh.AddWall GetTex("ElevExtPanels"), V * 0.16, -13.9 + 0.3, V * 0.16, -13.9 + 2.7, 7, 6, 1, 1
    
    'Plaque
    If Direction = 0 Then Plaque.AddWall GetTex("Plaque"), V * 0.16, -0.3, V * 0.16, -2.7, 1, 13, -1, 1
    If Direction = 1 Then Plaque.AddWall GetTex("Plaque"), V * 0.16, -13.9 + 0.3, V * 0.16, -13.9 + 2.7, 1, 13, -1, 1
    Plaque.SetBlendingMode (TV_BLEND_ALPHA)
    Plaque.SetColor RGBA(1, 1, 1, 0.1)
    
    'Interior Panels
    ElevatorMesh.AddWall GetTex("Marble3"), V * 0.15, -14, V * 0.15, -11, 19.5, 0.1, 1, 1
    ElevatorMesh.AddWall GetTex("Marble3"), V * 0.15, 0, V * 0.15, -3, 19.5, 0.1, 1, 1
    
    'Interior Doors
    ElevatorInsDoorL.AddWall GetTex("ElevDoors"), V * 0.1, -10.95, V * 0.1, -7.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorR.AddWall GetTex("ElevDoors"), V * 0.1, -6.95, V * 0.1, -2.95, 19.5, 0.1, 1, 1
    
End Sub

Private Sub Class_Terminate()
'Set ElevatorMesh = Nothing
'Set FloorIndicator = Nothing
'Set ElevatorInsDoorL = Nothing
'Set ElevatorInsDoorR = Nothing
'Set Plaque = Nothing
'Set CallButtonsUp = Nothing
'Set CallButtonsDown = Nothing
End Sub

Public Sub AddRoute(FloorID As Integer, Direction As Integer)
'Add call route to elevator routing table, in sorted order

Dim Position As Long
Dim Position2 As Long
Dim TempString As String
Dim TempString2 As String

If Direction = 1 Then
    Position = 1
    If UpQueue = "" Then UpQueue = LTrim(Str$(FloorID)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, UpQueue, ",")
    If Position2 = 0 Then Position2 = Len(UpQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(UpQueue, Position, Position2 - 1)) = FloorID Then Exit Sub
    If Val(Mid$(UpQueue, Position, Position2 - 1)) < FloorID And Mid$(UpQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt
    If Val(Mid$(UpQueue, Position, Position2 - 1)) > FloorID Or Mid$(UpQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(UpQueue, Position - 1)
        TempString2 = Mid$(UpQueue, Position)
        'Insert new number between 2 strings
        UpQueue = TempString + LTrim(Str$(FloorID)) + "," + TempString2
        PauseQueueSearch = False
        Exit Sub
    End If
SkipIt:
    Loop
End If

If Direction = -1 Then
    Position = 1
    If DownQueue = "" Then DownQueue = LTrim(Str$(FloorID)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, DownQueue, ",")
    If Position2 = 0 Then Position2 = Len(DownQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(DownQueue, Position, Position2 - 1)) = FloorID Then Exit Sub
    If Val(Mid$(DownQueue, Position, Position2 - 1)) < FloorID And Mid$(DownQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt2
    If Val(Mid$(DownQueue, Position, Position2 - 1)) > FloorID Or Mid$(DownQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(DownQueue, Position - 1)
        TempString2 = Mid$(DownQueue, Position)
        'Insert new number between 2 strings
        DownQueue = TempString + LTrim(Str$(FloorID)) + "," + TempString2
        PauseQueueSearch = False
        Exit Sub
    End If
SkipIt2:
    Loop
End If

End Sub

Public Sub DeleteRoute(FloorID As Integer, Direction As Integer)
'Delete call route from elevator routing table

Dim Position As Long
Dim Position2 As Long

If Direction = 1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, UpQueue, LTrim(Str$(FloorID)) + ",")
    Position2 = InStr(Position, UpQueue, ",")
    'join 2 strings, eliminating old number
    UpQueue = Left$(UpQueue, Position - 1) + Mid$(UpQueue, Position2 + 1)
End If

If Direction = -1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, DownQueue, LTrim(Str$(FloorID)) + ",")
    Position2 = InStr(Position, DownQueue, ",")
    'join 2 strings, eliminating old number
    DownQueue = Left$(DownQueue, Position - 1) + Mid$(DownQueue, Position2 + 1)
End If

End Sub

Public Sub Alarm()
'code for the elevator alarm goes here

End Sub

Public Sub CallElevator(FloorID As Integer, Direction As Integer)
'Calls elevator from specified floor, and gives desired direction to travel

End Sub

Private Sub OpenDoors()
'Opens elevator doors
OpenDoor = 0

On Error GoTo ErrorHandler

Sim.WaitTimer(Number).Enabled = True

DoorsOpen = True

Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
End Sub

Private Sub CloseDoors()
'Closes elevator doors
OpenDoor = 0

On Error GoTo ErrorHandler



DoorsOpen = False

Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
End Sub

Private Sub MoveElevatorToFloor()
'Main processing routine; sends elevator to floor specified in GotoFloor
Static IsRunning As Boolean

'On Error GoTo ErrorHandler

If IsRunning = True Then GoTo LoopStart

IsRunning = True

'get elevator's current Y position
ElevatorStart = ElevatorMesh.GetPosition.Y

'get elevator's current floor
ElevatorFloor = GetElevatorFloor

'If elevator is already on specified floor, open doors and exit
If ElevatorFloor = GotoFloor Then Call OpenDoors: Exit Sub

'close doors if open
If DoorsOpen = True Then Call CloseDoors

'Determine direction
If GotoFloor < ElevatorFloor Then ElevatorDirection = -1
If GotoFloor > ElevatorFloor Then ElevatorDirection = 1

'Determine distance to destination floor
DistanceToTravel = Abs(Abs(Floor(GotoFloor).FloorAltitude) - Abs(ElevatorStart))
Destination = Floor(GotoFloor).FloorAltitude
CalculateStoppingDistance = True

ElevatorSync = True '*******Temporary

'If user is riding this elevator, then turn off floor
If ElevatorSync = True Then
    Floor(CameraFloor).DisableFloor
End If

'Turn off sky, buildings, and landscape
Atmos.SkyBox_Enable False
Buildings.Enable False
Landscape.Enable False

'Play starting sound
If ElevatorSounds.PlayState <> TV_PLAYSTATE_PLAYING Then
    ElevatorSounds.Loop_ = False
    ElevatorSounds.Load App.Path + "\data\elevstart.wav"
    ElevatorSounds.Play
End If

'Get first rate increment value
ElevatorRate = ElevatorDirection * (ElevatorSpeed * Acceleration)

LoopStart:

'Movement sound
If ElevatorSounds.PlayState = TV_PLAYSTATE_ENDED And Brakes = False Then
    ElevatorSounds.Load App.Path + "\data\elevmove.wav"
    ElevatorSounds.Loop_ = True
    ElevatorSounds.Play
End If

'move elevator objects and camera
ElevatorMesh.MoveRelative 0, ElevatorRate / ElevatorSpeed, 0
If ElevatorSync = True Then Camera.SetPosition Camera.GetPosition.X, ElevatorMesh.GetPosition.Y + CameraDefAltitude, Camera.GetPosition.z
ElevatorInsDoorL.MoveRelative 0, ElevatorRate / ElevatorSpeed, 0
ElevatorInsDoorR.MoveRelative 0, ElevatorRate / ElevatorSpeed, 0
FloorIndicator.MoveRelative 0, ElevatorRate / ElevatorSpeed, 0
Plaque.MoveRelative 0, ElevatorRate / ElevatorSpeed, 0
ElevatorSounds.SetPosition ElevatorMesh.GetPosition.X / SoundDivisor, ElevatorMesh.GetPosition.Y / SoundDivisor, ElevatorMesh.GetPosition.z / SoundDivisor
ElevatorSounds.maxDistance = SoundMaxDistance

'change speeds
If ElevatorDirection = 1 Then ElevatorRate = ElevatorRate + (ElevatorSpeed * Acceleration)
If ElevatorDirection = -1 Then ElevatorRate = ElevatorRate - (ElevatorSpeed * Acceleration)

If ElevatorDirection = 1 And ElevatorRate > ElevatorSpeed Then ElevatorRate = ElevatorSpeed
If ElevatorDirection = -1 And ElevatorRate < -ElevatorSpeed Then ElevatorRate = -ElevatorSpeed
If ElevatorDirection = 1 And Brakes = True And ElevatorRate > 0 Then ElevatorRate = 0
If ElevatorDirection = -1 And Brakes = True And ElevatorRate < 0 Then ElevatorRate = 0

'get distance needed to stop elevator
If CalculateStoppingDistance = True And ElevatorDirection = 1 Then StoppingDistance = ElevatorMesh.GetPosition.Y - ElevatorStart
If CalculateStoppingDistance = True And ElevatorDirection = -1 Then StoppingDistance = ElevatorStart - ElevatorMesh.GetPosition.Y

If Abs(ElevatorRate) = ElevatorSpeed Then CalculateStoppingDistance = False

If Brakes = False And ElevatorDirection = 1 And ElevatorMesh.GetPosition.Y >= Destination - StoppingDistance Then
    CalculateStoppingDistance = False
    'slow down elevator
    ElevatorDirection = -1
    Brakes = True
    If ElevatorSounds.PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds.Loop_ = False
        ElevatorSounds.Stop_
    End If
    If ElevatorSounds.PlayState <> TV_PLAYSTATE_PLAYING Then
        ElevatorSounds.Loop_ = False
        ElevatorSounds.Load App.Path + "\data\elevstop.wav"
        ElevatorSounds.Play
    End If
End If

If Brakes = False And ElevatorDirection = -1 And ElevatorMesh.GetPosition.Y <= Destination + StoppingDistance Then
    CalculateStoppingDistance = False
    'slow down elevator
    ElevatorDirection = 1
    Brakes = True
    If ElevatorSounds.PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds.Loop_ = False
        ElevatorSounds.Stop_
    End If
    If ElevatorSounds.PlayState <> TV_PLAYSTATE_PLAYING Then
        ElevatorSounds.Loop_ = False
        ElevatorSounds.Load App.Path + "\data\elevstop.wav"
        ElevatorSounds.Play
    End If
End If

If Abs(ElevatorRate) <> 0 Then Exit Sub

ElevatorRate = 0
ElevatorDirection = 0
Brakes = False
Destination = 0
DistanceToTravel = 0
ElevatorStart = 0
IsRunning = False
MoveElevator = False
Call GetElevatorFloor
Call OpenDoors

'Turn on floor
If ElevatorSync = True Then
    Call Floor(GotoFloor).EnableFloor
End If

'Turn on sky, buildings, and landscape
Atmos.SkyBox_Enable True
Buildings.Enable True
Landscape.Enable True

Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
End Sub

Public Sub StopElevator()
'Tells elevator to stop moving, no matter where it is

On Error GoTo ErrorHandler



Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
End Sub

Public Sub OpenHatch()
'Opens the elevator's upper escape hatch, allowing access to the shaft

End Sub

Public Sub OpenDoorsEmergency()
'Simulates manually prying doors open.
'Slowly opens the elevator doors no matter where elevator is.
'If lined up with shaft doors, then opens the shaft doors also
OpenDoor = 0

On Error GoTo ErrorHandler


DoorsOpen = True

Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
End Sub

Public Sub OpenShaftDoors(FloorID As Integer)
'Simulates manually opening shaft doors
'Slowly opens the shaft's elevator doors no matter where elevator is.
'Cannot be used with OpenDoorsEmergency.
'This is if the elevator is not lined up with the shaft doors,
'and the user needs to open the shaft doors, usually while on top of elevator.
OpenDoor = 0

End Sub

Public Sub ProcessCallQueue()
'Processes the elevator's call queue, and sends elevators to called floors

End Sub

Public Function GetElevatorFloor() As Integer
'Determine floor that the elevator is on
Dim i As Integer
For i = -Basements To TotalFloors
    DoEvents
    If i < TotalFloors Then
        If ElevatorMesh.GetPosition.Y >= Floor(i).FloorAltitude And ElevatorMesh.GetPosition.Y < Floor(i + 1).FloorAltitude Then GetElevatorFloor = i: Exit Function
    End If
    If i = TotalFloors And ElevatorMesh.GetPosition.Y >= Floor(i).FloorAltitude Then GetElevatorFloor = i
Next i

End Function

Public Sub MonitorLoop()
'Monitors elevator and starts actions if needed

If OpenDoor = 1 Then Call OpenDoors
If OpenDoor = -1 Then Call CloseDoors
If OpenDoor = 2 Then Call OpenDoorsEmergency

If MoveElevator = True Then Call MoveElevatorToFloor
End Sub
