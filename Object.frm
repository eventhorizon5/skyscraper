VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form Object 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Object Manager"
   ClientHeight    =   7245
   ClientLeft      =   1650
   ClientTop       =   1545
   ClientWidth     =   4530
   Icon            =   "Object.frx":0000
   LinkTopic       =   "Form4"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7245
   ScaleWidth      =   4530
   Begin VB.CommandButton Command13 
      Caption         =   "Help"
      Height          =   375
      Left            =   3000
      TabIndex        =   26
      Top             =   4800
      Width           =   1335
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   375
      Left            =   0
      TabIndex        =   20
      Top             =   5280
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
      Caption         =   "Reset"
      Height          =   375
      Left            =   1800
      TabIndex        =   19
      Top             =   4080
      Width           =   615
   End
   Begin VB.CommandButton Command11 
      Caption         =   "All -"
      Height          =   375
      Left            =   3720
      TabIndex        =   18
      Top             =   3600
      Width           =   615
   End
   Begin VB.CommandButton Command10 
      Caption         =   "All +"
      Height          =   375
      Left            =   3720
      TabIndex        =   17
      Top             =   3240
      Width           =   615
   End
   Begin VB.CommandButton Command8 
      Caption         =   "-Y"
      Height          =   375
      Left            =   2655
      TabIndex        =   11
      Top             =   3600
      Width           =   750
   End
   Begin VB.CommandButton Command7 
      Caption         =   "+Y"
      Height          =   375
      Left            =   2655
      TabIndex        =   10
      Top             =   3240
      Width           =   735
   End
   Begin VB.CommandButton Command4 
      Caption         =   "+X"
      Height          =   375
      Left            =   1935
      TabIndex        =   7
      Top             =   3360
      Width           =   735
   End
   Begin VB.TextBox Text1 
      Height          =   1815
      Left            =   1200
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   16
      Top             =   5280
      Width           =   3135
   End
   Begin VB.OptionButton Option3 
      Caption         =   "Scale"
      Height          =   255
      Left            =   240
      TabIndex        =   15
      Top             =   4560
      Width           =   1215
   End
   Begin VB.CommandButton Command9 
      Caption         =   "Generate Code"
      Height          =   495
      Left            =   1560
      TabIndex        =   14
      Top             =   4560
      Width           =   1335
   End
   Begin VB.OptionButton Option2 
      Caption         =   "Move"
      Height          =   255
      Left            =   240
      TabIndex        =   13
      Top             =   4080
      Value           =   -1  'True
      Width           =   1215
   End
   Begin VB.OptionButton Option1 
      Caption         =   "Rotate"
      Height          =   255
      Left            =   240
      TabIndex        =   12
      Top             =   4320
      Width           =   1215
   End
   Begin VB.CommandButton Command6 
      Caption         =   "-Z"
      Height          =   375
      Left            =   1080
      TabIndex        =   9
      Top             =   3480
      Width           =   855
   End
   Begin VB.CommandButton Command5 
      Caption         =   "+Z"
      Height          =   375
      Left            =   1080
      TabIndex        =   8
      Top             =   3120
      Width           =   855
   End
   Begin VB.CommandButton Command3 
      Caption         =   "-X"
      Height          =   375
      Left            =   240
      TabIndex        =   6
      Top             =   3360
      Width           =   855
   End
   Begin VB.Frame Frame1 
      Caption         =   "File Select"
      Height          =   2895
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4215
      Begin VB.FileListBox File1 
         Height          =   2235
         Left            =   2160
         Pattern         =   "*.3ds"
         TabIndex        =   5
         Top             =   360
         Width           =   1935
      End
      Begin VB.DirListBox Dir1 
         Height          =   1890
         Left            =   120
         TabIndex        =   4
         Top             =   720
         Width           =   1935
      End
      Begin VB.DriveListBox Drive1 
         Height          =   315
         Left            =   120
         TabIndex        =   3
         Top             =   360
         Width           =   1935
      End
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Remove Object"
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      Top             =   4440
      Width           =   1335
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Insert Object"
      Height          =   375
      Left            =   3000
      TabIndex        =   0
      Top             =   4080
      Width           =   1335
   End
   Begin MSComctlLib.Slider Slider2 
      Height          =   375
      Left            =   0
      TabIndex        =   21
      Top             =   6000
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
      TabIndex        =   22
      Top             =   6720
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   661
      _Version        =   393216
      Max             =   100
      SelStart        =   100
      TickFrequency   =   10
      Value           =   100
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "ScaleChange"
      Height          =   255
      Left            =   0
      TabIndex        =   25
      Top             =   6480
      Width           =   1095
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "RotateChange"
      Height          =   255
      Left            =   0
      TabIndex        =   24
      Top             =   5760
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "MoveChange"
      Height          =   255
      Left            =   0
      TabIndex        =   23
      Top             =   5040
      Width           =   1095
   End
End
Attribute VB_Name = "Object"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.96 Beta
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

Private Sub Command1_Click()
   If Dir1.Path <> Dir1.List(Dir1.ListIndex) Then
      Dir1.Path = Dir1.List(Dir1.ListIndex)
      Exit Sub
   End If
If File1.FileName = "" Then Exit Sub

i = 500
Set Objects(i) = Scene.CreateMeshBuilder("Objects " + Str$(i))

'Call Init_Objects(Floor, 79)
Objects(i).Load3DSMesh (File1.Path + "\" + File1.FileName), , True, , True, True
'Objects(i).SetMaterial 0
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).SetRotation 0, 0, 0
Objects(i).ScaleMesh 1, 1, 1
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, Camera.GetPosition.z
'Objects(i).Optimize
End Sub

Private Sub Command10_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X + MoveChange, Objects(500).GetPosition.Y + MoveChange, Objects(500).GetPosition.z + MoveChange
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X + RotateChange, Objects(500).GetRotation.Y + RotateChange, Objects(500).GetRotation.z + RotateChange
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X + ScaleChange, Objects(500).GetMeshScale.Y + ScaleChange, Objects(500).GetMeshScale.z + ScaleChange

End Sub

Private Sub Command11_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X - MoveChange, Objects(500).GetPosition.Y - MoveChange, Objects(500).GetPosition.z - MoveChange
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X - RotateChange, Objects(500).GetRotation.Y - RotateChange, Objects(500).GetRotation.z - RotateChange
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X - ScaleChange, Objects(500).GetMeshScale.Y - ScaleChange, Objects(500).GetMeshScale.z - ScaleChange

End Sub

Private Sub Command12_Click()
Objects(i).SetRotation 0, 0, 0
Objects(i).ScaleMesh 1, 1, 1
Objects(i).SetPosition Camera.GetPosition.X, Camera.GetPosition.Y, Camera.GetPosition.z
End Sub

Private Sub Command13_Click()
MsgBox ("The Object Manager provides an easy to use method for adding 3D objects into the Skyscraper simulation.  First, choose an object from the file browser above.  Then click 'insert object'.  Use the buttons to resize, move, and rotate the object until it fits nicely.  Then click 'Generate Code' to create the code that can be put directly into Skyscraper's InitObjectsForFloor subroutine.  Note that you must change the code's index value in the first 2 lines of code (numbered as 0), based on the object's code position after it's pasted into the Skyscraper application.")
End Sub

Private Sub Command2_Click()
i = 500
'The destroymesh function is broken
On Error Resume Next
Objects(i).Enable False
'Sleep 2
Scene.DestroyMesh Objects(i)
Set Objects(i) = Nothing
End Sub

Private Sub Command3_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X - MoveChange, Objects(500).GetPosition.Y, Objects(500).GetPosition.z
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X - RotateChange, Objects(500).GetRotation.Y, Objects(500).GetRotation.z
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X - ScaleChange, Objects(500).GetMeshScale.Y, Objects(500).GetMeshScale.z
End Sub

Private Sub Command4_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X + MoveChange, Objects(500).GetPosition.Y, Objects(500).GetPosition.z
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X + RotateChange, Objects(500).GetRotation.Y, Objects(500).GetRotation.z
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X + ScaleChange, Objects(500).GetMeshScale.Y, Objects(500).GetMeshScale.z

End Sub

Private Sub Command5_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X, Objects(500).GetPosition.Y, Objects(500).GetPosition.z + MoveChange
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X, Objects(500).GetRotation.Y, Objects(500).GetRotation.z + RotateChange
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X, Objects(500).GetMeshScale.Y, Objects(500).GetMeshScale.z + ScaleChange

End Sub

Private Sub Command6_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X, Objects(500).GetPosition.Y, Objects(500).GetPosition.z - MoveChange
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X, Objects(500).GetRotation.Y, Objects(500).GetRotation.z - RotateChange
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X, Objects(500).GetMeshScale.Y, Objects(500).GetMeshScale.z - ScaleChange

End Sub

Private Sub Command7_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X, Objects(500).GetPosition.Y + MoveChange, Objects(500).GetPosition.z
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X, Objects(500).GetRotation.Y + RotateChange, Objects(500).GetRotation.z
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X, Objects(500).GetMeshScale.Y + ScaleChange, Objects(500).GetMeshScale.z
End Sub

Private Sub Command8_Click()
If Option2.Value = True Then Objects(500).SetPosition Objects(500).GetPosition.X, Objects(500).GetPosition.Y - MoveChange, Objects(500).GetPosition.z
If Option1.Value = True Then Objects(500).SetRotation Objects(500).GetRotation.X, Objects(500).GetRotation.Y - RotateChange, Objects(500).GetRotation.z
If Option3.Value = True Then Objects(500).ScaleMesh Objects(500).GetMeshScale.X, Objects(500).GetMeshScale.Y - ScaleChange, Objects(500).GetMeshScale.z

End Sub

Private Sub Command9_Click()
Text1.Text = "i = 0 + (150 * (Floor - 1))" + vbCrLf + "Call Init_Objects(Floor, 0)" + vbCrLf + "Objects(i).Load3DSMesh App.Path + """ + File1.Path + "\" + File1.FileName + """, , True, , True, True" + vbCrLf + "Objects(i).SetRotation " + Str$(Objects(500).GetRotation.X) + "," + Str$(Objects(500).GetRotation.Y) + "," + Str$(Objects(500).GetRotation.z) + vbCrLf + "Objects(i).ScaleMesh " + Str$(Objects(500).GetMeshScale.X) + "," + Str$(Objects(500).GetMeshScale.Y) + "," + Str$(Objects(500).GetMeshScale.z) + vbCrLf + "Objects(i).SetPosition " + Str$(Objects(500).GetPosition.X) + ", Floor + " + Str$(Objects(500).GetPosition.Y - ((CameraFloor - FloorHeight) / FloorHeight)) + "," + Str$(Objects(500).GetPosition.z) + vbCrLf
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

Private Sub Slider1_Change()
MoveChange = Slider1.Value * 0.01
End Sub

Private Sub Slider2_Change()
RotateChange = Slider2.Value * 0.001
End Sub

Private Sub Slider3_Change()
ScaleChange = Slider3.Value * 0.0005
End Sub
