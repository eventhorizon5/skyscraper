Attribute VB_Name = "ElevatorCode"
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

'This module contains the code for the 10 elevators (all 10 routines are almost
'completely identical.  The code was moved from the CoreRoutines module during
'the transition from a single to multi elevator program
'10 timers, 1 for each elevator, call the code in each of these modules
'in order for the 10 elevators to be independent and run simultaneously

'note - any changes in here must be made to all 10 subs

Option Explicit

Dim i1 As Single
Dim i2 As Single
Dim i3 As Single
Dim i4 As Single
Dim i5 As Single
Dim i6 As Single
Dim i7 As Single
Dim i8 As Single
Dim i9 As Single
Dim i10 As Single

Sub Elevator1Loop()

elevatorstart(1) = Elevator(1).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(1) = (Elevator(1).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(1) < 1 Then ElevatorFloor(1) = 1
      
      If GotoFloor(1) = ElevatorFloor(1) - 1 Then CurrentFloor(1) = ElevatorFloor(1)

      'If GotoFloor(1) <> 0 And GotoFloor(1) > CurrentFloor(1) And ElevatorDirection(1) = 0 And ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z <= 0 Then
      If GotoFloor(1) <> 0 And GotoFloor(1) > CurrentFloor(1) And ElevatorDirection(1) = 0 And ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z <= 0 Then
      ElevatorDirection(1) = 1
      OriginalLocation(1) = CurrentFloorExact(1)
      DistanceToTravel(1) = ((GotoFloor(1) * 25) + 25) - ((CurrentFloorExact(1) * 25) + 25)
      If ElevatorSync(1) = True Then
      Room(CameraFloor).Enable False
      For i1 = 1 To 10
      CallButtons(i1).Enable False
      Next i1
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(1) <> 0 And GotoFloor(1) < CurrentFloor(1) And ElevatorDirection(1) = 0 And ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z <= 0 Then
      If GotoFloor(1) <> 0 And GotoFloor(1) < CurrentFloor(1) And ElevatorDirection(1) = 0 And ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z <= 0 Then
      Elevator(1).Enable True
      ElevatorDirection(1) = -1
      OriginalLocation(1) = CurrentFloorExact(1)
      DistanceToTravel(1) = ((CurrentFloorExact(1) * 25) + 25) - ((GotoFloor(1) * 25) + 25)
      If ElevatorSync(1) = True Then
      Room(CameraFloor).Enable False
      For i1 = 1 To 10
      CallButtons(i1).Enable False
      Next i1
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(1) = Int((elevatorstart(1).Y - 25) / 25)
       CurrentFloorExact(1) = (elevatorstart(1).Y - 25) / 25
       'CurrentFloor(1) = (Elevator(1).GetPosition.y / 25) - 1
       'CurrentFloorExact(1) = Int((Elevator(1).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(1)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(1)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(1) / 5)
        
        If ElevatorEnable(1) >= 0 And ElevatorDirection(1) = 1 Then
        'sound
        If ElevatorSounds(1).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(1) = 0 And GotoFloor(1) <> ElevatorFloor(1) Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevstart.wav"
        ElevatorSounds(1).Volume = 0
        'ElevatorSounds(1).maxDistance = 100
        'Call ElevatorSounds(1).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(1).ConeOutsideVolume = 0
        'Call ElevatorSounds(1).SetPosition(-20.25, Elevator1(1).GetPosition.Y + 20, -23)
        ElevatorSounds(1).Play
        ElevatorCheck(1) = 1
        End If
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(1) = 1 Then
        ElevatorSounds(1).Load App.Path + "\elevmove.wav"
        ElevatorSounds(1).Loop_ = True
        ElevatorSounds(1).Play
        End If
        'movement
        Elevator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorL(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorR(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        FloorIndicator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Plaque(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Next i1
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, (ElevatorEnable(1) / 5), 0
        'ElevatorSounds(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ''ElevatorMusic(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ElevatorEnable(1) = ElevatorEnable(1) + 0.25
        If ElevatorEnable(1) <= 15 Then StoppingDistance(1) = CurrentFloorExact(1) - OriginalLocation(1) + 0.4
        If ElevatorEnable(1) > 15 Then ElevatorEnable(1) = 15
        Destination(1) = ((OriginalLocation(1) * 25) + 25) + DistanceToTravel(1) - 35
        If GotoFloor(1) <> 0 And elevatorstart(1).Y >= (Destination(1) - (StoppingDistance(1) * 25) + 25) Then ElevatorDirection(1) = -1: ElevatorCheck(1) = 0
        End If
      
        If ElevatorEnable(1) > 0 And ElevatorDirection(1) = -1 Then
        'Sounds
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(1) = 0 And FineTune(1) = False Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Stop_
        End If
        If ElevatorSounds(1).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(1) = 0 And FineTune(1) = False Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevstop.wav"
        ElevatorSounds(1).Play
        ElevatorCheck2(1) = 1
        End If
        'Movement
        Elevator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorL(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorR(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        FloorIndicator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Plaque(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Next i1
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, (ElevatorEnable(1) / 5), 0
        
        'ElevatorSounds(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ''ElevatorMusic(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ElevatorEnable(1) = ElevatorEnable(1) - 0.25
        If ElevatorEnable(1) < 0 Then ElevatorEnable(1) = 0
        If ElevatorEnable(1) = 0 Then ElevatorDirection(1) = 0
        If GotoFloor(1) <> 0 Then ElevatorCheck(1) = 0: FineTune(1) = True
        End If
      
        If ElevatorEnable(1) <= 0 And ElevatorDirection(1) = -1 Then
        If ElevatorSounds(1).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(1) = 0 Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevstart.wav"
        ElevatorSounds(1).Volume = 0
        'ElevatorSounds(1).maxDistance = 100
        'Call ElevatorSounds(1).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(1).ConeOutsideVolume = 0
        'Call ElevatorSounds(1).SetPosition(-20.25, Elevator(1).GetPosition.Y + 20, -23)
        ElevatorSounds(1).Play
        ElevatorCheck(1) = 1
        End If
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(1) = 1 Then
        ElevatorSounds(1).Load App.Path + "\elevmove.wav"
        ElevatorSounds(1).Loop_ = True
        ElevatorSounds(1).Play
        End If
        Elevator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorL(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorR(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        FloorIndicator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Plaque(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Next i1
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, (ElevatorEnable(1) / 5), 0
        'ElevatorSounds(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ''ElevatorMusic(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ElevatorEnable(1) = ElevatorEnable(1) - 0.25
        If ElevatorEnable(1) >= -15 Then StoppingDistance(1) = OriginalLocation(1) - CurrentFloorExact(1)
        If ElevatorEnable(1) < -15 Then ElevatorEnable(1) = -15
        Destination(1) = ((OriginalLocation(1) * 25) + 25) - DistanceToTravel(1)
        If GotoFloor(1) <> 0 And elevatorstart(1).Y <= (Destination(1) + (StoppingDistance(1) * 25) + 25) Then ElevatorDirection(1) = 1: ElevatorCheck(1) = 0
        End If
      
        If ElevatorEnable(1) < 0 And ElevatorDirection(1) = 1 Then
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(1) = 0 And FineTune(1) = False Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Stop_
        End If
        If ElevatorSounds(1).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(1) = 0 And FineTune(1) = False Then
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevstop.wav"
        ElevatorSounds(1).Play
        ElevatorCheck2(1) = 1
        End If
        Elevator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorL(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        ElevatorInsDoorR(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        FloorIndicator(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Plaque(1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, (ElevatorEnable(1) / 5), 0
        Next i1
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, (ElevatorEnable(1) / 5), 0
        'ElevatorSounds(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ''ElevatorMusic(1).SetPosition -20.25, Elevator(1).GetPosition.Y + 20, -23
        ElevatorEnable(1) = ElevatorEnable(1) + 0.25
        If ElevatorEnable(1) > 0 Then ElevatorEnable(1) = 0
        If ElevatorEnable(1) = 0 Then ElevatorDirection(1) = 0
        If GotoFloor(1) <> 0 Then ElevatorCheck(1) = 0: FineTune(1) = True
        End If
      
      If FineTune(1) = True And ElevatorEnable(1) = 0 And elevatorstart(1).Y > (GotoFloor(1) * 25) + 25 + -0.3 And elevatorstart(1).Y < (GotoFloor(1) * 25) + 25 + 0.3 Then
      FineTune(1) = False
      Room(CameraFloor).Enable True
      If ElevatorSync(1) = True Then
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
      For i1 = 1 To 10
      CallButtons(i1).Enable True
      Next i1
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(1) = 0
      OpenElevator(1) = 1
      ElevatorCheck(1) = 0
      ElevatorCheck2(1) = 0
      ElevatorCheck3(1) = 0
      ElevatorCheck4(1) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(1) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(1) = True Then
      If ElevatorCheck3(1) = 0 Then
      ElevatorSounds(1).Load App.Path + "\ding1.wav"
      ElevatorSounds(1).Play
      ElevatorCheck3(1) = 1
        For i1 = -1 To 138
        Buttons1(i1).SetColor RGBA(1, 1, 1, 1)
        Next i1
      End If
      If elevatorstart(1).Y < (GotoFloor(1) * 25) + 25 Then
      Elevator(1).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(1).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(1).MoveRelative 0, 0.2, 0
      Plaque(1).MoveRelative 0, 0.2, 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, 0.2, 0
        Next i1
        FloorIndicator(1).MoveRelative 0, 0.2, 0
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(1).Y > (GotoFloor(1) * 25) + 25 Then
      Elevator(1).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(1).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(1).MoveRelative 0, -0.2, 0
      Plaque(1).MoveRelative 0, -0.2, 0
        For i1 = -1 To 138
        Buttons1(i1).MoveRelative 0, -0.2, 0
        Next i1
        FloorIndicator(1).MoveRelative 0, -0.2, 0
        If ElevatorSync(1) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(1) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z >= 4 Then OpenElevator(1) = 0: GoTo OpenElevator1
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z >= 4 Then OpenElevator(1) = 0: GoTo OpenElevator1
      If ElevatorCheck4(1) = 0 Then
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(1).Stop_
        End If
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(1).Volume = 0
        'ElevatorSounds(1).maxDistance = 1000
        'Call ElevatorSounds(1).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(1).ConeOutsideVolume = 0
        'Call ElevatorSounds(1).SetPosition(-20.25, Elevator(1).GetPosition.Y, -23)
        ElevatorSounds(1).Play
        ElevatorCheck4(1) = 1
      End If
      OpenElevatorLoc(1) = OpenElevatorLoc(1) + 0.02
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z > 1 Then OpenElevator(1) = 2
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z > 1 Then OpenElevator(1) = 2
OpenElevator1:
      End If
      
      If OpenElevator(1) = 2 Then
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z > 3 Then OpenElevator(1) = 3
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z > 3 Then OpenElevator(1) = 3
      End If
      
      If OpenElevator(1) = 3 Then
      ElevatorCheck4(1) = 0
      OpenElevatorLoc(1) = OpenElevatorLoc(1) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z < 7 And OpenElevatorLoc(1) = 0 Then OpenElevatorLoc(1) = 0.02
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z < 7 And OpenElevatorLoc(1) = 0 Then OpenElevatorLoc(1) = 0.02
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      If OpenElevatorLoc(1) <= 0 Then
      OpenElevator(1) = 0
      OpenElevatorLoc(1) = 0
      Form1.Timer1.Enabled = True
      End If
      End If
      
      If OpenElevator(1) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z <= 0 Then OpenElevator(1) = 0: GoTo OpenElevator2
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z <= 0 Then OpenElevator(1) = 0: GoTo OpenElevator2
      If ElevatorCheck4(1) = 0 Then
        If ElevatorSounds(1).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(1).Stop_
        End If
        ElevatorSounds(1).Loop_ = False
        ElevatorSounds(1).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(1).Volume = 0
        'ElevatorSounds(1).maxDistance = 1000
        'Call ElevatorSounds(1).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(1).ConeOutsideVolume = 0
        'Call ElevatorSounds(1).SetPosition(-20.25, Elevator(1).GetPosition.Y, -23)
        ElevatorSounds(1).Play
        ElevatorCheck4(1) = 1
      End If
      OpenElevatorLoc(1) = OpenElevatorLoc(1) - 0.02
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z < 3 Then OpenElevator(1) = -2
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z < 3 Then OpenElevator(1) = -2
OpenElevator2:
      End If
      
      If OpenElevator(1) = -2 Then
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z < 1 Then OpenElevator(1) = -3
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z < 1 Then OpenElevator(1) = -3
      End If
      
      If OpenElevator(1) = -3 Then
      ElevatorCheck4(1) = 0
      OpenElevatorLoc(1) = OpenElevatorLoc(1) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(1)).GetPosition.z > 0 And OpenElevatorLoc(1) >= 0 Then OpenElevatorLoc(1) = -0.02
      If ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.z > 0 And OpenElevatorLoc(1) >= 0 Then OpenElevatorLoc(1) = -0.02
      ElevatorDoor1L(ElevatorFloor2(1)).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorDoor1R(ElevatorFloor2(1)).MoveRelative -OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorL(1).MoveRelative OpenElevatorLoc(1), 0, 0
      ElevatorInsDoorR(1).MoveRelative -OpenElevatorLoc(1), 0, 0
      If OpenElevatorLoc(1) >= 0 Then
      OpenElevator(1) = 0
      OpenElevatorLoc(1) = 0
      ElevatorInsDoorL(1).SetPosition ElevatorInsDoorL(1).GetPosition.X, ElevatorInsDoorL(1).GetPosition.Y, 0
      ElevatorInsDoorR(1).SetPosition ElevatorInsDoorR(1).GetPosition.X, ElevatorInsDoorR(1).GetPosition.Y, 0
      ElevatorDoor1L(ElevatorFloor2(1)).SetPosition ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.X, ElevatorDoor1L(ElevatorFloor2(1)).GetPosition.Y, 0
      ElevatorDoor1R(ElevatorFloor2(1)).SetPosition ElevatorDoor1R(ElevatorFloor2(1)).GetPosition.X, ElevatorDoor1R(ElevatorFloor2(1)).GetPosition.Y, 0
      End If
      End If
      
    
End Sub

Sub Elevator2Loop()

elevatorstart(2) = Elevator(2).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(2) = (Elevator(2).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(2) < 1 Then ElevatorFloor(2) = 1
      
      If GotoFloor(2) = ElevatorFloor(2) - 1 Then CurrentFloor(2) = ElevatorFloor(2)

      'If GotoFloor(2) <> 0 And GotoFloor(2) > CurrentFloor(2) And ElevatorDirection(2) = 0 And ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z <= 0 Then
      If GotoFloor(2) <> 0 And GotoFloor(2) > CurrentFloor(2) And ElevatorDirection(2) = 0 And ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z <= 0 Then
      ElevatorDirection(2) = 1
      OriginalLocation(2) = CurrentFloorExact(2)
      DistanceToTravel(2) = ((GotoFloor(2) * 25) + 25) - ((CurrentFloorExact(2) * 25) + 25)
      If ElevatorSync(2) = True Then
      Room(CameraFloor).Enable False
      For i2 = 1 To 10
      CallButtons(i2).Enable False
      Next i2
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(2) <> 0 And GotoFloor(2) < CurrentFloor(2) And ElevatorDirection(2) = 0 And ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z <= 0 Then
      If GotoFloor(2) <> 0 And GotoFloor(2) < CurrentFloor(2) And ElevatorDirection(2) = 0 And ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z <= 0 Then
      ElevatorDirection(2) = -1
      OriginalLocation(2) = CurrentFloorExact(2)
      DistanceToTravel(2) = ((CurrentFloorExact(2) * 25) + 25) - ((GotoFloor(2) * 25) + 25)
      If ElevatorSync(2) = True Then
      Room(CameraFloor).Enable False
      For i2 = 1 To 10
      CallButtons(i2).Enable False
      Next i2
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(2) = Int((elevatorstart(2).Y - 25) / 25)
       CurrentFloorExact(2) = (elevatorstart(2).Y - 25) / 25
       'CurrentFloor(2) = (Elevator(2).GetPosition.y / 25) - 1
       'CurrentFloorExact(2) = Int((Elevator(2).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(2)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(2)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(2) / 5)
        
        If ElevatorEnable(2) >= 0 And ElevatorDirection(2) = 1 Then
        'sound
        If ElevatorSounds(2).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(2) = 0 And GotoFloor(2) <> ElevatorFloor(2) Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevstart.wav"
        ElevatorSounds(2).Volume = 0
        'ElevatorSounds(2).maxDistance = 100
        'Call ElevatorSounds(2).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(2).ConeOutsideVolume = 0
        'Call ElevatorSounds(2).SetPosition(-20.25, Elevator1(2).GetPosition.Y + 20, -23)
        ElevatorSounds(2).Play
        ElevatorCheck(2) = 1
        End If
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(2) = 1 Then
        ElevatorSounds(2).Load App.Path + "\elevmove.wav"
        ElevatorSounds(2).Loop_ = True
        ElevatorSounds(2).Play
        End If
        'movement
        Elevator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorL(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorR(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        FloorIndicator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Plaque(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Next i2
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, (ElevatorEnable(2) / 5), 0
        'ElevatorSounds(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ''ElevatorMusic(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ElevatorEnable(2) = ElevatorEnable(2) + 0.25
        If ElevatorEnable(2) <= 15 Then StoppingDistance(2) = CurrentFloorExact(2) - OriginalLocation(2) + 0.4
        If ElevatorEnable(2) > 15 Then ElevatorEnable(2) = 15
        Destination(2) = ((OriginalLocation(2) * 25) + 25) + DistanceToTravel(2) - 35
        If GotoFloor(2) <> 0 And elevatorstart(2).Y >= (Destination(2) - (StoppingDistance(2) * 25) + 25) Then ElevatorDirection(2) = -1: ElevatorCheck(2) = 0
        End If
      
        If ElevatorEnable(2) > 0 And ElevatorDirection(2) = -1 Then
        'Sounds
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(2) = 0 And FineTune(2) = False Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Stop_
        End If
        If ElevatorSounds(2).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(2) = 0 And FineTune(2) = False Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevstop.wav"
        ElevatorSounds(2).Play
        ElevatorCheck2(2) = 1
        End If
        'Movement
        Elevator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorL(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorR(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        FloorIndicator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Plaque(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Next i2
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, (ElevatorEnable(2) / 5), 0
        
        'ElevatorSounds(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ''ElevatorMusic(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ElevatorEnable(2) = ElevatorEnable(2) - 0.25
        If ElevatorEnable(2) < 0 Then ElevatorEnable(2) = 0
        If ElevatorEnable(2) = 0 Then ElevatorDirection(2) = 0
        If GotoFloor(2) <> 0 Then ElevatorCheck(2) = 0: FineTune(2) = True
        End If
      
        If ElevatorEnable(2) <= 0 And ElevatorDirection(2) = -1 Then
        If ElevatorSounds(2).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(2) = 0 Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevstart.wav"
        ElevatorSounds(2).Volume = 0
        'ElevatorSounds(2).maxDistance = 100
        'Call ElevatorSounds(2).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(2).ConeOutsideVolume = 0
        'Call ElevatorSounds(2).SetPosition(-20.25, Elevator(2).GetPosition.Y + 20, -23)
        ElevatorSounds(2).Play
        ElevatorCheck(2) = 1
        End If
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(2) = 1 Then
        ElevatorSounds(2).Load App.Path + "\elevmove.wav"
        ElevatorSounds(2).Loop_ = True
        ElevatorSounds(2).Play
        End If
        Elevator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorL(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorR(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        FloorIndicator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Plaque(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Next i2
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, (ElevatorEnable(2) / 5), 0
        'ElevatorSounds(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ''ElevatorMusic(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ElevatorEnable(2) = ElevatorEnable(2) - 0.25
        If ElevatorEnable(2) >= -15 Then StoppingDistance(2) = OriginalLocation(2) - CurrentFloorExact(2)
        If ElevatorEnable(2) < -15 Then ElevatorEnable(2) = -15
        Destination(2) = ((OriginalLocation(2) * 25) + 25) - DistanceToTravel(2)
        If GotoFloor(2) <> 0 And elevatorstart(2).Y <= (Destination(2) + (StoppingDistance(2) * 25) + 25) Then ElevatorDirection(2) = 1: ElevatorCheck(2) = 0
        End If
      
        If ElevatorEnable(2) < 0 And ElevatorDirection(2) = 1 Then
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(2) = 0 And FineTune(2) = False Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Stop_
        End If
        If ElevatorSounds(2).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(2) = 0 And FineTune(2) = False Then
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevstop.wav"
        ElevatorSounds(2).Play
        ElevatorCheck2(2) = 1
        End If
        Elevator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorL(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        ElevatorInsDoorR(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        FloorIndicator(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Plaque(2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, (ElevatorEnable(2) / 5), 0
        Next i2
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, (ElevatorEnable(2) / 5), 0
        'ElevatorSounds(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ''ElevatorMusic(2).SetPosition -20.25, Elevator(2).GetPosition.Y + 20, -23
        ElevatorEnable(2) = ElevatorEnable(2) + 0.25
        If ElevatorEnable(2) > 0 Then ElevatorEnable(2) = 0
        If ElevatorEnable(2) = 0 Then ElevatorDirection(2) = 0
        If GotoFloor(2) <> 0 Then ElevatorCheck(2) = 0: FineTune(2) = True
        End If
      
      If FineTune(2) = True And ElevatorEnable(2) = 0 And elevatorstart(2).Y > (GotoFloor(2) * 25) + 25 + -0.3 And elevatorstart(2).Y < (GotoFloor(2) * 25) + 25 + 0.3 Then
      FineTune(2) = False
      If ElevatorSync(2) = True Then
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
      For i2 = 1 To 10
      CallButtons(i2).Enable True
      Next i2
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(2) = 0
      OpenElevator(2) = 1
      ElevatorCheck(2) = 0
      ElevatorCheck2(2) = 0
      ElevatorCheck3(2) = 0
      ElevatorCheck4(2) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(2) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(2) = True Then
      If ElevatorCheck3(2) = 0 Then
      ElevatorSounds(2).Load App.Path + "\ding1.wav"
      ElevatorSounds(2).Play
      ElevatorCheck3(2) = 1
        For i2 = -1 To 138
        Buttons2(i2).SetColor RGBA(1, 1, 1, 1)
        Next i2
      End If
      If elevatorstart(2).Y < (GotoFloor(2) * 25) + 25 Then
      Elevator(2).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(2).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(2).MoveRelative 0, 0.2, 0
      Plaque(2).MoveRelative 0, 0.2, 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, 0.2, 0
        Next i2
        FloorIndicator(2).MoveRelative 0, 0.2, 0
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(2).Y > (GotoFloor(2) * 25) + 25 Then
      Elevator(2).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(2).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(2).MoveRelative 0, -0.2, 0
      Plaque(2).MoveRelative 0, -0.2, 0
        For i2 = -1 To 138
        Buttons2(i2).MoveRelative 0, -0.2, 0
        Next i2
        FloorIndicator(2).MoveRelative 0, -0.2, 0
        If ElevatorSync(2) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(2) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z >= 4 Then OpenElevator(2) = 0: GoTo OpenElevator1
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z >= 4 Then OpenElevator(2) = 0: GoTo OpenElevator1
      If ElevatorCheck4(2) = 0 Then
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(2).Stop_
        End If
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(2).Volume = 0
        'ElevatorSounds(2).maxDistance = 1000
        'Call ElevatorSounds(2).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(2).ConeOutsideVolume = 0
        'Call ElevatorSounds(2).SetPosition(-20.25, Elevator(2).GetPosition.Y, -23)
        ElevatorSounds(2).Play
        ElevatorCheck4(2) = 1
      End If
      OpenElevatorLoc(2) = OpenElevatorLoc(2) + 0.02
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z > 1 Then OpenElevator(2) = 2
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z > 1 Then OpenElevator(2) = 2
OpenElevator1:
      End If
      
      If OpenElevator(2) = 2 Then
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z > 3 Then OpenElevator(2) = 3
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z > 3 Then OpenElevator(2) = 3
      End If
      
      If OpenElevator(2) = 3 Then
      ElevatorCheck4(2) = 0
      OpenElevatorLoc(2) = OpenElevatorLoc(2) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z < 7 And OpenElevatorLoc(2) = 0 Then OpenElevatorLoc(2) = 0.02
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z < 7 And OpenElevatorLoc(2) = 0 Then OpenElevatorLoc(2) = 0.02
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      If OpenElevatorLoc(2) <= 0 Then
      OpenElevator(2) = 0
      OpenElevatorLoc(2) = 0
      Form1.Timer2.Enabled = True
      End If
      End If
      
      If OpenElevator(2) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z <= 0 Then OpenElevator(2) = 0: GoTo OpenElevator2
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z <= 0 Then OpenElevator(2) = 0: GoTo OpenElevator2
      If ElevatorCheck4(2) = 0 Then
        If ElevatorSounds(2).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(2).Stop_
        End If
        ElevatorSounds(2).Loop_ = False
        ElevatorSounds(2).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(2).Volume = 0
        'ElevatorSounds(2).maxDistance = 1000
        'Call ElevatorSounds(2).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(2).ConeOutsideVolume = 0
        'Call ElevatorSounds(2).SetPosition(-20.25, Elevator(2).GetPosition.Y, -23)
        ElevatorSounds(2).Play
        ElevatorCheck4(2) = 1
      End If
      OpenElevatorLoc(2) = OpenElevatorLoc(2) - 0.02
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z < 3 Then OpenElevator(2) = -2
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z < 3 Then OpenElevator(2) = -2
OpenElevator2:
      End If
      
      If OpenElevator(2) = -2 Then
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z < 1 Then OpenElevator(2) = -3
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z < 1 Then OpenElevator(2) = -3
      End If
      
      If OpenElevator(2) = -3 Then
      ElevatorCheck4(2) = 0
      OpenElevatorLoc(2) = OpenElevatorLoc(2) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(2)).GetPosition.z > 0 And OpenElevatorLoc(2) >= 0 Then OpenElevatorLoc(2) = -0.02
      If ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.z > 0 And OpenElevatorLoc(2) >= 0 Then OpenElevatorLoc(2) = -0.02
      ElevatorDoor2L(ElevatorFloor2(2)).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorDoor2R(ElevatorFloor2(2)).MoveRelative -OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorL(2).MoveRelative OpenElevatorLoc(2), 0, 0
      ElevatorInsDoorR(2).MoveRelative -OpenElevatorLoc(2), 0, 0
      If OpenElevatorLoc(2) >= 0 Then
      OpenElevator(2) = 0
      OpenElevatorLoc(2) = 0
      ElevatorInsDoorL(2).SetPosition ElevatorInsDoorL(2).GetPosition.X, ElevatorInsDoorL(2).GetPosition.Y, 0
      ElevatorInsDoorR(2).SetPosition ElevatorInsDoorR(2).GetPosition.X, ElevatorInsDoorR(2).GetPosition.Y, 0
      ElevatorDoor2L(ElevatorFloor2(2)).SetPosition ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.X, ElevatorDoor2L(ElevatorFloor2(2)).GetPosition.Y, 0
      ElevatorDoor2R(ElevatorFloor2(2)).SetPosition ElevatorDoor2R(ElevatorFloor2(2)).GetPosition.X, ElevatorDoor2R(ElevatorFloor2(2)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator3Loop()

elevatorstart(3) = Elevator(3).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(3) = (Elevator(3).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(3) < 1 Then ElevatorFloor(3) = 1
      
      If GotoFloor(3) = ElevatorFloor(3) - 1 Then CurrentFloor(3) = ElevatorFloor(3)

      'If GotoFloor(3) <> 0 And GotoFloor(3) > CurrentFloor(3) And ElevatorDirection(3) = 0 And ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z <= 0 Then
      If GotoFloor(3) <> 0 And GotoFloor(3) > CurrentFloor(3) And ElevatorDirection(3) = 0 And ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z <= 0 Then
      ElevatorDirection(3) = 1
      OriginalLocation(3) = CurrentFloorExact(3)
      DistanceToTravel(3) = ((GotoFloor(3) * 25) + 25) - ((CurrentFloorExact(3) * 25) + 25)
      If ElevatorSync(3) = True Then
      Room(CameraFloor).Enable False
      For i3 = 1 To 10
      CallButtons(i3).Enable False
      Next i3
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(3) <> 0 And GotoFloor(3) < CurrentFloor(3) And ElevatorDirection(3) = 0 And ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z <= 0 Then
      If GotoFloor(3) <> 0 And GotoFloor(3) < CurrentFloor(3) And ElevatorDirection(3) = 0 And ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z <= 0 Then
      ElevatorDirection(3) = -1
      OriginalLocation(3) = CurrentFloorExact(3)
      DistanceToTravel(3) = ((CurrentFloorExact(3) * 25) + 25) - ((GotoFloor(3) * 25) + 25)
      If ElevatorSync(3) = True Then
      Room(CameraFloor).Enable False
      For i3 = 1 To 10
      CallButtons(i3).Enable False
      Next i3
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(3) = Int((elevatorstart(3).Y - 25) / 25)
       CurrentFloorExact(3) = (elevatorstart(3).Y - 25) / 25
       'CurrentFloor(3) = (Elevator(3).GetPosition.y / 25) - 1
       'CurrentFloorExact(3) = Int((Elevator(3).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(3)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(3)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(3) / 5)
        
        If ElevatorEnable(3) >= 0 And ElevatorDirection(3) = 1 Then
        'sound
        If ElevatorSounds(3).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(3) = 0 And GotoFloor(3) <> ElevatorFloor(3) Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevstart.wav"
        ElevatorSounds(3).Volume = 0
        'ElevatorSounds(3).maxDistance = 100
        'Call ElevatorSounds(3).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(3).ConeOutsideVolume = 0
        'Call ElevatorSounds(3).SetPosition(-20.25, Elevator1(3).GetPosition.Y + 20, -23)
        ElevatorSounds(3).Play
        ElevatorCheck(3) = 1
        End If
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(3) = 1 Then
        ElevatorSounds(3).Load App.Path + "\elevmove.wav"
        ElevatorSounds(3).Loop_ = True
        ElevatorSounds(3).Play
        End If
        'movement
        Elevator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorL(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorR(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        FloorIndicator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Plaque(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Next i3
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, (ElevatorEnable(3) / 5), 0
        'ElevatorSounds(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ''ElevatorMusic(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ElevatorEnable(3) = ElevatorEnable(3) + 0.25
        If ElevatorEnable(3) <= 15 Then StoppingDistance(3) = CurrentFloorExact(3) - OriginalLocation(3) + 0.4
        If ElevatorEnable(3) > 15 Then ElevatorEnable(3) = 15
        Destination(3) = ((OriginalLocation(3) * 25) + 25) + DistanceToTravel(3) - 35
        If GotoFloor(3) <> 0 And elevatorstart(3).Y >= (Destination(3) - (StoppingDistance(3) * 25) + 25) Then ElevatorDirection(3) = -1: ElevatorCheck(3) = 0
        End If
      
        If ElevatorEnable(3) > 0 And ElevatorDirection(3) = -1 Then
        'Sounds
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(3) = 0 And FineTune(3) = False Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Stop_
        End If
        If ElevatorSounds(3).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(3) = 0 And FineTune(3) = False Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevstop.wav"
        ElevatorSounds(3).Play
        ElevatorCheck2(3) = 1
        End If
        'Movement
        Elevator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorL(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorR(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        FloorIndicator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Plaque(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Next i3
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, (ElevatorEnable(3) / 5), 0
        
        'ElevatorSounds(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ''ElevatorMusic(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ElevatorEnable(3) = ElevatorEnable(3) - 0.25
        If ElevatorEnable(3) < 0 Then ElevatorEnable(3) = 0
        If ElevatorEnable(3) = 0 Then ElevatorDirection(3) = 0
        If GotoFloor(3) <> 0 Then ElevatorCheck(3) = 0: FineTune(3) = True
        End If
      
        If ElevatorEnable(3) <= 0 And ElevatorDirection(3) = -1 Then
        If ElevatorSounds(3).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(3) = 0 Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevstart.wav"
        ElevatorSounds(3).Volume = 0
        'ElevatorSounds(3).maxDistance = 100
        'Call ElevatorSounds(3).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(3).ConeOutsideVolume = 0
        'Call ElevatorSounds(3).SetPosition(-20.25, Elevator(3).GetPosition.Y + 20, -23)
        ElevatorSounds(3).Play
        ElevatorCheck(3) = 1
        End If
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(3) = 1 Then
        ElevatorSounds(3).Load App.Path + "\elevmove.wav"
        ElevatorSounds(3).Loop_ = True
        ElevatorSounds(3).Play
        End If
        Elevator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorL(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorR(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        FloorIndicator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Plaque(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Next i3
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, (ElevatorEnable(3) / 5), 0
        'ElevatorSounds(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ''ElevatorMusic(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ElevatorEnable(3) = ElevatorEnable(3) - 0.25
        If ElevatorEnable(3) >= -15 Then StoppingDistance(3) = OriginalLocation(3) - CurrentFloorExact(3)
        If ElevatorEnable(3) < -15 Then ElevatorEnable(3) = -15
        Destination(3) = ((OriginalLocation(3) * 25) + 25) - DistanceToTravel(3)
        If GotoFloor(3) <> 0 And elevatorstart(3).Y <= (Destination(3) + (StoppingDistance(3) * 25) + 25) Then ElevatorDirection(3) = 1: ElevatorCheck(3) = 0
        End If
      
        If ElevatorEnable(3) < 0 And ElevatorDirection(3) = 1 Then
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(3) = 0 And FineTune(3) = False Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Stop_
        End If
        If ElevatorSounds(3).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(3) = 0 And FineTune(3) = False Then
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevstop.wav"
        ElevatorSounds(3).Play
        ElevatorCheck2(3) = 1
        End If
        Elevator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorL(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        ElevatorInsDoorR(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        FloorIndicator(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Plaque(3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, (ElevatorEnable(3) / 5), 0
        Next i3
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, (ElevatorEnable(3) / 5), 0
        'ElevatorSounds(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ''ElevatorMusic(3).SetPosition -20.25, Elevator(3).GetPosition.Y + 20, -23
        ElevatorEnable(3) = ElevatorEnable(3) + 0.25
        If ElevatorEnable(3) > 0 Then ElevatorEnable(3) = 0
        If ElevatorEnable(3) = 0 Then ElevatorDirection(3) = 0
        If GotoFloor(3) <> 0 Then ElevatorCheck(3) = 0: FineTune(3) = True
        End If
      
      If FineTune(3) = True And ElevatorEnable(3) = 0 And elevatorstart(3).Y > (GotoFloor(3) * 25) + 25 + -0.3 And elevatorstart(3).Y < (GotoFloor(3) * 25) + 25 + 0.3 Then
      FineTune(3) = False
      Room(CameraFloor).Enable True
      If ElevatorSync(3) = True Then
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
      For i3 = 1 To 10
      CallButtons(i3).Enable True
      Next i3
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(3) = 0
      OpenElevator(3) = 1
      ElevatorCheck(3) = 0
      ElevatorCheck2(3) = 0
      ElevatorCheck3(3) = 0
      ElevatorCheck4(3) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(3) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(3) = True Then
      If ElevatorCheck3(3) = 0 Then
      ElevatorSounds(3).Load App.Path + "\ding1.wav"
      ElevatorSounds(3).Play
      ElevatorCheck3(3) = 1
        For i3 = -1 To 138
        Buttons3(i3).SetColor RGBA(1, 1, 1, 1)
        Next i3
      End If
      If elevatorstart(3).Y < (GotoFloor(3) * 25) + 25 Then
      Elevator(3).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(3).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(3).MoveRelative 0, 0.2, 0
      Plaque(3).MoveRelative 0, 0.2, 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, 0.2, 0
        Next i3
        FloorIndicator(3).MoveRelative 0, 0.2, 0
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(3).Y > (GotoFloor(3) * 25) + 25 Then
      Elevator(3).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(3).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(3).MoveRelative 0, -0.2, 0
      Plaque(3).MoveRelative 0, -0.2, 0
        For i3 = -1 To 138
        Buttons3(i3).MoveRelative 0, -0.2, 0
        Next i3
        FloorIndicator(3).MoveRelative 0, -0.2, 0
        If ElevatorSync(3) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(3) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z >= 4 Then OpenElevator(3) = 0: GoTo OpenElevator1
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z >= 4 Then OpenElevator(3) = 0: GoTo OpenElevator1
      If ElevatorCheck4(3) = 0 Then
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(3).Stop_
        End If
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(3).Volume = 0
        'ElevatorSounds(3).maxDistance = 1000
        'Call ElevatorSounds(3).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(3).ConeOutsideVolume = 0
        'Call ElevatorSounds(3).SetPosition(-20.25, Elevator(3).GetPosition.Y, -23)
        ElevatorSounds(3).Play
        ElevatorCheck4(3) = 1
      End If
      OpenElevatorLoc(3) = OpenElevatorLoc(3) + 0.02
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z > 1 Then OpenElevator(3) = 2
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z > 1 Then OpenElevator(3) = 2
OpenElevator1:
      End If
      
      If OpenElevator(3) = 2 Then
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z > 3 Then OpenElevator(3) = 3
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z > 3 Then OpenElevator(3) = 3
      End If
      
      If OpenElevator(3) = 3 Then
      ElevatorCheck4(3) = 0
      OpenElevatorLoc(3) = OpenElevatorLoc(3) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z < 7 And OpenElevatorLoc(3) = 0 Then OpenElevatorLoc(3) = 0.02
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z < 7 And OpenElevatorLoc(3) = 0 Then OpenElevatorLoc(3) = 0.02
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      If OpenElevatorLoc(3) <= 0 Then
      OpenElevator(3) = 0
      OpenElevatorLoc(3) = 0
      Form1.Timer3.Enabled = True
      End If
      End If
      
      If OpenElevator(3) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z <= 0 Then OpenElevator(3) = 0: GoTo OpenElevator2
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z <= 0 Then OpenElevator(3) = 0: GoTo OpenElevator2
      If ElevatorCheck4(3) = 0 Then
        If ElevatorSounds(3).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(3).Stop_
        End If
        ElevatorSounds(3).Loop_ = False
        ElevatorSounds(3).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(3).Volume = 0
        'ElevatorSounds(3).maxDistance = 1000
        'Call ElevatorSounds(3).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(3).ConeOutsideVolume = 0
        'Call ElevatorSounds(3).SetPosition(-20.25, Elevator(3).GetPosition.Y, -23)
        ElevatorSounds(3).Play
        ElevatorCheck4(3) = 1
      End If
      OpenElevatorLoc(3) = OpenElevatorLoc(3) - 0.02
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z < 3 Then OpenElevator(3) = -2
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z < 3 Then OpenElevator(3) = -2
OpenElevator2:
      End If
      
      If OpenElevator(3) = -2 Then
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z < 1 Then OpenElevator(3) = -3
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z < 1 Then OpenElevator(3) = -3
      End If
      
      If OpenElevator(3) = -3 Then
      ElevatorCheck4(3) = 0
      OpenElevatorLoc(3) = OpenElevatorLoc(3) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(3)).GetPosition.z > 0 And OpenElevatorLoc(3) >= 0 Then OpenElevatorLoc(3) = -0.02
      If ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.z > 0 And OpenElevatorLoc(3) >= 0 Then OpenElevatorLoc(3) = -0.02
      ElevatorDoor3L(ElevatorFloor2(3)).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorDoor3R(ElevatorFloor2(3)).MoveRelative -OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorL(3).MoveRelative OpenElevatorLoc(3), 0, 0
      ElevatorInsDoorR(3).MoveRelative -OpenElevatorLoc(3), 0, 0
      If OpenElevatorLoc(3) >= 0 Then
      OpenElevator(3) = 0
      OpenElevatorLoc(3) = 0
      ElevatorInsDoorL(3).SetPosition ElevatorInsDoorL(3).GetPosition.X, ElevatorInsDoorL(3).GetPosition.Y, 0
      ElevatorInsDoorR(3).SetPosition ElevatorInsDoorR(3).GetPosition.X, ElevatorInsDoorR(3).GetPosition.Y, 0
      ElevatorDoor3L(ElevatorFloor2(3)).SetPosition ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.X, ElevatorDoor3L(ElevatorFloor2(3)).GetPosition.Y, 0
      ElevatorDoor3R(ElevatorFloor2(3)).SetPosition ElevatorDoor3R(ElevatorFloor2(3)).GetPosition.X, ElevatorDoor3R(ElevatorFloor2(3)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator4Loop()

elevatorstart(4) = Elevator(4).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(4) = (Elevator(4).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(4) < 1 Then ElevatorFloor(4) = 1
      
      If GotoFloor(4) = ElevatorFloor(4) - 1 Then CurrentFloor(4) = ElevatorFloor(4)

      'If GotoFloor(4) <> 0 And GotoFloor(4) > CurrentFloor(4) And ElevatorDirection(4) = 0 And ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z <= 0 Then
      If GotoFloor(4) <> 0 And GotoFloor(4) > CurrentFloor(4) And ElevatorDirection(4) = 0 And ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z <= 0 Then
      ElevatorDirection(4) = 1
      OriginalLocation(4) = CurrentFloorExact(4)
      DistanceToTravel(4) = ((GotoFloor(4) * 25) + 25) - ((CurrentFloorExact(4) * 25) + 25)
      If ElevatorSync(4) = True Then
      Room(CameraFloor).Enable False
      For i4 = 1 To 10
      CallButtons(i4).Enable False
      Next i4
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(4) <> 0 And GotoFloor(4) < CurrentFloor(4) And ElevatorDirection(4) = 0 And ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z <= 0 Then
      If GotoFloor(4) <> 0 And GotoFloor(4) < CurrentFloor(4) And ElevatorDirection(4) = 0 And ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z <= 0 Then
      ElevatorDirection(4) = -1
      OriginalLocation(4) = CurrentFloorExact(4)
      DistanceToTravel(4) = ((CurrentFloorExact(4) * 25) + 25) - ((GotoFloor(4) * 25) + 25)
      If ElevatorSync(4) = True Then
      Room(CameraFloor).Enable False
      For i4 = 1 To 10
      CallButtons(i4).Enable False
      Next i4
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(4) = Int((elevatorstart(4).Y - 25) / 25)
       CurrentFloorExact(4) = (elevatorstart(4).Y - 25) / 25
       'CurrentFloor(4) = (Elevator(4).GetPosition.y / 25) - 1
       'CurrentFloorExact(4) = Int((Elevator(4).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(4)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(4)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(4) / 5)
        
        If ElevatorEnable(4) >= 0 And ElevatorDirection(4) = 1 Then
        'sound
        If ElevatorSounds(4).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(4) = 0 And GotoFloor(4) <> ElevatorFloor(4) Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevstart.wav"
        ElevatorSounds(4).Volume = 0
        'ElevatorSounds(4).maxDistance = 100
        'Call ElevatorSounds(4).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(4).ConeOutsideVolume = 0
        'Call ElevatorSounds(4).SetPosition(-20.25, Elevator1(4).GetPosition.Y + 20, -23)
        ElevatorSounds(4).Play
        ElevatorCheck(4) = 1
        End If
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(4) = 1 Then
        ElevatorSounds(4).Load App.Path + "\elevmove.wav"
        ElevatorSounds(4).Loop_ = True
        ElevatorSounds(4).Play
        End If
        'movement
        Elevator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorL(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorR(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        FloorIndicator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Plaque(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Next i4
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, (ElevatorEnable(4) / 5), 0
        'ElevatorSounds(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ''ElevatorMusic(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ElevatorEnable(4) = ElevatorEnable(4) + 0.25
        If ElevatorEnable(4) <= 15 Then StoppingDistance(4) = CurrentFloorExact(4) - OriginalLocation(4) + 0.4
        If ElevatorEnable(4) > 15 Then ElevatorEnable(4) = 15
        Destination(4) = ((OriginalLocation(4) * 25) + 25) + DistanceToTravel(4) - 35
        If GotoFloor(4) <> 0 And elevatorstart(4).Y >= (Destination(4) - (StoppingDistance(4) * 25) + 25) Then ElevatorDirection(4) = -1: ElevatorCheck(4) = 0
        End If
      
        If ElevatorEnable(4) > 0 And ElevatorDirection(4) = -1 Then
        'Sounds
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(4) = 0 And FineTune(4) = False Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Stop_
        End If
        If ElevatorSounds(4).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(4) = 0 And FineTune(4) = False Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevstop.wav"
        ElevatorSounds(4).Play
        ElevatorCheck2(4) = 1
        End If
        'Movement
        Elevator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorL(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorR(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        FloorIndicator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Plaque(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Next i4
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, (ElevatorEnable(4) / 5), 0
        
        'ElevatorSounds(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ''ElevatorMusic(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ElevatorEnable(4) = ElevatorEnable(4) - 0.25
        If ElevatorEnable(4) < 0 Then ElevatorEnable(4) = 0
        If ElevatorEnable(4) = 0 Then ElevatorDirection(4) = 0
        If GotoFloor(4) <> 0 Then ElevatorCheck(4) = 0: FineTune(4) = True
        End If
      
        If ElevatorEnable(4) <= 0 And ElevatorDirection(4) = -1 Then
        If ElevatorSounds(4).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(4) = 0 Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevstart.wav"
        ElevatorSounds(4).Volume = 0
        'ElevatorSounds(4).maxDistance = 100
        'Call ElevatorSounds(4).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(4).ConeOutsideVolume = 0
        'Call ElevatorSounds(4).SetPosition(-20.25, Elevator(4).GetPosition.Y + 20, -23)
        ElevatorSounds(4).Play
        ElevatorCheck(4) = 1
        End If
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(4) = 1 Then
        ElevatorSounds(4).Load App.Path + "\elevmove.wav"
        ElevatorSounds(4).Loop_ = True
        ElevatorSounds(4).Play
        End If
        Elevator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorL(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorR(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        FloorIndicator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Plaque(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Next i4
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, (ElevatorEnable(4) / 5), 0
        'ElevatorSounds(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ''ElevatorMusic(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ElevatorEnable(4) = ElevatorEnable(4) - 0.25
        If ElevatorEnable(4) >= -15 Then StoppingDistance(4) = OriginalLocation(4) - CurrentFloorExact(4)
        If ElevatorEnable(4) < -15 Then ElevatorEnable(4) = -15
        Destination(4) = ((OriginalLocation(4) * 25) + 25) - DistanceToTravel(4)
        If GotoFloor(4) <> 0 And elevatorstart(4).Y <= (Destination(4) + (StoppingDistance(4) * 25) + 25) Then ElevatorDirection(4) = 1: ElevatorCheck(4) = 0
        End If
      
        If ElevatorEnable(4) < 0 And ElevatorDirection(4) = 1 Then
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(4) = 0 And FineTune(4) = False Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Stop_
        End If
        If ElevatorSounds(4).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(4) = 0 And FineTune(4) = False Then
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevstop.wav"
        ElevatorSounds(4).Play
        ElevatorCheck2(4) = 1
        End If
        Elevator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorL(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        ElevatorInsDoorR(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        FloorIndicator(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Plaque(4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, (ElevatorEnable(4) / 5), 0
        Next i4
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, (ElevatorEnable(4) / 5), 0
        'ElevatorSounds(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ''ElevatorMusic(4).SetPosition -20.25, Elevator(4).GetPosition.Y + 20, -23
        ElevatorEnable(4) = ElevatorEnable(4) + 0.25
        If ElevatorEnable(4) > 0 Then ElevatorEnable(4) = 0
        If ElevatorEnable(4) = 0 Then ElevatorDirection(4) = 0
        If GotoFloor(4) <> 0 Then ElevatorCheck(4) = 0: FineTune(4) = True
        End If
      
      If FineTune(4) = True And ElevatorEnable(4) = 0 And elevatorstart(4).Y > (GotoFloor(4) * 25) + 25 + -0.3 And elevatorstart(4).Y < (GotoFloor(4) * 25) + 25 + 0.3 Then
      FineTune(4) = False
      Room(CameraFloor).Enable True
      If ElevatorSync(4) = True Then
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
      For i4 = 1 To 10
      CallButtons(i4).Enable True
      Next i4
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(4) = 0
      OpenElevator(4) = 1
      ElevatorCheck(4) = 0
      ElevatorCheck2(4) = 0
      ElevatorCheck3(4) = 0
      ElevatorCheck4(4) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(4) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(4) = True Then
      If ElevatorCheck3(4) = 0 Then
      ElevatorSounds(4).Load App.Path + "\ding1.wav"
      ElevatorSounds(4).Play
      ElevatorCheck3(4) = 1
        For i4 = -1 To 138
        Buttons4(i4).SetColor RGBA(1, 1, 1, 1)
        Next i4
      End If
      If elevatorstart(4).Y < (GotoFloor(4) * 25) + 25 Then
      Elevator(4).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(4).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(4).MoveRelative 0, 0.2, 0
      Plaque(4).MoveRelative 0, 0.2, 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, 0.2, 0
        Next i4
        FloorIndicator(4).MoveRelative 0, 0.2, 0
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(4).Y > (GotoFloor(4) * 25) + 25 Then
      Elevator(4).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(4).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(4).MoveRelative 0, -0.2, 0
      Plaque(4).MoveRelative 0, -0.2, 0
        For i4 = -1 To 138
        Buttons4(i4).MoveRelative 0, -0.2, 0
        Next i4
        FloorIndicator(4).MoveRelative 0, -0.2, 0
        If ElevatorSync(4) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(4) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z >= 4 Then OpenElevator(4) = 0: GoTo OpenElevator1
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z >= 4 Then OpenElevator(4) = 0: GoTo OpenElevator1
      If ElevatorCheck4(4) = 0 Then
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(4).Stop_
        End If
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(4).Volume = 0
        'ElevatorSounds(4).maxDistance = 1000
        'Call ElevatorSounds(4).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(4).ConeOutsideVolume = 0
        'Call ElevatorSounds(4).SetPosition(-20.25, Elevator(4).GetPosition.Y, -23)
        ElevatorSounds(4).Play
        ElevatorCheck4(4) = 1
      End If
      OpenElevatorLoc(4) = OpenElevatorLoc(4) + 0.02
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z > 1 Then OpenElevator(4) = 2
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z > 1 Then OpenElevator(4) = 2
OpenElevator1:
      End If
      
      If OpenElevator(4) = 2 Then
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z > 3 Then OpenElevator(4) = 3
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z > 3 Then OpenElevator(4) = 3
      End If
      
      If OpenElevator(4) = 3 Then
      ElevatorCheck4(4) = 0
      OpenElevatorLoc(4) = OpenElevatorLoc(4) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z < 7 And OpenElevatorLoc(4) = 0 Then OpenElevatorLoc(4) = 0.02
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z < 7 And OpenElevatorLoc(4) = 0 Then OpenElevatorLoc(4) = 0.02
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      If OpenElevatorLoc(4) <= 0 Then
      OpenElevator(4) = 0
      OpenElevatorLoc(4) = 0
      Form1.Timer4.Enabled = True
      End If
      End If
      
      If OpenElevator(4) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z <= 0 Then OpenElevator(4) = 0: GoTo OpenElevator2
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z <= 0 Then OpenElevator(4) = 0: GoTo OpenElevator2
      If ElevatorCheck4(4) = 0 Then
        If ElevatorSounds(4).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(4).Stop_
        End If
        ElevatorSounds(4).Loop_ = False
        ElevatorSounds(4).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(4).Volume = 0
        'ElevatorSounds(4).maxDistance = 1000
        'Call ElevatorSounds(4).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(4).ConeOutsideVolume = 0
        'Call ElevatorSounds(4).SetPosition(-20.25, Elevator(4).GetPosition.Y, -23)
        ElevatorSounds(4).Play
        ElevatorCheck4(4) = 1
      End If
      OpenElevatorLoc(4) = OpenElevatorLoc(4) - 0.02
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z < 3 Then OpenElevator(4) = -2
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z < 3 Then OpenElevator(4) = -2
OpenElevator2:
      End If
      
      If OpenElevator(4) = -2 Then
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z < 1 Then OpenElevator(4) = -3
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z < 1 Then OpenElevator(4) = -3
      End If
      
      If OpenElevator(4) = -3 Then
      ElevatorCheck4(4) = 0
      OpenElevatorLoc(4) = OpenElevatorLoc(4) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(4)).GetPosition.z > 0 And OpenElevatorLoc(4) >= 0 Then OpenElevatorLoc(4) = -0.02
      If ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.z > 0 And OpenElevatorLoc(4) >= 0 Then OpenElevatorLoc(4) = -0.02
      ElevatorDoor4L(ElevatorFloor2(4)).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorDoor4R(ElevatorFloor2(4)).MoveRelative -OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorL(4).MoveRelative OpenElevatorLoc(4), 0, 0
      ElevatorInsDoorR(4).MoveRelative -OpenElevatorLoc(4), 0, 0
      If OpenElevatorLoc(4) >= 0 Then
      OpenElevator(4) = 0
      OpenElevatorLoc(4) = 0
      ElevatorInsDoorL(4).SetPosition ElevatorInsDoorL(4).GetPosition.X, ElevatorInsDoorL(4).GetPosition.Y, 0
      ElevatorInsDoorR(4).SetPosition ElevatorInsDoorR(4).GetPosition.X, ElevatorInsDoorR(4).GetPosition.Y, 0
      ElevatorDoor4L(ElevatorFloor2(4)).SetPosition ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.X, ElevatorDoor4L(ElevatorFloor2(4)).GetPosition.Y, 0
      ElevatorDoor4R(ElevatorFloor2(4)).SetPosition ElevatorDoor4R(ElevatorFloor2(4)).GetPosition.X, ElevatorDoor4R(ElevatorFloor2(4)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator5Loop()

elevatorstart(5) = Elevator(5).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(5) = (Elevator(5).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(5) < 1 Then ElevatorFloor(5) = 1
      
      If GotoFloor(5) = ElevatorFloor(5) - 1 Then CurrentFloor(5) = ElevatorFloor(5)

      'If GotoFloor(5) <> 0 And GotoFloor(5) > CurrentFloor(5) And ElevatorDirection(5) = 0 And ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z <= 0 Then
      If GotoFloor(5) <> 0 And GotoFloor(5) > CurrentFloor(5) And ElevatorDirection(5) = 0 And ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z <= 0 Then
      ElevatorDirection(5) = 1
      OriginalLocation(5) = CurrentFloorExact(5)
      DistanceToTravel(5) = ((GotoFloor(5) * 25) + 25) - ((CurrentFloorExact(5) * 25) + 25)
      If ElevatorSync(5) = True Then
      Room(CameraFloor).Enable False
      For i5 = 1 To 10
      CallButtons(i5).Enable False
      Next i5
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(5) <> 0 And GotoFloor(5) < CurrentFloor(5) And ElevatorDirection(5) = 0 And ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z <= 0 Then
      If GotoFloor(5) <> 0 And GotoFloor(5) < CurrentFloor(5) And ElevatorDirection(5) = 0 And ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z <= 0 Then
      ElevatorDirection(5) = -1
      OriginalLocation(5) = CurrentFloorExact(5)
      DistanceToTravel(5) = ((CurrentFloorExact(5) * 25) + 25) - ((GotoFloor(5) * 25) + 25)
      If ElevatorSync(5) = True Then
      Room(CameraFloor).Enable False
      For i5 = 1 To 10
      CallButtons(i5).Enable False
      Next i5
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(5) = Int((elevatorstart(5).Y - 25) / 25)
       CurrentFloorExact(5) = (elevatorstart(5).Y - 25) / 25
       'CurrentFloor(5) = (Elevator(5).GetPosition.y / 25) - 1
       'CurrentFloorExact(5) = Int((Elevator(5).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(5)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(5)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(5) / 5)
        
        If ElevatorEnable(5) >= 0 And ElevatorDirection(5) = 1 Then
        'sound
        If ElevatorSounds(5).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(5) = 0 And GotoFloor(5) <> ElevatorFloor(5) Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevstart.wav"
        ElevatorSounds(5).Volume = 0
        'ElevatorSounds(5).maxDistance = 100
        'Call ElevatorSounds(5).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(5).ConeOutsideVolume = 0
        'Call ElevatorSounds(5).SetPosition(-20.25, Elevator1(5).GetPosition.Y + 20, -23)
        ElevatorSounds(5).Play
        ElevatorCheck(5) = 1
        End If
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(5) = 1 Then
        ElevatorSounds(5).Load App.Path + "\elevmove.wav"
        ElevatorSounds(5).Loop_ = True
        ElevatorSounds(5).Play
        End If
        'movement
        Elevator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorL(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorR(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        FloorIndicator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Plaque(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Next i5
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, (ElevatorEnable(5) / 5), 0
        'ElevatorSounds(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ''ElevatorMusic(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ElevatorEnable(5) = ElevatorEnable(5) + 0.25
        If ElevatorEnable(5) <= 15 Then StoppingDistance(5) = CurrentFloorExact(5) - OriginalLocation(5) + 0.4
        If ElevatorEnable(5) > 15 Then ElevatorEnable(5) = 15
        Destination(5) = ((OriginalLocation(5) * 25) + 25) + DistanceToTravel(5) - 35
        If GotoFloor(5) <> 0 And elevatorstart(5).Y >= (Destination(5) - (StoppingDistance(5) * 25) + 25) Then ElevatorDirection(5) = -1: ElevatorCheck(5) = 0
        End If
      
        If ElevatorEnable(5) > 0 And ElevatorDirection(5) = -1 Then
        'Sounds
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(5) = 0 And FineTune(5) = False Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Stop_
        End If
        If ElevatorSounds(5).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(5) = 0 And FineTune(5) = False Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevstop.wav"
        ElevatorSounds(5).Play
        ElevatorCheck2(5) = 1
        End If
        'Movement
        Elevator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorL(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorR(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        FloorIndicator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Plaque(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Next i5
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, (ElevatorEnable(5) / 5), 0
        
        'ElevatorSounds(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ''ElevatorMusic(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ElevatorEnable(5) = ElevatorEnable(5) - 0.25
        If ElevatorEnable(5) < 0 Then ElevatorEnable(5) = 0
        If ElevatorEnable(5) = 0 Then ElevatorDirection(5) = 0
        If GotoFloor(5) <> 0 Then ElevatorCheck(5) = 0: FineTune(5) = True
        End If
      
        If ElevatorEnable(5) <= 0 And ElevatorDirection(5) = -1 Then
        If ElevatorSounds(5).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(5) = 0 Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevstart.wav"
        ElevatorSounds(5).Volume = 0
        'ElevatorSounds(5).maxDistance = 100
        'Call ElevatorSounds(5).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(5).ConeOutsideVolume = 0
        'Call ElevatorSounds(5).SetPosition(-20.25, Elevator(5).GetPosition.Y + 20, -23)
        ElevatorSounds(5).Play
        ElevatorCheck(5) = 1
        End If
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(5) = 1 Then
        ElevatorSounds(5).Load App.Path + "\elevmove.wav"
        ElevatorSounds(5).Loop_ = True
        ElevatorSounds(5).Play
        End If
        Elevator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorL(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorR(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        FloorIndicator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Plaque(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Next i5
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, (ElevatorEnable(5) / 5), 0
        'ElevatorSounds(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ''ElevatorMusic(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ElevatorEnable(5) = ElevatorEnable(5) - 0.25
        If ElevatorEnable(5) >= -15 Then StoppingDistance(5) = OriginalLocation(5) - CurrentFloorExact(5)
        If ElevatorEnable(5) < -15 Then ElevatorEnable(5) = -15
        Destination(5) = ((OriginalLocation(5) * 25) + 25) - DistanceToTravel(5)
        If GotoFloor(5) <> 0 And elevatorstart(5).Y <= (Destination(5) + (StoppingDistance(5) * 25) + 25) Then ElevatorDirection(5) = 1: ElevatorCheck(5) = 0
        End If
      
        If ElevatorEnable(5) < 0 And ElevatorDirection(5) = 1 Then
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(5) = 0 And FineTune(5) = False Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Stop_
        End If
        If ElevatorSounds(5).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(5) = 0 And FineTune(5) = False Then
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevstop.wav"
        ElevatorSounds(5).Play
        ElevatorCheck2(5) = 1
        End If
        Elevator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorL(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        ElevatorInsDoorR(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        FloorIndicator(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Plaque(5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, (ElevatorEnable(5) / 5), 0
        Next i5
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, (ElevatorEnable(5) / 5), 0
        'ElevatorSounds(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ''ElevatorMusic(5).SetPosition -20.25, Elevator(5).GetPosition.Y + 20, -23
        ElevatorEnable(5) = ElevatorEnable(5) + 0.25
        If ElevatorEnable(5) > 0 Then ElevatorEnable(5) = 0
        If ElevatorEnable(5) = 0 Then ElevatorDirection(5) = 0
        If GotoFloor(5) <> 0 Then ElevatorCheck(5) = 0: FineTune(5) = True
        End If
      
      If FineTune(5) = True And ElevatorEnable(5) = 0 And elevatorstart(5).Y > (GotoFloor(5) * 25) + 25 + -0.3 And elevatorstart(5).Y < (GotoFloor(5) * 25) + 25 + 0.3 Then
      FineTune(5) = False
      If ElevatorSync(5) = True Then
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
      For i5 = 1 To 10
      CallButtons(i5).Enable True
      Next i5
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(5) = 0
      OpenElevator(5) = 1
      ElevatorCheck(5) = 0
      ElevatorCheck2(5) = 0
      ElevatorCheck3(5) = 0
      ElevatorCheck4(5) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(5) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(5) = True Then
      If ElevatorCheck3(5) = 0 Then
      ElevatorSounds(5).Load App.Path + "\ding1.wav"
      ElevatorSounds(5).Play
      ElevatorCheck3(5) = 1
        For i5 = -1 To 138
        Buttons5(i5).SetColor RGBA(1, 1, 1, 1)
        Next i5
      End If
      If elevatorstart(5).Y < (GotoFloor(5) * 25) + 25 Then
      Elevator(5).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(5).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(5).MoveRelative 0, 0.2, 0
      Plaque(5).MoveRelative 0, 0.2, 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, 0.2, 0
        Next i5
        FloorIndicator(5).MoveRelative 0, 0.2, 0
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(5).Y > (GotoFloor(5) * 25) + 25 Then
      Elevator(5).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(5).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(5).MoveRelative 0, -0.2, 0
      Plaque(5).MoveRelative 0, -0.2, 0
        For i5 = -1 To 138
        Buttons5(i5).MoveRelative 0, -0.2, 0
        Next i5
        FloorIndicator(5).MoveRelative 0, -0.2, 0
        If ElevatorSync(5) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(5) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z >= 4 Then OpenElevator(5) = 0: GoTo OpenElevator1
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z >= 4 Then OpenElevator(5) = 0: GoTo OpenElevator1
      If ElevatorCheck4(5) = 0 Then
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(5).Stop_
        End If
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(5).Volume = 0
        'ElevatorSounds(5).maxDistance = 1000
        'Call ElevatorSounds(5).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(5).ConeOutsideVolume = 0
        'Call ElevatorSounds(5).SetPosition(-20.25, Elevator(5).GetPosition.Y, -23)
        ElevatorSounds(5).Play
        ElevatorCheck4(5) = 1
      End If
      OpenElevatorLoc(5) = OpenElevatorLoc(5) + 0.02
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z > 1 Then OpenElevator(5) = 2
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z > 1 Then OpenElevator(5) = 2
OpenElevator1:
      End If
      
      If OpenElevator(5) = 2 Then
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z > 3 Then OpenElevator(5) = 3
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z > 3 Then OpenElevator(5) = 3
      End If
      
      If OpenElevator(5) = 3 Then
      ElevatorCheck4(5) = 0
      OpenElevatorLoc(5) = OpenElevatorLoc(5) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z < 7 And OpenElevatorLoc(5) = 0 Then OpenElevatorLoc(5) = 0.02
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z < 7 And OpenElevatorLoc(5) = 0 Then OpenElevatorLoc(5) = 0.02
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      If OpenElevatorLoc(5) <= 0 Then
      OpenElevator(5) = 0
      OpenElevatorLoc(5) = 0
      Form1.Timer5.Enabled = True
      End If
      End If
      
      If OpenElevator(5) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z <= 0 Then OpenElevator(5) = 0: GoTo OpenElevator2
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z <= 0 Then OpenElevator(5) = 0: GoTo OpenElevator2
      If ElevatorCheck4(5) = 0 Then
        If ElevatorSounds(5).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(5).Stop_
        End If
        ElevatorSounds(5).Loop_ = False
        ElevatorSounds(5).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(5).Volume = 0
        'ElevatorSounds(5).maxDistance = 1000
        'Call ElevatorSounds(5).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(5).ConeOutsideVolume = 0
        'Call ElevatorSounds(5).SetPosition(-20.25, Elevator(5).GetPosition.Y, -23)
        ElevatorSounds(5).Play
        ElevatorCheck4(5) = 1
      End If
      OpenElevatorLoc(5) = OpenElevatorLoc(5) - 0.02
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z < 3 Then OpenElevator(5) = -2
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z < 3 Then OpenElevator(5) = -2
OpenElevator2:
      End If
      
      If OpenElevator(5) = -2 Then
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z < 1 Then OpenElevator(5) = -3
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z < 1 Then OpenElevator(5) = -3
      End If
      
      If OpenElevator(5) = -3 Then
      ElevatorCheck4(5) = 0
      OpenElevatorLoc(5) = OpenElevatorLoc(5) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(5)).GetPosition.z > 0 And OpenElevatorLoc(5) >= 0 Then OpenElevatorLoc(5) = -0.02
      If ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.z > 0 And OpenElevatorLoc(5) >= 0 Then OpenElevatorLoc(5) = -0.02
      ElevatorDoor5L(ElevatorFloor2(5)).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorDoor5R(ElevatorFloor2(5)).MoveRelative -OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorL(5).MoveRelative OpenElevatorLoc(5), 0, 0
      ElevatorInsDoorR(5).MoveRelative -OpenElevatorLoc(5), 0, 0
      If OpenElevatorLoc(5) >= 0 Then
      OpenElevator(5) = 0
      OpenElevatorLoc(5) = 0
      ElevatorInsDoorL(5).SetPosition ElevatorInsDoorL(5).GetPosition.X, ElevatorInsDoorL(5).GetPosition.Y, 0
      ElevatorInsDoorR(5).SetPosition ElevatorInsDoorR(5).GetPosition.X, ElevatorInsDoorR(5).GetPosition.Y, 0
      ElevatorDoor5L(ElevatorFloor2(5)).SetPosition ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.X, ElevatorDoor5L(ElevatorFloor2(5)).GetPosition.Y, 0
      ElevatorDoor5R(ElevatorFloor2(5)).SetPosition ElevatorDoor5R(ElevatorFloor2(5)).GetPosition.X, ElevatorDoor5R(ElevatorFloor2(5)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator6Loop()

elevatorstart(6) = Elevator(6).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(6) = (Elevator(6).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(6) < 1 Then ElevatorFloor(6) = 1
      
      If GotoFloor(6) = ElevatorFloor(6) - 1 Then CurrentFloor(6) = ElevatorFloor(6)

      'If GotoFloor(6) <> 0 And GotoFloor(6) > CurrentFloor(6) And ElevatorDirection(6) = 0 And ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z <= 0 Then
      If GotoFloor(6) <> 0 And GotoFloor(6) > CurrentFloor(6) And ElevatorDirection(6) = 0 And ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z <= 0 Then
      ElevatorDirection(6) = 1
      OriginalLocation(6) = CurrentFloorExact(6)
      DistanceToTravel(6) = ((GotoFloor(6) * 25) + 25) - ((CurrentFloorExact(6) * 25) + 25)
      If ElevatorSync(6) = True Then
      Room(CameraFloor).Enable False
      For i6 = 1 To 10
      CallButtons(i6).Enable False
      Next i6
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(6) <> 0 And GotoFloor(6) < CurrentFloor(6) And ElevatorDirection(6) = 0 And ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z <= 0 Then
      If GotoFloor(6) <> 0 And GotoFloor(6) < CurrentFloor(6) And ElevatorDirection(6) = 0 And ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z <= 0 Then
      ElevatorDirection(6) = -1
      OriginalLocation(6) = CurrentFloorExact(6)
      DistanceToTravel(6) = ((CurrentFloorExact(6) * 25) + 25) - ((GotoFloor(6) * 25) + 25)
      If ElevatorSync(6) = True Then
      Room(CameraFloor).Enable False
      For i6 = 1 To 10
      CallButtons(i6).Enable False
      Next i6
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(6) = Int((elevatorstart(6).Y - 25) / 25)
       CurrentFloorExact(6) = (elevatorstart(6).Y - 25) / 25
       'CurrentFloor(6) = (Elevator(6).GetPosition.y / 25) - 1
       'CurrentFloorExact(6) = Int((Elevator(6).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(6)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(6)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(6) / 5)
        
        If ElevatorEnable(6) >= 0 And ElevatorDirection(6) = 1 Then
        'sound
        If ElevatorSounds(6).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(6) = 0 And GotoFloor(6) <> ElevatorFloor(6) Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevstart.wav"
        ElevatorSounds(6).Volume = 0
        'ElevatorSounds(6).maxDistance = 100
        'Call ElevatorSounds(6).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(6).ConeOutsideVolume = 0
        'Call ElevatorSounds(6).SetPosition(-20.25, Elevator1(6).GetPosition.Y + 20, -23)
        ElevatorSounds(6).Play
        ElevatorCheck(6) = 1
        End If
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(6) = 1 Then
        ElevatorSounds(6).Load App.Path + "\elevmove.wav"
        ElevatorSounds(6).Loop_ = True
        ElevatorSounds(6).Play
        End If
        'movement
        Elevator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorL(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorR(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        FloorIndicator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Plaque(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Next i6
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, (ElevatorEnable(6) / 5), 0
        'ElevatorSounds(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ''ElevatorMusic(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ElevatorEnable(6) = ElevatorEnable(6) + 0.25
        If ElevatorEnable(6) <= 15 Then StoppingDistance(6) = CurrentFloorExact(6) - OriginalLocation(6) + 0.4
        If ElevatorEnable(6) > 15 Then ElevatorEnable(6) = 15
        Destination(6) = ((OriginalLocation(6) * 25) + 25) + DistanceToTravel(6) - 35
        If GotoFloor(6) <> 0 And elevatorstart(6).Y >= (Destination(6) - (StoppingDistance(6) * 25) + 25) Then ElevatorDirection(6) = -1: ElevatorCheck(6) = 0
        End If
      
        If ElevatorEnable(6) > 0 And ElevatorDirection(6) = -1 Then
        'Sounds
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(6) = 0 And FineTune(6) = False Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Stop_
        End If
        If ElevatorSounds(6).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(6) = 0 And FineTune(6) = False Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevstop.wav"
        ElevatorSounds(6).Play
        ElevatorCheck2(6) = 1
        End If
        'Movement
        Elevator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorL(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorR(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        FloorIndicator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Plaque(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Next i6
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, (ElevatorEnable(6) / 5), 0
        
        'ElevatorSounds(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ''ElevatorMusic(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ElevatorEnable(6) = ElevatorEnable(6) - 0.25
        If ElevatorEnable(6) < 0 Then ElevatorEnable(6) = 0
        If ElevatorEnable(6) = 0 Then ElevatorDirection(6) = 0
        If GotoFloor(6) <> 0 Then ElevatorCheck(6) = 0: FineTune(6) = True
        End If
      
        If ElevatorEnable(6) <= 0 And ElevatorDirection(6) = -1 Then
        If ElevatorSounds(6).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(6) = 0 Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevstart.wav"
        ElevatorSounds(6).Volume = 0
        'ElevatorSounds(6).maxDistance = 100
        'Call ElevatorSounds(6).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(6).ConeOutsideVolume = 0
        'Call ElevatorSounds(6).SetPosition(-20.25, Elevator(6).GetPosition.Y + 20, -23)
        ElevatorSounds(6).Play
        ElevatorCheck(6) = 1
        End If
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(6) = 1 Then
        ElevatorSounds(6).Load App.Path + "\elevmove.wav"
        ElevatorSounds(6).Loop_ = True
        ElevatorSounds(6).Play
        End If
        Elevator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorL(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorR(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        FloorIndicator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Plaque(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Next i6
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, (ElevatorEnable(6) / 5), 0
        'ElevatorSounds(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ''ElevatorMusic(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ElevatorEnable(6) = ElevatorEnable(6) - 0.25
        If ElevatorEnable(6) >= -15 Then StoppingDistance(6) = OriginalLocation(6) - CurrentFloorExact(6)
        If ElevatorEnable(6) < -15 Then ElevatorEnable(6) = -15
        Destination(6) = ((OriginalLocation(6) * 25) + 25) - DistanceToTravel(6)
        If GotoFloor(6) <> 0 And elevatorstart(6).Y <= (Destination(6) + (StoppingDistance(6) * 25) + 25) Then ElevatorDirection(6) = 1: ElevatorCheck(6) = 0
        End If
      
        If ElevatorEnable(6) < 0 And ElevatorDirection(6) = 1 Then
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(6) = 0 And FineTune(6) = False Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Stop_
        End If
        If ElevatorSounds(6).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(6) = 0 And FineTune(6) = False Then
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevstop.wav"
        ElevatorSounds(6).Play
        ElevatorCheck2(6) = 1
        End If
        Elevator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorL(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        ElevatorInsDoorR(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        FloorIndicator(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Plaque(6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, (ElevatorEnable(6) / 5), 0
        Next i6
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, (ElevatorEnable(6) / 5), 0
        'ElevatorSounds(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ''ElevatorMusic(6).SetPosition -20.25, Elevator(6).GetPosition.Y + 20, -23
        ElevatorEnable(6) = ElevatorEnable(6) + 0.25
        If ElevatorEnable(6) > 0 Then ElevatorEnable(6) = 0
        If ElevatorEnable(6) = 0 Then ElevatorDirection(6) = 0
        If GotoFloor(6) <> 0 Then ElevatorCheck(6) = 0: FineTune(6) = True
        End If
      
      If FineTune(6) = True And ElevatorEnable(6) = 0 And elevatorstart(6).Y > (GotoFloor(6) * 25) + 25 + -0.3 And elevatorstart(6).Y < (GotoFloor(6) * 25) + 25 + 0.3 Then
      FineTune(6) = False
      If ElevatorSync(6) = True Then
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
      For i6 = 1 To 10
      CallButtons(i6).Enable True
      Next i6
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(6) = 0
      OpenElevator(6) = 1
      ElevatorCheck(6) = 0
      ElevatorCheck2(6) = 0
      ElevatorCheck3(6) = 0
      ElevatorCheck4(6) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(6) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(6) = True Then
      If ElevatorCheck3(6) = 0 Then
      ElevatorSounds(6).Load App.Path + "\ding1.wav"
      ElevatorSounds(6).Play
      ElevatorCheck3(6) = 1
        For i6 = -1 To 138
        Buttons6(i6).SetColor RGBA(1, 1, 1, 1)
        Next i6
      End If
      If elevatorstart(6).Y < (GotoFloor(6) * 25) + 25 Then
      Elevator(6).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(6).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(6).MoveRelative 0, 0.2, 0
      Plaque(6).MoveRelative 0, 0.2, 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, 0.2, 0
        Next i6
        FloorIndicator(6).MoveRelative 0, 0.2, 0
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(6).Y > (GotoFloor(6) * 25) + 25 Then
      Elevator(6).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(6).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(6).MoveRelative 0, -0.2, 0
      Plaque(6).MoveRelative 0, -0.2, 0
        For i6 = -1 To 138
        Buttons6(i6).MoveRelative 0, -0.2, 0
        Next i6
        FloorIndicator(6).MoveRelative 0, -0.2, 0
        If ElevatorSync(6) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(6) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z >= 4 Then OpenElevator(6) = 0: GoTo OpenElevator1
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z >= 4 Then OpenElevator(6) = 0: GoTo OpenElevator1
      If ElevatorCheck4(6) = 0 Then
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(6).Stop_
        End If
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(6).Volume = 0
        'ElevatorSounds(6).maxDistance = 1000
        'Call ElevatorSounds(6).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(6).ConeOutsideVolume = 0
        'Call ElevatorSounds(6).SetPosition(-20.25, Elevator(6).GetPosition.Y, -23)
        ElevatorSounds(6).Play
        ElevatorCheck4(6) = 1
      End If
      OpenElevatorLoc(6) = OpenElevatorLoc(6) + 0.02
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z > 1 Then OpenElevator(6) = 2
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z > 1 Then OpenElevator(6) = 2
OpenElevator1:
      End If
      
      If OpenElevator(6) = 2 Then
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z > 3 Then OpenElevator(6) = 3
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z > 3 Then OpenElevator(6) = 3
      End If
      
      If OpenElevator(6) = 3 Then
      ElevatorCheck4(6) = 0
      OpenElevatorLoc(6) = OpenElevatorLoc(6) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z < 7 And OpenElevatorLoc(6) = 0 Then OpenElevatorLoc(6) = 0.02
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z < 7 And OpenElevatorLoc(6) = 0 Then OpenElevatorLoc(6) = 0.02
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      If OpenElevatorLoc(6) <= 0 Then
      OpenElevator(6) = 0
      OpenElevatorLoc(6) = 0
      Form1.Timer6.Enabled = True
      End If
      End If
      
      If OpenElevator(6) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z <= 0 Then OpenElevator(6) = 0: GoTo OpenElevator2
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z <= 0 Then OpenElevator(6) = 0: GoTo OpenElevator2
      If ElevatorCheck4(6) = 0 Then
        If ElevatorSounds(6).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(6).Stop_
        End If
        ElevatorSounds(6).Loop_ = False
        ElevatorSounds(6).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(6).Volume = 0
        'ElevatorSounds(6).maxDistance = 1000
        'Call ElevatorSounds(6).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(6).ConeOutsideVolume = 0
        'Call ElevatorSounds(6).SetPosition(-20.25, Elevator(6).GetPosition.Y, -23)
        ElevatorSounds(6).Play
        ElevatorCheck4(6) = 1
      End If
      OpenElevatorLoc(6) = OpenElevatorLoc(6) - 0.02
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z < 3 Then OpenElevator(6) = -2
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z < 3 Then OpenElevator(6) = -2
OpenElevator2:
      End If
      
      If OpenElevator(6) = -2 Then
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z < 1 Then OpenElevator(6) = -3
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z < 1 Then OpenElevator(6) = -3
      End If
      
      If OpenElevator(6) = -3 Then
      ElevatorCheck4(6) = 0
      OpenElevatorLoc(6) = OpenElevatorLoc(6) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(6)).GetPosition.z > 0 And OpenElevatorLoc(6) >= 0 Then OpenElevatorLoc(6) = -0.02
      If ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.z > 0 And OpenElevatorLoc(6) >= 0 Then OpenElevatorLoc(6) = -0.02
      ElevatorDoor6L(ElevatorFloor2(6)).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorDoor6R(ElevatorFloor2(6)).MoveRelative -OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorL(6).MoveRelative OpenElevatorLoc(6), 0, 0
      ElevatorInsDoorR(6).MoveRelative -OpenElevatorLoc(6), 0, 0
      If OpenElevatorLoc(6) >= 0 Then
      OpenElevator(6) = 0
      OpenElevatorLoc(6) = 0
      ElevatorInsDoorL(6).SetPosition ElevatorInsDoorL(6).GetPosition.X, ElevatorInsDoorL(6).GetPosition.Y, 0
      ElevatorInsDoorR(6).SetPosition ElevatorInsDoorR(6).GetPosition.X, ElevatorInsDoorR(6).GetPosition.Y, 0
      ElevatorDoor6L(ElevatorFloor2(6)).SetPosition ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.X, ElevatorDoor6L(ElevatorFloor2(6)).GetPosition.Y, 0
      ElevatorDoor6R(ElevatorFloor2(6)).SetPosition ElevatorDoor6R(ElevatorFloor2(6)).GetPosition.X, ElevatorDoor6R(ElevatorFloor2(6)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator7Loop()

elevatorstart(7) = Elevator(7).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(7) = (Elevator(7).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(7) < 1 Then ElevatorFloor(7) = 1
      
      If GotoFloor(7) = ElevatorFloor(7) - 1 Then CurrentFloor(7) = ElevatorFloor(7)

      'If GotoFloor(7) <> 0 And GotoFloor(7) > CurrentFloor(7) And ElevatorDirection(7) = 0 And ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z <= 0 Then
      If GotoFloor(7) <> 0 And GotoFloor(7) > CurrentFloor(7) And ElevatorDirection(7) = 0 And ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z <= 0 Then
      ElevatorDirection(7) = 1
      OriginalLocation(7) = CurrentFloorExact(7)
      DistanceToTravel(7) = ((GotoFloor(7) * 25) + 25) - ((CurrentFloorExact(7) * 25) + 25)
      If ElevatorSync(7) = True Then
      Room(CameraFloor).Enable False
      For i7 = 1 To 10
      CallButtons(i7).Enable False
      Next i7
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(7) <> 0 And GotoFloor(7) < CurrentFloor(7) And ElevatorDirection(7) = 0 And ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z <= 0 Then
      If GotoFloor(7) <> 0 And GotoFloor(7) < CurrentFloor(7) And ElevatorDirection(7) = 0 And ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z <= 0 Then
      ElevatorDirection(7) = -1
      OriginalLocation(7) = CurrentFloorExact(7)
      DistanceToTravel(7) = ((CurrentFloorExact(7) * 25) + 25) - ((GotoFloor(7) * 25) + 25)
      If ElevatorSync(7) = True Then
      Room(CameraFloor).Enable False
      For i7 = 1 To 10
      CallButtons(i7).Enable False
      Next i7
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(7) = Int((elevatorstart(7).Y - 25) / 25)
       CurrentFloorExact(7) = (elevatorstart(7).Y - 25) / 25
       'CurrentFloor(7) = (Elevator(7).GetPosition.y / 25) - 1
       'CurrentFloorExact(7) = Int((Elevator(7).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(7)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(7)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(7) / 5)
        
        If ElevatorEnable(7) >= 0 And ElevatorDirection(7) = 1 Then
        'sound
        If ElevatorSounds(7).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(7) = 0 And GotoFloor(7) <> ElevatorFloor(7) Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevstart.wav"
        ElevatorSounds(7).Volume = 0
        'ElevatorSounds(7).maxDistance = 100
        'Call ElevatorSounds(7).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(7).ConeOutsideVolume = 0
        'Call ElevatorSounds(7).SetPosition(-20.25, Elevator1(7).GetPosition.Y + 20, -23)
        ElevatorSounds(7).Play
        ElevatorCheck(7) = 1
        End If
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(7) = 1 Then
        ElevatorSounds(7).Load App.Path + "\elevmove.wav"
        ElevatorSounds(7).Loop_ = True
        ElevatorSounds(7).Play
        End If
        'movement
        Elevator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorL(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorR(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        FloorIndicator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Plaque(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Next i7
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, (ElevatorEnable(7) / 5), 0
        'ElevatorSounds(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ''ElevatorMusic(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ElevatorEnable(7) = ElevatorEnable(7) + 0.25
        If ElevatorEnable(7) <= 15 Then StoppingDistance(7) = CurrentFloorExact(7) - OriginalLocation(7) + 0.4
        If ElevatorEnable(7) > 15 Then ElevatorEnable(7) = 15
        Destination(7) = ((OriginalLocation(7) * 25) + 25) + DistanceToTravel(7) - 35
        If GotoFloor(7) <> 0 And elevatorstart(7).Y >= (Destination(7) - (StoppingDistance(7) * 25) + 25) Then ElevatorDirection(7) = -1: ElevatorCheck(7) = 0
        End If
      
        If ElevatorEnable(7) > 0 And ElevatorDirection(7) = -1 Then
        'Sounds
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(7) = 0 And FineTune(7) = False Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Stop_
        End If
        If ElevatorSounds(7).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(7) = 0 And FineTune(7) = False Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevstop.wav"
        ElevatorSounds(7).Play
        ElevatorCheck2(7) = 1
        End If
        'Movement
        Elevator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorL(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorR(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        FloorIndicator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Plaque(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Next i7
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, (ElevatorEnable(7) / 5), 0
        
        'ElevatorSounds(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ''ElevatorMusic(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ElevatorEnable(7) = ElevatorEnable(7) - 0.25
        If ElevatorEnable(7) < 0 Then ElevatorEnable(7) = 0
        If ElevatorEnable(7) = 0 Then ElevatorDirection(7) = 0
        If GotoFloor(7) <> 0 Then ElevatorCheck(7) = 0: FineTune(7) = True
        End If
      
        If ElevatorEnable(7) <= 0 And ElevatorDirection(7) = -1 Then
        If ElevatorSounds(7).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(7) = 0 Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevstart.wav"
        ElevatorSounds(7).Volume = 0
        'ElevatorSounds(7).maxDistance = 100
        'Call ElevatorSounds(7).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(7).ConeOutsideVolume = 0
        'Call ElevatorSounds(7).SetPosition(-20.25, Elevator(7).GetPosition.Y + 20, -23)
        ElevatorSounds(7).Play
        ElevatorCheck(7) = 1
        End If
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(7) = 1 Then
        ElevatorSounds(7).Load App.Path + "\elevmove.wav"
        ElevatorSounds(7).Loop_ = True
        ElevatorSounds(7).Play
        End If
        Elevator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorL(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorR(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        FloorIndicator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Plaque(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Next i7
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, (ElevatorEnable(7) / 5), 0
        'ElevatorSounds(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ''ElevatorMusic(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ElevatorEnable(7) = ElevatorEnable(7) - 0.25
        If ElevatorEnable(7) >= -15 Then StoppingDistance(7) = OriginalLocation(7) - CurrentFloorExact(7)
        If ElevatorEnable(7) < -15 Then ElevatorEnable(7) = -15
        Destination(7) = ((OriginalLocation(7) * 25) + 25) - DistanceToTravel(7)
        If GotoFloor(7) <> 0 And elevatorstart(7).Y <= (Destination(7) + (StoppingDistance(7) * 25) + 25) Then ElevatorDirection(7) = 1: ElevatorCheck(7) = 0
        End If
      
        If ElevatorEnable(7) < 0 And ElevatorDirection(7) = 1 Then
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(7) = 0 And FineTune(7) = False Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Stop_
        End If
        If ElevatorSounds(7).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(7) = 0 And FineTune(7) = False Then
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevstop.wav"
        ElevatorSounds(7).Play
        ElevatorCheck2(7) = 1
        End If
        Elevator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorL(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        ElevatorInsDoorR(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        FloorIndicator(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Plaque(7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, (ElevatorEnable(7) / 5), 0
        Next i7
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, (ElevatorEnable(7) / 5), 0
        'ElevatorSounds(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ''ElevatorMusic(7).SetPosition -20.25, Elevator(7).GetPosition.Y + 20, -23
        ElevatorEnable(7) = ElevatorEnable(7) + 0.25
        If ElevatorEnable(7) > 0 Then ElevatorEnable(7) = 0
        If ElevatorEnable(7) = 0 Then ElevatorDirection(7) = 0
        If GotoFloor(7) <> 0 Then ElevatorCheck(7) = 0: FineTune(7) = True
        End If
      
      If FineTune(7) = True And ElevatorEnable(7) = 0 And elevatorstart(7).Y > (GotoFloor(7) * 25) + 25 + -0.3 And elevatorstart(7).Y < (GotoFloor(7) * 25) + 25 + 0.3 Then
      FineTune(7) = False
      If ElevatorSync(7) = True Then
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
      For i7 = 1 To 10
      CallButtons(i7).Enable True
      Next i7
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(7) = 0
      OpenElevator(7) = 1
      ElevatorCheck(7) = 0
      ElevatorCheck2(7) = 0
      ElevatorCheck3(7) = 0
      ElevatorCheck4(7) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(7) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(7) = True Then
      If ElevatorCheck3(7) = 0 Then
      ElevatorSounds(7).Load App.Path + "\ding1.wav"
      ElevatorSounds(7).Play
      ElevatorCheck3(7) = 1
        For i7 = -1 To 138
        Buttons7(i7).SetColor RGBA(1, 1, 1, 1)
        Next i7
      End If
      If elevatorstart(7).Y < (GotoFloor(7) * 25) + 25 Then
      Elevator(7).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(7).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(7).MoveRelative 0, 0.2, 0
      Plaque(7).MoveRelative 0, 0.2, 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, 0.2, 0
        Next i7
        FloorIndicator(7).MoveRelative 0, 0.2, 0
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(7).Y > (GotoFloor(7) * 25) + 25 Then
      Elevator(7).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(7).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(7).MoveRelative 0, -0.2, 0
      Plaque(7).MoveRelative 0, -0.2, 0
        For i7 = -1 To 138
        Buttons7(i7).MoveRelative 0, -0.2, 0
        Next i7
        FloorIndicator(7).MoveRelative 0, -0.2, 0
        If ElevatorSync(7) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(7) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z >= 4 Then OpenElevator(7) = 0: GoTo OpenElevator1
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z >= 4 Then OpenElevator(7) = 0: GoTo OpenElevator1
      If ElevatorCheck4(7) = 0 Then
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(7).Stop_
        End If
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(7).Volume = 0
        'ElevatorSounds(7).maxDistance = 1000
        'Call ElevatorSounds(7).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(7).ConeOutsideVolume = 0
        'Call ElevatorSounds(7).SetPosition(-20.25, Elevator(7).GetPosition.Y, -23)
        ElevatorSounds(7).Play
        ElevatorCheck4(7) = 1
      End If
      OpenElevatorLoc(7) = OpenElevatorLoc(7) + 0.02
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z > 1 Then OpenElevator(7) = 2
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z > 1 Then OpenElevator(7) = 2
OpenElevator1:
      End If
      
      If OpenElevator(7) = 2 Then
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z > 3 Then OpenElevator(7) = 3
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z > 3 Then OpenElevator(7) = 3
      End If
      
      If OpenElevator(7) = 3 Then
      ElevatorCheck4(7) = 0
      OpenElevatorLoc(7) = OpenElevatorLoc(7) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z < 7 And OpenElevatorLoc(7) = 0 Then OpenElevatorLoc(7) = 0.02
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z < 7 And OpenElevatorLoc(7) = 0 Then OpenElevatorLoc(7) = 0.02
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      If OpenElevatorLoc(7) <= 0 Then
      OpenElevator(7) = 0
      OpenElevatorLoc(7) = 0
      Form1.Timer7.Enabled = True
      End If
      End If
      
      If OpenElevator(7) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z <= 0 Then OpenElevator(7) = 0: GoTo OpenElevator2
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z <= 0 Then OpenElevator(7) = 0: GoTo OpenElevator2
      If ElevatorCheck4(7) = 0 Then
        If ElevatorSounds(7).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(7).Stop_
        End If
        ElevatorSounds(7).Loop_ = False
        ElevatorSounds(7).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(7).Volume = 0
        'ElevatorSounds(7).maxDistance = 1000
        'Call ElevatorSounds(7).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(7).ConeOutsideVolume = 0
        'Call ElevatorSounds(7).SetPosition(-20.25, Elevator(7).GetPosition.Y, -23)
        ElevatorSounds(7).Play
        ElevatorCheck4(7) = 1
      End If
      OpenElevatorLoc(7) = OpenElevatorLoc(7) - 0.02
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z < 3 Then OpenElevator(7) = -2
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z < 3 Then OpenElevator(7) = -2
OpenElevator2:
      End If
      
      If OpenElevator(7) = -2 Then
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z < 1 Then OpenElevator(7) = -3
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z < 1 Then OpenElevator(7) = -3
      End If
      
      If OpenElevator(7) = -3 Then
      ElevatorCheck4(7) = 0
      OpenElevatorLoc(7) = OpenElevatorLoc(7) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(7)).GetPosition.z > 0 And OpenElevatorLoc(7) >= 0 Then OpenElevatorLoc(7) = -0.02
      If ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.z > 0 And OpenElevatorLoc(7) >= 0 Then OpenElevatorLoc(7) = -0.02
      ElevatorDoor7L(ElevatorFloor2(7)).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorDoor7R(ElevatorFloor2(7)).MoveRelative -OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorL(7).MoveRelative OpenElevatorLoc(7), 0, 0
      ElevatorInsDoorR(7).MoveRelative -OpenElevatorLoc(7), 0, 0
      If OpenElevatorLoc(7) >= 0 Then
      OpenElevator(7) = 0
      OpenElevatorLoc(7) = 0
      ElevatorInsDoorL(7).SetPosition ElevatorInsDoorL(7).GetPosition.X, ElevatorInsDoorL(7).GetPosition.Y, 0
      ElevatorInsDoorR(7).SetPosition ElevatorInsDoorR(7).GetPosition.X, ElevatorInsDoorR(7).GetPosition.Y, 0
      ElevatorDoor7L(ElevatorFloor2(7)).SetPosition ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.X, ElevatorDoor7L(ElevatorFloor2(7)).GetPosition.Y, 0
      ElevatorDoor7R(ElevatorFloor2(7)).SetPosition ElevatorDoor7R(ElevatorFloor2(7)).GetPosition.X, ElevatorDoor7R(ElevatorFloor2(7)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator8Loop()

elevatorstart(8) = Elevator(8).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(8) = (Elevator(8).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(8) < 1 Then ElevatorFloor(8) = 1
      
      If GotoFloor(8) = ElevatorFloor(8) - 1 Then CurrentFloor(8) = ElevatorFloor(8)

      'If GotoFloor(8) <> 0 And GotoFloor(8) > CurrentFloor(8) And ElevatorDirection(8) = 0 And ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z <= 0 Then
      If GotoFloor(8) <> 0 And GotoFloor(8) > CurrentFloor(8) And ElevatorDirection(8) = 0 And ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z <= 0 Then
      ElevatorDirection(8) = 1
      OriginalLocation(8) = CurrentFloorExact(8)
      DistanceToTravel(8) = ((GotoFloor(8) * 25) + 25) - ((CurrentFloorExact(8) * 25) + 25)
      If ElevatorSync(8) = True Then
      Room(CameraFloor).Enable False
      For i8 = 1 To 10
      CallButtons(i8).Enable False
      Next i8
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(8) <> 0 And GotoFloor(8) < CurrentFloor(8) And ElevatorDirection(8) = 0 And ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z <= 0 Then
      If GotoFloor(8) <> 0 And GotoFloor(8) < CurrentFloor(8) And ElevatorDirection(8) = 0 And ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z <= 0 Then
      ElevatorDirection(8) = -1
      OriginalLocation(8) = CurrentFloorExact(8)
      DistanceToTravel(8) = ((CurrentFloorExact(8) * 25) + 25) - ((GotoFloor(8) * 25) + 25)
      If ElevatorSync(8) = True Then
      Room(CameraFloor).Enable False
      For i8 = 1 To 10
      CallButtons(i8).Enable False
      Next i8
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(8) = Int((elevatorstart(8).Y - 25) / 25)
       CurrentFloorExact(8) = (elevatorstart(8).Y - 25) / 25
       'CurrentFloor(8) = (Elevator(8).GetPosition.y / 25) - 1
       'CurrentFloorExact(8) = Int((Elevator(8).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(8)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(8)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(8) / 5)
        
        If ElevatorEnable(8) >= 0 And ElevatorDirection(8) = 1 Then
        'sound
        If ElevatorSounds(8).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(8) = 0 And GotoFloor(8) <> ElevatorFloor(8) Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevstart.wav"
        ElevatorSounds(8).Volume = 0
        'ElevatorSounds(8).maxDistance = 100
        'Call ElevatorSounds(8).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(8).ConeOutsideVolume = 0
        'Call ElevatorSounds(8).SetPosition(-20.25, Elevator1(8).GetPosition.Y + 20, -23)
        ElevatorSounds(8).Play
        ElevatorCheck(8) = 1
        End If
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(8) = 1 Then
        ElevatorSounds(8).Load App.Path + "\elevmove.wav"
        ElevatorSounds(8).Loop_ = True
        ElevatorSounds(8).Play
        End If
        'movement
        Elevator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorL(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorR(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        FloorIndicator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Plaque(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Next i8
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, (ElevatorEnable(8) / 5), 0
        'ElevatorSounds(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ''ElevatorMusic(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ElevatorEnable(8) = ElevatorEnable(8) + 0.25
        If ElevatorEnable(8) <= 15 Then StoppingDistance(8) = CurrentFloorExact(8) - OriginalLocation(8) + 0.4
        If ElevatorEnable(8) > 15 Then ElevatorEnable(8) = 15
        Destination(8) = ((OriginalLocation(8) * 25) + 25) + DistanceToTravel(8) - 35
        If GotoFloor(8) <> 0 And elevatorstart(8).Y >= (Destination(8) - (StoppingDistance(8) * 25) + 25) Then ElevatorDirection(8) = -1: ElevatorCheck(8) = 0
        End If
      
        If ElevatorEnable(8) > 0 And ElevatorDirection(8) = -1 Then
        'Sounds
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(8) = 0 And FineTune(8) = False Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Stop_
        End If
        If ElevatorSounds(8).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(8) = 0 And FineTune(8) = False Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevstop.wav"
        ElevatorSounds(8).Play
        ElevatorCheck2(8) = 1
        End If
        'Movement
        Elevator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorL(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorR(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        FloorIndicator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Plaque(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Next i8
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, (ElevatorEnable(8) / 5), 0
        
        'ElevatorSounds(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ''ElevatorMusic(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ElevatorEnable(8) = ElevatorEnable(8) - 0.25
        If ElevatorEnable(8) < 0 Then ElevatorEnable(8) = 0
        If ElevatorEnable(8) = 0 Then ElevatorDirection(8) = 0
        If GotoFloor(8) <> 0 Then ElevatorCheck(8) = 0: FineTune(8) = True
        End If
      
        If ElevatorEnable(8) <= 0 And ElevatorDirection(8) = -1 Then
        If ElevatorSounds(8).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(8) = 0 Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevstart.wav"
        ElevatorSounds(8).Volume = 0
        'ElevatorSounds(8).maxDistance = 100
        'Call ElevatorSounds(8).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(8).ConeOutsideVolume = 0
        'Call ElevatorSounds(8).SetPosition(-20.25, Elevator(8).GetPosition.Y + 20, -23)
        ElevatorSounds(8).Play
        ElevatorCheck(8) = 1
        End If
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(8) = 1 Then
        ElevatorSounds(8).Load App.Path + "\elevmove.wav"
        ElevatorSounds(8).Loop_ = True
        ElevatorSounds(8).Play
        End If
        Elevator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorL(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorR(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        FloorIndicator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Plaque(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Next i8
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, (ElevatorEnable(8) / 5), 0
        'ElevatorSounds(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ''ElevatorMusic(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ElevatorEnable(8) = ElevatorEnable(8) - 0.25
        If ElevatorEnable(8) >= -15 Then StoppingDistance(8) = OriginalLocation(8) - CurrentFloorExact(8)
        If ElevatorEnable(8) < -15 Then ElevatorEnable(8) = -15
        Destination(8) = ((OriginalLocation(8) * 25) + 25) - DistanceToTravel(8)
        If GotoFloor(8) <> 0 And elevatorstart(8).Y <= (Destination(8) + (StoppingDistance(8) * 25) + 25) Then ElevatorDirection(8) = 1: ElevatorCheck(8) = 0
        End If
      
        If ElevatorEnable(8) < 0 And ElevatorDirection(8) = 1 Then
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(8) = 0 And FineTune(8) = False Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Stop_
        End If
        If ElevatorSounds(8).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(8) = 0 And FineTune(8) = False Then
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevstop.wav"
        ElevatorSounds(8).Play
        ElevatorCheck2(8) = 1
        End If
        Elevator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorL(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        ElevatorInsDoorR(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        FloorIndicator(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Plaque(8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, (ElevatorEnable(8) / 5), 0
        Next i8
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, (ElevatorEnable(8) / 5), 0
        'ElevatorSounds(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ''ElevatorMusic(8).SetPosition -20.25, Elevator(8).GetPosition.Y + 20, -23
        ElevatorEnable(8) = ElevatorEnable(8) + 0.25
        If ElevatorEnable(8) > 0 Then ElevatorEnable(8) = 0
        If ElevatorEnable(8) = 0 Then ElevatorDirection(8) = 0
        If GotoFloor(8) <> 0 Then ElevatorCheck(8) = 0: FineTune(8) = True
        End If
      
      If FineTune(8) = True And ElevatorEnable(8) = 0 And elevatorstart(8).Y > (GotoFloor(8) * 25) + 25 + -0.3 And elevatorstart(8).Y < (GotoFloor(8) * 25) + 25 + 0.3 Then
      FineTune(8) = False
      If ElevatorSync(8) = True Then
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
      For i8 = 1 To 10
      CallButtons(i8).Enable True
      Next i8
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(8) = 0
      OpenElevator(8) = 1
      ElevatorCheck(8) = 0
      ElevatorCheck2(8) = 0
      ElevatorCheck3(8) = 0
      ElevatorCheck4(8) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(8) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(8) = True Then
      If ElevatorCheck3(8) = 0 Then
      ElevatorSounds(8).Load App.Path + "\ding1.wav"
      ElevatorSounds(8).Play
      ElevatorCheck3(8) = 1
        For i8 = -1 To 138
        Buttons8(i8).SetColor RGBA(1, 1, 1, 1)
        Next i8
      End If
      If elevatorstart(8).Y < (GotoFloor(8) * 25) + 25 Then
      Elevator(8).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(8).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(8).MoveRelative 0, 0.2, 0
      Plaque(8).MoveRelative 0, 0.2, 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, 0.2, 0
        Next i8
        FloorIndicator(8).MoveRelative 0, 0.2, 0
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(8).Y > (GotoFloor(8) * 25) + 25 Then
      Elevator(8).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(8).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(8).MoveRelative 0, -0.2, 0
      Plaque(8).MoveRelative 0, -0.2, 0
        For i8 = -1 To 138
        Buttons8(i8).MoveRelative 0, -0.2, 0
        Next i8
        FloorIndicator(8).MoveRelative 0, -0.2, 0
        If ElevatorSync(8) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(8) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z >= 4 Then OpenElevator(8) = 0: GoTo OpenElevator1
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z >= 4 Then OpenElevator(8) = 0: GoTo OpenElevator1
      If ElevatorCheck4(8) = 0 Then
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(8).Stop_
        End If
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(8).Volume = 0
        'ElevatorSounds(8).maxDistance = 1000
        'Call ElevatorSounds(8).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(8).ConeOutsideVolume = 0
        'Call ElevatorSounds(8).SetPosition(-20.25, Elevator(8).GetPosition.Y, -23)
        ElevatorSounds(8).Play
        ElevatorCheck4(8) = 1
      End If
      OpenElevatorLoc(8) = OpenElevatorLoc(8) + 0.02
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z > 1 Then OpenElevator(8) = 2
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z > 1 Then OpenElevator(8) = 2
OpenElevator1:
      End If
      
      If OpenElevator(8) = 2 Then
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z > 3 Then OpenElevator(8) = 3
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z > 3 Then OpenElevator(8) = 3
      End If
      
      If OpenElevator(8) = 3 Then
      ElevatorCheck4(8) = 0
      OpenElevatorLoc(8) = OpenElevatorLoc(8) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z < 7 And OpenElevatorLoc(8) = 0 Then OpenElevatorLoc(8) = 0.02
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z < 7 And OpenElevatorLoc(8) = 0 Then OpenElevatorLoc(8) = 0.02
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      If OpenElevatorLoc(8) <= 0 Then
      OpenElevator(8) = 0
      OpenElevatorLoc(8) = 0
      Form1.Timer8.Enabled = True
      End If
      End If
      
      If OpenElevator(8) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z <= 0 Then OpenElevator(8) = 0: GoTo OpenElevator2
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z <= 0 Then OpenElevator(8) = 0: GoTo OpenElevator2
      If ElevatorCheck4(8) = 0 Then
        If ElevatorSounds(8).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(8).Stop_
        End If
        ElevatorSounds(8).Loop_ = False
        ElevatorSounds(8).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(8).Volume = 0
        'ElevatorSounds(8).maxDistance = 1000
        'Call ElevatorSounds(8).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(8).ConeOutsideVolume = 0
        'Call ElevatorSounds(8).SetPosition(-20.25, Elevator(8).GetPosition.Y, -23)
        ElevatorSounds(8).Play
        ElevatorCheck4(8) = 1
      End If
      OpenElevatorLoc(8) = OpenElevatorLoc(8) - 0.02
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z < 3 Then OpenElevator(8) = -2
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z < 3 Then OpenElevator(8) = -2
OpenElevator2:
      End If
      
      If OpenElevator(8) = -2 Then
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z < 1 Then OpenElevator(8) = -3
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z < 1 Then OpenElevator(8) = -3
      End If
      
      If OpenElevator(8) = -3 Then
      ElevatorCheck4(8) = 0
      OpenElevatorLoc(8) = OpenElevatorLoc(8) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(8)).GetPosition.z > 0 And OpenElevatorLoc(8) >= 0 Then OpenElevatorLoc(8) = -0.02
      If ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.z > 0 And OpenElevatorLoc(8) >= 0 Then OpenElevatorLoc(8) = -0.02
      ElevatorDoor8L(ElevatorFloor2(8)).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorDoor8R(ElevatorFloor2(8)).MoveRelative -OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorL(8).MoveRelative OpenElevatorLoc(8), 0, 0
      ElevatorInsDoorR(8).MoveRelative -OpenElevatorLoc(8), 0, 0
      If OpenElevatorLoc(8) >= 0 Then
      OpenElevator(8) = 0
      OpenElevatorLoc(8) = 0
      ElevatorInsDoorL(8).SetPosition ElevatorInsDoorL(8).GetPosition.X, ElevatorInsDoorL(8).GetPosition.Y, 0
      ElevatorInsDoorR(8).SetPosition ElevatorInsDoorR(8).GetPosition.X, ElevatorInsDoorR(8).GetPosition.Y, 0
      ElevatorDoor8L(ElevatorFloor2(8)).SetPosition ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.X, ElevatorDoor8L(ElevatorFloor2(8)).GetPosition.Y, 0
      ElevatorDoor8R(ElevatorFloor2(8)).SetPosition ElevatorDoor8R(ElevatorFloor2(8)).GetPosition.X, ElevatorDoor8R(ElevatorFloor2(8)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator9Loop()

elevatorstart(9) = Elevator(9).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(9) = (Elevator(9).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(9) < 1 Then ElevatorFloor(9) = 1
      
      If GotoFloor(9) = ElevatorFloor(9) - 1 Then CurrentFloor(9) = ElevatorFloor(9)

      'If GotoFloor(9) <> 0 And GotoFloor(9) > CurrentFloor(9) And ElevatorDirection(9) = 0 And ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z <= 0 Then
      If GotoFloor(9) <> 0 And GotoFloor(9) > CurrentFloor(9) And ElevatorDirection(9) = 0 And ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z <= 0 Then
      ElevatorDirection(9) = 1
      OriginalLocation(9) = CurrentFloorExact(9)
      DistanceToTravel(9) = ((GotoFloor(9) * 25) + 25) - ((CurrentFloorExact(9) * 25) + 25)
      If ElevatorSync(9) = True Then
      Room(CameraFloor).Enable False
      For i9 = 1 To 10
      CallButtons(i9).Enable False
      Next i9
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(9) <> 0 And GotoFloor(9) < CurrentFloor(9) And ElevatorDirection(9) = 0 And ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z <= 0 Then
      If GotoFloor(9) <> 0 And GotoFloor(9) < CurrentFloor(9) And ElevatorDirection(9) = 0 And ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z <= 0 Then
      ElevatorDirection(9) = -1
      OriginalLocation(9) = CurrentFloorExact(9)
      DistanceToTravel(9) = ((CurrentFloorExact(9) * 25) + 25) - ((GotoFloor(9) * 25) + 25)
      If ElevatorSync(9) = True Then
      Room(CameraFloor).Enable False
      For i9 = 1 To 10
      CallButtons(i9).Enable False
      Next i9
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(9) = Int((elevatorstart(9).Y - 25) / 25)
       CurrentFloorExact(9) = (elevatorstart(9).Y - 25) / 25
       'CurrentFloor(9) = (Elevator(9).GetPosition.y / 25) - 1
       'CurrentFloorExact(9) = Int((Elevator(9).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(9)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(9)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(9) / 5)
        
        If ElevatorEnable(9) >= 0 And ElevatorDirection(9) = 1 Then
        'sound
        If ElevatorSounds(9).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(9) = 0 And GotoFloor(9) <> ElevatorFloor(9) Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevstart.wav"
        ElevatorSounds(9).Volume = 0
        'ElevatorSounds(9).maxDistance = 100
        'Call ElevatorSounds(9).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(9).ConeOutsideVolume = 0
        'Call ElevatorSounds(9).SetPosition(-20.25, Elevator1(9).GetPosition.Y + 20, -23)
        ElevatorSounds(9).Play
        ElevatorCheck(9) = 1
        End If
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(9) = 1 Then
        ElevatorSounds(9).Load App.Path + "\elevmove.wav"
        ElevatorSounds(9).Loop_ = True
        ElevatorSounds(9).Play
        End If
        'movement
        Elevator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorL(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorR(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        FloorIndicator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Plaque(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Next i9
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, (ElevatorEnable(9) / 5), 0
        'ElevatorSounds(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ''ElevatorMusic(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ElevatorEnable(9) = ElevatorEnable(9) + 0.25
        If ElevatorEnable(9) <= 15 Then StoppingDistance(9) = CurrentFloorExact(9) - OriginalLocation(9) + 0.4
        If ElevatorEnable(9) > 15 Then ElevatorEnable(9) = 15
        Destination(9) = ((OriginalLocation(9) * 25) + 25) + DistanceToTravel(9) - 35
        If GotoFloor(9) <> 0 And elevatorstart(9).Y >= (Destination(9) - (StoppingDistance(9) * 25) + 25) Then ElevatorDirection(9) = -1: ElevatorCheck(9) = 0
        End If
      
        If ElevatorEnable(9) > 0 And ElevatorDirection(9) = -1 Then
        'Sounds
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(9) = 0 And FineTune(9) = False Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Stop_
        End If
        If ElevatorSounds(9).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(9) = 0 And FineTune(9) = False Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevstop.wav"
        ElevatorSounds(9).Play
        ElevatorCheck2(9) = 1
        End If
        'Movement
        Elevator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorL(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorR(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        FloorIndicator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Plaque(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Next i9
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, (ElevatorEnable(9) / 5), 0
        
        'ElevatorSounds(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ''ElevatorMusic(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ElevatorEnable(9) = ElevatorEnable(9) - 0.25
        If ElevatorEnable(9) < 0 Then ElevatorEnable(9) = 0
        If ElevatorEnable(9) = 0 Then ElevatorDirection(9) = 0
        If GotoFloor(9) <> 0 Then ElevatorCheck(9) = 0: FineTune(9) = True
        End If
      
        If ElevatorEnable(9) <= 0 And ElevatorDirection(9) = -1 Then
        If ElevatorSounds(9).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(9) = 0 Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevstart.wav"
        ElevatorSounds(9).Volume = 0
        'ElevatorSounds(9).maxDistance = 100
        'Call ElevatorSounds(9).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(9).ConeOutsideVolume = 0
        'Call ElevatorSounds(9).SetPosition(-20.25, Elevator(9).GetPosition.Y + 20, -23)
        ElevatorSounds(9).Play
        ElevatorCheck(9) = 1
        End If
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(9) = 1 Then
        ElevatorSounds(9).Load App.Path + "\elevmove.wav"
        ElevatorSounds(9).Loop_ = True
        ElevatorSounds(9).Play
        End If
        Elevator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorL(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorR(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        FloorIndicator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Plaque(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Next i9
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, (ElevatorEnable(9) / 5), 0
        'ElevatorSounds(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ''ElevatorMusic(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ElevatorEnable(9) = ElevatorEnable(9) - 0.25
        If ElevatorEnable(9) >= -15 Then StoppingDistance(9) = OriginalLocation(9) - CurrentFloorExact(9)
        If ElevatorEnable(9) < -15 Then ElevatorEnable(9) = -15
        Destination(9) = ((OriginalLocation(9) * 25) + 25) - DistanceToTravel(9)
        If GotoFloor(9) <> 0 And elevatorstart(9).Y <= (Destination(9) + (StoppingDistance(9) * 25) + 25) Then ElevatorDirection(9) = 1: ElevatorCheck(9) = 0
        End If
      
        If ElevatorEnable(9) < 0 And ElevatorDirection(9) = 1 Then
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(9) = 0 And FineTune(9) = False Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Stop_
        End If
        If ElevatorSounds(9).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(9) = 0 And FineTune(9) = False Then
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevstop.wav"
        ElevatorSounds(9).Play
        ElevatorCheck2(9) = 1
        End If
        Elevator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorL(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        ElevatorInsDoorR(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        FloorIndicator(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Plaque(9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, (ElevatorEnable(9) / 5), 0
        Next i9
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, (ElevatorEnable(9) / 5), 0
        'ElevatorSounds(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ''ElevatorMusic(9).SetPosition -20.25, Elevator(9).GetPosition.Y + 20, -23
        ElevatorEnable(9) = ElevatorEnable(9) + 0.25
        If ElevatorEnable(9) > 0 Then ElevatorEnable(9) = 0
        If ElevatorEnable(9) = 0 Then ElevatorDirection(9) = 0
        If GotoFloor(9) <> 0 Then ElevatorCheck(9) = 0: FineTune(9) = True
        End If
      
      If FineTune(9) = True And ElevatorEnable(9) = 0 And elevatorstart(9).Y > (GotoFloor(9) * 25) + 25 + -0.3 And elevatorstart(9).Y < (GotoFloor(9) * 25) + 25 + 0.3 Then
      FineTune(9) = False
      If ElevatorSync(9) = True Then
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
      For i9 = 1 To 10
      CallButtons(i9).Enable True
      Next i9
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(9) = 0
      OpenElevator(9) = 1
      ElevatorCheck(9) = 0
      ElevatorCheck2(9) = 0
      ElevatorCheck3(9) = 0
      ElevatorCheck4(9) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(9) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(9) = True Then
      If ElevatorCheck3(9) = 0 Then
      ElevatorSounds(9).Load App.Path + "\ding1.wav"
      ElevatorSounds(9).Play
      ElevatorCheck3(9) = 1
        For i9 = -1 To 138
        Buttons9(i9).SetColor RGBA(1, 1, 1, 1)
        Next i9
      End If
      If elevatorstart(9).Y < (GotoFloor(9) * 25) + 25 Then
      Elevator(9).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(9).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(9).MoveRelative 0, 0.2, 0
      Plaque(9).MoveRelative 0, 0.2, 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, 0.2, 0
        Next i9
        FloorIndicator(9).MoveRelative 0, 0.2, 0
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(9).Y > (GotoFloor(9) * 25) + 25 Then
      Elevator(9).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(9).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(9).MoveRelative 0, -0.2, 0
      Plaque(9).MoveRelative 0, -0.2, 0
        For i9 = -1 To 138
        Buttons9(i9).MoveRelative 0, -0.2, 0
        Next i9
        FloorIndicator(9).MoveRelative 0, -0.2, 0
        If ElevatorSync(9) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(9) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z >= 4 Then OpenElevator(9) = 0: GoTo OpenElevator1
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z >= 4 Then OpenElevator(9) = 0: GoTo OpenElevator1
      If ElevatorCheck4(9) = 0 Then
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(9).Stop_
        End If
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(9).Volume = 0
        'ElevatorSounds(9).maxDistance = 1000
        'Call ElevatorSounds(9).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(9).ConeOutsideVolume = 0
        'Call ElevatorSounds(9).SetPosition(-20.25, Elevator(9).GetPosition.Y, -23)
        ElevatorSounds(9).Play
        ElevatorCheck4(9) = 1
      End If
      OpenElevatorLoc(9) = OpenElevatorLoc(9) + 0.02
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z > 1 Then OpenElevator(9) = 2
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z > 1 Then OpenElevator(9) = 2
OpenElevator1:
      End If
      
      If OpenElevator(9) = 2 Then
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z > 3 Then OpenElevator(9) = 3
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z > 3 Then OpenElevator(9) = 3
      End If
      
      If OpenElevator(9) = 3 Then
      ElevatorCheck4(9) = 0
      OpenElevatorLoc(9) = OpenElevatorLoc(9) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z < 7 And OpenElevatorLoc(9) = 0 Then OpenElevatorLoc(9) = 0.02
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z < 7 And OpenElevatorLoc(9) = 0 Then OpenElevatorLoc(9) = 0.02
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      If OpenElevatorLoc(9) <= 0 Then
      OpenElevator(9) = 0
      OpenElevatorLoc(9) = 0
      Form1.Timer9.Enabled = True
      End If
      End If
      
      If OpenElevator(9) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z <= 0 Then OpenElevator(9) = 0: GoTo OpenElevator2
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z <= 0 Then OpenElevator(9) = 0: GoTo OpenElevator2
      If ElevatorCheck4(9) = 0 Then
        If ElevatorSounds(9).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(9).Stop_
        End If
        ElevatorSounds(9).Loop_ = False
        ElevatorSounds(9).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(9).Volume = 0
        'ElevatorSounds(9).maxDistance = 1000
        'Call ElevatorSounds(9).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(9).ConeOutsideVolume = 0
        'Call ElevatorSounds(9).SetPosition(-20.25, Elevator(9).GetPosition.Y, -23)
        ElevatorSounds(9).Play
        ElevatorCheck4(9) = 1
      End If
      OpenElevatorLoc(9) = OpenElevatorLoc(9) - 0.02
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z < 3 Then OpenElevator(9) = -2
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z < 3 Then OpenElevator(9) = -2
OpenElevator2:
      End If
      
      If OpenElevator(9) = -2 Then
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z < 1 Then OpenElevator(9) = -3
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z < 1 Then OpenElevator(9) = -3
      End If
      
      If OpenElevator(9) = -3 Then
      ElevatorCheck4(9) = 0
      OpenElevatorLoc(9) = OpenElevatorLoc(9) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(9)).GetPosition.z > 0 And OpenElevatorLoc(9) >= 0 Then OpenElevatorLoc(9) = -0.02
      If ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.z > 0 And OpenElevatorLoc(9) >= 0 Then OpenElevatorLoc(9) = -0.02
      ElevatorDoor9L(ElevatorFloor2(9)).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorDoor9R(ElevatorFloor2(9)).MoveRelative -OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorL(9).MoveRelative OpenElevatorLoc(9), 0, 0
      ElevatorInsDoorR(9).MoveRelative -OpenElevatorLoc(9), 0, 0
      If OpenElevatorLoc(9) >= 0 Then
      OpenElevator(9) = 0
      OpenElevatorLoc(9) = 0
      ElevatorInsDoorL(9).SetPosition ElevatorInsDoorL(9).GetPosition.X, ElevatorInsDoorL(9).GetPosition.Y, 0
      ElevatorInsDoorR(9).SetPosition ElevatorInsDoorR(9).GetPosition.X, ElevatorInsDoorR(9).GetPosition.Y, 0
      ElevatorDoor9L(ElevatorFloor2(9)).SetPosition ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.X, ElevatorDoor9L(ElevatorFloor2(9)).GetPosition.Y, 0
      ElevatorDoor9R(ElevatorFloor2(9)).SetPosition ElevatorDoor9R(ElevatorFloor2(9)).GetPosition.X, ElevatorDoor9R(ElevatorFloor2(9)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub

Sub Elevator10Loop()

elevatorstart(10) = Elevator(10).GetPosition

'Find the floor that the elevator's on
ElevatorFloor(10) = (Elevator(10).GetPosition.Y - 25) / 25
      
      'If elevator goes below floor 2, then set elevatorfloor as 1
      If ElevatorFloor(10) < 1 Then ElevatorFloor(10) = 1
      
      If GotoFloor(10) = ElevatorFloor(10) - 1 Then CurrentFloor(10) = ElevatorFloor(10)

      'If GotoFloor(10) <> 0 And GotoFloor(10) > CurrentFloor(10) And ElevatorDirection(10) = 0 And ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z <= 0 Then
      If GotoFloor(10) <> 0 And GotoFloor(10) > CurrentFloor(10) And ElevatorDirection(10) = 0 And ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z <= 0 Then
      ElevatorDirection(10) = 1
      OriginalLocation(10) = CurrentFloorExact(10)
      DistanceToTravel(10) = ((GotoFloor(10) * 25) + 25) - ((CurrentFloorExact(10) * 25) + 25)
      If ElevatorSync(10) = True Then
      Room(CameraFloor).Enable False
      For i10 = 1 To 10
      CallButtons(i10).Enable False
      Next i10
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      'If GotoFloor(10) <> 0 And GotoFloor(10) < CurrentFloor(10) And ElevatorDirection(10) = 0 And ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z <= 0 Then
      If GotoFloor(10) <> 0 And GotoFloor(10) < CurrentFloor(10) And ElevatorDirection(10) = 0 And ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z <= 0 Then
      ElevatorDirection(10) = -1
      OriginalLocation(10) = CurrentFloorExact(10)
      DistanceToTravel(10) = ((CurrentFloorExact(10) * 25) + 25) - ((GotoFloor(10) * 25) + 25)
      If ElevatorSync(10) = True Then
      Room(CameraFloor).Enable False
      For i10 = 1 To 10
      CallButtons(i10).Enable False
      Next i10
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
      DestroyObjects (CameraFloor)
      ShaftsFloor(CameraFloor).Enable False
      Atmos.SkyBox_Enable False
      Stairs(CameraFloor).Enable False
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable False
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable False
      End If
      End If
      
       CurrentFloor(10) = Int((elevatorstart(10).Y - 25) / 25)
       CurrentFloorExact(10) = (elevatorstart(10).Y - 25) / 25
       'CurrentFloor(10) = (Elevator(10).GetPosition.y / 25) - 1
       'CurrentFloorExact(10) = Int((Elevator(10).GetPosition.y / 25) - 1)

       'Form2.Text1.Text = "Sound Location=7.75,20,7 " + vbCrLf + "Elevator Floor=" + Str$(ElevatorFloor(10)) + vbCrLf + "Camera Floor=" + Str$(CameraFloor) + vbCrLf + "Current Location= " + Str$(Int(Camera.GetPosition.x)) + "," + Str$(Int(Camera.GetPosition.y)) + "," + Str$(Int(Camera.GetPosition.z)) + vbCrLf + "Distance to Travel=" + Str$(DistanceToTravel(10)) + vbCrLf + "Destination=" + Str$(Destination) + vbCrLf + "Rate=" + Str$(ElevatorEnable(10) / 5)
        
        If ElevatorEnable(10) >= 0 And ElevatorDirection(10) = 1 Then
        'sound
        If ElevatorSounds(10).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(10) = 0 And GotoFloor(10) <> ElevatorFloor(10) Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevstart.wav"
        ElevatorSounds(10).Volume = 0
        'ElevatorSounds(10).maxDistance = 100
        'Call ElevatorSounds(10).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(10).ConeOutsideVolume = 0
        'Call ElevatorSounds(10).SetPosition(-20.25, Elevator1(10).GetPosition.Y + 20, -23)
        ElevatorSounds(10).Play
        ElevatorCheck(10) = 1
        End If
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(10) = 1 Then
        ElevatorSounds(10).Load App.Path + "\elevmove.wav"
        ElevatorSounds(10).Loop_ = True
        ElevatorSounds(10).Play
        End If
        'movement
        Elevator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorL(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorR(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        FloorIndicator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Plaque(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Next i10
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, (ElevatorEnable(10) / 5), 0
        'ElevatorSounds(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ''ElevatorMusic(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ElevatorEnable(10) = ElevatorEnable(10) + 0.25
        If ElevatorEnable(10) <= 15 Then StoppingDistance(10) = CurrentFloorExact(10) - OriginalLocation(10) + 0.4
        If ElevatorEnable(10) > 15 Then ElevatorEnable(10) = 15
        Destination(10) = ((OriginalLocation(10) * 25) + 25) + DistanceToTravel(10) - 35
        If GotoFloor(10) <> 0 And elevatorstart(10).Y >= (Destination(10) - (StoppingDistance(10) * 25) + 25) Then ElevatorDirection(10) = -1: ElevatorCheck(10) = 0
        End If
      
        If ElevatorEnable(10) > 0 And ElevatorDirection(10) = -1 Then
        'Sounds
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(10) = 0 And FineTune(10) = False Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Stop_
        End If
        If ElevatorSounds(10).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(10) = 0 And FineTune(10) = False Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevstop.wav"
        ElevatorSounds(10).Play
        ElevatorCheck2(10) = 1
        End If
        'Movement
        Elevator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorL(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorR(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        FloorIndicator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Plaque(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Next i10
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, (ElevatorEnable(10) / 5), 0
        
        'ElevatorSounds(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ''ElevatorMusic(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ElevatorEnable(10) = ElevatorEnable(10) - 0.25
        If ElevatorEnable(10) < 0 Then ElevatorEnable(10) = 0
        If ElevatorEnable(10) = 0 Then ElevatorDirection(10) = 0
        If GotoFloor(10) <> 0 Then ElevatorCheck(10) = 0: FineTune(10) = True
        End If
      
        If ElevatorEnable(10) <= 0 And ElevatorDirection(10) = -1 Then
        If ElevatorSounds(10).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck(10) = 0 Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevstart.wav"
        ElevatorSounds(10).Volume = 0
        'ElevatorSounds(10).maxDistance = 100
        'Call ElevatorSounds(10).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(10).ConeOutsideVolume = 0
        'Call ElevatorSounds(10).SetPosition(-20.25, Elevator(10).GetPosition.Y + 20, -23)
        ElevatorSounds(10).Play
        ElevatorCheck(10) = 1
        End If
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_ENDED And ElevatorCheck(10) = 1 Then
        ElevatorSounds(10).Load App.Path + "\elevmove.wav"
        ElevatorSounds(10).Loop_ = True
        ElevatorSounds(10).Play
        End If
        Elevator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorL(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorR(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        FloorIndicator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Plaque(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Next i10
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, (ElevatorEnable(10) / 5), 0
        'ElevatorSounds(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ''ElevatorMusic(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ElevatorEnable(10) = ElevatorEnable(10) - 0.25
        If ElevatorEnable(10) >= -15 Then StoppingDistance(10) = OriginalLocation(10) - CurrentFloorExact(10)
        If ElevatorEnable(10) < -15 Then ElevatorEnable(10) = -15
        Destination(10) = ((OriginalLocation(10) * 25) + 25) - DistanceToTravel(10)
        If GotoFloor(10) <> 0 And elevatorstart(10).Y <= (Destination(10) + (StoppingDistance(10) * 25) + 25) Then ElevatorDirection(10) = 1: ElevatorCheck(10) = 0
        End If
      
        If ElevatorEnable(10) < 0 And ElevatorDirection(10) = 1 Then
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_PLAYING And ElevatorCheck2(10) = 0 And FineTune(10) = False Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Stop_
        End If
        If ElevatorSounds(10).PlayState <> TV_PLAYSTATE_PLAYING And ElevatorCheck2(10) = 0 And FineTune(10) = False Then
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevstop.wav"
        ElevatorSounds(10).Play
        ElevatorCheck2(10) = 1
        End If
        Elevator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorL(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        ElevatorInsDoorR(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        FloorIndicator(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Plaque(10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, (ElevatorEnable(10) / 5), 0
        Next i10
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, (ElevatorEnable(10) / 5), 0
        'ElevatorSounds(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ''ElevatorMusic(10).SetPosition -20.25, Elevator(10).GetPosition.Y + 20, -23
        ElevatorEnable(10) = ElevatorEnable(10) + 0.25
        If ElevatorEnable(10) > 0 Then ElevatorEnable(10) = 0
        If ElevatorEnable(10) = 0 Then ElevatorDirection(10) = 0
        If GotoFloor(10) <> 0 Then ElevatorCheck(10) = 0: FineTune(10) = True
        End If
      
      If FineTune(10) = True And ElevatorEnable(10) = 0 And elevatorstart(10).Y > (GotoFloor(10) * 25) + 25 + -0.3 And elevatorstart(10).Y < (GotoFloor(10) * 25) + 25 + 0.3 Then
      FineTune(10) = False
      If ElevatorSync(10) = True Then
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
      For i10 = 1 To 10
      CallButtons(i10).Enable True
      Next i10
      ShaftsFloor(CameraFloor).Enable True
      Stairs(CameraFloor).Enable True
      Atmos.SkyBox_Enable True
      'If CameraFloor = 137 Then Shafts.Enable True
      InitObjectsForFloor (CameraFloor)
      If CameraFloor < 138 Then Stairs(CameraFloor + 1).Enable True
      If CameraFloor > 1 Then Stairs(CameraFloor - 1).Enable True
      End If
      GotoFloor(10) = 0
      OpenElevator(10) = 1
      ElevatorCheck(10) = 0
      ElevatorCheck2(10) = 0
      ElevatorCheck3(10) = 0
      ElevatorCheck4(10) = 0
      If CameraFloor > 1 Then Camera.SetPosition Camera.GetPosition.X, (CameraFloor * 25) + 25 + 10, Camera.GetPosition.z
      If CameraFloor = 1 And FloorIndicatorText(10) <> "M" Then Camera.SetPosition Camera.GetPosition.X, 10, Camera.GetPosition.z
      End If
      
      If FineTune(10) = True Then
      If ElevatorCheck3(10) = 0 Then
      ElevatorSounds(10).Load App.Path + "\ding1.wav"
      ElevatorSounds(10).Play
      ElevatorCheck3(10) = 1
        For i10 = -1 To 138
        Buttons10(i10).SetColor RGBA(1, 1, 1, 1)
        Next i10
      End If
      If elevatorstart(10).Y < (GotoFloor(10) * 25) + 25 Then
      Elevator(10).MoveRelative 0, 0.2, 0
      ElevatorInsDoorL(10).MoveRelative 0, 0.2, 0
      ElevatorInsDoorR(10).MoveRelative 0, 0.2, 0
      Plaque(10).MoveRelative 0, 0.2, 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, 0.2, 0
        Next i10
        FloorIndicator(10).MoveRelative 0, 0.2, 0
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, 0.2, 0
      End If
      If elevatorstart(10).Y > (GotoFloor(10) * 25) + 25 Then
      Elevator(10).MoveRelative 0, -0.2, 0
      ElevatorInsDoorL(10).MoveRelative 0, -0.2, 0
      ElevatorInsDoorR(10).MoveRelative 0, -0.2, 0
      Plaque(10).MoveRelative 0, -0.2, 0
        For i10 = -1 To 138
        Buttons10(i10).MoveRelative 0, -0.2, 0
        Next i10
        FloorIndicator(10).MoveRelative 0, -0.2, 0
        If ElevatorSync(10) = True Then Camera.MoveRelative 0, -0.2, 0
      End If
      End If
      
      If OpenElevator(10) = 1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z >= 4 Then OpenElevator(10) = 0: GoTo OpenElevator1
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z >= 4 Then OpenElevator(10) = 0: GoTo OpenElevator1
      If ElevatorCheck4(10) = 0 Then
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(10).Stop_
        End If
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevatoropen.wav"
        ElevatorSounds(10).Volume = 0
        'ElevatorSounds(10).maxDistance = 1000
        'Call ElevatorSounds(10).SetConeOrientation(0, -5, 0)
        'ElevatorSounds(10).ConeOutsideVolume = 0
        'Call ElevatorSounds(10).SetPosition(-20.25, Elevator(10).GetPosition.Y, -23)
        ElevatorSounds(10).Play
        ElevatorCheck4(10) = 1
      End If
      OpenElevatorLoc(10) = OpenElevatorLoc(10) + 0.02
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z > 1 Then OpenElevator(10) = 2
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z > 1 Then OpenElevator(10) = 2
OpenElevator1:
      End If
      
      If OpenElevator(10) = 2 Then
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z > 3 Then OpenElevator(10) = 3
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z > 3 Then OpenElevator(10) = 3
      End If
      
      If OpenElevator(10) = 3 Then
      ElevatorCheck4(10) = 0
      OpenElevatorLoc(10) = OpenElevatorLoc(10) - 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z < 7 And OpenElevatorLoc(10) = 0 Then OpenElevatorLoc(10) = 0.02
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z < 7 And OpenElevatorLoc(10) = 0 Then OpenElevatorLoc(10) = 0.02
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      If OpenElevatorLoc(10) <= 0 Then
      OpenElevator(10) = 0
      OpenElevatorLoc(10) = 0
      Form1.Timer10.Enabled = True
      End If
      End If
      
      If OpenElevator(10) = -1 Then
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z <= 0 Then OpenElevator(10) = 0: GoTo OpenElevator2
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z <= 0 Then OpenElevator(10) = 0: GoTo OpenElevator2
      If ElevatorCheck4(10) = 0 Then
        If ElevatorSounds(10).PlayState = TV_PLAYSTATE_PLAYING Then
        ElevatorSounds(10).Stop_
        End If
        ElevatorSounds(10).Loop_ = False
        ElevatorSounds(10).Load App.Path + "\elevatorclose.wav"
        ElevatorSounds(10).Volume = 0
        'ElevatorSounds(10).maxDistance = 1000
        'Call ElevatorSounds(10).SetConeOrientation(0, 0, 90)
        'ElevatorSounds(10).ConeOutsideVolume = 0
        'Call ElevatorSounds(10).SetPosition(-20.25, Elevator(10).GetPosition.Y, -23)
        ElevatorSounds(10).Play
        ElevatorCheck4(10) = 1
      End If
      OpenElevatorLoc(10) = OpenElevatorLoc(10) - 0.02
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z < 3 Then OpenElevator(10) = -2
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z < 3 Then OpenElevator(10) = -2
OpenElevator2:
      End If
      
      If OpenElevator(10) = -2 Then
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z < 1 Then OpenElevator(10) = -3
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z < 1 Then OpenElevator(10) = -3
      End If
      
      If OpenElevator(10) = -3 Then
      ElevatorCheck4(10) = 0
      OpenElevatorLoc(10) = OpenElevatorLoc(10) + 0.02
      'If ElevatorInsDoorL(ElevatorFloor2(10)).GetPosition.z > 0 And OpenElevatorLoc(10) >= 0 Then OpenElevatorLoc(10) = -0.02
      If ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.z > 0 And OpenElevatorLoc(10) >= 0 Then OpenElevatorLoc(10) = -0.02
      ElevatorDoor10L(ElevatorFloor2(10)).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorDoor10R(ElevatorFloor2(10)).MoveRelative -OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorL(10).MoveRelative OpenElevatorLoc(10), 0, 0
      ElevatorInsDoorR(10).MoveRelative -OpenElevatorLoc(10), 0, 0
      If OpenElevatorLoc(10) >= 0 Then
      OpenElevator(10) = 0
      OpenElevatorLoc(10) = 0
      ElevatorInsDoorL(10).SetPosition ElevatorInsDoorL(10).GetPosition.X, ElevatorInsDoorL(10).GetPosition.Y, 0
      ElevatorInsDoorR(10).SetPosition ElevatorInsDoorR(10).GetPosition.X, ElevatorInsDoorR(10).GetPosition.Y, 0
      ElevatorDoor10L(ElevatorFloor2(10)).SetPosition ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.X, ElevatorDoor10L(ElevatorFloor2(10)).GetPosition.Y, 0
      ElevatorDoor10R(ElevatorFloor2(10)).SetPosition ElevatorDoor10R(ElevatorFloor2(10)).GetPosition.X, ElevatorDoor10R(ElevatorFloor2(10)).GetPosition.Y, 0
      End If
      End If
      
      
End Sub
