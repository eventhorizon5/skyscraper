Attribute VB_Name = "ElevatorCode"
'Skycraper 0.92 Beta
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

Option Explicit



Sub DrawElevatorButtons(Number As Integer)
ButtonsEnabled = True

For i54 = -1 To 144
'Buttons(i54).Enable False
Scene.DestroyMesh Buttons(i54)
Set Buttons(i54) = Nothing
'Sleep 10
Next i54

'Sleep 100

For i54 = -1 To 144
Set Buttons(i54) = New TVMesh
Set Buttons(i54) = Scene.CreateMeshBuilder("Buttons " + Str$(i54))
Buttons(i54).SetPosition 0, 0, 0
Buttons(i54).SetRotation 0, 0, 0
Next i54

If Number <= 10 Then Call DrawElevatorButtons1(Number)
If Number > 10 And Number <= 20 Then Call DrawElevatorButtons2(Number)
If Number > 20 And Number <= 30 Then Call DrawElevatorButtons3(Number)
If Number > 30 And Number <= 40 Then Call DrawElevatorButtons4(Number)

End Sub

Sub DrawElevatorButtons1(Number As Integer)

Dim ShaftLeft As Single

ShaftLeft = 12.5

    'each panel can cover 60 floors
'Elevator 1
If Number = 1 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    
    Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(130).AddWall GetTex("Button130"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(131).AddWall GetTex("Button131"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(132).AddWall GetTex("Button132"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(133).AddWall GetTex("Button133"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(124).AddWall GetTex("Button124"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(125).AddWall GetTex("Button125"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(126).AddWall GetTex("Button126"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(119).AddWall GetTex("Button119"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(120).AddWall GetTex("Button120"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(121).AddWall GetTex("Button121"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(115).AddWall GetTex("Button115"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(116).AddWall GetTex("Button116"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(117).AddWall GetTex("Button117"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(112).AddWall GetTex("Button112"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(107).AddWall GetTex("Button107"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(102).AddWall GetTex("Button102"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(0).AddWall GetTex("ButtonM"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(0).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(2).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(40).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 2
If Number = 2 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45  + 0.8, (ShaftLeft+0.17), -18.15  + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(131).AddWall GetTex("Button131"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45  + 0.8, (ShaftLeft+0.17), -18.15  + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(133).AddWall GetTex("Button133"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    'Buttons(115).AddWall GetTex("Button115"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(116).AddWall GetTex("Button116"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(136).AddWall GetTex("Button136"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(129).AddWall GetTex("Button129"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(111).AddWall GetTex("Button111"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(135).AddWall GetTex("Button135"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(113).AddWall GetTex("Button113"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(106).AddWall GetTex("Button106"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(134).AddWall GetTex("Button134"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(108).AddWall GetTex("Button108"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(101).AddWall GetTex("Button101"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(132).AddWall GetTex("Button132"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(103).AddWall GetTex("Button103"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(0).AddWall GetTex("ButtonM"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(88).AddWall GetTex("Button88"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(40).AddWall GetTex("Button40"), (ShaftLeft+0.17), -18.45  + 0.8, (ShaftLeft+0.17), -18.15  + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 3
If Number = 3 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(131).AddWall GetTex("Button131"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(133).AddWall GetTex("Button133"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    'Buttons(115).AddWall GetTex("Button115"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(116).AddWall GetTex("Button116"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    'Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(132).AddWall GetTex("Button132"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    'Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(0).AddWall GetTex("ButtonM"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(88).AddWall GetTex("Button88"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 4
If Number = 4 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(131).AddWall GetTex("Button131"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(133).AddWall GetTex("Button133"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    'Buttons(115).AddWall GetTex("Button115"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(116).AddWall GetTex("Button116"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(136).AddWall GetTex("Button136"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(129).AddWall GetTex("Button129"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(111).AddWall GetTex("Button111"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(135).AddWall GetTex("Button135"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(113).AddWall GetTex("Button113"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(106).AddWall GetTex("Button106"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(134).AddWall GetTex("Button134"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(108).AddWall GetTex("Button108"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(101).AddWall GetTex("Button101"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(132).AddWall GetTex("Button132"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(103).AddWall GetTex("Button103"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(0).AddWall GetTex("ButtonM"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(88).AddWall GetTex("Button88"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    
    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(40).AddWall GetTex("Button40"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 5
If Number = 5 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(124).AddWall GetTex("Button124"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(125).AddWall GetTex("Button125"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(126).AddWall GetTex("Button126"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(121).AddWall GetTex("Button121"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(119).AddWall GetTex("Button119"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(120).AddWall GetTex("Button120"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(112).AddWall GetTex("Button112"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(107).AddWall GetTex("Button107"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(102).AddWall GetTex("Button102"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 6
If Number = 6 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(127).AddWall GetTex("Button127"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(128).AddWall GetTex("Button128"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(129).AddWall GetTex("Button129"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(124).AddWall GetTex("Button124"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(125).AddWall GetTex("Button125"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(126).AddWall GetTex("Button126"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(127).AddWall GetTex("Button127"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(121).AddWall GetTex("Button121"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(122).AddWall GetTex("Button122"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(123).AddWall GetTex("Button123"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(122).AddWall GetTex("Button122"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(118).AddWall GetTex("Button118"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(119).AddWall GetTex("Button119"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(120).AddWall GetTex("Button120"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(112).AddWall GetTex("Button112"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(113).AddWall GetTex("Button113"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(114).AddWall GetTex("Button114"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(109).AddWall GetTex("Button109"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(110).AddWall GetTex("Button110"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(111).AddWall GetTex("Button111"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(106).AddWall GetTex("Button106"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(107).AddWall GetTex("Button107"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(108).AddWall GetTex("Button108"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(103).AddWall GetTex("Button103"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(104).AddWall GetTex("Button104"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(105).AddWall GetTex("Button105"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(101).AddWall GetTex("Button101"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(102).AddWall GetTex("Button102"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 7
If Number = 7 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(124).AddWall GetTex("Button124"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(125).AddWall GetTex("Button125"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(126).AddWall GetTex("Button126"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(121).AddWall GetTex("Button121"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(119).AddWall GetTex("Button119"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(120).AddWall GetTex("Button120"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(112).AddWall GetTex("Button112"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(107).AddWall GetTex("Button107"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(102).AddWall GetTex("Button102"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 8
If Number = 8 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(127).AddWall GetTex("Button127"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(128).AddWall GetTex("Button128"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(129).AddWall GetTex("Button129"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(124).AddWall GetTex("Button124"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(125).AddWall GetTex("Button125"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(126).AddWall GetTex("Button126"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(127).AddWall GetTex("Button127"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(121).AddWall GetTex("Button121"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(122).AddWall GetTex("Button122"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(123).AddWall GetTex("Button123"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(122).AddWall GetTex("Button122"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(118).AddWall GetTex("Button118"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(119).AddWall GetTex("Button119"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(120).AddWall GetTex("Button120"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(112).AddWall GetTex("Button112"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(113).AddWall GetTex("Button113"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(114).AddWall GetTex("Button114"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(109).AddWall GetTex("Button109"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(110).AddWall GetTex("Button110"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(111).AddWall GetTex("Button111"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(106).AddWall GetTex("Button106"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(107).AddWall GetTex("Button107"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(108).AddWall GetTex("Button108"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(103).AddWall GetTex("Button103"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(104).AddWall GetTex("Button104"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(105).AddWall GetTex("Button105"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(101).AddWall GetTex("Button101"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(102).AddWall GetTex("Button102"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 9
If Number = 9 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(128).AddWall GetTex("Button128"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(124).AddWall GetTex("Button124"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(125).AddWall GetTex("Button125"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(126).AddWall GetTex("Button126"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(127).AddWall GetTex("Button127"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(121).AddWall GetTex("Button121"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(123).AddWall GetTex("Button123"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(122).AddWall GetTex("Button122"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(119).AddWall GetTex("Button119"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(120).AddWall GetTex("Button120"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(112).AddWall GetTex("Button112"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(107).AddWall GetTex("Button107"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(102).AddWall GetTex("Button102"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 10
If Number = 10 Then

    'Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    
    'Buttons(128).AddWall GetTex("Button128"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(127).AddWall GetTex("Button127"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(128).AddWall GetTex("Button128"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(129).AddWall GetTex("Button129"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(123).AddWall GetTex("Button123"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(124).AddWall GetTex("Button124"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(125).AddWall GetTex("Button125"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(126).AddWall GetTex("Button126"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(127).AddWall GetTex("Button127"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(121).AddWall GetTex("Button121"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(122).AddWall GetTex("Button122"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(123).AddWall GetTex("Button123"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(122).AddWall GetTex("Button122"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(118).AddWall GetTex("Button118"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(119).AddWall GetTex("Button119"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(120).AddWall GetTex("Button120"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(112).AddWall GetTex("Button112"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(113).AddWall GetTex("Button113"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(114).AddWall GetTex("Button114"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(109).AddWall GetTex("Button109"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(110).AddWall GetTex("Button110"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(111).AddWall GetTex("Button111"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(106).AddWall GetTex("Button106"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(107).AddWall GetTex("Button107"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(108).AddWall GetTex("Button108"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(103).AddWall GetTex("Button103"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(104).AddWall GetTex("Button104"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(105).AddWall GetTex("Button105"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(101).AddWall GetTex("Button101"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(102).AddWall GetTex("Button102"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If
End Sub


Sub DrawElevatorButtons2(Number As Integer)
'Elevator Buttons
Dim ShaftLeft As Single
ShaftLeft = 52.5
    
If Number = 11 Then

    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    Buttons(115).AddWall GetTex("Button115"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    Buttons(116).AddWall GetTex("Button116"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    Buttons(117).AddWall GetTex("Button117"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), 1, 1
    
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 2
If Number = 12 Then

    Buttons(100).AddWall GetTex("Button100"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    Buttons(101).AddWall GetTex("Button101"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    Buttons(115).AddWall GetTex("Button115"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    Buttons(116).AddWall GetTex("Button116"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    Buttons(117).AddWall GetTex("Button117"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12), -1, 1
    
    Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(92).AddWall GetTex("Button92"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(88).AddWall GetTex("Button88"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(82).AddWall GetTex("Button82"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 3
If Number = 13 Then

'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(131).AddWall GetTex("Button131"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(132).AddWall GetTex("Button132"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(133).AddWall GetTex("Button133"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(115).AddWall GetTex("Button115"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(116).AddWall GetTex("Button116"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(117).AddWall GetTex("Button117"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(118).AddWall GetTex("Button118"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    'Buttons(129).AddWall GetTex("Button129"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(111).AddWall GetTex("Button111"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(112).AddWall GetTex("Button112"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(113).AddWall GetTex("Button113"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    'Buttons(114).AddWall GetTex("Button114"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(106).AddWall GetTex("Button106"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(107).AddWall GetTex("Button107"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(108).AddWall GetTex("Button108"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    'Buttons(109).AddWall GetTex("Button109"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(101).AddWall GetTex("Button101"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(103).AddWall GetTex("Button103"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    'Buttons(104).AddWall GetTex("Button104"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(92).AddWall GetTex("Button92"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(88).AddWall GetTex("Button88"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(82).AddWall GetTex("Button82"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 4
If Number = 14 Then

'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(131).AddWall GetTex("Button131"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(132).AddWall GetTex("Button132"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(133).AddWall GetTex("Button133"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(115).AddWall GetTex("Button115"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(116).AddWall GetTex("Button116"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(117).AddWall GetTex("Button117"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(118).AddWall GetTex("Button118"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    'Buttons(129).AddWall GetTex("Button129"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(110).AddWall GetTex("Button110"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(111).AddWall GetTex("Button111"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(112).AddWall GetTex("Button112"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(113).AddWall GetTex("Button113"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    'Buttons(114).AddWall GetTex("Button114"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(105).AddWall GetTex("Button105"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(106).AddWall GetTex("Button106"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(107).AddWall GetTex("Button107"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(108).AddWall GetTex("Button108"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    'Buttons(109).AddWall GetTex("Button109"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(100).AddWall GetTex("Button100"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    'Buttons(101).AddWall GetTex("Button101"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(103).AddWall GetTex("Button103"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    'Buttons(104).AddWall GetTex("Button104"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(92).AddWall GetTex("Button92"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(88).AddWall GetTex("Button88"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(82).AddWall GetTex("Button82"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(40).AddWall GetTex("Button40"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 5
If Number = 15 Then

    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(92).AddWall GetTex("Button92"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(88).AddWall GetTex("Button88"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(82).AddWall GetTex("Button82"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 6
If Number = 16 Then

    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(92).AddWall GetTex("Button92"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(88).AddWall GetTex("Button88"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(82).AddWall GetTex("Button82"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 7
If Number = 17 Then

'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(92).AddWall GetTex("Button92"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(88).AddWall GetTex("Button88"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(82).AddWall GetTex("Button82"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 8
If Number = 18 Then

'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(92).AddWall GetTex("Button92"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(88).AddWall GetTex("Button88"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(82).AddWall GetTex("Button82"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 9
If Number = 19 Then

'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(98).AddWall GetTex("Button98"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(99).AddWall GetTex("Button99"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    Buttons(97).AddWall GetTex("Button97"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, 1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(96).AddWall GetTex("Button96"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    'Buttons(134).AddWall GetTex("Button134"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, 1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(92).AddWall GetTex("Button92"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    Buttons(93).AddWall GetTex("Button93"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    'Buttons(95).AddWall GetTex("Button95"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, 1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    Buttons(91).AddWall GetTex("Button91"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(88).AddWall GetTex("Button88"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    'Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, 1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(86).AddWall GetTex("Button86"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    Buttons(87).AddWall GetTex("Button87"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    'Buttons(94).AddWall GetTex("Button94"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, 1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    Buttons(85).AddWall GetTex("Button85"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    'Buttons(89).AddWall GetTex("Button89"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, 1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(82).AddWall GetTex("Button82"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(80).AddWall GetTex("Button80"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 10
If Number = 20 Then

'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(130).AddWall GetTex("Button130"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(98).AddWall GetTex("Button98"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(99).AddWall GetTex("Button99"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    Buttons(97).AddWall GetTex("Button97"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1, -1, 1
    
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(96).AddWall GetTex("Button96"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    'Buttons(134).AddWall GetTex("Button134"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 1.5, -1, 1
    
    'Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(92).AddWall GetTex("Button92"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    Buttons(93).AddWall GetTex("Button93"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    'Buttons(95).AddWall GetTex("Button95"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2, -1, 1
    
    'Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    Buttons(91).AddWall GetTex("Button91"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 2.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(88).AddWall GetTex("Button88"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    'Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3, -1, 1
    
    'Buttons(90).AddWall GetTex("Button90"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(86).AddWall GetTex("Button86"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    Buttons(87).AddWall GetTex("Button87"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    'Buttons(94).AddWall GetTex("Button94"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 3.5, -1, 1
    
    'Buttons(81).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    Buttons(85).AddWall GetTex("Button85"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    'Buttons(89).AddWall GetTex("Button89"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4, -1, 1
    
    'Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(82).AddWall GetTex("Button82"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(80).AddWall GetTex("Button80"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If
   
End Sub

Sub DrawElevatorButtons3(Number As Integer)
'Elevator Buttons
Dim ShaftLeft As Single
ShaftLeft = 90.5
    
'Elevator 1
If Number = 21 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 , -(ShaftLeft+0.17), -27.55, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 - 0.4, -(ShaftLeft+0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 - 0.8, -(ShaftLeft+0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 - 1.2, -(ShaftLeft+0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 - 1.6, -(ShaftLeft+0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(71).AddWall GetTex("Button71"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(72).AddWall GetTex("Button72"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(73).AddWall GetTex("Button73"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(74).AddWall GetTex("Button74"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(61).AddWall GetTex("Button61"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(62).AddWall GetTex("Button62"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(63).AddWall GetTex("Button63"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(64).AddWall GetTex("Button64"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(52).AddWall GetTex("Button52"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(53).AddWall GetTex("Button53"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(54).AddWall GetTex("Button54"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.5, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 , -(ShaftLeft+0.17), -27.55, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 - 0.4, -(ShaftLeft+0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 - 1.2, -(ShaftLeft+0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 - 1.6, -(ShaftLeft+0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 , -(ShaftLeft+0.17), -27.55, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 - 0.4, -(ShaftLeft+0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 - 0.8, -(ShaftLeft+0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 - 1.2, -(ShaftLeft+0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 - 1.6, -(ShaftLeft+0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85, -(ShaftLeft + 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 - 0.4, -(ShaftLeft + 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 - 0.8, -(ShaftLeft + 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 - 1.2, -(ShaftLeft + 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 - 1.6, -(ShaftLeft + 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 2
If Number = 22 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45  + 0.8, (ShaftLeft+0.17), -18.15  + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(50).AddWall GetTex("Button50"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(45).AddWall GetTex("Button45"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(46).AddWall GetTex("Button46"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(40).AddWall GetTex("Button40"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(41).AddWall GetTex("Button41"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(42).AddWall GetTex("Button42"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(43).AddWall GetTex("Button43"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(44).AddWall GetTex("Button44"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 , (ShaftLeft+0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45  + 0.4, (ShaftLeft+0.17), -18.15  + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45  + 0.8, (ShaftLeft+0.17), -18.15  + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45  + 1.2, (ShaftLeft+0.17), -18.15  + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45  + 1.6, (ShaftLeft+0.17), -18.15  + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45, (ShaftLeft + 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + 0.4, (ShaftLeft + 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + 0.8, (ShaftLeft + 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + 1.2, (ShaftLeft + 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + 1.6, (ShaftLeft + 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 3
If Number = 23 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(71).AddWall GetTex("Button71"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(72).AddWall GetTex("Button72"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(73).AddWall GetTex("Button73"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(74).AddWall GetTex("Button74"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(61).AddWall GetTex("Button61"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(62).AddWall GetTex("Button62"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(63).AddWall GetTex("Button63"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(64).AddWall GetTex("Button64"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(52).AddWall GetTex("Button52"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(53).AddWall GetTex("Button53"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(54).AddWall GetTex("Button54"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 1), -(ShaftLeft+0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 1), -(ShaftLeft + 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 1) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 4
If Number = 24 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(50).AddWall GetTex("Button50"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(45).AddWall GetTex("Button45"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(46).AddWall GetTex("Button46"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(40).AddWall GetTex("Button40"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(41).AddWall GetTex("Button41"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(42).AddWall GetTex("Button42"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(43).AddWall GetTex("Button43"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(44).AddWall GetTex("Button44"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 1), (ShaftLeft+0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 1), (ShaftLeft + 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 1) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

Call DrawElevatorButtons31(Number)

End Sub

Sub DrawElevatorButtons31(Number As Integer)
'Elevator Buttons
Dim ShaftLeft As Single
ShaftLeft = 90.5
    
'Elevator 5
If Number = 25 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(71).AddWall GetTex("Button71"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(72).AddWall GetTex("Button72"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(73).AddWall GetTex("Button73"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(74).AddWall GetTex("Button74"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(61).AddWall GetTex("Button61"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(62).AddWall GetTex("Button62"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(63).AddWall GetTex("Button63"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(64).AddWall GetTex("Button64"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(52).AddWall GetTex("Button52"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(53).AddWall GetTex("Button53"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(54).AddWall GetTex("Button54"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 2), -(ShaftLeft+0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 2), -(ShaftLeft + 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 2) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 6
If Number = 26 Then

'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(50).AddWall GetTex("Button50"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(45).AddWall GetTex("Button45"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(46).AddWall GetTex("Button46"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(40).AddWall GetTex("Button40"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(41).AddWall GetTex("Button41"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(42).AddWall GetTex("Button42"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(43).AddWall GetTex("Button43"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(44).AddWall GetTex("Button44"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 2), (ShaftLeft+0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 2), (ShaftLeft + 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 2) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 7
If Number = 27 Then

'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(71).AddWall GetTex("Button71"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(72).AddWall GetTex("Button72"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(73).AddWall GetTex("Button73"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(74).AddWall GetTex("Button74"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(61).AddWall GetTex("Button61"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(62).AddWall GetTex("Button62"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(63).AddWall GetTex("Button63"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(64).AddWall GetTex("Button64"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(52).AddWall GetTex("Button52"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(53).AddWall GetTex("Button53"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(54).AddWall GetTex("Button54"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 3), -(ShaftLeft+0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 3), -(ShaftLeft + 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 3) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 8
If Number = 28 Then

'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(50).AddWall GetTex("Button50"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(45).AddWall GetTex("Button45"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(46).AddWall GetTex("Button46"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(40).AddWall GetTex("Button40"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(41).AddWall GetTex("Button41"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(42).AddWall GetTex("Button42"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(43).AddWall GetTex("Button43"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(44).AddWall GetTex("Button44"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 3), (ShaftLeft+0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 3), (ShaftLeft + 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 3) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 9
If Number = 29 Then

'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    Buttons(75).AddWall GetTex("Button75"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(76).AddWall GetTex("Button76"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(77).AddWall GetTex("Button77"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(78).AddWall GetTex("Button78"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    
    Buttons(70).AddWall GetTex("Button70"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(71).AddWall GetTex("Button71"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(72).AddWall GetTex("Button72"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(73).AddWall GetTex("Button73"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(74).AddWall GetTex("Button74"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(66).AddWall GetTex("Button66"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(67).AddWall GetTex("Button67"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(68).AddWall GetTex("Button68"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(69).AddWall GetTex("Button69"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(60).AddWall GetTex("Button60"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(61).AddWall GetTex("Button61"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(62).AddWall GetTex("Button62"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(63).AddWall GetTex("Button63"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(64).AddWall GetTex("Button64"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(55).AddWall GetTex("Button55"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(56).AddWall GetTex("Button56"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(57).AddWall GetTex("Button57"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(58).AddWall GetTex("Button58"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(59).AddWall GetTex("Button59"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(50).AddWall GetTex("Button50"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(51).AddWall GetTex("Button51"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(52).AddWall GetTex("Button52"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(53).AddWall GetTex("Button53"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(54).AddWall GetTex("Button54"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(45).AddWall GetTex("Button45"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(46).AddWall GetTex("Button46"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(47).AddWall GetTex("Button47"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(48).AddWall GetTex("Button48"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(49).AddWall GetTex("Button49"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(40).AddWall GetTex("Button40"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(41).AddWall GetTex("Button41"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(42).AddWall GetTex("Button42"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(43).AddWall GetTex("Button43"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(44).AddWall GetTex("Button44"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft+0.17), -27.85 + (15 * 4), -(ShaftLeft+0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft+0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft+0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft + 0.17), -27.85 + (15 * 4), -(ShaftLeft + 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.4, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 0.8, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.2, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft + 0.17), -27.85 + (15 * 4) - 1.6, -(ShaftLeft + 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 10
If Number = 30 Then

'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    Buttons(75).AddWall GetTex("Button75"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(76).AddWall GetTex("Button76"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(77).AddWall GetTex("Button77"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(78).AddWall GetTex("Button78"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(79).AddWall GetTex("Button79"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    
    Buttons(70).AddWall GetTex("Button70"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(71).AddWall GetTex("Button71"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(72).AddWall GetTex("Button72"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(73).AddWall GetTex("Button73"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(74).AddWall GetTex("Button74"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(65).AddWall GetTex("Button65"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(66).AddWall GetTex("Button66"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(67).AddWall GetTex("Button67"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(68).AddWall GetTex("Button68"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(69).AddWall GetTex("Button69"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(60).AddWall GetTex("Button60"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(61).AddWall GetTex("Button61"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(62).AddWall GetTex("Button62"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(63).AddWall GetTex("Button63"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(64).AddWall GetTex("Button64"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(55).AddWall GetTex("Button55"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(56).AddWall GetTex("Button56"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(57).AddWall GetTex("Button57"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(58).AddWall GetTex("Button58"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(59).AddWall GetTex("Button59"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(50).AddWall GetTex("Button50"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(51).AddWall GetTex("Button51"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(52).AddWall GetTex("Button52"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(53).AddWall GetTex("Button53"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(54).AddWall GetTex("Button54"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(45).AddWall GetTex("Button45"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(46).AddWall GetTex("Button46"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(47).AddWall GetTex("Button47"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(48).AddWall GetTex("Button48"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(49).AddWall GetTex("Button49"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(40).AddWall GetTex("Button40"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(41).AddWall GetTex("Button41"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(42).AddWall GetTex("Button42"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(43).AddWall GetTex("Button43"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(44).AddWall GetTex("Button44"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft+0.17), -18.45 + (15 * 4), (ShaftLeft+0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft+0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft+0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft+0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft + 0.17), -18.45 + (15 * 4), (ShaftLeft + 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.4, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 0.8, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.2, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft + 0.17), -18.45 + (15 * 4) + 1.6, (ShaftLeft + 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

End Sub


Sub DrawElevatorButtons4(Number As Integer)
'Elevator Buttons
Dim ShaftLeft As Single
ShaftLeft = 130.5

'Elevator 1
If Number = 31 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 , -(ShaftLeft - 0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 , -(ShaftLeft - 0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 , -(ShaftLeft - 0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 , -(ShaftLeft - 0.17), -18.15 , 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45, -(ShaftLeft - 0.17), -18.15, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + 0.4, -(ShaftLeft - 0.17), -18.15 + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + 0.8, -(ShaftLeft - 0.17), -18.15 + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + 1.2, -(ShaftLeft - 0.17), -18.15 + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + 1.6, -(ShaftLeft - 0.17), -18.15 + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 2
If Number = 32 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 , (ShaftLeft - 0.17), -27.55 , 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 , (ShaftLeft - 0.17), -27.55 , 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(38).AddWall GetTex("Button38"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    Buttons(32).AddWall GetTex("Button32"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(33).AddWall GetTex("Button33"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(34).AddWall GetTex("Button34"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(35).AddWall GetTex("Button35"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(36).AddWall GetTex("Button36"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(27).AddWall GetTex("Button27"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(28).AddWall GetTex("Button28"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(29).AddWall GetTex("Button29"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(30).AddWall GetTex("Button30"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(31).AddWall GetTex("Button31"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(22).AddWall GetTex("Button22"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(23).AddWall GetTex("Button23"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(24).AddWall GetTex("Button24"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(25).AddWall GetTex("Button25"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(26).AddWall GetTex("Button26"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(17).AddWall GetTex("Button17"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(18).AddWall GetTex("Button18"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(19).AddWall GetTex("Button19"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(20).AddWall GetTex("Button20"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(21).AddWall GetTex("Button21"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(12).AddWall GetTex("Button12"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(13).AddWall GetTex("Button13"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(14).AddWall GetTex("Button14"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(15).AddWall GetTex("Button15"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(16).AddWall GetTex("Button16"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(7).AddWall GetTex("Button7"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(8).AddWall GetTex("Button8"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(9).AddWall GetTex("Button9"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(10).AddWall GetTex("Button10"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(11).AddWall GetTex("Button11"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(3).AddWall GetTex("Button3"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(4).AddWall GetTex("Button4"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(5).AddWall GetTex("Button5"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(6).AddWall GetTex("Button6"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 , (ShaftLeft - 0.17), -27.55 , 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 , (ShaftLeft - 0.17), -27.55 , 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85, (ShaftLeft - 0.17), -27.55, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 - 0.4, (ShaftLeft - 0.17), -27.55 - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 - 0.8, (ShaftLeft - 0.17), -27.55 - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 - 1.2, (ShaftLeft - 0.17), -27.55 - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 - 1.6, (ShaftLeft - 0.17), -27.55 - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 3
If Number = 33 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 1), -(ShaftLeft - 0.17), -18.15 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 1) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 1) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 4
If Number = 34 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(38).AddWall GetTex("Button38"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    Buttons(32).AddWall GetTex("Button32"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(33).AddWall GetTex("Button33"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(34).AddWall GetTex("Button34"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(35).AddWall GetTex("Button35"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(36).AddWall GetTex("Button36"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(27).AddWall GetTex("Button27"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(28).AddWall GetTex("Button28"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(29).AddWall GetTex("Button29"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(30).AddWall GetTex("Button30"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(31).AddWall GetTex("Button31"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(22).AddWall GetTex("Button22"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(23).AddWall GetTex("Button23"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(24).AddWall GetTex("Button24"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(25).AddWall GetTex("Button25"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(26).AddWall GetTex("Button26"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(17).AddWall GetTex("Button17"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(18).AddWall GetTex("Button18"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(19).AddWall GetTex("Button19"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(20).AddWall GetTex("Button20"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(21).AddWall GetTex("Button21"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(12).AddWall GetTex("Button12"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(13).AddWall GetTex("Button13"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(14).AddWall GetTex("Button14"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(15).AddWall GetTex("Button15"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(16).AddWall GetTex("Button16"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(7).AddWall GetTex("Button7"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(8).AddWall GetTex("Button8"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(9).AddWall GetTex("Button9"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(10).AddWall GetTex("Button10"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(11).AddWall GetTex("Button11"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(3).AddWall GetTex("Button3"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(4).AddWall GetTex("Button4"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(5).AddWall GetTex("Button5"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(6).AddWall GetTex("Button6"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 1), (ShaftLeft - 0.17), -27.55 + (15 * 1), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 1) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 1) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

Call DrawElevatorButtons41(Number)

End Sub

Sub CheckElevatorButtons()
If InElevator = False Then Exit Sub

'collision routine for checking if an elevator button is pressed

For i52 = -1 To 144
If i52 = 1 Then i52 = 2

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons(i52).GetMeshName Then
    If i52 > 138 Then
        If i52 = 139 Then OpenElevator(ElevatorNumber) = 1
        If i52 = 140 Then OpenElevator(ElevatorNumber) = -1
        If i52 = 142 And GotoFloor(ElevatorNumber) < ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
            Buttons(i52).SetColor RGBA(1, 1, 0, 1)
            GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) - 1
            FineTune(ElevatorNumber) = True
        End If
        If i52 = 142 And GotoFloor(ElevatorNumber) > ElevatorFloor(ElevatorNumber) And GotoFloor(ElevatorNumber) <> 0 Then
            Buttons(i52).SetColor RGBA(1, 1, 0, 1)
            GotoFloor(ElevatorNumber) = CurrentFloorExact(ElevatorNumber) + 1
            FineTune(ElevatorNumber) = True
        End If
        Exit Sub
    End If
    Buttons(i52).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync(ElevatorNumber) = True
    OpenElevator(ElevatorNumber) = -1
    GotoFloor(ElevatorNumber) = i52
End If

Next i52
End Sub


Sub DrawElevatorButtons41(Number As Integer)
'Elevator Buttons
Dim ShaftLeft As Single
ShaftLeft = 130.5

'Elevator 5
If Number = 35 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 2), -(ShaftLeft - 0.17), -18.15 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 2) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 2) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 6
If Number = 36 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(38).AddWall GetTex("Button38"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    Buttons(32).AddWall GetTex("Button32"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(33).AddWall GetTex("Button33"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(34).AddWall GetTex("Button34"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(35).AddWall GetTex("Button35"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(36).AddWall GetTex("Button36"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(27).AddWall GetTex("Button27"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(28).AddWall GetTex("Button28"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(29).AddWall GetTex("Button29"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(30).AddWall GetTex("Button30"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(31).AddWall GetTex("Button31"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(22).AddWall GetTex("Button22"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(23).AddWall GetTex("Button23"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(24).AddWall GetTex("Button24"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(25).AddWall GetTex("Button25"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(26).AddWall GetTex("Button26"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(17).AddWall GetTex("Button17"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(18).AddWall GetTex("Button18"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(19).AddWall GetTex("Button19"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(20).AddWall GetTex("Button20"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(21).AddWall GetTex("Button21"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(12).AddWall GetTex("Button12"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(13).AddWall GetTex("Button13"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(14).AddWall GetTex("Button14"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(15).AddWall GetTex("Button15"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(16).AddWall GetTex("Button16"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(7).AddWall GetTex("Button7"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(8).AddWall GetTex("Button8"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(9).AddWall GetTex("Button9"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(10).AddWall GetTex("Button10"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(11).AddWall GetTex("Button11"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(3).AddWall GetTex("Button3"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(4).AddWall GetTex("Button4"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(5).AddWall GetTex("Button5"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(6).AddWall GetTex("Button6"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 2), (ShaftLeft - 0.17), -27.55 + (15 * 2), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 2) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 2) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 7
If Number = 37 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 3), -(ShaftLeft - 0.17), -18.15 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 3) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 3) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 8
If Number = 38 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(38).AddWall GetTex("Button38"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    Buttons(32).AddWall GetTex("Button32"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(33).AddWall GetTex("Button33"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(34).AddWall GetTex("Button34"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(35).AddWall GetTex("Button35"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(36).AddWall GetTex("Button36"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(27).AddWall GetTex("Button27"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(28).AddWall GetTex("Button28"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(29).AddWall GetTex("Button29"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(30).AddWall GetTex("Button30"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(31).AddWall GetTex("Button31"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(22).AddWall GetTex("Button22"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(23).AddWall GetTex("Button23"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(24).AddWall GetTex("Button24"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(25).AddWall GetTex("Button25"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(26).AddWall GetTex("Button26"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(17).AddWall GetTex("Button17"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(18).AddWall GetTex("Button18"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(19).AddWall GetTex("Button19"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(20).AddWall GetTex("Button20"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(21).AddWall GetTex("Button21"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(12).AddWall GetTex("Button12"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(13).AddWall GetTex("Button13"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(14).AddWall GetTex("Button14"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(15).AddWall GetTex("Button15"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(16).AddWall GetTex("Button16"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(7).AddWall GetTex("Button7"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(8).AddWall GetTex("Button8"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(9).AddWall GetTex("Button9"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(10).AddWall GetTex("Button10"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(11).AddWall GetTex("Button11"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(3).AddWall GetTex("Button3"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(4).AddWall GetTex("Button4"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(5).AddWall GetTex("Button5"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(6).AddWall GetTex("Button6"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 3), (ShaftLeft - 0.17), -27.55 + (15 * 3), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 3) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 3) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If

'Elevator 9
If Number = 39 Then

    'Buttons(135).AddWall GetTex("Button135"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(1).AddWall GetTex("Button1"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(136).AddWall GetTex("Button136"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(137).AddWall GetTex("Button137"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), 1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    Buttons(37).AddWall GetTex("Button37"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(38).AddWall GetTex("Button38"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, 1, 1
    
    Buttons(32).AddWall GetTex("Button32"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(33).AddWall GetTex("Button33"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(34).AddWall GetTex("Button34"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(35).AddWall GetTex("Button35"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    Buttons(36).AddWall GetTex("Button36"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, 1, 1
    
    Buttons(27).AddWall GetTex("Button27"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(28).AddWall GetTex("Button28"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(29).AddWall GetTex("Button29"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(30).AddWall GetTex("Button30"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    Buttons(31).AddWall GetTex("Button31"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, 1, 1
    
    Buttons(22).AddWall GetTex("Button22"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(23).AddWall GetTex("Button23"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(24).AddWall GetTex("Button24"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(25).AddWall GetTex("Button25"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    Buttons(26).AddWall GetTex("Button26"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, 1, 1
    
    Buttons(17).AddWall GetTex("Button17"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(18).AddWall GetTex("Button18"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(19).AddWall GetTex("Button19"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(20).AddWall GetTex("Button20"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    Buttons(21).AddWall GetTex("Button21"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, 1, 1
    
    Buttons(12).AddWall GetTex("Button12"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(13).AddWall GetTex("Button13"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(14).AddWall GetTex("Button14"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(15).AddWall GetTex("Button15"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    Buttons(16).AddWall GetTex("Button16"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, 1, 1
    
    Buttons(7).AddWall GetTex("Button7"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(8).AddWall GetTex("Button8"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(9).AddWall GetTex("Button9"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(10).AddWall GetTex("Button10"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    Buttons(11).AddWall GetTex("Button11"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, 1, 1
    
    Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(3).AddWall GetTex("Button3"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(4).AddWall GetTex("Button4"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(5).AddWall GetTex("Button5"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    Buttons(6).AddWall GetTex("Button6"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, 1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(81).AddWall GetTex("Button81"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, 1, 1
    'Buttons(83).AddWall GetTex("Button83"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    'Buttons(84).AddWall GetTex("Button84"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, 1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(39).AddWall GetTex("Button39"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(65).AddWall GetTex("Button65"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    'Buttons(79).AddWall GetTex("Button79"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, 1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), -(ShaftLeft - 0.17), -18.45 + (15 * 4), -(ShaftLeft - 0.17), -18.15 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.4, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 0.8, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.2, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), -(ShaftLeft - 0.17), -18.45 + (15 * 4) + 1.6, -(ShaftLeft - 0.17), -18.15 + (15 * 4) + 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, 1, 1

End If

'Elevator 10
If Number = 40 Then

    'Buttons(135).AddWall GetTex("Button135"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(1).AddWall GetTex("Button1"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(136).AddWall GetTex("Button136"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(137).AddWall GetTex("Button137"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    'Buttons(138).AddWall GetTex("ButtonR"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12), -1, 1
    
    'Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    Buttons(37).AddWall GetTex("Button37"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(38).AddWall GetTex("Button38"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 0.5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 0.5, -1, 1
    
    Buttons(32).AddWall GetTex("Button32"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(33).AddWall GetTex("Button33"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(34).AddWall GetTex("Button34"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(35).AddWall GetTex("Button35"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    Buttons(36).AddWall GetTex("Button36"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1, -1, 1
    
    Buttons(27).AddWall GetTex("Button27"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(28).AddWall GetTex("Button28"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(29).AddWall GetTex("Button29"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(30).AddWall GetTex("Button30"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    Buttons(31).AddWall GetTex("Button31"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 1.5, -1, 1
    
    Buttons(22).AddWall GetTex("Button22"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(23).AddWall GetTex("Button23"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(24).AddWall GetTex("Button24"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(25).AddWall GetTex("Button25"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    Buttons(26).AddWall GetTex("Button26"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2, -1, 1
    
    Buttons(17).AddWall GetTex("Button17"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(18).AddWall GetTex("Button18"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(19).AddWall GetTex("Button19"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(20).AddWall GetTex("Button20"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    Buttons(21).AddWall GetTex("Button21"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 2.5, -1, 1
    
    Buttons(12).AddWall GetTex("Button12"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(13).AddWall GetTex("Button13"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(14).AddWall GetTex("Button14"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(15).AddWall GetTex("Button15"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    Buttons(16).AddWall GetTex("Button16"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3, -1, 1
    
    Buttons(7).AddWall GetTex("Button7"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(8).AddWall GetTex("Button8"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(9).AddWall GetTex("Button9"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(10).AddWall GetTex("Button10"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    Buttons(11).AddWall GetTex("Button11"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 3.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(3).AddWall GetTex("Button3"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(4).AddWall GetTex("Button4"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(5).AddWall GetTex("Button5"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    Buttons(6).AddWall GetTex("Button6"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4, -1, 1
    
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(81).AddWall GetTex("Button81"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 4.5, -1, 1
    'Buttons(83).AddWall GetTex("Button83"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    'Buttons(84).AddWall GetTex("Button84"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 4.5, -1, 1
    
    'Buttons(2).AddWall GetTex("Button2"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(39).AddWall GetTex("Button39"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(-1).AddWall GetTex("ButtonL"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(65).AddWall GetTex("Button65"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    'Buttons(79).AddWall GetTex("Button79"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.y + 12) - 5, -1, 1
    
    Buttons(139).AddWall GetTex("ButtonOpen"), (ShaftLeft - 0.17), -27.85 + (15 * 4), (ShaftLeft - 0.17), -27.55 + (15 * 4), 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(140).AddWall GetTex("ButtonClose"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.4, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.4, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(141).AddWall GetTex("ButtonCancel"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 0.8, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 0.8, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(142).AddWall GetTex("ButtonStop"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.2, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.2, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1
    Buttons(143).AddWall GetTex("ButtonAlarm"), (ShaftLeft - 0.17), -27.85 + (15 * 4) - 1.6, (ShaftLeft - 0.17), -27.55 + (15 * 4) - 1.6, 0.3, (Elevator(Number).GetPosition.Y + 12) - 5.5, -1, 1

End If
   
End Sub

Sub ElevatorLoop(Number As Single)
On Error Resume Next

If ElevatorEnable(Number) = 0 And GotoFloor(Number) = 0 And OpenElevator(Number) = 0 And FineTune(Number) = False Then Exit Sub

elevatorstart(Number) = Elevator(Number).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(Number) = (Elevator(Number).GetPosition.Y - FloorHeight) / FloorHeight
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(Number) < 1 Then ElevatorFloor(Number) = 1
      
      If GotoFloor(Number) = ElevatorFloor(Number) - 1 Then CurrentFloor(Number) = ElevatorFloor(Number)

      'If GotoFloor(Number) <> 0 And GotoFloor(Number) > CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z <= 0 Then
      If GotoFloor(Number) <> 0 And GotoFloor(Number) > CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorDoorL(Number).GetPosition.z <= 0 Then
      ElevatorDirection(Number) = 1
      OriginalLocation(Number) = CurrentFloorExact(Number)
      DistanceToTravel(Number) = ((GotoFloor(Number) * FloorHeight) + FloorHeight) - ((CurrentFloorExact(Number) * FloorHeight) + FloorHeight)
      If ElevatorSync(Number) = True Then
      Room(CameraFloor).Enable False
      For ElevTemp(Number) = 1 To 40
      CallButtons(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable False
      ElevatorDoorR(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(Number) <> 0 And GotoFloor(Number) < CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z <= 0 Then
      If GotoFloor(Number) <> 0 And GotoFloor(Number) < CurrentFloor(Number) And ElevatorDirection(Number) = 0 And ElevatorDoorL(Number).GetPosition.z <= 0 Then
      Elevator(Number).Enable True
      ElevatorDirection(Number) = -1
      OriginalLocation(Number) = CurrentFloorExact(Number)
      DistanceToTravel(Number) = ((CurrentFloorExact(Number) * FloorHeight) + FloorHeight) - ((GotoFloor(Number) * FloorHeight) + FloorHeight)
      If ElevatorSync(Number) = True Then
      Room(CameraFloor).Enable False
      For ElevTemp(Number) = 1 To 40
      CallButtons(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable False
      ElevatorDoorR(ElevTemp(Number)).Enable False
      Next ElevTemp(Number)
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(Number) = Int((elevatorstart(Number).Y - FloorHeight) / FloorHeight)
       CurrentFloorExact(Number) = (elevatorstart(Number).Y - FloorHeight) / FloorHeight
       'CurrentFloor(Number) = (Elevator(Number).GetPosition.y / FloorHeight) - 1
       'CurrentFloorExact(Number) = Int((Elevator(Number).GetPosition.y / FloorHeight) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(Number)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(Number)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(Number) / 5)
        
        If ElevatorEnable(Number) >= 0 And ElevatorDirection(Number) = 1 Then
        'sound
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(Number) = 0 And GotoFloor(Number) <> ElevatorFloor(Number) Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevstart.wav"
        ElevatorSounds(Number).Volume = 0
        'ElevatorSounds(Number).maxDistance = 100
        'Call ElevatorSounds(Number).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(Number).ConeOutsideVolume = 0
        'Call ElevatorSounds(Number).SetPosition(-20.25, Elevator1(Number).GetPosition.Y + 20, -23)
        ElevatorSounds(Number).Play
        ElevatorCheck(Number) = 1
        End If
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(Number) = 1 Then
        ElevatorSounds(Number).Load App.Path + "\elevmove.wav"
        ElevatorSounds(Number).Loop_ = True
        ElevatorSounds(Number).Play
        End If
        'movement
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        'ElevatorSounds(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ''ElevatorMusic(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ElevatorEnable(Number) = ElevatorEnable(Number) + 0.25
        If ElevatorEnable(Number) <= 15 Then StoppingDistance(Number) = CurrentFloorExact(Number) - OriginalLocation(Number) + 0.4
        If ElevatorEnable(Number) > 15 Then ElevatorEnable(Number) = 15
        Destination(Number) = ((OriginalLocation(Number) * FloorHeight) + FloorHeight) + DistanceToTravel(Number) - 35
        If GotoFloor(Number) <> 0 And elevatorstart(Number).Y >= (Destination(Number) - (StoppingDistance(Number) * FloorHeight) + FloorHeight) Then ElevatorDirection(Number) = -1: ElevatorCheck(Number) = 0
        End If
      
        If ElevatorEnable(Number) > 0 And ElevatorDirection(Number) = -1 Then
        'Sounds
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Stop_
        End If
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevstop.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck2(Number) = 1
        End If
        'Movement
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        
        'ElevatorSounds(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ''ElevatorMusic(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ElevatorEnable(Number) = ElevatorEnable(Number) - 0.25
        If ElevatorEnable(Number) < 0 Then ElevatorEnable(Number) = 0
        If ElevatorEnable(Number) = 0 Then ElevatorDirection(Number) = 0
        If GotoFloor(Number) <> 0 Then ElevatorCheck(Number) = 0: FineTune(Number) = True
        End If
      
        If ElevatorEnable(Number) <= 0 And ElevatorDirection(Number) = -1 Then
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(Number) = 0 Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevstart.wav"
        ElevatorSounds(Number).Volume = 0
        'ElevatorSounds(Number).maxDistance = 100
        'Call ElevatorSounds(Number).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(Number).ConeOutsideVolume = 0
        'Call ElevatorSounds(Number).SetPosition(-20.25, Elevator(Number).GetPosition.Y + 20, -23)
        ElevatorSounds(Number).Play
        ElevatorCheck(Number) = 1
        End If
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(Number) = 1 Then
        ElevatorSounds(Number).Load App.Path + "\elevmove.wav"
        ElevatorSounds(Number).Loop_ = True
        ElevatorSounds(Number).Play
        End If
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        'ElevatorSounds(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ''ElevatorMusic(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ElevatorEnable(Number) = ElevatorEnable(Number) - 0.25
        If ElevatorEnable(Number) >= -15 Then StoppingDistance(Number) = OriginalLocation(Number) - CurrentFloorExact(Number)
        If ElevatorEnable(Number) < -15 Then ElevatorEnable(Number) = -15
        Destination(Number) = ((OriginalLocation(Number) * FloorHeight) + FloorHeight) - DistanceToTravel(Number)
        If GotoFloor(Number) <> 0 And elevatorstart(Number).Y <= (Destination(Number) + (StoppingDistance(Number) * FloorHeight) + FloorHeight) Then ElevatorDirection(Number) = 1: ElevatorCheck(Number) = 0
        End If
      
        If ElevatorEnable(Number) < 0 And ElevatorDirection(Number) = 1 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Stop_
        End If
        If ElevatorSounds(Number).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(Number) = 0 And FineTune(Number) = False Then
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevstop.wav"
        ElevatorSounds(Number).Play
        ElevatorCheck2(Number) = 1
        End If
        Elevator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorL(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        ElevatorInsDoorR(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        FloorIndicator(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Plaque(Number).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        Next ElevTemp(Number)
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, (ElevatorEnable(Number) / ElevatorSpeed), 0
        'ElevatorSounds(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ''ElevatorMusic(Number).SetPosition -20.25, Elevator(Number).GetPosition.Y + 20, -23
        ElevatorEnable(Number) = ElevatorEnable(Number) + 0.25
        If ElevatorEnable(Number) > 0 Then ElevatorEnable(Number) = 0
        If ElevatorEnable(Number) = 0 Then ElevatorDirection(Number) = 0
        If GotoFloor(Number) <> 0 Then ElevatorCheck(Number) = 0: FineTune(Number) = True
        End If
      
      If FineTune(Number) = True And ElevatorEnable(Number) = 0 And elevatorstart(Number).Y > (GotoFloor(Number) * FloorHeight) + FloorHeight + -0.3 And elevatorstart(Number).Y < (GotoFloor(Number) * FloorHeight) + FloorHeight + 0.3 Then
      FineTune(Number) = False
      Room(CameraFloor).Enable True
      If ElevatorSync(Number) = True Then
      For ElevTemp(Number) = 1 To 40
      ElevatorDoorL(ElevTemp(Number)).Enable True
      ElevatorDoorR(ElevTemp(Number)).Enable True
      Next ElevTemp(Number)
      For ElevTemp(Number) = 1 To 40
      CallButtons(ElevTemp(Number)).Enable True
      Next ElevTemp(Number)
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      Call InitRealtime(CameraFloor)
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(Number) = 0
      OpenElevator(Number) = 1
      ElevatorCheck(Number) = 0
      ElevatorCheck2(Number) = 0
      ElevatorCheck3(Number) = 0
      ElevatorCheck4(Number) = 0
      If CameraFloor > 1 And ElevatorSync(Number) = True Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * FloorHeight) + FloorHeight + 10, Camera.GetPosition.z
      If CameraFloor = 1 And ElevatorSync(Number) = True And FloorIndicatorText(Number) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(Number) = True Then
      If ElevatorCheck3(Number) = 0 Then
      ElevatorSounds(Number).Load App.Path + "\ding1.wav"
      ElevatorSounds(Number).Play
      ElevatorCheck3(Number) = 1
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).SetColor RGBA(1, 1, 1, 1)
        Next ElevTemp(Number)
      End If
      If elevatorstart(Number).Y < (GotoFloor(Number) * FloorHeight) + FloorHeight Then
      Elevator(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      ElevatorInsDoorL(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      ElevatorInsDoorR(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
      Plaque(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, ElevatorFineTuneSpeed, 0
        Next ElevTemp(Number)
        FloorIndicator(Number).MoveRelative 0, ElevatorFineTuneSpeed, 0
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, ElevatorFineTuneSpeed, 0
      End If
      If elevatorstart(Number).Y > (GotoFloor(Number) * FloorHeight) + FloorHeight Then
      Elevator(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      ElevatorInsDoorL(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      ElevatorInsDoorR(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
      Plaque(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        For ElevTemp(Number) = -1 To 144
        If ButtonsEnabled = True And ElevatorSync(Number) = True Then Buttons(ElevTemp(Number)).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        Next ElevTemp(Number)
        FloorIndicator(Number).MoveRelative 0, -ElevatorFineTuneSpeed, 0
        If ElevatorSync(Number) = True Then Camera.MoveRelative 0, -ElevatorFineTuneSpeed, 0
      End If
      End If
      
      If OpenElevator(Number) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z >= 4 Then OpenElevator(Number) = 0: GoTo OpenElevator1
      If ElevatorDoorL(Number).GetPosition.z >= 4 Then OpenElevator(Number) = 0: GoTo OpenElevator1
      If ElevatorCheck4(Number) = 0 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(Number).Stop_
        End If
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(Number).Volume = 0
        'ElevatorSounds(Number).maxDistance = 1000
        'Call ElevatorSounds(Number).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(Number).ConeOutsideVolume = 0
        'Call ElevatorSounds(Number).SetPosition(-20.25, Elevator(Number).GetPosition.Y, -23)
        ElevatorSounds(Number).Play
        ElevatorCheck4(Number) = 1
      End If
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) + 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z > 1 Then OpenElevator(Number) = 2
      If ElevatorDoorL(Number).GetPosition.z > 1 Then OpenElevator(Number) = 2
OpenElevator1:
      End If
      
      If OpenElevator(Number) = 2 Then
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z > 3 Then OpenElevator(Number) = 3
      If ElevatorDoorL(Number).GetPosition.z > 3 Then OpenElevator(Number) = 3
      End If
      
      If OpenElevator(Number) = 3 Then
      ElevatorCheck4(Number) = 0
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z < 7 And OpenElevatorLoc(Number) = 0 Then OpenElevatorLoc(Number) = 0.02
      If ElevatorDoorL(Number).GetPosition.z < 7 And OpenElevatorLoc(Number) = 0 Then OpenElevatorLoc(Number) = 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      If OpenElevatorLoc(Number) <= 0 Then
      OpenElevator(Number) = 0
      OpenElevatorLoc(Number) = 0
      If Number = 1 Then Form1.Timer1.Enabled = True
      If Number = 2 Then Form1.Timer2.Enabled = True
      If Number = 3 Then Form1.Timer3.Enabled = True
      If Number = 4 Then Form1.Timer4.Enabled = True
      If Number = 5 Then Form1.Timer5.Enabled = True
      If Number = 6 Then Form1.Timer6.Enabled = True
      If Number = 7 Then Form1.Timer7.Enabled = True
      If Number = 8 Then Form1.Timer8.Enabled = True
      If Number = 9 Then Form1.Timer9.Enabled = True
      If Number = 10 Then Form1.Timer10.Enabled = True
      If Number = 11 Then Form1.Timer11.Enabled = True
      If Number = 12 Then Form1.Timer12.Enabled = True
      If Number = 13 Then Form1.Timer13.Enabled = True
      If Number = 14 Then Form1.Timer14.Enabled = True
      If Number = 15 Then Form1.Timer15.Enabled = True
      If Number = 16 Then Form1.Timer16.Enabled = True
      If Number = 17 Then Form1.Timer17.Enabled = True
      If Number = 18 Then Form1.Timer18.Enabled = True
      If Number = 19 Then Form1.Timer19.Enabled = True
      If Number = 20 Then Form1.Timer20.Enabled = True
      If Number = 21 Then Form1.Timer21.Enabled = True
      If Number = 22 Then Form1.Timer22.Enabled = True
      If Number = 23 Then Form1.Timer23.Enabled = True
      If Number = 24 Then Form1.Timer24.Enabled = True
      If Number = 25 Then Form1.Timer25.Enabled = True
      If Number = 26 Then Form1.Timer26.Enabled = True
      If Number = 27 Then Form1.Timer27.Enabled = True
      If Number = 28 Then Form1.Timer28.Enabled = True
      If Number = 29 Then Form1.Timer29.Enabled = True
      If Number = 30 Then Form1.Timer30.Enabled = True
      If Number = 31 Then Form1.Timer31.Enabled = True
      If Number = 32 Then Form1.Timer32.Enabled = True
      If Number = 33 Then Form1.Timer33.Enabled = True
      If Number = 34 Then Form1.Timer34.Enabled = True
      If Number = 35 Then Form1.Timer35.Enabled = True
      If Number = 36 Then Form1.Timer36.Enabled = True
      If Number = 37 Then Form1.Timer37.Enabled = True
      If Number = 38 Then Form1.Timer38.Enabled = True
      If Number = 39 Then Form1.Timer39.Enabled = True
      If Number = 40 Then Form1.Timer40.Enabled = True
      End If
      End If
      
      If OpenElevator(Number) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z <= 0 Then OpenElevator(Number) = 0: GoTo OpenElevator2
      If ElevatorDoorL(Number).GetPosition.z <= 0 Then OpenElevator(Number) = 0: GoTo OpenElevator2
      If ElevatorCheck4(Number) = 0 Then
        If ElevatorSounds(Number).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(Number).Stop_
        End If
        ElevatorSounds(Number).Loop_ = False
        ElevatorSounds(Number).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(Number).Volume = 0
        'ElevatorSounds(Number).maxDistance = 1000
        'Call ElevatorSounds(Number).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(Number).ConeOutsideVolume = 0
        'Call ElevatorSounds(Number).SetPosition(-20.25, Elevator(Number).GetPosition.Y, -23)
        ElevatorSounds(Number).Play
        ElevatorCheck4(Number) = 1
      End If
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) - 0.02
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z < 3 Then OpenElevator(Number) = -2
      If ElevatorDoorL(Number).GetPosition.z < 3 Then OpenElevator(Number) = -2
OpenElevator2:
      End If
      
      If OpenElevator(Number) = -2 Then
      ElevatorDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorL(Number).MoveRelative OpenElevatorLoc(Number), 0, 0
      ElevatorInsDoorR(Number).MoveRelative -OpenElevatorLoc(Number), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z < 1 Then OpenElevator(Number) = -3
      If ElevatorDoorL(Number).GetPosition.z < 1 Then OpenElevator(Number) = -3
      End If
      
      If OpenElevator(Number) = -3 Then
      ElevatorCheck4(Number) = 0
      OpenElevatorLoc(Number) = OpenElevatorLoc(Number) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(Number)).GetPosition.z > 0 And OpenElevatorLoc(Number) >= 0 Then OpenElevatorLoc(Number) = -0.02
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
      
    
End Sub

