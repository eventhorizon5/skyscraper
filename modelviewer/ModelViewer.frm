VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form Viewer 
   Caption         =   "ModelViewer"
   ClientHeight    =   6285
   ClientLeft      =   1650
   ClientTop       =   1830
   ClientWidth     =   7995
   Icon            =   "ModelViewer.frx":0000
   LinkTopic       =   "Form1"
   MinButton       =   0   'False
   ScaleHeight     =   6285
   ScaleWidth      =   7995
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   0
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Menu File 
      Caption         =   "File"
      Begin VB.Menu Open 
         Caption         =   "Open"
      End
      Begin VB.Menu Settings 
         Caption         =   "Settings"
      End
      Begin VB.Menu Exit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu Help 
      Caption         =   "Help"
      Begin VB.Menu About 
         Caption         =   "About"
      End
   End
End
Attribute VB_Name = "Viewer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'ModelViewer
'for the Skyscraper 3D Simulation
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
Public LightRange As Long
Public QuickMesh As Boolean
Private TV8 As TVEngine
Private Mesh As TVMesh
Private Scene As TVScene
Private DoLoop As Boolean
Private InputEngine As TVInputEngine
Private TextureFactory As TVTextureFactory
Private MatFactory As TVMaterialFactory
Private LightEngine As TVLightEngine
Private Camera As TVCamera
Private Light As D3DLIGHT8
Private EControls As Boolean

Private Sub About_Click()
frmAbout.Show
End Sub

Private Sub Exit_Click()
DoLoop = False
End Sub

Private Sub Form_GotFocus()
EControls = True
End Sub

Private Sub Form_Load()
LightRange = 400
DoLoop = True
Set TV8 = New TVEngine
TV8.Init3DWindowedMode Me.hWnd
TV8.DisplayFPS = True
Set Scene = New TVScene
Set Mesh = New TVMesh
Set Mesh = Scene.CreateMeshBuilder
Set TextureFactory = New TVTextureFactory
Set InputEngine = New TVInputEngine
Set Camera = New TVCamera
Set MatFactory = New TVMaterialFactory
Set LightEngine = New TVLightEngine
Dim FileName As String
'MatFactory.CreateMaterial ("QuickMAT")
'MatFactory.CreateMaterialQuick 0.2, 0.2, 0.2, 1, "QuickMAT"
Scene.SetViewFrustum 90, 200000

Light.Type = D3DLIGHT_POINT
Light.Position = Vector3(0, 0, -300)
Light.Ambient = DXColor(0.8, 0.8, 0.8, 1)
Light.diffuse = DXColor(0.7, 0.7, 0.7, 1)
Light.Attenuation0 = 0.4
Light.Range = LightRange
LightEngine.CreateLight Light, "Light"
LightEngine.UpdateLight 1, Light

FileName = Command()
If LCase(Right$(FileName, 4)) = ".3ds" Then Mesh.Load3DSMesh FileName
If LCase(Right$(FileName, 4)) = ".tvm" Then Mesh.LoadTVModel FileName
If LCase(Right$(FileName, 2)) = ".x" Then Mesh.LoadXFile FileName
If LCase$(Left$(Right$(FileName, 5), 4)) = ".3ds" Then Mesh.Load3DSMesh Mid$(FileName, 2, Len(FileName) - 2)
If LCase$(Left$(Right$(FileName, 5), 4)) = ".tvm" Then Mesh.LoadTVModel Mid$(FileName, 2, Len(FileName) - 2)
If LCase$(Left$(Right$(FileName, 3), 2)) = ".x" Then Mesh.LoadXFile Mid$(FileName, 2, Len(FileName) - 2)
Camera.SetPosition 0, 0, -300
Mesh.SetPosition 0, 0, 0
Mesh.SetRotation 0, 0, 0
Me.Show
MatFactory.CreateMaterialQuick 0.2, 0.2, 0.2, 1, "QuickMAT"

Do
Light.Range = LightRange
LightEngine.UpdateLight 1, Light

If QuickMesh = True Then Mesh.SetMaterial (GetMat("QuickMAT"))

Dim tmpMouseX As Long, tmpMouseY As Long, tmpMouseB1 As Integer, tmpMouseB2 As Integer, tmpMouseB3 As Integer, Roll As Long

InputEngine.GetMouseState tmpMouseX, tmpMouseY, tmpMouseB1, tmpMouseB2, tmpMouseB3, Roll

If tmpMouseB1 <> 0 And EControls = True Then
    If InputEngine.IsKeyPressed(TV_KEY_LEFTCONTROL) Then Mesh.MoveRelative 0, -tmpMouseY, 0: GoTo Skip
    If InputEngine.IsKeyPressed(TV_KEY_LEFTSHIFT) Then Mesh.SetPosition Mesh.GetPosition.x, Mesh.GetPosition.y, Mesh.GetPosition.z + tmpMouseY: GoTo Skip
End If

If tmpMouseB1 <> 0 And EControls = True Then
    Mesh.RotateY -tmpMouseX / 100
    Mesh.RotateX -tmpMouseY / 100
    Mesh.RotateZ -tmpMouseY / 100
End If

If EControls = True Then
If InputEngine.IsKeyPressed(TV_KEY_LEFTARROW) Then Mesh.SetPosition Mesh.GetPosition.x - 1, Mesh.GetPosition.y, Mesh.GetPosition.z
If InputEngine.IsKeyPressed(TV_KEY_RIGHTARROW) Then Mesh.SetPosition Mesh.GetPosition.x + 1, Mesh.GetPosition.y, Mesh.GetPosition.z
If InputEngine.IsKeyPressed(TV_KEY_UPARROW) Then Mesh.SetPosition Mesh.GetPosition.x, Mesh.GetPosition.y + 1, Mesh.GetPosition.z
If InputEngine.IsKeyPressed(TV_KEY_DOWNARROW) Then Mesh.SetPosition Mesh.GetPosition.x, Mesh.GetPosition.y - 1, Mesh.GetPosition.z
End If

Skip:
DoEvents
TV8.Clear
Scene.RenderAllMeshes
TV8.RenderToScreen
Loop Until DoLoop = False

Set Mesh = Nothing
Set Scene = Nothing
Set TV8 = Nothing
Set Camera = Nothing
Set InputEngine = Nothing
End

End Sub

Private Sub Form_LostFocus()
EControls = False
End Sub

Private Sub Form_Resize()
TV8.ResizeDevice

End Sub

Private Sub Form_Unload(Cancel As Integer)
DoLoop = False
Set Mesh = Nothing
Set Scene = Nothing
Set TV8 = Nothing
Set Camera = Nothing
Set InputEngine = Nothing
End
    
End Sub

Private Sub Open_Click()
On Error GoTo 400
Const MB_OK = 0, MB_OKCANCEL = 1    ' Define buttons.
Const MB_YESNOCANCEL = 3, MB_YESNO = 4
Const MB_ICONSTOP = 16, MB_ICONQUESTION = 32    ' Define Icons.
Const MB_ICONEXCLAMATION = 48, MB_ICONINFORMATION = 64
Const MB_DEFBUTTON2 = 256, IDYES = 6, IDNO = 7  ' Define other.
Dim DgDef, Msg, Response, Title ' Declare variables.
CommonDialog1.InitDir = App.Path
CommonDialog1.CancelError = True
CommonDialog1.DefaultExt = ".3ds"
CommonDialog1.Filter = "3D Studio Mesh (*.3ds)|*.3ds|DirectX Mesh (*.x)|*.x|TrueVision3D Model (*.tvm)|*.tvm|"
CommonDialog1.DialogTitle = "Load Mesh"
CommonDialog1.Action = 1
On Error GoTo 401
Dim FileName As String
FileName = CommonDialog1.FileName
Mesh.ResetMesh
If LCase(Right$(FileName, 4)) = ".3ds" Then Mesh.Load3DSMesh FileName
If LCase(Right$(FileName, 4)) = ".tvm" Then Mesh.LoadTVModel FileName
If LCase(Right$(FileName, 2)) = ".x" Then Mesh.LoadXFile FileName
Camera.SetPosition 0, 0, -300
Mesh.SetPosition 0, 0, 0
Mesh.SetRotation 0, 0, 0
Exit Sub
400 Exit Sub
401 MsgBox ("Error loading file")
End Sub

Private Sub Settings_Click()
frmSettings.Show
End Sub
