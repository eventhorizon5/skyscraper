VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Skyscraper 0.2 Alpha"
   ClientHeight    =   4485
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6030
   LinkTopic       =   "Form1"
   ScaleHeight     =   4485
   ScaleWidth      =   6030
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skyscraper 0.2 Alpha

' General declarations :
Public TV As TVEngine
Public Scene As TVScene
'Public Mesh As TVMesh
Public TextureFactory As TVTextureFactory
Public Inp As TVInputEngine
Public Camera As TVCamera

Dim linestart As D3DVECTOR
Dim lineend As D3DVECTOR
Dim ColRes As TV_COLLISIONRESULT

'Our mesh will be stored in a Mesh8 class.
'Note that Mesh objects must be created by
'the Scene8 class
Private Room As TVMesh
Private Elevator1 As TVMesh
Private Shafts As TVMesh
Private External As TVMesh

Public ElevatorUp As Boolean







Private Sub Form_Load()
    '1. Show the form in that the scene will be rendered.
    Set TV = New TVEngine
    Set Scene = New TVScene
    Set Camera = New TVCamera
Set Room = New TVMesh
Set Elevator1 = New TVMesh
Set Shafts = New TVMesh
Set External = New TVMesh

    Set TextureFactory = New TVTextureFactory

    Form1.Show
    
      If TV.ShowDriverDialog = False Then End
   DoEvents
   '2. Initialize the engine with the selected mode
   'TV.SetSearchDirectory App.Path
   TV.Initialize Form1.hWnd
   
  Set Inp = New TVInputEngine
  TV.SetSearchDirectory App.Path
  TV.DisplayFPS = True
  Print "Please Wait..";
  Print "."
    
   '2. Initialize the engine in windowed mode.
    '   Note that you have to use the Hwnd of the desired form or picturebox
    TV.Init3DWindowedMode Form1.hWnd
    'TV.Init3DFullscreen 640, 480, 16
    '2b. Defines the data directory. Usually it's the application path
    
    '3. Create the room
    '3a : Create the mesh from the scene object
    Set Room = Scene.CreateMeshBuilder("Room")
    Set Elevator1 = Scene.CreateMeshBuilder
    Set Shafts = Scene.CreateMeshBuilder
    Set External = Scene.CreateMeshBuilder
    
    '3d : Load a brick texture. Note that TrueVision can load
    '     BMP, JPG, DDS, PNG  , GIF picture files.
    TextureFactory.LoadTexture "brick1.bmp", "BrickTexture"
    TextureFactory.LoadTexture "LobbyFront.jpg", "LobbyFront"
    TextureFactory.LoadTexture "windows11c.jpg", "MainWindows"
    
    '3b : Add the walls to the room using the Mesh8.AddWall method
    '     We will set the texture later...
    Room.AddWall GetTex("BrickTexture"), -10, -10, 10, -10, 20
    Room.AddWall GetTex("BrickTexture"), 10, -10, 10, 10, 20
    Room.AddWall GetTex("BrickTexture"), 10, 10, -10, 10, 20
    Room.AddWall GetTex("BrickTexture"), -10, 10, -10, -10, 20
    'GoTo 2
    'Elevator shafts are 19 feet in from the north, 51 feet in from the west and are 8 feet wide
    'Lobby
    Room.AddFloor GetTex("BrickTexture"), -150, -93.75, 150, 93.75, 0, 10, 10
    
    External.AddWall GetTex("LobbyFront"), -150, -93.75, 150, -93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 150, -93.75, 150, 93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 150, 93.75, -150, 93.75, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -150, 93.75, -150, -93.75, 75, 0, 3, 1
          
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 75, 0, 1, 6
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 46.25, 75, 0, 3, 6
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 46.25, -32.5, 46.25, 75, 0, 1, 6
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 46.25, -32.5, -46.25, 75, 0, 3, 6
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 75, 0, 1, 6
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 46.25, 75, 0, 3, 6
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 46.25, 32.5, 46.25, 75, 0, 1, 6
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 46.25, 32.5, -46.25, 75, 0, 3, 6
          
    'Floors 2 to 39
    For I = 2 To 39
    'Room.AddFloor GetTex("BrickTexture"), -150, -93.75, 150, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("BrickTexture"), -150, -93.75, 150, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -150, 46.25, 150, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -150, -46.25, -32.5, 46.25, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), 32.5, -46.25, 150, 46.25, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (I * 25) + 25, 1, 3
    
    External.AddWall GetTex("MainWindows"), -150, -93.75, 150, -93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), 150, -93.75, 150, 93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), 150, 93.75, -150, 93.75, 25, (I * 25) + 25, 9, 1
    External.AddWall GetTex("MainWindows"), -150, 93.75, -150, -93.75, 25, (I * 25) + 25, 9, 1
    
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 46.25, -32.5, 46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 46.25, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 46.25, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 46.25, 32.5, 46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 46.25, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    'Landings
    External.AddFloor GetTex("BrickTexture"), -150, -93.75, -115, 93.75, (40 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 150, -93.75, 115, 93.75, (40 * 25) + 25, 10, 10
    
    'Floors 40 to 79 (minus 14 feet on both sides where 20=8 feet)
    For I = 40 To 79
    'Room.AddFloor GetTex("BrickTexture"), -115, -93.75, 115, 93.75, (I * 25) + 25, 10, 10
    
    Room.AddFloor GetTex("BrickTexture"), -115, -93.75, 115, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -115, 30.83, 115, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -115, -46.25, -32.5, 30.83, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), 32.5, -46.25, 115, 30.83, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 30.83, (I * 25) + 25, 1, 3
    
    External.AddWall GetTex("MainWindows"), -115, -93.75, 115, -93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 115, -93.75, 115, 93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 115, 93.75, -115, 93.75, 25, (I * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), -115, 93.75, -115, -93.75, 25, (I * 25) + 25, 7, 1
    
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 30.83, -32.5, 30.83, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 30.83, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 30.83, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 30.83, 32.5, 30.83, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 30.83, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    External.AddFloor GetTex("BrickTexture"), -115, -93.75, -90, 93.75, (80 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 115, -93.75, 90, 93.75, (80 * 25) + 25, 10, 10
    
    'Floors 80 to 117
    For I = 80 To 117
    'Room.AddFloor GetTex("BrickTexture"), -90, -93.75, 90, 93.75, (I * 25) + 25, 10, 10
    
    Room.AddFloor GetTex("BrickTexture"), -90, -93.75, 90, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -90, 9, 90, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -90, -46.25, -32.5, 9, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), 32.5, -46.25, 90, 9, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 9, (I * 25) + 25, 1, 3
    
    External.AddWall GetTex("MainWindows"), -90, -93.75, 90, -93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 90, -93.75, 90, 93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 90, 93.75, -90, 93.75, 25, (I * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), -90, 93.75, -90, -93.75, 25, (I * 25) + 25, 5, 1
    
    'Elevators
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 15.41, -32.5, 15.41, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 15.41, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 15.41, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 15.41, 32.5, 15.41, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 15.41, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I
    
    External.AddFloor GetTex("BrickTexture"), -90, -93.75, -65, 93.75, (118 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 90, -93.75, 65, 93.75, (118 * 25) + 25, 10, 10
    
    'Floors 118 to 134 (minus 10 feet)
    For I = 118 To 134
    'Room.AddFloor GetTex("BrickTexture"), -65, -93.75, 65, 93.75, (I * 25) + 25, 10, 10
    
    Room.AddFloor GetTex("BrickTexture"), -65, -93.75, 65, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -65, 0, 65, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -65, -46.25, -32.5, 0, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), 32.5, -46.25, 65, 0, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (I * 25) + 25, 1, 3
    
    External.AddWall GetTex("MainWindows"), -65, -93.75, 65, -93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 65, -93.75, 65, 93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 65, 93.75, -65, 93.75, 25, (I * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -65, 93.75, -65, -93.75, 25, (I * 25) + 25, 4, 1
    
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, 0, -32.5, 0, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, 0, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, 0, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, 0, 32.5, 0, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, 0, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I

    External.AddFloor GetTex("BrickTexture"), -65, -93.75, -50, 93.75, (135 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 65, -93.75, 50, 93.75, (135 * 25) + 25, 10, 10
    
    'Floors 135 to 137 (minus 6 feet)
    For I = 135 To 137
    'Room.AddFloor GetTex("BrickTexture"), -50, -93.75, 50, 93.75, (I * 25) + 25, 10, 10
    Room.AddFloor GetTex("BrickTexture"), -50, -93.75, 50, -46.25, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -50, -15.42, 50, 93.75, (I * 25) + 25, 10, 3
    Room.AddFloor GetTex("BrickTexture"), -50, -46.25, -32.5, -15.42, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), 32.5, -46.25, 50, -15.42, (I * 25) + 25, 3, 3
    Room.AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (I * 25) + 25, 1, 3
    
    External.AddWall GetTex("MainWindows"), -50, -93.75, 50, -93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 50, -93.75, 50, 93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 50, 93.75, -50, 93.75, 25, (I * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -50, 93.75, -50, -93.75, 25, (I * 25) + 25, 3, 1
    
    'Elevators
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5, -15.42, -32.5, -15.42, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5, -15.42, -32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -46.25, 12.5, -46.25, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -46.25, 12.5, -15.42, 25, (I * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -15.42, 32.5, -15.42, 25, (I * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5, -15.42, 32.5, -46.25, 25, (I * 25) + 25, 3, 2
    
    Next I

    Shafts.AddWall GetTex("BrickTexture"), -12.5, -30.85, -32.5, -30.85, (25 * 138) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5, -30.85, 32.5, -30.85, (25 * 138) + 25, 1, 2
        
    External.AddFloor GetTex("BrickTexture"), -50, -93.75, 50, 93.75, (138 * 25) + 25, 10, 10
      
    'Elevator1
    Elevator1.AddFloor GetTex("BrickTexture"), -10, -10, 10, 10
    Elevator1.AddWall GetTex("BrickTexture"), -10, -10, 10, -10, 10
    Elevator1.AddWall GetTex("BrickTexture"), 10, -10, 10, 10, 10
    'Elevator1.AddWall GetTex("BrickTexture"), 10, 10, -10, 10, 10
    Elevator1.AddWall GetTex("BrickTexture"), -10, 10, -10, -10, 10
2
    '4 : Set the camera startpos
    'Camera.SetCameraPosition 0, 10, 0
    Camera.SetPosition 0, 10, 90
    
    'Camera.SetCameraLookAt 0, 10, 1
    Camera.SetLookAt 0, 10, 1
    
    'The main game loop :
    Do
      linestart = Camera.GetPosition

      'Clear the screen. You must do this before rendering anything on the screen.
      TV.Clear
      
      'Call the room rendering method
      'Room.enable False
      'Shafts.enable False
      'External.enable False
      'Room.Optimize
      'Shafts.Optimize
      'External.Optimize
      Scene.RenderAllMeshes
      
      'Render everything on the screen
      TV.RenderToScreen
      
      'Move the scene according to the keyboard presses.
      If Inp.IsKeyPressed(TV_KEY_UP) = True Then Camera.MoveRelative 1, 0, 0
      If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then Camera.MoveRelative -1, 0, 0
      If Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then Camera.RotateY 0.1
      If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then Camera.RotateY -0.1
      
      If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True Then Camera.RotateX -0.006
      If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True Then Camera.RotateX 0.006
      
      If Inp.IsKeyPressed(TV_KEY_HOME) = True Then Camera.MoveRelative 0, 1, 0
      If Inp.IsKeyPressed(TV_KEY_END) = True Then Camera.MoveRelative 0, -1, 0
    
      If Inp.IsKeyPressed(TV_KEY_TAB) = True Then ElevatorUp = True
      'Elevator1.SetVelocity 10, 10, 10
      If ElevatorUp = True Then Elevator1.MoveRelative 0, 0.5, 0
      
      lineend = Camera.GetPosition
    
    If Elevator1.Collision(linestart, lineend, TV_TESTTYPE_ACCURATETESTING) = True Then
        Camera.SetPosition linestart.x, linestart.y, linestart.z
    Else
        Camera.SetPosition lineend.x, lineend.y, lineend.z
    End If
    
    'Check the Escape Key that ends up the tutorial
    Loop Until Inp.IsKeyPressed(TV_KEY_ESCAPE) = True
    
    'Don't forget to shut down the engine
    Set TV = Nothing
    End
End Sub



