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
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.4 Alpha
'Created by Ryan Thoryk
'http://www.tliquest.net/skyscraper

Option Explicit
Dim TV As TVEngine
Dim Scene As TVScene
Dim Mesh As TVMesh
Dim External As TVMesh
Dim Room As TVMesh
Dim Shafts As TVMesh
Dim Elevator1 As TVMesh

Dim Inp As TVInputEngine
Dim TextureFactory As TVTextureFactory
Dim linestart As D3DVECTOR
Dim lineend As D3DVECTOR
Dim elevator1start As D3DVECTOR
Dim isRunning As Boolean
Dim Camera As TVCamera
Dim ColRes As TV_COLLISIONRESULT
Dim I As Integer
Dim ElevatorEnable As Single
Dim ElevatorDirection As Integer '1=up -1=down


Private Sub Form_Load()
isRunning = True
Form2.Show
Me.Show
Set TV = New TVEngine
Set Scene = New TVScene
Set Mesh = New TVMesh
Set External = New TVMesh
Set Room = New TVMesh
Set Shafts = New TVMesh
Set Elevator1 = New TVMesh

Set Camera = New TVCamera

Set TextureFactory = New TVTextureFactory

Print "Skyscraper 0.3 Alpha"
  
  If TV.ShowDriverDialog = False Then End
   DoEvents
   '2. Initialize the engine with the selected mode
   TV.SetSearchDirectory App.Path
   TV.Initialize Me.hWnd
   
    'TV.Init3DWindowedMode Form1.hWnd
    'TV.Init3DFullscreen 640, 480, 16
  Set Inp = New TVInputEngine
  TV.SetSearchDirectory App.Path
  'TV.DisplayFPS = True
  Set Mesh = Scene.CreateMeshBuilder("Mesh")
  Set External = Scene.CreateMeshBuilder("External")
  Set Room = Scene.CreateMeshBuilder("Room")
  Set Shafts = Scene.CreateMeshBuilder("Shafts")
  Set Elevator1 = Scene.CreateMeshBuilder("Elevator1")
  
  Scene.SetViewFrustum 90, 10000
  'TextureFactory.LoadTexture "..\..\..\media\stone_wall.bmp", "Floor"
  
  TextureFactory.LoadTexture "brick1.bmp", "BrickTexture"
  TextureFactory.LoadTexture "LobbyFront.jpg", "LobbyFront"
  TextureFactory.LoadTexture "windows11c.jpg", "MainWindows"
  TextureFactory.LoadTexture "granite.bmp", "Granite"
  TextureFactory.LoadTexture "marbl3.bmp", "Marble3"
  TextureFactory.LoadTexture "marbl4.bmp", "Marble4"
  TextureFactory.LoadTexture "elev1.bmp", "Elev1"
  TextureFactory.LoadTexture "textur15.bmp", "Wood1"
  TextureFactory.LoadTexture "text16.bmp", "Wood2"
  TextureFactory.LoadTexture "text12.bmp", "Wall1"
  
  'Mesh.AddFloor GetTex("Floor"), 0, 0, 1000, 1000, 0, 10, 10
  'Mesh.AddWall GetTex("Floor"), 1000, 0, 1000, 1000, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 0, 0, 0, 1000, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 1000, 0, 0, 0, 100, 0, 5
  'Mesh.AddWall GetTex("Floor"), 1000, 1000, 0, 1000, 100, 0, 5
  'Mesh.Optimize
  
    'Elevator Mesh are 19 feet in from the north, 51 feet in from the west and are 8 feet wide
    'Lobby
Print "Processing Main Floor..."
    I = 1
    Room.AddFloor GetTex("Marble4"), -150, -93.75, 150, 93.75, 0, 50, 31
    
    External.AddWall GetTex("LobbyFront"), -150, -93.75, 150, -93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 150, -93.75, 150, 93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 150, 93.75, -150, 93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -150, 93.75, -150, -93.75, 75, 0, 3, 1
          
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, 0, 1, 2
    'Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 46.25, 25, 0, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -30, 25, 0, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, 46.25, 25, 0, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -30, -12.5, -16, 5.5, 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 46.25, -32.5, 46.25, 25, 0, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 46.25, -32.5, -46.25, 25, 0, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, 0, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 46.25, 25, 0, 3, 2
    'cut side wall start
    'Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, -30, 25, 0, 3, 2
    'Shafts.AddWall GetTex("BrickTexture"), 12.5, -16, 12.5, 46.25, 25, 0, 3, 2
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 46.25, 32.5, 46.25, 25, 0, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 46.25, 32.5, -46.25, 25, 0, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 46.25, 25, 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 46.25, -32.5, 46.25, 25, 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 46.25, -32.5, -46.25, 25, 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 46.25, 25, 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 46.25, 32.5, 46.25, 25, 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 46.25, 32.5, -46.25, 25, 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, 50, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 46.25, 25, 50, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 46.25, -32.5, 46.25, 25, 50, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 46.25, -32.5, -46.25, 25, 50, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, 50, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 46.25, 25, 50, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 46.25, 32.5, 46.25, 25, 50, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 46.25, 32.5, -46.25, 25, 50, 3, 2
    
    'Ceiling
    Room.AddFloor GetTex("Marble3"), -150, -93.75, 150, -46.25, (I * 25) + 49.5, 30, 5
    Room.AddFloor GetTex("Marble3"), -150, 46.25, 150, 93.75, (I * 25) + 49.5, 30, 5
    Room.AddFloor GetTex("Marble3"), -150, -46.25, -32.5, 46.25, (I * 25) + 49.5, 11, 10
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 150, 46.25, (I * 25) + 49.5, 11, 10
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (I * 25) + 49.5, 2, 10
    
    
'GoTo 2
Print "Processing Floors 2 to 39..."

    'Floors 2 to 39
    For I = 2 To 39
    'Mesh.AddFloor GetTex("Granite"), -150, -93.75, 150, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("Granite"), -150, -93.75, 150, -46.25, (I * 25) + 25, 30, 5
    Room.AddFloor GetTex("Granite"), -150, 46.25, 150, 93.75, (I * 25) + 25, 30, 5
    Room.AddFloor GetTex("Granite"), -150, -46.25, -32.5, 46.25, (I * 25) + 25, 11, 10
    Room.AddFloor GetTex("Granite"), 32.5, -46.25, 150, 46.25, (I * 25) + 25, 11, 10
    Room.AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, (I * 25) + 25, 2, 10
    
    Room.AddFloor GetTex("Marble3"), -150, -93.75, 150, -46.25, (I * 25) + 49.5, 30, 5
    Room.AddFloor GetTex("Marble3"), -150, 46.25, 150, 93.75, (I * 25) + 49.5, 30, 5
    Room.AddFloor GetTex("Marble3"), -150, -46.25, -32.5, 46.25, (I * 25) + 49.5, 11, 10
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 150, 46.25, (I * 25) + 49.5, 11, 10
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (I * 25) + 49.5, 2, 10
    
    External.AddWall GetTex("MainWindows"), -150, -93.75, 150, -93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), 150, -93.75, 150, 93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), 150, 93.75, -150, 93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), -150, 93.75, -150, -93.75, 25, (I * 25) + 25, 9, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (I * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 46.25, -32.5, 46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 46.25, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 46.25, 32.5, 46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 46.25, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    'Landings
    External.AddFloor GetTex("BrickTexture"), -150, -93.75, -115, 93.75, (40 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 150, -93.75, 115, 93.75, (40 * 25) + 25, 10, 10
    
Print "Processing Floors 40 to 79"
    'Floors 40 to 79 (minus 14 feet on both sides where 20=8 feet)
    For I = 40 To 79
    'Mesh.AddFloor GetTex("Granite"), -115, -93.75, 115, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("Granite"), -115, -93.75, 115, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -115, 30.83, 115, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -115, -46.25, -32.5, 30.83, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), 32.5, -46.25, 115, 30.83, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 30.83, (I * 25) + 25, 1, 3
    
    Room.AddFloor GetTex("Marble3"), -115, -93.75, 115, -46.25, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -115, 30.83, 115, 93.75, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -115, -46.25, -32.5, 30.83, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 115, 30.83, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 30.83, (I * 25) + 49.5, 1, 3
   
    External.AddWall GetTex("MainWindows"), -115, -93.75, 115, -93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 115, -93.75, 115, 93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 115, 93.75, -115, 93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), -115, 93.75, -115, -93.75, 25, (I * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (I * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 30.83, -32.5, 30.83, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 30.83, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 30.83, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 30.83, 32.5, 30.83, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 30.83, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    External.AddFloor GetTex("BrickTexture"), -115, -93.75, -90, 93.75, (80 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 115, -93.75, 90, 93.75, (80 * 25) + 25, 10, 10
    
Print "Processing Floors 80 to 117"
    'Floors 80 to 117
    For I = 80 To 117
    'Mesh.AddFloor GetTex("Granite"), -90, -93.75, 90, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("Granite"), -90, -93.75, 90, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -90, 9, 90, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -90, -46.25, -32.5, 9, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), 32.5, -46.25, 90, 9, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 9, (I * 25) + 25, 1, 3
    
    Room.AddFloor GetTex("Marble3"), -90, -93.75, 90, -46.25, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -90, 9, 90, 93.75, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -90, -46.25, -32.5, 9, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 90, 9, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 9, (I * 25) + 49.5, 1, 3
    
    External.AddWall GetTex("MainWindows"), -90, -93.75, 90, -93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 90, -93.75, 90, 93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 90, 93.75, -90, 93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), -90, 93.75, -90, -93.75, 25, (I * 25) + 25, 5, 1
    
    'Elevators
    
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (I * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 15.41, -32.5, 15.41, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 15.41, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 15.41, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 15.41, 32.5, 15.41, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 15.41, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    External.AddFloor GetTex("BrickTexture"), -90, -93.75, -65, 93.75, (118 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 90, -93.75, 65, 93.75, (118 * 25) + 25, 10, 10
    
Print "Processing Floors 118 to 134"
    'Floors 118 to 134 (minus 10 feet)
    For I = 118 To 134
    'Mesh.AddFloor GetTex("Granite"), -65, -93.75, 65, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("Granite"), -65, -93.75, 65, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -65, 0, 65, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("Granite"), -65, -46.25, -32.5, 0, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), 32.5, -46.25, 65, 0, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (I * 25) + 25, 1, 3
    
    Room.AddFloor GetTex("Marble3"), -65, -93.75, 65, -46.25, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -65, 0, 65, 93.75, (I * 25) + 49.5, 10, 3
    Room.AddFloor GetTex("Marble3"), -65, -46.25, -32.5, 0, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 65, 0, (I * 25) + 49.5, 3, 3
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (I * 25) + 49.5, 1, 3
     
    External.AddWall GetTex("MainWindows"), -65, -93.75, 65, -93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 65, -93.75, 65, 93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 65, 93.75, -65, 93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -65, 93.75, -65, -93.75, 25, (I * 25) + 25, 4, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (I * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I

    External.AddFloor GetTex("BrickTexture"), -65, -93.75, -50, 93.75, (135 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 65, -93.75, 50, 93.75, (135 * 25) + 25, 10, 10
    
Print "Processing Floors 135 to 137"
    'Floors 135 to 137 (minus 6 feet)
    For I = 135 To 137
    'Mesh.AddFloor GetTex("Granite"), -50, -93.75, 50, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("Granite"), -50, -93.75, 50, -46.25, (I * 25) + 25, 8, 3.8
    Room.AddFloor GetTex("Granite"), -50, -15.42, 50, 93.75, (I * 25) + 25, 8, 8.6
    Room.AddFloor GetTex("Granite"), -50, -46.25, -32.5, -15.42, (I * 25) + 25, 1.4, 2.4
    Room.AddFloor GetTex("Granite"), 32.5, -46.25, 50, -15.42, (I * 25) + 25, 1.4, 2.4
    Room.AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (I * 25) + 25, 2, 2.4
    
    Room.AddFloor GetTex("Marble3"), -50, -93.75, 50, -46.25, (I * 25) + 49.5, 8, 3.8
    Room.AddFloor GetTex("Marble3"), -50, -15.42, 50, 93.75, (I * 25) + 49.5, 8, 8.6
    Room.AddFloor GetTex("Marble3"), -50, -46.25, -32.5, -15.42, (I * 25) + 49.5, 1.4, 2.4
    Room.AddFloor GetTex("Marble3"), 32.5, -46.25, 50, -15.42, (I * 25) + 49.5, 1.4, 2.4
    Room.AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (I * 25) + 49.5, 2, 2.4
    
    External.AddWall GetTex("MainWindows"), -50, -93.75, 50, -93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 50, -93.75, 50, 93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 50, 93.75, -50, 93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -50, 93.75, -50, -93.75, 25, (I * 25) + 25, 3, 1
    
    'Elevators
    Shafts.AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -16, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (I * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("Wall1"), -12.5, -15.42, -32.5, -15.42, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), -32.5, -15.42, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -15.42, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("Wall1"), 12.5, -15.42, 32.5, -15.42, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("Wall1"), 32.5, -15.42, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I

    Shafts.AddWall GetTex("BrickTexture"), -12.5, -30.85, -32.5, -30.85, (25 * 138) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -30.85, 32.5, -30.85, (25 * 138) + 25, 1, 2
        
    Shafts.AddFloor GetTex("BrickTexture"), -50, -93.75, 50, 93.75, (138 * 25) + 25, 10, 10
      
    'Elevator1
    'Old one Elevator1.AddFloor GetTex("BrickTexture"), -32, -30, -12, -16, 1
    Elevator1.AddFloor GetTex("Wood2"), -28, -30, -12.5, -16, 0.1, 1, 1
    Elevator1.AddFloor GetTex("Elev1"), -28, -30, -12.5, -16, 19.5, 2, 2
    Elevator1.AddWall GetTex("Wood1"), -28, -30, -12.5, -30, 19.5, 0.1, 2, 2
    'Elevator1.AddWall GetTex("BrickTexture"), -12, -30, -12, -16, 19.5
    Elevator1.AddWall GetTex("Wood1"), -28, -16, -12.5, -16, 19.5, 0.1, 2, 2
    Elevator1.AddWall GetTex("Wood1"), -28, -16, -28, -30, 19.5, 0.1, 2, 2
2
    '4 : Set the camera startpos
    'Camera.SetCameraPosition 0, 10, 0
    Camera.SetPosition 0, 10, 90
    
    'Camera.SetCameraLookAt 0, 10, 1
    Camera.SetLookAt 0, 10, -90
    
  'Room.Enable False
  'External.Enable False
  'Shafts.Enable False
  'Elevator1.Enable False
  
  Dim stuff As Boolean
  Do Until isRunning = False Or Inp.IsKeyPressed(TV_KEY_ESCAPE) = True

    linestart = Camera.GetPosition
    elevator1start = Elevator1.GetPosition

    TV.Clear
    
   If Inp.IsKeyPressed(TV_KEY_UP) = True Then Camera.MoveRelative 0.7, 0, 0
      If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then Camera.MoveRelative -0.7, 0, 0
      If Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then Camera.RotateY 0.05
      If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then Camera.RotateY -0.05
      
      If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True Then Camera.RotateX -0.006
      If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True Then Camera.RotateX 0.006
      
      If Inp.IsKeyPressed(TV_KEY_HOME) = True Then Camera.MoveRelative 0, 1, 0
      If Inp.IsKeyPressed(TV_KEY_END) = True Then Camera.MoveRelative 0, -1, 0
      If Inp.IsKeyPressed(TV_KEY_1) = True Then ElevatorDirection = 1
      If Inp.IsKeyPressed(TV_KEY_2) = True Then ElevatorDirection = -1
      
      If Inp.IsKeyPressed(TV_KEY_F1) = True Then TV.ScreenShot ("c:\shot.bmp")
      
      If GotoFloor <> 0 And GotoFloor > CurrentFloor And ElevatorDirection = 0 Then
      ElevatorDirection = 1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((GotoFloor * 25) + 25) - ((CurrentFloorExact * 25) + 25)
      End If
      If GotoFloor <> 0 And GotoFloor < CurrentFloor And ElevatorDirection = 0 Then
      ElevatorDirection = -1
      OriginalLocation = CurrentFloorExact
      DistanceToTravel = ((CurrentFloorExact * 25) + 25) - ((GotoFloor * 25) + 25)
      End If
      
       CurrentFloor = Int((elevator1start.y - 25) / 25)
       CurrentFloorExact = (elevator1start.y - 25) / 25
       
       Form2.Text1.Text = "ElevatorEnable=" + Str$(ElevatorEnable) + vbCrLf + "ElevatorDirection=" + Str$(ElevatorDirection) + vbCrLf + "Floor=" + Str$(CurrentFloor) + vbCrLf + "Current Location+" + Str$(elevator1start.y) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable / 5)
        
        If ElevatorEnable >= 0 And ElevatorDirection = 1 Then
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable <= 15 Then StoppingDistance = CurrentFloorExact - OriginalLocation
        If ElevatorEnable > 15 Then ElevatorEnable = 15
        Destination = ((OriginalLocation * 25) + 25) + DistanceToTravel - 35
        If GotoFloor <> 0 And elevator1start.y >= (Destination - (StoppingDistance * 25) + 25) Then ElevatorDirection = -1
        End If
      
        If ElevatorEnable > 0 And ElevatorDirection = -1 Then
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable < 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then FineTune = True
        End If
      
        If ElevatorEnable <= 0 And ElevatorDirection = -1 Then
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        ElevatorEnable = ElevatorEnable - 0.25
        If ElevatorEnable >= -15 Then StoppingDistance = OriginalLocation - CurrentFloorExact
        If ElevatorEnable < -15 Then ElevatorEnable = -15
        Destination = ((OriginalLocation * 25) + 25) - DistanceToTravel
        If GotoFloor <> 0 And elevator1start.y <= (Destination + (StoppingDistance * 25) + 25) Then ElevatorDirection = 1
        End If
      
        If ElevatorEnable < 0 And ElevatorDirection = 1 Then
        Elevator1.MoveRelative 0, (ElevatorEnable / 5), 0
        If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, (ElevatorEnable / 5), 0
        ElevatorEnable = ElevatorEnable + 0.25
        If ElevatorEnable > 0 Then ElevatorEnable = 0
        If ElevatorEnable = 0 Then ElevatorDirection = 0
        If GotoFloor <> 0 Then FineTune = True
        End If
      
      If FineTune = True And ElevatorEnable = 0 And elevator1start.y > (GotoFloor * 25) + 25 + -0.3 And elevator1start.y < (GotoFloor * 25) + 25 + 0.3 Then FineTune = False: GotoFloor = 0
      If FineTune = True Then
      If elevator1start.y < (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, 0.2, 0
      If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevator1start.y > (GotoFloor * 25) + 25 Then
      Elevator1.MoveRelative 0, -0.2, 0
      If Form2.Check1.Value = 1 Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      lineend = Camera.GetPosition
    
    'If Elevator1.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
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
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
isRunning = False
End Sub
