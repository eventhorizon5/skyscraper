VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ElevatorObject"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'Skyscraper 1.1 Alpha - Elevator Subsystem Class
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

Public Number As Integer 'elevator number
Public ElevatorMesh As TVMesh 'elevator mesh object
Public FloorIndicator As TVMesh 'floor indicator object
Public ElevatorInsDoorL As TVMesh 'left inside door
Public ElevatorInsDoorR As TVMesh 'right inside door
Public Plaque As TVMesh 'plaque
Public CallButtonsUp As TVMesh 'up call button
Public CallButtonsDown As TVMesh 'down call button
Public UpQueue As String 'up call queue
Public DownQueue As String 'down call queue

Option Explicit

Public Sub Initialize(Num As Integer)
Number = Num

'create object meshes
Set ElevatorMesh = Scene.CreateMeshBuilder("Elevator" + Str$(Number))
Set FloorIndicator = Scene.CreateMeshBuilder("FloorIndicator" + Str$(Number))
Set ElevatorInsDoorL = Scene.CreateMeshBuilder("ElevatorInsDoorL" + Str$(Number))
Set ElevatorInsDoorR = Scene.CreateMeshBuilder("ElevatorInsDoorR" + Str$(Number))
Set Plaque = Scene.CreateMeshBuilder("Plaque" + Str$(Number))
Set CallButtonsUp = Scene.CreateMeshBuilder("CallButtonsUp" + Str$(Number))
Set CallButtonsDown = Scene.CreateMeshBuilder("CallButtonsDown" + Str$(Number))

End Sub

Private Sub Class_Terminate()
'Set ElevatorMesh = Nothing
'Set FloorIndicator = Nothing
'Set ElevatorInsDoorL = Nothing
'Set ElevatorInsDoorR = Nothing
'Set Plaque = Nothing
'Set CallButtonsUp = Nothing
'Set CallButtonsDown = Nothing
End Sub

Sub AddRoute(Floor As Integer, Direction As Integer)
'Add call route to elevator routing table, in sorted order

Dim Position As Long
Dim Position2 As Long
Dim TempString As String
Dim TempString2 As String

If Direction = 1 Then
    Position = 1
    If UpQueue = "" Then UpQueue = LTrim(Str$(Floor)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, UpQueue, ",")
    If Position2 = 0 Then Position2 = Len(UpQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(UpQueue, Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(UpQueue, Position, Position2 - 1)) < Floor And Mid$(UpQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt
    If Val(Mid$(UpQueue, Position, Position2 - 1)) > Floor Or Mid$(UpQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(UpQueue, Position - 1)
        TempString2 = Mid$(UpQueue, Position)
        'Insert new number between 2 strings
        UpQueue = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch(Number) = False
        Exit Sub
    End If
SkipIt:
    Loop
End If

If Direction = -1 Then
    Position = 1
    If DownQueue = "" Then DownQueue = LTrim(Str$(Floor)) + ",": If QueuePositionDirection = 0 Then PauseQueueSearch = False: Exit Sub
    Do
    Position2 = InStr(Position, DownQueue, ",")
    If Position2 = 0 Then Position2 = Len(DownQueue) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(DownQueue, Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(DownQueue, Position, Position2 - 1)) < Floor And Mid$(DownQueue, Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt2
    If Val(Mid$(DownQueue, Position, Position2 - 1)) > Floor Or Mid$(DownQueue, Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(DownQueue, Position - 1)
        TempString2 = Mid$(DownQueue, Position)
        'Insert new number between 2 strings
        DownQueue = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch = False
        Exit Sub
    End If
SkipIt2:
    Loop
End If

End Sub

Sub DeleteRoute(Floor As Integer, Direction As Integer)
'Delete call route from elevator routing table

Dim Position As Long
Dim Position2 As Long

If Direction = 1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, UpQueue, LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, UpQueue, ",")
    'join 2 strings, eliminating old number
    UpQueue = Left$(UpQueue, Position - 1) + Mid$(UpQueue, Position2 + 1)
End If

If Direction = -1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, DownQueue, LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, DownQueue, ",")
    'join 2 strings, eliminating old number
    DownQueue = Left$(DownQueue, Position - 1) + Mid$(DownQueue, Position2 + 1)
End If

End Sub

Sub Alarm()

End Sub

