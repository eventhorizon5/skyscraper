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

Public Room As TVMesh
Public CrawlSpace As TVMesh
Public CrawlSpaceShaft1 As TVMesh
Public CrawlSpaceShaft2 As TVMesh
Public CrawlSpaceShaft3 As TVMesh
Public CrawlSpaceShaft4 As TVMesh
Public Shafts1 As TVMesh
Public Shafts2 As TVMesh
Public Shafts3 As TVMesh
Public Shafts4 As TVMesh
Public ShaftsFloor As TVMesh
Public Stairs As TVMesh
Private FakeStairDoor(1 To 4) As TVMesh
Private Objects(150) As TVMesh
Public Enabled As Boolean
Public StairDataTable As Boolean
Public FloorHeightTable As Boolean
Public FloorNameTable As Boolean
Public Number As Integer

Public Function GetName() As String
'Returns the string name of the specified floor
GetFloorName = FloorNameTable(Floor)
End Function

Public Sub SetName(FName As String)
'Sets the specified floor's name
FloorNameTable(Floor) = FName
End Sub

Public Function GetAltitude(Floor As Integer) As Single
'Gets altitude (Y) value of floor

GetFloorAltitude = FloorHeightTable(Floor, 0)

End Function

Public Function GetHeight() As Single
'Get height of floor from the floor height table
GetFloorHeight = FloorHeightTable(Floor, 1)

End Function

Public Sub SetHeight(Height As Single)
'Set height of floor in the floor height table
FloorHeightTable(Floor, 1) = Height

End Sub

Public Sub SetCameraFloor()
'Moves camera to specified floor (sets altitude to the floor's altitude plus 10)
Camera.SetPosition Camera.GetPosition.X, GetFloorAltitude(Floor) + 10, Camera.GetPosition.X

End Sub

Public Sub Init(Num As Integer)
'Set floor's object number
Number = Num

'Create and name new meshes
Set Room = Scene.CreateMeshBuilder("Room " + Str$(Number))
Set CrawlSpace = Scene.CreateMeshBuilder("CrawlSpace " + Str$(Number))
Set CrawlSpaceShaft1 = Scene.CreateMeshBuilder("CrawlSpaceShaft1 " + Str$(Number))
Set CrawlSpaceShaft2 = Scene.CreateMeshBuilder("CrawlSpaceShaft2 " + Str$(Number))
Set CrawlSpaceShaft3 = Scene.CreateMeshBuilder("CrawlSpaceShaft3 " + Str$(Number))
Set CrawlSpaceShaft4 = Scene.CreateMeshBuilder("CrawlSpaceShaft4 " + Str$(Number))
Set ShaftsFloor = Scene.CreateMeshBuilder("ShaftsFloor " + Str$(Number))
Set Shafts1 = Scene.CreateMeshBuilder("Shafts1 " + Str$(Number))
Set Shafts2 = Scene.CreateMeshBuilder("Shafts2 " + Str$(Number))
Set Shafts3 = Scene.CreateMeshBuilder("Shafts3 " + Str$(Number))
Set Shafts4 = Scene.CreateMeshBuilder("Shafts4 " + Str$(Number))

End Sub
