Attribute VB_Name = "API"
'this module is marked for removal

Option Explicit

'Function GetCameraFloor() As Integer
''Determine what floor the camera is on
'Dim curfloor As Integer
'Dim total As Single

''Find the upper and lower bounds of each floor, then see if the
''camera altitude is within the bounds
''if camera is above TopFloor, then returns TopFloor
'For curfloor = BottomFloor To TopFloor
'DoEvents
'total = GetFloorAltitude(curfloor)
'If Camera.GetPosition.Y >= total And Camera.GetPosition.Y < total + GetFloorHeight(curfloor) Then Exit For
'Next curfloor

'GetCameraFloor = curfloor

'End Function

'Function GetFloorExact(Number As Single) As Single
''Determine what floor the specified altitude is part of
'Dim curfloor As Integer

''Find the upper and lower bounds of each floor, then see if the
''altitude is within the bounds
'For curfloor = BottomFloor To TopFloor
'DoEvents
'If Number >= GetFloorAltitude(curfloor) And Number < GetFloorAltitude(curfloor) + GetFloorHeight(curfloor) Then Exit For
'Next curfloor

'GetFloorExact = curfloor + ((Number - GetFloorAltitude(curfloor)) / GetFloorHeight(curfloor))
'End Function

