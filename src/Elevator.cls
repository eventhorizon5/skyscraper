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
