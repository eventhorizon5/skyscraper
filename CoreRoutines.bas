Attribute VB_Name = "CoreRoutines"
'Skycraper 0.9 Beta
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

'2/15/03 - Code amount - 79 pages
'3/1/03 - Code amount - 129 pages

'Building title - "Triton Center"
'
Option Explicit

Global TV As TVEngine
Global Scene As TVScene
Global Mesh As TVMesh
Global External As TVMesh
Global Room(138) As TVMesh
Global Shafts1(138) As TVMesh
Global Shafts2(138) As TVMesh
Global ShaftsFloor(138) As TVMesh
'Global Buttons(-1 To 138) As TVMesh
Global Buttons1(-1 To 138) As TVMesh '-update from Buttons
Global Buttons2(-1 To 138) As TVMesh '-update from Buttons
Global Buttons3(-1 To 138) As TVMesh '-update from Buttons
Global Buttons4(-1 To 138) As TVMesh '-update from Buttons
Global Buttons5(-1 To 138) As TVMesh '-update from Buttons
Global Buttons6(-1 To 138) As TVMesh '-update from Buttons
Global Buttons7(-1 To 138) As TVMesh '-update from Buttons
Global Buttons8(-1 To 138) As TVMesh '-update from Buttons
Global Buttons9(-1 To 138) As TVMesh '-update from Buttons
Global Buttons10(-1 To 138) As TVMesh '-update from Buttons
'Global Elevator1 As TVMesh
Global Elevator(10) As TVMesh '-update from Elevator1
'Global FloorIndicator1 As TVMesh
Global FloorIndicator(10) As TVMesh '-update from FloorIndicator1
'Global Elevator1DoorL As TVMesh
'Global Elevator1DoorR As TVMesh
Global ElevatorInsDoorL(10) As TVMesh '-update from Elevator1DoorL
Global ElevatorInsDoorR(10) As TVMesh '-update
'Global ElevatorDoorL(-1 To 138) As TVMesh
'Global ElevatorDoorR(-1 To 138) As TVMesh
Global ElevatorDoor1L(-1 To 138) As TVMesh '-update from ElevatorDoorL
Global ElevatorDoor1R(-1 To 138) As TVMesh '-update
Global ElevatorDoor2L(-1 To 138) As TVMesh '-update
Global ElevatorDoor2R(-1 To 138) As TVMesh '-update
Global ElevatorDoor3L(-1 To 138) As TVMesh '-update
Global ElevatorDoor3R(-1 To 138) As TVMesh '-update
Global ElevatorDoor4L(-1 To 138) As TVMesh '-update
Global ElevatorDoor4R(-1 To 138) As TVMesh '-update
Global ElevatorDoor5L(-1 To 138) As TVMesh '-update
Global ElevatorDoor5R(-1 To 138) As TVMesh '-update
Global ElevatorDoor6L(-1 To 138) As TVMesh '-update
Global ElevatorDoor6R(-1 To 138) As TVMesh '-update
Global ElevatorDoor7L(-1 To 138) As TVMesh '-update
Global ElevatorDoor7R(-1 To 138) As TVMesh '-update
Global ElevatorDoor8L(-1 To 138) As TVMesh '-update
Global ElevatorDoor8R(-1 To 138) As TVMesh '-update
Global ElevatorDoor9L(-1 To 138) As TVMesh '-update
Global ElevatorDoor9R(-1 To 138) As TVMesh '-update
Global ElevatorDoor10L(-1 To 138) As TVMesh '-update
Global ElevatorDoor10R(-1 To 138) As TVMesh '-update
Global Stairs(138) As TVMesh
Global CallButtons(10) As TVMesh
Global Light As TVLightEngine
Global MatFactory As New TVMaterialFactory
Global LightID As Integer
Global LightD As D3DLIGHT8
'Global Plaque As TVMesh
Global Plaque(10) As TVMesh '-update

Global SoundEngine As TVSoundEngine
'Global ElevatorMusic As TVSoundWave3D
Global ElevatorMusic(10) As TVSoundWave3D '-update
'Global Elevator1Sounds As TVSoundWave3D
Global ElevatorSounds(10) As TVSoundWave3D '-update from Elevator1Sounds
Global Listener As TVListener
Global ListenerDirection As D3DVECTOR
'Global MousePositionX As Long
'Global MousePositionY As Long
Global Focused As Boolean
Global LineTest As D3DVECTOR
Global LineTest2 As D3DVECTOR
Global KeepAltitude As Integer
'Global ElevatorSync As Boolean
Global ElevatorSync(10) As Boolean '-update

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
'Global elevator1start As D3DVECTOR
Global elevatorstart(10) As D3DVECTOR '-update from elevator1start
Global isRunning As Boolean
Global Camera As TVCamera
Global ColRes As TV_COLLISIONRESULT
Global i As Single
Global i50 As Single
Global i51 As Single
Global i52 As Single
Global i53 As Single
Global j As Single
Global j50 As Single
'Global ElevatorEnable As Single
Global ElevatorEnable(10) As Single '-update
'Global ElevatorDirection As Integer '1=up -1=down
Global ElevatorDirection(10) As Integer '1=up -1=down UPDATE
Global OpenElevatorLoc(10) As Single
Global ElevatorCheck(10) As Integer
Global ElevatorCheck2(10) As Integer
Global ElevatorCheck3(10) As Integer
Global ElevatorCheck4(10) As Integer
Global X As Integer
Global CollisionResult As TVCollisionResult

'Global GotoFloor As Single
Global GotoFloor(10) As Single '-update
Global CurrentFloor(10) As Integer
Global CurrentFloorExact(10) As Single
Global DistanceToTravel(10) As Single
Global Destination(10) As Single
Global OriginalLocation(10) As Single
Global StoppingDistance(10) As Single
Global FineTune(10) As Boolean
'Global OpenElevator As Integer '1=open -1=close
Global OpenElevator(10) As Integer '1=open -1=close UPDATE
'Global ElevatorFloor As Integer
Global ElevatorFloor(10) As Integer 'update
Global CameraFloor As Integer
Global CameraFloorExact As Single
Global PartialFloor As Single
Global InStairwell As Boolean
'Global FloorIndicator As String
Global FloorIndicatorText(10) As String 'update
Global ElevatorFloor2(10) As Integer
Global CameraFloor2 As Integer
Global FloorIndicatorPic(10) As String
Global FloorIndicatorPicOld(10) As String
'3D Objects
Global Objects(150 * 138) As TVMesh
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
Global FloorEnabled(138) As Boolean
Global InElevator As Boolean

Global tmpMouseX As Long, tmpMouseY As Long, tmpMouseB1 As Integer
Global EnableCollisions As Boolean
Global Gravity As Single
Global IsFalling As Boolean
Global lngOldTick As Long
Global FallRate As Single
Global CameraOriginalPos As Single
Sub CheckCollisions()
 'Main collision code
LineTest = lineend
 
 If lineend.X > linestart.X Then LineTest.X = lineend.X + 2
 If lineend.X < linestart.X Then LineTest.X = lineend.X - 2
 If lineend.z > linestart.z Then LineTest.z = lineend.z + 2
 If lineend.z < linestart.z Then LineTest.z = lineend.z - 2
    
'Turn on collisions
        Room(CameraFloor).SetCollisionEnable True
        External.SetCollisionEnable True
        Buildings.SetCollisionEnable True
        Shafts1(CameraFloor).SetCollisionEnable True
        Shafts2(CameraFloor).SetCollisionEnable True
        ShaftsFloor(CameraFloor).SetCollisionEnable True
        For i50 = 1 To 10
        Elevator(i50).SetCollisionEnable True
        ElevatorInsDoorL(i50).SetCollisionEnable True
        ElevatorInsDoorR(i50).SetCollisionEnable True
        Next i50
        If CameraFloor = 1 Then
        ElevatorDoor1L(-1).SetCollisionEnable True
        ElevatorDoor1R(-1).SetCollisionEnable True
        ElevatorDoor2L(-1).SetCollisionEnable True
        ElevatorDoor2R(-1).SetCollisionEnable True
        ElevatorDoor3L(-1).SetCollisionEnable True
        ElevatorDoor3R(-1).SetCollisionEnable True
        ElevatorDoor4L(-1).SetCollisionEnable True
        ElevatorDoor4R(-1).SetCollisionEnable True
        ElevatorDoor5L(-1).SetCollisionEnable True
        ElevatorDoor5R(-1).SetCollisionEnable True
        ElevatorDoor6L(-1).SetCollisionEnable True
        ElevatorDoor6R(-1).SetCollisionEnable True
        ElevatorDoor7L(-1).SetCollisionEnable True
        ElevatorDoor7R(-1).SetCollisionEnable True
        ElevatorDoor8L(-1).SetCollisionEnable True
        ElevatorDoor8R(-1).SetCollisionEnable True
        ElevatorDoor9L(-1).SetCollisionEnable True
        ElevatorDoor9R(-1).SetCollisionEnable True
        ElevatorDoor10L(-1).SetCollisionEnable True
        ElevatorDoor10R(-1).SetCollisionEnable True
        ElevatorDoor1L(0).SetCollisionEnable True
        ElevatorDoor1R(0).SetCollisionEnable True
        ElevatorDoor2L(0).SetCollisionEnable True
        ElevatorDoor2R(0).SetCollisionEnable True
        Else
        ElevatorDoor1L(CameraFloor).SetCollisionEnable True
        ElevatorDoor1R(CameraFloor).SetCollisionEnable True
        ElevatorDoor2L(CameraFloor).SetCollisionEnable True
        ElevatorDoor2R(CameraFloor).SetCollisionEnable True
        ElevatorDoor3L(CameraFloor).SetCollisionEnable True
        ElevatorDoor3R(CameraFloor).SetCollisionEnable True
        ElevatorDoor4L(CameraFloor).SetCollisionEnable True
        ElevatorDoor4R(CameraFloor).SetCollisionEnable True
        ElevatorDoor5L(CameraFloor).SetCollisionEnable True
        ElevatorDoor5R(CameraFloor).SetCollisionEnable True
        ElevatorDoor6L(CameraFloor).SetCollisionEnable True
        ElevatorDoor6R(CameraFloor).SetCollisionEnable True
        ElevatorDoor7L(CameraFloor).SetCollisionEnable True
        ElevatorDoor7R(CameraFloor).SetCollisionEnable True
        ElevatorDoor8L(CameraFloor).SetCollisionEnable True
        ElevatorDoor8R(CameraFloor).SetCollisionEnable True
        ElevatorDoor9L(CameraFloor).SetCollisionEnable True
        ElevatorDoor9R(CameraFloor).SetCollisionEnable True
        ElevatorDoor10L(CameraFloor).SetCollisionEnable True
        ElevatorDoor10R(CameraFloor).SetCollisionEnable True
        End If
        Stairs(CameraFloor).SetCollisionEnable True
        
        
        
 'Elevator Collision
 For i50 = 1 To 10
 
    If Elevator(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorInsDoorL(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorInsDoorR(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
        
 Next i50
    
 'Collision code for all other objects
    i50 = CameraFloor
    If CameraFloor = 1 Then i50 = -1
    If ElevatorDoor1L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor1R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor2L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor2R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor3L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor3R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor4L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor4R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor5L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor5R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor6L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor6R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor7L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor7R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor8L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor8R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor9L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor9R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor10L(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor10R(i50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If CameraFloor = 1 Then
    If ElevatorDoor1L(0).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor1R(0).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor2L(0).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ElevatorDoor2R(0).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    End If
    
    
    If External.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If Room(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If Stairs(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If Buildings.Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If Shafts1(CameraFloor).IsMeshEnabled = True Then If Shafts1(CameraFloor).IsMeshEnabled = True Then If Shafts1(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If Shafts2(CameraFloor).IsMeshEnabled = True Then If Shafts2(CameraFloor).IsMeshEnabled = True Then If Shafts2(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    If ShaftsFloor(CameraFloor).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
    
'On Error Resume Next
'For i50 = 1 To 150
'j50 = i50 + (150 * (CameraFloor - 1))
'If Objects(j50).IsMeshEnabled = True Then
'    Objects(j50).SetCollisionEnable True
'    MsgBox (j50)
'    If Objects(j50).Collision(linestart, LineTest, TV_TESTTYPE_ACCURATETESTING) = True Then Camera.SetPosition linestart.X, Camera.GetPosition.Y, linestart.z: GoTo CollisionEnd
'    Objects(j50).SetCollisionEnable False
'End If
'Next i50

CollisionEnd:

'Turn off collisions
        Room(CameraFloor).SetCollisionEnable False
        External.SetCollisionEnable False
        Buildings.SetCollisionEnable False
        Shafts1(CameraFloor).SetCollisionEnable False
        Shafts2(CameraFloor).SetCollisionEnable False
        ShaftsFloor(CameraFloor).SetCollisionEnable False
        For i50 = 1 To 10
        Elevator(i50).SetCollisionEnable False
        ElevatorInsDoorL(i50).SetCollisionEnable False
        ElevatorInsDoorR(i50).SetCollisionEnable False
        Next i50
        If CameraFloor = 1 Then
        ElevatorDoor1L(-1).SetCollisionEnable False
        ElevatorDoor1R(-1).SetCollisionEnable False
        ElevatorDoor2L(-1).SetCollisionEnable False
        ElevatorDoor2R(-1).SetCollisionEnable False
        ElevatorDoor3L(-1).SetCollisionEnable False
        ElevatorDoor3R(-1).SetCollisionEnable False
        ElevatorDoor4L(-1).SetCollisionEnable False
        ElevatorDoor4R(-1).SetCollisionEnable False
        ElevatorDoor5L(-1).SetCollisionEnable False
        ElevatorDoor5R(-1).SetCollisionEnable False
        ElevatorDoor6L(-1).SetCollisionEnable False
        ElevatorDoor6R(-1).SetCollisionEnable False
        ElevatorDoor7L(-1).SetCollisionEnable False
        ElevatorDoor7R(-1).SetCollisionEnable False
        ElevatorDoor8L(-1).SetCollisionEnable False
        ElevatorDoor8R(-1).SetCollisionEnable False
        ElevatorDoor9L(-1).SetCollisionEnable False
        ElevatorDoor9R(-1).SetCollisionEnable False
        ElevatorDoor10L(-1).SetCollisionEnable False
        ElevatorDoor10R(-1).SetCollisionEnable False
        ElevatorDoor1L(0).SetCollisionEnable False
        ElevatorDoor1R(0).SetCollisionEnable False
        ElevatorDoor2L(0).SetCollisionEnable False
        ElevatorDoor2R(0).SetCollisionEnable False
        Else
        ElevatorDoor1L(CameraFloor).SetCollisionEnable False
        ElevatorDoor1R(CameraFloor).SetCollisionEnable False
        ElevatorDoor2L(CameraFloor).SetCollisionEnable False
        ElevatorDoor2R(CameraFloor).SetCollisionEnable False
        ElevatorDoor3L(CameraFloor).SetCollisionEnable False
        ElevatorDoor3R(CameraFloor).SetCollisionEnable False
        ElevatorDoor4L(CameraFloor).SetCollisionEnable False
        ElevatorDoor4R(CameraFloor).SetCollisionEnable False
        ElevatorDoor5L(CameraFloor).SetCollisionEnable False
        ElevatorDoor5R(CameraFloor).SetCollisionEnable False
        ElevatorDoor6L(CameraFloor).SetCollisionEnable False
        ElevatorDoor6R(CameraFloor).SetCollisionEnable False
        ElevatorDoor7L(CameraFloor).SetCollisionEnable False
        ElevatorDoor7R(CameraFloor).SetCollisionEnable False
        ElevatorDoor8L(CameraFloor).SetCollisionEnable False
        ElevatorDoor8R(CameraFloor).SetCollisionEnable False
        ElevatorDoor9L(CameraFloor).SetCollisionEnable False
        ElevatorDoor9R(CameraFloor).SetCollisionEnable False
        ElevatorDoor10L(CameraFloor).SetCollisionEnable False
        ElevatorDoor10R(CameraFloor).SetCollisionEnable False
        End If
        Stairs(CameraFloor).SetCollisionEnable False

End Sub

Sub DrawElevatorWalls(FloorID As Integer, SectionNum As Integer, OpenShaft As Boolean, e1 As Boolean, e2 As Boolean, e3 As Boolean, e4 As Boolean, e5 As Boolean, e6 As Boolean, e7 As Boolean, e8 As Boolean, e9 As Boolean, e10 As Boolean)

Dim q
'sectionnum:
'1 = lobby
'2 = 2-39
'3 = 40-79
'4 = 80-117
'5 = 118-134
'6 = 135-136
'7 = 137-138

Dim TextureName As String
Dim ShaftEnd As Single

If SectionNum = 1 Then TextureName = "Wall2"
If SectionNum > 1 And SectionNum < 7 Then TextureName = "Wall1"
If SectionNum = 7 Then TextureName = "BrickTexture"

q = FloorID
If FloorID = 1 Then q = -1

'Call Button Panels
    If e1 = True Then CallButtons(1).AddWall GetTex("CallButtonsTex"), -12.49, -18, -12.49, -17, 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e1 = True And q = -1 Then CallButtons(1).AddWall GetTex("CallButtonsTex"), -12.49, -18, -12.49, -17, 1.5, 8.5 + (0 * 25) + 25, 1, 1
    If e2 = True Then CallButtons(2).AddWall GetTex("CallButtonsTex"), 12.49, -29, 12.49, -28, 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e2 = True And q = -1 Then CallButtons(2).AddWall GetTex("CallButtonsTex"), 12.49, -29, 12.49, -28, 1.5, 8.5 + (0 * 25) + 25, 1, 1
    If e3 = True Then CallButtons(3).AddWall GetTex("CallButtonsTex"), -12.49, -18 + (15 * 1), -12.49, -17 + (15 * 1), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e4 = True Then CallButtons(4).AddWall GetTex("CallButtonsTex"), 12.49, -29 + (15 * 1), 12.49, -28 + (15 * 1), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e5 = True Then CallButtons(5).AddWall GetTex("CallButtonsTex"), -12.49, -18 + (15 * 2), -12.49, -17 + (15 * 2), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e6 = True Then CallButtons(6).AddWall GetTex("CallButtonsTex"), 12.49, -29 + (15 * 2), 12.49, -28 + (15 * 2), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e7 = True Then CallButtons(7).AddWall GetTex("CallButtonsTex"), -12.49, -18 + (15 * 3), -12.49, -17 + (15 * 3), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e8 = True Then CallButtons(8).AddWall GetTex("CallButtonsTex"), 12.49, -29 + (15 * 3), 12.49, -28 + (15 * 3), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e9 = True Then CallButtons(9).AddWall GetTex("CallButtonsTex"), -12.49, -18 + (15 * 4), -12.49, -17 + (15 * 4), 1.5, 8.5 + (q * 25) + 25, 1, 1
    If e10 = True Then CallButtons(10).AddWall GetTex("CallButtonsTex"), 12.49, -29 + (15 * 4), 12.49, -28 + (15 * 4), 1.5, 8.5 + (q * 25) + 25, 1, 1
 
        'wall left of stairs
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -12.5, -40.3, 19.5, (q * 25) + 25, (6 * 0.086), (19.5 * 0.08)
        'wall between stairs and 1st elevator
        If e1 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -32.5, -12.5, -30, 19.5, (q * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -32.5, -12.5 - 0.05, -30.5, 19.5, (q * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
        Else
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -32.5, -12.5, -16, 19.5, (q * 25) + 25, (16.5 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -32.5, -12.5 - 0.05, -16, 19.5, (q * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
        End If
        'wall on left, after 2nd elevator
        If e2 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, -30, 19.5, (q * 25) + 25, (16.25 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, -30, 19.5, (q * 25) + 25, (16.25 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 7 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, -16, 19.5, (q * 25) + 25, (30 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, -16, 19.5, (q * 25) + 25, (30 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 1st and 3rd elevator
        If e3 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -16, -12.5, -15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -16, -12.5 - 0.05, -15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 6 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -16, -12.5, -1, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -16, -12.5 - 0.05, -1, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 3rd and 5th elevator
        If e5 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -1, -12.5, 0, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -1, -12.5 - 0.05, 0, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 5 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -1, -12.5, 14, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex(TextureName), -12.5 - 0.05, -1, -12.5 - 0.05, 14, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 5th and 7th elevator
        If e7 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 14, -12.5, 15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 14, -12.5 - 0.05, 15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 14, -12.5, 29, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 14, -12.5 - 0.05, 29, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 7th and 9th elevator
        If e9 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 29, -12.5, 30, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 29, -12.5 - 0.05, 30, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 3 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 29, -12.5, 44, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 29, -12.5 - 0.05, 44, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 2nd and 4th elevator
        If e4 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -16, 12.5, -15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -16, 12.5 + 0.05, -15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 6 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -16, 12.5, -1, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -16, 12.5 + 0.05, -1, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 4th and 6th elevator
        If e6 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -1, 12.5, 0, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -1, 12.5 + 0.05, 0, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 5 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -1, 12.5, 14, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -1, 12.5 + 0.05, 14, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 6th and 8th elevator
        If e8 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 14, 12.5, 15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 14, 12.5 + 0.05, 15, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 14, 12.5, 29, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 14, 12.5 + 0.05, 29, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 8th and 10th elevator
        If e10 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 29, 12.5, 30, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 29, 12.5 + 0.05, 30, 19.5, (q * 25) + 25, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 3 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 29, 12.5, 44, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 29, 12.5 + 0.05, 44, 19.5, (q * 25) + 25, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        
        'Mezzanine stuff
        If FloorID = 1 Then
            'right side
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -12.5, -40.3, 19.5, 27, (6 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -32.5, -12.5, -30, 19.5, 27, (2.5 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -16, -12.5, 46.25, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -12.5, 46.25, 3.5, 27 + 19.5, (92.5 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -32.5, -12.5 - 0.05, -30.5, 19.5, 27, (2.5 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -16, -12.5 - 0.05, 46.25, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -30.5, -12.5 - 0.05, 46.25, 3.5, 27 + 19.5, (76.25 * 0.086), (19.5 * 0.08)
            'left side
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, -30, 19.5, 27, (16.25 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -16, 12.5, 46.25, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, 46.25, 3.5, 27 + 19.5, (92.5 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -16, 12.5 + 0.05, 46.25, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, 46.25, 3.5, 27 + 19.5, (92.5 * 0.086), (19.5 * 0.08)
        End If
        
        If SectionNum <= 2 Then ShaftEnd = 46.25
        If SectionNum = 3 Then ShaftEnd = 30.83
        If SectionNum = 4 Then ShaftEnd = 15.41
        If SectionNum = 5 Then ShaftEnd = 0
        If SectionNum >= 6 Then ShaftEnd = -15.42
        
        'this section places any shaft floors/ceilings that need to be made
        If FloorID = 1 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, -30, -32.5, 46.25, 0.05, (20 * 0.086), (46 * 0.08)
        Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, -30, 32.5, 46.25, 0.05, (20 * 0.086), (46 * 0.08)
        End If
        If FloorID = 39 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, 30.83, -32.5, 46.25, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, 30.83, 32.5, 46.25, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        End If
        If FloorID = 79 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, 15.41, -32.5, 30.83, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, 15.41, 32.5, 30.83, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        End If
        If FloorID = 117 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, 0, -32.5, 15.41, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, 0, 32.5, 15.41, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        End If
        If FloorID = 134 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, -15.42, -32.5, 0, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, -15.42, 32.5, 0, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        End If
        If FloorID = 138 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -12.5, -30, -32.5, -15.42, (FloorID * 25) + 25 + 24.95, (20 * 0.086), (16 * 0.08)
        'Shafts2(FloorID).AddFloor GetTex("BrickTexture"), 12.5, -30, 32.5, -15.42, (FloorID*25)+25+24.95, (20 * 0.086), (16 * 0.08)
        End If
        
        'this section places the wall that extends to the end of the shaft, according to where the last elevator is
        If SectionNum = 7 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -16, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -16, -12.5 - 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 6 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -16, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -16, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -16, -12.5 - 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -16, 12.5 + 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 5 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -1, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((12.5 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -1, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-12.5 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5, -1, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((12.5 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5, -1, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-12.5 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 4 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 14, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 14, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 14, -12.5 - 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 14, 12.5 + 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 3 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 29, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 29, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 29, -12.5 - 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 29, 12.5 + 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum <= 2 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 44, -12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 44, 12.5, ShaftEnd, 19.5, (q * 25) + 25, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 44, -12.5 - 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, 44, 12.5 + 0.05, ShaftEnd, 19.5, (q * 25) + 25, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        
        'walls above
        If SectionNum <> 1 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -12.5, ShaftEnd, 5.5, 19.5 + (q * 25) + 25, ((46.25 + ShaftEnd) * 0.086), (5.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -30.5, -12.5 - 0.05, ShaftEnd, 5.5, 19.5 + (q * 25) + 25, ((30 + ShaftEnd) * 0.086), (5.5 * 0.08)
            If SectionNum <> 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, ShaftEnd, 5.5, 19.5 + (q * 25) + 25, ((46.25 + ShaftEnd) * 0.086), (5.5 * 0.08)
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, ShaftEnd, 5.5, 19.5 + (q * 25) + 25, ((46.25 + ShaftEnd) * 0.086), (5.5 * 0.08)
            End If
        End If
        If SectionNum = 1 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -12.5, 46.25, 7.5, 19.5 + (q * 25) + 25, 92.5 * 0.086, (7.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, 46.25, 7.5, 19.5 + (q * 25) + 25, 92.5 * 0.086, (7.5 * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, -30.5, -12.5 - 0.05, 46.25, 7.5, 19.5 + (q * 25) + 25, 76.25 * 0.086, (7.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, 46.25, 7.5, 19.5 + (q * 25) + 25, 92.5 * 0.086, (7.5 * 0.08)
        End If
    'other parts
    
    If OpenShaft = True Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, -46.25, 32.5, -43, 25, (q * 25) + 25, 3.25 * 0.086, 2
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, -46.25, 32.5, -34, 10, (q * 25) + 25 + 15, 12.25 * 0.086, 10 * 0.08
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, -46.25, 32.5, -34, 8, (q * 25) + 25, 12.25 * 0.086, 8 * 0.08
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, -46.25, 32.5 - 0.05, -43, 25, (q * 25) + 25, 3.25 * 0.086, 2
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, -46.25, 32.5 - 0.05, -34, 10, (q * 25) + 25 + 15, 12.25 * 0.086, 10 * 0.08
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, -46.25, 32.5 - 0.05, -34, 8, (q * 25) + 25, 12.25 * 0.086, 8 * 0.08
    End If
    
    If SectionNum <> 7 Then
    ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, -46.25, 12.5, -46.25, 25, (q * 25) + 25, 20 * 0.086, 2
    Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5, -46.25 + 0.05, 12.5, -46.25 + 0.05, 25, (q * 25) + 25, 20 * 0.086, 2
    End If
    
    ShaftsFloor(FloorID).AddWall GetTex(TextureName), -32.5, -46.25, -12.5, -46.25, 25, (q * 25) + 25, 20 * 0.086, 2
    Shafts1(FloorID).AddWall GetTex("BrickTexture"), -32.5, -30 + 0.05, -12.5, -30 + 0.05, 25, (q * 25) + 25, 20 * 0.086, 2
            
            If SectionNum = 1 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 12.5, 46.25, 25, 50, 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 46.25, -12.5, -46.25, 25, 50, 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, -46.25, 32.5, -46.25, 50, 25, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, 46.25, 32.5, 46.25, 50, 25, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, 46.25, 32.5, -46.25, 50, 25, 92.5 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, -46.25, -32.5, -46.25, 50, 25, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, 46.25, -32.5, 46.25, 50, 25, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -32.5, 46.25, -32.5, -46.25, 50, 25, 20 * 0.086, 4
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5 + 0.05, -46.25, 12.5 + 0.05, 46.25, 25, 50, 92.5 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5 - 0.05, 46.25, -12.5 - 0.05, -30.5, 25, 50, 16.25 * 0.086, 2
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5, -46.25 + 0.05, 32.5, -46.25 + 0.05, 50, 25, 20 * 0.086, 4
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5, 46.25 - 0.05, 32.5, 46.25 - 0.05, 50, 25, 20 * 0.086, 4
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, 46.25, 32.5 - 0.05, -46.25, 50, 25, 92.5 * 0.086, 4
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5, -30 + 0.05, -32.5, -30 + 0.05, 50, 25, 20 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5, 46.25 - 0.05, -32.5, 46.25 - 0.05, 50, 25, 20 * 0.086, 4
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -32.5 + 0.05, 46.25, -32.5 + 0.05, -30.5, 50, 25, 16.25 * 0.086, 4
            End If
                
            'Main shaft area
            If SectionNum <> 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 12.5, ShaftEnd, 32.5, ShaftEnd, 25, (q * 25) + 25, 20 * 0.086, 2
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 12.5, ShaftEnd - 0.05, 32.5, ShaftEnd - 0.05, 25, (q * 25) + 25, 20 * 0.086, 2
                If OpenShaft = False Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, ShaftEnd, 32.5, -46.25, 25, (q * 25) + 25, (46.25 + ShaftEnd) * 0.086, 2
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, ShaftEnd, 32.5 - 0.05, -46.25, 25, (q * 25) + 25, (46.25 + ShaftEnd) * 0.086, 2
                End If
                If OpenShaft = True Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), 32.5, ShaftEnd, 32.5, -34, 25, (q * 25) + 25, (34 + ShaftEnd) * 0.086, 2
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), 32.5 - 0.05, ShaftEnd, 32.5 - 0.05, -34, 25, (q * 25) + 25, (34 + ShaftEnd) * 0.086, 2
                End If
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, ShaftEnd, -32.5, ShaftEnd, 25, (q * 25) + 25, 20 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -32.5, ShaftEnd, -32.5, -46.25, 25, (q * 25) + 25, (46.25 + ShaftEnd) * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5, ShaftEnd - 0.05, -32.5, ShaftEnd - 0.05, 25, (q * 25) + 25, 20 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -32.5 + 0.05, ShaftEnd, -32.5 + 0.05, -30.5, 25, (q * 25) + 25, (30 + ShaftEnd) * 0.086, 2
            End If
            If SectionNum = 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -12.5, ShaftEnd, -32.5, ShaftEnd, 25, (q * 25) + 25, 20 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -32.5, ShaftEnd, -32.5, -46.25, 25, (q * 25) + 25, (46.25 + ShaftEnd) * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -12.5, ShaftEnd - 0.05, -32.5, ShaftEnd - 0.05, 25, (q * 25) + 25, 20 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -32.5 + 0.05, ShaftEnd, -32.5 + 0.05, -30.5, 25, (q * 25) + 25, (30 + ShaftEnd) * 0.086, 2
            End If
            
    'Exterior Doors
    If q = -1 Then
        If e1 = True Then ElevatorDoor1L(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05, -12.55, -22.95, 19.6, 0, 1, 1
        If e1 = True Then ElevatorDoor1R(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05, -12.55, -27.05, 19.6, 0, 1, 1
        If e1 = True Then Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16, -12.55, -30, 19.6, 0, 1, 1
        ElevatorDoor1L(0).AddWall GetTex("ElevDoors"), -12.55, -19.05, -12.55, -22.95, 19.6, 27, 1, 1
        ElevatorDoor1R(0).AddWall GetTex("ElevDoors"), -12.55, -23.05, -12.55, -27.05, 19.6, 27, 1, 1
        Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16, -12.55, -30, 19.6, 27, 1, 1
        If e2 = True Then ElevatorDoor2L(-1).AddWall GetTex("ElevDoors"), 12.55, -19.05, 12.55, -22.95, 19.6, 0, 1, 1
        If e2 = True Then ElevatorDoor2R(-1).AddWall GetTex("ElevDoors"), 12.55, -23.05, 12.55, -27.05, 19.6, 0, 1, 1
        If e2 = True Then Shafts2(1).AddWall GetTex("ElevDoors"), 12.55 + 0.05, -16, 12.55 + 0.05, -30, 19.6, 0, 1, 1
        ElevatorDoor2L(0).AddWall GetTex("ElevDoors"), 12.55, -19.05, 12.55, -22.95, 19.6, 27, 1, 1
        ElevatorDoor2R(0).AddWall GetTex("ElevDoors"), 12.55, -23.05, 12.55, -27.05, 19.6, 27, 1, 1
        Shafts2(1).AddWall GetTex("ElevDoors"), 12.55, -16, 12.55, -30, 19.6, 27, 1, 1
        If e3 = True Then ElevatorDoor3L(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05 + (15 * 1), -12.55, -22.95 + (15 * 1), 19.6, 0, 1, 1
        If e3 = True Then ElevatorDoor3R(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05 + (15 * 1), -12.55, -27.05 + (15 * 1), 19.6, 0, 1, 1
        If e3 = True Then Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 1), -12.55, -30 + (15 * 1), 19.6, 0, 1, 1
        If e4 = True Then ElevatorDoor4L(-1).AddWall GetTex("ElevDoors"), 12.55, -19.05 + (15 * 1), 12.55, -22.95 + (15 * 1), 19.6, 0, 1, 1
        If e4 = True Then ElevatorDoor4R(-1).AddWall GetTex("ElevDoors"), 12.55, -23.05 + (15 * 1), 12.55, -27.05 + (15 * 1), 19.6, 0, 1, 1
        If e4 = True Then Shafts2(1).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 1), 12.55, -30 + (15 * 1), 19.6, 0, 1, 1
        If e5 = True Then ElevatorDoor5L(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05 + (15 * 2), -12.55, -22.95 + (15 * 2), 19.6, 0, 1, 1
        If e5 = True Then ElevatorDoor5R(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05 + (15 * 2), -12.55, -27.05 + (15 * 2), 19.6, 0, 1, 1
        If e5 = True Then Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 2), -12.55, -30 + (15 * 2), 19.6, 0, 1, 1
        If e6 = True Then ElevatorDoor6L(-1).AddWall GetTex("ElevDoors"), 12.55, -19.05 + (15 * 2), 12.55, -22.95 + (15 * 2), 19.6, 0, 1, 1
        If e6 = True Then ElevatorDoor6R(-1).AddWall GetTex("ElevDoors"), 12.55, -23.05 + (15 * 2), 12.55, -27.05 + (15 * 2), 19.6, 0, 1, 1
        If e6 = True Then Shafts2(1).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 2), 12.55, -30 + (15 * 2), 19.6, 0, 1, 1
        If e7 = True Then ElevatorDoor7L(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05 + (15 * 3), -12.55, -22.95 + (15 * 3), 19.6, 0, 1, 1
        If e7 = True Then ElevatorDoor7R(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05 + (15 * 3), -12.55, -27.05 + (15 * 3), 19.6, 0, 1, 1
        If e7 = True Then Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 3), -12.55, -30 + (15 * 3), 19.6, 0, 1, 1
        If e8 = True Then ElevatorDoor8L(-1).AddWall GetTex("ElevDoors"), 12.55, -19.05 + (15 * 3), 12.55, -22.95 + (15 * 3), 19.6, 0, 1, 1
        If e8 = True Then ElevatorDoor8R(-1).AddWall GetTex("ElevDoors"), 12.55, -23.05 + (15 * 3), 12.55, -27.05 + (15 * 3), 19.6, 0, 1, 1
        If e8 = True Then Shafts2(1).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 3), 12.55, -30 + (15 * 3), 19.6, 0, 1, 1
        If e9 = True Then ElevatorDoor9L(-1).AddWall GetTex("ElevDoors"), -12.55, -19.05 + (15 * 4), -12.55, -22.95 + (15 * 4), 19.6, 0, 1, 1
        If e9 = True Then ElevatorDoor9R(-1).AddWall GetTex("ElevDoors"), -12.55, -23.05 + (15 * 4), -12.55, -27.05 + (15 * 4), 19.6, 0, 1, 1
        If e9 = True Then Shafts1(1).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 4), -12.55, -30 + (15 * 4), 19.6, 0, 1, 1
        If e10 = True Then ElevatorDoor10L(-1).AddWall GetTex("ElevDoors"), 12.55, -19.05 + (15 * 4), 12.55, -22.95 + (15 * 4), 19.6, 0, 1, 1
        If e10 = True Then ElevatorDoor10R(-1).AddWall GetTex("ElevDoors"), 12.55, -23.05 + (15 * 4), 12.55, -27.05 + (15 * 4), 19.6, 0, 1, 1
        If e10 = True Then Shafts2(1).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 4), 12.55, -30 + (15 * 4), 19.6, 0, 1, 1
    
        'elev 1
        If e1 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, 0, 0.5, 1
        If e1 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, 0, 0.5, 1
        'elev 2
        If e2 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16, 12.5, -19, 19.6, 0, 0.5, 1
        If e2 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30, 12.5, -27, 19.6, 0, 0.5, 1
        'elev 3
        If e3 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 1), -12.5, -19 + (15 * 1), 19.6, 0, 0.5, 1
        If e3 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 1), -12.5, -27 + (15 * 1), 19.6, 0, 0.5, 1
        'elev 4
        If e4 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 1), 12.5, -19 + (15 * 1), 19.6, 0, 0.5, 1
        If e4 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 1), 12.5, -27 + (15 * 1), 19.6, 0, 0.5, 1
        'elev 5
        If e5 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 2), -12.5, -19 + (15 * 2), 19.6, 0, 0.5, 1
        If e5 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 2), -12.5, -27 + (15 * 2), 19.6, 0, 0.5, 1
        'elev 6
        If e6 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 2), 12.5, -19 + (15 * 2), 19.6, 0, 0.5, 1
        If e6 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 2), 12.5, -27 + (15 * 2), 19.6, 0, 0.5, 1
        'elev 7
        If e7 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 3), -12.5, -19 + (15 * 3), 19.6, 0, 0.5, 1
        If e7 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 3), -12.5, -27 + (15 * 3), 19.6, 0, 0.5, 1
        'elev 8
        If e8 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 3), 12.5, -19 + (15 * 3), 19.6, 0, 0.5, 1
        If e8 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 3), 12.5, -27 + (15 * 3), 19.6, 0, 0.5, 1
        'elev 9
        If e9 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 4), -12.5, -19 + (15 * 4), 19.6, 0, 0.5, 1
        If e9 = True Then Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 4), -12.5, -27 + (15 * 4), 19.6, 0, 0.5, 1
        'elev 10
        If e10 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 4), 12.5, -19 + (15 * 4), 19.6, 0, 0.5, 1
        If e10 = True Then Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 4), 12.5, -27 + (15 * 4), 19.6, 0, 0.5, 1
    
        'elev 1
        Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, 27, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, 27, 0.5, 1
        'elev 2
        Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -16, 12.5, -19, 19.6, 27, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), 12.5, -30, 12.5, -27, 19.6, 27, 0.5, 1
End If
    
    If e1 = True Then ElevatorDoor1L(FloorID).AddWall GetTex("ElevDoors"), -12.55, -19, -12.55, -23, 19.6, (q * 25) + 25, 1, 1
    If e1 = True Then ElevatorDoor1R(FloorID).AddWall GetTex("ElevDoors"), -12.55, -23, -12.55, -27, 19.6, (q * 25) + 25, 1, 1
    If e1 = True Then Shafts1(FloorID).AddWall GetTex("ElevDoors"), -12.55, -16, -12.55, -30, 19.6, (q * 25) + 25, 1, 1
    If e1 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -16, -12.5, -19, 19.6, (q * 25) + 25, 0.5, 1
    If e1 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -30, -12.5, -27, 19.6, (q * 25) + 25, 0.5, 1
    
    If e2 = True Then ElevatorDoor2L(FloorID).AddWall GetTex("ElevDoors"), 12.55, -19, 12.55, -23, 19.6, (q * 25) + 25, 1, 1
    If e2 = True Then ElevatorDoor2R(FloorID).AddWall GetTex("ElevDoors"), 12.55, -23, 12.55, -27, 19.6, (q * 25) + 25, 1, 1
    If e2 = True Then Shafts2(FloorID).AddWall GetTex("ElevDoors"), 12.55, -16, 12.55, -30, 19.6, (q * 25) + 25, 1, 1
    'elev 2
    If e2 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -16, 12.5, -19, 19.6, (q * 25) + 25, 0.5, 1
    If e2 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -30, 12.5, -27, 19.6, (q * 25) + 25, 0.5, 1
    
    If e3 = True Then ElevatorDoor3L(FloorID).AddWall GetTex("ElevDoors"), -12.55, -19 + (15 * 1), -12.55, -23 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    If e3 = True Then ElevatorDoor3R(FloorID).AddWall GetTex("ElevDoors"), -12.55, -23 + (15 * 1), -12.55, -27 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    If e3 = True Then Shafts1(FloorID).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 1), -12.55, -30 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    If e4 = True Then ElevatorDoor4L(FloorID).AddWall GetTex("ElevDoors"), 12.55, -19 + (15 * 1), 12.55, -23 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    If e4 = True Then ElevatorDoor4R(FloorID).AddWall GetTex("ElevDoors"), 12.55, -23 + (15 * 1), 12.55, -27 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    If e4 = True Then Shafts2(FloorID).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 1), 12.55, -30 + (15 * 1), 19.6, (q * 25) + 25, 1, 1
    'elev 3
    If e3 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 1), -12.5, -19 + (15 * 1), 19.6, (q * 25) + 25, 0.5, 1
    If e3 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 1), -12.5, -27 + (15 * 1), 19.6, (q * 25) + 25, 0.5, 1
    'elev 4
    If e4 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 1), 12.5, -19 + (15 * 1), 19.6, (q * 25) + 25, 0.5, 1
    If e4 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 1), 12.5, -27 + (15 * 1), 19.6, (q * 25) + 25, 0.5, 1
    
    If e5 = True Then ElevatorDoor5L(FloorID).AddWall GetTex("ElevDoors"), -12.55, -19 + (15 * 2), -12.55, -23 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    If e5 = True Then ElevatorDoor5R(FloorID).AddWall GetTex("ElevDoors"), -12.55, -23 + (15 * 2), -12.55, -27 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    If e5 = True Then Shafts1(FloorID).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 2), -12.55, -30 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    If e6 = True Then ElevatorDoor6L(FloorID).AddWall GetTex("ElevDoors"), 12.55, -19 + (15 * 2), 12.55, -23 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    If e6 = True Then ElevatorDoor6R(FloorID).AddWall GetTex("ElevDoors"), 12.55, -23 + (15 * 2), 12.55, -27 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    If e6 = True Then Shafts2(FloorID).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 2), 12.55, -30 + (15 * 2), 19.6, (q * 25) + 25, 1, 1
    'elev 5
    If e5 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 2), -12.5, -19 + (15 * 2), 19.6, (q * 25) + 25, 0.5, 1
    If e5 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 2), -12.5, -27 + (15 * 2), 19.6, (q * 25) + 25, 0.5, 1
    'elev 6
    If e6 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 2), 12.5, -19 + (15 * 2), 19.6, (q * 25) + 25, 0.5, 1
    If e6 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 2), 12.5, -27 + (15 * 2), 19.6, (q * 25) + 25, 0.5, 1
    
    If e7 = True Then ElevatorDoor7L(FloorID).AddWall GetTex("ElevDoors"), -12.55, -19 + (15 * 3), -12.55, -23 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    If e7 = True Then ElevatorDoor7R(FloorID).AddWall GetTex("ElevDoors"), -12.55, -23 + (15 * 3), -12.55, -27 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    If e7 = True Then Shafts1(FloorID).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 3), -12.55, -30 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    If e8 = True Then ElevatorDoor8L(FloorID).AddWall GetTex("ElevDoors"), 12.55, -19 + (15 * 3), 12.55, -23 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    If e8 = True Then ElevatorDoor8R(FloorID).AddWall GetTex("ElevDoors"), 12.55, -23 + (15 * 3), 12.55, -27 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    If e8 = True Then Shafts2(FloorID).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 3), 12.55, -30 + (15 * 3), 19.6, (q * 25) + 25, 1, 1
    'elev 7
    If e7 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 3), -12.5, -19 + (15 * 3), 19.6, (q * 25) + 25, 0.5, 1
    If e7 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 3), -12.5, -27 + (15 * 3), 19.6, (q * 25) + 25, 0.5, 1
    'elev 8
    If e8 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 3), 12.5, -19 + (15 * 3), 19.6, (q * 25) + 25, 0.5, 1
    If e8 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 3), 12.5, -27 + (15 * 3), 19.6, (q * 25) + 25, 0.5, 1
    
    If e9 = True Then ElevatorDoor9L(FloorID).AddWall GetTex("ElevDoors"), -12.55, -19 + (15 * 4), -12.55, -23 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    If e9 = True Then ElevatorDoor9R(FloorID).AddWall GetTex("ElevDoors"), -12.55, -23 + (15 * 4), -12.55, -27 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    If e9 = True Then Shafts1(FloorID).AddWall GetTex("ElevDoors"), -12.55, -16 + (15 * 4), -12.55, -30 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    If e10 = True Then ElevatorDoor10L(FloorID).AddWall GetTex("ElevDoors"), 12.55, -19 + (15 * 4), 12.55, -23 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    If e10 = True Then ElevatorDoor10R(FloorID).AddWall GetTex("ElevDoors"), 12.55, -23 + (15 * 4), 12.55, -27 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    If e10 = True Then Shafts2(FloorID).AddWall GetTex("ElevDoors"), 12.55, -16 + (15 * 4), 12.55, -30 + (15 * 4), 19.6, (q * 25) + 25, 1, 1
    'elev 9
    If e9 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -16 + (15 * 4), -12.5, -19 + (15 * 4), 19.6, (q * 25) + 25, 0.5, 1
    If e9 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), -12.5, -30 + (15 * 4), -12.5, -27 + (15 * 4), 19.6, (q * 25) + 25, 0.5, 1
    'elev 10
    If e10 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -16 + (15 * 4), 12.5, -19 + (15 * 4), 19.6, (q * 25) + 25, 0.5, 1
    If e10 = True Then Room(FloorID).AddWall GetTex("ElevExtPanels"), 12.5, -30 + (15 * 4), 12.5, -27 + (15 * 4), 19.6, (q * 25) + 25, 0.5, 1
    
    
End Sub
Sub Fall()
'This detects if the user is above a hole or something (ready to fall)
Room(CameraFloor).SetCollisionEnable True
Shafts1(CameraFloor).SetCollisionEnable True
Shafts2(CameraFloor).SetCollisionEnable True
Buildings.SetCollisionEnable True
External.SetCollisionEnable True

If Room(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 12, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = False And _
    Shafts1(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 12, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = False And _
    Shafts2(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 12, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = False And _
    External.Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 12, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = False And _
    Buildings.Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 12, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = False And InElevator = False And InStairwell = False Then IsFalling = True

Room(CameraFloor).SetCollisionEnable False
Shafts1(CameraFloor).SetCollisionEnable False
Shafts2(CameraFloor).SetCollisionEnable False
Buildings.SetCollisionEnable False
External.SetCollisionEnable False

'*********************************
If IsFalling = False Then Exit Sub

'The gravity originally acted weird
Dim TimeRate As Single
Gravity = 5
If FallRate = 0 Then
lngOldTick = GetTickCount()
CameraOriginalPos = Camera.GetPosition.Y
End If

'MsgBox ((GetTickCount() / 1000) - (lngOldTick / 1000))

'Basically this is Fallrate=Gravity*SecondsPassed
TimeRate = ((GetTickCount() / 1000) - (lngOldTick / 1000))
FallRate = (Gravity * TimeRate) ^ 1.8
If FallRate = 0 Then FallRate = 0.1
If TimeRate = 0 Then TimeRate = 0.1
'MsgBox ("Gravity:" + Str$(Gravity) + " Time Passed:" + Str$((GetTickCount() / 1000) - (lngOldTick / 1000)))

Camera.SetPosition Camera.GetPosition.X, CameraOriginalPos - FallRate, Camera.GetPosition.z

Room(CameraFloor).SetCollisionEnable True
Shafts1(CameraFloor).SetCollisionEnable True
Shafts2(CameraFloor).SetCollisionEnable True
Buildings.SetCollisionEnable True

If Room(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - (FallRate / TimeRate), Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = True Then
FallRate = 0
IsFalling = False
If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
If CameraFloor = 1 Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
End If
If Shafts1(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - (FallRate / TimeRate), Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = True Then
FallRate = 0
IsFalling = False
If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
If CameraFloor = 1 Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
End If
If Shafts2(CameraFloor).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - (FallRate / TimeRate), Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = True Then
FallRate = 0
IsFalling = False
If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
If CameraFloor = 1 Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
End If
If Buildings.Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - (FallRate / TimeRate), Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = True Then
FallRate = 0
IsFalling = False
If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
If CameraFloor = 1 Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
End If

Room(CameraFloor).SetCollisionEnable False
Shafts1(CameraFloor).SetCollisionEnable False
Shafts2(CameraFloor).SetCollisionEnable False
Buildings.SetCollisionEnable False

End Sub

Sub Init_Simulator()
'On Error GoTo ErrorHandler
isRunning = True
EnableCollisions = True
Form2.Show
Form1.Show
Set TV = New TVEngine
Set Scene = New TVScene
Set Mesh = New TVMesh
Set External = New TVMesh
Set Buildings = New TVMesh
For i = 1 To 138
Set Room(i) = New TVMesh
Set Stairs(i) = New TVMesh
Set ShaftsFloor(i) = New TVMesh
Set Shafts1(i) = New TVMesh
Set Shafts2(i) = New TVMesh
Next i
For i = -1 To 138
Set ElevatorDoor1L(i) = New TVMesh
Set ElevatorDoor1R(i) = New TVMesh
Set ElevatorDoor2L(i) = New TVMesh
Set ElevatorDoor2R(i) = New TVMesh
Set ElevatorDoor3L(i) = New TVMesh
Set ElevatorDoor3R(i) = New TVMesh
Set ElevatorDoor4L(i) = New TVMesh
Set ElevatorDoor4R(i) = New TVMesh
Set ElevatorDoor5L(i) = New TVMesh
Set ElevatorDoor5R(i) = New TVMesh
Set ElevatorDoor6L(i) = New TVMesh
Set ElevatorDoor6R(i) = New TVMesh
Set ElevatorDoor7L(i) = New TVMesh
Set ElevatorDoor7R(i) = New TVMesh
Set ElevatorDoor8L(i) = New TVMesh
Set ElevatorDoor8R(i) = New TVMesh
Set ElevatorDoor9L(i) = New TVMesh
Set ElevatorDoor9R(i) = New TVMesh
Set ElevatorDoor10L(i) = New TVMesh
Set ElevatorDoor10R(i) = New TVMesh
Set Buttons1(i) = New TVMesh
Set Buttons2(i) = New TVMesh
Set Buttons3(i) = New TVMesh
Set Buttons4(i) = New TVMesh
Set Buttons5(i) = New TVMesh
Set Buttons6(i) = New TVMesh
Set Buttons7(i) = New TVMesh
Set Buttons8(i) = New TVMesh
Set Buttons9(i) = New TVMesh
Set Buttons10(i) = New TVMesh
Next i

For i = 1 To 10
Set Elevator(i) = New TVMesh
Set FloorIndicator(i) = New TVMesh
Set ElevatorInsDoorL(i) = New TVMesh
Set ElevatorInsDoorR(i) = New TVMesh
'Set ElevatorMusic(i) = New TV3DMedia.TVSoundWave3D
Set ElevatorSounds(i) = New TV3DMedia.TVSoundWave3D
Set Plaque(i) = New TVMesh
Set CallButtons(i) = New TVMesh
Next i

Set Camera = New TVCamera

Set TextureFactory = New TVTextureFactory
Set SoundEngine = New TV3DMedia.TVSoundEngine
Set Light = New TVLightEngine


   
Form1.Print " "
Form1.Print "Skyscraper 0.9 Beta"
Form1.Print "©2003 Ryan Thoryk"
Form1.Print " "
Form1.Print "Skyscraper comes with ABSOLUTELY NO WARRANTY. This is free"
Form1.Print "software, and you are welcome to redistribute it under certain"
Form1.Print "conditions. For details, see the file gpl.txt"
Form1.Print " "

  If TV.ShowDriverDialog = False Then End
   DoEvents
   '2. Initialize the engine with the selected mode
   TV.SetSearchDirectory App.Path
Form1.Print "Initializing TrueVision3D..."
   'TV.Initialize Form1.hWnd
    TV.Init3DWindowedMode Form1.hWnd
    'TV.Init3DFullscreen 640, 480, 16

Form1.Print "Creating Meshes..."
  Set Inp = New TVInputEngine
  TV.SetSearchDirectory App.Path
  TV.DisplayFPS = True
  
  'TV.MultiSampleTp = TV3D_MULTISAMPLE_16_SAMPLES
  'TV.EnableAntialising True
    
  Scene.SetTextureFilter (TV_FILTER_ANISOTROPIC)
  'Scene.SetDithering True
  TV.EnableAntialising True
  
  Scene.LoadCursor "pointer.bmp", TV_COLORKEY_BLACK, 14, 16
  
  Set Mesh = Scene.CreateMeshBuilder("Mesh")
  Set External = Scene.CreateMeshBuilder("External")
  For i = 1 To 138
  Set Room(i) = Scene.CreateMeshBuilder("Room " + Str$(i))
  Set Stairs(i) = Scene.CreateMeshBuilder("Stairs " + Str$(i))
  Set ShaftsFloor(i) = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(i))
  Set Shafts1(i) = Scene.CreateMeshBuilder("Shafts1 " + Str$(i))
  Set Shafts2(i) = Scene.CreateMeshBuilder("Shafts2 " + Str$(i))
  Next i
  For i = -1 To 138
  Set ElevatorDoor1L(i) = Scene.CreateMeshBuilder("ElevatorDoor1L " + Str$(i))
  Set ElevatorDoor1R(i) = Scene.CreateMeshBuilder("ElevatorDoor1R " + Str$(i))
  Set ElevatorDoor2L(i) = Scene.CreateMeshBuilder("ElevatorDoor2L " + Str$(i))
  Set ElevatorDoor2R(i) = Scene.CreateMeshBuilder("ElevatorDoor2R " + Str$(i))
  Set ElevatorDoor3L(i) = Scene.CreateMeshBuilder("ElevatorDoor3L " + Str$(i))
  Set ElevatorDoor3R(i) = Scene.CreateMeshBuilder("ElevatorDoor3R " + Str$(i))
  Set ElevatorDoor4L(i) = Scene.CreateMeshBuilder("ElevatorDoor4L " + Str$(i))
  Set ElevatorDoor4R(i) = Scene.CreateMeshBuilder("ElevatorDoor4R " + Str$(i))
  Set ElevatorDoor5L(i) = Scene.CreateMeshBuilder("ElevatorDoor5L " + Str$(i))
  Set ElevatorDoor5R(i) = Scene.CreateMeshBuilder("ElevatorDoor5R " + Str$(i))
  Set ElevatorDoor6L(i) = Scene.CreateMeshBuilder("ElevatorDoor6L " + Str$(i))
  Set ElevatorDoor6R(i) = Scene.CreateMeshBuilder("ElevatorDoor6R " + Str$(i))
  Set ElevatorDoor7L(i) = Scene.CreateMeshBuilder("ElevatorDoor7L " + Str$(i))
  Set ElevatorDoor7R(i) = Scene.CreateMeshBuilder("ElevatorDoor7R " + Str$(i))
  Set ElevatorDoor8L(i) = Scene.CreateMeshBuilder("ElevatorDoor8L " + Str$(i))
  Set ElevatorDoor8R(i) = Scene.CreateMeshBuilder("ElevatorDoor8R " + Str$(i))
  Set ElevatorDoor9L(i) = Scene.CreateMeshBuilder("ElevatorDoor9L " + Str$(i))
  Set ElevatorDoor9R(i) = Scene.CreateMeshBuilder("ElevatorDoor9R " + Str$(i))
  Set ElevatorDoor10L(i) = Scene.CreateMeshBuilder("ElevatorDoor10L " + Str$(i))
  Set ElevatorDoor10R(i) = Scene.CreateMeshBuilder("ElevatorDoor10R " + Str$(i))
  Set Buttons1(i) = Scene.CreateMeshBuilder("Buttons1 " + Str$(i))
  Set Buttons2(i) = Scene.CreateMeshBuilder("Buttons2 " + Str$(i))
  Set Buttons3(i) = Scene.CreateMeshBuilder("Buttons3 " + Str$(i))
  Set Buttons4(i) = Scene.CreateMeshBuilder("Buttons4 " + Str$(i))
  Set Buttons5(i) = Scene.CreateMeshBuilder("Buttons5 " + Str$(i))
  Set Buttons6(i) = Scene.CreateMeshBuilder("Buttons6 " + Str$(i))
  Set Buttons7(i) = Scene.CreateMeshBuilder("Buttons7 " + Str$(i))
  Set Buttons8(i) = Scene.CreateMeshBuilder("Buttons8 " + Str$(i))
  Set Buttons9(i) = Scene.CreateMeshBuilder("Buttons9 " + Str$(i))
  Set Buttons10(i) = Scene.CreateMeshBuilder("Buttons10 " + Str$(i))
  Next i
  For i = 1 To 10
  Set Elevator(i) = Scene.CreateMeshBuilder("Elevator" + Str$(i))
  Set FloorIndicator(i) = Scene.CreateMeshBuilder("FloorIndicator" + Str$(i))
  Set ElevatorInsDoorL(i) = Scene.CreateMeshBuilder("ElevatorInsDoorL" + Str$(i))
  Set ElevatorInsDoorR(i) = Scene.CreateMeshBuilder("ElevatorInsDoorR" + Str$(i))
  Set Plaque(i) = Scene.CreateMeshBuilder("Plaque" + Str$(i))
  Set CallButtons(i) = Scene.CreateMeshBuilder("CallButtons" + Str$(i))
  Next i
  Set Buildings = Scene.CreateMeshBuilder("Buildings")
    
Form1.Print "Mesh Areas Created"
  Scene.SetViewFrustum 90, 10000
  'TextureFactory.LoadTexture "..\..\..\media\stone_wall.jpg", "Floor"
  
  TextureFactory.LoadTexture "brick1.jpg", "BrickTexture"
  TextureFactory.LoadTexture "LobbyFront.jpg", "LobbyFront"
  TextureFactory.LoadTexture "windows11c.jpg", "MainWindows"
  TextureFactory.LoadTexture "granite.jpg", "Granite"
  'TextureFactory.LoadTexture "marbl3.jpg", "Marble3"
  'TextureFactory.LoadTexture "text12.jpg", "Marble3"
  TextureFactory.LoadTexture "symb5.jpg", "Marble3"
  TextureFactory.LoadTexture "marb047.jpg", "Marble4"
  TextureFactory.LoadTexture "elev1.jpg", "Elev1"
  TextureFactory.LoadTexture "textur15.jpg", "Wood1"
  TextureFactory.LoadTexture "text16.jpg", "Wood2"
  'TextureFactory.LoadTexture "text12.jpg", "Wall1"
  TextureFactory.LoadTexture "marbl3.jpg", "Wall1"
  TextureFactory.LoadTexture "marb123.jpg", "Wall2"
  'TextureFactory.LoadTexture "marbl3.jpg", "Wall2"
  'TextureFactory.LoadTexture "marb056.jpg", "Wall3"
  TextureFactory.LoadTexture "cutston.jpg", "Ceiling1"
  TextureFactory.LoadTexture "text12.jpg", "Wall3"
  TextureFactory.LoadTexture "text16.jpg", "ElevDoors"
  TextureFactory.LoadTexture "marb148.jpg", "ElevExtPanels"
  TextureFactory.LoadTexture "mason01.jpg", "Concrete"
  TextureFactory.LoadTexture "text13.jpg", "Stairs"
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
  TextureFactory.LoadTexture "windows08.jpg", "Windows8"
  TextureFactory.LoadTexture "windows11.jpg", "Windows11"
  
    
  TextureFactory.LoadTexture "top.jpg", "SkyTop"
  TextureFactory.LoadTexture "bottom.jpg", "SkyBottom"
  TextureFactory.LoadTexture "left.jpg", "SkyLeft"
  TextureFactory.LoadTexture "right.jpg", "SkyRight"
  TextureFactory.LoadTexture "front.jpg", "SkyFront"
  TextureFactory.LoadTexture "back.jpg", "SkyBack"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator1"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator2"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator3"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator4"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator5"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator6"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator7"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator8"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator9"
  'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "FloorIndicator10"
  TextureFactory.LoadTexture App.Path + "\objects\benedeti.jpg", "ColumnTex", , , TV_COLORKEY_NO
  TextureFactory.LoadTexture App.Path + "\data\plaque.jpg", "Plaque"
  
  'loads all the floor indicator/button pictures
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "ButtonL"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\M.jpg", "ButtonM"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\R.jpg", "ButtonR"
  For i = 2 To 137
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\" + Mid$(Str$(i), 2) + ".jpg", "Button" + Mid$(Str$(i), 2)
  Next i

   
Form1.Print "Textures Loaded"
      
  'Sound System
    
    Call SoundEngine.Init(Form1.hWnd)
    
Form1.Print "Sound Initialized"

    'Load the file into the classes.
    'ElevatorMusic.Load App.Path + "\elevmusic3.wav"
    'ElevatorMusic.Load App.Path + "\elevmusic2.wav"
'Form1.Print "Music Loaded"
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
Form1.Print "Loading 3D Objects..."
End Sub


Sub OptimizeMeshes()
  Form1.Print "Optimizing Meshes.";
  For i = 1 To 138
  Form1.Print ".";
  Room(i).Optimize
  Stairs(i).Optimize
  ShaftsFloor(i).Optimize
  Shafts1(i).Optimize
  Shafts2(i).Optimize
  Next i
  Form1.Print "."
  For i = -1 To 138
  Form1.Print ".";
  ElevatorDoor1L(i).Optimize
  ElevatorDoor1R(i).Optimize
  ElevatorDoor2L(i).Optimize
  ElevatorDoor2R(i).Optimize
  ElevatorDoor3L(i).Optimize
  ElevatorDoor3R(i).Optimize
  ElevatorDoor4L(i).Optimize
  ElevatorDoor4R(i).Optimize
  ElevatorDoor5L(i).Optimize
  ElevatorDoor5R(i).Optimize
  ElevatorDoor6L(i).Optimize
  ElevatorDoor6R(i).Optimize
  ElevatorDoor7L(i).Optimize
  ElevatorDoor7R(i).Optimize
  ElevatorDoor8L(i).Optimize
  ElevatorDoor8R(i).Optimize
  ElevatorDoor9L(i).Optimize
  ElevatorDoor9R(i).Optimize
  ElevatorDoor10L(i).Optimize
  ElevatorDoor10R(i).Optimize
  Buttons1(i).Optimize
  Buttons2(i).Optimize
  Buttons3(i).Optimize
  Buttons4(i).Optimize
  Buttons5(i).Optimize
  Buttons6(i).Optimize
  Buttons7(i).Optimize
  Buttons8(i).Optimize
  Buttons9(i).Optimize
  Buttons10(i).Optimize
  Next i
  Form1.Print ".";
  External.Optimize
  Form1.Print ".";
  For i = 1 To 10
  Elevator(i).Optimize
  ElevatorInsDoorL(i).Optimize
  ElevatorInsDoorR(i).Optimize
  Plaque(i).Optimize
  Form1.Print ".";
  ElevatorInsDoorL(i).Optimize
  Form1.Print ".";
  ElevatorInsDoorR(i).Optimize
  Next i
  Form1.Print ".";
  Buildings.Optimize
  
  Form1.Print "Done"
  
End Sub

Sub ProcessFloors()
'Lobby
Call ProcessLobby
Form1.Print "Processing Floors 2 to 39...";
Call Process2to39
Form1.Print "Processing Floors 40 to 79";
Call Process40to79
Form1.Print "Processing Floors 80 to 117";
Call Process80to117
Form1.Print "Processing Floors 118 to 132";
Call Process118to132
Form1.Print "Processing Floors 133 to 138";
Call ProcessOtherFloors
Form1.Print "Initializing Lobby..."
Call InitObjectsForFloor(1)
 
Form1.Print "Processing Outside..."
Call ProcessOutside
Call ProcessMisc
Call ProcessStairs
Call OptimizeMeshes
End Sub

Sub ProcessLobby()


Form1.Print "Processing Lobby..."
    i = 1
    Room(i).AddFloor GetTex("Marble4"), -160, -150, 160, 150, 0, 50, 31
    'Mezzanine Level
    Room(i).AddFloor GetTex("Granite"), -80, -55, 80, -46.25, 27, 30, 1
    Room(i).AddFloor GetTex("Granite"), -80, 46.25, 80, 55, 27, 30, 1
    Room(i).AddFloor GetTex("Granite"), -80, -46.25, -32.5, 46.25, 27, 6, 10
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 80, 46.25, 27, 6, 10
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, 27, 2, 10
    
    Room(i).AddFloor GetTex("Ceiling1"), -80, -55, 80, -46.25, 26.9, 30, 1
    Room(i).AddFloor GetTex("Ceiling1"), -80, 46.25, 80, 55, 26.9, 30, 1
    Room(i).AddFloor GetTex("Ceiling1"), -80, -46.25, -32.5, 46.25, 26.9, 11, 10
    Room(i).AddFloor GetTex("Ceiling1"), 32.5, -46.25, 80, 46.25, 26.9, 11, 10
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 46.25, 26.9, 2, 10
    
    'End of Mezzanine
    
    'External.AddWall GetTex("LobbyFront"), -160, -150, 160, -150, 75, 0, 3, 1
    'External.AddWall GetTex("LobbyFront"), 160, -150, 160, 150, 75, 0, 3, 1
    'External.AddWall GetTex("LobbyFront"), 160, 150, -160, 150, 75, 0, 3, 1
    'External.AddWall GetTex("LobbyFront"), -160, 150, -160, -150, 75, 0, 3, 1
              
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, -150 + 0.1, 160 - 0.1, -150 + 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, -150 + 0.1, 160 - 0.1, 150 - 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, 150 - 0.1, -160 + 0.1, 150 - 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, 150 - 0.1, -160 + 0.1, -150 + 0.1, 75, 0, 3, 1
                  
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, 75, -1, 1
                  
    'always make sure this call is also in the ProcessFloors sub, but under the DrawElevatorShafts name
    Call DrawElevatorWalls(Int(i), 1, False, True, True, True, True, True, True, True, True, True, True)
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, 0, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, 0, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, 0, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, 0, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, 0, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, 0, (10 * 0.086) * 4, 3 * 4
    
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 7.5, 19.5, (10.3 * 0.086) * 4, (7.5 * 0.086) * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 3.5, 19.5 + 27, (10.3 * 0.086) * 4, (3.5 * 0.086) * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 25, 50, (10.3 * 0.086) * 4, (25 * 0.086) * 2
    

    'Service elevator shaft (North)
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, -130, -12.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -32.5, -130, -32.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, -110, -32.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, -130, -32.5, -130, 75, 0, 1, 3
    Buildings.AddWall GetTex("Concrete"), -30, -130, -30, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), -50, -130, -50, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), -30, -110, -50, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), -30, -130, -50, -130, (136 * 25) + 50, 0, 1, 138
    
    'Service elevator shaft (South)
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, 130, -12.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -32.5, 130, -32.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, 110, -32.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), -12.5, 130, -32.5, 130, 75, 0, 1, 3
    Buildings.AddWall GetTex("Concrete"), -30, 130, -30, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), -50, 130, -50, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), -30, 110, -50, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), -30, 130, -50, 130, (131 * 25) + 50, 0, 1, 133
    
    'Stairwell shaft (North)
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, -130, 12.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 32.5, -130, 32.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, -110, 32.5, -110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, -130, 32.5, -130, 75, 0, 1, 3
    Buildings.AddWall GetTex("Concrete"), 30, -130, 30, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), 50, -130, 50, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), 30, -110, 50, -110, (136 * 25) + 50, 0, 1, 138
    Buildings.AddWall GetTex("Concrete"), 30, -130, 50, -130, (136 * 25) + 50, 0, 1, 138
    
    'Stairwell shaft (South)
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, 130, 12.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 32.5, 130, 32.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, 110, 32.5, 110, 75, 0, 1, 3
    'ShaftsFloor(i).AddWall GetTex("Concrete"), 12.5, 130, 32.5, 130, 75, 0, 1, 3
    Buildings.AddWall GetTex("Concrete"), 30, 130, 30, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), 50, 130, 50, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), 30, 110, 50, 110, (131 * 25) + 50, 0, 1, 133
    Buildings.AddWall GetTex("Concrete"), 30, 130, 50, 130, (131 * 25) + 50, 0, 1, 133
    
    'Ceiling
    Room(i).AddFloor GetTex("Ceiling1"), -160, -150, 160, -46.25, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, 46.25, 160, 150, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, -46.25, -32.5, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), 32.5, -46.25, 160, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 49.5, 2, 5
    
        
End Sub

Sub Process2to39()
    
    'Floors 2 to 39
    For i = 2 To 39
    Form1.Print ".";
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
    
    'External.AddWall GetTex("MainWindows"), -160, -150, 160, -150, 25, (i * 25) + 25, 7.6, 1
    'External.AddWall GetTex("MainWindows"), 160, -150, 160, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 160, 150, -160, 150, 25, (i * 25) + 25, 7.6, 1
    'External.AddWall GetTex("MainWindows"), -160, 150, -160, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    If i = 2 Or i = 39 Then Call DrawElevatorWalls(Int(i), 2, True, True, False, False, False, False, False, False, False, True, True)
    If i <> 2 And i <> 39 Then Call DrawElevatorWalls(Int(i), 2, True, False, False, False, False, False, False, False, False, True, True)
    
    'Room(I) Walls
    'Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), -160, 46.3, -32.5, 46.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, 46.3, 160, 46.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -160, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 160, -46.3, 25, (i * 25) + 25, 2, 1
    
    'top walls
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, -110 - 3.9, -71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -110 + 3.9, -71.3, -45 - 3.9, -71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -45 + 3.9, -71.3, -20 - 3.9, -71.3, 19.5, (i * 25) + 25, ((45 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -20 + 3.9, -71.3, 45 - 3.9, -71.3, 19.5, (i * 25) + 25, ((45 + 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 45 + 3.9, -71.3, 110 - 3.9, -71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110 + 3.9, -71.3, 160, -71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 5.5, (i * 25) + 25 + 19.5, ((160 * 2) * 0.086), (5.5 * 0.08)
    
    'bottom walls
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, -110 - 3.9, 71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -110 + 3.9, 71.3, -45 - 3.9, 71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -45 + 3.9, 71.3, 20 - 3.9, 71.3, 19.5, (i * 25) + 25, ((45 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 71.3, 45 - 3.9, 71.3, 19.5, (i * 25) + 25, ((45 + 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 45 + 3.9, 71.3, 110 - 3.9, 71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110 + 3.9, 71.3, 160, 71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, 160, 71.3, 5.5, (i * 25) + 25 + 19.5, ((160 * 2) * 0.086), (5.5 * 0.08)
    
    'top middle walls
    Room(i).AddWall GetTex("Wall3"), -160, -46.25, -108 - 3.9, -46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -108 + 3.9, -46.25, -87 - 3.9, -46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -87 + 3.9, -46.25, -32.5, -46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.25, 39 - 3.9, -46.25, 19.5, (i * 25) + 25, ((39 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 39 + 3.9, -46.25, 87 - 3.9, -46.25, 19.5, (i * 25) + 25, ((87 - 39) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 87 + 3.9, -46.25, 108 - 3.9, -46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 108 + 3.9, -46.25, 160, -46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -160, -46.25, -32.5, -46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.25, 160, -46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, ((46.25 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 50, -20, 32.5, -20, 25, (i * 25) + 25, ((50 - 32.5) * 0.086), 1
    
    'bottom middle walls
    Room(i).AddWall GetTex("Wall3"), -160, 46.25, -108 - 3.9, 46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -108 + 3.9, 46.25, -87 - 3.9, 46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -87 + 3.9, 46.25, -32.5, 46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.25, 87 - 3.9, 46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 87 + 3.9, 46.25, 108 - 3.9, 46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 108 + 3.9, 46.25, 160, 46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -160, 46.25, -32.5, 46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.25, 160, 46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -96, -71.3, -96, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -96, 71.3, -96, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -32, -71.3, -32, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -32, 71.3, -32, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 32, -71.3, 32, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 32, 71.3, 32, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -71.3, 96, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 96, 71.3, 96, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -46.3, 96, 46.3, 25, (i * 25) + 25, ((46.3 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -96, -46.3, -96, 46.3, 25, (i * 25) + 25, ((46.3 * 2) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -160, 0, -32, 0, 25, (i * 25) + 25, ((160 - 32) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 160, 0, 32, 0, 25, (i * 25) + 25, ((160 - 32) * 0.086), 1
    
    Next i
Form1.Print "done"
    'Landings
    'External.AddFloor GetTex("BrickTexture"), -160, -150, -135, 150, (40 * 25) + 25, 10, 10
    'External.AddFloor GetTex("BrickTexture"), 160, -150, 135, 150, (40 * 25) + 25, 10, 10

End Sub
Sub Process40to79()
    
    'Floors 40 to 79 (minus 14 feet on both sides where 20=8 feet)
    For i = 40 To 79
    Form1.Print ".";
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
   
    'External.AddWall GetTex("MainWindows"), -135, -150, 135, -150, 25, (i * 25) + 25, 6.5, 1
    'External.AddWall GetTex("MainWindows"), 135, -150, 135, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 135, 150, -135, 150, 25, (i * 25) + 25, 6.5, 1
    'External.AddWall GetTex("MainWindows"), -135, 150, -135, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    If i = 40 Or i = 65 Or i = 79 Then Call DrawElevatorWalls(Int(i), 3, True, True, False, False, False, False, False, True, True, False, False)
    If i <> 40 And i <> 65 And i <> 79 Then Call DrawElevatorWalls(Int(i), 3, True, False, False, False, False, False, False, True, True, False, False)
               
    'Room(I) Walls
    
    'Top of upper hallway
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, -71.3, -37 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, -71.3, -20 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -20 + 3.9, -71.3, 37 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, -71.3, 90 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, -71.3, 135, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom of lower hallway
    Room(i).AddWall GetTex("Wall3"), -135, 55.83, 135, 55.83, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), -135, 55.83, -90 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 55.83, -37 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, 55.83, 20 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 55.83, 37 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, 55.83, 90 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, 55.83, 135, 55.83, 19.5, (i * 25) + 25, 2, 1
    
    'Top of lower hallway
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -32.5, 30.83, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -90 - 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 30.83, -72 - 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -72 + 3.9, 30.83, -32.5, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 30.83, 32.5, 30.83, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 30.83, 90 + 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 - 3.9, 30.83, 72 + 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 72 - 3.9, 30.83, 32.5, 30.83, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom of upper hallway
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -90 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, -46.3, -72 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -72 + 3.9, -46.3, -32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, -46.3, 90 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 - 3.9, -46.3, 72 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 72 - 3.9, -46.3, 40 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 40 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
        
    Room(i).AddWall GetTex("Wall3"), -81, -71.3, -81, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -81, 55.83, -81, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -27, -71.3, -27, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -27, 55.83, -27, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 27, -71.3, 27, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 27, 55.83, 27, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 81, -71.3, 81, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 81, 55.83, 81, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 81, -46.3, 81, 30.83, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -81, -46.3, -81, 30.83, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, 0, -32, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 0, 32, 0, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 25, (i * 25) + 25, 9, 1
    
    'Lower room split wall
    Room(i).AddWall GetTex("Wall3"), -135, 102, 135, 102, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), -135, 102, -90 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 102, -37 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, 102, 20 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 102, 37 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, 102, 90 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, 102, 135, 102, 19.5, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, 4, 1
    Room(i).AddWall GetTex("Wall3"), 50, -20, 32.5, -20, 25, (i * 25) + 25, 4, 1
    
    Next i
Form1.Print "done"
    
    'External.AddFloor GetTex("BrickTexture"), -135, -150, -110, 150, (80 * 25) + 25, 10, 10
    'External.AddFloor GetTex("BrickTexture"), 135, -150, 110, 150, (80 * 25) + 25, 10, 10

End Sub
Sub Process80to117()
    
    'Floors 80 to 117
    For i = 80 To 117
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -110, -150, 110, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, 15.41, 110, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -32.5, 15.41, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 110, 15.41, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 15.41, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, 15.41, 110, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -32.5, 15.41, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 110, 15.41, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 15.41, (i * 25) + 49.5, 1, 3
    
    'External.AddWall GetTex("MainWindows"), -110, -150, 110, -150, 25, (i * 25) + 25, 5, 1
    'External.AddWall GetTex("MainWindows"), 110, -150, 110, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 110, 150, -110, 150, 25, (i * 25) + 25, 5, 1
    'External.AddWall GetTex("MainWindows"), -110, 150, -110, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
       
    If i = 80 Then Call DrawElevatorWalls(Int(i), 4, True, True, True, True, True, True, True, False, False, False, False)
    If i >= 81 And i <= 99 Then Call DrawElevatorWalls(Int(i), 4, True, True, False, False, False, True, True, False, False, False, False)
    If i >= 100 And i <= 114 Then Call DrawElevatorWalls(Int(i), 4, True, True, False, True, True, False, False, False, False, False, False)
    If i >= 115 And i <= 117 Then Call DrawElevatorWalls(Int(i), 4, True, True, False, False, False, False, False, False, False, False, False)
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 40, -12.5, 40, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 40, 50, 40, 25, (i * 25) + 25, 2, 1
    
    'Bottom Horizontal Hallway, 2 Top Walls
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -32.5, 15.41, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -61 - 3.9, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -61 + 3.9, 15.41, -32.5, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 15.41, 32.5, 15.41, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 15.41, 61 + 3.9, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 61 - 3.9, 15.41, 32.5, 15.41, 19.5, (i * 25) + 25, 9, 1
    
    'Top Horizontal Hallway, 2 Bottom Walls
    Room(i).AddWall GetTex("Wall3"), -70, -46.3, -32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -46.3, -61 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -61 + 3.9, -46.3, -32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.3, 61 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 61 - 3.9, -46.3, 39 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 39 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
        
    'Right Hallways, Right Wall
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, 150, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -118 + 3.9, -70, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -102 + 3.9, -70, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -60 + 3.9, -70, 27 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 27 + 3.9, -70, 66 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 66 + 3.9, -70, 102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 102 + 3.9, -70, 116 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 116 + 3.9, -70, 150, 19.5, (i * 25) + 25, 9, 1
    
    'Left Hallways, Left Wall
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, 150, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -118 + 3.9, 70, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -102 + 3.9, 70, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -60 + 3.9, 70, 27 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 27 + 3.9, 70, 66 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 66 + 3.9, 70, 102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 102 + 3.9, 70, 116 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 116 + 3.9, 70, 150, 19.5, (i * 25) + 25, 9, 1
    
    'center bottom hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 40, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 116 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 116 - 3.9, -12.5, 102 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 102 - 3.9, -12.5, 66 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 66 - 3.9, -12.5, 40, 19.5, (i * 25) + 25, 9, 1
    
    'center bottom hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 40, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 116 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 116 - 3.9, 12.5, 102 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 102 - 3.9, 12.5, 66 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 66 - 3.9, 12.5, 40, 19.5, (i * 25) + 25, 9, 1
    
    'center top hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -118 + 3.9, -12.5, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -102 + 3.9, -12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'center top hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -118 + 3.9, 12.5, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -102 + 3.9, 12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -110, 110, -70, 110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 110, -12.5, 110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 110, 70, 110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 110, 12.5, 110, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 75, -70, 75, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 75, -12.5, 75, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 75, 70, 75, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 75, 12.5, 75, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 40, -70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 40, -12.5, 40, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 40, 70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 40, 12.5, 40, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -110, -70, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -110, -12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, -110, 70, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -71.3, -70, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -71.3, 70, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
    
    'Left Bottom Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, 40, 50, 150, 25, (i * 25) + 25, 9, 1
    
    'Right Bottom Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, 40, -50, 150, 25, (i * 25) + 25, 2, 1
    
    'Left Top Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -150, 25, (i * 25) + 25, 9, 1
    
    'Right Top Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -150, 25, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 45, -46.25, 45, -25, 25, (i * 25) + 25, 4, 1
    Room(i).AddWall GetTex("Wall3"), 45, -25, 32.5, -25, 25, (i * 25) + 25, 4, 1
    
    
    Next i
Form1.Print "done"
    'External.AddFloor GetTex("BrickTexture"), -110, -150, -85, 150, (118 * 25) + 25, 10, 10
    'External.AddFloor GetTex("BrickTexture"), 110, -150, 85, 150, (118 * 25) + 25, 10, 10

End Sub
Sub Process118to132()
    
    'Floors 118 to 132 (minus 10 feet)
    For i = 118 To 132
    Form1.Print ".";
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
     
    'External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
        
    If i = 118 Or i = 129 Then Call DrawElevatorWalls(Int(i), 5, True, True, False, True, True, False, False, False, False, False, False)
    If i >= 119 And i <= 128 Then Call DrawElevatorWalls(Int(i), 5, True, False, False, True, True, False, False, False, False, False, False)
    If i = 130 Or i = 131 Then Call DrawElevatorWalls(Int(i), 5, True, True, False, True, True, False, False, False, False, False, False)
    If i = 132 Then Call DrawElevatorWalls(Int(i), 5, False, True, True, False, False, False, False, False, False, False, False)
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 25, -12.5, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 32.5, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    'This wall is cut for the service door
    Room(i).AddWall GetTex("Wall3"), 50, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), 50, -46.3, 39 + 3.9, -46.3, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 39 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 2, 1
        
    'Top Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -60 + 3.9, -50, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -60 + 3.9, 50, -46.3, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 13 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 13 + 3.9, -50, 25, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 13 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 13 + 3.9, 50, 25, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 126 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 126 - 3.9, -12.5, 115 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 115 - 3.9, -12.5, 67 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 67 - 3.9, -12.5, 54 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 54 - 3.9, -12.5, 25, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 126 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 126 - 3.9, 12.5, 115 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 115 - 3.9, 12.5, 67 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 67 - 3.9, 12.5, 54 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 54 - 3.9, 12.5, 25, 19.5, (i * 25) + 25, 9, 1
    
    'Top Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -117 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -117 + 3.9, -12.5, -101 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -101 + 3.9, -12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -117 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -117 + 3.9, 12.5, -101 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -101 + 3.9, 12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -85, 90, -12.5, 90, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 90, 12.5, 90, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 120, -12.5, 120, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 120, 12.5, 120, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 60, -12.5, 60, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 60, 12.5, 60, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 25, -50, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -110, -12.5, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, -71.3, -50, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -20, -32.5, -20, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -20, 32.5, -20, 25, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, 4, 1
    
    Next i
Form1.Print "done"
    
End Sub

Sub DestroyObjects(Floor As Integer)
'currently, 150 objects per floor (150*138)
For i = (1 + (150 * (Floor - 1))) To (150 + (150 * (Floor - 1)))
'The destroymesh function is broken
On Error Resume Next
Objects(i).Enable False
'Sleep 2
Scene.DestroyMesh Objects(i)
Set Objects(i) = Nothing
Next i

'objectindex + (150 * (currentfloor - 1 ))

End Sub
Sub InitObjectsForFloor(Floor As Integer)

'If Test1 = False Then Exit Sub
'Test1 = False
'Exit Sub
If Floor = 1 Then

''Column 1
'i = 1 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 1)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 45
''Objects(i).Optimize

''Column 2
'i = 2 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 2)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 15
''Objects(i).Optimize

''Column 3
'i = 3 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 3)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -15
''Objects(i).Optimize

''Column 4
'i = 4 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 4)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -45
''Objects(i).Optimize

''Column 5
'i = 5 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 5)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 45
''Objects(i).Optimize

''Column 6
'i = 6 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 6)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 15
''Objects(i).Optimize

''Column 7
'i = 7 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 7)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -15
''Objects(i).Optimize

''Column 8
'i = 8 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 8)
'Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -45
''Objects(i).Optimize

'Door 1
i = 9 + (150 * (Floor - 1))
Call Init_Objects(Floor, 9)
Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, 0, -36.4

'Door 2
i = 10 + (150 * (Floor - 1))
Call Init_Objects(Floor, 10)
Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, 27, -36.4

End If

If Floor >= 2 And Floor <= 39 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 2 To 27
'Windows 2 to 27
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 1) * 12) + ((j - 1) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 28
i = 28 + (150 * (Floor - 1))
Call Init_Objects(Floor, 28)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 29 To 54
'Windows 29 to 54
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 28) * 12) + ((j - 28) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Window 55
i = 55 + (150 * (Floor - 1))
Call Init_Objects(Floor, 55)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 159.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 56 to 79
For j = 56 To 79
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 159.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 55) * 12) + ((j - 55) * 0.1)
'Objects(i).Optimize
Next j

'Window 80
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -159.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 81 to 104
For j = 81 To 104
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -159.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 80) * 12) + ((j - 80) * 0.1)
'Objects(i).Optimize
Next j

'Stairway Door
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

'Door - Room 01
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 110, (Floor * 25) + 25, -71.3

'Door - Room 03
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 45, (Floor * 25) + 25, -71.3

'Door - Room 05
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -20, (Floor * 25) + 25, -71.3

'Door - Room 07
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -45, (Floor * 25) + 25, -71.3

'Door - Room 09
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -110, (Floor * 25) + 25, -71.3

'Door - Room 02
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 108, (Floor * 25) + 25, -46.25

'Door - Room 08
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -108, (Floor * 25) + 25, -46.25

'Door - Room 04
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 87, (Floor * 25) + 25, -46.25

'Door - Room 06
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -87, (Floor * 25) + 25, -46.25

'Service Door (shaft)
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door - Room 10
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 110, (Floor * 25) + 25, 71.3

'Door - Room 12
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 45, (Floor * 25) + 25, 71.3

'Door - Room 14
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 71.3

'Door - Room 16
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -45, (Floor * 25) + 25, 71.3

'Door - Room 18
i = 120 + (150 * (Floor - 1))
Call Init_Objects(Floor, 120)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -110, (Floor * 25) + 25, 71.3

'Door - Room 11
i = 121 + (150 * (Floor - 1))
Call Init_Objects(Floor, 121)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 108, (Floor * 25) + 25, 46.25

'Door - Room 17
i = 122 + (150 * (Floor - 1))
Call Init_Objects(Floor, 122)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -108, (Floor * 25) + 25, 46.25

'Door - Room 13
i = 123 + (150 * (Floor - 1))
Call Init_Objects(Floor, 123)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 87, (Floor * 25) + 25, 46.25

'Door - Room 15
i = 124 + (150 * (Floor - 1))
Call Init_Objects(Floor, 124)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -87, (Floor * 25) + 25, 46.25

End If

If Floor >= 40 And Floor <= 79 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 134.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 134.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -134.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -134.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 129, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 72
'Windows 52 to 72
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 51) * 12) + ((j - 51) * 0.1), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 73
i = 73 + (150 * (Floor - 1))
Call Init_Objects(Floor, 73)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 129, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 74 To 94
'Windows 74 to 94
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 73) * 12) + ((j - 73) * 0.1), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Stairway Door
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

'Door 01
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, -71.3

'Door 03
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, -71.3

'Door 05
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -20, (Floor * 25) + 25, -71.3

'Door 07
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, -71.3

'Door 09
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, -71.3

'Door 02
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, -46.25

'Door 04
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 72, (Floor * 25) + 25, -46.25

'Service Room
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 40, (Floor * 25) + 25, -46.25

'Door 06
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -72, (Floor * 25) + 25, -46.25

'Door 08
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, -46.25

'Door 10a
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 55.83

'Door 12a
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, 55.83

'Door 14a
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 55.83

'Door 16a
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, 55.83

'Door 18a
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 55.83

'Door 11
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 30.83

'Door 13
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 72, (Floor * 25) + 25, 30.83

'Door 15
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -72, (Floor * 25) + 25, 30.83

'Door 17
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 30.83

'Door 10b
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 102

'Door 12b
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, 102

'Door 14b
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 102

'Door 16b
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, 102

'Door 18b
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 102



End If

If Floor >= 80 And Floor <= 117 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 109.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 109.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -109.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -109.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 103, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 68
'Windows 52 to 68
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 69
i = 69 + (150 * (Floor - 1))
Call Init_Objects(Floor, 69)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 103, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 70 To 86
'Windows 70 to 86
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 69) * 12) + ((j - 69) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Stairway Door
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor >= 80 And Floor <= 115 Then

'Door 06
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -102

'Door 07
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -102

'Door 05
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -102

'Door 08
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -102

'Door 02
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -118

'Door 03
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -118

'Door 01
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -118

'Door 04
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -118

'Door 09
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -60

'Door 11
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -60

'Service Door
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door 13
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 27

'Door 16
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 27

'Door 18
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 66

'Door 19
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 66

'Door 17
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 66

'Door 20
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 66

'Door 22
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 102

'Door 23
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 102

'Door 21
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 102

'Door 24
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 102

'Door 26
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 116

'Door 27
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 116

'Door 25
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 116

'Door 28
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 116

'Door 12
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61, (Floor * 25) + 25, -46.25

'Door 10
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61, (Floor * 25) + 25, -46.25

'Door 15
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -61, (Floor * 25) + 25, 15.41

'Door 14
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 61, (Floor * 25) + 25, 15.41


End If

If Floor >= 118 And Floor <= 134 Then
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 84.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 84.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -84.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -84.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 78, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 64
'Windows 52 to 64
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 65
i = 65 + (150 * (Floor - 1))
Call Init_Objects(Floor, 65)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 78, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 66 To 78
'Windows 66 to 78
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Stairway Door
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor >= 118 And Floor <= 132 Then

'Door 01
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -117

'Door 02
i = 81 + (150 * (Floor - 1))
Call Init_Objects(Floor, 81)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -117

'Door 03
i = 82 + (150 * (Floor - 1))
Call Init_Objects(Floor, 82)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -101

'Door 04
i = 83 + (150 * (Floor - 1))
Call Init_Objects(Floor, 83)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -101

'Door 05
i = 84 + (150 * (Floor - 1))
Call Init_Objects(Floor, 84)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 50, (Floor * 25) + 25, -60

'Door 06
i = 85 + (150 * (Floor - 1))
Call Init_Objects(Floor, 85)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -50, (Floor * 25) + 25, -60

'Service Door
i = 86 + (150 * (Floor - 1))
Call Init_Objects(Floor, 86)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door 07
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 50, (Floor * 25) + 25, 13

'Door 08
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -50, (Floor * 25) + 25, 13

'Door 09
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 54

'Door 10
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 54

'Door 11
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 67

'Door 12
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 67

'Door 13
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 115

'Door 14
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 115

'Door 15
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 126

'Door 16
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 126


End If

If Floor = 133 Or Floor = 134 Then
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).Load3DsMesh App.Path + "\objects\swimpool.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).SetRotation 0, -1.58, 0
Objects(i).ScaleMesh 0.2, 0.2, 0.2
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 0 + ((134 * 25) + 25), 92
'Objects(i).Optimize
End If

If Floor = 135 Or Floor = 136 Then
Floor = 135
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 62 To 74
'Windows 62 to 74
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Stairway Door
i = 75 + (150 * (Floor - 1))
Call Init_Objects(Floor, 75)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

'-----------------------------
Floor = 136
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 62 To 74
'Windows 62 to 74
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Stairway Door
i = 75 + (150 * (Floor - 1))
Call Init_Objects(Floor, 75)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor = 137 Then
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\piping.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.04, 0.04, 0.04
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 10 + ((Floor * 25) + 25), 60
Objects(i).Optimize

'Stairway Door
i = 2 + (150 * (Floor - 1))
Call Init_Objects(Floor, 2)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor = 138 Then

'Stairway Door
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

End Sub
Sub Init_Objects(Floor As Integer, ObjectIndex As Integer)
'currently, 150 objects per floor (150*138)
i53 = ObjectIndex + (150 * (Floor - 1))
Set Objects(i53) = New TVMesh
Set Objects(i53) = Scene.CreateMeshBuilder("Objects " + Str$(i53))
'MsgBox (Str$(i53))
'objectindex + (150 * (currentfloor - 1 ))
    
End Sub

Sub ProcessOtherFloors()
i = 133
    Form1.Print ".";
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
     
    'External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    Call DrawElevatorWalls(Int(i), 5, True, True, False, False, False, False, False, False, False, False, False)
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 25, -12.5, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, 0, 50, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
        
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 25, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 25, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Individual Rooms
    'Room(i).AddWall GetTex("Wall3"), -85, 90, -12.5, 90, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 90, 12.5, 90, 25, (i * 25) + 25, 2, 1
    'Room(i).AddWall GetTex("Wall3"), -85, 120, -12.5, 120, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 120, 12.5, 120, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 60, 85, 60, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 60, 12.5, 60, 25, (i * 25) + 25, 2, 1
    'Room(i).AddWall GetTex("Wall3"), -85, 25, -50, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -110, -12.5, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, -46.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
        

    Form1.Print ".";
    i = 134
    'Mesh.AddFloor GetTex("Granite"), -85, -150, 85, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, (i * 25) + 25, 10, 3
    'Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (i * 25) + 25, 1, 3
    
    'modified floor for pool
    Room(i).AddFloor GetTex("Granite"), -50, 0, 50, 60, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, 0, -50, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), 50, 0, 85, 150, (i * 25) + 25, 10, 3
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * 25) + 49.5, 1, 3
     
    'External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    'External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    Call DrawElevatorWalls(Int(i), 5, False, True, True, True, True, False, False, False, False, False, False)
       
'Form1.Print "done"
    'External.AddFloor GetTex("BrickTexture"), -85, -150, -60, 150, (135 * 25) + 25, 10, 10
    'External.AddFloor GetTex("BrickTexture"), 85, -150, 60, 150, (135 * 25) + 25, 10, 10
    

'Form1.Print "Processing Floors 135 to 137";
    'Floors 135 to 137 (minus 6 feet)
    
    'Floor 135
    
    'For i = 135 To 137
    Form1.Print ".";
    i = 135
    'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("Granite"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("Marble3"), -60, -15.42, 60, 0, (i * 25) + 49.5, 8, 1
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    'External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
        
    Call DrawElevatorWalls(Int(i), 6, False, True, True, False, False, False, False, False, False, False, False)
         

    'Floor 136
    Form1.Print ".";
    i = 136
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("Granite"), -60, -15.42, 60, 0, (i * 25) + 25, 8, 1
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("Marble3"), -60, -15.42, 60, 150, (i * 25) + 49.5, 8, 8.6
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    'External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, (i * 25) + 25, 0, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
        
    Call DrawElevatorWalls(Int(i), 6, False, True, True, False, False, False, False, False, False, False, False)
   
    
    'Floor 137
    Form1.Print ".";
    i = 137
    'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 49.5, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    'External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'External.AddWall GetTex("Concrete"), -60, -150, 60, -150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    'External.AddWall GetTex("Concrete"), 60, -150, 60, 150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    'External.AddWall GetTex("Concrete"), 60, 150, -60, 150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    'External.AddWall GetTex("Concrete"), -60, 150, -60, -150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, -150 + 0.1, 60 - 0.1, -150 + 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, -150 + 0.1, 60 - 0.1, 150 - 0.1, 25, (i * 25) + 25, 7, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, 150 - 0.1, -60 + 0.1, 150 - 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, 150 - 0.1, -60 + 0.1, -150 + 0.1, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 50, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 50, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 50, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 50, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 50, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 50, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
        
    Call DrawElevatorWalls(Int(i), 7, False, True, False, False, False, False, False, False, False, False, False)
        
'Form1.Print "done"


'Roof Layout
'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
i = 138
Form1.Print ".";
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("BrickTexture"), -32.5, -46.25, -12.5, -15.42, (i * 25) + 50.1, 2, 2.4
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 25, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 25, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    Call DrawElevatorWalls(Int(i), 7, False, True, False, False, False, False, False, False, False, False, False)

Form1.Print "done"
    
End Sub

Function GetVar(Location As Integer, Index As Integer) As Integer
'This function loads lines from a data file, and returns the desired value
Dim TempString As String
Dim CurrentIndex As Integer
Dim StringStart As Integer
Dim StringEnd As Integer
Dim GetVarTemp As String
CurrentIndex = 0
Open FileName For Random As #1
Get #1, Location, TempString
Close #1
For i = 1 To Len(TempString)
If Mid$(TempString, i, 1) = "," Then CurrentIndex = CurrentIndex + 1
If CurrentIndex = Index - 1 Then GoTo GetVar2
Next i
GetVar2:

If CurrentIndex > 0 Then StringStart = i + 1 Else StringStart = i
    For j = StringStart To Len(TempString) + 1
    If Mid$(TempString, j, 1) = "," Or j = Len(TempString) Then StringEnd = j + 1
    Next j
GetVarTemp = Mid$(TempString, StringStart, StringEnd - StringStart)
GetVar = Val(GetVarTemp)

End Function

Sub DrawElevatorButtons()
'Elevator Buttons
    
    'each panel can cover 60 floors
    
    
'Elevator 1

    Buttons1(135).AddWall GetTex("Button135"), -12.67, -27.85, -12.67, -27.55, 0.3, 12, -1, 1
    'Buttons1(1).AddWall GetTex("Button1"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12, -1, 1
    Buttons1(136).AddWall GetTex("Button136"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12, -1, 1
    Buttons1(137).AddWall GetTex("Button137"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12, -1, 1
    Buttons1(138).AddWall GetTex("ButtonR"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12, -1, 1
    
    Buttons1(130).AddWall GetTex("Button130"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 0.5, -1, 1
    Buttons1(131).AddWall GetTex("Button131"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 0.5, -1, 1
    Buttons1(132).AddWall GetTex("Button132"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 0.5, -1, 1
    Buttons1(133).AddWall GetTex("Button133"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 0.5, -1, 1
    Buttons1(134).AddWall GetTex("Button134"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 0.5, -1, 1
    
    Buttons1(115).AddWall GetTex("Button115"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 1, -1, 1
    Buttons1(116).AddWall GetTex("Button116"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 1, -1, 1
    Buttons1(117).AddWall GetTex("Button117"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 1, -1, 1
    Buttons1(118).AddWall GetTex("Button118"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 1, -1, 1
    Buttons1(129).AddWall GetTex("Button129"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 1, -1, 1
    
    Buttons1(110).AddWall GetTex("Button110"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 1.5, -1, 1
    Buttons1(111).AddWall GetTex("Button111"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons1(112).AddWall GetTex("Button112"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons1(113).AddWall GetTex("Button113"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 1.5, -1, 1
    Buttons1(114).AddWall GetTex("Button114"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 1.5, -1, 1
    
    Buttons1(105).AddWall GetTex("Button105"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 2, -1, 1
    Buttons1(106).AddWall GetTex("Button106"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 2, -1, 1
    Buttons1(107).AddWall GetTex("Button107"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 2, -1, 1
    Buttons1(108).AddWall GetTex("Button108"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 2, -1, 1
    Buttons1(109).AddWall GetTex("Button109"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 2, -1, 1
    
    Buttons1(100).AddWall GetTex("Button100"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 2.5, -1, 1
    Buttons1(101).AddWall GetTex("Button101"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons1(102).AddWall GetTex("Button102"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons1(103).AddWall GetTex("Button103"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 2.5, -1, 1
    Buttons1(104).AddWall GetTex("Button104"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 2.5, -1, 1
    
    Buttons1(95).AddWall GetTex("Button95"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 3, -1, 1
    Buttons1(96).AddWall GetTex("Button96"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 3, -1, 1
    Buttons1(97).AddWall GetTex("Button97"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 3, -1, 1
    Buttons1(98).AddWall GetTex("Button98"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 3, -1, 1
    Buttons1(99).AddWall GetTex("Button99"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 3, -1, 1
    
    Buttons1(90).AddWall GetTex("Button90"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 3.5, -1, 1
    Buttons1(91).AddWall GetTex("Button91"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons1(92).AddWall GetTex("Button92"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons1(93).AddWall GetTex("Button93"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 3.5, -1, 1
    Buttons1(94).AddWall GetTex("Button94"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 3.5, -1, 1
    
    Buttons1(85).AddWall GetTex("Button85"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 4, -1, 1
    Buttons1(86).AddWall GetTex("Button86"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 4, -1, 1
    Buttons1(87).AddWall GetTex("Button87"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 4, -1, 1
    Buttons1(88).AddWall GetTex("Button88"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 4, -1, 1
    Buttons1(89).AddWall GetTex("Button89"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 4, -1, 1
    
    Buttons1(80).AddWall GetTex("Button80"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 4.5, -1, 1
    Buttons1(81).AddWall GetTex("Button81"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons1(82).AddWall GetTex("Button82"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 4.5, -1, 1
    Buttons1(83).AddWall GetTex("Button83"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 4.5, -1, 1
    Buttons1(84).AddWall GetTex("Button84"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 4.5, -1, 1
    
    Buttons1(2).AddWall GetTex("Button2"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 5, -1, 1
    Buttons1(39).AddWall GetTex("Button39"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 5, -1, 1
    Buttons1(40).AddWall GetTex("Button40"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 5, -1, 1
    Buttons1(65).AddWall GetTex("Button65"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 5, -1, 1
    Buttons1(79).AddWall GetTex("Button79"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 5, -1, 1
    
    Buttons1(-1).AddWall GetTex("ButtonL"), -12.67, -27.85, -12.67, -27.55, 0.3, 12 - 5.5, -1, 1
    'Buttons1(1).AddWall GetTex("Button1"), -12.67, -27.85 - 0.4, -12.67, -27.55 - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons1(1).AddWall GetTex("Button1"), -12.67, -27.85 - 0.8, -12.67, -27.55 - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons1(1).AddWall GetTex("Button1"), -12.67, -27.85 - 1.2, -12.67, -27.55 - 1.2, 0.3, 12 - 5.5, -1, 1
    Buttons1(0).AddWall GetTex("ButtonM"), -12.67, -27.85 - 1.6, -12.67, -27.55 - 1.6, 0.3, 12 - 5.5, -1, 1

'Elevator 2

    'Buttons2(135).AddWall GetTex("Button135"), 12.67, -18.45, 12.67, -18.15, 0.3, 12, 1, 1
    'Buttons2(1).AddWall GetTex("Button1"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12, 1, 1
    'Buttons2(136).AddWall GetTex("Button136"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12, 1, 1
    'Buttons2(137).AddWall GetTex("Button137"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12, 1, 1
    'Buttons2(138).AddWall GetTex("ButtonR"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12, 1, 1
    
    'Buttons2(130).AddWall GetTex("Button130"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 0.5, 1, 1
    'Buttons2(131).AddWall GetTex("Button131"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 0.5, 1, 1
    'Buttons2(135).AddWall GetTex("Button135"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 0.5, 1, 1
    'Buttons2(133).AddWall GetTex("Button133"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 0.5, 1, 1
    'Buttons2(134).AddWall GetTex("Button134"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 0.5, 1, 1
    
    'Buttons2(115).AddWall GetTex("Button115"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 1, 1, 1
    'Buttons2(116).AddWall GetTex("Button116"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 1, 1, 1
    Buttons2(136).AddWall GetTex("Button136"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 1, 1, 1
    'Buttons2(118).AddWall GetTex("Button118"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 1, 1, 1
    'Buttons2(129).AddWall GetTex("Button129"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 1, 1, 1
    
    'Buttons2(110).AddWall GetTex("Button110"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 1.5, 1, 1
    'Buttons2(111).AddWall GetTex("Button111"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 1.5, 1, 1
    Buttons2(135).AddWall GetTex("Button135"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 1.5, 1, 1
    'Buttons2(113).AddWall GetTex("Button113"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 1.5, 1, 1
    'Buttons2(114).AddWall GetTex("Button114"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 1.5, 1, 1
    
    'Buttons2(105).AddWall GetTex("Button105"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 2, 1, 1
    'Buttons2(106).AddWall GetTex("Button106"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 2, 1, 1
    Buttons2(134).AddWall GetTex("Button134"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 2, 1, 1
    'Buttons2(108).AddWall GetTex("Button108"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 2, 1, 1
    'Buttons2(109).AddWall GetTex("Button109"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 2, 1, 1
    
    'Buttons2(100).AddWall GetTex("Button100"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 2.5, 1, 1
    'Buttons2(101).AddWall GetTex("Button101"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 2.5, 1, 1
    Buttons2(132).AddWall GetTex("Button132"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 2.5, 1, 1
    'Buttons2(103).AddWall GetTex("Button103"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 2.5, 1, 1
    'Buttons2(104).AddWall GetTex("Button104"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 2.5, 1, 1
    
    'Buttons2(95).AddWall GetTex("Button95"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 3, 1, 1
    'Buttons2(96).AddWall GetTex("Button96"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 3, 1, 1
    Buttons2(80).AddWall GetTex("Button80"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 3, 1, 1
    'Buttons2(98).AddWall GetTex("Button98"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 3, 1, 1
    'Buttons2(99).AddWall GetTex("Button99"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 3, 1, 1
    
    'Buttons2(90).AddWall GetTex("Button90"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 3.5, 1, 1
    'Buttons2(91).AddWall GetTex("Button91"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 3.5, 1, 1
    Buttons2(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 3.5, 1, 1
    'Buttons2(93).AddWall GetTex("Button93"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 3.5, 1, 1
    'Buttons2(94).AddWall GetTex("Button94"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 3.5, 1, 1
    
    'Buttons2(85).AddWall GetTex("Button85"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 4, 1, 1
    'Buttons2(86).AddWall GetTex("Button86"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 4, 1, 1
    Buttons2(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 4, 1, 1
    'Buttons2(88).AddWall GetTex("Button88"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 4, 1, 1
    'Buttons2(89).AddWall GetTex("Button89"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 4, 1, 1
    
    'Buttons2(80).AddWall GetTex("Button80"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 4.5, 1, 1
    'Buttons2(81).AddWall GetTex("Button81"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 4.5, 1, 1
    'Buttons2(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 4.5, 1, 1
    'Buttons2(83).AddWall GetTex("Button83"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 4.5, 1, 1
    'Buttons2(84).AddWall GetTex("Button84"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 4.5, 1, 1
    
    'Buttons2(2).AddWall GetTex("Button2"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 5, 1, 1
    'Buttons2(39).AddWall GetTex("Button39"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 5, 1, 1
    'Buttons2(40).AddWall GetTex("Button40"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 5, 1, 1
    'Buttons2(65).AddWall GetTex("Button65"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 5, 1, 1
    'Buttons2(79).AddWall GetTex("Button79"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 5, 1, 1
    
    'Buttons2(-1).AddWall GetTex("ButtonL"), 12.67, -18.45, 12.67, -18.15, 0.3, 12 - 5.5, 1, 1
    'Buttons2(1).AddWall GetTex("Button1"), 12.67, -18.45 + 0.4, 12.67, -18.15 + 0.4, 0.3, 12 - 5.5, 1, 1
    'Buttons2(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + 0.8, 12.67, -18.15 + 0.8, 0.3, 12 - 5.5, 1, 1
    'Buttons2(1).AddWall GetTex("Button1"), 12.67, -18.45 + 1.2, 12.67, -18.15 + 1.2, 0.3, 12 - 5.5, 1, 1
    'Buttons2(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + 1.6, 12.67, -18.15 + 1.6, 0.3, 12 - 5.5, 1, 1

'Elevator 3

    'Buttons3(135).AddWall GetTex("Button135"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12, -1, 1
    'Buttons3(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*1) - 0.4, -12.67, -27.55 + (15*1) - 0.4, 0.3, 12, -1, 1
    'Buttons3(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12, -1, 1
    'Buttons3(137).AddWall GetTex("Button137"), -12.67, -27.85 + (15*1) - 1.2, -12.67, -27.55 + (15*1) - 1.2, 0.3, 12, -1, 1
    'Buttons3(138).AddWall GetTex("ButtonR"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12, -1, 1
    
    'Buttons3(128).AddWall GetTex("Button128"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 0.5, -1, 1
    Buttons3(128).AddWall GetTex("Button128"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 0.5, -1, 1
    Buttons3(129).AddWall GetTex("Button129"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 0.5, -1, 1
    Buttons3(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 0.5, -1, 1
    'Buttons3(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 0.5, -1, 1
    
    'Buttons3(123).AddWall GetTex("Button123"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 1, -1, 1
    Buttons3(125).AddWall GetTex("Button125"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 1, -1, 1
    Buttons3(126).AddWall GetTex("Button126"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 1, -1, 1
    Buttons3(127).AddWall GetTex("Button127"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 1, -1, 1
    'Buttons3(127).AddWall GetTex("Button127"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 1, -1, 1
    
    'Buttons3(118).AddWall GetTex("Button118"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 1.5, -1, 1
    Buttons3(122).AddWall GetTex("Button122"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons3(123).AddWall GetTex("Button123"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons3(124).AddWall GetTex("Button124"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 1.5, -1, 1
    'Buttons3(122).AddWall GetTex("Button122"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 1.5, -1, 1
    
    'Buttons3(105).AddWall GetTex("Button105"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12 - 2, -1, 1
    Buttons3(119).AddWall GetTex("Button119"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 2, -1, 1
    Buttons3(120).AddWall GetTex("Button120"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 2, -1, 1
    Buttons3(121).AddWall GetTex("Button121"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 2, -1, 1
    'Buttons3(109).AddWall GetTex("Button109"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12 - 2, -1, 1
    
    'Buttons3(110).AddWall GetTex("Button110"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 2.5, -1, 1
    Buttons3(113).AddWall GetTex("Button113"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons3(114).AddWall GetTex("Button114"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons3(118).AddWall GetTex("Button118"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 2.5, -1, 1
    'Buttons3(114).AddWall GetTex("Button114"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 2.5, -1, 1
    
    'Buttons3(105).AddWall GetTex("Button105"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 3, -1, 1
    Buttons3(110).AddWall GetTex("Button110"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 3, -1, 1
    Buttons3(111).AddWall GetTex("Button111"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 3, -1, 1
    Buttons3(112).AddWall GetTex("Button112"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 3, -1, 1
    'Buttons3(109).AddWall GetTex("Button109"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 3, -1, 1
    
    'Buttons3(100).AddWall GetTex("Button100"), -12.67, -27.85 + (15 * 1), -12.67, -27.55 + (15 * 1), 0.3, 12 - 3.5, -1, 1
    Buttons3(107).AddWall GetTex("Button107"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons3(108).AddWall GetTex("Button108"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons3(109).AddWall GetTex("Button109"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 3.5, -1, 1
    'Buttons3(104).AddWall GetTex("Button104"), -12.67, -27.85 + (15 * 1) - 1.6, -12.67, -27.55 + (15 * 1) - 1.6, 0.3, 12 - 3.5, -1, 1
    
    'Buttons3(105).AddWall GetTex("Button105"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12 - 4, -1, 1
    Buttons3(104).AddWall GetTex("Button104"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 4, -1, 1
    Buttons3(105).AddWall GetTex("Button105"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 4, -1, 1
    Buttons3(106).AddWall GetTex("Button106"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 4, -1, 1
    'Buttons3(109).AddWall GetTex("Button109"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12 - 4, -1, 1
    
    'Buttons3(100).AddWall GetTex("Button100"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12 - 4.5, -1, 1
    Buttons3(101).AddWall GetTex("Button101"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons3(102).AddWall GetTex("Button102"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 4.5, -1, 1
    Buttons3(103).AddWall GetTex("Button103"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 4.5, -1, 1
    'Buttons3(104).AddWall GetTex("Button104"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12 - 4.5, -1, 1
    
    'Buttons3(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12 - 5, -1, 1
    Buttons3(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 1) - 0.4, -12.67, -27.55 + (15 * 1) - 0.4, 0.3, 12 - 5, -1, 1
    Buttons3(80).AddWall GetTex("Button80"), -12.67, -27.85 + (15 * 1) - 0.8, -12.67, -27.55 + (15 * 1) - 0.8, 0.3, 12 - 5, -1, 1
    Buttons3(100).AddWall GetTex("Button100"), -12.67, -27.85 + (15 * 1) - 1.2, -12.67, -27.55 + (15 * 1) - 1.2, 0.3, 12 - 5, -1, 1
    'Buttons3(0).AddWall GetTex("ButtonM"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12 - 5, -1, 1
    
    'Buttons3(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*1), -12.67, -27.55 + (15*1), 0.3, 12 - 5.5, -1, 1
    'Buttons3(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*1) - 0.4, -12.67, -27.55 + (15*1) - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons3(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*1) - 0.8, -12.67, -27.55 + (15*1) - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons3(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*1) - 1.2, -12.67, -27.55 + (15*1) - 1.2, 0.3, 12 - 5.5, -1, 1
    'Buttons3(0).AddWall GetTex("ButtonM"), -12.67, -27.85 + (15*1) - 1.6, -12.67, -27.55 + (15*1) - 1.6, 0.3, 12 - 5.5, -1, 1

'Elevator 4

    'Buttons4(135).AddWall GetTex("Button135"), 12.67, -18.45 + (15*1), 12.67, -18.15 + (15*1), 0.3, 12, 1, 1
    'Buttons4(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*1) + 0.4, 12.67, -18.15 + (15*1) + 0.4, 0.3, 12, 1, 1
    'Buttons4(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12, 1, 1
    'Buttons4(137).AddWall GetTex("Button137"), 12.67, -18.45 + (15*1) + 1.2, 12.67, -18.15 + (15*1) + 1.2, 0.3, 12, 1, 1
    'Buttons4(138).AddWall GetTex("ButtonR"), 12.67, -18.45 + (15*1) + 1.6, 12.67, -18.15 + (15*1) + 1.6, 0.3, 12, 1, 1
    
    'Buttons4(128).AddWall GetTex("Button128"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 0.5, 1, 1
    Buttons4(128).AddWall GetTex("Button128"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 0.5, 1, 1
    Buttons4(129).AddWall GetTex("Button129"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 0.5, 1, 1
    Buttons4(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 0.5, 1, 1
    'Buttons4(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 0.5, 1, 1
    
    'Buttons4(123).AddWall GetTex("Button123"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 1, 1, 1
    Buttons4(125).AddWall GetTex("Button125"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 1, 1, 1
    Buttons4(126).AddWall GetTex("Button126"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 1, 1, 1
    Buttons4(127).AddWall GetTex("Button127"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 1, 1, 1
    'Buttons4(127).AddWall GetTex("Button127"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 1, 1, 1
    
    'Buttons4(118).AddWall GetTex("Button118"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 1.5, 1, 1
    Buttons4(122).AddWall GetTex("Button122"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 1.5, 1, 1
    Buttons4(123).AddWall GetTex("Button123"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 1.5, 1, 1
    Buttons4(124).AddWall GetTex("Button124"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 1.5, 1, 1
    'Buttons4(122).AddWall GetTex("Button122"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 1.5, 1, 1
    
    'Buttons4(105).AddWall GetTex("Button105"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 2, 1, 1
    Buttons4(119).AddWall GetTex("Button119"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 2, 1, 1
    Buttons4(120).AddWall GetTex("Button120"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 2, 1, 1
    Buttons4(121).AddWall GetTex("Button121"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 2, 1, 1
    'Buttons4(109).AddWall GetTex("Button109"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 2, 1, 1
    
    'Buttons4(110).AddWall GetTex("Button110"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 2.5, 1, 1
    Buttons4(113).AddWall GetTex("Button113"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 2.5, 1, 1
    Buttons4(114).AddWall GetTex("Button114"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 2.5, 1, 1
    Buttons4(118).AddWall GetTex("Button118"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 2.5, 1, 1
    'Buttons4(114).AddWall GetTex("Button114"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 2.5, 1, 1
    
    'Buttons4(105).AddWall GetTex("Button105"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 3, 1, 1
    Buttons4(110).AddWall GetTex("Button110"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 3, 1, 1
    Buttons4(111).AddWall GetTex("Button111"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 3, 1, 1
    Buttons4(112).AddWall GetTex("Button112"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 3, 1, 1
    'Buttons4(109).AddWall GetTex("Button109"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 3, 1, 1
    
    'Buttons4(100).AddWall GetTex("Button100"), 12.67, -18.45 + (15 * 1), 12.67, -18.15 + (15 * 1), 0.3, 12 - 3.5, 1, 1
    Buttons4(107).AddWall GetTex("Button107"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 3.5, 1, 1
    Buttons4(108).AddWall GetTex("Button108"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 3.5, 1, 1
    Buttons4(109).AddWall GetTex("Button109"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 3.5, 1, 1
    'Buttons4(104).AddWall GetTex("Button104"), 12.67, -18.45 + (15 * 1) + 1.6, 12.67, -18.15 + (15 * 1) + 1.6, 0.3, 12 - 3.5, 1, 1
    
    'Buttons4(85).AddWall GetTex("Button85"), 12.67, -18.45 + (15*1), 12.67, -18.15 + (15*1), 0.3, 12 - 4, 1, 1
    Buttons4(104).AddWall GetTex("Button104"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 4, 1, 1
    Buttons4(105).AddWall GetTex("Button105"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 4, 1, 1
    Buttons4(106).AddWall GetTex("Button106"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 4, 1, 1
    'Buttons4(89).AddWall GetTex("Button89"), 12.67, -18.45 + (15*1) + 1.6, 12.67, -18.15 + (15*1) + 1.6, 0.3, 12 - 4, 1, 1
    
    'Buttons4(80).AddWall GetTex("Button80"), 12.67, -18.45 + (15*1), 12.67, -18.15 + (15*1), 0.3, 12 - 4.5, 1, 1
    Buttons4(101).AddWall GetTex("Button101"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 4.5, 1, 1
    Buttons4(102).AddWall GetTex("Button102"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 4.5, 1, 1
    Buttons4(103).AddWall GetTex("Button103"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 4.5, 1, 1
    'Buttons4(84).AddWall GetTex("Button84"), 12.67, -18.45 + (15*1) + 1.6, 12.67, -18.15 + (15*1) + 1.6, 0.3, 12 - 4.5, 1, 1
    
    'Buttons4(2).AddWall GetTex("Button2"), 12.67, -18.45 + (15*1), 12.67, -18.15 + (15*1), 0.3, 12 - 5, 1, 1
    Buttons4(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 1) + 0.4, 12.67, -18.15 + (15 * 1) + 0.4, 0.3, 12 - 5, 1, 1
    Buttons4(80).AddWall GetTex("Button80"), 12.67, -18.45 + (15 * 1) + 0.8, 12.67, -18.15 + (15 * 1) + 0.8, 0.3, 12 - 5, 1, 1
    Buttons4(100).AddWall GetTex("Button100"), 12.67, -18.45 + (15 * 1) + 1.2, 12.67, -18.15 + (15 * 1) + 1.2, 0.3, 12 - 5, 1, 1
    'Buttons4(79).AddWall GetTex("Button79"), 12.67, -18.45 + (15*1) + 1.6, 12.67, -18.15 + (15*1) + 1.6, 0.3, 12 - 5, 1, 1
    
    'Buttons4(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15*1), 12.67, -18.15 + (15*1), 0.3, 12 - 5.5, 1, 1
    'Buttons4(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*1) + 0.4, 12.67, -18.15 + (15*1) + 0.4, 0.3, 12 - 5.5, 1, 1
    'Buttons4(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*1) + 0.8, 12.67, -18.15 + (15*1) + 0.8, 0.3, 12 - 5.5, 1, 1
    'Buttons4(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*1) + 1.2, 12.67, -18.15 + (15*1) + 1.2, 0.3, 12 - 5.5, 1, 1
    'Buttons4(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + (15*1) + 1.6, 12.67, -18.15 + (15*1) + 1.6, 0.3, 12 - 5.5, 1, 1

'Elevator 5

    'Buttons5(96).AddWall GetTex("Button96"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12, -1, 1
    'Buttons5(97).AddWall GetTex("Button97"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12, -1, 1
    'Buttons5(98).AddWall GetTex("Button98"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12, -1, 1
    'Buttons5(99).AddWall GetTex("Button99"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12, -1, 1
    'Buttons5(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12, -1, 1
    
    'Buttons5(130).AddWall GetTex("Button130"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 0.5, -1, 1
    'Buttons5(98).AddWall GetTex("Button98"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 0.5, -1, 1
    'Buttons5(99).AddWall GetTex("Button99"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 0.5, -1, 1
    'Buttons5(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 0.5, -1, 1
    'Buttons5(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 0.5, -1, 1
    
    'Buttons5(91).AddWall GetTex("Button91"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12 - 1, -1, 1
    Buttons5(98).AddWall GetTex("Button98"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 1, -1, 1
    Buttons5(99).AddWall GetTex("Button99"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 1, -1, 1
    Buttons5(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 1, -1, 1
    'Buttons5(95).AddWall GetTex("Button95"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12 - 1, -1, 1
    
    'Buttons5(96).AddWall GetTex("Button96"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12 - 1.5, -1, 1
    Buttons5(95).AddWall GetTex("Button95"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons5(96).AddWall GetTex("Button96"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons5(97).AddWall GetTex("Button97"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 1.5, -1, 1
    'Buttons5(134).AddWall GetTex("Button134"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12 - 1.5, -1, 1
    
    'Buttons5(91).AddWall GetTex("Button91"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12 - 2, -1, 1
    Buttons5(92).AddWall GetTex("Button92"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 2, -1, 1
    Buttons5(93).AddWall GetTex("Button93"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 2, -1, 1
    Buttons5(94).AddWall GetTex("Button94"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 2, -1, 1
    'Buttons5(95).AddWall GetTex("Button95"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12 - 2, -1, 1
    
    'Buttons5(86).AddWall GetTex("Button86"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12 - 2.5, -1, 1
    Buttons5(89).AddWall GetTex("Button89"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons5(90).AddWall GetTex("Button90"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons5(91).AddWall GetTex("Button91"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 2.5, -1, 1
    'Buttons5(90).AddWall GetTex("Button90"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12 - 2.5, -1, 1
    
    'Buttons5(81).AddWall GetTex("Button81"), -12.67, -27.85 + (15 * 2), -12.67, -27.55 + (15 * 2), 0.3, 12 - 3, -1, 1
    Buttons5(86).AddWall GetTex("Button86"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 3, -1, 1
    Buttons5(87).AddWall GetTex("Button87"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 3, -1, 1
    Buttons5(88).AddWall GetTex("Button88"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 3, -1, 1
    'Buttons5(85).AddWall GetTex("Button85"), -12.67, -27.85 + (15 * 2) - 1.6, -12.67, -27.55 + (15 * 2) - 1.6, 0.3, 12 - 3, -1, 1
    
    'Buttons5(90).AddWall GetTex("Button90"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 3.5, -1, 1
    Buttons5(83).AddWall GetTex("Button83"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons5(84).AddWall GetTex("Button84"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons5(85).AddWall GetTex("Button85"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 3.5, -1, 1
    'Buttons5(94).AddWall GetTex("Button94"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 3.5, -1, 1
    
    'Buttons5(81).AddWall GetTex("Button85"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 4, -1, 1
    Buttons5(80).AddWall GetTex("Button80"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 4, -1, 1
    Buttons5(81).AddWall GetTex("Button81"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 4, -1, 1
    Buttons5(82).AddWall GetTex("Button82"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 4, -1, 1
    'Buttons5(89).AddWall GetTex("Button89"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 4, -1, 1
    
    'Buttons5(80).AddWall GetTex("Button80"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 4.5, -1, 1
    'Buttons5(80).AddWall GetTex("Button80"), -12.67, -27.85 + (15 * 2) - 0.4, -12.67, -27.55 + (15 * 2) - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons5(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 4.5, -1, 1
    'Buttons5(82).AddWall GetTex("Button82"), -12.67, -27.85 + (15 * 2) - 1.2, -12.67, -27.55 + (15 * 2) - 1.2, 0.3, 12 - 4.5, -1, 1
    'Buttons5(84).AddWall GetTex("Button84"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 4.5, -1, 1
    
    'Buttons5(2).AddWall GetTex("Button2"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 5, -1, 1
    'Buttons5(39).AddWall GetTex("Button39"), -12.67, -27.85 + (15*2) - 0.4, -12.67, -27.55 + (15*2) - 0.4, 0.3, 12 - 5, -1, 1
    'Buttons5(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 2) - 0.8, -12.67, -27.55 + (15 * 2) - 0.8, 0.3, 12 - 5, -1, 1
    'Buttons5(65).AddWall GetTex("Button65"), -12.67, -27.85 + (15*2) - 1.2, -12.67, -27.55 + (15*2) - 1.2, 0.3, 12 - 5, -1, 1
    'Buttons5(79).AddWall GetTex("Button79"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 5, -1, 1
    
    'Buttons5(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*2), -12.67, -27.55 + (15*2), 0.3, 12 - 5.5, -1, 1
    'Buttons5(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*2) - 0.4, -12.67, -27.55 + (15*2) - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons5(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*2) - 0.8, -12.67, -27.55 + (15*2) - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons5(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*2) - 1.2, -12.67, -27.55 + (15*2) - 1.2, 0.3, 12 - 5.5, -1, 1
    'Buttons5(0).AddWall GetTex("ButtonM"), -12.67, -27.85 + (15*2) - 1.6, -12.67, -27.55 + (15*2) - 1.6, 0.3, 12 - 5.5, -1, 1

'Elevator 6

    'Buttons6(96).AddWall GetTex("Button96"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12, 1, 1
    'Buttons6(97).AddWall GetTex("Button97"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12, 1, 1
    'Buttons6(98).AddWall GetTex("Button98"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12, 1, 1
    'Buttons6(99).AddWall GetTex("Button99"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12, 1, 1
    'Buttons6(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12, 1, 1
    
    'Buttons6(130).AddWall GetTex("Button130"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 0.5, 1, 1
    'Buttons6(131).AddWall GetTex("Button131"), 12.67, -18.45 + (15*2) + 0.4, 12.67, -18.15 + (15*2) + 0.4, 0.3, 12 - 0.5, 1, 1
    'Buttons6(132).AddWall GetTex("Button132"), 12.67, -18.45 + (15*2) + 0.8, 12.67, -18.15 + (15*2) + 0.8, 0.3, 12 - 0.5, 1, 1
    'Buttons6(133).AddWall GetTex("Button133"), 12.67, -18.45 + (15*2) + 1.2, 12.67, -18.15 + (15*2) + 1.2, 0.3, 12 - 0.5, 1, 1
    'Buttons6(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 0.5, 1, 1
    
    'Buttons6(91).AddWall GetTex("Button91"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12 - 1, 1, 1
    Buttons6(98).AddWall GetTex("Button98"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 1, 1, 1
    Buttons6(99).AddWall GetTex("Button99"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 1, 1, 1
    Buttons6(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 1, 1, 1
    'Buttons6(95).AddWall GetTex("Button95"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12 - 1, 1, 1
    
    'Buttons6(96).AddWall GetTex("Button96"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12 - 1.5, 1, 1
    Buttons6(95).AddWall GetTex("Button95"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 1.5, 1, 1
    Buttons6(96).AddWall GetTex("Button96"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 1.5, 1, 1
    Buttons6(97).AddWall GetTex("Button97"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 1.5, 1, 1
    'Buttons6(134).AddWall GetTex("Button134"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12 - 1.5, 1, 1
    
    'Buttons6(91).AddWall GetTex("Button91"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12 - 2, 1, 1
    Buttons6(92).AddWall GetTex("Button92"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 2, 1, 1
    Buttons6(93).AddWall GetTex("Button93"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 2, 1, 1
    Buttons6(94).AddWall GetTex("Button94"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 2, 1, 1
    'Buttons6(95).AddWall GetTex("Button95"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12 - 2, 1, 1
    
    'Buttons6(86).AddWall GetTex("Button86"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12 - 2.5, 1, 1
    Buttons6(89).AddWall GetTex("Button89"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 2.5, 1, 1
    Buttons6(90).AddWall GetTex("Button90"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 2.5, 1, 1
    Buttons6(91).AddWall GetTex("Button91"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 2.5, 1, 1
    'Buttons6(90).AddWall GetTex("Button90"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12 - 2.5, 1, 1
    
    'Buttons6(81).AddWall GetTex("Button81"), 12.67, -18.45 + (15 * 2), 12.67, -18.15 + (15 * 2), 0.3, 12 - 3, 1, 1
    Buttons6(86).AddWall GetTex("Button86"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 3, 1, 1
    Buttons6(87).AddWall GetTex("Button87"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 3, 1, 1
    Buttons6(88).AddWall GetTex("Button88"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 3, 1, 1
    'Buttons6(85).AddWall GetTex("Button85"), 12.67, -18.45 + (15 * 2) + 1.6, 12.67, -18.15 + (15 * 2) + 1.6, 0.3, 12 - 3, 1, 1
    
    'Buttons6(90).AddWall GetTex("Button90"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 3.5, 1, 1
    Buttons6(83).AddWall GetTex("Button83"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 3.5, 1, 1
    Buttons6(84).AddWall GetTex("Button84"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 3.5, 1, 1
    Buttons6(85).AddWall GetTex("Button85"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 3.5, 1, 1
    'Buttons6(94).AddWall GetTex("Button94"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 3.5, 1, 1
    
    'Buttons6(85).AddWall GetTex("Button85"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 4, 1, 1
    Buttons6(80).AddWall GetTex("Button80"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 4, 1, 1
    Buttons6(81).AddWall GetTex("Button81"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 4, 1, 1
    Buttons6(82).AddWall GetTex("Button82"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 4, 1, 1
    'Buttons6(89).AddWall GetTex("Button89"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 4, 1, 1
    
    'Buttons6(80).AddWall GetTex("Button80"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 4.5, 1, 1
    'Buttons6(80).AddWall GetTex("Button80"), 12.67, -18.45 + (15 * 2) + 0.4, 12.67, -18.15 + (15 * 2) + 0.4, 0.3, 12 - 4.5, 1, 1
    Buttons6(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 4.5, 1, 1
    'Buttons6(82).AddWall GetTex("Button82"), 12.67, -18.45 + (15 * 2) + 1.2, 12.67, -18.15 + (15 * 2) + 1.2, 0.3, 12 - 4.5, 1, 1
    'Buttons6(84).AddWall GetTex("Button84"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 4.5, 1, 1
    
    'Buttons6(2).AddWall GetTex("Button2"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 5, 1, 1
    'Buttons6(39).AddWall GetTex("Button39"), 12.67, -18.45 + (15*2) + 0.4, 12.67, -18.15 + (15*2) + 0.4, 0.3, 12 - 5, 1, 1
    'Buttons6(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 2) + 0.8, 12.67, -18.15 + (15 * 2) + 0.8, 0.3, 12 - 5, 1, 1
    'Buttons6(65).AddWall GetTex("Button65"), 12.67, -18.45 + (15*2) + 1.2, 12.67, -18.15 + (15*2) + 1.2, 0.3, 12 - 5, 1, 1
    'Buttons6(79).AddWall GetTex("Button79"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 5, 1, 1
    
    'Buttons6(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15*2), 12.67, -18.15 + (15*2), 0.3, 12 - 5.5, 1, 1
    'Buttons6(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*2) + 0.4, 12.67, -18.15 + (15*2) + 0.4, 0.3, 12 - 5.5, 1, 1
    'Buttons6(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*2) + 0.8, 12.67, -18.15 + (15*2) + 0.8, 0.3, 12 - 5.5, 1, 1
    'Buttons6(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*2) + 1.2, 12.67, -18.15 + (15*2) + 1.2, 0.3, 12 - 5.5, 1, 1
    'Buttons6(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + (15*2) + 1.6, 12.67, -18.15 + (15*2) + 1.6, 0.3, 12 - 5.5, 1, 1

'Elevator 7

    'Buttons7(135).AddWall GetTex("Button135"), -12.67, -27.85 + (15*3), -12.67, -27.55 + (15*3), 0.3, 12, -1, 1
    'Buttons7(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*3) - 0.4, -12.67, -27.55 + (15*3) - 0.4, 0.3, 12, -1, 1
    'Buttons7(136).AddWall GetTex("Button136"), -12.67, -27.85 + (15*3) - 0.8, -12.67, -27.55 + (15*3) - 0.8, 0.3, 12, -1, 1
    'Buttons7(137).AddWall GetTex("Button137"), -12.67, -27.85 + (15*3) - 1.2, -12.67, -27.55 + (15*3) - 1.2, 0.3, 12, -1, 1
    'Buttons7(138).AddWall GetTex("ButtonR"), -12.67, -27.85 + (15*3) - 1.6, -12.67, -27.55 + (15*3) - 1.6, 0.3, 12, -1, 1
    
    Buttons7(75).AddWall GetTex("Button75"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 0.5, -1, 1
    Buttons7(76).AddWall GetTex("Button76"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 0.5, -1, 1
    Buttons7(77).AddWall GetTex("Button77"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 0.5, -1, 1
    Buttons7(78).AddWall GetTex("Button78"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 0.5, -1, 1
    Buttons7(79).AddWall GetTex("Button79"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 0.5, -1, 1
    
    Buttons7(70).AddWall GetTex("Button70"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 1, -1, 1
    Buttons7(71).AddWall GetTex("Button71"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 1, -1, 1
    Buttons7(72).AddWall GetTex("Button72"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 1, -1, 1
    Buttons7(73).AddWall GetTex("Button73"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 1, -1, 1
    Buttons7(74).AddWall GetTex("Button74"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 1, -1, 1
    
    Buttons7(65).AddWall GetTex("Button65"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 1.5, -1, 1
    Buttons7(66).AddWall GetTex("Button66"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons7(67).AddWall GetTex("Button67"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons7(68).AddWall GetTex("Button68"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 1.5, -1, 1
    Buttons7(69).AddWall GetTex("Button69"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 1.5, -1, 1
    
    Buttons7(60).AddWall GetTex("Button60"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 2, -1, 1
    Buttons7(61).AddWall GetTex("Button61"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 2, -1, 1
    Buttons7(62).AddWall GetTex("Button62"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 2, -1, 1
    Buttons7(63).AddWall GetTex("Button63"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 2, -1, 1
    Buttons7(64).AddWall GetTex("Button64"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 2, -1, 1
    
    Buttons7(55).AddWall GetTex("Button55"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 2.5, -1, 1
    Buttons7(56).AddWall GetTex("Button56"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons7(57).AddWall GetTex("Button57"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons7(58).AddWall GetTex("Button58"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 2.5, -1, 1
    Buttons7(59).AddWall GetTex("Button59"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 2.5, -1, 1
    
    Buttons7(50).AddWall GetTex("Button50"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 3, -1, 1
    Buttons7(51).AddWall GetTex("Button51"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 3, -1, 1
    Buttons7(52).AddWall GetTex("Button52"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 3, -1, 1
    Buttons7(53).AddWall GetTex("Button53"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 3, -1, 1
    Buttons7(54).AddWall GetTex("Button54"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 3, -1, 1
    
    Buttons7(45).AddWall GetTex("Button45"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 3.5, -1, 1
    Buttons7(46).AddWall GetTex("Button46"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons7(47).AddWall GetTex("Button47"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons7(48).AddWall GetTex("Button48"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 3.5, -1, 1
    Buttons7(49).AddWall GetTex("Button49"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 3.5, -1, 1
    
    Buttons7(40).AddWall GetTex("Button40"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 4, -1, 1
    Buttons7(41).AddWall GetTex("Button41"), -12.67, -27.85 + (15 * 3) - 0.4, -12.67, -27.55 + (15 * 3) - 0.4, 0.3, 12 - 4, -1, 1
    Buttons7(42).AddWall GetTex("Button42"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 4, -1, 1
    Buttons7(43).AddWall GetTex("Button43"), -12.67, -27.85 + (15 * 3) - 1.2, -12.67, -27.55 + (15 * 3) - 1.2, 0.3, 12 - 4, -1, 1
    Buttons7(44).AddWall GetTex("Button44"), -12.67, -27.85 + (15 * 3) - 1.6, -12.67, -27.55 + (15 * 3) - 1.6, 0.3, 12 - 4, -1, 1
    
    'Buttons7(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 3), -12.67, -27.55 + (15 * 3), 0.3, 12 - 4.5, -1, 1
    'Buttons7(41).AddWall GetTex("Button41"), -12.67, -27.85 + (15*3) - 0.4, -12.67, -27.55 + (15*3) - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons7(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 4.5, -1, 1
    'Buttons7(43).AddWall GetTex("Button43"), -12.67, -27.85 + (15*3) - 1.2, -12.67, -27.55 + (15*3) - 1.2, 0.3, 12 - 4.5, -1, 1
    'Buttons7(44).AddWall GetTex("Button44"), -12.67, -27.85 + (15*3) - 1.6, -12.67, -27.55 + (15*3) - 1.6, 0.3, 12 - 4.5, -1, 1
    
    'Buttons7(2).AddWall GetTex("Button2"), -12.67, -27.85 + (15*3), -12.67, -27.55 + (15*3), 0.3, 12 - 5, -1, 1
    'Buttons7(39).AddWall GetTex("Button39"), -12.67, -27.85 + (15*3) - 0.4, -12.67, -27.55 + (15*3) - 0.4, 0.3, 12 - 5, -1, 1
    'Buttons7(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 3) - 0.8, -12.67, -27.55 + (15 * 3) - 0.8, 0.3, 12 - 5, -1, 1
    'Buttons7(65).AddWall GetTex("Button65"), -12.67, -27.85 + (15*3) - 1.2, -12.67, -27.55 + (15*3) - 1.2, 0.3, 12 - 5, -1, 1
    'Buttons7(79).AddWall GetTex("Button79"), -12.67, -27.85 + (15*3) - 1.6, -12.67, -27.55 + (15*3) - 1.6, 0.3, 12 - 5, -1, 1
    
    'Buttons7(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*3), -12.67, -27.55 + (15*3), 0.3, 12 - 5.5, -1, 1
    'Buttons7(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*3) - 0.4, -12.67, -27.55 + (15*3) - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons7(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*3) - 0.8, -12.67, -27.55 + (15*3) - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons7(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*3) - 1.2, -12.67, -27.55 + (15*3) - 1.2, 0.3, 12 - 5.5, -1, 1
    'Buttons7(0).AddWall GetTex("ButtonM"), -12.67, -27.85 + (15*3) - 1.6, -12.67, -27.55 + (15*3) - 1.6, 0.3, 12 - 5.5, -1, 1

'Elevator 8

    'Buttons8(135).AddWall GetTex("Button135"), 12.67, -18.45 + (15*3), 12.67, -18.15 + (15*3), 0.3, 12, 1, 1
    'Buttons8(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*3) + 0.4, 12.67, -18.15 + (15*3) + 0.4, 0.3, 12, 1, 1
    'Buttons8(136).AddWall GetTex("Button136"), 12.67, -18.45 + (15*3) + 0.8, 12.67, -18.15 + (15*3) + 0.8, 0.3, 12, 1, 1
    'Buttons8(137).AddWall GetTex("Button137"), 12.67, -18.45 + (15*3) + 1.2, 12.67, -18.15 + (15*3) + 1.2, 0.3, 12, 1, 1
    'Buttons8(138).AddWall GetTex("ButtonR"), 12.67, -18.45 + (15*3) + 1.6, 12.67, -18.15 + (15*3) + 1.6, 0.3, 12, 1, 1
    
    Buttons8(75).AddWall GetTex("Button75"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 0.5, 1, 1
    Buttons8(76).AddWall GetTex("Button76"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 0.5, 1, 1
    Buttons8(77).AddWall GetTex("Button77"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 0.5, 1, 1
    Buttons8(78).AddWall GetTex("Button78"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 0.5, 1, 1
    Buttons8(79).AddWall GetTex("Button79"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 0.5, 1, 1
    
    Buttons8(70).AddWall GetTex("Button70"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 1, 1, 1
    Buttons8(71).AddWall GetTex("Button71"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 1, 1, 1
    Buttons8(72).AddWall GetTex("Button72"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 1, 1, 1
    Buttons8(73).AddWall GetTex("Button73"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 1, 1, 1
    Buttons8(74).AddWall GetTex("Button74"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 1, 1, 1
    
    Buttons8(65).AddWall GetTex("Button65"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 1.5, 1, 1
    Buttons8(66).AddWall GetTex("Button66"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 1.5, 1, 1
    Buttons8(67).AddWall GetTex("Button67"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 1.5, 1, 1
    Buttons8(68).AddWall GetTex("Button68"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 1.5, 1, 1
    Buttons8(69).AddWall GetTex("Button69"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 1.5, 1, 1
    
    Buttons8(60).AddWall GetTex("Button60"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 2, 1, 1
    Buttons8(61).AddWall GetTex("Button61"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 2, 1, 1
    Buttons8(62).AddWall GetTex("Button62"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 2, 1, 1
    Buttons8(63).AddWall GetTex("Button63"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 2, 1, 1
    Buttons8(64).AddWall GetTex("Button64"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 2, 1, 1
    
    Buttons8(55).AddWall GetTex("Button55"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 2.5, 1, 1
    Buttons8(56).AddWall GetTex("Button56"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 2.5, 1, 1
    Buttons8(57).AddWall GetTex("Button57"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 2.5, 1, 1
    Buttons8(58).AddWall GetTex("Button58"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 2.5, 1, 1
    Buttons8(59).AddWall GetTex("Button59"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 2.5, 1, 1
    
    Buttons8(50).AddWall GetTex("Button50"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 3, 1, 1
    Buttons8(51).AddWall GetTex("Button51"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 3, 1, 1
    Buttons8(52).AddWall GetTex("Button52"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 3, 1, 1
    Buttons8(53).AddWall GetTex("Button53"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 3, 1, 1
    Buttons8(54).AddWall GetTex("Button54"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 3, 1, 1
    
    Buttons8(45).AddWall GetTex("Button45"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 3.5, 1, 1
    Buttons8(46).AddWall GetTex("Button46"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 3.5, 1, 1
    Buttons8(47).AddWall GetTex("Button47"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 3.5, 1, 1
    Buttons8(48).AddWall GetTex("Button48"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 3.5, 1, 1
    Buttons8(49).AddWall GetTex("Button49"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 3.5, 1, 1
    
    Buttons8(40).AddWall GetTex("Button40"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 4, 1, 1
    Buttons8(41).AddWall GetTex("Button41"), 12.67, -18.45 + (15 * 3) + 0.4, 12.67, -18.15 + (15 * 3) + 0.4, 0.3, 12 - 4, 1, 1
    Buttons8(42).AddWall GetTex("Button42"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 4, 1, 1
    Buttons8(43).AddWall GetTex("Button43"), 12.67, -18.45 + (15 * 3) + 1.2, 12.67, -18.15 + (15 * 3) + 1.2, 0.3, 12 - 4, 1, 1
    Buttons8(44).AddWall GetTex("Button44"), 12.67, -18.45 + (15 * 3) + 1.6, 12.67, -18.15 + (15 * 3) + 1.6, 0.3, 12 - 4, 1, 1
    
    'Buttons8(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 3), 12.67, -18.15 + (15 * 3), 0.3, 12 - 4.5, 1, 1
    'Buttons8(81).AddWall GetTex("Button81"), 12.67, -18.45 + (15*3) + 0.4, 12.67, -18.15 + (15*3) + 0.4, 0.3, 12 - 4.5, 1, 1
    Buttons8(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 4.5, 1, 1
    'Buttons8(83).AddWall GetTex("Button83"), 12.67, -18.45 + (15*3) + 1.2, 12.67, -18.15 + (15*3) + 1.2, 0.3, 12 - 4.5, 1, 1
    'Buttons8(84).AddWall GetTex("Button84"), 12.67, -18.45 + (15*3) + 1.6, 12.67, -18.15 + (15*3) + 1.6, 0.3, 12 - 4.5, 1, 1
    
    'Buttons8(2).AddWall GetTex("Button2"), 12.67, -18.45 + (15*3), 12.67, -18.15 + (15*3), 0.3, 12 - 5, 1, 1
    'Buttons8(39).AddWall GetTex("Button39"), 12.67, -18.45 + (15*3) + 0.4, 12.67, -18.15 + (15*3) + 0.4, 0.3, 12 - 5, 1, 1
    'Buttons8(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 3) + 0.8, 12.67, -18.15 + (15 * 3) + 0.8, 0.3, 12 - 5, 1, 1
    'Buttons8(65).AddWall GetTex("Button65"), 12.67, -18.45 + (15*3) + 1.2, 12.67, -18.15 + (15*3) + 1.2, 0.3, 12 - 5, 1, 1
    'Buttons8(79).AddWall GetTex("Button79"), 12.67, -18.45 + (15*3) + 1.6, 12.67, -18.15 + (15*3) + 1.6, 0.3, 12 - 5, 1, 1
    
    'Buttons8(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15*3), 12.67, -18.15 + (15*3), 0.3, 12 - 5.5, 1, 1
    'Buttons8(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*3) + 0.4, 12.67, -18.15 + (15*3) + 0.4, 0.3, 12 - 5.5, 1, 1
    'Buttons8(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*3) + 0.8, 12.67, -18.15 + (15*3) + 0.8, 0.3, 12 - 5.5, 1, 1
    'Buttons8(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*3) + 1.2, 12.67, -18.15 + (15*3) + 1.2, 0.3, 12 - 5.5, 1, 1
    'Buttons8(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + (15*3) + 1.6, 12.67, -18.15 + (15*3) + 1.6, 0.3, 12 - 5.5, 1, 1

'Elevator 9

    'Buttons9(135).AddWall GetTex("Button135"), -12.67, -27.85 + (15*4), -12.67, -27.55 + (15*4), 0.3, 12, -1, 1
    'Buttons9(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*4) - 0.4, -12.67, -27.55 + (15*4) - 0.4, 0.3, 12, -1, 1
    'Buttons9(136).AddWall GetTex("Button136"), -12.67, -27.85 + (15*4) - 0.8, -12.67, -27.55 + (15*4) - 0.8, 0.3, 12, -1, 1
    'Buttons9(137).AddWall GetTex("Button137"), -12.67, -27.85 + (15*4) - 1.2, -12.67, -27.55 + (15*4) - 1.2, 0.3, 12, -1, 1
    'Buttons9(138).AddWall GetTex("ButtonR"), -12.67, -27.85 + (15*4) - 1.6, -12.67, -27.55 + (15*4) - 1.6, 0.3, 12, -1, 1
    
    'Buttons9(37).AddWall GetTex("Button37"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 0.5, -1, 1
    Buttons9(37).AddWall GetTex("Button37"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 0.5, -1, 1
    Buttons9(38).AddWall GetTex("Button38"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 0.5, -1, 1
    Buttons9(39).AddWall GetTex("Button39"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 0.5, -1, 1
    'Buttons9(39).AddWall GetTex("Button39"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 0.5, -1, 1
    
    Buttons9(32).AddWall GetTex("Button32"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 1, -1, 1
    Buttons9(33).AddWall GetTex("Button33"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 1, -1, 1
    Buttons9(34).AddWall GetTex("Button34"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 1, -1, 1
    Buttons9(35).AddWall GetTex("Button35"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 1, -1, 1
    Buttons9(36).AddWall GetTex("Button36"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 1, -1, 1
    
    Buttons9(27).AddWall GetTex("Button27"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 1.5, -1, 1
    Buttons9(28).AddWall GetTex("Button28"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons9(29).AddWall GetTex("Button29"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons9(30).AddWall GetTex("Button30"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 1.5, -1, 1
    Buttons9(31).AddWall GetTex("Button31"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 1.5, -1, 1
    
    Buttons9(22).AddWall GetTex("Button22"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 2, -1, 1
    Buttons9(23).AddWall GetTex("Button23"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 2, -1, 1
    Buttons9(24).AddWall GetTex("Button24"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 2, -1, 1
    Buttons9(25).AddWall GetTex("Button25"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 2, -1, 1
    Buttons9(26).AddWall GetTex("Button26"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 2, -1, 1
    
    Buttons9(17).AddWall GetTex("Button17"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 2.5, -1, 1
    Buttons9(18).AddWall GetTex("Button18"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons9(19).AddWall GetTex("Button19"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons9(20).AddWall GetTex("Button20"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 2.5, -1, 1
    Buttons9(21).AddWall GetTex("Button21"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 2.5, -1, 1
    
    Buttons9(12).AddWall GetTex("Button12"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 3, -1, 1
    Buttons9(13).AddWall GetTex("Button13"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 3, -1, 1
    Buttons9(14).AddWall GetTex("Button14"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 3, -1, 1
    Buttons9(15).AddWall GetTex("Button15"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 3, -1, 1
    Buttons9(16).AddWall GetTex("Button16"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 3, -1, 1
    
    Buttons9(7).AddWall GetTex("Button7"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 3.5, -1, 1
    Buttons9(8).AddWall GetTex("Button8"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons9(9).AddWall GetTex("Button9"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons9(10).AddWall GetTex("Button10"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 3.5, -1, 1
    Buttons9(11).AddWall GetTex("Button11"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 3.5, -1, 1
    
    Buttons9(2).AddWall GetTex("Button2"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 4, -1, 1
    Buttons9(3).AddWall GetTex("Button3"), -12.67, -27.85 + (15 * 4) - 0.4, -12.67, -27.55 + (15 * 4) - 0.4, 0.3, 12 - 4, -1, 1
    Buttons9(4).AddWall GetTex("Button4"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 4, -1, 1
    Buttons9(5).AddWall GetTex("Button5"), -12.67, -27.85 + (15 * 4) - 1.2, -12.67, -27.55 + (15 * 4) - 1.2, 0.3, 12 - 4, -1, 1
    Buttons9(6).AddWall GetTex("Button6"), -12.67, -27.85 + (15 * 4) - 1.6, -12.67, -27.55 + (15 * 4) - 1.6, 0.3, 12 - 4, -1, 1
    
    'Buttons9(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 4), -12.67, -27.55 + (15 * 4), 0.3, 12 - 4.5, -1, 1
    'Buttons9(81).AddWall GetTex("Button81"), -12.67, -27.85 + (15*4) - 0.4, -12.67, -27.55 + (15*4) - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons9(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 4.5, -1, 1
    'Buttons9(83).AddWall GetTex("Button83"), -12.67, -27.85 + (15*4) - 1.2, -12.67, -27.55 + (15*4) - 1.2, 0.3, 12 - 4.5, -1, 1
    'Buttons9(84).AddWall GetTex("Button84"), -12.67, -27.85 + (15*4) - 1.6, -12.67, -27.55 + (15*4) - 1.6, 0.3, 12 - 4.5, -1, 1
    
    'Buttons9(2).AddWall GetTex("Button2"), -12.67, -27.85 + (15*4), -12.67, -27.55 + (15*4), 0.3, 12 - 5, -1, 1
    'Buttons9(39).AddWall GetTex("Button39"), -12.67, -27.85 + (15*4) - 0.4, -12.67, -27.55 + (15*4) - 0.4, 0.3, 12 - 5, -1, 1
    'Buttons9(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15 * 4) - 0.8, -12.67, -27.55 + (15 * 4) - 0.8, 0.3, 12 - 5, -1, 1
    'Buttons9(65).AddWall GetTex("Button65"), -12.67, -27.85 + (15*4) - 1.2, -12.67, -27.55 + (15*4) - 1.2, 0.3, 12 - 5, -1, 1
    'Buttons9(79).AddWall GetTex("Button79"), -12.67, -27.85 + (15*4) - 1.6, -12.67, -27.55 + (15*4) - 1.6, 0.3, 12 - 5, -1, 1
    
    'Buttons9(-1).AddWall GetTex("ButtonL"), -12.67, -27.85 + (15*4), -12.67, -27.55 + (15*4), 0.3, 12 - 5.5, -1, 1
    'Buttons9(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*4) - 0.4, -12.67, -27.55 + (15*4) - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons9(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*4) - 0.8, -12.67, -27.55 + (15*4) - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons9(1).AddWall GetTex("Button1"), -12.67, -27.85 + (15*4) - 1.2, -12.67, -27.55 + (15*4) - 1.2, 0.3, 12 - 5.5, -1, 1
    'Buttons9(0).AddWall GetTex("ButtonM"), -12.67, -27.85 + (15*4) - 1.6, -12.67, -27.55 + (15*4) - 1.6, 0.3, 12 - 5.5, -1, 1

'Elevator 10

    'Buttons10(135).AddWall GetTex("Button135"), 12.67, -18.45 + (15*4), 12.67, -18.15 + (15*4), 0.3, 12, 1, 1
    'Buttons10(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*4) + 0.4, 12.67, -18.15 + (15*4) + 0.4, 0.3, 12, 1, 1
    'Buttons10(136).AddWall GetTex("Button136"), 12.67, -18.45 + (15*4) + 0.8, 12.67, -18.15 + (15*4) + 0.8, 0.3, 12, 1, 1
    'Buttons10(137).AddWall GetTex("Button137"), 12.67, -18.45 + (15*4) + 1.2, 12.67, -18.15 + (15*4) + 1.2, 0.3, 12, 1, 1
    'Buttons10(138).AddWall GetTex("ButtonR"), 12.67, -18.45 + (15*4) + 1.6, 12.67, -18.15 + (15*4) + 1.6, 0.3, 12, 1, 1
    
    'Buttons10(37).AddWall GetTex("Button37"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 0.5, 1, 1
    Buttons10(37).AddWall GetTex("Button37"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 0.5, 1, 1
    Buttons10(38).AddWall GetTex("Button38"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 0.5, 1, 1
    Buttons10(39).AddWall GetTex("Button39"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 0.5, 1, 1
    'Buttons10(39).AddWall GetTex("Button39"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 0.5, 1, 1
    
    Buttons10(32).AddWall GetTex("Button32"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 1, 1, 1
    Buttons10(33).AddWall GetTex("Button33"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 1, 1, 1
    Buttons10(34).AddWall GetTex("Button34"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 1, 1, 1
    Buttons10(35).AddWall GetTex("Button35"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 1, 1, 1
    Buttons10(36).AddWall GetTex("Button36"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 1, 1, 1
    
    Buttons10(27).AddWall GetTex("Button27"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 1.5, 1, 1
    Buttons10(28).AddWall GetTex("Button28"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 1.5, 1, 1
    Buttons10(29).AddWall GetTex("Button29"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 1.5, 1, 1
    Buttons10(30).AddWall GetTex("Button30"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 1.5, 1, 1
    Buttons10(31).AddWall GetTex("Button31"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 1.5, 1, 1
    
    Buttons10(22).AddWall GetTex("Button22"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 2, 1, 1
    Buttons10(23).AddWall GetTex("Button23"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 2, 1, 1
    Buttons10(24).AddWall GetTex("Button24"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 2, 1, 1
    Buttons10(25).AddWall GetTex("Button25"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 2, 1, 1
    Buttons10(26).AddWall GetTex("Button26"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 2, 1, 1
    
    Buttons10(17).AddWall GetTex("Button17"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 2.5, 1, 1
    Buttons10(18).AddWall GetTex("Button18"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 2.5, 1, 1
    Buttons10(19).AddWall GetTex("Button19"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 2.5, 1, 1
    Buttons10(20).AddWall GetTex("Button20"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 2.5, 1, 1
    Buttons10(21).AddWall GetTex("Button21"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 2.5, 1, 1
    
    Buttons10(12).AddWall GetTex("Button12"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 3, 1, 1
    Buttons10(13).AddWall GetTex("Button13"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 3, 1, 1
    Buttons10(14).AddWall GetTex("Button14"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 3, 1, 1
    Buttons10(15).AddWall GetTex("Button15"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 3, 1, 1
    Buttons10(16).AddWall GetTex("Button16"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 3, 1, 1
    
    Buttons10(7).AddWall GetTex("Button7"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 3.5, 1, 1
    Buttons10(8).AddWall GetTex("Button8"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 3.5, 1, 1
    Buttons10(9).AddWall GetTex("Button9"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 3.5, 1, 1
    Buttons10(10).AddWall GetTex("Button10"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 3.5, 1, 1
    Buttons10(11).AddWall GetTex("Button11"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 3.5, 1, 1
    
    Buttons10(2).AddWall GetTex("Button2"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 4, 1, 1
    Buttons10(3).AddWall GetTex("Button3"), 12.67, -18.45 + (15 * 4) + 0.4, 12.67, -18.15 + (15 * 4) + 0.4, 0.3, 12 - 4, 1, 1
    Buttons10(4).AddWall GetTex("Button4"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 4, 1, 1
    Buttons10(5).AddWall GetTex("Button5"), 12.67, -18.45 + (15 * 4) + 1.2, 12.67, -18.15 + (15 * 4) + 1.2, 0.3, 12 - 4, 1, 1
    Buttons10(6).AddWall GetTex("Button6"), 12.67, -18.45 + (15 * 4) + 1.6, 12.67, -18.15 + (15 * 4) + 1.6, 0.3, 12 - 4, 1, 1
    
    'Buttons10(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 4), 12.67, -18.15 + (15 * 4), 0.3, 12 - 4.5, 1, 1
    'Buttons10(81).AddWall GetTex("Button81"), 12.67, -18.45 + (15*4) + 0.4, 12.67, -18.15 + (15*4) + 0.4, 0.3, 12 - 4.5, 1, 1
    Buttons10(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 4.5, 1, 1
    'Buttons10(83).AddWall GetTex("Button83"), 12.67, -18.45 + (15*4) + 1.2, 12.67, -18.15 + (15*4) + 1.2, 0.3, 12 - 4.5, 1, 1
    'Buttons10(84).AddWall GetTex("Button84"), 12.67, -18.45 + (15*4) + 1.6, 12.67, -18.15 + (15*4) + 1.6, 0.3, 12 - 4.5, 1, 1
    
    'Buttons10(2).AddWall GetTex("Button2"), 12.67, -18.45 + (15*4), 12.67, -18.15 + (15*4), 0.3, 12 - 5, 1, 1
    'Buttons10(39).AddWall GetTex("Button39"), 12.67, -18.45 + (15*4) + 0.4, 12.67, -18.15 + (15*4) + 0.4, 0.3, 12 - 5, 1, 1
    'Buttons10(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15 * 4) + 0.8, 12.67, -18.15 + (15 * 4) + 0.8, 0.3, 12 - 5, 1, 1
    'Buttons10(65).AddWall GetTex("Button65"), 12.67, -18.45 + (15*4) + 1.2, 12.67, -18.15 + (15*4) + 1.2, 0.3, 12 - 5, 1, 1
    'Buttons10(79).AddWall GetTex("Button79"), 12.67, -18.45 + (15*4) + 1.6, 12.67, -18.15 + (15*4) + 1.6, 0.3, 12 - 5, 1, 1
    
    'Buttons10(-1).AddWall GetTex("ButtonL"), 12.67, -18.45 + (15*4), 12.67, -18.15 + (15*4), 0.3, 12 - 5.5, 1, 1
    'Buttons10(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*4) + 0.4, 12.67, -18.15 + (15*4) + 0.4, 0.3, 12 - 5.5, 1, 1
    'Buttons10(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*4) + 0.8, 12.67, -18.15 + (15*4) + 0.8, 0.3, 12 - 5.5, 1, 1
    'Buttons10(1).AddWall GetTex("Button1"), 12.67, -18.45 + (15*4) + 1.2, 12.67, -18.15 + (15*4) + 1.2, 0.3, 12 - 5.5, 1, 1
    'Buttons10(0).AddWall GetTex("ButtonM"), 12.67, -18.45 + (15*4) + 1.6, 12.67, -18.15 + (15*4) + 1.6, 0.3, 12 - 5.5, 1, 1

   
End Sub

Sub CheckElevatorButtons()
'collision routine for checking if an elevator button is pressed

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons1(-1).GetMeshName Then
    Buttons1(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(1) = True
    OpenElevator(1) = -1
    GotoFloor(1) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons2(-1).GetMeshName Then
    Buttons2(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(2) = True
    OpenElevator(2) = -1
    GotoFloor(2) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons3(-1).GetMeshName Then
    Buttons3(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(3) = True
    OpenElevator(3) = -1
    GotoFloor(3) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons4(-1).GetMeshName Then
    Buttons4(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(4) = True
    OpenElevator(4) = -1
    GotoFloor(4) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons5(-1).GetMeshName Then
    Buttons5(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(5) = True
    OpenElevator(5) = -1
    GotoFloor(5) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons6(-1).GetMeshName Then
    Buttons6(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(6) = True
    OpenElevator(6) = -1
    GotoFloor(6) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons7(-1).GetMeshName Then
    Buttons7(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(7) = True
    OpenElevator(7) = -1
    GotoFloor(7) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons8(-1).GetMeshName Then
    Buttons8(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(8) = True
    OpenElevator(8) = -1
    GotoFloor(8) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons9(-1).GetMeshName Then
    Buttons9(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(9) = True
    OpenElevator(9) = -1
    GotoFloor(9) = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons10(-1).GetMeshName Then
    Buttons10(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(10) = True
    OpenElevator(10) = -1
    GotoFloor(10) = -1
End If

'*******************************
If CollisionResult.GetCollisionMesh.GetMeshName = Buttons1(0).GetMeshName Then
    Buttons1(0).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(1) = True
    OpenElevator(1) = -1
    GotoFloor(1) = 0.1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons2(0).GetMeshName Then
    Buttons2(0).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(2) = True
    OpenElevator(2) = -1
    GotoFloor(2) = 0.1
End If

'*************************************

For i52 = 2 To 138
If CollisionResult.GetCollisionMesh.GetMeshName = Buttons1(i52).GetMeshName Then
    Buttons1(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(ElevatorNumber) = True
    OpenElevator(ElevatorNumber) = -1
    GotoFloor(ElevatorNumber) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons2(i52).GetMeshName Then
    Buttons2(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(2) = True
    OpenElevator(2) = -1
    GotoFloor(2) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons3(i52).GetMeshName Then
    Buttons3(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(3) = True
    OpenElevator(3) = -1
    GotoFloor(3) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons4(i52).GetMeshName Then
    Buttons4(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(4) = True
    OpenElevator(4) = -1
    GotoFloor(4) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons5(i52).GetMeshName Then
    Buttons5(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(5) = True
    OpenElevator(5) = -1
    GotoFloor(5) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons6(i52).GetMeshName Then
    Buttons6(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(6) = True
    OpenElevator(6) = -1
    GotoFloor(6) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons7(i52).GetMeshName Then
    Buttons7(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(7) = True
    OpenElevator(7) = -1
    GotoFloor(7) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons8(i52).GetMeshName Then
    Buttons8(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(8) = True
    OpenElevator(8) = -1
    GotoFloor(8) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons9(i52).GetMeshName Then
    Buttons9(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(9) = True
    OpenElevator(9) = -1
    GotoFloor(9) = i52
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons10(i52).GetMeshName Then
    Buttons10(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(10) = True
    OpenElevator(10) = -1
    GotoFloor(10) = i52
End If

Next i52
End Sub

Sub OpenDoor()
OpeningDoor = OpeningDoor + 1

'DoorRotated values:
'0 is horizontal, opens downward
'1 is vertical, opens to the left
'2 is horizontal, opens upward
'3 is vertical, opens to the right
On Error Resume Next

Objects(DoorNumber).RotateY (OpeningDoor / 110)
If DoorRotated = 0 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (OpeningDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (OpeningDoor / 38)
If DoorRotated = 1 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (OpeningDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (OpeningDoor / 48)
If DoorRotated = 2 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (OpeningDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (OpeningDoor / 38)
If DoorRotated = 3 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (OpeningDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (OpeningDoor / 48)

If OpeningDoor = 17 Then
    OpeningDoor = 0
    If DoorRotated = 0 Then Objects(DoorNumber).SetMeshName ("DoorAO " + Str$(DoorNumber))
    If DoorRotated = 1 Then Objects(DoorNumber).SetMeshName ("DoorBO " + Str$(DoorNumber))
    If DoorRotated = 2 Then Objects(DoorNumber).SetMeshName ("DoorCO " + Str$(DoorNumber))
    If DoorRotated = 3 Then Objects(DoorNumber).SetMeshName ("DoorDO " + Str$(DoorNumber))
End If
End Sub

Sub CloseDoor()
ClosingDoor = ClosingDoor - 1
On Error Resume Next
Objects(DoorNumber).RotateY -(ClosingDoor / 110)
If DoorRotated = 0 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (ClosingDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (ClosingDoor / 38)
If DoorRotated = 1 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (ClosingDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (ClosingDoor / 48)
If DoorRotated = 2 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (ClosingDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (ClosingDoor / 38)
If DoorRotated = 3 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (ClosingDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (ClosingDoor / 48)

If ClosingDoor = 0 And DoorRotated = 0 Then Objects(DoorNumber).SetMeshName ("DoorA " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 1 Then Objects(DoorNumber).SetMeshName ("DoorB " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 2 Then Objects(DoorNumber).SetMeshName ("DoorC " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 3 Then Objects(DoorNumber).SetMeshName ("DoorD " + Str$(DoorNumber))

End Sub

Sub SlowToFPS(ByVal FrameRate As Long)
Dim lngTicksPerFrame As Long
Static lngOldTickCount As Long
lngTicksPerFrame = 1000 / FrameRate
While GetTickCount() < lngOldTickCount
Sleep 5
Wend
lngOldTickCount = GetTickCount() + lngTicksPerFrame
End Sub

Sub ProcessOutside()

'Outside of Building
    External.AddFloor GetTex("Walkway"), -160, 150, 160, 200, 0, 4, 1
    
    External.AddFloor GetTex("Road1"), -160, 200, 160, 260, 0, 4, 1
    External.AddFloor GetTex("Road1"), -160, 260, 160, 320, 0, 4, -1
    External.AddFloor GetTex("Road1"), 230, 200, 550, 260, 0, 4, 1
    External.AddFloor GetTex("Road1"), 230, 260, 550, 320, 0, 4, -1
    External.AddFloor GetTex("Road1"), 620, 200, 940, 260, 0, 4, 1
    External.AddFloor GetTex("Road1"), 620, 260, 940, 320, 0, 4, -1
    
    External.AddFloor GetTex("Road4"), 230, 200, 160, 260, 0, 1, -1
    External.AddFloor GetTex("Road4"), 230, 260, 160, 320, 0, 1, 1
    External.AddFloor GetTex("Road4"), -230, 200, -160, 260, 0, 1, -1
    External.AddFloor GetTex("Road4"), -230, 260, -160, 320, 0, 1, 1
    
    External.AddFloor GetTex("Road5"), -230, -150, -160, 200, 0, 1, 5
    External.AddFloor GetTex("Road5"), -230, 320, -160, 670, 0, 1, 5
    External.AddFloor GetTex("Road5"), 230, -150, 160, 200, 0, 1, 5
    External.AddFloor GetTex("Road5"), 230, 320, 160, 670, 0, 1, 5
    
    External.AddFloor GetTex("Road5"), -620, -150, -550, 200, 0, 1, 5
    External.AddFloor GetTex("Road5"), -620, 320, -550, 670, 0, 1, 5
    External.AddFloor GetTex("Road5"), 620, -150, 550, 200, 0, 1, 5
    External.AddFloor GetTex("Road5"), 620, 320, 550, 670, 0, 1, 5
    
    'Other Towers
    
    '*** Building directly south of the Triton Center
    External.AddFloor GetTex("Walkway"), -160, 320, 160, 670, -0.1, 4, 10
    Buildings.AddWall GetTex("Windows11"), -160, 320, 160, 320, 25 * 15, 0, 10, 15
    Buildings.AddWall GetTex("Windows11"), -160, 670, 160, 670, 25 * 15, 0, 10, 15
    Buildings.AddWall GetTex("Windows11"), -160, 320, -160, 670, 25 * 15, 0, 10, 15
    Buildings.AddWall GetTex("Windows11"), 160, 320, 160, 670, 25 * 15, 0, 10, 15
    Buildings.AddFloor GetTex("Concrete"), -160, 320, 160, 670, 25 * 15, 30, 30
    
    '*** Building directly west of the Triton Center
    External.AddFloor GetTex("Walkway"), 230, -200, 550, 200, -0.1, 4, 10
    Buildings.AddWall GetTex("Windows11"), 500, -150, 280, -150, 25 * 86, 0, 4, 86
    Buildings.AddWall GetTex("Windows11"), 500, 200, 280, 200, 25 * 86, 0, 4, 86
    Buildings.AddWall GetTex("Windows11"), 500, -150, 500, 200, 25 * 86, 0, 4, 86
    Buildings.AddWall GetTex("Windows11"), 280, -150, 280, 200, 25 * 86, 0, 4, 86
    Buildings.AddFloor GetTex("Concrete"), 500, -150, 280, 200, 25 * 86, 30, 30
    
    '*** Building directly east of the Triton Center
    External.AddFloor GetTex("Walkway"), -230, -200, -550, 200, -0.1, 4, 10
    Buildings.AddWall GetTex("Windows11"), -450, -100, -300, -100, 25 * 45, 0, 4, 45
    Buildings.AddWall GetTex("Windows11"), -450, 100, -300, 100, 25 * 45, 0, 4, 45
    Buildings.AddWall GetTex("Windows11"), -450, -100, -450, 100, 25 * 45, 0, 4, 45
    Buildings.AddWall GetTex("Windows11"), -300, -100, -300, 100, 25 * 45, 0, 4, 45
    Buildings.AddFloor GetTex("Concrete"), -450, -100, -300, 100, 25 * 45, 30, 30
    
    '*** 2 buildings west of the Triton Center
    
    External.AddFloor GetTex("Walkway"), 620, -200, 940, 200, -0.1, 4, 10
    Buildings.AddWall GetTex("Windows11"), 900, -100, 750, -100, 25 * 25, 0, 2, 25
    Buildings.AddWall GetTex("Windows11"), 900, 100, 750, 100, 25 * 25, 0, 2, 25
    Buildings.AddWall GetTex("Windows11"), 900, -100, 900, 100, 25 * 25, 0, 4, 25
    Buildings.AddWall GetTex("Windows11"), 750, -100, 750, 100, 25 * 25, 0, 4, 25
    Buildings.AddFloor GetTex("Concrete"), 900, -100, 750, 100, 25 * 25, 30, 30
    
    '*** 2 buildings east of the Triton Center
    
    External.AddFloor GetTex("Walkway"), -620, -200, -940, 200, -0.1, 4, 10
    Buildings.AddWall GetTex("Windows11"), -920, -100, -650, -100, 25 * 5, 0, 2, 5
    Buildings.AddWall GetTex("Windows11"), -920, 100, -650, 100, 25 * 5, 0, 2, 5
    Buildings.AddWall GetTex("Windows11"), -920, -100, -920, 100, 25 * 5, 0, 4, 5
    Buildings.AddWall GetTex("Windows11"), -650, -100, -650, 100, 25 * 5, 0, 4, 5
    Buildings.AddFloor GetTex("Concrete"), -920, -100, -650, 100, 25 * 5, 30, 30
    
    
    External.AddWall GetTex("LobbyFront"), -160, -150, 160, -150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, -150, 160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, 150, -160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -160, 150, -160, -150, 75, 0, 3, 1

For i = 2 To 39
    External.AddWall GetTex("MainWindows"), -160, -150, 160, -150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), 160, -150, 160, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 160, 150, -160, 150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), -160, 150, -160, -150, 25, (i * 25) + 25, 7, 1
Next i


For i = 40 To 79
    External.AddWall GetTex("MainWindows"), -135, -150, 135, -150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), 135, -150, 135, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 135, 150, -135, 150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), -135, 150, -135, -150, 25, (i * 25) + 25, 7, 1
Next i

For i = 80 To 117
    External.AddWall GetTex("MainWindows"), -110, -150, 110, -150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 110, -150, 110, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 110, 150, -110, 150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), -110, 150, -110, -150, 25, (i * 25) + 25, 7, 1
Next i
    
For i = 118 To 134
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
  
Next i

For i = 135 To 136
    External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
Next i

i = 137
    External.AddWall GetTex("Concrete"), -60, -150, 60, -150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, -150, 60, 150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, 150, -60, 150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), -60, 150, -60, -150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    
'Landings
    Buildings.AddFloor GetTex("BrickTexture"), -160, -150, -135, 150, (40 * 25) + 25, 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 160, -150, 135, 150, (40 * 25) + 25, 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -135, -150, -110, 150, (80 * 25) + 25, 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 135, -150, 110, 150, (80 * 25) + 25, 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -110, -150, -85, 150, (118 * 25) + 25, 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 110, -150, 85, 150, (118 * 25) + 25, 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -85, -150, -60, 150, (135 * 25) + 25, 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 85, -150, 60, 150, (135 * 25) + 25, 10, 10

'Antennae
    '1
    Buildings.AddWall GetTex("Concrete"), -25, 10, -25, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 15, -25, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 10, -25, 10, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 10, -20, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    
    '2
    Buildings.AddWall GetTex("Concrete"), 25, 10, 25, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 15, 25, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 10, 25, 10, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 10, 20, 15, 20 * 25, (138 * 25) + 25, 1 * 2, 20 * 4
    
End Sub

Sub MainLoop()

'Calls frame limiter function, which sets the max frame rate
'note - the frame rate determines elevator speed, walking speed, etc
'In order to raise it, elevator timers and walking speed must both be changed
SlowToFPS (20)

If OpeningDoor > 0 Then Call OpenDoor
If ClosingDoor > 0 Then Call CloseDoor

'Determine floor that the camera is on, if the camera is not in the stairwell
If InStairwell = False Then CameraFloor = (Camera.GetPosition.Y - 25 - 10) / 25
If CameraFloor < 1 Then CameraFloor = 1
            
If Plaque(ElevatorNumber).IsMeshEnabled = False Then InElevator = False
If GotoFloor(ElevatorNumber) = 0 Then ElevatorSync(ElevatorNumber) = False

'This code checks to see if the camera is in an elevator or not (to draw the elevator buttons, set current elevator, etc)
'It draws a testing line below the camera, to see if the line hits the floor of an elevator.
    
For i50 = 1 To 10
    
    'This code fixes a bug where the camera's height changes in the elevator if the user moves
    If ElevatorSync(i50) = True And GotoFloor(i50) <> 0 And OpenElevator(i50) = 0 Then Camera.SetPosition Camera.GetPosition.X, Elevator(i50).GetPosition.Y + 10, Camera.GetPosition.z
    
    'detects if the person is in the elevator
    Elevator(i50).SetCollisionEnable True
    If Elevator(i50).Collision(Camera.GetPosition, Vector(Camera.GetPosition.X, Camera.GetPosition.Y - 15, Camera.GetPosition.z), TV_TESTTYPE_ACCURATETESTING) = True Then
        ElevatorNumber = i50
        InElevator = True
                
        'displays 3 floors of the inside shaft while the elevator's moving
        If ElevatorSync(i50) = True And OpenElevator(i50) = 0 And GotoFloor(i50) <> 0 Then
            If ElevatorFloor(i50) > 1 And (i50 = 2 Or i50 = 4 Or i50 = 6 Or i50 = 8 Or i50 = 10) Then Shafts2(ElevatorFloor(i50) - 1).Enable True
            If ElevatorFloor(i50) > 1 And (i50 = 1 Or i50 = 3 Or i50 = 5 Or i50 = 7 Or i50 = 9) Then Shafts1(ElevatorFloor(i50) - 1).Enable True
            If ElevatorFloor(i50) < 138 And (i50 = 2 Or i50 = 4 Or i50 = 6 Or i50 = 8 Or i50 = 10) Then Shafts2(ElevatorFloor(i50) + 1).Enable True
            If ElevatorFloor(i50) < 138 And (i50 = 1 Or i50 = 3 Or i50 = 5 Or i50 = 7 Or i50 = 9) Then Shafts1(ElevatorFloor(i50) + 1).Enable True
            If i50 = 2 Or i50 = 4 Or i50 = 6 Or i50 = 8 Or i50 = 10 Then Shafts2(ElevatorFloor(i50)).Enable True
            If i50 = 1 Or i50 = 3 Or i50 = 5 Or i50 = 7 Or i50 = 9 Then Shafts1(ElevatorFloor(i50)).Enable True
            If ElevatorFloor(i50) > 2 And (i50 = 2 Or i50 = 4 Or i50 = 6 Or i50 = 8 Or i50 = 10) Then Shafts2(ElevatorFloor(i50) - 2).Enable False
            If ElevatorFloor(i50) > 2 And (i50 = 1 Or i50 = 3 Or i50 = 5 Or i50 = 7 Or i50 = 9) Then Shafts1(ElevatorFloor(i50) - 2).Enable False
            If ElevatorFloor(i50) < 137 And (i50 = 2 Or i50 = 4 Or i50 = 6 Or i50 = 8 Or i50 = 10) Then Shafts2(ElevatorFloor(i50) + 2).Enable False
            If ElevatorFloor(i50) < 137 And (i50 = 1 Or i50 = 3 Or i50 = 5 Or i50 = 7 Or i50 = 9) Then Shafts1(ElevatorFloor(i50) + 2).Enable False
        Else
            If ElevatorFloor(i50) > 1 Then Shafts1(ElevatorFloor(i50) - 1).Enable False: Shafts2(ElevatorFloor(i50) - 1).Enable False
            If ElevatorFloor(i50) < 138 Then Shafts1(ElevatorFloor(i50) + 1).Enable False: Shafts2(ElevatorFloor(i50) + 1).Enable False
            Shafts1(ElevatorFloor(i50)).Enable False
            Shafts2(ElevatorFloor(i50)).Enable False
            If ElevatorFloor(i50) > 2 Then Shafts1(ElevatorFloor(i50) - 2).Enable False: Shafts2(ElevatorFloor(i50) - 2).Enable False
            If ElevatorFloor(i50) < 137 Then Shafts1(ElevatorFloor(i50) + 2).Enable False: Shafts2(ElevatorFloor(i50) + 2).Enable False
        End If
        
        If Plaque(i50).IsMeshEnabled = False Then
            Plaque(i50).Enable True
            FloorIndicator(i50).Enable True
            Plaque(i50).SetPosition Plaque(i50).GetPosition.X, Plaque(i50).GetPosition.Y + ((CurrentFloorExact(i50) - ((Plaque(i50).GetPosition.Y - 25) / 25)) * 25), Plaque(i50).GetPosition.z
            FloorIndicator(i50).SetPosition FloorIndicator(i50).GetPosition.X, FloorIndicator(i50).GetPosition.Y + ((CurrentFloorExact(i50) - ((FloorIndicator(i50).GetPosition.Y - 25) / 25)) * 25), FloorIndicator(i50).GetPosition.z
            For j50 = -1 To 138
                If i50 = 1 Then Buttons1(j50).SetPosition Buttons1(j50).GetPosition.X, Buttons1(j50).GetPosition.Y + ((CurrentFloorExact(1) - ((Buttons1(j50).GetPosition.Y - 25) / 25)) * 25), Buttons1(j50).GetPosition.z
                If i50 = 2 Then Buttons2(j50).SetPosition Buttons2(j50).GetPosition.X, Buttons2(j50).GetPosition.Y + ((CurrentFloorExact(2) - ((Buttons2(j50).GetPosition.Y - 25) / 25)) * 25), Buttons2(j50).GetPosition.z
                If i50 = 3 Then Buttons3(j50).SetPosition Buttons3(j50).GetPosition.X, Buttons3(j50).GetPosition.Y + ((CurrentFloorExact(3) - ((Buttons3(j50).GetPosition.Y - 25) / 25)) * 25), Buttons3(j50).GetPosition.z
                If i50 = 4 Then Buttons4(j50).SetPosition Buttons4(j50).GetPosition.X, Buttons4(j50).GetPosition.Y + ((CurrentFloorExact(4) - ((Buttons4(j50).GetPosition.Y - 25) / 25)) * 25), Buttons4(j50).GetPosition.z
                If i50 = 5 Then Buttons5(j50).SetPosition Buttons5(j50).GetPosition.X, Buttons5(j50).GetPosition.Y + ((CurrentFloorExact(5) - ((Buttons5(j50).GetPosition.Y - 25) / 25)) * 25), Buttons5(j50).GetPosition.z
                If i50 = 6 Then Buttons6(j50).SetPosition Buttons6(j50).GetPosition.X, Buttons6(j50).GetPosition.Y + ((CurrentFloorExact(6) - ((Buttons6(j50).GetPosition.Y - 25) / 25)) * 25), Buttons6(j50).GetPosition.z
                If i50 = 7 Then Buttons7(j50).SetPosition Buttons7(j50).GetPosition.X, Buttons7(j50).GetPosition.Y + ((CurrentFloorExact(7) - ((Buttons7(j50).GetPosition.Y - 25) / 25)) * 25), Buttons7(j50).GetPosition.z
                If i50 = 8 Then Buttons8(j50).SetPosition Buttons8(j50).GetPosition.X, Buttons8(j50).GetPosition.Y + ((CurrentFloorExact(8) - ((Buttons8(j50).GetPosition.Y - 25) / 25)) * 25), Buttons8(j50).GetPosition.z
                If i50 = 9 Then Buttons9(j50).SetPosition Buttons9(j50).GetPosition.X, Buttons9(j50).GetPosition.Y + ((CurrentFloorExact(9) - ((Buttons9(j50).GetPosition.Y - 25) / 25)) * 25), Buttons9(j50).GetPosition.z
                If i50 = 10 Then Buttons10(j50).SetPosition Buttons10(j50).GetPosition.X, Buttons10(j50).GetPosition.Y + ((CurrentFloorExact(10) - ((Buttons10(j50).GetPosition.Y - 25) / 25)) * 25), Buttons10(j50).GetPosition.z
                If i50 = 1 Then Buttons1(j50).Enable True
                If i50 = 2 Then Buttons2(j50).Enable True
                If i50 = 3 Then Buttons3(j50).Enable True
                If i50 = 4 Then Buttons4(j50).Enable True
                If i50 = 5 Then Buttons5(j50).Enable True
                If i50 = 6 Then Buttons6(j50).Enable True
                If i50 = 7 Then Buttons7(j50).Enable True
                If i50 = 8 Then Buttons8(j50).Enable True
                If i50 = 9 Then Buttons9(j50).Enable True
                If i50 = 10 Then Buttons10(j50).Enable True
            Next j50
        End If
    Else
    
    If Plaque(i50).IsMeshEnabled = True And ElevatorSync(i50) = False Then
            Plaque(i50).Enable False
            FloorIndicator(i50).Enable False
            For j50 = -1 To 138
                If i50 = 1 Then Buttons1(j50).Enable False
                If i50 = 2 Then Buttons2(j50).Enable False
                If i50 = 3 Then Buttons3(j50).Enable False
                If i50 = 4 Then Buttons4(j50).Enable False
                If i50 = 5 Then Buttons5(j50).Enable False
                If i50 = 6 Then Buttons6(j50).Enable False
                If i50 = 7 Then Buttons7(j50).Enable False
                If i50 = 8 Then Buttons8(j50).Enable False
                If i50 = 9 Then Buttons9(j50).Enable False
                If i50 = 10 Then Buttons10(j50).Enable False
            Next j50
        End If
    End If
Next i50
   
'This code changes the elevator floor indicator picture
For i50 = 1 To 10
FloorIndicatorPic(i50) = Str$(ElevatorFloor(i50))
FloorIndicatorPic(i50) = "Button" + Mid$(FloorIndicatorPic(i50), 2)
If ElevatorFloor(i50) = 1 Then FloorIndicatorPic(i50) = "ButtonL"
If ElevatorFloor(i50) = 1 And FloorIndicatorText(i50) = "M" Then FloorIndicatorPic(i50) = "ButtonM"
If ElevatorFloor(i50) = 138 Then FloorIndicatorPic(i50) = "ButtonR"
If FloorIndicatorPic(i50) <> FloorIndicatorPicOld(i50) Then
    
    'TextureFactory.DeleteTexture GetTex("FloorIndicator" + Str$(i50))
    'TextureFactory.LoadTexture App.Path + "\data\floorindicators\" + FloorIndicatorPic(i50), "FloorIndicator" + Str$(i50)
    FloorIndicator(i50).SetTexture GetTex(FloorIndicatorPic(i50))
    
End If
FloorIndicatorPicOld(i50) = FloorIndicatorPic(i50)

ElevatorFloor2(i50) = ((Elevator(i50).GetPosition.Y - 10) / 25) - 1

'Update the floor indicator
FloorIndicatorText(i50) = Str$(ElevatorFloor(i50))
If ElevatorFloor(i50) = 1 Then FloorIndicatorText(i50) = "L"
If ElevatorFloor(i50) = 138 Then FloorIndicatorText(i50) = "R"
If Elevator(i50).GetPosition.Y > 27 And ElevatorFloor(i50) < 2 Then FloorIndicatorText(i50) = "M"

Next i50
   
Form2.Label1.Caption = FloorIndicatorText(1)
Form2.Label2.Caption = FloorIndicatorText(2)
Form2.Label3.Caption = FloorIndicatorText(3)
Form2.Label4.Caption = FloorIndicatorText(4)
Form2.Label5.Caption = FloorIndicatorText(5)
Form2.Label6.Caption = FloorIndicatorText(6)
Form2.Label7.Caption = FloorIndicatorText(7)
Form2.Label8.Caption = FloorIndicatorText(8)
Form2.Label9.Caption = FloorIndicatorText(9)
Form2.Label10.Caption = FloorIndicatorText(10)
   
'Simple Frame Limiter

'If TV.GetFPS > 20 And GotoFloor = 0 Then
'Form1.MainTimer.Interval = (TV.GetFPS - 20) * 1.5
'End If
'If GotoFloor(ElevatorNumber) <> 0 Then
'Form1.MainTimer.Interval = 45
'Else
'End If
'If TV.GetFPS < 20 Then
'Form1.MainTimer.Interval = 1
'End If


Dim a As Single
''update lights
'a = a + TV.TimeElapsed * 0.001
'      LightD.Position = Vector(0, 10, Sin(a) * 50 + 50)
'      Light.UpdateLight 1, LightD
      
      

CameraFloor2 = ((Camera.GetPosition.Y - 10) / 25) - 1

'this determines if the person is inside the stairwell shaft or not, and sets a variable accordingly.
If Camera.GetPosition.X < -12.5 And Camera.GetPosition.X > -32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30 Then
InStairwell = True
Else
InStairwell = False
End If

If CameraFloor = 137 Then
For i50 = 1 To 138
Shafts2(i50).Enable True
Next i50
Else
'For i50 = 1 To 138
'Shafts(i50).Enable False
'Next i50
End If

'floors 135 and 136 are combined. this simply makes sure that they are off when not in use :)

If CameraFloor <> 136 And CameraFloor <> 135 Then
Room(136).Enable False
ElevatorDoor1L(136).Enable False
ElevatorDoor1R(136).Enable False
ElevatorDoor1L(135).Enable False
ElevatorDoor1R(135).Enable False
ElevatorDoor2L(136).Enable False
ElevatorDoor2R(136).Enable False
ElevatorDoor2L(135).Enable False
ElevatorDoor2R(135).Enable False
Room(135).Enable False
ShaftsFloor(135).Enable False
ShaftsFloor(136).Enable False
Else
If CameraFloor = 136 Or CameraFloor = 135 Then
    If GotoFloor(ElevatorNumber) = 0 And InStairwell = False Then
    Room(136).Enable True
    ElevatorDoor1L(136).Enable True
    ElevatorDoor1R(136).Enable True
    ElevatorDoor1L(135).Enable True
    ElevatorDoor1R(135).Enable True
    ElevatorDoor2L(136).Enable True
    ElevatorDoor2R(136).Enable True
    ElevatorDoor2L(135).Enable True
    ElevatorDoor2R(135).Enable True
    Room(135).Enable True
    ShaftsFloor(135).Enable True
    ShaftsFloor(136).Enable True
    End If
End If
End If

'this section makes sure all the extra objects on the 1st and M floors are created
If CameraFloor <> 1 And ElevatorDoor1L(1).IsMeshEnabled = False Then
ElevatorDoor1L(-1).Enable False
ElevatorDoor1R(-1).Enable False
ElevatorDoor2L(-1).Enable False
ElevatorDoor2R(-1).Enable False
ElevatorDoor3L(-1).Enable False
ElevatorDoor3R(-1).Enable False
ElevatorDoor4L(-1).Enable False
ElevatorDoor4R(-1).Enable False
ElevatorDoor5L(-1).Enable False
ElevatorDoor5R(-1).Enable False
ElevatorDoor6L(-1).Enable False
ElevatorDoor6R(-1).Enable False
ElevatorDoor7L(-1).Enable False
ElevatorDoor7R(-1).Enable False
ElevatorDoor8L(-1).Enable False
ElevatorDoor8R(-1).Enable False
ElevatorDoor9L(-1).Enable False
ElevatorDoor9R(-1).Enable False
ElevatorDoor10L(-1).Enable False
ElevatorDoor10R(-1).Enable False
ElevatorDoor1L(0).Enable False
ElevatorDoor1R(0).Enable False
ElevatorDoor2L(0).Enable False
ElevatorDoor2R(0).Enable False
End If
If CameraFloor = 1 And GotoFloor(ElevatorNumber) = 0 And ElevatorDoor1L(1).IsMeshEnabled = False Then
ElevatorDoor1L(-1).Enable True
ElevatorDoor1R(-1).Enable True
ElevatorDoor2L(-1).Enable True
ElevatorDoor2R(-1).Enable True
ElevatorDoor3L(-1).Enable True
ElevatorDoor3R(-1).Enable True
ElevatorDoor4L(-1).Enable True
ElevatorDoor4R(-1).Enable True
ElevatorDoor5L(-1).Enable True
ElevatorDoor5R(-1).Enable True
ElevatorDoor6L(-1).Enable True
ElevatorDoor6R(-1).Enable True
ElevatorDoor7L(-1).Enable True
ElevatorDoor7R(-1).Enable True
ElevatorDoor8L(-1).Enable True
ElevatorDoor8R(-1).Enable True
ElevatorDoor9L(-1).Enable True
ElevatorDoor9R(-1).Enable True
ElevatorDoor10L(-1).Enable True
ElevatorDoor10R(-1).Enable True
ElevatorDoor1L(0).Enable True
ElevatorDoor1R(0).Enable True
ElevatorDoor2L(0).Enable True
ElevatorDoor2R(0).Enable True
End If

'This section turns on and off the external mesh (outside windows, not inside windows), based on the camera's current location
If CameraFloor >= 1 And CameraFloor <= 39 Then
    If Camera.GetPosition.X < -160 Or Camera.GetPosition.X > 160 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    If External.IsMeshEnabled = False Then
        External.Enable True
        'Buildings.Enable True
        Room(CameraFloor).Enable False
        ShaftsFloor(CameraFloor).Enable False
        DestroyObjects (CameraFloor)
    End If
    Else
    If External.IsMeshEnabled = True Then
        External.Enable False
        'Buildings.Enable False
        Room(CameraFloor).Enable True
        ShaftsFloor(CameraFloor).Enable True
        InitObjectsForFloor (CameraFloor)
    End If
    End If
End If
If CameraFloor >= 40 And CameraFloor <= 79 Then
    If Camera.GetPosition.X < -135 Or Camera.GetPosition.X > 135 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    If External.IsMeshEnabled = False Then
        External.Enable True
        'Buildings.Enable True
        Room(CameraFloor).Enable False
        ShaftsFloor(CameraFloor).Enable False
        DestroyObjects (CameraFloor)
    End If
    Else
    If External.IsMeshEnabled = True Then
        External.Enable False
        'Buildings.Enable False
        Room(CameraFloor).Enable True
        ShaftsFloor(CameraFloor).Enable True
        InitObjectsForFloor (CameraFloor)
    End If
    End If
End If
If CameraFloor >= 80 And CameraFloor <= 117 Then
    If Camera.GetPosition.X < -110 Or Camera.GetPosition.X > 110 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    If External.IsMeshEnabled = False Then
        External.Enable True
        'Buildings.Enable True
        Room(CameraFloor).Enable False
        ShaftsFloor(CameraFloor).Enable False
        DestroyObjects (CameraFloor)
    End If
    Else
    If External.IsMeshEnabled = True Then
        External.Enable False
        'Buildings.Enable False
        Room(CameraFloor).Enable True
        ShaftsFloor(CameraFloor).Enable True
        InitObjectsForFloor (CameraFloor)
    End If
    End If
End If
If CameraFloor >= 118 And CameraFloor <= 134 Then
    If Camera.GetPosition.X < -85 Or Camera.GetPosition.X > 85 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    If External.IsMeshEnabled = False Then
        External.Enable True
        'Buildings.Enable True
        Room(CameraFloor).Enable False
        ShaftsFloor(CameraFloor).Enable False
        DestroyObjects (CameraFloor)
    End If
    Else
    If External.IsMeshEnabled = True Then
        External.Enable False
        'Buildings.Enable False
        Room(CameraFloor).Enable True
        ShaftsFloor(CameraFloor).Enable True
        InitObjectsForFloor (CameraFloor)
    End If
    End If
End If
If CameraFloor >= 135 And CameraFloor <= 138 Then
    If Camera.GetPosition.X < -60 Or Camera.GetPosition.X > 60 Or Camera.GetPosition.z < -150 Or Camera.GetPosition.z > 150 Then
    If External.IsMeshEnabled = False Then
        External.Enable True
        'Buildings.Enable True
        Room(CameraFloor).Enable False
        ShaftsFloor(CameraFloor).Enable False
        DestroyObjects (CameraFloor)
    End If
    Else
    If External.IsMeshEnabled = True Then
        External.Enable False
        'Buildings.Enable False
        Room(CameraFloor).Enable True
        ShaftsFloor(CameraFloor).Enable True
        InitObjectsForFloor (CameraFloor)
    End If
    End If
End If

'Calls the Fall sub, and if IsFalling is true then the user falls until they hit something
If EnableCollisions = True Then Call Fall

''This section turns on and off the Shafts mesh (inside the elevator and pipe shafts) when the camera is located inside them.
'If CameraFloor = ElevatorFloor(ElevatorNumber) And Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < -16 And CameraFloor <> 137 Then
'For i50 = 1 To 138
'Shafts(i50).Enable False
'Next i50
''Atmos.SkyBox_Enable False
'GoTo EndShafts
'Else
''Atmos.SkyBox_Enable True
'End If

If CameraFloor = 137 Then GoTo EndShafts
If GotoFloor(ElevatorNumber) <> 0 Then GoTo EndShafts
'GoTo EndShafts

'if user is in service room, turn on shaft
If Camera.GetPosition.X < 50 And Camera.GetPosition.X > 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -20 And CameraFloor <> 1 And CameraFloor <> 132 And CameraFloor < 134 Then
For i50 = 1 To 138
Shafts2(i50).Enable True
Next i50
GoTo EndShafts
End If
'if user is outside service room, turn off shaft
If Camera.GetPosition.X < 50 And Camera.GetPosition.X > 32.5 And Camera.GetPosition.z < -46.25 And CameraFloor <> 1 And CameraFloor <> 132 And CameraFloor < 134 Then
For i50 = 1 To 138
Shafts2(i50).Enable False
Next i50
GoTo EndShafts
End If

If CameraFloor >= 1 And CameraFloor <= 39 Then
    'right shaft (the one with the stairs)
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 46.25 And InElevator = False Then
    For i50 = 1 To 138
    Shafts1(i50).Enable True
    Next i50
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 46.25 And InElevator = False Then
    For i50 = 1 To 138
    Shafts2(i50).Enable True
    Next i50
    Else
    For i50 = 1 To 138
    Shafts1(i50).Enable False
    Shafts2(i50).Enable False
    Next i50
    End If
    End If
    
End If
If CameraFloor >= 40 And CameraFloor <= 79 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 30.83 And InElevator = False Then
    For i50 = 1 To 138
    Shafts1(i50).Enable True
    Next i50
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 30.83 And InElevator = False Then
    For i50 = 1 To 138
    Shafts2(i50).Enable True
    Next i50
    Else
    For i50 = 1 To 138
    Shafts1(i50).Enable False
    Shafts2(i50).Enable False
    Next i50
    End If
    End If
End If
If CameraFloor >= 80 And CameraFloor <= 117 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 15.41 And InElevator = False Then
    For i50 = 1 To 138
    Shafts1(i50).Enable True
    Next i50
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 15.41 And InElevator = False Then
    For i50 = 1 To 138
    Shafts2(i50).Enable True
    Next i50
    Else
    For i50 = 1 To 138
    Shafts1(i50).Enable False
    Shafts2(i50).Enable False
    Next i50
    End If
    End If
End If
If CameraFloor >= 118 And CameraFloor <= 134 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < 0 And InElevator = False Then
    For i50 = 1 To 138
    Shafts1(i50).Enable True
    Next i50
    Else
    'left shaft (the one with the pipe shaft)
    If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < 0 And InElevator = False Then
    For i50 = 1 To 138
    Shafts2(i50).Enable True
    Next i50
    Else
    For i50 = 1 To 138
    Shafts1(i50).Enable False
    Shafts2(i50).Enable False
    Next i50
    End If
    End If
End If
If CameraFloor >= 135 And CameraFloor <= 138 Then
    If Camera.GetPosition.X > -32.5 And Camera.GetPosition.X < -12.5 And Camera.GetPosition.z > -30 And Camera.GetPosition.z < -15.42 And InElevator = False Then
    For i50 = 1 To 138
    Shafts1(i50).Enable True
    Next i50
    Else
    'left shaft (the one with the pipe shaft)
    'If Camera.GetPosition.X > 12.5 And Camera.GetPosition.X < 32.5 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -15.42 and InElevator=False Then
    'Shafts.Enable True
    'Else
    For i50 = 1 To 138
    Shafts1(i50).Enable False
    Shafts2(i50).Enable False
    Next i50
    'End If
    End If
End If
EndShafts:

    linestart = Camera.GetPosition

 Inp.GetAbsMouseState tmpMouseX, tmpMouseY, tmpMouseB1

'Click
 If tmpMouseB1 <> 0 Then
        '...check if the mouse pointer has collided with a Teapot.
        'If FloorIndicator1.Collision=True
        
        Room(CameraFloor).SetCollisionEnable False
        External.SetCollisionEnable False
        Buildings.SetCollisionEnable False
        For i50 = 1 To 138
        Shafts1(i50).SetCollisionEnable False
        Shafts2(i50).SetCollisionEnable False
        Next i50
        ShaftsFloor(CameraFloor).SetCollisionEnable False
        For i50 = 1 To 10
        Elevator(i50).SetCollisionEnable False
        ElevatorInsDoorL(i50).SetCollisionEnable False
        ElevatorInsDoorR(i50).SetCollisionEnable False
        Next i50
        If CameraFloor = 1 Then
        ElevatorDoor1L(-1).SetCollisionEnable False
        ElevatorDoor1R(-1).SetCollisionEnable False
        ElevatorDoor2L(-1).SetCollisionEnable False
        ElevatorDoor2R(-1).SetCollisionEnable False
        ElevatorDoor3L(-1).SetCollisionEnable False
        ElevatorDoor3R(-1).SetCollisionEnable False
        ElevatorDoor4L(-1).SetCollisionEnable False
        ElevatorDoor4R(-1).SetCollisionEnable False
        ElevatorDoor5L(-1).SetCollisionEnable False
        ElevatorDoor5R(-1).SetCollisionEnable False
        ElevatorDoor6L(-1).SetCollisionEnable False
        ElevatorDoor6R(-1).SetCollisionEnable False
        ElevatorDoor7L(-1).SetCollisionEnable False
        ElevatorDoor7R(-1).SetCollisionEnable False
        ElevatorDoor8L(-1).SetCollisionEnable False
        ElevatorDoor8R(-1).SetCollisionEnable False
        ElevatorDoor9L(-1).SetCollisionEnable False
        ElevatorDoor9R(-1).SetCollisionEnable False
        ElevatorDoor10L(-1).SetCollisionEnable False
        ElevatorDoor10R(-1).SetCollisionEnable False
        ElevatorDoor1L(0).SetCollisionEnable False
        ElevatorDoor1R(0).SetCollisionEnable False
        ElevatorDoor2L(0).SetCollisionEnable False
        ElevatorDoor2R(0).SetCollisionEnable False
        Else
        ElevatorDoor1L(CameraFloor).SetCollisionEnable False
        ElevatorDoor1R(CameraFloor).SetCollisionEnable False
        ElevatorDoor2L(CameraFloor).SetCollisionEnable False
        ElevatorDoor2R(CameraFloor).SetCollisionEnable False
        ElevatorDoor3L(CameraFloor).SetCollisionEnable False
        ElevatorDoor3R(CameraFloor).SetCollisionEnable False
        ElevatorDoor4L(CameraFloor).SetCollisionEnable False
        ElevatorDoor4R(CameraFloor).SetCollisionEnable False
        ElevatorDoor5L(CameraFloor).SetCollisionEnable False
        ElevatorDoor5R(CameraFloor).SetCollisionEnable False
        ElevatorDoor6L(CameraFloor).SetCollisionEnable False
        ElevatorDoor6R(CameraFloor).SetCollisionEnable False
        ElevatorDoor7L(CameraFloor).SetCollisionEnable False
        ElevatorDoor7R(CameraFloor).SetCollisionEnable False
        ElevatorDoor8L(CameraFloor).SetCollisionEnable False
        ElevatorDoor8R(CameraFloor).SetCollisionEnable False
        ElevatorDoor9L(CameraFloor).SetCollisionEnable False
        ElevatorDoor9R(CameraFloor).SetCollisionEnable False
        ElevatorDoor10L(CameraFloor).SetCollisionEnable False
        ElevatorDoor10R(CameraFloor).SetCollisionEnable False
        End If
        Stairs(CameraFloor).SetCollisionEnable False
        If CameraFloor > 1 Then Stairs(CameraFloor - 1).SetCollisionEnable False
        If CameraFloor < 138 Then Stairs(CameraFloor + 1).SetCollisionEnable False
        If CameraFloor = 135 Then Room(136).SetCollisionEnable False
        If CameraFloor = 136 Then Room(135).SetCollisionEnable False
        
        Set CollisionResult = Scene.MousePicking(tmpMouseX, tmpMouseY, TV_COLLIDE_MESH, TV_TESTTYPE_ACCURATETESTING)
         
        
        If CollisionResult.IsCollision Then
        Call CheckElevatorButtons
            For i50 = 1 To 10
            If CollisionResult.GetCollisionMesh.GetMeshName = CallButtons(i50).GetMeshName Then
                
                'use other elevator if it's closer
                j50 = i50
                If i50 = 3 And Abs(ElevatorFloor(3) - CameraFloor) > Abs(ElevatorFloor(4) - CameraFloor) Then j50 = 4
                If i50 = 4 And Abs(ElevatorFloor(4) - CameraFloor) > Abs(ElevatorFloor(3) - CameraFloor) Then j50 = 3
                If i50 = 5 And Abs(ElevatorFloor(5) - CameraFloor) > Abs(ElevatorFloor(6) - CameraFloor) Then j50 = 6
                If i50 = 6 And Abs(ElevatorFloor(6) - CameraFloor) > Abs(ElevatorFloor(5) - CameraFloor) Then j50 = 5
                If i50 = 7 And Abs(ElevatorFloor(7) - CameraFloor) > Abs(ElevatorFloor(8) - CameraFloor) Then j50 = 8
                If i50 = 8 And Abs(ElevatorFloor(8) - CameraFloor) > Abs(ElevatorFloor(7) - CameraFloor) Then j50 = 7
                If i50 = 9 And Abs(ElevatorFloor(9) - CameraFloor) > Abs(ElevatorFloor(10) - CameraFloor) Then j50 = 10
                If i50 = 10 And Abs(ElevatorFloor(10) - CameraFloor) > Abs(ElevatorFloor(9) - CameraFloor) Then j50 = 9
                
                If ElevatorFloor(j50) <> CameraFloor Then
                ElevatorSync(j50) = False
                OpenElevator(j50) = -1
                GotoFloor(j50) = CameraFloor
                    If GotoFloor(j50) = 1 Then GotoFloor(j50) = -1
                GoTo EndCall
                End If
                If ElevatorFloor(j50) = 1 And Camera.GetPosition.Y > 27 And FloorIndicatorText(j50) <> "M" Then
                ElevatorSync(j50) = False
                OpenElevator(j50) = -1
                GotoFloor(j50) = 0.1
                GoTo EndCall
                End If
                If ElevatorFloor(j50) = 1 And Camera.GetPosition.Y < 27 And FloorIndicatorText(j50) = "M" Then
                ElevatorSync(j50) = False
                OpenElevator(j50) = -1
                GotoFloor(j50) = 0.1
                GoTo EndCall
                End If
                OpenElevator(j50) = 1
            End If
EndCall:
            Next i50
            'CollisionResult.GetCollisionMesh.Enable False
        If OpeningDoor = 0 And ClosingDoor = 0 Then
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorA " Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 6, 20))
            DoorRotated = 0
            Call OpenDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorB " Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 6, 20))
            DoorRotated = 1
            Call OpenDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorC " Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 6, 20))
            DoorRotated = 2
            Call OpenDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorD " Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 6, 20))
            DoorRotated = 3
            Call OpenDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorAO" Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 7, 20))
            DoorRotated = 0
            ClosingDoor = 18
            Call CloseDoor
            End If
        
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorBO" Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 7, 20))
            DoorRotated = 1
            ClosingDoor = 18
            Call CloseDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorCO" Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 7, 20))
            DoorRotated = 2
            ClosingDoor = 18
            Call CloseDoor
            End If
            
            If Left(CollisionResult.GetCollisionMesh.GetMeshName, 6) = "DoorDO" Then
            DoorNumber = Val(Mid$(CollisionResult.GetCollisionMesh.GetMeshName, 7, 20))
            DoorRotated = 3
            ClosingDoor = 18
            Call CloseDoor
            End If
        
        End If
            
            End If
    End If



    TV.Clear
    'Call Listener.SetPosition(Camera.GetPosition.X, Camera.GetPosition.Y, Camera.GetPosition.z)
    'Call ElevatorMusic.GetPosition(ListenerDirection.X, ListenerDirection.Y, ListenerDirection.z)
    'Call Camera.GetRotation(ListenerDirection.x, ListenerDirection.y, ListenerDirection.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    'Call Listener.SetOrientation(ListenerDirection.x, Camera.GetPosition.y, ListenerDirection.z, Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z)
    
      
'*** First movement system


      'If Inp.IsKeyPressed(TV_KEY_UP) = True And Focused = True Then
      If Inp.IsKeyPressed(TV_KEY_UP) = True Then
      KeepAltitude = Camera.GetPosition.Y
      If Inp.IsKeyPressed(TV_KEY_Z) = False Then Camera.MoveRelative 0.7, 0, 0
      If Inp.IsKeyPressed(TV_KEY_Z) = True Then Camera.MoveRelative 1.4, 0, 0
      If Camera.GetPosition.Y <> KeepAltitude Then Camera.SetPosition Camera.GetPosition.X, KeepAltitude, Camera.GetPosition.z
      End If
      
      'If Inp.IsKeyPressed(TV_KEY_DOWN) = True And Focused = True Then
      If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then
      KeepAltitude = Camera.GetPosition.Y
      If Inp.IsKeyPressed(TV_KEY_Z) = False Then Camera.MoveRelative -0.7, 0, 0
      If Inp.IsKeyPressed(TV_KEY_Z) = True Then Camera.MoveRelative -1.4, 0, 0
      If Camera.GetPosition.Y <> KeepAltitude Then Camera.SetPosition Camera.GetPosition.X, KeepAltitude, Camera.GetPosition.z
      End If
      
      'If Inp.IsKeyPressed(TV_KEY_RIGHT) = True And Focused = True Then Camera.RotateY 0.07
      'If Inp.IsKeyPressed(TV_KEY_LEFT) = True And Focused = True Then Camera.RotateY -0.07
      If Inp.IsKeyPressed(TV_KEY_RIGHT) = True And Inp.IsKeyPressed(TV_KEY_Z) = False Then Camera.RotateY 0.07
      If Inp.IsKeyPressed(TV_KEY_LEFT) = True And Inp.IsKeyPressed(TV_KEY_Z) = False Then Camera.RotateY -0.07
      If Inp.IsKeyPressed(TV_KEY_RIGHT) = True And Inp.IsKeyPressed(TV_KEY_Z) = True Then Camera.RotateY 0.14
      If Inp.IsKeyPressed(TV_KEY_LEFT) = True And Inp.IsKeyPressed(TV_KEY_Z) = True Then Camera.RotateY -0.14
      
'*** Second movement system (has horrible problems)
      'If Inp.IsKeyPressed(TV_KEY_UP) = True Then Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z - (Camera.GetLookAt.z / 100), Camera.GetLookAt.x, Camera.GetLookAt.y, Camera.GetLookAt.z - 0.7
      'If Inp.IsKeyPressed(TV_KEY_DOWN) = True Then Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x, Camera.GetLookAt.y, Camera.GetLookAt.z + 0.7
      'If Inp.IsKeyPressed(TV_KEY_RIGHT) = True Then Camera.SetCamera Camera.GetPosition.x - 0.7, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x - 0.7, Camera.GetLookAt.y, Camera.GetLookAt.z
      'If Inp.IsKeyPressed(TV_KEY_LEFT) = True Then Camera.SetCamera Camera.GetPosition.x + 0.7, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x + 0.7, Camera.GetLookAt.y, Camera.GetLookAt.z
            
'*** Third (new) Movement System, should be released with version 0.6 (i50 had problems with it)
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
        
      'If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True And Focused = True Then Camera.RotateX -0.006
      'If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True And Focused = True Then Camera.RotateX 0.006
      If Inp.IsKeyPressed(TV_KEY_PAGEUP) = True Then Camera.RotateX -0.006
      If Inp.IsKeyPressed(TV_KEY_PAGEDOWN) = True Then Camera.RotateX 0.006
      
      'If Inp.IsKeyPressed(TV_KEY_HOME) = True And Focused = True Then Camera.MoveRelative 0, 1, 0
      'If Inp.IsKeyPressed(TV_KEY_END) = True And Focused = True Then Camera.MoveRelative 0, -1, 0
      If Inp.IsKeyPressed(TV_KEY_HOME) = True Then Camera.MoveRelative 0, 1, 0
      If Inp.IsKeyPressed(TV_KEY_END) = True Then Camera.MoveRelative 0, -1, 0
      'If Inp.IsKeyPressed(TV_KEY_1) = True And Focused = True Then ElevatorDirection = 1
      'If Inp.IsKeyPressed(TV_KEY_2) = True And Focused = True Then ElevatorDirection = -1
      'If Inp.IsKeyPressed(TV_KEY_3) = True And Focused = True Then OpenElevator(ElevatorNumber) = 1
      'If Inp.IsKeyPressed(TV_KEY_4) = True And Focused = True Then OpenElevator(ElevatorNumber) = -1
      'If Inp.IsKeyPressed(TV_KEY_5) = True And Focused = True Then Call ElevatorMusic.Play
      'If Inp.IsKeyPressed(TV_KEY_6) = True And Focused = True Then Call ElevatorMusic.Stop_
      If Inp.IsKeyPressed(TV_KEY_SPACE) = True Then Camera.SetRotation 0, 0, 0
      'If Inp.IsKeyPressed(TV_KEY_6) = True Then MsgBox (Str$(Camera.GetLookAt.X) + Str$(Camera.GetLookAt.Y) + Str$(Camera.GetLookAt.z))
      If Inp.IsKeyPressed(TV_KEY_7) = True Then IsFalling = True
      
        'Inp.GetMouseState MousePositionX, MousePositionY
      'MsgBox (Str$(MousePositionX) + "," + Str$(MousePositionY))
      'Camera.SetLookAt -50, -50, -50
      'Camera.SetCamera Camera.GetPosition.x, Camera.GetPosition.y, Camera.GetPosition.z, Camera.GetLookAt.x - (MousePositionX / 100), Camera.GetLookAt.y - (MousePositionY / 100), Camera.GetLookAt.z
      
      'Orientation: 0-1 pan to center, -1 left 1 right, ?, same as 1st, same as 2nd, ?
      'If Inp.IsKeyPressed(TV_KEY_7) = True And Focused = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
      'If Inp.IsKeyPressed(TV_KEY_8) = True And Focused = True Then Call Listener.SetOrientation(0, 0, 1, 0, 0, 1)
        
      If Inp.IsKeyPressed(TV_KEY_F1) = True And Focused = True Then TV.ScreenShot ("c:\shot.bmp")

      
Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(ElevatorNumber)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.X)) + "," + Str$(Int(Camera.GetPosition.Y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "GotoFloor=" + Str$(GotoFloor(ElevatorNumber)) + vbCrLf + "DistancetoDest=" + Str$(Abs(GotoFloor(ElevatorNumber) - CurrentFloor(ElevatorNumber))) + vbCrLf + "Rate=" + Str$(ElevatorEnable(ElevatorNumber) / 5)
             
      'ElevatorFloor(ElevatorNumber) = (Elevator(ElevatorNumber).GetPosition.Y - 25) / 25
      'If ElevatorFloor(ElevatorNumber) < 1 Then ElevatorFloor(ElevatorNumber) = 1
      
      If InStairwell = False Then CameraFloor = (Camera.GetPosition.Y - 25 - 10) / 25
      If CameraFloor < 1 Then CameraFloor = 1
   
      lineend = Camera.GetPosition
          
If EnableCollisions = True Then Call CheckCollisions

        
    On Error Resume Next
    Atmos.Atmosphere_Render
    Scene.RenderAllMeshes
    TV.RenderToScreen
    DoEvents
  
End Sub

Sub StairsLoop()
'Stairs Movement
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 22 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoor1L(CameraFloor).Enable False
      ElevatorDoor1R(CameraFloor).Enable False
      ElevatorDoor2L(CameraFloor).Enable False
      ElevatorDoor2R(CameraFloor).Enable False
      ElevatorDoor3L(CameraFloor).Enable False
      ElevatorDoor3R(CameraFloor).Enable False
      ElevatorDoor4L(CameraFloor).Enable False
      ElevatorDoor4R(CameraFloor).Enable False
      ElevatorDoor5L(CameraFloor).Enable False
      ElevatorDoor5R(CameraFloor).Enable False
      ElevatorDoor6L(CameraFloor).Enable False
      ElevatorDoor6R(CameraFloor).Enable False
      ElevatorDoor7L(CameraFloor).Enable False
      ElevatorDoor7R(CameraFloor).Enable False
      ElevatorDoor8L(CameraFloor).Enable False
      ElevatorDoor8R(CameraFloor).Enable False
      ElevatorDoor9L(CameraFloor).Enable False
      ElevatorDoor9R(CameraFloor).Enable False
      ElevatorDoor10L(CameraFloor).Enable False
      ElevatorDoor10R(CameraFloor).Enable False
      For i51 = 1 To 10
      CallButtons(i51).Enable False
      Next i51
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
      ElevatorDoor1L(CameraFloor).Enable True
      ElevatorDoor1R(CameraFloor).Enable True
      ElevatorDoor2L(CameraFloor).Enable True
      ElevatorDoor2R(CameraFloor).Enable True
      ElevatorDoor3L(CameraFloor).Enable True
      ElevatorDoor3R(CameraFloor).Enable True
      ElevatorDoor4L(CameraFloor).Enable True
      ElevatorDoor4R(CameraFloor).Enable True
      ElevatorDoor5L(CameraFloor).Enable True
      ElevatorDoor5R(CameraFloor).Enable True
      ElevatorDoor6L(CameraFloor).Enable True
      ElevatorDoor6R(CameraFloor).Enable True
      ElevatorDoor7L(CameraFloor).Enable True
      ElevatorDoor7R(CameraFloor).Enable True
      ElevatorDoor8L(CameraFloor).Enable True
      ElevatorDoor8R(CameraFloor).Enable True
      ElevatorDoor9L(CameraFloor).Enable True
      ElevatorDoor9R(CameraFloor).Enable True
      ElevatorDoor10L(CameraFloor).Enable True
      ElevatorDoor10R(CameraFloor).Enable True
      ShaftsFloor(CameraFloor).Enable True
      For i51 = 1 To 10
      CallButtons(i51).Enable True
      Next i51
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      'If CameraFloor = 137 Then
      'For i51 = 1 To 138
      'Shafts(i51).Enable True
      'Next i51
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
    Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
    End If
    If Camera.GetPosition.X <= -12.5 And Camera.GetPosition.X > -12.5 - 6 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -30.85 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + 2 + PartialFloor Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10 + PartialFloor, Camera.GetPosition.z
   
    If Camera.GetPosition.X <= -12.5 - 6 And Camera.GetPosition.X > -12.5 - 8 And Camera.GetPosition.z > -46.25 And Camera.GetPosition.z < -46.25 + 7.71 And Camera.GetPosition.Y = (CameraFloor * 25) + 25 + 10 + PartialFloor Then
      Room(CameraFloor).Enable False
      ElevatorDoor1L(CameraFloor).Enable False
      ElevatorDoor1R(CameraFloor).Enable False
      ElevatorDoor2L(CameraFloor).Enable False
      ElevatorDoor2R(CameraFloor).Enable False
      ElevatorDoor3L(CameraFloor).Enable False
      ElevatorDoor3R(CameraFloor).Enable False
      ElevatorDoor4L(CameraFloor).Enable False
      ElevatorDoor4R(CameraFloor).Enable False
      ElevatorDoor5L(CameraFloor).Enable False
      ElevatorDoor5R(CameraFloor).Enable False
      ElevatorDoor6L(CameraFloor).Enable False
      ElevatorDoor6R(CameraFloor).Enable False
      ElevatorDoor7L(CameraFloor).Enable False
      ElevatorDoor7R(CameraFloor).Enable False
      ElevatorDoor8L(CameraFloor).Enable False
      ElevatorDoor8R(CameraFloor).Enable False
      ElevatorDoor9L(CameraFloor).Enable False
      ElevatorDoor9R(CameraFloor).Enable False
      ElevatorDoor10L(CameraFloor).Enable False
      ElevatorDoor10R(CameraFloor).Enable False
      ShaftsFloor(CameraFloor).Enable False
      For i51 = 1 To 10
      CallButtons(i51).Enable False
      Next i51
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
      ElevatorDoor1L(CameraFloor).Enable True
      ElevatorDoor1R(CameraFloor).Enable True
      ElevatorDoor2L(CameraFloor).Enable True
      ElevatorDoor2R(CameraFloor).Enable True
      ElevatorDoor3L(CameraFloor).Enable True
      ElevatorDoor3R(CameraFloor).Enable True
      ElevatorDoor4L(CameraFloor).Enable True
      ElevatorDoor4R(CameraFloor).Enable True
      ElevatorDoor5L(CameraFloor).Enable True
      ElevatorDoor5R(CameraFloor).Enable True
      ElevatorDoor6L(CameraFloor).Enable True
      ElevatorDoor6R(CameraFloor).Enable True
      ElevatorDoor7L(CameraFloor).Enable True
      ElevatorDoor7R(CameraFloor).Enable True
      ElevatorDoor8L(CameraFloor).Enable True
      ElevatorDoor8R(CameraFloor).Enable True
      ElevatorDoor9L(CameraFloor).Enable True
      ElevatorDoor9R(CameraFloor).Enable True
      ElevatorDoor10L(CameraFloor).Enable True
      ElevatorDoor10R(CameraFloor).Enable True
      For i51 = 1 To 10
      CallButtons(i51).Enable True
      Next i51
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      InitObjectsForFloor (CameraFloor)
      'If CameraFloor = 137 Then
      'For i51 = 1 To 138
      'Shafts(i51).Enable True
      'Next i51
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

Sub ProcessMisc()


    Shafts1(1).AddWall GetTex("BrickTexture"), -12.5, -30.85, -32.5, -30.85, 75, 0, 2, 2
    For i = 2 To 138
    Shafts1(i).AddWall GetTex("BrickTexture"), -12.5, -30.85, -32.5, -30.85, 25, (25 * i) + 25, 2, 2
    Next i
    For i = 2 To 136
    Shafts2(i).AddWall GetTex("BrickTexture"), 12.5, -30.85, 32.5, -30.85, 25, (25 * i) + 25, 2, 2
    Next i
    
    'Shafts.AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, (25 * 138) + 25, 0, 2, 139 * 2
        
    Stairs(1).AddWall GetTex("Concrete"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 22, 0, (7.71 * 0.086) * 4, 1 * 4
    Stairs(138).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.5, 25, (138 * 25) + 25, (7.71 * 0.086) * 4, 1 * 4
    
    'Shafts.AddFloor GetTex("BrickTexture"), -60, -150, 60, 150, (138 * 25) + 25, 10, 10

    'Elevator1
    'Old one Elevator1.AddFloor GetTex("BrickTexture"), -32, -30, -12, -16, 1
    Elevator(1).AddFloor GetTex("Wood2"), -28, -29.9, -12.5, -16, 0.1, 1, 1
    Elevator(1).AddFloor GetTex("Elev1"), -28, -29.9, -12.5, -16, 19.5, 2, 2
    Elevator(1).AddWall GetTex("Wood1"), -28, -29.9, -12.5, -29.9, 19.5, 0.1, 2, 2
    Elevator(1).AddWall GetTex("Wood1"), -28, -16, -12.5, -16, 19.5, 0.1, 2, 2
    Elevator(1).AddWall GetTex("Wood1"), -28, -16, -28, -29.9, 19.5, 0.1, 2, 2
    
    Elevator(2).AddFloor GetTex("Wood2"), 28, -30, 12.5, -16, 0.1, 1, 1
    Elevator(2).AddFloor GetTex("Elev1"), 28, -30, 12.5, -16, 19.5, 2, 2
    Elevator(2).AddWall GetTex("Wood1"), 28, -30, 12.5, -30, 19.5, 0.1, 2, 2
    Elevator(2).AddWall GetTex("Wood1"), 28, -16, 12.5, -16, 19.5, 0.1, 2, 2
    Elevator(2).AddWall GetTex("Wood1"), 28, -16, 28, -30, 19.5, 0.1, 2, 2
    
    Elevator(3).AddFloor GetTex("Wood2"), -28, -15, -12.5, -1, 0.1, 1, 1
    Elevator(3).AddFloor GetTex("Elev1"), -28, -15, -12.5, -1, 19.5, 2, 2
    Elevator(3).AddWall GetTex("Wood1"), -28, -15, -12.5, -15, 19.5, 0.1, 2, 2
    Elevator(3).AddWall GetTex("Wood1"), -28, -1, -12.5, -1, 19.5, 0.1, 2, 2
    Elevator(3).AddWall GetTex("Wood1"), -28, -1, -28, -15, 19.5, 0.1, 2, 2
    
    Elevator(4).AddFloor GetTex("Wood2"), 28, -15, 12.5, -1, 0.1, 1, 1
    Elevator(4).AddFloor GetTex("Elev1"), 28, -15, 12.5, -1, 19.5, 2, 2
    Elevator(4).AddWall GetTex("Wood1"), 28, -15, 12.5, -15, 19.5, 0.1, 2, 2
    Elevator(4).AddWall GetTex("Wood1"), 28, -1, 12.5, -1, 19.5, 0.1, 2, 2
    Elevator(4).AddWall GetTex("Wood1"), 28, -1, 28, -15, 19.5, 0.1, 2, 2
    
    Elevator(5).AddFloor GetTex("Wood2"), -28, 0, -12.5, 14, 0.1, 1, 1
    Elevator(5).AddFloor GetTex("Elev1"), -28, 0, -12.5, 14, 19.5, 2, 2
    Elevator(5).AddWall GetTex("Wood1"), -28, 0, -12.5, 0, 19.5, 0.1, 2, 2
    Elevator(5).AddWall GetTex("Wood1"), -28, 14, -12.5, 14, 19.5, 0.1, 2, 2
    Elevator(5).AddWall GetTex("Wood1"), -28, 14, -28, 0, 19.5, 0.1, 2, 2
    
    Elevator(6).AddFloor GetTex("Wood2"), 28, 0, 12.5, 14, 0.1, 1, 1
    Elevator(6).AddFloor GetTex("Elev1"), 28, 0, 12.5, 14, 19.5, 2, 2
    Elevator(6).AddWall GetTex("Wood1"), 28, 0, 12.5, 0, 19.5, 0.1, 2, 2
    Elevator(6).AddWall GetTex("Wood1"), 28, 14, 12.5, 14, 19.5, 0.1, 2, 2
    Elevator(6).AddWall GetTex("Wood1"), 28, 14, 28, 0, 19.5, 0.1, 2, 2
    
    Elevator(7).AddFloor GetTex("Wood2"), -28, 15, -12.5, 29, 0.1, 1, 1
    Elevator(7).AddFloor GetTex("Elev1"), -28, 15, -12.5, 29, 19.5, 2, 2
    Elevator(7).AddWall GetTex("Wood1"), -28, 15, -12.5, 15, 19.5, 0.1, 2, 2
    Elevator(7).AddWall GetTex("Wood1"), -28, 29, -12.5, 29, 19.5, 0.1, 2, 2
    Elevator(7).AddWall GetTex("Wood1"), -28, 29, -28, 15, 19.5, 0.1, 2, 2
    
    Elevator(8).AddFloor GetTex("Wood2"), 28, 15, 12.5, 29, 0.1, 1, 1
    Elevator(8).AddFloor GetTex("Elev1"), 28, 15, 12.5, 29, 19.5, 2, 2
    Elevator(8).AddWall GetTex("Wood1"), 28, 15, 12.5, 15, 19.5, 0.1, 2, 2
    Elevator(8).AddWall GetTex("Wood1"), 28, 29, 12.5, 29, 19.5, 0.1, 2, 2
    Elevator(8).AddWall GetTex("Wood1"), 28, 29, 28, 15, 19.5, 0.1, 2, 2
    
    Elevator(9).AddFloor GetTex("Wood2"), -28, 30, -12.5, 44, 0.1, 1, 1
    Elevator(9).AddFloor GetTex("Elev1"), -28, 30, -12.5, 44, 19.5, 2, 2
    Elevator(9).AddWall GetTex("Wood1"), -28, 30, -12.5, 30, 19.5, 0.1, 2, 2
    Elevator(9).AddWall GetTex("Wood1"), -28, 44, -12.5, 44, 19.5, 0.1, 2, 2
    Elevator(9).AddWall GetTex("Wood1"), -28, 44, -28, 30, 19.5, 0.1, 2, 2
    
    Elevator(10).AddFloor GetTex("Wood2"), 28, 30, 12.5, 44, 0.1, 1, 1
    Elevator(10).AddFloor GetTex("Elev1"), 28, 30, 12.5, 44, 19.5, 2, 2
    Elevator(10).AddWall GetTex("Wood1"), 28, 30, 12.5, 30, 19.5, 0.1, 2, 2
    Elevator(10).AddWall GetTex("Wood1"), 28, 44, 12.5, 44, 19.5, 0.1, 2, 2
    Elevator(10).AddWall GetTex("Wood1"), 28, 44, 28, 30, 19.5, 0.1, 2, 2
    
    'Floor Indicator
    FloorIndicator(1).AddWall GetTex("ButtonL"), -12.66, -29.5, -12.66, -27.5, 1.5, 16, -1, 1
    FloorIndicator(2).AddWall GetTex("ButtonL"), 12.66, -18.5, 12.66, -16.5, 1.5, 16, 1, 1
    FloorIndicator(3).AddWall GetTex("ButtonL"), -12.66, -29.5 + (15 * 1), -12.66, -27.5 + (15 * 1), 1.5, 16, -1, 1
    FloorIndicator(4).AddWall GetTex("ButtonL"), 12.66, -18.5 + (15 * 1), 12.66, -16.5 + (15 * 1), 1.5, 16, 1, 1
    FloorIndicator(5).AddWall GetTex("ButtonL"), -12.66, -29.5 + (15 * 2), -12.66, -27.5 + (15 * 2), 1.5, 16, -1, 1
    FloorIndicator(6).AddWall GetTex("ButtonL"), 12.66, -18.5 + (15 * 2), 12.66, -16.5 + (15 * 2), 1.5, 16, 1, 1
    FloorIndicator(7).AddWall GetTex("ButtonL"), -12.66, -29.5 + (15 * 3), -12.66, -27.5 + (15 * 3), 1.5, 16, -1, 1
    FloorIndicator(8).AddWall GetTex("ButtonL"), 12.66, -18.5 + (15 * 3), 12.66, -16.5 + (15 * 3), 1.5, 16, 1, 1
    FloorIndicator(9).AddWall GetTex("ButtonL"), -12.66, -29.5 + (15 * 4), -12.66, -27.5 + (15 * 4), 1.5, 16, -1, 1
    FloorIndicator(10).AddWall GetTex("ButtonL"), 12.66, -18.5 + (15 * 4), 12.66, -16.5 + (15 * 4), 1.5, 16, 1, 1
    'Button Panel
    Elevator(1).AddWall GetTex("ElevExtPanels"), -12.66, -29.7, -12.66, -27.3, 7, 6, 1, 1
    Elevator(2).AddWall GetTex("ElevExtPanels"), 12.66, -18.7, 12.66, -16.3, 7, 6, 1, 1
    Elevator(3).AddWall GetTex("ElevExtPanels"), -12.66, -29.7 + (15 * 1), -12.66, -27.3 + (15 * 1), 7, 6, 1, 1
    Elevator(4).AddWall GetTex("ElevExtPanels"), 12.66, -18.7 + (15 * 1), 12.66, -16.3 + (15 * 1), 7, 6, 1, 1
    Elevator(5).AddWall GetTex("ElevExtPanels"), -12.66, -29.7 + (15 * 2), -12.66, -27.3 + (15 * 2), 7, 6, 1, 1
    Elevator(6).AddWall GetTex("ElevExtPanels"), 12.66, -18.7 + (15 * 2), 12.66, -16.3 + (15 * 2), 7, 6, 1, 1
    Elevator(7).AddWall GetTex("ElevExtPanels"), -12.66, -29.7 + (15 * 3), -12.66, -27.3 + (15 * 3), 7, 6, 1, 1
    Elevator(8).AddWall GetTex("ElevExtPanels"), 12.66, -18.7 + (15 * 3), 12.66, -16.3 + (15 * 3), 7, 6, 1, 1
    Elevator(9).AddWall GetTex("ElevExtPanels"), -12.66, -29.7 + (15 * 4), -12.66, -27.3 + (15 * 4), 7, 6, 1, 1
    Elevator(10).AddWall GetTex("ElevExtPanels"), 12.66, -18.7 + (15 * 4), 12.66, -16.3 + (15 * 4), 7, 6, 1, 1
    'Plaque
    Plaque(1).AddWall GetTex("Plaque"), -12.66, -29.7, -12.66, -27.3, 1, 13, -1, 1
    Plaque(1).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(1).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(2).AddWall GetTex("Plaque"), 12.66, -18.7, 12.66, -16.3, 1, 13, 1, 1
    Plaque(2).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(2).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(3).AddWall GetTex("Plaque"), -12.66, -29.7 + (15 * 1), -12.66, -27.3 + (15 * 1), 1, 13, -1, 1
    Plaque(3).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(3).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(4).AddWall GetTex("Plaque"), 12.66, -18.7 + (15 * 1), 12.66, -16.3 + (15 * 1), 1, 13, 1, 1
    Plaque(4).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(4).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(5).AddWall GetTex("Plaque"), -12.66, -29.7 + (15 * 2), -12.66, -27.3 + (15 * 2), 1, 13, -1, 1
    Plaque(5).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(5).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(6).AddWall GetTex("Plaque"), 12.66, -18.7 + (15 * 2), 12.66, -16.3 + (15 * 2), 1, 13, 1, 1
    Plaque(6).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(6).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(7).AddWall GetTex("Plaque"), -12.66, -29.7 + (15 * 3), -12.66, -27.3 + (15 * 3), 1, 13, -1, 1
    Plaque(7).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(7).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(8).AddWall GetTex("Plaque"), 12.66, -18.7 + (15 * 3), 12.66, -16.3 + (15 * 3), 1, 13, 1, 1
    Plaque(8).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(8).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(9).AddWall GetTex("Plaque"), -12.66, -29.7 + (15 * 4), -12.66, -27.3 + (15 * 4), 1, 13, -1, 1
    Plaque(9).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(9).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(10).AddWall GetTex("Plaque"), 12.66, -18.7 + (15 * 4), 12.66, -16.3 + (15 * 4), 1, 13, 1, 1
    Plaque(10).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(10).SetColor RGBA(1, 1, 1, 0.1)
    'Interior Panels
    Elevator(1).AddWall GetTex("Marble3"), -12.65, -16, -12.65, -19, 19.5, 0.1, 1, 1
    Elevator(1).AddWall GetTex("Marble3"), -12.65, -30, -12.65, -27, 19.5, 0.1, 1, 1
    Elevator(2).AddWall GetTex("Marble3"), 12.65, -16, 12.65, -19, 19.5, 0.1, 1, 1
    Elevator(2).AddWall GetTex("Marble3"), 12.65, -30, 12.65, -27, 19.5, 0.1, 1, 1
    Elevator(3).AddWall GetTex("Marble3"), -12.65, -16 + (15 * 1), -12.65, -19 + (15 * 1), 19.5, 0.1, 1, 1
    Elevator(3).AddWall GetTex("Marble3"), -12.65, -30 + (15 * 1), -12.65, -27 + (15 * 1), 19.5, 0.1, 1, 1
    Elevator(4).AddWall GetTex("Marble3"), 12.65, -16 + (15 * 1), 12.65, -19 + (15 * 1), 19.5, 0.1, 1, 1
    Elevator(4).AddWall GetTex("Marble3"), 12.65, -30 + (15 * 1), 12.65, -27 + (15 * 1), 19.5, 0.1, 1, 1
    Elevator(5).AddWall GetTex("Marble3"), -12.65, -16 + (15 * 2), -12.65, -19 + (15 * 2), 19.5, 0.1, 1, 1
    Elevator(5).AddWall GetTex("Marble3"), -12.65, -30 + (15 * 2), -12.65, -27 + (15 * 2), 19.5, 0.1, 1, 1
    Elevator(6).AddWall GetTex("Marble3"), 12.65, -16 + (15 * 2), 12.65, -19 + (15 * 2), 19.5, 0.1, 1, 1
    Elevator(6).AddWall GetTex("Marble3"), 12.65, -30 + (15 * 2), 12.65, -27 + (15 * 2), 19.5, 0.1, 1, 1
    Elevator(7).AddWall GetTex("Marble3"), -12.65, -16 + (15 * 3), -12.65, -19 + (15 * 3), 19.5, 0.1, 1, 1
    Elevator(7).AddWall GetTex("Marble3"), -12.65, -30 + (15 * 3), -12.65, -27 + (15 * 3), 19.5, 0.1, 1, 1
    Elevator(8).AddWall GetTex("Marble3"), 12.65, -16 + (15 * 3), 12.65, -19 + (15 * 3), 19.5, 0.1, 1, 1
    Elevator(8).AddWall GetTex("Marble3"), 12.65, -30 + (15 * 3), 12.65, -27 + (15 * 3), 19.5, 0.1, 1, 1
    Elevator(9).AddWall GetTex("Marble3"), -12.65, -16 + (15 * 4), -12.65, -19 + (15 * 4), 19.5, 0.1, 1, 1
    Elevator(9).AddWall GetTex("Marble3"), -12.65, -30 + (15 * 4), -12.65, -27 + (15 * 4), 19.5, 0.1, 1, 1
    Elevator(10).AddWall GetTex("Marble3"), 12.65, -16 + (15 * 4), 12.65, -19 + (15 * 4), 19.5, 0.1, 1, 1
    Elevator(10).AddWall GetTex("Marble3"), 12.65, -30 + (15 * 4), 12.65, -27 + (15 * 4), 19.5, 0.1, 1, 1
    'Interior Doors
    ElevatorInsDoorL(1).AddWall GetTex("ElevDoors"), -12.6, -19.05, -12.6, -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(1).AddWall GetTex("ElevDoors"), -12.6, -23.05, -12.6, -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(2).AddWall GetTex("ElevDoors"), 12.6, -19.05, 12.6, -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(2).AddWall GetTex("ElevDoors"), 12.6, -23.05, 12.6, -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(3).AddWall GetTex("ElevDoors"), -12.6, -19.05 + (15 * 1), -12.6, -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(3).AddWall GetTex("ElevDoors"), -12.6, -23.05 + (15 * 1), -12.6, -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(4).AddWall GetTex("ElevDoors"), 12.6, -19.05 + (15 * 1), 12.6, -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(4).AddWall GetTex("ElevDoors"), 12.6, -23.05 + (15 * 1), 12.6, -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(5).AddWall GetTex("ElevDoors"), -12.6, -19.05 + (15 * 2), -12.6, -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(5).AddWall GetTex("ElevDoors"), -12.6, -23.05 + (15 * 2), -12.6, -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(6).AddWall GetTex("ElevDoors"), 12.6, -19.05 + (15 * 2), 12.6, -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(6).AddWall GetTex("ElevDoors"), 12.6, -23.05 + (15 * 2), 12.6, -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(7).AddWall GetTex("ElevDoors"), -12.6, -19.05 + (15 * 3), -12.6, -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(7).AddWall GetTex("ElevDoors"), -12.6, -23.05 + (15 * 3), -12.6, -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(8).AddWall GetTex("ElevDoors"), 12.6, -19.05 + (15 * 3), 12.6, -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(8).AddWall GetTex("ElevDoors"), 12.6, -23.05 + (15 * 3), 12.6, -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(9).AddWall GetTex("ElevDoors"), -12.6, -19.05 + (15 * 4), -12.6, -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(9).AddWall GetTex("ElevDoors"), -12.6, -23.05 + (15 * 4), -12.6, -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(10).AddWall GetTex("ElevDoors"), 12.6, -19.05 + (15 * 4), 12.6, -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(10).AddWall GetTex("ElevDoors"), 12.6, -23.05 + (15 * 4), 12.6, -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    
    Call DrawElevatorButtons
    
End Sub

Sub ProcessStairs()
Form1.Print "Processing Stairs...";
    
i = 1
'Stairs on the first floor, section 1
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 - 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 - 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 - 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 - 25
    
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 - 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 - 25

'Stairs on the first floor, section 2
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14
    
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12

'Stairs on the first floor, section 3
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 + 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 + 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 + 25
    
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 + 25


    For i = 2 To 137
    Form1.Print ".";
    'Stairs
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 6, -30.85, 2, (i * 25) + 0 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 8, -30.85, 2, (i * 25) + 2 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 10, -30.85, 2, (i * 25) + 4 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 12, -30.85, 2, (i * 25) + 6 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 14, -30.85, 2, (i * 25) + 8 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25 + 7.71, -12.5 - 16, -30.85, 2, (i * 25) + 10 + 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -30.85, (i * 25) + 2 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -30.85, (i * 25) + 4 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -30.85, (i * 25) + 6 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -30.85, (i * 25) + 8 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -30.85, (i * 25) + 10 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 20, -30.85, (i * 25) + 12 + 25
    
    Stairs(i).AddFloor GetTex("stairs"), -12.5, -46.25, -12.5 - 6, -30.85, (i * 25) + 25 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 6, -46.25 + 7.71, -12.5 - 8, -46.25, (i * 25) + 22 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 8, -46.25 + 7.71, -12.5 - 10, -46.25, (i * 25) + 20 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 10, -46.25 + 7.71, -12.5 - 12, -46.25, (i * 25) + 18 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 12, -46.25 + 7.71, -12.5 - 14, -46.25, (i * 25) + 16 + 25
    Stairs(i).AddFloor GetTex("stairs"), -12.5 - 14, -46.25 + 7.71, -12.5 - 16, -46.25, (i * 25) + 14 + 25
    
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 6, -46.25, -12.5 - 6, -46.25 + 7.71, 3, (i * 25) + 22 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 8, -46.25, -12.5 - 8, -46.25 + 7.71, 2, (i * 25) + 20 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 10, -46.25, -12.5 - 10, -46.25 + 7.71, 2, (i * 25) + 18 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 12, -46.25, -12.5 - 12, -46.25 + 7.71, 2, (i * 25) + 16 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 14, -46.25, -12.5 - 14, -46.25 + 7.71, 2, (i * 25) + 14 + 25
    Stairs(i).AddWall GetTex("stairs"), -12.5 - 16, -46.25, -12.5 - 16, -46.25 + 7.71, 2, (i * 25) + 12 + 25
    
    Next i
    Form1.Print "done"


End Sub

