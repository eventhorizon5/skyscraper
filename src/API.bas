Attribute VB_Name = "API"
'Skyscraper 1.1 Alpha - Core API functions
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
'

Option Explicit

Function IsEven(Number As Integer) As Boolean
'Determine if the passed number is even.
'If number divides evenly, return true
If Number / 2 = Int(Number / 2) Then
    IsEven = True
Else
    IsEven = False
End If

End Function

Function GetCameraFloor() As Integer
'Determine what floor the camera is on
Dim curfloor As Integer
Dim total As Single

'Find the upper and lower bounds of each floor, then see if the
'camera altitude is within the bounds
'if camera is above TopFloor, then returns TopFloor
For curfloor = BottomFloor To TopFloor
DoEvents
total = GetFloorAltitude(curfloor)
If Camera.GetPosition.Y >= total And Camera.GetPosition.Y < total + GetFloorHeight(curfloor) Then Exit For
Next curfloor

GetCameraFloor = curfloor

End Function

Function GetFloorExact(Number As Single) As Single
'Determine what floor the specified altitude is part of
Dim curfloor As Integer

'Find the upper and lower bounds of each floor, then see if the
'altitude is within the bounds
For curfloor = BottomFloor To TopFloor
DoEvents
If Number >= GetFloorAltitude(curfloor) And Number < GetFloorAltitude(curfloor) + GetFloorHeight(curfloor) Then Exit For
Next curfloor

GetFloorExact = curfloor + ((Number - GetFloorAltitude(curfloor)) / GetFloorHeight(curfloor))
End Function


Sub CreateWallBox(DestMesh As TVMesh, TextureName As String, WidthX As Single, LengthZ As Single, CenterX As Single, CenterZ As Single, Floor As Integer, CSpace As Boolean, ResX As Single, ResY As Single)
Dim Height As Single
Dim WallHeight As Single
Dim x1 As Single
Dim x2 As Single
Dim z1 As Single
Dim z2 As Single
Height = GetFloorAltitude(Floor)
WallHeight = GetFloorHeight(Floor) - CrawlSpaceHeight
If CSpace = True Then
    Height = WallHeight
    WallHeight = CrawlSpaceHeight
End If
x1 = CenterX - (WidthX / 2)
x2 = CenterX + (WidthX / 2)
z1 = CenterZ - (LengthZ / 2)
z1 = CenterZ + (LengthZ / 2)

DestMesh.AddWall GetTex(TextureName), x1, z1, x2, z1, WallHeight, Height, (WidthX * (0.086 * ResX)), ResY
DestMesh.AddWall GetTex(TextureName), x2, z1, x2, z2, WallHeight, Height, (WidthX * (0.086 * ResX)), ResY
DestMesh.AddWall GetTex(TextureName), x2, z2, x1, z2, WallHeight, Height, (WidthX * (0.086 * ResX)), ResY
DestMesh.AddWall GetTex(TextureName), x1, z2, x1, z1, WallHeight, Height, (WidthX * (0.086 * ResX)), ResY

End Sub

