Attribute VB_Name = "Init"
'Skyscraper 1.1 Alpha - Main initialization code
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

'2/15/03 - Code amount - 79 pages
'3/1/03 - Code amount - 129 pages
'4/26/03 - Code amount - 280 pages
'10/6/03 - Code amount - 297 pages

Option Explicit


Global TV As TVEngine
Global Scene As TVScene
Global Effect As TVGraphicEffect
Global External As TVMesh
Global Landscape As TVMesh
Global PipeShaft(4) As TVMesh
Global Buttons(-11 To 144) As TVMesh
Global CallButtonsUp(40) As TVMesh
Global CallButtonsDown(40) As TVMesh
Global StairDoor(1 To 4) As TVMesh
Global Light As TVLightEngine
Global MatFactory As New TVMaterialFactory
Global LightID As Integer
Global LightD As D3DLIGHT8
Global IntroMesh As TVMesh

'New stuff
Global Floor(-10 To 138) As FloorC
Global Elevator(40) As ElevatorC

Global SoundEngine As TVSoundEngine
Global Listener As TVListener
Global ListenerDirection As D3DVECTOR
'Global MousePositionX As Long
'Global MousePositionY As Long
Global Focused As Boolean
Global LineTest As D3DVECTOR
Global LineTest2 As D3DVECTOR
Global KeepAltitude As Integer

Global Atmos As New TVAtmosphere
Global sngPositionX As Single
Global sngPositionY As Single
Global sngPositionZ As Single
Global snglookatX As Single
Global snglookatY As Single
Global snglookatZ As Single
Global sngAngleX As Single
Global sngAngleY As Single
Global sngWalk As Single
Global sngStrafe As Single

Global Inp As TVInputEngine
Global TextureFactory As TVTextureFactory
Global linestart As D3DVECTOR
Global lineend As D3DVECTOR
Global isRunning As Boolean
Global Camera As TVCamera
Global ColRes As TV_COLLISIONRESULT
Global i As Single
Global i50 As Single
Global i51 As Single
Global i52 As Single
Global i53 As Single
Global i54 As Single
Global j As Single
Global j50 As Single
Global X As Integer
Global CollisionResult As TVCollisionResult

Global CameraFloor As Integer
'Global CameraFloorExact As Single
Global InStairwell As Boolean
'3D Objects
Global Buildings As TVMesh
Global FileName As String
Global OpeningDoor As Integer
Global ClosingDoor As Integer
Global DoorNumber As Integer
Global DoorRotated As Integer
Global ElevatorNumber As Integer
Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)
Declare Function GetTickCount Lib "kernel32.dll" () As Long

Global Test1 As Boolean
Global InElevator As Boolean

Global tmpMouseX As Long, tmpMouseY As Long, tmpMouseB1 As Integer
Global EnableCollisions As Boolean
Global Gravity As Single
Global IsFalling As Boolean
Global lngOldTick As Long
Global FallRate As Single
Global CameraOriginalPos As Single

Global ButtonsEnabled As Boolean
Global CrawlSpaceHeight As Single
Global ElevatorSpeed As Single
Global ElevatorFineTuneSpeed As Single
Global CallingStairDoors As Boolean
Global SelectedObject As String
Global MainMusic As TVSoundMP3

'Global CallButtonTemp(40) As Integer
Global RenderOnly As Boolean
Global IntroOn As Boolean
Global SkipIntro As Boolean
Global SoundDivisor As Integer
Global SoundMaxDistance As Integer
Global LookUp As Integer
Global BottomFloor As Integer
Global TopFloor As Integer


Sub CalculateFloorAltitudes()
'This stores the altitude of all floors

Dim curfloor As Single
Dim total As Single
total = 0

For curfloor = 0 To (TopFloor - 1)
total = total + FloorHeightTable(curfloor, 1)
FloorHeightTable(curfloor + 1, 0) = total
Next curfloor

total = 0

For curfloor = -1 To BottomFloor Step -1
total = total - FloorHeightTable(curfloor, 1)
FloorHeightTable(curfloor, 0) = total
Next curfloor

End Sub

Sub CleanUp()
    Set Listener = Nothing
    Scene.DestroyAllMeshes
    Set SoundEngine = Nothing
    Set TV = Nothing
    Set Camera = Nothing
    Set TextureFactory = Nothing
    Set Scene = Nothing
    Set Light = Nothing
    Set External = Nothing
    'Set Shafts = Nothing
    For i = BottomFloor To TopFloor
    Set Room(i) = Nothing
    Set CrawlSpace(i) = Nothing
    Set CrawlSpaceShaft1(i) = Nothing
    Set CrawlSpaceShaft2(i) = Nothing
    Set CrawlSpaceShaft3(i) = Nothing
    Set CrawlSpaceShaft4(i) = Nothing
    Set Shafts1(i) = Nothing
    Set Shafts2(i) = Nothing
    Set Shafts3(i) = Nothing
    Set Shafts4(i) = Nothing
    Set ShaftsFloor(i) = Nothing
    Set Stairs(i) = Nothing
    Next i
    For i = 1 To 40
    Set ElevatorMesh(i) = Nothing
    Set FloorIndicator(i) = Nothing
    Set ElevatorInsDoorL(i) = Nothing
    Set ElevatorInsDoorR(i) = Nothing
    Set ElevatorDoorL(i) = Nothing
    Set ElevatorDoorR(i) = Nothing
    Set Plaque(i) = Nothing
    Set ElevatorMusic(i) = Nothing
    Set ElevatorSounds(i) = Nothing
    Next i
End Sub

Sub Init_Simulator()
Sim.ScaleWidth = 10440
Sim.ScaleMode = 1
Sim.ScaleHeight = 7920

On Error GoTo ErrorHandler
isRunning = True
EnableCollisions = True
DebugPanel.Show
Sim.Show
  
Sim.Label1.Caption = "Skyscraper " + LTrim(Str$(App.Major)) + "." + LTrim(Str$(App.Minor)) + " Alpha - Build" + Str$(App.Revision) + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "©2004 Ryan Thoryk" + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "Compiled on July 21, 2004" + vbCrLf + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "Skyscraper comes with ABSOLUTELY NO WARRANTY. This is free" + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "software, and you are welcome to redistribute it under certain" + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "conditions. For details, see the file gpl.txt" + vbCrLf
Sim.Label1.Caption = Sim.Label1.Caption + "Build number counting has been done since version 0.7" + vbCrLf

DoEvents
   
Sleep 2000
   
'Set TV = New TVEngine
'Set Scene = New TVScene

Set Camera = New TVCamera

'Set TextureFactory = New TVTextureFactory
'Set SoundEngine = New TV3DMedia.TVSoundEngine
Set Light = New TVLightEngine

'If TV.ShowDriverDialog = False Then End

   '2. Initialize the engine with the selected mode
    TV.SetSearchDirectory App.Path
Sim.Label2.Caption = "Initializing TrueVision3D..."
   'TV.Initialize Sim.hWnd
    TV.Init3DWindowedMode Sim.hWnd
    'TV.Init3DFullscreen 640, 480, 16

  'Set Inp = New TVInputEngine
  'TV.SetSearchDirectory App.Path
  TV.DisplayFPS = True
  
  'TV.MultiSampleTp = TV3D_MULTISAMPLE_16_SAMPLES
  'TV.EnableAntialising True
    
  Scene.SetTextureFilter (TV_FILTER_ANISOTROPIC)
  'Scene.SetDithering True
  TV.EnableAntialising True
  
  Scene.LoadCursor App.Path + "\data\pointer.bmp", TV_COLORKEY_BLACK, 14, 16
  
  'Set Mesh = Scene.CreateMeshBuilder("Mesh")
  Sim.Label2.Caption = "Processing Meshes..."
  Set IntroMesh = Scene.CreateMeshBuilder("IntroMesh")
  
  'Create floor objects, and initialize them
  For i = BottomFloor To TopFloor
  DoEvents
  Floor(i) = New FloorObject
  Call Floor(i).Init(i)
  Next i
  
  'Create pipe shafts
  For i = 1 To 4
  DoEvents
  Set PipeShaft(i) = Scene.CreateMeshBuilder("PipeShaft " + Str$(i))
  Next i
  
  'Create elevator objects
  For i = 1 To 40
  DoEvents
  Elevator(i) = New ElevatorObject
  Call Elevator(i).Init(i)
  Next i
  
  Set Buildings = Scene.CreateMeshBuilder("Buildings")
  Set External = Scene.CreateMeshBuilder("External")
  Set Landscape = Scene.CreateMeshBuilder("Landscape")
    
  'Elevator Button Meshes
  For i54 = -11 To 144
  DoEvents
  Set Buttons(i54) = Scene.CreateMeshBuilder("Buttons " + Str$(i54))
  'Buttons(i54).SetPosition 0, ElevatorMesh(Number).GetPosition.Y, 0
  'Buttons(i54).SetMeshCenter 0, 0, 0
  'Buttons(i54).SetPosition 0, 0, 0
  'Buttons(i54).SetRotation 0, 0, 0
  Next i54
  
  DoEvents
  Sim.Label2.Caption = "Loading Textures..."
  Scene.SetViewFrustum 90, 200000
  
  TextureFactory.LoadTexture App.Path + "\data\brick1.jpg", "BrickTexture"
  TextureFactory.LoadTexture App.Path + "\data\LobbyFront.jpg", "LobbyFront"
  TextureFactory.LoadTexture App.Path + "\data\windows11c.jpg", "MainWindows"
  TextureFactory.LoadTexture App.Path + "\data\granite.jpg", "Granite"
  'TextureFactory.LoadTexture App.Path+"\data\marbl3.jpg", "Marble3"
  'TextureFactory.LoadTexture App.Path+"\data\text12.jpg", "Marble3"
  TextureFactory.LoadTexture App.Path + "\data\symb5.jpg", "Marble3"
  TextureFactory.LoadTexture App.Path + "\data\marb047.jpg", "Marble4"
  TextureFactory.LoadTexture App.Path + "\data\elev1.jpg", "Elev1"
  TextureFactory.LoadTexture App.Path + "\data\textur15.jpg", "Wood1"
  TextureFactory.LoadTexture App.Path + "\data\text16.jpg", "Wood2"
  'TextureFactory.LoadTexture App.Path+"\data\text12.jpg", "Wall1"
  TextureFactory.LoadTexture App.Path + "\data\marbl3.jpg", "Wall1"
  TextureFactory.LoadTexture App.Path + "\data\marb123.jpg", "Wall2"
  'TextureFactory.LoadTexture App.Path+"\data\marbl3.jpg", "Wall2"
  'TextureFactory.LoadTexture App.Path+"\data\marb056.jpg", "Wall3"
  TextureFactory.LoadTexture App.Path + "\data\cutston.jpg", "Ceiling1"
  TextureFactory.LoadTexture App.Path + "\data\text12.jpg", "Wall3"
  TextureFactory.LoadTexture App.Path + "\data\text16.jpg", "ElevDoors"
  TextureFactory.LoadTexture App.Path + "\data\marb148.jpg", "ElevExtPanels"
  TextureFactory.LoadTexture App.Path + "\data\mason01.jpg", "Concrete"
  TextureFactory.LoadTexture App.Path + "\data\text13.jpg", "Stairs"
  TextureFactory.LoadTexture App.Path + "\data\wooddoor3.jpg", "Door1"
  TextureFactory.LoadTexture App.Path + "\data\wooddoor1.jpg", "Door2"
  TextureFactory.LoadTexture App.Path + "\data\servicedoor2.jpg", "StairsDoor"
  TextureFactory.LoadTexture App.Path + "\data\miscdoor8.jpg", "StairsDoor2"
  TextureFactory.LoadTexture App.Path + "\data\button1.jpg", "CallButtonsTex"
  TextureFactory.LoadTexture App.Path + "\data\sidewalk1r.jpg", "Road1"
  TextureFactory.LoadTexture App.Path + "\data\walkway.jpg", "Walkway"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner1.jpg", "Road2"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner2.jpg", "Road3"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner3.jpg", "Road4"
  TextureFactory.LoadTexture App.Path + "\data\roadfull.jpg", "Road5"
  TextureFactory.LoadTexture App.Path + "\data\windows08.jpg", "Windows8"
  TextureFactory.LoadTexture App.Path + "\data\windows11.jpg", "Windows11"
  TextureFactory.LoadTexture App.Path + "\data\downtown.jpg", "Downtown"
  TextureFactory.LoadTexture App.Path + "\data\suburbs.jpg", "Suburbs"
      
  TextureFactory.LoadTexture App.Path + "\data\top.jpg", "SkyTop"
  TextureFactory.LoadTexture App.Path + "\data\bottom.jpg", "SkyBottom"
  TextureFactory.LoadTexture App.Path + "\data\left.jpg", "SkyLeft"
  TextureFactory.LoadTexture App.Path + "\data\right.jpg", "SkyRight"
  TextureFactory.LoadTexture App.Path + "\data\front.jpg", "SkyFront"
  TextureFactory.LoadTexture App.Path + "\data\back.jpg", "SkyBack"
  TextureFactory.LoadTexture App.Path + "\objects\benedeti.jpg", "ColumnTex", , , TV_COLORKEY_NO
  TextureFactory.LoadTexture App.Path + "\data\plaque.jpg", "Plaque"
  TextureFactory.LoadTexture App.Path + "\data\floorsign.jpg", "FloorSign"
  TextureFactory.LoadTexture App.Path + "\data\floorsignballroom.jpg", "FloorSignBallroom"
  TextureFactory.LoadTexture App.Path + "\data\floorsignbalcony.jpg", "FloorSignBalcony"
  TextureFactory.LoadTexture App.Path + "\data\floorsignhotel.jpg", "FloorSignHotel"
  TextureFactory.LoadTexture App.Path + "\data\floorsignlobby.jpg", "FloorSignLobby"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmaint.jpg", "FloorSignMaint"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmez.jpg", "FloorSignMez"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmechanical.jpg", "FloorSignMechanical"
  TextureFactory.LoadTexture App.Path + "\data\floorsignobservatory.jpg", "FloorSignObservatory"
  TextureFactory.LoadTexture App.Path + "\data\floorsignoffices.jpg", "FloorSignOffices"
  TextureFactory.LoadTexture App.Path + "\data\floorsignpool.jpg", "FloorSignPool"
  TextureFactory.LoadTexture App.Path + "\data\floorsignresidential.jpg", "FloorSignResidential"
  TextureFactory.LoadTexture App.Path + "\data\floorsignroof.jpg", "FloorSignRoof"
  TextureFactory.LoadTexture App.Path + "\data\floorsignskylobby.jpg", "FloorSignSkylobby"
  
  'loads all the floor indicator/button pictures
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "ButtonL"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\M.jpg", "ButtonM"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\R.jpg", "ButtonR"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\open.jpg", "ButtonOpen"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\close.jpg", "ButtonClose"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\stop.jpg", "ButtonStop"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\alarm.jpg", "ButtonAlarm"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\cancel.jpg", "ButtonCancel"
  For i = 2 To TopFloor
  DoEvents
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\" + Mid$(Str$(i), 2) + ".jpg", "Button" + Mid$(Str$(i), 2)
  Next i

   
  'Sound System
    
    'Call SoundEngine.Init(Sim.hWnd)
    
    'Load the file into the classes.
    'ElevatorMusic.Load App.Path + "\data\elevmusic3.wav"
    'ElevatorMusic.Load App.Path + "\data\elevmusic2.wav"
    'Set sound properties.
    'ElevatorMusic.Volume = -300
    'ElevatorMusic.maxDistance = 1000
    'Call ElevatorMusic.SetConeOrientation(0, 0, 90)
    'ElevatorMusic.ConeOutsideVolume = -300
    'Call ElevatorMusic.SetPosition(-20.25, 20, -23)
    'Call ElevatorMusic.SetPosition(0, 10, 0)
    'Setup the 3D listener.
    Set Listener = SoundEngine.Get3DListener
    'Call Listener.SetPosition(picDraw.ScaleWidth / 2, 0, picDraw.ScaleHeight / 2)
    'Call Listener.SetPosition(0, 0, 0)
    'Listener.rolloffFactor = 0.1
    'Listener.distanceFactor = 50
DoEvents
  
Exit Sub

ErrorHandler:
   MsgBox "An error occurred during the simulator initialization process." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
   
End Sub

Sub OptimizeMeshes()
  External.Optimize
  Landscape.Optimize
  Buildings.Optimize
  PipeShaft(1).Optimize
  PipeShaft(2).Optimize
  PipeShaft(3).Optimize
  PipeShaft(4).Optimize
  
  For i = BottomFloor To TopFloor
  DoEvents
  Sim.Label2.Caption = "Optimizing Meshes Part 1 (of 2)... " + Str$(Int(((i + 11) / 148) * 100)) + "%"
  Room(i).Optimize
  CrawlSpace(i).Optimize
  CrawlSpaceShaft1(i).Optimize
  CrawlSpaceShaft2(i).Optimize
  CrawlSpaceShaft3(i).Optimize
  CrawlSpaceShaft4(i).Optimize
  ShaftsFloor(i).Optimize
  Shafts1(i).Optimize
  Shafts2(i).Optimize
  Shafts3(i).Optimize
  Shafts4(i).Optimize
  Next i
  For i = 1 To 40
  DoEvents
  Sim.Label2.Caption = "Optimizing Meshes Part 2 (of 2)... " + Str$(Int((i / 40) * 100)) + "%"
  ElevatorMesh(i).Optimize
  ElevatorInsDoorL(i).Optimize
  ElevatorInsDoorR(i).Optimize
  Plaque(i).Optimize
  Next i
  
End Sub

Sub ProcessFloors()
'Lobby
'Call ProcessBasement
Call ProcessLobby
'Call Process2to39
'Call Process41to79
'Call Process81to114
'Call Process118to129
'Call ProcessOtherFloors
'Call ProcessOtherFloors2
Sim.Label2.Caption = "Initializing Lobby... "
Call InitRealtime(0)
Call InitObjectsForFloor(0)

Sim.Label2.Caption = "Processing Outside... "
Call ProcessOutside
Sim.Label2.Caption = "Processing Elevators... "
Call ProcessMisc
'Call OptimizeMeshes

End Sub

Sub Start()
ElevatorNumber = 1
CrawlSpaceHeight = 7
'With the next 2 variables, lower=faster
ElevatorSpeed = 8
'ElevatorSpeed = 5 'Original Value
ElevatorFineTuneSpeed = 0.15
'ElevatorFineTuneSpeed = 0.2 'Original Value
CameraFloor = 0
SoundDivisor = 10
SoundMaxDistance = 1000
TopFloor = 138
BottomFloor = -10
SkipIntro = True

Call Init_Simulator
Call ProcessFloorHeights
Call CalculateFloorAltitudes
Call ProcessFloors
Call ProcessFloorNames

    '4 : Old movement system startposition stuff
    'Camera.SetPosition 0, 10, 130
    'Camera.SetLookAt 0, 10, -90
    'Camera.RotateY 3.15
    
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
  External.Enable True
  For i = BottomFloor To TopFloor
  DoEvents
  Room(i).Enable False
  CrawlSpace(i).Enable False
  CrawlSpaceShaft1(i).Enable False
  CrawlSpaceShaft2(i).Enable False
  CrawlSpaceShaft3(i).Enable False
  CrawlSpaceShaft4(i).Enable False
  ShaftsFloor(i).Enable False
  Shafts1(i).Enable False
  Shafts2(i).Enable False
  Shafts3(i).Enable False
  Shafts4(i).Enable False
  Next i
  PipeShaft(1).Enable False
  PipeShaft(2).Enable False
  PipeShaft(3).Enable False
  PipeShaft(4).Enable False
  
  'For i = -11 To TopFloor
  'StairDoor(i).Enable False
  'Next i
  
  For i = 1 To 40
  DoEvents
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

Sim.IntroMusic.Enabled = False
MainMusic.Stop_
Effect.FadeIn 1500

'Start intro animation
RenderOnly = True
Sim.MainTimer.Enabled = True
If SkipIntro = True Then GoTo IntroEnd

IntroMesh.SetPosition 0, 5, 70
Camera.SetPosition -1000, 1550, 1000
IntroOn = True

While Camera.GetPosition.X < -1
DoEvents
Wend

IntroEnd:
RenderOnly = False
IntroOn = False
Camera.SetPosition 0, 10, 130
Camera.SetLookAt 0, 10, -90
Camera.SetRotation 0, 3.15, 0

DebugPanel.Enabled = True
DebugPanel.Timer1.Enabled = True
Sim.Elevator1Timer.Enabled = True
Sim.Elevator2Timer.Enabled = True
Sim.Elevator3Timer.Enabled = True
Sim.Elevator4Timer.Enabled = True
Sim.Elevator5Timer.Enabled = True
Sim.Elevator6Timer.Enabled = True
Sim.Elevator7Timer.Enabled = True
Sim.Elevator8Timer.Enabled = True
Sim.Elevator9Timer.Enabled = True
Sim.Elevator10Timer.Enabled = True
Sim.Elevator11Timer.Enabled = True
Sim.Elevator12Timer.Enabled = True
Sim.Elevator13Timer.Enabled = True
Sim.Elevator14Timer.Enabled = True
Sim.Elevator15Timer.Enabled = True
Sim.Elevator16Timer.Enabled = True
Sim.Elevator17Timer.Enabled = True
Sim.Elevator18Timer.Enabled = True
Sim.Elevator19Timer.Enabled = True
Sim.Elevator20Timer.Enabled = True
Sim.Elevator21Timer.Enabled = True
Sim.Elevator22Timer.Enabled = True
Sim.Elevator23Timer.Enabled = True
Sim.Elevator24Timer.Enabled = True
Sim.Elevator25Timer.Enabled = True
Sim.Elevator26Timer.Enabled = True
Sim.Elevator27Timer.Enabled = True
Sim.Elevator28Timer.Enabled = True
Sim.Elevator29Timer.Enabled = True
Sim.Elevator30Timer.Enabled = True
Sim.Elevator31Timer.Enabled = True
Sim.Elevator32Timer.Enabled = True
Sim.Elevator33Timer.Enabled = True
Sim.Elevator34Timer.Enabled = True
Sim.Elevator35Timer.Enabled = True
Sim.Elevator36Timer.Enabled = True
Sim.Elevator37Timer.Enabled = True
Sim.Elevator38Timer.Enabled = True
Sim.Elevator39Timer.Enabled = True
Sim.Elevator40Timer.Enabled = True
Sim.StairsTimer.Enabled = True
  'Do Until isRunning = False Or Inp.IsKeyPressed(TV_KEY_ESCAPE) = True
'Loop
  
  'Set TV = Nothing
  'End
  
ErrorHandler:
'  Dim Msg
'If Err.Number <> 0 Then
'   Msg = "Error # " & Str(Err.Number) & " was generated by " _
'         & Err.Source & Chr(13) & Err.Description
'   MsgBox Msg, , "Error", Err.HelpFile, Err.HelpContext
'      End



End Sub


