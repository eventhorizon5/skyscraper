Attribute VB_Name = "SimCore"
'Skyscraper 1.1 Alpha - Simulator core
'Copyright �2004 Ryan Thoryk
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
'
'2/15/03 - Code amount - 79 pages
'3/1/03 - Code amount - 129 pages
'4/26/03 - Code amount - 280 pages
'10/6/03 - Code amount - 297 pages

Option Explicit

'Truevision Engine
Public TV As TVEngine
Public Scene As TVScene
Public Effect As TVGraphicEffect
Public Light As TVLightEngine
Public MatFactory As New TVMaterialFactory
Public SoundEngine As TVSoundEngine
Public Listener As TVListener
Public ListenerDirection As D3DVECTOR
Public Atmos As New TVAtmosphere
Public Inp As TVInputEngine
Public TextureFactory As TVTextureFactory
Public Camera As TVCamera
Public MainMusic As TVSoundMP3

'Global Simulation Data
Public CameraDefAltitude As Single 'default vertical offset of camera
Public Gravity As Single
Public Dest As Object 'Output object for visual simulation, usually a form
Public isRunning As Boolean 'is sim engine running?
Public EnableCollisions As Boolean 'turns collisions on/off
Public ElevatorShafts As Integer 'number of elevator shafts
Public TotalFloors As Integer 'number of above-ground floors
Public StartFloor As Integer 'starting floor
Public Basements As Integer 'number of basement floors
Public Elevators As Integer 'number of elevators
Public PipeShafts As Integer 'number of pipe shafts
Public StairsNum As Integer 'number of stairwells
Public Floor() As FloorObject 'Floor data structure
Public Elevator() As ElevatorObject 'Elevator data structure
Public PipeShaft() As TVMesh 'pipe shaft object
Public Buildings As TVMesh 'external buildings
Public External As TVMesh 'external walls for building
Public Landscape As TVMesh 'outside landscape
Public IntroMesh As TVMesh 'introduction mesh
Public Buttons() As TVMesh 'elevator buttons
Public ElevatorSpeed As Single 'elevator speed
Public ElevatorFineTuneSpeed As Single 'elevator finetune speed

'External calls
Public Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Public Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)
Public Declare Function GetTickCount Lib "kernel32.dll" () As Long

Public Sub Start(FileName As String, InitTV As Boolean, DestObject As Object)
'Start simulator - this replaces old init_simulator function

'On Error GoTo ErrorHandler

'Set output object
Set Dest = DestObject
Dest.ScaleMode = 1

'Print banner
Dest.ForeColor = RGB(255, 255, 255)
Dest.BackColor = RGB(0, 0, 0)
Call PrintBanner
DoEvents

'Wait 2 seconds
Sleep 2000

Dest.Print Spc(2); "Initializing TrueVision3D..."
DoEvents

isRunning = True
EnableCollisions = True

'Initialize TrueVision engine objects if specified
If InitTV = True Then
    Set TV = New TVEngine
    Set Scene = New TVScene
    Set TextureFactory = New TVTextureFactory
    Set SoundEngine = New TV3DMedia.TVSoundEngine
    Set Inp = New TVInputEngine
End If

'create camera object
Set Camera = New TVCamera

'Create a light object
Set Light = New TVLightEngine

'create listener object
Set Listener = SoundEngine.Get3DListener

'Initialize Truevision
TV.SetSearchDirectory App.Path
'TV.Initialize Sim.hWnd
TV.Init3DWindowedMode Sim.hWnd
'TV.Init3DFullscreen 640, 480, 16

'Show FPS on screen
TV.DisplayFPS = True

'Set up 3D listener
Set Listener = SoundEngine.Get3DListener

'testing code
'TV.MultiSampleTp = TV3D_MULTISAMPLE_16_SAMPLES
'TV.EnableAntialising True

Scene.SetTextureFilter (TV_FILTER_ANISOTROPIC)
'Scene.SetDithering True
TV.EnableAntialising True

'load mouse cursor
Scene.LoadCursor App.Path + "\data\pointer.bmp", TV_COLORKEY_BLACK, 14, 16

Sleep 1000

'***************************
'* Load building data file *
'***************************
Call PrintBanner
Dest.Print Spc(2); "Processing Meshes..."

DoEvents

'These Triton calls will be replaced with routines
'to load and parse the building data file.
'This one loads the global parameters
Call Triton_Globals

Set IntroMesh = Scene.CreateMeshBuilder("IntroMesh")

Dim i As Integer

'level 0 is floor 1, level 1 is floor 2, etc)

'resize floor and elevator objects to numbers specified
ReDim Floor(-Basements To TotalFloors)
ReDim Elevator(Elevators)

For i = -Basements To TotalFloors
  DoEvents
  'create new floor
  Set Floor(i) = New FloorObject
  'run custom constructor
  Floor(i).Initialize (i)
Next i
  
For i = 1 To Elevators
  DoEvents
  'create new elevator
  Set Elevator(i) = New ElevatorObject
  'run custom constructor
  Elevator(i).Initialize (i)
Next i

ReDim PipeShaft(PipeShafts)

For i = 1 To PipeShafts
    DoEvents
    Set PipeShaft(i) = Scene.CreateMeshBuilder("PipeShaft " + Str$(i))
Next i

Set Buildings = Scene.CreateMeshBuilder("Buildings")
Set External = Scene.CreateMeshBuilder("External")
Set Landscape = Scene.CreateMeshBuilder("Landscape")
    
'Elevator Button Meshes
ReDim Buttons(-Basements - 1 To TotalFloors + 5)
For i = -Basements - 1 To TotalFloors + 5
    DoEvents
    Set Buttons(i) = Scene.CreateMeshBuilder("Buttons " + Str$(i))
Next i
  
Dest.Print Spc(2); "Loading Textures..."
DoEvents

'expand 3D view distance
Scene.SetViewFrustum 90, 200000
  
'load texture files
Call Triton_LoadTextures

Call Triton_ProcessBasement
Call Triton_ProcessFloors

Sim.MainTimer.Enabled = True

Exit Sub

ErrorHandler:
   MsgBox "An error occurred during the simulator initialization process." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End

End Sub

Private Sub PrintBanner()
Dest.Cls
Dest.Print vbCrLf
Dest.Print Spc(2); "Skyscraper " + LTrim(Str$(App.Major)) + "." + LTrim(Str$(App.Minor)) + " Alpha - Build" + Str$(App.Revision)
Dest.Print Spc(2); "Powered by SBSE " + LTrim(Str$(App.Major - 1)) + "." + LTrim(Str$(App.Minor))
Dest.Print Spc(2); "Compiled on July 21, 2004" + vbCrLf
Dest.Print Spc(2); "Skyscraper comes with ABSOLUTELY NO WARRANTY. This is free"
Dest.Print Spc(2); "software, and you are welcome to redistribute it under certain"
Dest.Print Spc(2); "conditions. For details, see the file gpl.txt"
Dest.Print Spc(2); "Build number counting has been done since version 0.7" + vbCrLf
Dest.Print Spc(2); "Skyscraper �2003-2004 Ryan Thoryk"
Dest.Print Spc(2); "Scalable Building Simulation Engine �2004 Ryan Thoryk" + vbCrLf

End Sub

Public Sub MainLoop()
Render:
    'If IntroOn = True Then Call Intro
    
    'On Error Resume Next
    TV.Clear
    Atmos.Atmosphere_Render
    Scene.RenderAllMeshes
    TV.RenderToScreen
    DoEvents

End Sub
