VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "FloorObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Skyscraper 1.1 Alpha - Floor Class
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

Public Level As TVMesh
Public CrawlSpace As TVMesh
Public ShaftsFloor As TVMesh
Private CrawlSpaceShaft() As TVMesh
Private Shafts() As TVMesh
Private Stairs() As TVMesh
Private FakeStairDoor() As TVMesh
Private Objects() As TVMesh
Public Enabled As Boolean
'Public StairDataTable As Boolean
Public Number As Integer
Public FloorName As String
Public FloorAltitude As Single
Public FloorHeight As Single
Public CrawlSpaceHeight As Single

Public Sub SetCameraFloor()
'Moves camera to specified floor (sets altitude to the floor's altitude plus CameraAltitude)
Camera.SetPosition Camera.GetPosition.X, FloorAltitude + CameraDefAltitude, Camera.GetPosition.X
End Sub

Public Sub AddFloor(Texture As String, x1 As Single, z1 As Single, x2 As Single, z2 As Single, voffset As Single, tw As Single, th As Single)
'Adds a floor with the specified dimensions and vertical offset
Dim tw2 As Single
Dim th2 As Single
tw2 = tw
th2 = th

'Sizing formulas from version 1.0
If tw = 0 Then tw2 = (Abs(x1) + Abs(x2)) * 0.086
If th = 0 Then tw2 = (Abs(z1) + Abs(z2)) * 0.08

Level.AddFloor GetTex(Texture), x1, z1, x2, z2, FloorAltitude + voffset, tw2, th2
End Sub

Public Sub AddWall(Texture As String, x1 As Single, z1 As Single, x2 As Single, z2 As Single, height As Single, voffset As Single, tw As Single, th As Single)
'Adds a wall with the specified dimensions
Dim tw2 As Single
Dim th2 As Single
tw2 = tw
th2 = th

'Sizing formulas from version 1.0
If tw = 0 Then tw2 = (Abs(x1) + Abs(x2)) * 0.086
If th = 0 Then tw2 = (Abs(z1) + Abs(z2)) * 0.08

Level.AddWall GetTex(Texture), x1, z1, x2, z2, height, FloorAltitude + voffset, tw2, th2
End Sub

Sub CreateWallBox(TextureName As String, WidthX As Single, LengthZ As Single, CenterX As Single, CenterZ As Single, CSpace As Boolean, ResX As Single, ResY As Single)
Dim height As Single
Dim WallHeight As Single
Dim x1 As Single
Dim x2 As Single
Dim z1 As Single
Dim z2 As Single
height = FloorAltitude
WallHeight = FloorHeight - CrawlSpaceHeight
If CSpace = True Then
    height = WallHeight
    WallHeight = CrawlSpaceHeight
End If
x1 = CenterX - (WidthX / 2)
x2 = CenterX + (WidthX / 2)
z1 = CenterZ - (LengthZ / 2)
z1 = CenterZ + (LengthZ / 2)

Level.AddWall GetTex(TextureName), x1, z1, x2, z1, WallHeight, height, (WidthX * (0.086 * ResX)), ResY
Level.AddWall GetTex(TextureName), x2, z1, x2, z2, WallHeight, height, (WidthX * (0.086 * ResX)), ResY
Level.AddWall GetTex(TextureName), x2, z2, x1, z2, WallHeight, height, (WidthX * (0.086 * ResX)), ResY
Level.AddWall GetTex(TextureName), x1, z2, x1, z1, WallHeight, height, (WidthX * (0.086 * ResX)), ResY

End Sub

Public Sub Initialize(Num As Integer)
'Set floor's object number
Number = Num
'size shaft arrays accordingly
ReDim CrawlSpaceShaft(ElevatorShafts)
ReDim Shafts(ElevatorShafts)

'Create and name new meshes
Set Level = Scene.CreateMeshBuilder("Level " + Str$(Number))
Set CrawlSpace = Scene.CreateMeshBuilder("CrawlSpace " + Str$(Number))
Set ShaftsFloor = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(Number))
Dim i As Integer
For i = 1 To ElevatorShafts
    DoEvents
    Set CrawlSpaceShaft(i) = Scene.CreateMeshBuilder("CrawlSpaceShaft" + LTrim(Str$(i)) + " " + Str$(Number))
    Set Shafts(i) = Scene.CreateMeshBuilder("Shafts" + LTrim(Str$(i)) + " " + Str$(Number))
Next i

End Sub

Private Sub Class_Terminate()
'Set Level = Nothing
'Set CrawlSpace = Nothing
'Set ShaftsFloor = Nothing
'Dim i As Integer
'For i = 1 To ElevatorShafts
'    DoEvents
'    Set CrawlSpaceShaft(i) = Nothing
'    Set Shafts(i) = Nothing
'Next i

End Sub
