VERSION 5.00
Begin VB.Form Form3 
   Caption         =   "Title Screen"
   ClientHeight    =   6255
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   8850
   Icon            =   "Form3.frx":0000
   LinkTopic       =   "Form3"
   ScaleHeight     =   6255
   ScaleWidth      =   8850
   StartUpPosition =   3  'Windows Default
End
Attribute VB_Name = "Form3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skycraper 0.9 Beta
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper

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

Private isRunning As Boolean
'Public TV As TVEngine
'Public Scene As TVScene
'Public Inp As TVInputEngine
Public scr As TVScreen2DText
'Public texturefactory As TVTextureFactory
Public Screen As TVScreen2DImmediate
Public Effect As TVGraphicEffect
Public RollOnTexture As Integer
Public Explosion As TVMesh
Public Fin As Integer

Public bMain As Boolean, bMulti As Boolean, bOptions As Boolean, bSing As Boolean

Public mx As Long, my As Long, b1 As Integer
Public Function IsOnButton(X, Y, X2, Y2) As Boolean
  IsOnButton = False
  If mx > X Then
    If mx < X2 Then
      If my > Y Then
        If my < Y2 Then
          IsOnButton = True
        End If
      End If
    End If
  End If
End Function

Public Sub DrawMenu()
  Dim i As Long
  Dim b(5) As Boolean

  b(0) = Screen.DRAW_RollOver(GetTex("sp2"), GetTex("sp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2), (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 50)
  b(1) = Screen.DRAW_RollOver(GetTex("mp2"), GetTex("mp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 50, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 100)
  b(2) = Screen.DRAW_RollOver(GetTex("op2"), GetTex("op1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 100, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 150)
  b(2) = Screen.DRAW_RollOver(GetTex("qt2"), GetTex("qt1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 150, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 200)
  For i = 0 To 3
    If b(i) = True Then RollOnTexture = i
  Next i
End Sub

Private Sub Command1_Click()
isRunning = False
Set TV = Nothing
  'Set TV = New TVEngine
Form1.Show
  'If TV.ShowDriverDialog = False Then End
  'DoEvents
  'TV.Initialize Form1.hWnd
End Sub

Private Sub Form_Load()
'On Error Resume Next
  Set TV = New TVEngine
  Set Scene = New TVScene
  Set scr = New TVScreen2DText
  Set TextureFactory = New TVTextureFactory
  Set Effect = New TVGraphicEffect
  Set Explosion = New TVMesh
  Set Screen = New TVScreen2DImmediate
  Dim Fin As Integer
  isRunning = True
  Show
  DoEvents
  'TV.EnableHardwareTL True
  If TV.ShowDriverDialog = False Then End
  DoEvents
  TV.Initialize Me.hWnd
  
  Set Inp = New TVInputEngine
  TV.SetSearchDirectory "..\..\..\media"

  TextureFactory.LoadTexture "mp1.bmp", "mp1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "mp2.bmp", "mp2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "op1.bmp", "op1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "op2.bmp", "op2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "qt1.bmp", "qt1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "qt2.bmp", "qt2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "sp1.bmp", "sp1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "sp2.bmp", "sp2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture "warcraft3b_640.jpg", "Image"

  Effect.FadeIn 1500
  Scene.LoadCursor "arrow.bmp", TV_COLORKEY_BLACK, 24, 24
  
   Scene.LoadShaders "common.shader"
   
   Set Explosion = Scene.CreateBillboard(0, 7, 15, 0, 100, 30)
   Explosion.SetShader GetShader("Explosion")
  
  Scene.SetCamera 25, 0, -50, 25, 0, 0
  
  bMain = True
  
  'font stuff
  scr.TextureFont_Create "Options", "Arial", 10, True, False, False
    scr.TextureFont_Create "Back", "Arial", 20, True, False, False
    scr.TextureFont_Create "Single", "Arial", 10, True, False, False
     scr.TextureFont_Create "Multi", "Arial", 10, True, False, False
     scr.TextureFont_Create "Menu", "Arial", 10, True, False, False

    

  
  
  
  Do Until Effect.FadeFinished = True And isRunning = False
    
    TV.Clear
    Screen.DRAW_Texture GetTex("Image"), 0, 0, Me.ScaleWidth, Me.ScaleHeight
    'scr.CreateUserFont "menu", "Arial", 10, True, False, False
    
    
    Scene.RenderAllMeshes
    
    If bMain = True Then
     Main
    End If
    
    If bSing = True Then
     Sing
    End If
    
    If bMulti = True Then
     Multi
    End If
    
    If bOptions = True Then
     Options
    End If
    
    If bMain = True Then
     If IsOnButton((Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 150, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 200) = True Then
      If b1 = 128 Then
       Fin = 1
       Effect.FadeOut 1000
       isRunning = False
      End If
     End If
    End If
    
    TV.RenderToScreen
    
    If Inp.IsKeyPressed(TV_KEY_ESCAPE) = True Then
      Fin = 1
      'Effect.FadeIn 1000
      isRunning = False
      Set TV = Nothing
        Form3.Hide
        Form1.Show
        Exit Sub
    End If
  DoEvents
  Loop
    Set Effect = Nothing
  
End Sub

Sub Main()
    DrawMenu
    Inp.GetAbsMouseState mx, my, b1
    
    'Single Player
    If IsOnButton((Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2), (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 50) = True Then
    
      If b1 = 128 Then
        Effect.FadeIn 1000
        Sing
      End If
    End If
    
    'Multiplayer
    If IsOnButton((Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 50, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 100) = True Then
      If b1 = 128 Then
        Effect.FadeIn 1000
        Multi
      End If
    End If
    
    'Options
    If IsOnButton((Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 100, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 150) = True Then
      If b1 = 128 Then
        Effect.FadeIn 1000
        Options
      End If
    End If
            
    Dim blahmx As String, blahmy As String
    blahmx = mx
    blahmy = my
    'scr.DrawText "X = " & blahmx, 10, 10, RGBA(1, 0, 0, 1), "menu"
    'scr.DrawText "Y = " & blahmy, 10, 25, RGBA(1, 0, 0, 1), "menu"
End Sub

Sub Options()
bOptions = True
bMain = False
bMulti = False
bSing = False
    
    
    
    scr.TextureFont_DrawText "Options Here", Me.ScaleWidth / 2, Me.ScaleHeight / 2, RGBA(1, 0, 0, 1), "Options"
    scr.TextureFont_DrawText "Back", Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, RGBA(1, 0, 0, 1), "Back"
    Inp.GetAbsMouseState mx, my, b1
    
    If IsOnButton(Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, Me.ScaleWidth / 2 + 125, Me.ScaleHeight / 2 + 100) = True Then
      If b1 = 128 Then
      Effect.FadeIn 1000
       bOptions = False
       bMain = True
       bMulti = False
       bSing = False
      End If
    End If
    
    Dim blahmx As String, blahmy As String
    blahmx = mx
    blahmy = my
    'scr.DrawText "X = " & blahmx, 10, 10, RGBA(1, 0, 0, 1), "menu"
    'scr.DrawText "Y = " & blahmy, 10, 25, RGBA(1, 0, 0, 1), "menu"
End Sub

Sub Multi()
bOptions = False
bMain = False
bMulti = True
bSing = False
    
    
    
    scr.TextureFont_DrawText "Multiplayer Here", Me.ScaleWidth / 2, Me.ScaleHeight / 2, RGBA(1, 0, 0, 1), "Multi"
    scr.TextureFont_DrawText "Back", Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, RGBA(1, 0, 0, 1), "Back"
    Inp.GetAbsMouseState mx, my, b1
    
    If IsOnButton(Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, Me.ScaleWidth / 2 + 125, Me.ScaleHeight / 2 + 100) = True Then
      If b1 = 128 Then
      Effect.FadeIn 1000
       bOptions = False
       bMain = True
       bMulti = False
       bSing = False
      End If
    End If
    
    Dim blahmx As String, blahmy As String
    blahmx = mx
    blahmy = my
    'scr.DrawText "X = " & blahmx, 10, 10, RGBA(1, 0, 0, 1), "menu"
    'scr.DrawText "Y = " & blahmy, 10, 25, RGBA(1, 0, 0, 1), "menu"
End Sub

Sub Sing()
bOptions = False
bMain = False
bMulti = False
bSing = True
    
   
    scr.TextureFont_DrawText "Single Player Here", Me.ScaleWidth / 2, Me.ScaleHeight / 2, RGBA(1, 0, 0, 1), "Single"
    scr.TextureFont_DrawText "Back", Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, RGBA(1, 0, 0, 1), "Back"
    Inp.GetAbsMouseState mx, my, b1
    
    If IsOnButton(Me.ScaleWidth / 2, Me.ScaleHeight / 2 + 50, Me.ScaleWidth / 2 + 125, Me.ScaleHeight / 2 + 100) = True Then
      If b1 = 128 Then
      Effect.FadeIn 1000
       bOptions = False
       bMain = True
       bMulti = False
       bSing = False
      End If
    End If
    
    Dim blahmx As String, blahmy As String
    blahmx = mx
    blahmy = my
    'scr.DrawText "X = " & blahmx, 10, 10, RGBA(1, 0, 0, 1), "menu"
    'scr.DrawText "Y = " & blahmy, 10, 25, RGBA(1, 0, 0, 1), "menu"
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
isRunning = False
'If UnloadMode = 0 Then Effect.FadeFinished = True
End Sub

Private Sub Form_Resize()
If Not TV Is Nothing Then
    TV.ResizeDevice
End If

End Sub

Private Sub Form_Unload(Cancel As Integer)
'Set TV = Nothing
'Set Scene = Nothing
'Set Inp = Nothing
Set scr = Nothing
Set TextureFactory = Nothing
Set Screen = Nothing

Set Explosion = Nothing
End Sub

