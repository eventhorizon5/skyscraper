Attribute VB_Name = "CoreRoutines2"
'Skycraper 0.95 Beta
'Copyright (C) 2003 Ryan Thoryk
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

Sub DrawElevatorWalls(FloorID As Integer, SectionNum As Integer, ShaftNum As Integer, JoinShafts As Boolean, OpenShaft As Boolean, e1 As Boolean, e2 As Boolean, e3 As Boolean, e4 As Boolean, e5 As Boolean, e6 As Boolean, e7 As Boolean, e8 As Boolean, e9 As Boolean, e10 As Boolean)

'Shaftnum is the number of the shaft, for layout purposes
'1=center in (for floors 118-138)
'2=center out (for floors 80-117)
'3=outer in (for floors 40-79)
'4=outer out (for floors 2-39)

'Openshaft determines if the pipe shaft door(s) should be created for that floor

Dim q
Dim WallOffset As Single

'SectionNum determines the length of the shafts
'1 = originally lobby (5 elevators each w/lobby tex)
'2 = originally 2-39 (5 elevators each w/room tex)
'3 = originally 40-79 (4 elevators each)
'4 = originally 80-117 (3 elevators each)
'5 = originally 118-134 (2 elevators each)
'6 = originally 135-136 (1 elevator each)
'7 = originally 137-138 (1 elevator each w/brick texture)

'WallOffset is the distance between the walls and the elevator itself

Dim TextureName As String
Dim ShaftEnd As Single
Dim ShaftLeft As Single
Dim ShaftRight As Single

If SectionNum = 1 Then TextureName = "Wall2"
If SectionNum > 1 And SectionNum < 7 Then TextureName = "Wall1"
If SectionNum = 7 Then TextureName = "BrickTexture"
If FloorID = 1 Then TextureName = "Wall2"

If ShaftNum = 1 Then ShaftLeft = 12.5: ShaftRight = 32.5
If ShaftNum = 2 Then ShaftLeft = 52.5: ShaftRight = 32.5
If ShaftNum = 3 Then ShaftLeft = 90.5: ShaftRight = 110.5
If ShaftNum = 4 Then ShaftLeft = 130.5: ShaftRight = 110.5

If ShaftNum = 1 Or ShaftNum = 3 Then WallOffset = 0.05
If ShaftNum = 2 Or ShaftNum = 4 Then WallOffset = -0.05

q = FloorID
If FloorID = 1 Then q = -1
If FloorID = -1 Then q = -2
If FloorID = -2 Then q = -3
If FloorID = -3 Then q = -4
If FloorID = -4 Then q = -5
If FloorID = -5 Then q = -6
If FloorID = -6 Then q = -7
If FloorID = -7 Then q = -8
If FloorID = -8 Then q = -9
If FloorID = -9 Then q = -10
If FloorID = -10 Then q = -11

        'wall left of stairs
        If ShaftNum = 1 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, -40.3, 19.5, (q * FloorHeight) + FloorHeight, (6 * 0.086), (19.5 * 0.08)
        Else
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, -32.5, 19.5, (q * FloorHeight) + FloorHeight, (6 * 0.086), (19.5 * 0.08)
        End If
        'wall between stairs and 1st elevator
        If e1 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -32.5, -ShaftLeft, -30, 19.5, (q * FloorHeight) + FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -32.5, -ShaftLeft - WallOffset, -30.5, 19.5, (q * FloorHeight) + FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -32.5, -ShaftLeft - WallOffset, -30.5, 19.5, (q * FloorHeight) + FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
        Else
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -32.5, -ShaftLeft, -16, 19.5, (q * FloorHeight) + FloorHeight, (16.5 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -32.5, -ShaftLeft - WallOffset, -16, 19.5, (q * FloorHeight) + FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -32.5, -ShaftLeft - WallOffset, -16, 19.5, (q * FloorHeight) + FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
        End If
        'wall on left, after 2nd elevator
        If e2 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, -30, 19.5, (q * FloorHeight) + FloorHeight, (16.25 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, -30, 19.5, (q * FloorHeight) + FloorHeight, (16.25 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, -30, 19.5, (q * FloorHeight) + FloorHeight, (16.25 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 7 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, -16, 19.5, (q * FloorHeight) + FloorHeight, (30 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, -16, 19.5, (q * FloorHeight) + FloorHeight, (30 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, -16, 19.5, (q * FloorHeight) + FloorHeight, (30 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 1st and 3rd elevator
        If e3 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -16, -ShaftLeft, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 6 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -16, -ShaftLeft, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 3rd and 5th elevator
        If e5 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -1, -ShaftLeft, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -1, -ShaftLeft - WallOffset, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -1, -ShaftLeft - WallOffset, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 5 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -1, -ShaftLeft, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -1, -ShaftLeft - WallOffset, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -1, -ShaftLeft - WallOffset, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 5th and 7th elevator
        If e7 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 14, -ShaftLeft, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 14, -ShaftLeft, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 7th and 9th elevator
        If e9 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 29, -ShaftLeft, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 3 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 29, -ShaftLeft, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 2nd and 4th elevator
        If e4 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -16, ShaftLeft, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, -15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 6 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -16, ShaftLeft, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, -1, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 4th and 6th elevator
        If e6 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -1, ShaftLeft, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -1, ShaftLeft + WallOffset, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -1, ShaftLeft + WallOffset, 0, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 5 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -1, ShaftLeft, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -1, ShaftLeft + WallOffset, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -1, ShaftLeft + WallOffset, 14, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 6th and 8th elevator
        If e8 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 14, ShaftLeft, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, 15, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 14, ShaftLeft, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, 29, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        'wall between 8th and 10th elevator
        If e10 = True Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 29, ShaftLeft, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, 30, 19.5, (q * FloorHeight) + FloorHeight, (1 * 0.086), (19.5 * 0.08)
        Else
            If SectionNum < 3 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 29, ShaftLeft, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, 44, 19.5, (q * FloorHeight) + FloorHeight, (15 * 0.086), (19.5 * 0.08)
            End If
        End If
        
        'Mezzanine stuff
        If FloorID = 1 Then
            'right side
            If ShaftNum = 1 Or ShaftNum = 2 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, -40.3, 19.5, FloorHeight, (6 * 0.086), (19.5 * 0.08)
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -32.5, -ShaftLeft, -30, 19.5, FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -16, -ShaftLeft, 0, 19.5, FloorHeight, (16 * 0.086), (19.5 * 0.08)
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, 0, (FloorHeight - 19.5), FloorHeight + 19.5, (92.5 * 0.086), (19.5 * 0.08)
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -32.5, -ShaftLeft - WallOffset, -30.5, 19.5, FloorHeight, (2.5 * 0.086), (19.5 * 0.08)
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, 0, 19.5, FloorHeight, (16 * 0.086), (19.5 * 0.08)
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -30.5, -ShaftLeft - WallOffset, 0, (FloorHeight - 19.5), FloorHeight + 19.5, (76.25 * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, 46.25, FloorHeight, FloorHeight, (92.5 * 0.086), (23 * 0.08)
                Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -46.25, -ShaftLeft - WallOffset, 46.25, FloorHeight, FloorHeight, (92.5 * 0.086), (23 * 0.08)
            End If
            
            'left side
            If ShaftNum = 1 Or ShaftNum = 2 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, -30, 19.5, FloorHeight, (16.25 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -16, ShaftLeft, 0, 19.5, FloorHeight, (16 * 0.086), (19.5 * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 0, (FloorHeight - 19.5), FloorHeight + 19.5, (92.5 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, 0, 19.5, FloorHeight, (16 * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 0, (FloorHeight - 19.5), FloorHeight + 19.5, (92.5 * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 46.25, FloorHeight, FloorHeight, (92.5 * 0.086), (23 * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 46.25, FloorHeight, FloorHeight, (92.5 * 0.086), (23 * 0.08)
            End If
        End If
        
        If SectionNum <= 2 Then ShaftEnd = 46.25
        If SectionNum = 3 Then ShaftEnd = 30.83
        If SectionNum = 4 Then ShaftEnd = 15.41
        If SectionNum = 5 Then ShaftEnd = 0
        If SectionNum >= 6 Then ShaftEnd = -15.42
        
        'this section places any shaft floors/ceilings that need to be made
        If FloorID = 1 Then
            If ShaftNum = 1 Or ShaftNum = 2 Then
            Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -30, -ShaftRight, 0, 0.05, (20 * 0.086), (46 * 0.08)
            Shafts2(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, -30, ShaftRight, 0, 0.05, (20 * 0.086), (46 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            Shafts3(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -30, -ShaftRight, 46.25, 0.05, (20 * 0.086), (46 * 0.08)
            Shafts4(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, -30, ShaftRight, 46.25, 0.05, (20 * 0.086), (46 * 0.08)
            End If
        End If
        If FloorID = 39 Then
            If ShaftNum = 4 Then
            Shafts3(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -46.25, -ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            Shafts4(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, -46.25, ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            End If
        End If
        If FloorID = 79 Then
            If ShaftNum = 3 Then
            Shafts3(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -46.25, -ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            Shafts4(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, -46.25, ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            End If
        End If
        If FloorID = 117 Then
            If ShaftNum = 2 Then
            Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -46.25, -ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            Shafts2(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, -46.25, ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            End If
        End If
        If FloorID = 129 Then
            If ShaftNum = 1 Then
            Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, 0, -ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            Shafts2(FloorID).AddFloor GetTex("BrickTexture"), ShaftLeft, 0, ShaftRight, 46.25, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
            End If
        End If
        If FloorID = 138 Then
        Shafts1(FloorID).AddFloor GetTex("BrickTexture"), -ShaftLeft, -30, -ShaftRight, -15.42, (FloorID * FloorHeight) + FloorHeight + 24.95, (20 * 0.086), (16 * 0.08)
        End If
        
        'this section places the wall that extends to the end of the shaft, according to where the last elevator is
        If SectionNum = 7 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -16, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 6 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -16, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -16, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -16, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -16, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((16 + ShaftEnd) * 0.086), (19.5 * 0.08)
        End If
        If SectionNum = 5 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -1, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -1, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, -1, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, -1, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, -1, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, -1, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-ShaftLeft + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
        End If
        If SectionNum = 4 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 14, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 14, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 14, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 14, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-14 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
        End If
        If SectionNum = 3 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 29, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 29, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 29, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 29, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-29 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
        End If
        If SectionNum <= 2 Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 44, -ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 44, ShaftLeft, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 44, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 44, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
            If ShaftNum = 3 Or ShaftNum = 4 Then
            Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 44, -ShaftLeft - WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, 44, ShaftLeft + WallOffset, ShaftEnd, 19.5, (q * FloorHeight) + FloorHeight, ((-44 + ShaftEnd) * 0.086), (19.5 * 0.08)
            End If
        End If
        
        'walls above
        If FloorID <> 1 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((46.25 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
            If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -30.5, -ShaftLeft - WallOffset, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((30 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
            If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -30.5, -ShaftLeft - WallOffset, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((30 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
            If SectionNum <> 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((46.25 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
                If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((46.25 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
                If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, ShaftEnd, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, ((46.25 + ShaftEnd) * 0.086), ((FloorHeight - 19.5) * 0.08)
            End If
        End If
        If FloorID = 1 Then
            If ShaftNum = 3 Or ShaftNum = 4 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, 46.25, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 46.25, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -30.5, -ShaftLeft - WallOffset, 46.25, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 76.25 * 0.086, ((FloorHeight - 19.5) * 0.08)
            Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 46.25, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            End If
            If ShaftNum = 1 Or ShaftNum = 2 Then
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftLeft, 0, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 0, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, -30.5, -ShaftLeft - WallOffset, 0, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 76.25 * 0.086, ((FloorHeight - 19.5) * 0.08)
            Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 0, (FloorHeight - 19.5), 19.5 + (q * FloorHeight) + FloorHeight, 92.5 * 0.086, ((FloorHeight - 19.5) * 0.08)
            End If
        End If
    'other parts
    
    If OpenShaft = True Then
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, -46.25, ShaftRight, -43, FloorHeight, (q * FloorHeight) + FloorHeight, 3.25 * 0.086, 2
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, -46.25, ShaftRight, -34, 10, (q * FloorHeight) + FloorHeight + 15, 12.25 * 0.086, 10 * 0.08
        ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, -46.25, ShaftRight, -34, 8, (q * FloorHeight) + FloorHeight, 12.25 * 0.086, 8 * 0.08
        If ShaftNum = 1 Or ShaftNum = 2 Then
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -43, FloorHeight, (q * FloorHeight) + FloorHeight, 3.25 * 0.086, 2
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -34, 10, (q * FloorHeight) + FloorHeight + 15, 12.25 * 0.086, 10 * 0.08
        Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -34, 8, (q * FloorHeight) + FloorHeight, 12.25 * 0.086, 8 * 0.08
        End If
        If ShaftNum = 3 Or ShaftNum = 4 Then
        Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -43, FloorHeight, (q * FloorHeight) + FloorHeight, 3.25 * 0.086, 2
        Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -34, 10, (q * FloorHeight) + FloorHeight + 15, 12.25 * 0.086, 10 * 0.08
        Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), -46.25, (ShaftRight - WallOffset), -34, 8, (q * FloorHeight) + FloorHeight, 12.25 * 0.086, 8 * 0.08
        End If
    End If
    
    If SectionNum <> 7 Then
    ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, -46.25, ShaftLeft, -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
    If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftRight, -46.25 + WallOffset, ShaftLeft, -46.25 + WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
    If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftRight, -46.25 + WallOffset, ShaftLeft, -46.25 + WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
    End If
    
    ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftRight, -46.25, -ShaftLeft, -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
    If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight, -30 + WallOffset, -ShaftLeft, -30 + WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
    If ShaftNum = 3 Or ShaftNum = 4 Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight, -30 + WallOffset, -ShaftLeft, -30 + WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
            
            If FloorID = 1 Then
                If ShaftNum = 3 Or ShaftNum = 4 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 46.25, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 46.25, -ShaftLeft, -46.25, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 46.25, ShaftRight, 46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, 46.25, ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 92.5 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 46.25, -ShaftRight, 46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftRight, 46.25, -ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 46.25, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 46.25, -ShaftLeft - WallOffset, -30.5, FloorHeight, (FloorHeight * 2), 16.25 * 0.086, 2
                Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, -46.25 + WallOffset, ShaftRight, -46.25 + WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, 46.25 - WallOffset, ShaftRight, 46.25 - WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), 46.25, (ShaftRight - WallOffset), -46.25, (FloorHeight * 2), FloorHeight, 92.5 * 0.086, 4
                Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, -30 + WallOffset, -ShaftRight, -30 + WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 2
                Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, 46.25 - WallOffset, -ShaftRight, 46.25 - WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight + WallOffset, 46.25, -ShaftRight + WallOffset, -30.5, (FloorHeight * 2), FloorHeight, 16.25 * 0.086, 4
                End If
                
                If ShaftNum = 1 Or ShaftNum = 2 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftLeft, 0, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 0, -ShaftLeft, -46.25, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, -46.25, ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, 0, ShaftRight, 0, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, 0, ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 92.5 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, -46.25, -ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, 0, -ShaftRight, 0, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftRight, 0, -ShaftRight, -46.25, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft + WallOffset, -46.25, ShaftLeft + WallOffset, 0, FloorHeight, (FloorHeight * 2), 92.5 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft - WallOffset, 0, -ShaftLeft - WallOffset, -30.5, FloorHeight, (FloorHeight * 2), 16.25 * 0.086, 2
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, -46.25 + WallOffset, ShaftRight, -46.25 + WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, 0 - WallOffset, ShaftRight, 0 - WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), 0, (ShaftRight - WallOffset), -46.25, (FloorHeight * 2), FloorHeight, 92.5 * 0.086, 4
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, -30 + WallOffset, -ShaftRight, -30 + WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, 0 - WallOffset, -ShaftRight, 0 - WallOffset, (FloorHeight * 2), FloorHeight, 20 * 0.086, 4
                'Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight + WallOffset, 0, -ShaftRight + WallOffset, -30.5, (FloorHeight * 2), FloorHeight, 16.25 * 0.086, 4
                End If
            End If
                
            'Main shaft area
            If SectionNum <> 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftLeft, ShaftEnd, ShaftRight, ShaftEnd, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, ShaftEnd - WallOffset, ShaftRight, ShaftEnd - WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), ShaftLeft, ShaftEnd - WallOffset, ShaftRight, ShaftEnd - WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                If OpenShaft = False And JoinShafts = False Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, ShaftEnd, ShaftRight, -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, (46.25 + ShaftEnd) * 0.086, 2
                If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), ShaftEnd, (ShaftRight - WallOffset), -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, (46.25 + ShaftEnd) * 0.086, 2
                If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), ShaftEnd, (ShaftRight - WallOffset), -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, (46.25 + ShaftEnd) * 0.086, 2
                End If
                If OpenShaft = True And JoinShafts = False Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), ShaftRight, ShaftEnd, ShaftRight, -34, FloorHeight, (q * FloorHeight) + FloorHeight, (34 + ShaftEnd) * 0.086, 2
                If ShaftNum = 1 Or ShaftNum = 2 Then Shafts2(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), ShaftEnd, (ShaftRight - WallOffset), -34, FloorHeight, (q * FloorHeight) + FloorHeight, (34 + ShaftEnd) * 0.086, 2
                If ShaftNum = 3 Or ShaftNum = 4 Then Shafts4(FloorID).AddWall GetTex("BrickTexture"), (ShaftRight - WallOffset), ShaftEnd, (ShaftRight - WallOffset), -34, FloorHeight, (q * FloorHeight) + FloorHeight, (34 + ShaftEnd) * 0.086, 2
                End If
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, ShaftEnd, -ShaftRight, ShaftEnd, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                If JoinShafts = False Then ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftRight, ShaftEnd, -ShaftRight, -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, (46.25 + ShaftEnd) * 0.086, 2
                If ShaftNum = 1 Or ShaftNum = 2 Then
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, ShaftEnd - WallOffset, -ShaftRight, ShaftEnd - WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                    If JoinShafts = False Then Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight + WallOffset, ShaftEnd, -ShaftRight + WallOffset, -30.5, FloorHeight, (q * FloorHeight) + FloorHeight, (30 + ShaftEnd) * 0.086, 2
                End If
                If ShaftNum = 3 Or ShaftNum = 4 Then
                Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, ShaftEnd - WallOffset, -ShaftRight, ShaftEnd - WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                    If JoinShafts = False Then Shafts3(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight + WallOffset, ShaftEnd, -ShaftRight + WallOffset, -30.5, FloorHeight, (q * FloorHeight) + FloorHeight, (30 + ShaftEnd) * 0.086, 2
                End If
            End If
            If SectionNum = 7 Then
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftLeft, ShaftEnd, -ShaftRight, ShaftEnd, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                ShaftsFloor(FloorID).AddWall GetTex(TextureName), -ShaftRight, ShaftEnd, -ShaftRight, -46.25, FloorHeight, (q * FloorHeight) + FloorHeight, (46.25 + ShaftEnd) * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftLeft, ShaftEnd - WallOffset, -ShaftRight, ShaftEnd - WallOffset, FloorHeight, (q * FloorHeight) + FloorHeight, 20 * 0.086, 2
                Shafts1(FloorID).AddWall GetTex("BrickTexture"), -ShaftRight + WallOffset, ShaftEnd, -ShaftRight + WallOffset, -30.5, FloorHeight, (q * FloorHeight) + FloorHeight, (30 + ShaftEnd) * 0.086, 2
            End If
    
    Call DrawElevatorWalls2(FloorID, SectionNum, ShaftNum, JoinShafts, OpenShaft, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10)
    
End Sub
Sub DrawElevatorWalls2(FloorID As Integer, SectionNum As Integer, ShaftNum As Integer, JoinShafts As Boolean, OpenShaft As Boolean, e1 As Boolean, e2 As Boolean, e3 As Boolean, e4 As Boolean, e5 As Boolean, e6 As Boolean, e7 As Boolean, e8 As Boolean, e9 As Boolean, e10 As Boolean)
Dim TextureName As String
Dim ShaftEnd As Single
Dim ShaftLeft As Single
Dim ShaftRight As Single
Dim WallOffset As Single
Dim q

If SectionNum = 1 Then TextureName = "Wall2"
If SectionNum > 1 And SectionNum < 7 Then TextureName = "Wall1"
If SectionNum = 7 Then TextureName = "BrickTexture"
If FloorID = 1 Then TextureName = "Wall2"

If ShaftNum = 1 Then ShaftLeft = 12.5: ShaftRight = 32.5
If ShaftNum = 2 Then ShaftLeft = 52.5: ShaftRight = 32.5
If ShaftNum = 3 Then ShaftLeft = 90.5: ShaftRight = 110.5
If ShaftNum = 4 Then ShaftLeft = 130.5: ShaftRight = 110.5

If ShaftNum = 1 Or ShaftNum = 3 Then WallOffset = 0.05
If ShaftNum = 2 Or ShaftNum = 4 Then WallOffset = -0.05

q = FloorID
If FloorID = 1 Then q = -1
If FloorID = -1 Then q = -2
If FloorID = -2 Then q = -3
If FloorID = -3 Then q = -4
If FloorID = -4 Then q = -5
If FloorID = -5 Then q = -6
If FloorID = -6 Then q = -7
If FloorID = -7 Then q = -8
If FloorID = -8 Then q = -9
If FloorID = -9 Then q = -10
If FloorID = -10 Then q = -11

If SectionNum <= 2 Then ShaftEnd = 46.25
        If SectionNum = 3 Then ShaftEnd = 30.83
        If SectionNum = 4 Then ShaftEnd = 15.41
        If SectionNum = 5 Then ShaftEnd = 0
        If SectionNum >= 6 Then ShaftEnd = -15.42
       
'Exterior Doors
    If q = -1 Then
        If e1 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, 0, 1, 1
        If e1 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts3(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, 0, 1, 1
        
        If ShaftNum = 1 Or ShaftNum = 2 Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, FloorHeight, 1, 1
        
        If e2 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset) + WallOffset, -16, (ShaftLeft + WallOffset) + WallOffset, -30, 19.6, 0, 1, 1
        If e2 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset) + WallOffset, -16, (ShaftLeft + WallOffset) + WallOffset, -30, 19.6, 0, 1, 1
        
        If ShaftNum = 1 Then
        Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, FloorHeight, 1, 1
        End If
        If ShaftNum = 2 Then
        Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, FloorHeight, 1, 1
        End If
        
        If e3 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, 0, 1, 1
        If e3 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts3(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, 0, 1, 1
        
        If e4 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, 0, 1, 1
        If e4 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts4(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, 0, 1, 1
        
        If e5 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, 0, 1, 1
        If e5 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts3(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, 0, 1, 1
        
        If e6 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, 0, 1, 1
        If e6 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts4(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, 0, 1, 1
        
        If e7 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, 0, 1, 1
        If e7 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts3(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, 0, 1, 1
        
        If e8 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, 0, 1, 1
        If e8 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts4(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, 0, 1, 1
        
        If e9 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts1(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, 0, 1, 1
        If e9 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts3(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, 0, 1, 1
        
        If e10 = True And (ShaftNum = 1 Or ShaftNum = 2) Then Shafts2(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, 0, 1, 1
        If e10 = True And (ShaftNum = 3 Or ShaftNum = 4) Then Shafts4(1).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, 0, 1, 1
        
        'elev 1
        If e1 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16, -ShaftLeft, -19, 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30, -ShaftLeft, -27, 19.6, 0, 0.5, 1
        End If
        'elev 2
        If e2 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16, ShaftLeft, -19, 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30, ShaftLeft, -27, 19.6, 0, 0.5, 1
        End If
        'elev 3
        If e3 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 1), -ShaftLeft, -19 + (15 * 1), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 1), -ShaftLeft, -27 + (15 * 1), 19.6, 0, 0.5, 1
        End If
        'elev 4
        If e4 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 1), ShaftLeft, -19 + (15 * 1), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 1), ShaftLeft, -27 + (15 * 1), 19.6, 0, 0.5, 1
        End If
        'elev 5
        If e5 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 2), -ShaftLeft, -19 + (15 * 2), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 2), -ShaftLeft, -27 + (15 * 2), 19.6, 0, 0.5, 1
        End If
        'elev 6
        If e6 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 2), ShaftLeft, -19 + (15 * 2), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 2), ShaftLeft, -27 + (15 * 2), 19.6, 0, 0.5, 1
        End If
        'elev 7
        If e7 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 3), -ShaftLeft, -19 + (15 * 3), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 3), -ShaftLeft, -27 + (15 * 3), 19.6, 0, 0.5, 1
        End If
        'elev 8
        If e8 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 3), ShaftLeft, -19 + (15 * 3), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 3), ShaftLeft, -27 + (15 * 3), 19.6, 0, 0.5, 1
        End If
        'elev 9
        If e9 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 4), -ShaftLeft, -19 + (15 * 4), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 4), -ShaftLeft, -27 + (15 * 4), 19.6, 0, 0.5, 1
        End If
        'elev 10
        If e10 = True Then
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 4), ShaftLeft, -19 + (15 * 4), 19.6, 0, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 4), ShaftLeft, -27 + (15 * 4), 19.6, 0, 0.5, 1
        End If
        
        If ShaftNum = 1 Then
        'elev 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16, -ShaftLeft, -19, 19.6, FloorHeight, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30, -ShaftLeft, -27, 19.6, FloorHeight, 0.5, 1
        'elev 2
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16, ShaftLeft, -19, 19.6, FloorHeight, 0.5, 1
        Room(1).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30, ShaftLeft, -27, 19.6, FloorHeight, 0.5, 1
        End If
    End If
    
    If e1 = True Then
    If ShaftNum = 1 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16, -(ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16, -ShaftLeft, -19, 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30, -ShaftLeft, -27, 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e2 = True Then
    If ShaftNum = 1 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16, (ShaftLeft + WallOffset), -30, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    
    'elev 2
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16, ShaftLeft, -19, 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30, ShaftLeft, -27, 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e3 = True Then
    If ShaftNum = 1 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 1), -(ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 1), -ShaftLeft, -19 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 1), -ShaftLeft, -27 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e4 = True Then
    If ShaftNum = 1 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 1), (ShaftLeft + WallOffset), -30 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 1), ShaftLeft, -19 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 1), ShaftLeft, -27 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e5 = True Then
    If ShaftNum = 1 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 2), -(ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 2), -ShaftLeft, -19 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 2), -ShaftLeft, -27 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e6 = True Then
    If ShaftNum = 1 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 2), (ShaftLeft + WallOffset), -30 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 2), ShaftLeft, -19 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 2), ShaftLeft, -27 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e7 = True Then
    If ShaftNum = 1 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 3), -(ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 3), -ShaftLeft, -19 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 3), -ShaftLeft, -27 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e8 = True Then
    If ShaftNum = 1 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 3), (ShaftLeft + WallOffset), -30 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 3), ShaftLeft, -19 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 3), ShaftLeft, -27 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e9 = True Then
    If ShaftNum = 1 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts1(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts3(FloorID).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -16 + (15 * 4), -(ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -16 + (15 * 4), -ShaftLeft, -19 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), -ShaftLeft, -30 + (15 * 4), -ShaftLeft, -27 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
    
    If e10 = True Then
    If ShaftNum = 1 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 2 Then
    Shafts2(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 3 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    If ShaftNum = 4 Then
    Shafts4(FloorID).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -16 + (15 * 4), (ShaftLeft + WallOffset), -30 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
    End If
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -16 + (15 * 4), ShaftLeft, -19 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    Room(FloorID).AddWall GetTex("ElevExtPanels"), ShaftLeft, -30 + (15 * 4), ShaftLeft, -27 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 0.5, 1
    End If
        
End Sub

Sub ProcessRealtime(FloorID As Integer, SectionNum As Integer, ShaftNum As Integer, JoinShafts As Boolean, OpenShaft As Boolean, e1 As Boolean, e2 As Boolean, e3 As Boolean, e4 As Boolean, e5 As Boolean, e6 As Boolean, e7 As Boolean, e8 As Boolean, e9 As Boolean, e10 As Boolean)
'This subroutine is similar to the DrawElevatorWalls routines, and it is designed to create the external elevator doors and other objects in realtime.

Dim ShaftEnd As Single
Dim ShaftLeft As Single
Dim ShaftRight As Single
Dim WallOffset As Single
Dim WallOffset2 As Single
Dim q As Integer

q = FloorID
If FloorID = 1 Then q = -1

If ShaftNum = 1 Then ShaftLeft = 12.5: ShaftRight = 32.5
If ShaftNum = 2 Then ShaftLeft = 52.5: ShaftRight = 32.5
If ShaftNum = 3 Then ShaftLeft = 90.5: ShaftRight = 110.5
If ShaftNum = 4 Then ShaftLeft = 130.5: ShaftRight = 110.5

If SectionNum <= 2 Then ShaftEnd = 46.25
If SectionNum = 3 Then ShaftEnd = 30.83
If SectionNum = 4 Then ShaftEnd = 15.41
If SectionNum = 5 Then ShaftEnd = 0
If SectionNum >= 6 Then ShaftEnd = -15.42

If ShaftNum = 1 Or ShaftNum = 3 Then WallOffset = 0.05: WallOffset2 = 0.01
If ShaftNum = 2 Or ShaftNum = 4 Then WallOffset = -0.05: WallOffset2 = 0.01

'Call Button Panels
    'original height offset - 8.5
    If e1 = True And ShaftNum = 1 Then CallButtonsDown(1).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18, -(ShaftLeft - WallOffset2), -17, 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(1).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18, -(ShaftLeft - WallOffset2), -17, 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e1 = True And ShaftNum = 1 And FloorID = 1 Then CallButtonsDown(1).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18, -(ShaftLeft - WallOffset2), -17, 1.5, 8 + (0 * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(1).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18, -(ShaftLeft - WallOffset2), -17, 1.5, 9.5 + (0 * FloorHeight) + FloorHeight, 1, 1
    'If e2 = True And ShaftNum = 1 Then CallButtons(2).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29, (ShaftLeft - WallOffset2), -28, 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e2 = True And ShaftNum = 1 And FloorID = 1 Then CallButtons(2).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29, (ShaftLeft - WallOffset2), -28, 1.5, 8.5 + (0 * FloorHeight) + FloorHeight, 1, 1
    If e3 = True And ShaftNum = 1 Then CallButtonsDown(3).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 1), -(ShaftLeft - WallOffset2), -15 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(3).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 1), -(ShaftLeft - WallOffset2), -15 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e4 = True And ShaftNum = 1 Then CallButtonsDown(4).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 1), (ShaftLeft - WallOffset2), -30 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(4).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 1), (ShaftLeft - WallOffset2), -30 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e5 = True And ShaftNum = 1 Then CallButtons(5).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18 + (15 * 2), -(ShaftLeft - WallOffset2), -17 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e6 = True And ShaftNum = 1 Then CallButtonsDown(6).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -16 + (15 * 2), (ShaftLeft - WallOffset2), -15 + (15 * 2), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(6).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -16 + (15 * 2), (ShaftLeft - WallOffset2), -15 + (15 * 2), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e7 = True And ShaftNum = 1 Then CallButtonsDown(7).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -31 + (15 * 3), -(ShaftLeft - WallOffset2), -30 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(7).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -31 + (15 * 3), -(ShaftLeft - WallOffset2), -30 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e8 = True And ShaftNum = 1 Then CallButtons(8).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29 + (15 * 3), (ShaftLeft - WallOffset2), -28 + (15 * 3), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e9 = True And ShaftNum = 1 Then CallButtons(9).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18 + (15 * 4), -(ShaftLeft - WallOffset2), -17 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e10 = True And ShaftNum = 1 Then CallButtons(10).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29 + (15 * 4), (ShaftLeft - WallOffset2), -28 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    
    If e2 = True And ShaftNum = 2 Then CallButtonsDown(12).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18, (ShaftLeft + WallOffset2), -17, 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(12).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18, (ShaftLeft + WallOffset2), -17, 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e2 = True And ShaftNum = 2 And FloorID = 1 Then CallButtons(12).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18, (ShaftLeft + WallOffset2), -17, 1.5, 8.5 + (0 * FloorHeight) + FloorHeight, 1, 1
    If e1 = True And ShaftNum = 2 Then CallButtonsDown(11).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29, -(ShaftLeft + WallOffset2), -28, 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(11).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29, -(ShaftLeft + WallOffset2), -28, 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e1 = True And ShaftNum = 2 And FloorID = 1 Then CallButtons(11).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29, -(ShaftLeft + WallOffset2), -28, 1.5, 8.5 + (0 * FloorHeight) + FloorHeight, 1, 1
    If e4 = True And ShaftNum = 2 Then CallButtonsDown(14).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 1), (ShaftLeft + WallOffset2), -17 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(14).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 1), (ShaftLeft + WallOffset2), -17 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e3 = True And ShaftNum = 2 Then CallButtonsDown(13).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 1), -(ShaftLeft + WallOffset2), -28 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(13).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 1), -(ShaftLeft + WallOffset2), -28 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e6 = True And ShaftNum = 2 Then CallButtons(16).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 2), (ShaftLeft + WallOffset2), -17 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e5 = True And ShaftNum = 2 Then CallButtons(15).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 2), -(ShaftLeft + WallOffset2), -28 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e8 = True And ShaftNum = 2 Then CallButtonsDown(18).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 3), (ShaftLeft + WallOffset2), -15 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(18).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 3), (ShaftLeft + WallOffset2), -15 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e7 = True And ShaftNum = 2 Then CallButtonsDown(17).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 3), -(ShaftLeft + WallOffset2), -30 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(17).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 3), -(ShaftLeft + WallOffset2), -30 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e10 = True And ShaftNum = 2 Then CallButtons(20).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 4), (ShaftLeft + WallOffset2), -17 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e9 = True And ShaftNum = 2 Then CallButtons(19).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 4), -(ShaftLeft + WallOffset2), -28 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    
    
    'If e1 = True And ShaftNum = 3 Then CallButtons(21).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18, -(ShaftLeft - WallOffset2), -17, 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e2 = True And ShaftNum = 3 Then CallButtons(22).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29, (ShaftLeft - WallOffset2), -28, 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e3 = True And ShaftNum = 3 Then CallButtonsDown(23).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 1), -(ShaftLeft - WallOffset2), -15 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(23).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 1), -(ShaftLeft - WallOffset2), -15 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e4 = True And ShaftNum = 3 Then CallButtonsDown(24).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 1), (ShaftLeft - WallOffset2), -30 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(24).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 1), (ShaftLeft - WallOffset2), -30 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e5 = True And ShaftNum = 3 Then CallButtons(25).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18 + (15 * 2), -(ShaftLeft - WallOffset2), -17 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e6 = True And ShaftNum = 3 Then CallButtons(26).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29 + (15 * 2), (ShaftLeft - WallOffset2), -28 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e7 = True And ShaftNum = 3 Then CallButtonsDown(27).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 3), -(ShaftLeft - WallOffset2), -15 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(27).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -16 + (15 * 3), -(ShaftLeft - WallOffset2), -15 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e8 = True And ShaftNum = 3 Then CallButtonsDown(28).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 3), (ShaftLeft - WallOffset2), -30 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(28).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -31 + (15 * 3), (ShaftLeft - WallOffset2), -30 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e9 = True And ShaftNum = 3 Then CallButtons(29).AddWall GetTex("CallButtonsTex"), -(ShaftLeft - WallOffset2), -18 + (15 * 4), -(ShaftLeft - WallOffset2), -17 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e10 = True And ShaftNum = 3 Then CallButtons(30).AddWall GetTex("CallButtonsTex"), (ShaftLeft - WallOffset2), -29 + (15 * 4), (ShaftLeft - WallOffset2), -28 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    
    'If e2 = True And ShaftNum = 4 Then CallButtons(32).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18, (ShaftLeft + WallOffset2), -17, 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e1 = True And ShaftNum = 4 Then CallButtons(31).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29, -(ShaftLeft + WallOffset2), -28, 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e4 = True And ShaftNum = 4 Then CallButtonsDown(34).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 1), (ShaftLeft + WallOffset2), -15 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(34).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 1), (ShaftLeft + WallOffset2), -15 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e3 = True And ShaftNum = 4 Then CallButtonsDown(33).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 1), -(ShaftLeft + WallOffset2), -30 + (15 * 1), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(33).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 1), -(ShaftLeft + WallOffset2), -30 + (15 * 1), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e6 = True And ShaftNum = 4 Then CallButtons(36).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 2), (ShaftLeft + WallOffset2), -17 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e5 = True And ShaftNum = 4 Then CallButtons(35).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 2), -(ShaftLeft + WallOffset2), -28 + (15 * 2), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e8 = True And ShaftNum = 4 Then CallButtonsDown(38).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 3), (ShaftLeft + WallOffset2), -15 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(38).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -16 + (15 * 3), (ShaftLeft + WallOffset2), -15 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    If e7 = True And ShaftNum = 4 Then CallButtonsDown(37).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 3), -(ShaftLeft + WallOffset2), -30 + (15 * 3), 1.5, 8 + (q * FloorHeight) + FloorHeight, 1, 1: CallButtonsUp(37).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -31 + (15 * 3), -(ShaftLeft + WallOffset2), -30 + (15 * 3), 1.5, 9.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e10 = True And ShaftNum = 4 Then CallButtons(40).AddWall GetTex("CallButtonsTex"), (ShaftLeft + WallOffset2), -18 + (15 * 4), (ShaftLeft + WallOffset2), -17 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1
    'If e9 = True And ShaftNum = 4 Then CallButtons(39).AddWall GetTex("CallButtonsTex"), -(ShaftLeft + WallOffset2), -29 + (15 * 4), -(ShaftLeft + WallOffset2), -28 + (15 * 4), 1.5, 8.5 + (q * FloorHeight) + FloorHeight, 1, 1


        If e1 = True And ShaftNum = 1 Then ElevatorDoorL(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 1 Then ElevatorDoorR(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 2 Then ElevatorDoorL(11).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 2 Then ElevatorDoorR(11).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 3 Then ElevatorDoorL(21).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 3 Then ElevatorDoorR(21).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 4 Then ElevatorDoorL(31).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e1 = True And ShaftNum = 4 Then ElevatorDoorR(31).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If FloorID = 1 Then
        If ShaftNum = 1 Then ElevatorDoorL(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, FloorHeight, 1, 1
        If ShaftNum = 1 Then ElevatorDoorR(1).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, FloorHeight, 1, 1
        If ShaftNum = 2 Then ElevatorDoorL(11).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05, -(ShaftLeft + WallOffset), -22.95, 19.6, FloorHeight, 1, 1
        If ShaftNum = 2 Then ElevatorDoorR(11).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05, -(ShaftLeft + WallOffset), -27.05, 19.6, FloorHeight, 1, 1
        End If
        
        If e2 = True And ShaftNum = 1 Then ElevatorDoorL(2).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 1 Then ElevatorDoorR(2).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 2 Then ElevatorDoorL(12).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 2 Then ElevatorDoorR(12).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 3 Then ElevatorDoorL(22).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 3 Then ElevatorDoorR(22).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 4 Then ElevatorDoorL(32).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e2 = True And ShaftNum = 4 Then ElevatorDoorR(32).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If FloorID = 1 Then
        If ShaftNum = 1 Then
        ElevatorDoorL(2).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, FloorHeight, 1, 1
        ElevatorDoorR(2).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, FloorHeight, 1, 1
        End If
        If ShaftNum = 2 Then
        ElevatorDoorL(12).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05, (ShaftLeft + WallOffset), -22.95, 19.6, FloorHeight, 1, 1
        ElevatorDoorR(12).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05, (ShaftLeft + WallOffset), -27.05, 19.6, FloorHeight, 1, 1
        End If
        End If
        
        If e3 = True And ShaftNum = 1 Then ElevatorDoorL(3).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 1), -(ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 1 Then ElevatorDoorR(3).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 1), -(ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 2 Then ElevatorDoorL(13).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 1), -(ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 2 Then ElevatorDoorR(13).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 1), -(ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 3 Then ElevatorDoorL(23).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 1), -(ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 3 Then ElevatorDoorR(23).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 1), -(ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 4 Then ElevatorDoorL(33).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 1), -(ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e3 = True And ShaftNum = 4 Then ElevatorDoorR(33).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 1), -(ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
       
        If e4 = True And ShaftNum = 1 Then ElevatorDoorL(4).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 1), (ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 1 Then ElevatorDoorR(4).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 1), (ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 2 Then ElevatorDoorL(14).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 1), (ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 2 Then ElevatorDoorR(14).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 1), (ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 3 Then ElevatorDoorL(24).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 1), (ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 3 Then ElevatorDoorR(24).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 1), (ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 4 Then ElevatorDoorL(34).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 1), (ShaftLeft + WallOffset), -22.95 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e4 = True And ShaftNum = 4 Then ElevatorDoorR(34).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 1), (ShaftLeft + WallOffset), -27.05 + (15 * 1), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e5 = True And ShaftNum = 1 Then ElevatorDoorL(5).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 2), -(ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 1 Then ElevatorDoorR(5).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 2), -(ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 2 Then ElevatorDoorL(15).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 2), -(ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 2 Then ElevatorDoorR(15).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 2), -(ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 3 Then ElevatorDoorL(25).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 2), -(ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 3 Then ElevatorDoorR(25).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 2), -(ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 4 Then ElevatorDoorL(35).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 2), -(ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e5 = True And ShaftNum = 4 Then ElevatorDoorR(35).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 2), -(ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e6 = True And ShaftNum = 1 Then ElevatorDoorL(6).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 2), (ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 1 Then ElevatorDoorR(6).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 2), (ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 2 Then ElevatorDoorL(16).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 2), (ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 2 Then ElevatorDoorR(16).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 2), (ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 3 Then ElevatorDoorL(26).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 2), (ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 3 Then ElevatorDoorR(26).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 2), (ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 4 Then ElevatorDoorL(36).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 2), (ShaftLeft + WallOffset), -22.95 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e6 = True And ShaftNum = 4 Then ElevatorDoorR(36).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 2), (ShaftLeft + WallOffset), -27.05 + (15 * 2), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e7 = True And ShaftNum = 1 Then ElevatorDoorL(7).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 3), -(ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 1 Then ElevatorDoorR(7).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 3), -(ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 2 Then ElevatorDoorL(17).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 3), -(ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 2 Then ElevatorDoorR(17).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 3), -(ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 3 Then ElevatorDoorL(27).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 3), -(ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 3 Then ElevatorDoorR(27).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 3), -(ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 4 Then ElevatorDoorL(37).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 3), -(ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e7 = True And ShaftNum = 4 Then ElevatorDoorR(37).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 3), -(ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e8 = True And ShaftNum = 1 Then ElevatorDoorL(8).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 3), (ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 1 Then ElevatorDoorR(8).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 3), (ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 2 Then ElevatorDoorL(18).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 3), (ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 2 Then ElevatorDoorR(18).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 3), (ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 3 Then ElevatorDoorL(28).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 3), (ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 3 Then ElevatorDoorR(28).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 3), (ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 4 Then ElevatorDoorL(38).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 3), (ShaftLeft + WallOffset), -22.95 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e8 = True And ShaftNum = 4 Then ElevatorDoorR(38).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 3), (ShaftLeft + WallOffset), -27.05 + (15 * 3), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e9 = True And ShaftNum = 1 Then ElevatorDoorL(9).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 4), -(ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 1 Then ElevatorDoorR(9).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 4), -(ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 2 Then ElevatorDoorL(19).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 4), -(ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 2 Then ElevatorDoorR(19).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 4), -(ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 3 Then ElevatorDoorL(29).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 4), -(ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 3 Then ElevatorDoorR(29).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 4), -(ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 4 Then ElevatorDoorL(39).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -19.05 + (15 * 4), -(ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e9 = True And ShaftNum = 4 Then ElevatorDoorR(39).AddWall GetTex("ElevDoors"), -(ShaftLeft + WallOffset), -23.05 + (15 * 4), -(ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        
        If e10 = True And ShaftNum = 1 Then ElevatorDoorL(10).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 4), (ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 1 Then ElevatorDoorR(10).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 4), (ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 2 Then ElevatorDoorL(20).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 4), (ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 2 Then ElevatorDoorR(20).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 4), (ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 3 Then ElevatorDoorL(30).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 4), (ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 3 Then ElevatorDoorR(30).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 4), (ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 4 Then ElevatorDoorL(40).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -19.05 + (15 * 4), (ShaftLeft + WallOffset), -22.95 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
        If e10 = True And ShaftNum = 4 Then ElevatorDoorR(40).AddWall GetTex("ElevDoors"), (ShaftLeft + WallOffset), -23.05 + (15 * 4), (ShaftLeft + WallOffset), -27.05 + (15 * 4), 19.6, (q * FloorHeight) + FloorHeight, 1, 1
                
End Sub

Sub InitObjectsForFloor(Floor As Integer)

'If Test1 = False Then Exit Sub
'Test1 = False
'Exit Sub
If Floor = 1 Then

''Column 1
'i = 1 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 1)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 45
'Objects(i).SetPosition -75, 5, 45
'Objects(i).Optimize

''Column 2
'i = 2 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 2)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 15
''Objects(i).Optimize

''Column 3
'i = 3 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 3)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -15
''Objects(i).Optimize

''Column 4
'i = 4 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 4)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -45
''Objects(i).Optimize

''Column 5
'i = 5 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 5)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 45
''Objects(i).Optimize

''Column 6
'i = 6 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 6)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 15
''Objects(i).Optimize

''Column 7
'i = 7 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 7)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -15
''Objects(i).Optimize

''Column 8
'i = 8 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 8)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -45
''Objects(i).Optimize

''Door 1
'i = 9 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 9)
'Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, 0, -36.4

''Door 2
'i = 10 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 10)
'Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, FloorHeight, -36.4

End If

If Floor >= 2 And Floor <= 39 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 2 To 27
'Windows 2 to 27
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 1) * 12) + ((j - 1) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 28
i = 28 + (150 * (Floor - 1))
Call Init_Objects(Floor, 28)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 29 To 54
'Windows 29 to 54
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 28) * 12) + ((j - 28) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

'Window 55
i = 55 + (150 * (Floor - 1))
Call Init_Objects(Floor, 55)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 159.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 56 to 79
For j = 56 To 79
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 159.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 55) * 12) + ((j - 55) * 0.1)
'Objects(i).Optimize
Next j

'Window 80
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -159.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 81 to 104
For j = 81 To 104
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -159.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 80) * 12) + ((j - 80) * 0.1)
'Objects(i).Optimize
Next j

''Stairway Door
'i = 105 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 105)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

'Door 01
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -130

'Door 02
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -90

'Door 03
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 130

'Door 04
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 90

'Door 05
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, -130

'Door 06
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, -90

'Door 07
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, 130

'Door 08
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, 90

'Door 09
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -130

'Door 10
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -90

'Door 11
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 130

'Door 12
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 90

'Door 13
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, -130

'Door 14
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, -90

'Door 15
i = 120 + (150 * (Floor - 1))
Call Init_Objects(Floor, 120)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, 130

'Door 16
i = 121 + (150 * (Floor - 1))
Call Init_Objects(Floor, 121)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, 90

'Service Door
i = 122 + (150 * (Floor - 1))
Call Init_Objects(Floor, 122)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61.25, (Floor * FloorHeight) + FloorHeight, -46.25

'Service Door 2
i = 123 + (150 * (Floor - 1))
Call Init_Objects(Floor, 123)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61.25, (Floor * FloorHeight) + FloorHeight, -46.25

'Door 17
i = 124 + (150 * (Floor - 1))
Call Init_Objects(Floor, 124)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 22.5, (Floor * FloorHeight) + FloorHeight, 46.25

'Door 18
i = 125 + (150 * (Floor - 1))
Call Init_Objects(Floor, 125)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -22.5, (Floor * FloorHeight) + FloorHeight, 46.25

End If

If Floor >= 40 And Floor <= 79 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 134.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 134.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -134.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -134.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 129, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 72
'Windows 52 to 72
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 51) * 12) + ((j - 51) * 0.1), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 73
i = 73 + (150 * (Floor - 1))
Call Init_Objects(Floor, 73)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 129, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 74 To 94
'Windows 74 to 94
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 73) * 12) + ((j - 73) * 0.1), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

''Stairway Door
'i = 95 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 95)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

'Door 01
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -130

'Door 02
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -90

'Door 03
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 130

'Door 04
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 90

'Door 05
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, -130

'Door 06
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, -90

'Door 07
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, 130

'Door 08
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, (Floor * FloorHeight) + FloorHeight, 90

'Door 09
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -130

'Door 10
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -90

'Door 11
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 130

'Door 12
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 90

'Door 13
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, -130

'Door 14
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, -90

'Door 15
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, 130

'Door 16
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, (Floor * FloorHeight) + FloorHeight, 90

'Service Door
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61.25, (Floor * FloorHeight) + FloorHeight, -46.25

'Service Door 2
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61.25, (Floor * FloorHeight) + FloorHeight, -46.25

'Door 17
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 22.5, (Floor * FloorHeight) + FloorHeight, 46.25

'Door 18
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -22.5, (Floor * FloorHeight) + FloorHeight, 46.25

End If

If Floor >= 80 And Floor <= 117 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 109.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 109.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -109.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -109.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 103, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 68
'Windows 52 to 68
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 51) * 12) + ((j - 51) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 69
i = 69 + (150 * (Floor - 1))
Call Init_Objects(Floor, 69)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 103, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 70 To 86
'Windows 70 to 86
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 69) * 12) + ((j - 69) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

''Stairway Door
'i = 87 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 87)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

End If

If Floor >= 81 And Floor <= 114 Then

'Door 01
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -118

'Door 02
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -102

'Door 03
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, -60

'Door 04
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 27

'Door 05
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 66

'Door 06
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 102

'Door 07
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * FloorHeight) + FloorHeight, 116

'Door 08
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -118

'Door 09
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -102

'Door 10
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, -60

'Door 11
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 27

'Door 12
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 66

'Door 13
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 102

'Door 14
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * FloorHeight) + FloorHeight, 116

'Door 15
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, 118

'Door 16
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, 102

'Door 17
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, 118

'Door 18
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, 102

'Door 19
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, -118

'Door 20
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, -102

'Door 21
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, -118

'Door 22
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, -102

End If

If Floor >= 118 And Floor <= 134 Then
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 84.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 84.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -84.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -84.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 78, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 64
'Windows 52 to 64
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 51) * 12) + ((j - 51) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 65
i = 65 + (150 * (Floor - 1))
Call Init_Objects(Floor, 65)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 78, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 66 To 78
'Windows 66 to 78
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 65) * 12) + ((j - 65) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

''Stairway Door
'i = 79 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 79)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

End If

If Floor >= 118 And Floor <= 129 Then

'Door 01
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -60, (Floor * FloorHeight) + FloorHeight, -60

'Door 02
i = 81 + (150 * (Floor - 1))
Call Init_Objects(Floor, 81)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 60, (Floor * FloorHeight) + FloorHeight, -60

'Door 03
i = 82 + (150 * (Floor - 1))
Call Init_Objects(Floor, 82)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -60, (Floor * FloorHeight) + FloorHeight, 60

'Door 04
i = 83 + (150 * (Floor - 1))
Call Init_Objects(Floor, 83)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 60, (Floor * FloorHeight) + FloorHeight, 60

'Door 05
i = 84 + (150 * (Floor - 1))
Call Init_Objects(Floor, 84)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, 117

'Door 06
i = 85 + (150 * (Floor - 1))
Call Init_Objects(Floor, 85)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, 101

'Door 07
i = 86 + (150 * (Floor - 1))
Call Init_Objects(Floor, 86)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, 117

'Door 08
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, 101

'Door 09
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, -117

'Door 10
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * FloorHeight) + FloorHeight, -101

'Door 11
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, -117

'Door 12
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * FloorHeight) + FloorHeight, -101

'Service Door
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * FloorHeight) + FloorHeight, -46.3

End If

If Floor = 133 Or Floor = 134 Then
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).LoadXFile App.Path + "\objects\swimpool.x", True
'Objects(i).SetMaterial 0
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).SetRotation 0, -1.58, 0
Objects(i).ScaleMesh 0.2, 0.2, 0.2
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 0 + ((134 * FloorHeight) + FloorHeight), 92
'Objects(i).Optimize
End If

If Floor = 135 Or Floor = 136 Then
Floor = 135
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 53, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 53, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 18, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 62 To 71
'Windows 62 to 71
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 18 - ((j - 65) * 12) + ((j - 65) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

''Stairway Door
'i = 75 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 75)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

'-----------------------------
Floor = 136
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition 59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
Objects(i).SetPosition -59.5, ((Floor * FloorHeight) + FloorHeight) - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 53, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, ((Floor * FloorHeight) + FloorHeight) - 0.5, 149.5
Objects(i).SetPosition 18, ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize

For j = 62 To 71
'Windows 62 to 71
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 18 - ((j - 65) * 12) + ((j - 65) * 0.15), ((Floor * FloorHeight) + FloorHeight) - 0.5, -149.5
'Objects(i).Optimize
Next j

''Stairway Door
'i = 75 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 75)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

End If

If Floor = 137 Then
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\piping.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.04, 0.04, 0.04
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 10 + ((Floor * FloorHeight) + FloorHeight), 60
Objects(i).Optimize

''Stairway Door
'i = 2 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 2)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

End If

If Floor = 138 Then

''Stairway Door
'i = 1 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 1)
'Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, (Floor * FloorHeight) + FloorHeight, -36.4

End If

End Sub
