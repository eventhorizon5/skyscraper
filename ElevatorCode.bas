Attribute VB_Name = "ElevatorCode"
'Skycraper 0.97 Beta - Elevator simulation code
'Copyright (C) 2004 Ryan Thoryk
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

Option Explicit
Sub ProcessCallQueue(Number As Integer)
'new calling queue - old one was slow and inefficient

Dim Position As Long
Dim Position2 As Long
Dim Floor As Integer

If PauseQueueSearch(Number) = True Then Exit Sub

If QueuePositionDirection(Number) = 0 Then
    If UpQueue(Number) <> "" Then QueuePositionDirection(Number) = 1
    If DownQueue(Number) <> "" Then QueuePositionDirection(Number) = -1
End If
If UpQueue(Number) = "" And DownQueue(Number) = "" Then
    QueuePositionDirection(Number) = 0
    PauseQueueSearch(Number) = True
    Exit Sub
End If
If QueuePositionDirection(Number) = 1 And UpQueue(Number) = "" Then QueuePositionDirection(Number) = 0
If QueuePositionDirection(Number) = -1 And DownQueue(Number) = "" Then QueuePositionDirection(Number) = 0

If QueuePositionDirection(Number) = 1 Then
    Position = 1
    Do
    Position2 = InStr(Position, UpQueue(Number), ",")
    'Search through queue list and find next valid floor call (direction-wise)
    If Val(Mid$(UpQueue(Number), Position, Position2 - 1)) < Floor Then
        Position = Position2 + 1
        Exit Sub
    Else
        PauseQueueSearch(Number) = True
        Floor = Val(Mid$(UpQueue(Number), Position, Position2 - 1))
        Call DeleteRoute(Floor, Number, 1)
        OpenElevator(Number) = -1
        If InElevator = True Then ElevatorSync(Number) = True
        If Floor <> 0 Then GotoFloor(Number) = Floor
        If Floor = 0 Then GotoFloor(Number) = 0.001
        Exit Sub
    End If
    Loop
End If

If QueuePositionDirection(Number) = -1 Then
    Position = 1
    Do
    Position2 = InStr(Position, DownQueue(Number), ",")
    'Search through queue list and find next valid floor call (direction-wise)
    If Val(Mid$(DownQueue(Number), Position, Position2 - 1)) < Floor Then
        Position = Position2 + 1
        Exit Sub
    Else
        PauseQueueSearch(Number) = True
        Floor = Val(Mid$(DownQueue(Number), Position, Position2 - 1))
        Call DeleteRoute(Floor, Number, -1)
        OpenElevator(Number) = -1
        If InElevator = True Then ElevatorSync(Number) = True
        If Floor <> 0 Then GotoFloor(Number) = Floor
        If Floor = 0 Then GotoFloor(Number) = 0.001
        Exit Sub
    End If
    Loop
End If

End Sub
Sub AddRoute(Floor As Integer, Number As Integer, Direction As Integer)
'Add call route to elevator routing table, in sorted order

Dim Position As Long
Dim Position2 As Long
Dim TempString As String
Dim TempString2 As String

If Direction = 1 Then
    Position = 1
    If UpQueue(Number) = "" Then UpQueue(Number) = LTrim(Str$(Floor)) + ",": If QueuePositionDirection(Number) = 0 Then PauseQueueSearch(Number) = False: Exit Sub
    Do
    Position2 = InStr(Position, UpQueue(Number), ",")
    If Position2 = 0 Then Position2 = Len(UpQueue(Number)) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(UpQueue(Number), Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(UpQueue(Number), Position, Position2 - 1)) < Floor And Mid$(UpQueue(Number), Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt
    If Val(Mid$(UpQueue(Number), Position, Position2 - 1)) > Floor Or Mid$(UpQueue(Number), Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(UpQueue(Number), Position - 1)
        TempString2 = Mid$(UpQueue(Number), Position)
        'Insert new number between 2 strings
        UpQueue(Number) = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch(Number) = False
        Exit Sub
    End If
SkipIt:
    Loop
End If

If Direction = -1 Then
    Position = 1
    If DownQueue(Number) = "" Then DownQueue(Number) = LTrim(Str$(Floor)) + ",": If QueuePositionDirection(Number) = 0 Then PauseQueueSearch(Number) = False: Exit Sub
    Do
    Position2 = InStr(Position, DownQueue(Number), ",")
    If Position2 = 0 Then Position2 = Len(DownQueue(Number)) + 1
    'Find the 2 numbers that the new number should go between
    If Val(Mid$(DownQueue(Number), Position, Position2 - 1)) = Floor Then Exit Sub
    If Val(Mid$(DownQueue(Number), Position, Position2 - 1)) < Floor And Mid$(DownQueue(Number), Position, Position2 - 1) <> "" Then Position = Position2 + 1: GoTo SkipIt2
    If Val(Mid$(DownQueue(Number), Position, Position2 - 1)) > Floor Or Mid$(DownQueue(Number), Position, Position2 - 1) = "" Then
        'break queue into 2 strings
        TempString = Left$(DownQueue(Number), Position - 1)
        TempString2 = Mid$(DownQueue(Number), Position)
        'Insert new number between 2 strings
        DownQueue(Number) = TempString + LTrim(Str$(Floor)) + "," + TempString2
        PauseQueueSearch(Number) = False
        Exit Sub
    End If
SkipIt2:
    Loop
End If

End Sub
Sub DeleteRoute(Floor As Integer, Number As Integer, Direction As Integer)
'Delete call route from elevator routing table

Dim Position As Long
Dim Position2 As Long

If Direction = 1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, UpQueue(Number), LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, UpQueue(Number), ",")
    'join 2 strings, eliminating old number
    UpQueue(Number) = Left$(UpQueue(Number), Position - 1) + Mid$(UpQueue(Number), Position2 + 1)
End If

If Direction = -1 Then
    'break string in 2; 1 before and 1 after number searched for
    Position = InStr(1, DownQueue(Number), LTrim(Str$(Floor)) + ",")
    Position2 = InStr(Position, DownQueue(Number), ",")
    'join 2 strings, eliminating old number
    DownQueue(Number) = Left$(DownQueue(Number), Position - 1) + Mid$(DownQueue(Number), Position2 + 1)
End If

End Sub
Sub StopElevator(Number As Integer)
'If GotoFloor(ElevatorNumber) < ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
    'Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    'GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) - 1
    'FineTune(ElevatorNumber) = True
'End If
'If GotoFloor(ElevatorNumber) > ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
    'Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    'GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) + 1
    'FineTune(ElevatorNumber) = True
'End If

ElevatorDirection(Number) = -ElevatorDirection(Number)

End Sub
Sub Alarm(Number As Integer)

End Sub
Sub CallElevator(Floor As Integer, Section As Integer, Direction As Integer)

'This subroutine is used to autoselect an elevator in the specified section
'The direction value is set by what button the user presses on the call button panel

'Elevator Sections:
'Section Num - Elev Nums
'1 - 1
'2 - 2,3,4
'3 - 5,6,7,8,9,10
'4 - 11
'5 - 12
'6 - 13
'7 - 14
'8 - 15,17,19
'9 - 16,18,20
'10 - 21,23,25,27,29
'11 - 22,24,26,28,30
'12 - 31,33,35,37,39
'13 - 32,34,36,38,40

Dim Number As Integer

'Autoselect closest elevator in section (if applicable)
If Section = 1 Then Number = 1
If Section = 2 Then
If Abs(ElevatorFloor(2) - Floor) <= Abs(ElevatorFloor(3) - Floor) And Abs(ElevatorFloor(2) - Floor) <= Abs(ElevatorFloor(4) - Floor) Then
    If UpQueue(2) = "" And DownQueue(2) = "" Then Number = 2
    If ElevatorFloor(2) > Floor And QueuePositionDirection(2) = -1 And Direction = -1 Then Number = 2
    If ElevatorFloor(2) < Floor And QueuePositionDirection(2) = 1 And Direction = 1 Then Number = 2
End If
If Abs(ElevatorFloor(3) - Floor) <= Abs(ElevatorFloor(4) - Floor) And Abs(ElevatorFloor(3) - Floor) <= Abs(ElevatorFloor(2) - Floor) Then
    If UpQueue(3) = "" And DownQueue(3) = "" Then Number = 3
    If ElevatorFloor(3) > Floor And QueuePositionDirection(3) = -1 And Direction = -1 Then Number = 3
    If ElevatorFloor(3) < Floor And QueuePositionDirection(3) = 1 And Direction = 1 Then Number = 3
End If
If Abs(ElevatorFloor(4) - Floor) <= Abs(ElevatorFloor(2) - Floor) And Abs(ElevatorFloor(4) - Floor) <= Abs(ElevatorFloor(3) - Floor) Then
    If UpQueue(4) = "" And DownQueue(4) = "" Then Number = 4
    If ElevatorFloor(4) > Floor And QueuePositionDirection(4) = -1 And Direction = -1 Then Number = 4
    If ElevatorFloor(4) < Floor And QueuePositionDirection(4) = 1 And Direction = 1 Then Number = 4
End If
End If
If Section = 3 Then
If Abs(ElevatorFloor(5) - Floor) <= Abs(ElevatorFloor(6) - Floor) And Abs(ElevatorFloor(5) - Floor) <= Abs(ElevatorFloor(7) - Floor) And Abs(ElevatorFloor(5) - Floor) <= Abs(ElevatorFloor(8) - Floor) And Abs(ElevatorFloor(5) - Floor) <= Abs(ElevatorFloor(9) - Floor) And Abs(ElevatorFloor(5) - Floor) <= Abs(ElevatorFloor(10) - Floor) Then
    If UpQueue(5) = "" And DownQueue(5) = "" Then Number = 5
    If ElevatorFloor(5) > Floor And QueuePositionDirection(5) = -1 And Direction = -1 Then Number = 5
    If ElevatorFloor(5) < Floor And QueuePositionDirection(5) = 1 And Direction = 1 Then Number = 5
End If
If Abs(ElevatorFloor(6) - Floor) <= Abs(ElevatorFloor(7) - Floor) And Abs(ElevatorFloor(6) - Floor) <= Abs(ElevatorFloor(8) - Floor) And Abs(ElevatorFloor(6) - Floor) <= Abs(ElevatorFloor(9) - Floor) And Abs(ElevatorFloor(6) - Floor) <= Abs(ElevatorFloor(10) - Floor) And Abs(ElevatorFloor(6) - Floor) <= Abs(ElevatorFloor(5) - Floor) Then
    If UpQueue(6) = "" And DownQueue(6) = "" Then Number = 6
    If ElevatorFloor(6) > Floor And QueuePositionDirection(6) = -1 And Direction = -1 Then Number = 6
    If ElevatorFloor(6) < Floor And QueuePositionDirection(6) = 1 And Direction = 1 Then Number = 6
End If
If Abs(ElevatorFloor(7) - Floor) <= Abs(ElevatorFloor(8) - Floor) And Abs(ElevatorFloor(7) - Floor) <= Abs(ElevatorFloor(9) - Floor) And Abs(ElevatorFloor(7) - Floor) <= Abs(ElevatorFloor(10) - Floor) And Abs(ElevatorFloor(7) - Floor) <= Abs(ElevatorFloor(5) - Floor) And Abs(ElevatorFloor(7) - Floor) <= Abs(ElevatorFloor(6) - Floor) Then
    If UpQueue(7) = "" And DownQueue(7) = "" Then Number = 7
    If ElevatorFloor(7) > Floor And QueuePositionDirection(7) = -1 And Direction = -1 Then Number = 7
    If ElevatorFloor(7) < Floor And QueuePositionDirection(7) = 1 And Direction = 1 Then Number = 7
End If
If Abs(ElevatorFloor(8) - Floor) <= Abs(ElevatorFloor(9) - Floor) And Abs(ElevatorFloor(8) - Floor) <= Abs(ElevatorFloor(10) - Floor) And Abs(ElevatorFloor(8) - Floor) <= Abs(ElevatorFloor(5) - Floor) And Abs(ElevatorFloor(8) - Floor) <= Abs(ElevatorFloor(6) - Floor) And Abs(ElevatorFloor(8) - Floor) <= Abs(ElevatorFloor(7) - Floor) Then
    If UpQueue(8) = "" And DownQueue(8) = "" Then Number = 8
    If ElevatorFloor(8) > Floor And QueuePositionDirection(8) = -1 And Direction = -1 Then Number = 8
    If ElevatorFloor(8) < Floor And QueuePositionDirection(8) = 1 And Direction = 1 Then Number = 8
End If
If Abs(ElevatorFloor(9) - Floor) <= Abs(ElevatorFloor(10) - Floor) And Abs(ElevatorFloor(9) - Floor) <= Abs(ElevatorFloor(5) - Floor) And Abs(ElevatorFloor(9) - Floor) <= Abs(ElevatorFloor(6) - Floor) And Abs(ElevatorFloor(9) - Floor) <= Abs(ElevatorFloor(7) - Floor) And Abs(ElevatorFloor(9) - Floor) <= Abs(ElevatorFloor(8) - Floor) Then
    If UpQueue(9) = "" And DownQueue(9) = "" Then Number = 9
    If ElevatorFloor(9) > Floor And QueuePositionDirection(9) = -1 And Direction = -1 Then Number = 9
    If ElevatorFloor(9) < Floor And QueuePositionDirection(9) = 1 And Direction = 1 Then Number = 9
End If
If Abs(ElevatorFloor(10) - Floor) <= Abs(ElevatorFloor(5) - Floor) And Abs(ElevatorFloor(10) - Floor) <= Abs(ElevatorFloor(6) - Floor) And Abs(ElevatorFloor(10) - Floor) <= Abs(ElevatorFloor(7) - Floor) And Abs(ElevatorFloor(10) - Floor) <= Abs(ElevatorFloor(8) - Floor) And Abs(ElevatorFloor(10) - Floor) <= Abs(ElevatorFloor(9) - Floor) Then
    If UpQueue(10) = "" And DownQueue(10) = "" Then Number = 10
    If ElevatorFloor(10) > Floor And QueuePositionDirection(10) = -1 And Direction = -1 Then Number = 10
    If ElevatorFloor(10) < Floor And QueuePositionDirection(10) = 1 And Direction = 1 Then Number = 10
End If
End If
If Section = 4 Then Number = 11
If Section = 5 Then Number = 12
If Section = 6 Then Number = 13
If Section = 7 Then Number = 14
If Section = 8 Then
If Abs(ElevatorFloor(15) - Floor) <= Abs(ElevatorFloor(17) - Floor) And Abs(ElevatorFloor(15) - Floor) <= Abs(ElevatorFloor(19) - Floor) Then
    If UpQueue(15) = "" And DownQueue(15) = "" Then Number = 15
    If ElevatorFloor(15) > Floor And QueuePositionDirection(15) = -1 And Direction = -1 Then Number = 15
    If ElevatorFloor(15) < Floor And QueuePositionDirection(15) = 1 And Direction = 1 Then Number = 15
End If
If Abs(ElevatorFloor(17) - Floor) <= Abs(ElevatorFloor(19) - Floor) And Abs(ElevatorFloor(17) - Floor) <= Abs(ElevatorFloor(15) - Floor) Then
    If UpQueue(17) = "" And DownQueue(17) = "" Then Number = 17
    If ElevatorFloor(17) > Floor And QueuePositionDirection(17) = -1 And Direction = -1 Then Number = 17
    If ElevatorFloor(17) < Floor And QueuePositionDirection(17) = 1 And Direction = 1 Then Number = 17
End If
If Abs(ElevatorFloor(19) - Floor) <= Abs(ElevatorFloor(15) - Floor) And Abs(ElevatorFloor(19) - Floor) <= Abs(ElevatorFloor(17) - Floor) Then
    If UpQueue(19) = "" And DownQueue(19) = "" Then Number = 19
    If ElevatorFloor(19) > Floor And QueuePositionDirection(19) = -1 And Direction = -1 Then Number = 19
    If ElevatorFloor(19) < Floor And QueuePositionDirection(19) = 1 And Direction = 1 Then Number = 19
End If
End If
If Section = 9 Then
If Abs(ElevatorFloor(16) - Floor) <= Abs(ElevatorFloor(18) - Floor) And Abs(ElevatorFloor(16) - Floor) <= Abs(ElevatorFloor(20) - Floor) Then
    If UpQueue(16) = "" And DownQueue(16) = "" Then Number = 16
    If ElevatorFloor(16) > Floor And QueuePositionDirection(16) = -1 And Direction = -1 Then Number = 16
    If ElevatorFloor(16) < Floor And QueuePositionDirection(16) = 1 And Direction = 1 Then Number = 16
End If
If Abs(ElevatorFloor(18) - Floor) <= Abs(ElevatorFloor(20) - Floor) And Abs(ElevatorFloor(18) - Floor) <= Abs(ElevatorFloor(16) - Floor) Then
    If UpQueue(18) = "" And DownQueue(18) = "" Then Number = 18
    If ElevatorFloor(18) > Floor And QueuePositionDirection(18) = -1 And Direction = -1 Then Number = 18
    If ElevatorFloor(18) < Floor And QueuePositionDirection(18) = 1 And Direction = 1 Then Number = 18
End If
If Abs(ElevatorFloor(20) - Floor) <= Abs(ElevatorFloor(16) - Floor) And Abs(ElevatorFloor(20) - Floor) <= Abs(ElevatorFloor(18) - Floor) Then
    If UpQueue(20) = "" And DownQueue(20) = "" Then Number = 20
    If ElevatorFloor(20) > Floor And QueuePositionDirection(20) = -1 And Direction = -1 Then Number = 20
    If ElevatorFloor(20) < Floor And QueuePositionDirection(20) = 1 And Direction = 1 Then Number = 20
End If
End If
If Section = 10 Then
If Abs(ElevatorFloor(21) - Floor) <= Abs(ElevatorFloor(23) - Floor) And Abs(ElevatorFloor(21) - Floor) <= Abs(ElevatorFloor(25) - Floor) And Abs(ElevatorFloor(21) - Floor) <= Abs(ElevatorFloor(27) - Floor) And Abs(ElevatorFloor(21) - Floor) <= Abs(ElevatorFloor(29) - Floor) Then
    If UpQueue(21) = "" And DownQueue(21) = "" Then Number = 21
    If ElevatorFloor(21) > Floor And QueuePositionDirection(21) = -1 And Direction = -1 Then Number = 21
    If ElevatorFloor(21) < Floor And QueuePositionDirection(21) = 1 And Direction = 1 Then Number = 21
End If
If Abs(ElevatorFloor(23) - Floor) <= Abs(ElevatorFloor(25) - Floor) And Abs(ElevatorFloor(23) - Floor) <= Abs(ElevatorFloor(27) - Floor) And Abs(ElevatorFloor(23) - Floor) <= Abs(ElevatorFloor(29) - Floor) And Abs(ElevatorFloor(23) - Floor) <= Abs(ElevatorFloor(21) - Floor) Then
    If UpQueue(23) = "" And DownQueue(23) = "" Then Number = 23
    If ElevatorFloor(23) > Floor And QueuePositionDirection(23) = -1 And Direction = -1 Then Number = 23
    If ElevatorFloor(23) < Floor And QueuePositionDirection(23) = 1 And Direction = 1 Then Number = 23
End If
If Abs(ElevatorFloor(25) - Floor) <= Abs(ElevatorFloor(27) - Floor) And Abs(ElevatorFloor(25) - Floor) <= Abs(ElevatorFloor(29) - Floor) And Abs(ElevatorFloor(25) - Floor) <= Abs(ElevatorFloor(21) - Floor) And Abs(ElevatorFloor(25) - Floor) <= Abs(ElevatorFloor(23) - Floor) Then
    If UpQueue(25) = "" And DownQueue(25) = "" Then Number = 25
    If ElevatorFloor(25) > Floor And QueuePositionDirection(25) = -1 And Direction = -1 Then Number = 25
    If ElevatorFloor(25) < Floor And QueuePositionDirection(25) = 1 And Direction = 1 Then Number = 25
End If
If Abs(ElevatorFloor(27) - Floor) <= Abs(ElevatorFloor(29) - Floor) And Abs(ElevatorFloor(27) - Floor) <= Abs(ElevatorFloor(21) - Floor) And Abs(ElevatorFloor(27) - Floor) <= Abs(ElevatorFloor(23) - Floor) And Abs(ElevatorFloor(27) - Floor) <= Abs(ElevatorFloor(25) - Floor) Then
    If UpQueue(27) = "" And DownQueue(27) = "" Then Number = 27
    If ElevatorFloor(27) > Floor And QueuePositionDirection(27) = -1 And Direction = -1 Then Number = 27
    If ElevatorFloor(27) < Floor And QueuePositionDirection(27) = 1 And Direction = 1 Then Number = 27
End If
If Abs(ElevatorFloor(29) - Floor) <= Abs(ElevatorFloor(21) - Floor) And Abs(ElevatorFloor(29) - Floor) <= Abs(ElevatorFloor(23) - Floor) And Abs(ElevatorFloor(29) - Floor) <= Abs(ElevatorFloor(25) - Floor) And Abs(ElevatorFloor(29) - Floor) <= Abs(ElevatorFloor(27) - Floor) Then
    If UpQueue(29) = "" And DownQueue(29) = "" Then Number = 29
    If ElevatorFloor(29) > Floor And QueuePositionDirection(29) = -1 And Direction = -1 Then Number = 29
    If ElevatorFloor(29) < Floor And QueuePositionDirection(29) = 1 And Direction = 1 Then Number = 29
End If
End If
If Section = 11 Then
If Abs(ElevatorFloor(22) - Floor) <= Abs(ElevatorFloor(24) - Floor) And Abs(ElevatorFloor(22) - Floor) <= Abs(ElevatorFloor(26) - Floor) And Abs(ElevatorFloor(22) - Floor) <= Abs(ElevatorFloor(28) - Floor) And Abs(ElevatorFloor(22) - Floor) <= Abs(ElevatorFloor(30) - Floor) Then
    If UpQueue(22) = "" And DownQueue(22) = "" Then Number = 22
    If ElevatorFloor(22) > Floor And QueuePositionDirection(22) = -1 And Direction = -1 Then Number = 22
    If ElevatorFloor(22) < Floor And QueuePositionDirection(22) = 1 And Direction = 1 Then Number = 22
End If
If Abs(ElevatorFloor(24) - Floor) <= Abs(ElevatorFloor(26) - Floor) And Abs(ElevatorFloor(24) - Floor) <= Abs(ElevatorFloor(28) - Floor) And Abs(ElevatorFloor(24) - Floor) <= Abs(ElevatorFloor(30) - Floor) And Abs(ElevatorFloor(24) - Floor) <= Abs(ElevatorFloor(22) - Floor) Then
    If UpQueue(24) = "" And DownQueue(24) = "" Then Number = 24
    If ElevatorFloor(24) > Floor And QueuePositionDirection(24) = -1 And Direction = -1 Then Number = 24
    If ElevatorFloor(24) < Floor And QueuePositionDirection(24) = 1 And Direction = 1 Then Number = 24
End If
If Abs(ElevatorFloor(26) - Floor) <= Abs(ElevatorFloor(28) - Floor) And Abs(ElevatorFloor(26) - Floor) <= Abs(ElevatorFloor(30) - Floor) And Abs(ElevatorFloor(26) - Floor) <= Abs(ElevatorFloor(22) - Floor) And Abs(ElevatorFloor(26) - Floor) <= Abs(ElevatorFloor(24) - Floor) Then
    If UpQueue(26) = "" And DownQueue(26) = "" Then Number = 26
    If ElevatorFloor(26) > Floor And QueuePositionDirection(26) = -1 And Direction = -1 Then Number = 26
    If ElevatorFloor(26) < Floor And QueuePositionDirection(26) = 1 And Direction = 1 Then Number = 26
End If
If Abs(ElevatorFloor(28) - Floor) <= Abs(ElevatorFloor(30) - Floor) And Abs(ElevatorFloor(28) - Floor) <= Abs(ElevatorFloor(22) - Floor) And Abs(ElevatorFloor(28) - Floor) <= Abs(ElevatorFloor(24) - Floor) And Abs(ElevatorFloor(28) - Floor) <= Abs(ElevatorFloor(26) - Floor) Then
    If UpQueue(28) = "" And DownQueue(28) = "" Then Number = 28
    If ElevatorFloor(28) > Floor And QueuePositionDirection(28) = -1 And Direction = -1 Then Number = 28
    If ElevatorFloor(28) < Floor And QueuePositionDirection(28) = 1 And Direction = 1 Then Number = 28
End If
If Abs(ElevatorFloor(30) - Floor) <= Abs(ElevatorFloor(22) - Floor) And Abs(ElevatorFloor(30) - Floor) <= Abs(ElevatorFloor(24) - Floor) And Abs(ElevatorFloor(30) - Floor) <= Abs(ElevatorFloor(26) - Floor) And Abs(ElevatorFloor(30) - Floor) <= Abs(ElevatorFloor(28) - Floor) Then
    If UpQueue(30) = "" And DownQueue(30) = "" Then Number = 30
    If ElevatorFloor(30) > Floor And QueuePositionDirection(30) = -1 And Direction = -1 Then Number = 30
    If ElevatorFloor(30) < Floor And QueuePositionDirection(30) = 1 And Direction = 1 Then Number = 30
End If
End If
If Section = 12 Then
If Abs(ElevatorFloor(31) - Floor) <= Abs(ElevatorFloor(33) - Floor) And Abs(ElevatorFloor(31) - Floor) <= Abs(ElevatorFloor(35) - Floor) And Abs(ElevatorFloor(31) - Floor) <= Abs(ElevatorFloor(37) - Floor) And Abs(ElevatorFloor(31) - Floor) <= Abs(ElevatorFloor(39) - Floor) Then
    If UpQueue(31) = "" And DownQueue(31) = "" Then Number = 31
    If ElevatorFloor(31) > Floor And QueuePositionDirection(31) = -1 And Direction = -1 Then Number = 31
    If ElevatorFloor(31) < Floor And QueuePositionDirection(31) = 1 And Direction = 1 Then Number = 31
End If
If Abs(ElevatorFloor(33) - Floor) <= Abs(ElevatorFloor(35) - Floor) And Abs(ElevatorFloor(33) - Floor) <= Abs(ElevatorFloor(37) - Floor) And Abs(ElevatorFloor(33) - Floor) <= Abs(ElevatorFloor(39) - Floor) And Abs(ElevatorFloor(33) - Floor) <= Abs(ElevatorFloor(31) - Floor) Then
    If UpQueue(33) = "" And DownQueue(33) = "" Then Number = 33
    If ElevatorFloor(33) > Floor And QueuePositionDirection(33) = -1 And Direction = -1 Then Number = 33
    If ElevatorFloor(33) < Floor And QueuePositionDirection(33) = 1 And Direction = 1 Then Number = 33
End If
If Abs(ElevatorFloor(35) - Floor) <= Abs(ElevatorFloor(37) - Floor) And Abs(ElevatorFloor(35) - Floor) <= Abs(ElevatorFloor(39) - Floor) And Abs(ElevatorFloor(35) - Floor) <= Abs(ElevatorFloor(31) - Floor) And Abs(ElevatorFloor(35) - Floor) <= Abs(ElevatorFloor(33) - Floor) Then
    If UpQueue(35) = "" And DownQueue(35) = "" Then Number = 35
    If ElevatorFloor(35) > Floor And QueuePositionDirection(35) = -1 And Direction = -1 Then Number = 35
    If ElevatorFloor(35) < Floor And QueuePositionDirection(35) = 1 And Direction = 1 Then Number = 35
End If
If Abs(ElevatorFloor(37) - Floor) <= Abs(ElevatorFloor(39) - Floor) And Abs(ElevatorFloor(37) - Floor) <= Abs(ElevatorFloor(31) - Floor) And Abs(ElevatorFloor(37) - Floor) <= Abs(ElevatorFloor(33) - Floor) And Abs(ElevatorFloor(37) - Floor) <= Abs(ElevatorFloor(35) - Floor) Then
    If UpQueue(37) = "" And DownQueue(37) = "" Then Number = 37
    If ElevatorFloor(37) > Floor And QueuePositionDirection(37) = -1 And Direction = -1 Then Number = 37
    If ElevatorFloor(37) < Floor And QueuePositionDirection(37) = 1 And Direction = 1 Then Number = 37
End If
If Abs(ElevatorFloor(39) - Floor) <= Abs(ElevatorFloor(31) - Floor) And Abs(ElevatorFloor(39) - Floor) <= Abs(ElevatorFloor(33) - Floor) And Abs(ElevatorFloor(39) - Floor) <= Abs(ElevatorFloor(35) - Floor) And Abs(ElevatorFloor(39) - Floor) <= Abs(ElevatorFloor(37) - Floor) Then
    If UpQueue(39) = "" And DownQueue(39) = "" Then Number = 39
    If ElevatorFloor(39) > Floor And QueuePositionDirection(39) = -1 And Direction = -1 Then Number = 39
    If ElevatorFloor(39) < Floor And QueuePositionDirection(39) = 1 And Direction = 1 Then Number = 39
End If
End If
If Section = 13 Then
If Abs(ElevatorFloor(32) - Floor) <= Abs(ElevatorFloor(34) - Floor) And Abs(ElevatorFloor(32) - Floor) <= Abs(ElevatorFloor(36) - Floor) And Abs(ElevatorFloor(32) - Floor) <= Abs(ElevatorFloor(38) - Floor) And Abs(ElevatorFloor(32) - Floor) <= Abs(ElevatorFloor(40) - Floor) Then
    If UpQueue(32) = "" And DownQueue(32) = "" Then Number = 32
    If ElevatorFloor(32) > Floor And QueuePositionDirection(32) = -1 And Direction = -1 Then Number = 32
    If ElevatorFloor(32) < Floor And QueuePositionDirection(32) = 1 And Direction = 1 Then Number = 32
End If
If Abs(ElevatorFloor(34) - Floor) <= Abs(ElevatorFloor(36) - Floor) And Abs(ElevatorFloor(34) - Floor) <= Abs(ElevatorFloor(38) - Floor) And Abs(ElevatorFloor(34) - Floor) <= Abs(ElevatorFloor(40) - Floor) And Abs(ElevatorFloor(34) - Floor) <= Abs(ElevatorFloor(32) - Floor) Then
    If UpQueue(34) = "" And DownQueue(34) = "" Then Number = 34
    If ElevatorFloor(34) > Floor And QueuePositionDirection(34) = -1 And Direction = -1 Then Number = 34
    If ElevatorFloor(34) < Floor And QueuePositionDirection(34) = 1 And Direction = 1 Then Number = 34
End If
If Abs(ElevatorFloor(36) - Floor) <= Abs(ElevatorFloor(38) - Floor) And Abs(ElevatorFloor(36) - Floor) <= Abs(ElevatorFloor(40) - Floor) And Abs(ElevatorFloor(36) - Floor) <= Abs(ElevatorFloor(32) - Floor) And Abs(ElevatorFloor(36) - Floor) <= Abs(ElevatorFloor(34) - Floor) Then
    If UpQueue(36) = "" And DownQueue(36) = "" Then Number = 36
    If ElevatorFloor(36) > Floor And QueuePositionDirection(36) = -1 And Direction = -1 Then Number = 36
    If ElevatorFloor(36) < Floor And QueuePositionDirection(36) = 1 And Direction = 1 Then Number = 36
End If
If Abs(ElevatorFloor(38) - Floor) <= Abs(ElevatorFloor(40) - Floor) And Abs(ElevatorFloor(38) - Floor) <= Abs(ElevatorFloor(32) - Floor) And Abs(ElevatorFloor(38) - Floor) <= Abs(ElevatorFloor(34) - Floor) And Abs(ElevatorFloor(38) - Floor) <= Abs(ElevatorFloor(36) - Floor) Then
    If UpQueue(38) = "" And DownQueue(38) = "" Then Number = 38
    If ElevatorFloor(38) > Floor And QueuePositionDirection(38) = -1 And Direction = -1 Then Number = 38
    If ElevatorFloor(38) < Floor And QueuePositionDirection(38) = 1 And Direction = 1 Then Number = 38
End If
If Abs(ElevatorFloor(40) - Floor) <= Abs(ElevatorFloor(32) - Floor) And Abs(ElevatorFloor(40) - Floor) <= Abs(ElevatorFloor(34) - Floor) And Abs(ElevatorFloor(40) - Floor) <= Abs(ElevatorFloor(36) - Floor) And Abs(ElevatorFloor(40) - Floor) <= Abs(ElevatorFloor(38) - Floor) Then
    If UpQueue(40) = "" And DownQueue(40) = "" Then Number = 40
    If ElevatorFloor(40) > Floor And QueuePositionDirection(40) = -1 And Direction = -1 Then Number = 40
    If ElevatorFloor(40) < Floor And QueuePositionDirection(40) = 1 And Direction = 1 Then Number = 40
End If
End If
 
 If ElevatorFloor(Number) <> Floor Then
 ElevatorSync(Number) = False
 Call AddRoute(Floor, Number, Direction)
 Exit Sub
 End If
 
 OpenElevator(Number) = 1
End Sub
Sub DrawElevatorButtons(Number As Integer)
ButtonsEnabled = True

'New button handling code
For i54 = -11 To 144
Buttons(i54).ResetMesh
Buttons(i54).SetPosition 0, 0, 0
Buttons(i54).SetRotation 0, 0, 0
Next i54

'General Buttons
Call ElevatorButton(139, "Open", Number, 56)
Call ElevatorButton(140, "Close", Number, 57)
Call ElevatorButton(141, "Cancel", Number, 58)
Call ElevatorButton(142, "Stop", Number, 59)
Call ElevatorButton(143, "Alarm", Number, 60)
    
'Elevator 1
If Number = 1 Then

    Call ElevatorButton(134, "134", Number, 6)
    Call ElevatorButton(135, "135", Number, 7)
    Call ElevatorButton(136, "136", Number, 8)
    Call ElevatorButton(137, "137", Number, 9)
    Call ElevatorButton(138, "R", Number, 10)
    
    Call ElevatorButton(129, "129", Number, 11)
    Call ElevatorButton(130, "130", Number, 12)
    Call ElevatorButton(131, "131", Number, 13)
    Call ElevatorButton(132, "132", Number, 14)
    Call ElevatorButton(133, "133", Number, 15)
    
    Call ElevatorButton(124, "124", Number, 16)
    Call ElevatorButton(125, "125", Number, 17)
    Call ElevatorButton(126, "126", Number, 18)
    Call ElevatorButton(127, "127", Number, 19)
    Call ElevatorButton(128, "128", Number, 20)
    
    Call ElevatorButton(119, "119", Number, 21)
    Call ElevatorButton(120, "120", Number, 22)
    Call ElevatorButton(121, "121", Number, 23)
    Call ElevatorButton(122, "122", Number, 24)
    Call ElevatorButton(123, "123", Number, 25)
    
    Call ElevatorButton(114, "114", Number, 26)
    Call ElevatorButton(115, "115", Number, 27)
    Call ElevatorButton(116, "116", Number, 28)
    Call ElevatorButton(117, "117", Number, 29)
    Call ElevatorButton(118, "118", Number, 30)
    
    Call ElevatorButton(109, "109", Number, 31)
    Call ElevatorButton(110, "110", Number, 32)
    Call ElevatorButton(111, "111", Number, 33)
    Call ElevatorButton(112, "112", Number, 34)
    Call ElevatorButton(113, "113", Number, 35)
    
    Call ElevatorButton(104, "104", Number, 36)
    Call ElevatorButton(105, "105", Number, 37)
    Call ElevatorButton(106, "106", Number, 38)
    Call ElevatorButton(107, "107", Number, 39)
    Call ElevatorButton(108, "108", Number, 40)
    
    Call ElevatorButton(79, "79", Number, 41)
    Call ElevatorButton(80, "80", Number, 42)
    Call ElevatorButton(101, "101", Number, 43)
    Call ElevatorButton(102, "102", Number, 44)
    Call ElevatorButton(103, "103", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 46)
    Call ElevatorButton(1, "M", Number, 47)
    Call ElevatorButton(2, "2", Number, 48)
    Call ElevatorButton(39, "39", Number, 49)
    Call ElevatorButton(40, "40", Number, 50)
    
End If

'Elevator 2
If Number >= 2 And Number <= 4 Then

    Call ElevatorButton(136, "136", Number, 13)
    Call ElevatorButton(135, "135", Number, 18)
    Call ElevatorButton(134, "134", Number, 23)
    Call ElevatorButton(132, "132", Number, 28)
    Call ElevatorButton(80, "80", Number, 33)
    Call ElevatorButton(1, "M", Number, 38)
    Call ElevatorButton(0, "L", Number, 43)

End If

If Number >= 5 And Number <= 10 Then

    Call ElevatorButton(127, "127", Number, 7)
    Call ElevatorButton(128, "128", Number, 8)
    Call ElevatorButton(129, "129", Number, 9)
    
    Call ElevatorButton(124, "124", Number, 12)
    Call ElevatorButton(125, "125", Number, 13)
    Call ElevatorButton(126, "126", Number, 14)
    
    Call ElevatorButton(121, "121", Number, 17)
    Call ElevatorButton(122, "122", Number, 18)
    Call ElevatorButton(123, "123", Number, 19)
    
    Call ElevatorButton(118, "118", Number, 22)
    Call ElevatorButton(119, "119", Number, 23)
    Call ElevatorButton(120, "120", Number, 24)
    
    Call ElevatorButton(112, "112", Number, 27)
    Call ElevatorButton(113, "113", Number, 28)
    Call ElevatorButton(114, "114", Number, 29)
    
    Call ElevatorButton(109, "109", Number, 32)
    Call ElevatorButton(110, "110", Number, 33)
    Call ElevatorButton(111, "111", Number, 34)
    
    Call ElevatorButton(106, "106", Number, 37)
    Call ElevatorButton(107, "107", Number, 38)
    Call ElevatorButton(108, "108", Number, 39)
    
    Call ElevatorButton(103, "103", Number, 42)
    Call ElevatorButton(104, "104", Number, 43)
    Call ElevatorButton(105, "105", Number, 44)
    
    Call ElevatorButton(100, "100", Number, 47)
    Call ElevatorButton(101, "101", Number, 48)
    Call ElevatorButton(102, "102", Number, 49)
    
    Call ElevatorButton(80, "80", Number, 53)
    
End If

    
If Number = 11 Then

    Call ElevatorButton(51, "51", Number, 1)
    Call ElevatorButton(80, "80", Number, 2)
    Call ElevatorButton(115, "115", Number, 3)
    Call ElevatorButton(116, "116", Number, 4)
    
    Call ElevatorButton(46, "46", Number, 6)
    Call ElevatorButton(47, "47", Number, 7)
    Call ElevatorButton(48, "48", Number, 8)
    Call ElevatorButton(49, "49", Number, 9)
    Call ElevatorButton(50, "50", Number, 10)
    
    Call ElevatorButton(41, "41", Number, 11)
    Call ElevatorButton(42, "42", Number, 12)
    Call ElevatorButton(43, "43", Number, 13)
    Call ElevatorButton(44, "44", Number, 14)
    Call ElevatorButton(45, "45", Number, 15)
    
    Call ElevatorButton(36, "36", Number, 16)
    Call ElevatorButton(37, "37", Number, 17)
    Call ElevatorButton(38, "38", Number, 18)
    Call ElevatorButton(39, "39", Number, 19)
    Call ElevatorButton(40, "40", Number, 20)
    
    Call ElevatorButton(31, "31", Number, 21)
    Call ElevatorButton(32, "32", Number, 22)
    Call ElevatorButton(33, "33", Number, 23)
    Call ElevatorButton(34, "34", Number, 24)
    Call ElevatorButton(35, "35", Number, 25)
    
    Call ElevatorButton(26, "26", Number, 26)
    Call ElevatorButton(27, "27", Number, 27)
    Call ElevatorButton(28, "28", Number, 28)
    Call ElevatorButton(29, "29", Number, 29)
    Call ElevatorButton(30, "30", Number, 30)
    
    Call ElevatorButton(21, "21", Number, 31)
    Call ElevatorButton(22, "22", Number, 32)
    Call ElevatorButton(23, "23", Number, 33)
    Call ElevatorButton(24, "24", Number, 34)
    Call ElevatorButton(25, "25", Number, 35)
    
    Call ElevatorButton(16, "16", Number, 36)
    Call ElevatorButton(17, "17", Number, 37)
    Call ElevatorButton(18, "18", Number, 38)
    Call ElevatorButton(19, "19", Number, 39)
    Call ElevatorButton(20, "20", Number, 40)
    
    Call ElevatorButton(11, "11", Number, 41)
    Call ElevatorButton(12, "12", Number, 42)
    Call ElevatorButton(13, "13", Number, 43)
    Call ElevatorButton(14, "14", Number, 44)
    Call ElevatorButton(15, "15", Number, 45)
    
    Call ElevatorButton(6, "6", Number, 46)
    Call ElevatorButton(7, "7", Number, 47)
    Call ElevatorButton(8, "8", Number, 48)
    Call ElevatorButton(9, "9", Number, 49)
    Call ElevatorButton(10, "10", Number, 50)
    
    Call ElevatorButton(0, "L", Number, 51)
    Call ElevatorButton(2, "2", Number, 52)
    Call ElevatorButton(3, "3", Number, 53)
    Call ElevatorButton(4, "4", Number, 54)
    Call ElevatorButton(5, "5", Number, 55)
    
End If

If Number = 12 Then

    Call ElevatorButton(100, "100", Number, 1)
    Call ElevatorButton(101, "101", Number, 2)
    Call ElevatorButton(115, "115", Number, 3)
    Call ElevatorButton(116, "116", Number, 4)
    
    Call ElevatorButton(95, "95", Number, 6)
    Call ElevatorButton(96, "96", Number, 7)
    Call ElevatorButton(97, "97", Number, 8)
    Call ElevatorButton(98, "98", Number, 9)
    Call ElevatorButton(99, "99", Number, 10)
    
    Call ElevatorButton(90, "90", Number, 11)
    Call ElevatorButton(91, "91", Number, 12)
    Call ElevatorButton(92, "92", Number, 13)
    Call ElevatorButton(93, "93", Number, 14)
    Call ElevatorButton(94, "94", Number, 15)
    
    Call ElevatorButton(85, "85", Number, 16)
    Call ElevatorButton(86, "86", Number, 17)
    Call ElevatorButton(87, "87", Number, 18)
    Call ElevatorButton(88, "88", Number, 19)
    Call ElevatorButton(89, "89", Number, 20)
    
    Call ElevatorButton(80, "80", Number, 21)
    Call ElevatorButton(81, "81", Number, 22)
    Call ElevatorButton(82, "82", Number, 23)
    Call ElevatorButton(83, "83", Number, 24)
    Call ElevatorButton(84, "84", Number, 25)
    
    Call ElevatorButton(75, "75", Number, 26)
    Call ElevatorButton(76, "76", Number, 27)
    Call ElevatorButton(77, "77", Number, 28)
    Call ElevatorButton(78, "78", Number, 29)
    Call ElevatorButton(79, "79", Number, 30)
    
    Call ElevatorButton(70, "70", Number, 31)
    Call ElevatorButton(71, "71", Number, 32)
    Call ElevatorButton(72, "72", Number, 33)
    Call ElevatorButton(73, "73", Number, 34)
    Call ElevatorButton(74, "74", Number, 35)
    
    Call ElevatorButton(65, "65", Number, 36)
    Call ElevatorButton(66, "66", Number, 37)
    Call ElevatorButton(67, "67", Number, 38)
    Call ElevatorButton(68, "68", Number, 39)
    Call ElevatorButton(69, "69", Number, 40)
    
    Call ElevatorButton(60, "60", Number, 41)
    Call ElevatorButton(61, "61", Number, 42)
    Call ElevatorButton(62, "62", Number, 43)
    Call ElevatorButton(63, "63", Number, 44)
    Call ElevatorButton(64, "64", Number, 45)
    
    Call ElevatorButton(55, "55", Number, 46)
    Call ElevatorButton(56, "56", Number, 47)
    Call ElevatorButton(57, "57", Number, 48)
    Call ElevatorButton(58, "58", Number, 49)
    Call ElevatorButton(59, "59", Number, 50)
    
    Call ElevatorButton(0, "L", Number, 51)
    Call ElevatorButton(51, "51", Number, 52)
    Call ElevatorButton(52, "52", Number, 53)
    Call ElevatorButton(53, "53", Number, 54)
    Call ElevatorButton(54, "54", Number, 55)
    
End If

If Number = 13 Or Number = 14 Then

    Call ElevatorButton(80, "80", Number, 28)
    Call ElevatorButton(0, "L", Number, 33)
    
End If

If Number >= 15 And Number <= 20 Then

    Call ElevatorButton(98, "98", Number, 7)
    Call ElevatorButton(99, "99", Number, 9)
    
    Call ElevatorButton(96, "96", Number, 12)
    Call ElevatorButton(97, "97", Number, 14)
    
    Call ElevatorButton(94, "94", Number, 17)
    Call ElevatorButton(95, "95", Number, 19)
    
    Call ElevatorButton(92, "92", Number, 22)
    Call ElevatorButton(93, "93", Number, 24)
    
    Call ElevatorButton(90, "90", Number, 27)
    Call ElevatorButton(91, "91", Number, 29)
    
    Call ElevatorButton(88, "88", Number, 32)
    Call ElevatorButton(89, "89", Number, 34)
    
    Call ElevatorButton(86, "86", Number, 37)
    Call ElevatorButton(87, "87", Number, 39)
    
    Call ElevatorButton(84, "84", Number, 42)
    Call ElevatorButton(85, "85", Number, 44)
    
    Call ElevatorButton(82, "82", Number, 47)
    Call ElevatorButton(83, "83", Number, 49)
    
    Call ElevatorButton(80, "80", Number, 52)
    Call ElevatorButton(81, "81", Number, 54)
    
End If
    
If Number >= 21 And Number <= 30 Then

    Call ElevatorButton(75, "75", Number, 6)
    Call ElevatorButton(76, "76", Number, 7)
    Call ElevatorButton(77, "77", Number, 8)
    Call ElevatorButton(78, "78", Number, 9)
    Call ElevatorButton(79, "79", Number, 10)
    
    Call ElevatorButton(70, "70", Number, 11)
    Call ElevatorButton(71, "71", Number, 12)
    Call ElevatorButton(72, "72", Number, 13)
    Call ElevatorButton(73, "73", Number, 14)
    Call ElevatorButton(74, "74", Number, 15)
    
    Call ElevatorButton(65, "65", Number, 16)
    Call ElevatorButton(66, "66", Number, 17)
    Call ElevatorButton(67, "67", Number, 18)
    Call ElevatorButton(68, "68", Number, 19)
    Call ElevatorButton(69, "69", Number, 20)
    
    Call ElevatorButton(60, "60", Number, 21)
    Call ElevatorButton(61, "61", Number, 22)
    Call ElevatorButton(62, "62", Number, 23)
    Call ElevatorButton(63, "63", Number, 24)
    Call ElevatorButton(64, "64", Number, 25)
    
    Call ElevatorButton(55, "55", Number, 26)
    Call ElevatorButton(56, "56", Number, 27)
    Call ElevatorButton(57, "57", Number, 28)
    Call ElevatorButton(58, "58", Number, 29)
    Call ElevatorButton(59, "59", Number, 30)
    
    Call ElevatorButton(50, "50", Number, 31)
    Call ElevatorButton(51, "51", Number, 32)
    Call ElevatorButton(52, "52", Number, 33)
    Call ElevatorButton(53, "53", Number, 34)
    Call ElevatorButton(54, "54", Number, 35)
    
    Call ElevatorButton(45, "45", Number, 36)
    Call ElevatorButton(46, "46", Number, 37)
    Call ElevatorButton(47, "47", Number, 38)
    Call ElevatorButton(48, "48", Number, 39)
    Call ElevatorButton(49, "49", Number, 40)
    
    Call ElevatorButton(40, "40", Number, 41)
    Call ElevatorButton(41, "41", Number, 42)
    Call ElevatorButton(42, "42", Number, 43)
    Call ElevatorButton(43, "43", Number, 44)
    Call ElevatorButton(44, "44", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 48)
    
End If

If Number >= 31 And Number <= 40 Then

    Call ElevatorButton(37, "37", Number, 7)
    Call ElevatorButton(38, "38", Number, 8)
    Call ElevatorButton(39, "39", Number, 9)
    
    Call ElevatorButton(32, "32", Number, 11)
    Call ElevatorButton(33, "33", Number, 12)
    Call ElevatorButton(34, "34", Number, 13)
    Call ElevatorButton(35, "35", Number, 14)
    Call ElevatorButton(36, "36", Number, 15)
    
    Call ElevatorButton(27, "27", Number, 16)
    Call ElevatorButton(28, "28", Number, 17)
    Call ElevatorButton(29, "29", Number, 18)
    Call ElevatorButton(30, "30", Number, 19)
    Call ElevatorButton(31, "31", Number, 20)
    
    Call ElevatorButton(22, "22", Number, 21)
    Call ElevatorButton(23, "23", Number, 22)
    Call ElevatorButton(24, "24", Number, 23)
    Call ElevatorButton(25, "25", Number, 24)
    Call ElevatorButton(26, "26", Number, 25)
    
    Call ElevatorButton(17, "17", Number, 26)
    Call ElevatorButton(18, "18", Number, 27)
    Call ElevatorButton(19, "19", Number, 28)
    Call ElevatorButton(20, "20", Number, 29)
    Call ElevatorButton(21, "21", Number, 30)
    
    Call ElevatorButton(12, "12", Number, 31)
    Call ElevatorButton(13, "13", Number, 32)
    Call ElevatorButton(14, "14", Number, 33)
    Call ElevatorButton(15, "15", Number, 34)
    Call ElevatorButton(16, "16", Number, 35)
    
    Call ElevatorButton(7, "7", Number, 36)
    Call ElevatorButton(8, "8", Number, 37)
    Call ElevatorButton(9, "9", Number, 38)
    Call ElevatorButton(10, "10", Number, 39)
    Call ElevatorButton(11, "11", Number, 40)
    
    Call ElevatorButton(2, "2", Number, 41)
    Call ElevatorButton(3, "3", Number, 42)
    Call ElevatorButton(4, "4", Number, 43)
    Call ElevatorButton(5, "5", Number, 44)
    Call ElevatorButton(6, "6", Number, 45)
    
    Call ElevatorButton(0, "L", Number, 48)
    
End If
End Sub
Sub CheckElevatorButtons()
If InElevator = False Then Exit Sub

'collision routine for checking if an elevator button is pressed

For i52 = -11 To 144

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons(i52).GetMeshName Then
    If i52 > 138 Then
        If i52 = 139 Then OpenElevator(ElevatorNumber) = 1 'Open button
        If i52 = 140 Then OpenElevator(ElevatorNumber) = -1 'Close button
        'If i52 = 141 Then Call DeleteRoute(QueuePositionDirection(ElevatorNumber)) 'Cancel button
        If i52 = 142 Then Call StopElevator(ElevatorNumber) 'Stop button
        If i52 = 143 Then Call Alarm(ElevatorNumber) 'Alarm button
        Exit Sub
    End If
    Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(ElevatorNumber) = True
    'OpenElevator(ElevatorNumber) = -1
    'GotoFloor(ElevatorNumber) = i52
    Dim Direction As Integer
    If i52 < ElevatorFloor(ElevatorNumber) Then Direction = -1
    If i52 > ElevatorFloor(ElevatorNumber) Then Direction = 1
    Call AddRoute(Int(i52), ElevatorNumber, Direction)
End If

Next i52
End Sub

Sub ElevatorLoop(Number As Integer)
Dim FloorHeight As Single
Dim FloorAltitude As Single
FloorHeight = GetFloorHeight(Number)
FloorAltitude = GetFloorAltitude(Number)

On Error GoTo ErrorHandler
If DebugPanel.Check11.Value = 0 Then Exit Sub

Call ProcessCallQueue(Number)

If ElevatorEnable(Number) = 0 And GotoFloor(Number) = 0 And OpenElevator(Number) = 0 And FineTune(Number) = False Then Exit Sub

elevatorstart(Number) = Elevator(Number).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(Number) = GetElevatorFloor(Number)
      
      If GotoFloor(Number) = ElevatorFloor(Number) - 1 Then CurrentFloor(Number) = ElevatorFloor(Number)

      If GotoFloor(Number) <> 0 And GotoFloor(Number) > CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorDoorL(Number).GetPosition.z <= 0 Then
      ElevatorDirection(Number) = 1
      OriginalLocation(Number) = CurrentFloorExact(Number)
      DistanceToTravel(Number) = GetFloorAltitude(Int(GotoFloor(Number))) - GetFloorAltitude(Int(CurrentFloorExact(Number)))
      If ElevatorSync(Number) = True Then
      Call DeleteStairDoors
      Room(CameraFloor).Enable False
      CrawlSpace(CameraFloor).Enable False
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable False
      For ElevTemp(Number) = 1 To 40
      CallButtonsUp(ElevTemp(Number)).Enable False
      CallButtonsDown(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable False
      ElevatorDoorR(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Buildings.Enable False
      Landscape.Enable False
      If StairDataTable(CameraFloor) = True Then DeleteStairs (CameraFloor)
      If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = True Then DeleteStairs (CameraFloor + 1)
      If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = True Then DeleteStairs (CameraFloor - 1)
      End If
      End If
      If GotoFloor(Number) <> 0 And GotoFloor(Number) < CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorDoorL(Number).GetPosition.z <= 0 Then
      Elevator(Number).Enable True
      ElevatorDirection(Number) = -1
      OriginalLocation(Number) = CurrentFloorExact(Number)
      DistanceToTravel(Number) = GetFloorAltitude(Int(CurrentFloorExact(Number))) - GetFloorAltitude(Int(GotoFloor(Number)))
      If ElevatorSync(Number) = True Then
      Call DeleteStairDoors
      Room(CameraFloor).Enable False
      CrawlSpace(CameraFloor).Enable False
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable False
      For ElevTemp(Number) = 1 To 40
      CallButtonsUp(ElevTemp(Number)).Enable False
      CallButtonsDown(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable False
      ElevatorDoorR(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Buildings.Enable False
      Landscape.Enable False
      If StairDataTable(CameraFloor) = True Then DeleteStairs (CameraFloor)
      If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = True Then DeleteStairs (CameraFloor + 1)
      If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = True Then DeleteStairs (CameraFloor - 1)
      End If
      End If
      
       CurrentFloorExact(Number) = GetFloorExact(elevatorstart(Number).Y)
       CurrentFloor(Number) = Int(CurrentFloorExact(Number))
        
        If ElevatorEnable(Number) >= 0 And ElevatorDirection(Number) = 1 Then
        'sound
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(Number) = 0 And GotoFloor(Number) <> ElevatorFloor(Number) Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevstart.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck(Number) = 1
        End If
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(Number) = 1 Then
        ElevatorSounds(Number).Load App.Path + "\data\elevmove.wav"
        ElevatorSounds(Number).Loop_ = True
        ElevatorSounds(Number).Play
        End If
        'movement
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorEnable(Number) = ElevatorEnable(Number) + 0.25
        If ElevatorEnable(Number) <= 15 Then StoppingDistance(Number) = CurrentFloorExact(Number) - OriginalLocation(Number) + 0.4
        If ElevatorEnable(Number) > 15 Then ElevatorEnable(Number) = 15
        Destination(Number) = GetFloorExact(OriginalLocation(Number)) + DistanceToTravel(Number) - 40
        If GotoFloor(Number) <> 0 And elevatorstart(Number).Y >= Destination(Number) - GetFloorExact(StoppingDistance(Number)) Then ElevatorDirection(Number) = -1: ElevatorCheck(Number) = 0
        End If
      
        If ElevatorEnable(Number) > 0 And ElevatorDirection(Number) = -1 Then
        'Sounds
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Stop_
        End If
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevstop.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck2(Number) = 1
        End If
        'Movement
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        
        ElevatorEnable(Number) = ElevatorEnable(Number) - 0.25
        If ElevatorEnable(Number) < 0 Then ElevatorEnable(Number) = 0
        If ElevatorEnable(Number) = 0 Then ElevatorDirection(Number) = 0
        If GotoFloor(Number) <> 0 Then ElevatorCheck(Number) = 0: FineTune(Number) = True
        End If
      
        If ElevatorEnable(Number) <= 0 And ElevatorDirection(Number) = -1 Then
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(Number) = 0 Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevstart.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck(Number) = 1
        End If
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(Number) = 1 Then
        ElevatorSounds(Number).Load App.Path + "\data\elevmove.wav"
        ElevatorSounds(Number).Loop_ = True
        ElevatorSounds(Number).Play
        End If
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorEnable(Number) = ElevatorEnable(Number) - 0.25
        If ElevatorEnable(Number) >= -15 Then StoppingDistance(Number) = OriginalLocation(Number) - CurrentFloorExact(Number)
        If ElevatorEnable(Number) < -15 Then ElevatorEnable(Number) = -15
        Destination(Number) = GetFloorExact(OriginalLocation(Number)) - DistanceToTravel(Number) - 15
        If GotoFloor(Number) <> 0 And elevatorstart(Number).Y <= Destination(Number) + GetFloorExact(StoppingDistance(Number)) Then ElevatorDirection(Number) = 1: ElevatorCheck(Number) = 0
        End If
      
        If ElevatorEnable(Number) < 0 And ElevatorDirection(Number) = 1 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Stop_
        End If
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevstop.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck2(Number) = 1
        End If
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorEnable(Number) = ElevatorEnable(Number) + 0.25
        If ElevatorEnable(Number) > 0 Then ElevatorEnable(Number) = 0
        If ElevatorEnable(Number) = 0 Then ElevatorDirection(Number) = 0
        If GotoFloor(Number) <> 0 Then ElevatorCheck(Number) = 0: FineTune(Number) = True
        End If
      
      If FineTune(Number) = True And ElevatorEnable(Number) = 0 And elevatorstart(Number).Y > GetFloorExact(GotoFloor(Number)) + -0.3 And elevatorstart(Number).Y < (GotoFloor(Number) * FloorHeight) + FloorHeight + 0.3 Then
      FineTune(Number) = False
      Room(CameraFloor).Enable True
      CrawlSpace(CameraFloor).Enable True
      If CameraFloor > BottomFloor Then CrawlSpace(CameraFloor - 1).Enable True
      If ElevatorSync(Number) = True Then
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable True
      ElevatorDoorR(ElevTemp(Number)).Enable True
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      CallButtonsUp(ElevTemp(Number)).Enable True
      CallButtonsDown(ElevTemp(Number)).Enable True
      Next ElevTemp(Number)
      ShaftsFloor(CameraFloor).Enable True
      'If StairDataTable(CameraFloor) = False Then CreateStairs (CameraFloor)
      Atmos.SkyBox_Enable True
      Buildings.Enable True
      Landscape.Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      Call InitRealtime(CameraFloor)
      InitObjectsForFloor (CameraFloor)
      'If CameraFloor < TopFloor And StairDataTable(CameraFloor + 1) = False Then CreateStairs (CameraFloor + 1)
      'If CameraFloor > BottomFloor And StairDataTable(CameraFloor - 1) = False Then CreateStairs (CameraFloor - 1)
      End If
      GotoFloor(Number) = 0
      OpenElevator(Number) = 1
      ElevatorCheck(Number) = 0
      ElevatorCheck2(Number) = 0
      ElevatorCheck3(Number) = 0
      ElevatorCheck4(Number) = 0
      If CameraFloor > BottomFloor And ElevatorSync(Number) = True Then Camera.SetPosition Camera.GetPosition.X, GetFloorAltitude(CameraFloor) + 10, Camera.GetPosition.z
      End If
      
      If FineTune(Number) = True Then
      If ElevatorCheck3(Number) = 0 Then
      ElevatorSounds(Number).Load App.Path + "\data\ding1.wav"
      ElevatorSounds(Number).Play
      ElevatorCheck3(Number) = 1
        'For ElevTemp(Number) = -11 To 144
        'If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).SetColor RGBA(1, 1, 1, 1)
        'Next ElevTemp(Number)
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(GotoFloor(Number)).SetColor RGBA(1, 1, 1, 1)
      End If
      If elevatorstart(Number).Y < GetFloorExact(GotoFloor(Number)) Then
      Elevator(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      ElevatorInsDoorL(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      ElevatorInsDoorR(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      Plaque(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, ElevatorFineTuneSpeed, 0
        Next ElevTemp(Number)
        FloorIndicator(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, ElevatorFineTuneSpeed, 0
      End If
      If elevatorstart(Number).Y > GetFloorExact(GotoFloor(Number)) Then
      Elevator(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      ElevatorInsDoorL(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      ElevatorInsDoorR(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      Plaque(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        For ElevTemp(Number) = -11 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        Next ElevTemp(Number)
        FloorIndicator(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      End If
      End If
      
      If OpenElevator(Number) = 1 Then
      
      Dim jxx As Integer
      Dim jyy As Integer
      jxx = CameraFloor
      jyy = ElevatorFloor(Number)
      If jxx <> jyy Then OpenElevator(Number) = 0: Exit Sub
      
      If ElevatorDoorL(Number).GetPosition.z >= 4 Then OpenElevator(Number) = 0: GoTo OpenElevator1
      If ElevatorCheck4(Number) = 0 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(Number).Stop_
        End If
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevatoropen.wav"
        ElevatorSounds(Number).SetPosition Elevator(Number).GetPosition.X / SoundDivisor, Elevator(Number).GetPosition.Y / SoundDivisor, Elevator(Number).GetPosition.z / SoundDivisor
        ElevatorSounds(Number).maxDistance = SoundMaxDistance
        ElevatorSounds(Number).Play
        ElevatorCheck4(Number) = 1
      End If
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) + 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If ElevatorDoorL(Number).GetPosition.z > 1 Then OpenElevator(Number) = 2
OpenElevator1:
      End If
      
      If OpenElevator(Number) = 2 Then
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If ElevatorDoorL(Number).GetPosition.z > 3 Then OpenElevator(Number) = 3
      End If
      
      If OpenElevator(Number) = 3 Then
      ElevatorCheck4(Number) = 0
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) - 0.02
      If ElevatorDoorL(Number).GetPosition.z < 7 And OpenElevatorLoc(Number) = 0 Then OpenElevatorLoc(Number) = 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If OpenElevatorLoc(Number) <= 0 Then
      OpenElevator(Number) = 0
      OpenElevatorLoc(Number) = 0
      If Number = 1 Then Sim.Timer1.Enabled = True
      If Number = 2 Then Sim.Timer2.Enabled = True
      If Number = 3 Then Sim.Timer3.Enabled = True
      If Number = 4 Then Sim.Timer4.Enabled = True
      If Number = 5 Then Sim.Timer5.Enabled = True
      If Number = 6 Then Sim.Timer6.Enabled = True
      If Number = 7 Then Sim.Timer7.Enabled = True
      If Number = 8 Then Sim.Timer8.Enabled = True
      If Number = 9 Then Sim.Timer9.Enabled = True
      If Number = 10 Then Sim.Timer10.Enabled = True
      If Number = 11 Then Sim.Timer11.Enabled = True
      If Number = 12 Then Sim.Timer12.Enabled = True
      If Number = 13 Then Sim.Timer13.Enabled = True
      If Number = 14 Then Sim.Timer14.Enabled = True
      If Number = 15 Then Sim.Timer15.Enabled = True
      If Number = 16 Then Sim.Timer16.Enabled = True
      If Number = 17 Then Sim.Timer17.Enabled = True
      If Number = 18 Then Sim.Timer18.Enabled = True
      If Number = 19 Then Sim.Timer19.Enabled = True
      If Number = 20 Then Sim.Timer20.Enabled = True
      If Number = 21 Then Sim.Timer21.Enabled = True
      If Number = 22 Then Sim.Timer22.Enabled = True
      If Number = 23 Then Sim.Timer23.Enabled = True
      If Number = 24 Then Sim.Timer24.Enabled = True
      If Number = 25 Then Sim.Timer25.Enabled = True
      If Number = 26 Then Sim.Timer26.Enabled = True
      If Number = 27 Then Sim.Timer27.Enabled = True
      If Number = 28 Then Sim.Timer28.Enabled = True
      If Number = 29 Then Sim.Timer29.Enabled = True
      If Number = 30 Then Sim.Timer30.Enabled = True
      If Number = 31 Then Sim.Timer31.Enabled = True
      If Number = 32 Then Sim.Timer32.Enabled = True
      If Number = 33 Then Sim.Timer33.Enabled = True
      If Number = 34 Then Sim.Timer34.Enabled = True
      If Number = 35 Then Sim.Timer35.Enabled = True
      If Number = 36 Then Sim.Timer36.Enabled = True
      If Number = 37 Then Sim.Timer37.Enabled = True
      If Number = 38 Then Sim.Timer38.Enabled = True
      If Number = 39 Then Sim.Timer39.Enabled = True
      If Number = 40 Then Sim.Timer40.Enabled = True
      End If
      End If
      
      If OpenElevator(Number) = -1 Then
      
      If GotoFloor(Number) = 0 Then PauseQueueSearch(Number) = False
      
      If ElevatorDoorL(Number).GetPosition.z <= 0 Then OpenElevator(Number) = 0: GoTo OpenElevator2
      If ElevatorCheck4(Number) = 0 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(Number).Stop_
        End If
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\data\elevatorclose.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck4(Number) = 1
      End If
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) - 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If ElevatorDoorL(Number).GetPosition.z < 3 Then OpenElevator(Number) = -2
OpenElevator2:
      End If
      
      If OpenElevator(Number) = -2 Then
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If ElevatorDoorL(Number).GetPosition.z < 1 Then OpenElevator(Number) = -3
      End If
      
      If OpenElevator(Number) = -3 Then
      ElevatorCheck4(Number) = 0
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) + 0.02
      If ElevatorDoorL(Number).GetPosition.z > 0 And OpenElevatorLoc(Number) >= 0 Then OpenElevatorLoc(Number) = -0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If OpenElevatorLoc(Number) >= 0 Then
      OpenElevator(Number) = 0
      OpenElevatorLoc(Number) = 0
      ElevatorInsDoorL(Number).SetPosition ElevatorInsDoorL(Number).GetPosition.X, ElevatorInsDoorL(Number).GetPosition.Y, 0
      ElevatorInsDoorR(Number).SetPosition ElevatorInsDoorR(Number).GetPosition.X, ElevatorInsDoorR(Number).GetPosition.Y, 0
      ElevatorDoorL(Number).SetPosition ElevatorDoorL(Number).GetPosition.X, ElevatorDoorL(Number).GetPosition.Y, 0
      ElevatorDoorR(Number).SetPosition ElevatorDoorR(Number).GetPosition.X, ElevatorDoorR(Number).GetPosition.Y, 0
      End If
      End If
Exit Sub
ErrorHandler:
   MsgBox "An error occurred in the elevator simulation subsystem." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End
    
End Sub

Sub ElevatorButton(FloorID As Integer, ButtonLabel As String, Number As Integer, ButtonIndex As Integer)
    
    'This code segment places all of the actual elevator button processing code into a single algorithm
    'FloorID - Exact floor that button calls
    'ButtonLabel - Simplified label for button
    'ButtonLabel2 - Texture name of button derived from ButtonLabel
    'Number - Elevator number
    'ButtonIndex - Position of button on table below
    'ShaftLeft/Right - the left and right x coordinate boundaries of the elevator's shaft
    'Side - -1 if elevator is on left side (elevator number is even), and 1 if it is on the right
    'Side2 - -1 if elevator is in an odd-numbered shaft, and 1 if the shaft is even
    'ElevIndex - Elevator's group number (0-4); 1 or 2=group0, 3 or 4=group1, etc
    'Offset - Initial Z offset of buttons
    'IndexH - Horizontal index of button
    'IndexV - Vertical index of button
    
    'ButtonIndex values (based on visual look of panel)
    '1  2  3  4  5    0
    '6  7  8  9  10   1
    '11 12 13 14 15   2
    '16 17 18 19 20   3
    '21 22 23 24 25   4
    '26 27 28 29 30   5
    '31 32 33 34 35   6
    '36 37 38 39 40   7
    '41 42 43 44 45   8
    '46 47 48 49 50   9
    '51 52 53 54 55   10
    '56 57 58 59 60   11
                      'V
  'H 0  1  2  3  4
    'So button 54 would have an IndexH of 3 and an IndexV of 10, while button 6 would have an IndexH of 0 and an IndexV of 1
    
    Dim ButtonLabel2 As String
    Dim ShaftLeft As Single
    Dim Side As Integer
    Dim Side2 As Integer
    Dim ElevIndex As Integer
    Dim Offset As Single
    Dim IndexH As Integer
    Dim IndexV As Integer
    
    ButtonLabel = LCase(ButtonLabel)
    
    If IsNumeric(ButtonLabel) Then ButtonLabel2 = "Button" + ButtonLabel
    If ButtonLabel = "l" Then ButtonLabel2 = "ButtonL"
    If ButtonLabel = "m" Then ButtonLabel2 = "ButtonM"
    If ButtonLabel = "r" Then ButtonLabel2 = "ButtonR"
    If ButtonLabel = "open" Then ButtonLabel2 = "ButtonOpen"
    If ButtonLabel = "close" Then ButtonLabel2 = "ButtonClose"
    If ButtonLabel = "stop" Then ButtonLabel2 = "ButtonStop"
    If ButtonLabel = "alarm" Then ButtonLabel2 = "ButtonAlarm"
    If ButtonLabel = "cancel" Then ButtonLabel2 = "ButtonCancel"
    
    ElevIndex = Number
    If IsEven(ElevIndex) = True Then ElevIndex = ElevIndex - 1
    If Number <= 10 Then ShaftLeft = 12.5: Side2 = 1: ElevIndex = Int(ElevIndex / 2)
    If Number > 10 And Number <= 20 Then ShaftLeft = 52.5: Side2 = -1: ElevIndex = Int((ElevIndex - 10) / 2)
    If Number > 20 And Number <= 30 Then ShaftLeft = 90.5: Side2 = 1: ElevIndex = Int((ElevIndex - 20) / 2)
    If Number > 30 And Number <= 40 Then ShaftLeft = 130.5: Side2 = -1: ElevIndex = Int((ElevIndex - 30) / 2)
    
    If IsEven(Number) = False Then
        Side = -1
        If Side2 = 1 Then Offset = -27.85
        If Side2 = -1 Then Offset = -18.45
    Else
        Side = 1
        If Side2 = -1 Then Offset = -27.85
        If Side2 = 1 Then Offset = -18.45
    End If
            
    If ButtonIndex <= 5 Then IndexH = ButtonIndex - 1: IndexV = 0
    If ButtonIndex > 5 And ButtonIndex <= 10 Then IndexH = ButtonIndex - 6: IndexV = 1
    If ButtonIndex > 10 And ButtonIndex <= 15 Then IndexH = ButtonIndex - 11: IndexV = 2
    If ButtonIndex > 15 And ButtonIndex <= 20 Then IndexH = ButtonIndex - 16: IndexV = 3
    If ButtonIndex > 20 And ButtonIndex <= 25 Then IndexH = ButtonIndex - 21: IndexV = 4
    If ButtonIndex > 25 And ButtonIndex <= 30 Then IndexH = ButtonIndex - 26: IndexV = 5
    If ButtonIndex > 30 And ButtonIndex <= 35 Then IndexH = ButtonIndex - 31: IndexV = 6
    If ButtonIndex > 35 And ButtonIndex <= 40 Then IndexH = ButtonIndex - 36: IndexV = 7
    If ButtonIndex > 40 And ButtonIndex <= 45 Then IndexH = ButtonIndex - 41: IndexV = 8
    If ButtonIndex > 45 And ButtonIndex <= 50 Then IndexH = ButtonIndex - 46: IndexV = 9
    If ButtonIndex > 50 And ButtonIndex <= 55 Then IndexH = ButtonIndex - 51: IndexV = 10
    If ButtonIndex > 55 And ButtonIndex <= 60 Then IndexH = ButtonIndex - 56: IndexV = 11
    
    Buttons(FloorID).AddWall GetTex(ButtonLabel2), Side * (ShaftLeft + (0.17 * Side2)), Offset + (15 * ElevIndex) + (0.4 * IndexH * Side * Side2), Side * (ShaftLeft + (0.17 * Side2)), (Offset + 0.3) + (15 * ElevIndex) + (0.4 * IndexH * Side * Side2), 0.3, (Elevator(Number).GetPosition.Y + 12) - (0.5 * IndexV), Side * Side2, 1
    
End Sub

