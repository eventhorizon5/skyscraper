VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "SkyScraper"
   ClientHeight    =   7920
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10440
   LinkTopic       =   "Form1"
   ScaleHeight     =   7920
   ScaleWidth      =   10440
   StartUpPosition =   2  'CenterScreen
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
'Skycraper 0.5 Alpha
'Created by Ryan Thoryk
'http://www.tliquest.net/skyscraper

'Currently this code is very sloppy and in one big chunk,
'especially the elevator movement code.
'I'll clean it up later, I promise :)

Option Explicit
Dim TV As TVEngine
Dim Scene As TVScene
Dim Mesh As TVMesh
Dim External As TVMesh
Dim Room(138) As TVMesh
Dim Shafts As TVMesh
Dim Elevator1 As TVMesh
Dim Elevator1DoorL As TVMesh
Dim Elevator1DoorR As TVMesh
Dim ElevatorDoorL(138) As TVMesh
Dim ElevatorDoorR(138) As TVMesh
Dim Stairs1 As TVMesh
Dim SoundEngine As TVSoundEngine
Dim ElevatorMusic As TVSoundWave3D
Dim Elevator1Sounds As TVSoundWave3D
Dim Listener As TVListener
Dim ListenerDirection As D3DVECTOR
'Dim MousePositionX As Long
'Dim MousePositionY As Long
Dim Focused As Boolean

Private sngPositionX As Single
Private sngPositionY As Single
Private sngPositionZ As Single
Private snglookatX As Single
Private snglookatY As Single
Private snglookatZ As Single
Private sngAngleX As Single
Private sngAngleY As Single
Private sngWalk As Single
Private sngStrafe As Single

Dim Inp As TVInputEngine
Dim TextureFactory As TVTextureFactory
Dim linestart As D3DVECTOR
Dim lineend As D3DVECTOR
Dim elevator1start As D3DVECTOR
Dim isRunning As Boolean
Dim Camera As TVCamera
Dim ColRes As TV_COLLISIONRESULT
Dim i As Integer
Dim ElevatorEnable As Single
Dim ElevatorDirection As Integer '1=up -1=down
Dim OpenElevatorLoc As Single
Dim ElevatorCheck As Integer
Dim ElevatorCheck2 As Integer
Dim ElevatorCheck3 As Integer
Dim ElevatorCheck4 As Integer



Private Sub Form_GotFocus()
Focused = True
End Sub

Private Sub Form_Load()
On Error GoTo ErrorHandler
isRunning = True
Form2.Show
Me.Show
Set TV = New TVEngine
Set Scene = New TVScene
Set Mesh = New TVMesh
Set External = New TVMesh
For i = 1 To 138
Set Room(i) = New TVMesh
Set ElevatorDoorL(i) = New TVMesh
Set ElevatorDoorR(i) = New TVMesh
Next i
Set Shafts = New TVMesh
Set Elevator1 = New TVMesh
Set Elevator1DoorL = New TVMesh
Set Elevator1DoorR = New TVMesh
Set Stairs1 = New TVMesh

Set Camera = New TVCamera

Set TextureFactory = New TVTextureFactory
Set SoundEngine = New TV3DMedia.TVSoundEngine
Set ElevatorMusic = New TV3DMedia.TVSoundWave3D
Set Elevator1Sounds = New TV3DMedia.TVSoundWave3D

Print "Skyscraper 0.5 Alpha"
  
  If TV.ShowDriverDialog = False Then End
   DoEvents
   '2. Initialize the engine with the selected mode
   TV.SetSearchDirectory App.Path
Print "Initializing TrueVision3D..."
   'TV.Initialize Me.hWnd
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
  Set ElevatorDoorL(i) = Scene.CreateMeshBuilder("ElevatorDoorL " + Str$(i))
  Set ElevatorDoorR(i) = Scene.CreateMeshBuilder("ElevatorDoorR " + Str$(i))
  Next i
  Set Shafts = Scene.CreateMeshBuilder("Shafts")
  Set Stairs1 = Scene.CreateMeshBuilder("Stairs1")
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
  
Print "Textures Loaded"
      
  'Sound System
    
    Call SoundEngine.Init(Form1.hWnd)
    
Print "Sound Initialized"

    'Load the file into the classes.
    ElevatorMusic.Load App.Path + "\elevmusic3.wav"
    'ElevatorMusic.Load App.Path + "\elevmusic2.wav"
Print "Music Loaded"
    'Set sound properties.
    ElevatorMusic.Volume = -300
    ElevatorMusic.maxDistance = 1000
    Call ElevatorMusic.SetConeOrientation(0, 0, 90)
    ElevatorMusic.ConeOutsideVolume = -300
    Call ElevatorMusic.SetPosition(-20.25, 20, -23)
    'Call ElevatorMusic.SetPosition(0, 10, 0)
    'Setup the 3D listener.
    Set Listener = SoundEngine.Get3DListener
    'Call Listener.SetPosition(picDraw.ScaleWidth / 2, 0, picDraw.ScaleHeight / 2)
    'Call Listener.SetPosition(0, 0, 0)
    Listener.rolloffFactor = 0.1
    'Listener.distanceFactor = 50
    
  'Mesh.AddFloor GetTex("Floor"), 0, 0, 1000, 1000, 0, 10, 10
  'Mesh.AddWall GetTex("Floor"), 1000, 0, 1000, 1000, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 0, 0, 0, 1000, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 1000, 0, 0, 0, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 1000, 1000, 0, 1000, 100, 0, 5
  'Mesh.Optimize
  
    'Elevator Mesh are 19 feet in from the north, 51 feet in from the west and are 8 feet wide
    'Lobby
Print "Processing Main Floor..."
    i = 1
    Room(i).AddFloor GetTex("Marble4"), -160, -150, 160, 150, 0, 50, 31
    
    External.AddWall GetTex("LobbyFront"), -160, -150, 160, -150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, -150, 160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, 150, -160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -160, 150, -160, -150, 75, 0, 3, 1
          
    'Elevators
    Shafts.AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 0, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, -30, 25, 0, 0.8, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, -16, -12.5, 46.25, 25, 0, 4, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, -30, -12.5, -16, 5.5, 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 0, 2, 2
    Shafts.AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 0, 2, 2
    Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    'cut side wall start
    'Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 0, 1, 2
    'Shafts.AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 0, 4, 2
    'cut side wall end
    Shafts.AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 0, 2, 2
    Shafts.AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 25, 2, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 25, 8, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 25, 2, 2
    Shafts.AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 25, 2, 2
    Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 25, 8, 2
    Shafts.AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 25, 2, 2
    Shafts.AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 50, 2, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 50, 8, 2
    Shafts.AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 50, 2, 2
    Shafts.AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 50, 8, 2
    
    Shafts.AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 50, 2, 2
    Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 50, 8, 2
    Shafts.AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 50, 2, 2
    Shafts.AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 50, 8, 2
    
    'Ceiling
    Room(i).AddFloor GetTex("Ceiling1"), -160, -150, 160, -46.25, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, 46.25, 160, 150, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, -46.25, -32.5, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), 32.5, -46.25, 160, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 49.5, 2, 5
    
    
'GoTo 2
Print "Processing Floors 2 to 39..."

    'Floors 2 to 39
    For i = 2 To 39
    'Mesh.AddFloor GetTex("Granite"), -160, -150, 160, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -160, -150, 160, -46.25, (i * 25) + 25, 30, 5
    Room(i).AddFloor GetTex("Granite"), -160, 46.25, 160, 150, (i * 25) + 25, 30, 5
    Room(i).AddFloor GetTex("Granite"), -160, -46.25, -32.5, 46.25, (i * 25) + 25, 11, 10
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 160, 46.25, (i * 25) + 25, 11, 10
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 25, 2, 10
    
    Room(i).AddFloor GetTex("Marble3"), -160, -150, 160, -46.25, (i * 25) + 49.5, 30, 5
    Room(i).AddFloor GetTex("Marble3"), -160, 46.25, 160, 150, (i * 25) + 49.5, 30, 5
    Room(i).AddFloor GetTex("Marble3"), -160, -46.25, -32.5, 46.25, (i * 25) + 49.5, 11, 10
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 160, 46.25, (i * 25) + 49.5, 11, 10
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 49.5, 2, 10
    
    External.AddWall GetTex("MainWindows"), -160, -150, 160, -150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), 160, -150, 160, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 160, 150, -160, 150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), -160, 150, -160, -150, 25, (i * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 46.25, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 46.25, -32.5, 46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 46.25, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 46.25, 32.5, 46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 46.25, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, 160, 71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -160, 46.3, -32.5, 46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.3, 160, 46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -160, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 160, -46.3, 25, (i * 25) + 25, 2, 1
    
    Next i
    
    'Landings
    External.AddFloor GetTex("BrickTexture"), -160, -150, -135, 150, (40 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 160, -150, 135, 150, (40 * 25) + 25, 10, 10
'GoTo 2

Print "Processing Floors 40 to 79"
    'Floors 40 to 79 (minus 14 feet on both sides where 20=8 feet)
    For i = 40 To 79
    'Mesh.AddFloor GetTex("Granite"), -135, -150, 135, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -135, -150, 135, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -135, 30.83, 135, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -135, -46.25, -32.5, 30.83, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 135, 30.83, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 30.83, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -135, -150, 135, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -135, 30.83, 135, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -135, -46.25, -32.5, 30.83, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 135, 30.83, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 30.83, (i * 25) + 49.5, 1, 3
   
    External.AddWall GetTex("MainWindows"), -135, -150, 135, -150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), 135, -150, 135, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 135, 150, -135, 150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), -135, 150, -135, -150, 25, (i * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 30.83, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 30.83, -32.5, 30.83, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 30.83, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 30.83, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 30.83, 32.5, 30.83, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 30.83, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, 71.3, 135, 71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -32.5, 30.83, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 30.83, 135, 30.83, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 135, -46.3, 25, (i * 25) + 25, 2, 1
    
    Next i
    
    External.AddFloor GetTex("BrickTexture"), -135, -150, -110, 150, (80 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 135, -150, 110, 150, (80 * 25) + 25, 10, 10
    
Print "Processing Floors 80 to 117"
    'Floors 80 to 117
    For i = 80 To 117
    'Mesh.AddFloor GetTex("Granite"), -110, -150, 110, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, 9, 110, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -32.5, 9, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 110, 9, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 9, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, 9, 110, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -32.5, 9, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 110, 9, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 9, (i * 25) + 49.5, 1, 3
    
    External.AddWall GetTex("MainWindows"), -110, -150, 110, -150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 110, -150, 110, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 110, 150, -110, 150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), -110, 150, -110, -150, 25, (i * 25) + 25, 7, 1
    
    'Elevators
    
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 15.41, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 15.41, -32.5, 15.41, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 15.41, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 15.41, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 15.41, 32.5, 15.41, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 15.41, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -110, -71.3, 110, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -110, 71.3, 110, 71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 15.41, -32.5, 15.41, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 15.41, 110, 15.41, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 110, -46.3, 25, (i * 25) + 25, 2, 1
        
    Next i
    
    External.AddFloor GetTex("BrickTexture"), -110, -150, -85, 150, (118 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 110, -150, 85, 150, (118 * 25) + 25, 10, 10
    
Print "Processing Floors 118 to 134"
    'Floors 118 to 134 (minus 10 feet)
    For i = 118 To 134
    'Mesh.AddFloor GetTex("Granite"), -85, -150, 85, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * 25) + 49.5, 1, 3
     
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -85, -71.3, 85, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -85, 71.3, 85, 71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 0, 85, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 85, -46.3, 25, (i * 25) + 25, 2, 1
    
    Next i

    External.AddFloor GetTex("BrickTexture"), -85, -150, -60, 150, (135 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 85, -150, 60, 150, (135 * 25) + 25, 10, 10
    
Print "Processing Floors 135 to 137"
    'Floors 135 to 137 (minus 6 feet)
    For i = 135 To 137
    'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("Granite"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), 12.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("Marble3"), -60, -15.42, 60, 150, (i * 25) + 49.5, 8, 8.6
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), 12.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'Shafts.AddWall GetTex("Wall1"), 12.5, -15.42, 32.5, -15.42, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), 32.5, -15.42, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next i



'Roof Layout
'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
i = 138
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("BrickTexture"), -32.5, -46.25, -12.5, -15.42, (i * 25) + 50.1, 2, 2.4
    
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall3"), -12.5, -30.85, -32.5, -30.85, (25 * 138) + 25, 0, 1, 139
    Shafts.AddWall GetTex("Wall3"), 12.5, -30.85, 32.5, -30.85, (25 * 135) + 25, 0, 1, 139
    
    Shafts.AddWall GetTex("Wall3"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, (25 * 138) + 25, 0, 1, 139
        
    'Shafts.AddFloor GetTex("BrickTexture"), -60, -150, 60, 150, (138 * 25) + 25, 10, 10
2
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
    ElevatorDoorL(1).AddWall GetTex("ElevDoors"), -12.55, -19.05, -12.55, -22.95, 19.6, 0, 1, 1
    ElevatorDoorR(1).AddWall GetTex("ElevDoors"), -12.55, -23.05, -12.55, -27.05, 19.6, 0, 1, 1
    For i = 2 To 138
    ElevatorDoorL(i).AddWall GetTex("ElevDoors"), -12.55, -19, -12.55, -23, 19.6, (i * 25) + 25, 1, 1
    ElevatorDoorR(i).AddWall GetTex("ElevDoors"), -12.55, -23, -12.55, -27, 19.6, (i * 25) + 25, 1, 1
    'Exterior Panels
    Room(i).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, (i * 25) + 25, 0.5, 1
    Room(i).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, (i * 25) + 25, 0.5, 1
    
    Next i
    
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, 0, 0.5, 1
    Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, 0, 0.5, 1
    
    'For i = 0 To 139
    For i = 130 To 138
    Print "Processing Stairs on floor " + Str$(i)
    'Stairs
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10
    
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12
    
    Stairs1.AddFloor GetTex("Wood2"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16
    Stairs1.AddFloor GetTex("Wood2"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14
    
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14
    Stairs1.AddWall GetTex("Wood2"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12
    
    Next i
    
    '4 : Old movement system startposition stuff
    'Camera.SetCameraPosition 0, 10, 0
    Camera.SetPosition 0, 10, 90
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
  ElevatorDoorL(i).Enable False
  ElevatorDoorR(i).Enable False
  Next i
  
  Room(1).Enable True
  ElevatorDoorL(1).Enable True
  ElevatorDoorR(1).Enable True
  
  'External.Enable False
  'Shafts.Enable False
  'Elevator1.Enable False
  'Room.Optimize
  'External.Optimize
  'Shafts.Optimize
  'Elevator1.Optimize
  
  ElevatorMusic.Loop_ = True
  'Call ElevatorMusic.Play
  
  
  Dim stuff As Boolean
  Do Until isRunning = False Or Inp.IsKeyPressed(TV_KEY_ESCAPE) = True

    linestart = Camera.GetPosition
    elevator1start = Elevator1.GetPosition

    TV.Clear
    Call Listener.SetPosition(Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    Call ElevatorMusic.GetPosition(ListenerDirection.x, ListenerDirection.y, ListenerDirection.z)
    'Call Camera.GetRotation(ListenerDirection.x, ListenerDirection.y, ListenerDirection.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    
    
'*** First movement system
      If Inp.IsKeyPressed(TV_KEY_UP) = True Then Camera.MoveRelative 0.7, 0, 0
      If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then Camera.MoveRelative -0.7, 0, 0
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
        
      'If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True Then Camera.RotateX -0.006
      'If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True Then Camera.RotateX 0.006
      
      'If Inp.IsKeyPressed(TV_KEY_HOME) = True Then Camera.MoveRelative 0, 1, 0
      'If Inp.IsKeyPressed(TV_KEY_END) = True Then Camera.MoveRelative 0, -1, 0
      'If Inp.IsKeyPressed(TV_KEY_1) = True Then ElevatorDirection = 1
      'If Inp.IsKeyPressed(TV_KEY_2) = True Then ElevatorDirection = -1
      If Inp.IsKeyPressed(TV_KEY_3) = True Then OpenElevator = 1
      If Inp.IsKeyPressed(TV_KEY_4) = True Then OpenElevator = -1
      If Inp.IsKeyPressed(TV_KEY_5) = True Then Call ElevatorMusic.Play
      If Inp.IsKeyPressed(TV_KEY_6) = True Then Call ElevatorMusic.Stop_
      
      'Inp.GetMouseState MousePositionX, MousePositionY
      'MsgBox (Str$(MousePositionX) + "," + Str$(MousePositionY))
      'Camera.SetLookAt -50, -50, -50
      'Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x - (MousePositionX / 100), Camera.GetLookAt.y - (MousePositionY / 100), Camera.GetLookAt.z
      
      'Orientation: 0-1 pan to center, -1 left 1 right, ?, same as 1st, same as 2nd, ?
      'If Inp.IsKeyPressed(TV_KEY_7) = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
      'If Inp.IsKeyPressed(TV_KEY_8) = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
        
      If Inp.IsKeyPressed(TV_KEY_F1) = True Then TV.ScreenShot ("c:\shot.bmp")
      
      ElevatorFloor = (Elevator1.GetPosition.y - 25) / 25
      If ElevatorFloor < 1 Then ElevatorFloor = 1
      
      CameraFloor = (Camera.GetPosition.y - 25 - 10) / 25
      If CameraFloor < 1 Then CameraFloor = 1
      
      If GotoFloor <> 0 And GotoFloor > CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor).GetPosition.z <= 0 Then
      ElevatorDirection = 1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((GotoFloor * 25) + 25) - ((CurrentFloorExact * 25) + 25)
      If Form2.Check1.Value = 1 Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor).Enable False
      ElevatorDoorR(CameraFloor).Enable False
      End If
      End If
      If GotoFloor <> 0 And GotoFloor < CurrentFloor And ElevatorDirection = 0 And ElevatorDoorL(ElevatorFloor).GetPosition.z <= 0 Then
      ElevatorDirection = -1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((CurrentFloorExact * 25) + 25) - ((GotoFloor * 25) + 25)
      If Form2.Check1.Value = 1 Then
      Room(CameraFloor).Enable False
      ElevatorDoorL(CameraFloor).Enable False
      ElevatorDoorR(CameraFloor).Enable False
      End If
      End If
      
       CurrentFloor = Int((elevator1start.y - 25) / 25)
       CurrentFloorExact = (elevator1start.y - 25) / 25
       Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable / 5)
        
        If ElevatorEnable >= 0 And ElevatorDirection = 1 Then
        'sound
        If Elevator1Sounds.PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck = 0 And GotoFloor <> ElevatorFloor Then
        Elevator1Sounds.Loop_ = False
        Elevator1Sounds.Load App.Path + "\elevstart.wav"
        Elevator1Sounds.Volume = 0
        Elevator1Sounds.maxDistance = 100
        Call Elevator1Sounds.SetConeOrientation(0, 0, 90)
        Elevator1Sounds.ConeOutsideVolume = 0
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.y + 20, -23)
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
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable <= 15 Then StoppingDistance = CurrentFloorExact - OriginalLocation + 0.4
        If ElevatorEnable > 15 Then ElevatorEnable = 15
        Destination = ((OriginalLocation * 25) + 25) + DistanceToTravel - 35
        If GotoFloor <> 0 And elevator1start.y >= (Destination - (StoppingDistance * 25) + 25) Then ElevatorDirection = -1: ElevatorCheck = 0
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
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
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
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.y + 20, -23)
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
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable >= -15 Then StoppingDistance = OriginalLocation - CurrentFloorExact
        If ElevatorEnable < -15 Then ElevatorEnable = -15
        Destination = ((OriginalLocation * 25) + 25) - DistanceToTravel
        If GotoFloor <> 0 And elevator1start.y <= (Destination + (StoppingDistance * 25) + 25) Then ElevatorDirection = 1: ElevatorCheck = 0
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
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        Elevator1Sounds.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorMusic.SetPosition -20.25, Elevator1.GetPosition.y + 20, -23
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable > 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then ElevatorCheck = 0: FineTune = True
        End If
      
      If FineTune = True And ElevatorEnable = 0 And elevator1start.y > (GotoFloor * 25) + 25 + -0.3 And elevator1start.y < (GotoFloor * 25) + 25 + 0.3 Then
      FineTune = False
      Room(CameraFloor).Enable True
      ElevatorDoorL(CameraFloor).Enable True
      ElevatorDoorR(CameraFloor).Enable True
      GotoFloor = 0
      OpenElevator = 1
      ElevatorCheck = 0
      ElevatorCheck2 = 0
      ElevatorCheck3 = 0
      ElevatorCheck4 = 0
      End If
      
      If FineTune = True Then
      If ElevatorCheck3 = 0 Then
      Elevator1Sounds.Load App.Path + "\ding1.wav"
      Elevator1Sounds.Play
      ElevatorCheck3 = 1
      End If
      If elevator1start.y < (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, 0.2, 0
      Elevator1DoorL.MoveRelative 0, 0.2, 0
      Elevator1DoorR.MoveRelative 0, 0.2, 0
      If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevator1start.y > (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, -0.2, 0
      Elevator1DoorL.MoveRelative 0, -0.2, 0
      Elevator1DoorR.MoveRelative 0, -0.2, 0
      If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator = 1 Then
      If ElevatorDoorL(ElevatorFloor).GetPosition.z >= 4 Then OpenElevator = 0: GoTo OpenElevator1
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
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor).GetPosition.z > 1 Then OpenElevator = 2
OpenElevator1:
      End If
      
      If OpenElevator = 2 Then
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor).GetPosition.z > 3 Then OpenElevator = 3
      End If
      
      If OpenElevator = 3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      If ElevatorDoorL(ElevatorFloor).GetPosition.z < 7 And OpenElevatorLoc = 0 Then OpenElevatorLoc = 0.02
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc <= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Timer1.Enabled = True
      End If
      End If
      
      If OpenElevator = -1 Then
      If ElevatorDoorL(ElevatorFloor).GetPosition.z <= 0 Then OpenElevator = 0: GoTo OpenElevator2
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
        Call Elevator1Sounds.SetPosition(-20.25, Elevator1.GetPosition.y, -23)
        Elevator1Sounds.Play
        ElevatorCheck4 = 1
      End If
      OpenElevatorLoc = OpenElevatorLoc - 0.02
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor).GetPosition.z < 3 Then OpenElevator = -2
OpenElevator2:
      End If
      
      If OpenElevator = -2 Then
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If ElevatorDoorL(ElevatorFloor).GetPosition.z < 1 Then OpenElevator = -3
      End If
      
      If OpenElevator = -3 Then
      ElevatorCheck4 = 0
      OpenElevatorLoc = OpenElevatorLoc + 0.02
      If ElevatorDoorL(ElevatorFloor).GetPosition.z > 0 And OpenElevatorLoc >= 0 Then OpenElevatorLoc = -0.02
      ElevatorDoorL(ElevatorFloor).MoveRelative OpenElevatorLoc, 0, 0
      ElevatorDoorR(ElevatorFloor).MoveRelative -OpenElevatorLoc, 0, 0
      Elevator1DoorL.MoveRelative OpenElevatorLoc, 0, 0
      Elevator1DoorR.MoveRelative -OpenElevatorLoc, 0, 0
      If OpenElevatorLoc >= 0 Then
      OpenElevator = 0
      OpenElevatorLoc = 0
      Elevator1DoorL.SetPosition Elevator1DoorL.GetPosition.x, Elevator1DoorL.GetPosition.y, 0
      Elevator1DoorR.SetPosition Elevator1DoorR.GetPosition.x, Elevator1DoorR.GetPosition.y, 0
      ElevatorDoorL(ElevatorFloor).SetPosition ElevatorDoorL(ElevatorFloor).GetPosition.x, ElevatorDoorL(ElevatorFloor).GetPosition.y, 0
      ElevatorDoorR(ElevatorFloor).SetPosition ElevatorDoorR(ElevatorFloor).GetPosition.x, ElevatorDoorR(ElevatorFloor).GetPosition.y, 0
      End If
      End If
      
      lineend = Camera.GetPosition
    
    If Elevator1.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If ElevatorDoorL(CameraFloor).Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If ElevatorDoorR(CameraFloor).Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If Room(CameraFloor).Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If Shafts.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If Elevator1DoorL.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    If Elevator1DoorR.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    'If External.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
    '    Camera.SetPosition linestart.x, linestart.y, linestart.z
    'Else
    '    Camera.SetPosition lineend.x, lineend.y, lineend.z
    'End If
    
    'If Mesh.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
    '    Camera.SetPosition linestart.x, linestart.y, linestart.z
    'Else
    '    Camera.SetPosition lineend.x, lineend.y, lineend.z
    'End If
    
        
    Scene.RenderAllMeshes
    TV.RenderToScreen
    DoEvents
  Loop
  
  Set TV = Nothing
  End
  
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
Set ElevatorMusic = Nothing
Set Elevator1Sounds = Nothing

    Set Listener = Nothing
    Set SoundEngine = Nothing
End Sub

Private Sub Timer1_Timer()
OpenElevator = -1
Timer1.Enabled = False
End Sub
