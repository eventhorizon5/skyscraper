VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Object 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Object Manager"
   ClientHeight    =   7545
   ClientLeft      =   1650
   ClientTop       =   1545
   ClientWidth     =   4530
   Icon            =   "Object.frx":0000
   LinkTopic       =   "Form4"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7545
   ScaleWidth      =   4530
   Begin VB.CommandButton Command8 
      Caption         =   "-Y"
      Height          =   375
      Left            =   2595
      TabIndex        =   11
      ToolTipText     =   "Decrease vertical value"
      Top             =   3900
      Width           =   855
   End
   Begin VB.CommandButton Command7 
      Caption         =   "+Y"
      Height          =   375
      Left            =   2595
      TabIndex        =   10
      ToolTipText     =   "Increase vertical value"
      Top             =   3540
      Width           =   855
   End
   Begin VB.Timer Timer1 
      Interval        =   100
      Left            =   4080
      Top             =   0
   End
   Begin VB.CommandButton Command13 
      Caption         =   "Help"
      Height          =   375
      Left            =   3000
      TabIndex        =   25
      ToolTipText     =   "Click here for help"
      Top             =   5160
      Width           =   1335
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   375
      Left            =   0
      TabIndex        =   19
      ToolTipText     =   "Amount to move object"
      Top             =   5640
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   661
      _Version        =   393216
      Max             =   100
      SelStart        =   100
      TickFrequency   =   10
      Value           =   100
   End
   Begin VB.CommandButton Command12 
      Caption         =   "Reset Object"
      Height          =   375
      Left            =   1560
      TabIndex        =   18
      ToolTipText     =   "Undo changes made to an object"
      Top             =   4680
      Width           =   1215
   End
   Begin VB.CommandButton Command11 
      Caption         =   "All -"
      Height          =   375
      Left            =   3660
      TabIndex        =   17
      ToolTipText     =   "Decrease all values"
      Top             =   3900
      Width           =   750
   End
   Begin VB.CommandButton Command10 
      Caption         =   "All +"
      Height          =   375
      Left            =   3660
      TabIndex        =   16
      ToolTipText     =   "Increase all values"
      Top             =   3540
      Width           =   750
   End
   Begin VB.CommandButton Command4 
      Caption         =   "+X"
      Height          =   375
      Left            =   1755
      TabIndex        =   7
      ToolTipText     =   "Increase horizontal value"
      Top             =   3720
      Width           =   855
   End
   Begin VB.TextBox Text1 
      Height          =   1815
      Left            =   1200
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   15
      ToolTipText     =   "Code generated for use in the Skyscraper simulation code"
      Top             =   5640
      Width           =   3135
   End
   Begin VB.OptionButton Option3 
      Caption         =   "Scale"
      Height          =   255
      Left            =   240
      TabIndex        =   14
      ToolTipText     =   "Resize object"
      Top             =   4920
      Width           =   1215
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Move"
      Height          =   255
      Left            =   240
      TabIndex        =   13
      ToolTipText     =   "Move object"
      Top             =   4440
      Value           =   -1  'True
      Width           =   1215
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Rotate"
      Height          =   255
      Left            =   240
      TabIndex        =   12
      ToolTipText     =   "Rotate object"
      Top             =   4680
      Width           =   1215
   End
   Begin VB.CommandButton Command6 
      Caption         =   "-Z"
      Height          =   375
      Left            =   915
      TabIndex        =   9
      ToolTipText     =   "Decrease Depth Value"
      Top             =   3900
      Width           =   855
   End
   Begin VB.CommandButton Command5 
      Caption         =   "+Z"
      Height          =   375
      Left            =   915
      TabIndex        =   8
      ToolTipText     =   "Increase Depth Value"
      Top             =   3540
      Width           =   855
   End
   Begin VB.CommandButton Command3 
      Caption         =   "-X"
      Height          =   375
      Left            =   75
      TabIndex        =   6
      ToolTipText     =   "Decrease horizontal value"
      Top             =   3720
      Width           =   855
   End
   Begin VB.Frame Frame1 
      Caption         =   "File Select"
      Height          =   2655
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4215
      Begin VB.FileListBox File1 
         Height          =   2235
         Left            =   2160
         Pattern         =   "*.3ds"
         TabIndex        =   5
         Top             =   240
         Width           =   1935
      End
      Begin VB.DirListBox Dir1 
         Height          =   1890
         Left            =   120
         TabIndex        =   4
         Top             =   600
         Width           =   1935
      End
      Begin VB.DriveListBox Drive1 
         Height          =   315
         Left            =   120
         TabIndex        =   3
         Top             =   240
         Width           =   1935
      End
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Remove Object"
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      ToolTipText     =   "Remove the selected object from the simulation"
      Top             =   4800
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Insert Object"
      Height          =   375
      Left            =   3000
      TabIndex        =   0
      ToolTipText     =   "Insert a new object into the simulation"
      Top             =   4440
      Width           =   1335
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   375
      Left            =   0
      TabIndex        =   20
      ToolTipText     =   "Amount to rotate object"
      Top             =   6360
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   661
      _Version        =   393216
      Max             =   100
      SelStart        =   100
      TickFrequency   =   10
      Value           =   100
   End
   Begin MSComctlLib.Slider Slider3 
      Height          =   375
      Left            =   0
      TabIndex        =   21
      ToolTipText     =   "Amount to resize object"
      Top             =   7080
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   661
      _Version        =   393216
      Max             =   100
      SelStart        =   100
      TickFrequency   =   10
      Value           =   100
   End
   Begin VB.Label Label7 
      Caption         =   "Code for Skyscraper:"
      Height          =   255
      Left            =   1320
      TabIndex        =   29
      Top             =   5280
      Width           =   1575
   End
   Begin VB.Label Label6 
      Alignment       =   2  'Center
      Caption         =   "Currently selected object:"
      Height          =   255
      Left            =   2400
      TabIndex        =   28
      Top             =   2850
      Width           =   1935
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      BorderStyle     =   1  'Fixed Single
      Height          =   255
      Left            =   2400
      TabIndex        =   27
      ToolTipText     =   "Currently selected simulation object"
      Top             =   3120
      Width           =   1935
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Caption         =   "Load an object from a file, or click on one in the simulation"
      Height          =   375
      Left            =   120
      TabIndex        =   26
      Top             =   2880
      Width           =   2055
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "ScaleChange"
      Height          =   255
      Left            =   0
      TabIndex        =   24
      Top             =   6840
      Width           =   1095
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "RotateChange"
      Height          =   255
      Left            =   0
      TabIndex        =   23
      Top             =   6120
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "MoveChange"
      Height          =   255
      Left            =   0
      TabIndex        =   22
      Top             =   5400
      Width           =   1095
   End
End
Attribute VB_Name = "Object"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.97 Beta - Object Manager
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
Dim ScaleChange As Single
Dim RotateChange As Single
Dim MoveChange As Single
Dim ObjectIndex As Integer
Dim OrigRotation(2) As Single
Dim OrigScale(2) As Single
Dim OrigPosition(2) As Single


Private Sub Command1_Click()
   If Dir1.Path <> Dir1.List(Dir1.ListIndex) Then
      Dir1.Path = Dir1.List(Dir1.ListIndex)
      Exit Sub
   End If
If File1.FileName = "" Then Exit Sub

Dim oindex As Integer
Dim Junk As Boolean
oindex = 1
On Error GoTo FoundEnd
Do
oindex = oindex + 1
i = oindex + (150 * (CameraFloor - 1))
Junk = Objects(i).IsMeshEnabled
If oindex > 150 Then
    MsgBox "Cannot create more than 150 objects per floor", vbCritical
    Exit Sub
End If
Loop
FoundEnd:

Call Init_Objects(CameraFloor, oindex)
Objects(i).Load3DSMesh (File1.Path + "\" + File1.FileName), , True, , True, True
'Objects(i).SetMaterial 0
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).SetRotation 0, 0, 0
Objects(i).ScaleMesh 1, 1, 1

OrigRotation(0) = 0
OrigRotation(1) = 0
OrigRotation(2) = 0
OrigScale(0) = 1
OrigScale(1) = 1
OrigScale(2) = 1
OrigPosition(0) = Camera.GetPosition.X
OrigPosition(1) = Camera.GetPosition.Y
OrigPosition(2) = Camera.GetPosition.z

'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, Camera.GetPosition.z
'Objects(i).Optimize
ObjectIndex = i
End Sub

Private Sub Command10_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X + MoveChange, Objects(ObjectIndex).GetPosition.Y + MoveChange, Objects(ObjectIndex).GetPosition.z + MoveChange
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X + RotateChange, Objects(ObjectIndex).GetRotation.Y + RotateChange, Objects(ObjectIndex).GetRotation.z + RotateChange
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X + ScaleChange, Objects(ObjectIndex).GetMeshScale.Y + ScaleChange, Objects(ObjectIndex).GetMeshScale.z + ScaleChange

End Sub

Private Sub Command11_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X - MoveChange, Objects(ObjectIndex).GetPosition.Y - MoveChange, Objects(ObjectIndex).GetPosition.z - MoveChange
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X - RotateChange, Objects(ObjectIndex).GetRotation.Y - RotateChange, Objects(ObjectIndex).GetRotation.z - RotateChange
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X - ScaleChange, Objects(ObjectIndex).GetMeshScale.Y - ScaleChange, Objects(ObjectIndex).GetMeshScale.z - ScaleChange

End Sub

Private Sub Command12_Click()
On Error Resume Next
Objects(i).SetRotation OrigRotation(0), OrigRotation(1), OrigRotation(2)
Objects(i).ScaleMesh OrigScale(0), OrigScale(1), OrigScale(2)
Objects(i).SetPosition OrigPosition(0), OrigPosition(1), OrigPosition(2)
End Sub

Private Sub Command13_Click()
MsgBox ("The Object Manager provides an easy to use method for adding, manipulating, and removing 3D objects in the Skyscraper simulation.  To load a new object, choose an object from the file browser above, and click 'insert object'.  To use an already existing object, just click on it in the simulation.  Use the buttons to resize, move, and rotate the object until it fits nicely.  Then click 'Generate Code' to create the code that can be put directly into Skyscraper's InitObjectsForFloor subroutine.  Note that you may have to change the code's index value in the first 2 lines of code based on the object's code position after it's pasted into the Skyscraper application.")
End Sub

Private Sub Command2_Click()
i = ObjectIndex
'The destroymesh function is broken
On Error Resume Next
Objects(i).Enable False
'Sleep 2
Scene.DestroyMesh Objects(i)
Set Objects(i) = Nothing
End Sub

Private Sub Command3_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X - MoveChange, Objects(ObjectIndex).GetPosition.Y, Objects(ObjectIndex).GetPosition.z
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X - RotateChange, Objects(ObjectIndex).GetRotation.Y, Objects(ObjectIndex).GetRotation.z
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X - ScaleChange, Objects(ObjectIndex).GetMeshScale.Y, Objects(ObjectIndex).GetMeshScale.z
End Sub

Private Sub Command4_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X + MoveChange, Objects(ObjectIndex).GetPosition.Y, Objects(ObjectIndex).GetPosition.z
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X + RotateChange, Objects(ObjectIndex).GetRotation.Y, Objects(ObjectIndex).GetRotation.z
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X + ScaleChange, Objects(ObjectIndex).GetMeshScale.Y, Objects(ObjectIndex).GetMeshScale.z

End Sub

Private Sub Command5_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X, Objects(ObjectIndex).GetPosition.Y, Objects(ObjectIndex).GetPosition.z + MoveChange
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X, Objects(ObjectIndex).GetRotation.Y, Objects(ObjectIndex).GetRotation.z + RotateChange
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X, Objects(ObjectIndex).GetMeshScale.Y, Objects(ObjectIndex).GetMeshScale.z + ScaleChange

End Sub

Private Sub Command6_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X, Objects(ObjectIndex).GetPosition.Y, Objects(ObjectIndex).GetPosition.z - MoveChange
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X, Objects(ObjectIndex).GetRotation.Y, Objects(ObjectIndex).GetRotation.z - RotateChange
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X, Objects(ObjectIndex).GetMeshScale.Y, Objects(ObjectIndex).GetMeshScale.z - ScaleChange

End Sub

Private Sub Command7_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X, Objects(ObjectIndex).GetPosition.Y + MoveChange, Objects(ObjectIndex).GetPosition.z
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X, Objects(ObjectIndex).GetRotation.Y + RotateChange, Objects(ObjectIndex).GetRotation.z
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X, Objects(ObjectIndex).GetMeshScale.Y + ScaleChange, Objects(ObjectIndex).GetMeshScale.z
End Sub

Private Sub Command8_Click()
On Error Resume Next
If Option2.Value = True Then Objects(ObjectIndex).SetPosition Objects(ObjectIndex).GetPosition.X, Objects(ObjectIndex).GetPosition.Y - MoveChange, Objects(ObjectIndex).GetPosition.z
If Option1.Value = True Then Objects(ObjectIndex).SetRotation Objects(ObjectIndex).GetRotation.X, Objects(ObjectIndex).GetRotation.Y - RotateChange, Objects(ObjectIndex).GetRotation.z
If Option3.Value = True Then Objects(ObjectIndex).ScaleMesh Objects(ObjectIndex).GetMeshScale.X, Objects(ObjectIndex).GetMeshScale.Y - ScaleChange, Objects(ObjectIndex).GetMeshScale.z

End Sub

Private Sub Dir1_Change()
File1.Path = Dir1.Path
End Sub

Private Sub Drive1_Change()

   On Error GoTo DriveHandler
   ' If new drive was selected, the Dir1 box
   ' updates its display.
   Dir1.Path = Drive1.Drive
   Exit Sub
' If there is an error, reset drvList.Drive with the
' drive from dirList.Path.
DriveHandler:
   Drive1.Drive = Dir1.Path
   Exit Sub

End Sub

Private Sub Form_Load()
Dir1.Path = App.Path
MoveChange = 1
RotateChange = 0.1
ScaleChange = 0.05
End Sub

Private Sub Label5_Change()
On Error Resume Next
OrigRotation(0) = Objects(ObjectIndex).GetRotation.X
OrigRotation(1) = Objects(ObjectIndex).GetRotation.Y
OrigRotation(2) = Objects(ObjectIndex).GetRotation.z
OrigScale(0) = Objects(ObjectIndex).GetMeshScale.X
OrigScale(1) = Objects(ObjectIndex).GetMeshScale.Y
OrigScale(2) = Objects(ObjectIndex).GetMeshScale.z
OrigPosition(0) = Objects(ObjectIndex).GetPosition.X
OrigPosition(1) = Objects(ObjectIndex).GetPosition.Y
OrigPosition(2) = Objects(ObjectIndex).GetPosition.z
End Sub


Private Sub Slider1_Change()
MoveChange = Slider1.Value * 0.01
End Sub

Private Sub Slider2_Change()
RotateChange = Slider2.Value * 0.001
End Sub

Private Sub Slider3_Change()
ScaleChange = Slider3.Value * 0.0005
End Sub

Private Sub Timer1_Timer()
On Error GoTo ErrorHandler
Dim Junk As Boolean
Dim OutText As String
Label5.Caption = SelectedObject
If Left$(SelectedObject, 8) = "Objects" Then ObjectIndex = Val(Str$(Mid$(SelectedObject, 8)))
Junk = Objects(ObjectIndex).IsMeshEnabled

'Code Generation
OutText = "i = " + LTrim(Str$(ObjectIndex)) + " + (150 * (Floor - 1))" + vbCrLf + "Call Init_Objects(Floor, " + LTrim(Str$(ObjectIndex)) + ")" + vbCrLf + "Objects(i).Load3DSMesh App.Path + """ + File1.Path + "\" + File1.FileName + """, , True, , True, True" + vbCrLf + "Objects(i).SetRotation " + Str$(Objects(ObjectIndex).GetRotation.X) + "," + Str$(Objects(ObjectIndex).GetRotation.Y) + "," + Str$(Objects(ObjectIndex).GetRotation.z) + vbCrLf + "Objects(i).ScaleMesh " + Str$(Objects(ObjectIndex).GetMeshScale.X) + "," + Str$(Objects(ObjectIndex).GetMeshScale.Y) + "," + Str$(Objects(ObjectIndex).GetMeshScale.z) + vbCrLf + "Objects(i).SetPosition " + Str$(Objects(ObjectIndex).GetPosition.X) + ", "
If CameraFloor > 1 Then OutText = OutText + "FloorAltitude + "
OutText = OutText + Str$(Objects(ObjectIndex).GetPosition.Y - GetFloorAltitude(CameraFloor)) + "," + Str$(Objects(ObjectIndex).GetPosition.z) + vbCrLf
If Text1.Text <> OutText Then Text1.Text = OutText
Exit Sub
ErrorHandler:
End Sub
