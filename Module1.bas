Attribute VB_Name = "Globals"
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

'2/15/03 - Code amount - 79 pages

'Building title - "Triton Center"
'

Global TV As TVEngine
Global Scene As TVScene
Global Mesh As TVMesh
Global External As TVMesh
Global Room(138) As TVMesh
Global Shafts As TVMesh
Global ShaftsFloor(138) As TVMesh
Global Buttons(-1 To 138) As TVMesh
Global Elevator1 As TVMesh
Global FloorIndicator1 As TVMesh
Global Elevator1DoorL As TVMesh
Global Elevator1DoorR As TVMesh
Global ElevatorDoorL(-1 To 138) As TVMesh
Global ElevatorDoorR(-1 To 138) As TVMesh
Global Stairs(138) As TVMesh
Global CallButtons As TVMesh
Global Light As TVLightEngine
Global MatFactory As New TVMaterialFactory
Global LightID As Integer
Global LightD As D3DLIGHT8
Global Plaque As TVMesh

Global SoundEngine As TVSoundEngine
Global ElevatorMusic As TVSoundWave3D
Global Elevator1Sounds As TVSoundWave3D
Global Listener As TVListener
Global ListenerDirection As D3DVECTOR
'Global MousePositionX As Long
'Global MousePositionY As Long
Global Focused As Boolean
Global LineTest As D3DVECTOR
Global LineTest2 As D3DVECTOR
Global KeepAltitude As Integer
Global ElevatorSync As Boolean

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
Global elevator1start As D3DVECTOR
Global isRunning As Boolean
Global Camera As TVCamera
Global ColRes As TV_COLLISIONRESULT
Global i As Single
Global j As Single
Global ElevatorEnable As Single
Global ElevatorDirection As Integer '1=up -1=down
Global OpenElevatorLoc As Single
Global ElevatorCheck As Integer
Global ElevatorCheck2 As Integer
Global ElevatorCheck3 As Integer
Global ElevatorCheck4 As Integer
Global X As Integer
Global CollisionResult As TVCollisionResult

Global GotoFloor As Single
Global CurrentFloor As Integer
Global CurrentFloorExact As Single
Global DistanceToTravel As Single
Global Destination As Single
Global OriginalLocation As Single
Global StoppingDistance As Single
Global FineTune As Boolean
Global OpenElevator As Integer '1=open -1=close
Global ElevatorFloor As Integer
Global CameraFloor As Integer
Global PartialFloor As Single
Global InStairwell As Boolean
Global FloorIndicator As String
Global ElevatorFloor2 As Integer
Global CameraFloor2 As Integer
Global FloorIndicatorPic As String
Global FloorIndicatorPicOld As String
'3D Objects
Global Objects(150 * 138) As TVMesh
Global FileName As String
Global OpeningDoor As Integer
Global ClosingDoor As Integer
Global DoorNumber As Integer
Global DoorRotated As Integer

Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Declare Sub Sleep Lib "kernel32.dll" (ByVal dwMilliseconds As Long)
Declare Function GetTickCount Lib "kernel32.dll" () As Long
