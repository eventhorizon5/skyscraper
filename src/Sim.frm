VERSION 5.00
Begin VB.Form Sim 
   BackColor       =   &H80000007&
   Caption         =   "SkyScraper"
   ClientHeight    =   7920
   ClientLeft      =   2025
   ClientTop       =   1860
   ClientWidth     =   10440
   Icon            =   "Sim.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   7920
   ScaleWidth      =   10440
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   49
      Interval        =   10000
      Left            =   6600
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   48
      Interval        =   10000
      Left            =   6000
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   47
      Interval        =   10000
      Left            =   5400
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   46
      Interval        =   10000
      Left            =   4800
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   45
      Interval        =   10000
      Left            =   4200
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   44
      Interval        =   10000
      Left            =   3600
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   43
      Interval        =   10000
      Left            =   3000
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   42
      Interval        =   10000
      Left            =   2400
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   41
      Interval        =   10000
      Left            =   1800
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   40
      Interval        =   10000
      Left            =   1200
      Top             =   6240
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   39
      Interval        =   10000
      Left            =   6600
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   38
      Interval        =   10000
      Left            =   6000
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   37
      Interval        =   10000
      Left            =   5400
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   36
      Interval        =   10000
      Left            =   4800
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   35
      Interval        =   10000
      Left            =   4200
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   34
      Interval        =   10000
      Left            =   3600
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   33
      Interval        =   10000
      Left            =   3000
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   32
      Interval        =   10000
      Left            =   2400
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   31
      Interval        =   10000
      Left            =   1800
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   30
      Interval        =   10000
      Left            =   1200
      Top             =   5760
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   29
      Interval        =   10000
      Left            =   6600
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   28
      Interval        =   10000
      Left            =   6000
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   27
      Interval        =   10000
      Left            =   5400
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   26
      Interval        =   10000
      Left            =   4800
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   25
      Interval        =   10000
      Left            =   4200
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   24
      Interval        =   10000
      Left            =   3600
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   23
      Interval        =   10000
      Left            =   3000
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   22
      Interval        =   10000
      Left            =   2400
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   21
      Interval        =   10000
      Left            =   1800
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   20
      Interval        =   10000
      Left            =   1200
      Top             =   5280
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   19
      Interval        =   10000
      Left            =   6600
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   18
      Interval        =   10000
      Left            =   6000
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   17
      Interval        =   10000
      Left            =   5400
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   16
      Interval        =   10000
      Left            =   4800
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   15
      Interval        =   10000
      Left            =   4200
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   14
      Interval        =   10000
      Left            =   3600
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   13
      Interval        =   10000
      Left            =   3000
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   12
      Interval        =   10000
      Left            =   2400
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   11
      Interval        =   10000
      Left            =   1800
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   10
      Interval        =   10000
      Left            =   1200
      Top             =   4800
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   9
      Interval        =   10000
      Left            =   6600
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   8
      Interval        =   10000
      Left            =   6000
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   7
      Interval        =   10000
      Left            =   5400
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   6
      Interval        =   10000
      Left            =   4800
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   5
      Interval        =   10000
      Left            =   4200
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   4
      Interval        =   10000
      Left            =   3600
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   3
      Interval        =   10000
      Left            =   3000
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   2
      Interval        =   10000
      Left            =   2400
      Top             =   4320
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   1
      Interval        =   10000
      Left            =   1800
      Top             =   4320
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   49
      Interval        =   1
      Left            =   6600
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   48
      Interval        =   1
      Left            =   6000
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   47
      Interval        =   1
      Left            =   5400
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   46
      Interval        =   1
      Left            =   4800
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   45
      Interval        =   1
      Left            =   4200
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   44
      Interval        =   1
      Left            =   3600
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   43
      Interval        =   1
      Left            =   3000
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   42
      Interval        =   1
      Left            =   2400
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   41
      Interval        =   1
      Left            =   1800
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   40
      Interval        =   1
      Left            =   1200
      Top             =   3720
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   39
      Interval        =   1
      Left            =   6600
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   38
      Interval        =   1
      Left            =   6000
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   37
      Interval        =   1
      Left            =   5400
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   36
      Interval        =   1
      Left            =   4800
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   35
      Interval        =   1
      Left            =   4200
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   34
      Interval        =   1
      Left            =   3600
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   33
      Interval        =   1
      Left            =   3000
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   32
      Interval        =   1
      Left            =   2400
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   31
      Interval        =   1
      Left            =   1800
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   30
      Interval        =   1
      Left            =   1200
      Top             =   3240
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   29
      Interval        =   1
      Left            =   6600
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   28
      Interval        =   1
      Left            =   6000
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   27
      Interval        =   1
      Left            =   5400
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   26
      Interval        =   1
      Left            =   4800
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   25
      Interval        =   1
      Left            =   4200
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   24
      Interval        =   1
      Left            =   3600
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   23
      Interval        =   1
      Left            =   3000
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   22
      Interval        =   1
      Left            =   2400
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   21
      Interval        =   1
      Left            =   1800
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   20
      Interval        =   1
      Left            =   1200
      Top             =   2760
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   19
      Interval        =   1
      Left            =   6600
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   18
      Interval        =   1
      Left            =   6000
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   17
      Interval        =   1
      Left            =   5400
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   16
      Interval        =   1
      Left            =   4800
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   15
      Interval        =   1
      Left            =   4200
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   14
      Interval        =   1
      Left            =   3600
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   13
      Interval        =   1
      Left            =   3000
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   12
      Interval        =   1
      Left            =   2400
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   11
      Interval        =   1
      Left            =   1800
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   10
      Interval        =   1
      Left            =   1200
      Top             =   2280
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   9
      Interval        =   1
      Left            =   6600
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   8
      Interval        =   1
      Left            =   6000
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   7
      Interval        =   1
      Left            =   5400
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   6
      Interval        =   1
      Left            =   4800
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   5
      Interval        =   1
      Left            =   4200
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   4
      Interval        =   1
      Left            =   3600
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   3
      Interval        =   1
      Left            =   3000
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   2
      Interval        =   1
      Left            =   2400
      Top             =   1800
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   1
      Interval        =   1
      Left            =   1800
      Top             =   1800
   End
   Begin VB.Timer IntroMusic 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   2880
      Top             =   360
   End
   Begin VB.Timer StairsTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   2160
      Top             =   360
   End
   Begin VB.Timer ElevatorTimer 
      Enabled         =   0   'False
      Index           =   0
      Interval        =   1
      Left            =   1200
      Top             =   1800
   End
   Begin VB.Timer MainTimer 
      Enabled         =   0   'False
      Interval        =   1
      Left            =   960
      Top             =   360
   End
   Begin VB.Timer WaitTimer 
      Enabled         =   0   'False
      Index           =   0
      Interval        =   10000
      Left            =   1200
      Top             =   4320
   End
End
Attribute VB_Name = "Sim"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Skyscraper 1.1 Alpha - Simulation Window
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

Option Explicit

Private IsRunning As Boolean
Public scr As TVScreen2DText
Public Screen As TVScreen2DImmediate
Public RollOnTexture As Integer
Public Explosion As TVMesh
Public Fin As Integer

Public bMain As Boolean, bMulti As Boolean, bOptions As Boolean, bSing As Boolean

Public mx As Long, my As Long, b1 As Integer
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
        'Multi
      End If
    End If
    
    'Options
    If IsOnButton((Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 100, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 150) = True Then
      If b1 = 128 Then
        Effect.FadeIn 1000
        'Options
        Design
      End If
    End If
            
    Dim blahmx As String, blahmy As String
    blahmx = mx
    blahmy = my
    'scr.DrawText "X = " & blahmx, 10, 10, RGBA(1, 0, 0, 1), "menu"
    'scr.DrawText "Y = " & blahmy, 10, 25, RGBA(1, 0, 0, 1), "menu"
End Sub


Public Sub DrawMenu()
  Dim i As Long
  Dim b(5) As Boolean

  'b(0) = Screen.DRAW_RollOver(GetTex("sp2"), GetTex("sp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2), (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 50)
  'b(1) = Screen.DRAW_RollOver(GetTex("mp2"), GetTex("mp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 50, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 100)
  'b(2) = Screen.DRAW_RollOver(GetTex("op2"), GetTex("op1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 100, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 150)
  'b(2) = Screen.DRAW_RollOver(GetTex("qt2"), GetTex("qt1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 150, (Me.ScaleWidth / 2) + 105, (Me.ScaleHeight / 2) + 200)
  b(0) = Screen.DRAW_RollOver(GetTex("sp2"), GetTex("sp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2), (Me.ScaleWidth / 2) + 175, (Me.ScaleHeight / 2) + 50)
  b(1) = Screen.DRAW_RollOver(GetTex("mp2"), GetTex("mp1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 50, (Me.ScaleWidth / 2) + 175, (Me.ScaleHeight / 2) + 100)
  b(2) = Screen.DRAW_RollOver(GetTex("op2"), GetTex("op1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 100, (Me.ScaleWidth / 2) + 175, (Me.ScaleHeight / 2) + 150)
  b(2) = Screen.DRAW_RollOver(GetTex("qt2"), GetTex("qt1"), (Me.ScaleWidth / 2) - 30, (Me.ScaleHeight / 2) + 150, (Me.ScaleWidth / 2) + 175, (Me.ScaleHeight / 2) + 200)
  For i = 0 To 3
    If b(i) = True Then RollOnTexture = i
  Next i
End Sub
Public Function IsOnButton(X, Y, x2, Y2) As Boolean
  IsOnButton = False
  If mx > X Then
    If mx < x2 Then
      If my > Y Then
        If my < Y2 Then
          IsOnButton = True
        End If
      End If
    End If
  End If
End Function
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
Sub Design()
bOptions = False
bMain = False
bMulti = False
bSing = True
    
'Start with no other menus
Fin = 1
      'Effect.FadeIn 1000
      IsRunning = False
        Set scr = Nothing
        Set Screen = Nothing
        Designer.Show
        Exit Sub
End Sub

Sub Sing()
bOptions = False
bMain = False
bMulti = False
bSing = True
    
'Start with no other menus
Fin = 1
      'Effect.FadeIn 1000
      IsRunning = False
        Set scr = Nothing
        Set Screen = Nothing
        Sim.Refresh
        DebugPanel.Show
        Sim.Show
        Start "Hello", False, Me
        Exit Sub
'End of new start
   
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

Private Sub ElevatorTimer_Timer(Index As Integer)
If Index + 1 <= Elevators Then Call Elevator(Index + 1).MonitorLoop
End Sub

Private Sub Form_GotFocus()
'Focused = True
End Sub

Public Sub Form_Load()
'On Error GoTo ErrorHandler
Sim.ScaleWidth = 671
Sim.ScaleMode = 3
Sim.ScaleHeight = 504

  Set TV = New TVEngine
  Set Scene = New TVScene
  Set scr = New TVScreen2DText
  Set TextureFactory = New TVTextureFactory
  Set Effect = New TVGraphicEffect
  Set SoundEngine = New TV3DMedia.TVSoundEngine
  Set Screen = New TVScreen2DImmediate
  Set MainMusic = New TV3DMedia.TVSoundMP3
  Dim Fin As Integer
  IsRunning = True
  Show
  DoEvents
  'TV.EnableHardwareTL True
  If TV.ShowDriverDialog = False Then End
  DoEvents
  TV.Initialize Me.hWnd
  
  Set Inp = New TVInputEngine
  'TV.SetSearchDirectory "data"
  Call SoundEngine.Init(Sim.hWnd)

  TextureFactory.LoadTexture App.Path + "\data\mp1.bmp", "mp1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\mp2.bmp", "mp2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\op1.bmp", "op1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\op2.bmp", "op2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\qt1.bmp", "qt1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\qt2.bmp", "qt2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\sp1.bmp", "sp1", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\sp2.bmp", "sp2", , , TV_COLORKEY_BLACK
  TextureFactory.LoadTexture App.Path + "\data\background.jpg", "Image"
  TextureFactory.LoadTexture App.Path + "\data\skyscraper.jpg", "Title", , , TV_COLORKEY_BLACK

  Effect.FadeIn 1500
  Scene.LoadCursor App.Path + "\data\pointer.bmp", TV_COLORKEY_BLACK, 24, 24
  
GoTo StartSim
  
   'Scene.LoadShaders App.Path + "\data\common.shader"
   
  Scene.SetCamera 25, 0, -50, 25, 0, 0
  
  bMain = True
  
  'font stuff
  scr.TextureFont_Create "Options", "Arial", 10, True, False, False
    scr.TextureFont_Create "Back", "Arial", 20, True, False, False
    scr.TextureFont_Create "Single", "Arial", 10, True, False, False
     scr.TextureFont_Create "Multi", "Arial", 10, True, False, False
     scr.TextureFont_Create "Menu", "Arial", 10, True, False, False

  MainMusic.Load App.Path + "\data\intro.mp3"
  IntroMusic.Enabled = True
  
  Do Until Effect.FadeFinished = True And IsRunning = False
    
  Dim TexInfo As TV_TEXTURE
  TexInfo = TextureFactory.GetTextureInfo(GetTex("Title"))
  
    TV.Clear
    Screen.DRAW_Texture GetTex("Image"), 0, 0, Me.ScaleWidth, Me.ScaleHeight
    Screen.DRAW_Texture GetTex("Title"), (Me.ScaleWidth / 2) - (TexInfo.Width / 3), (Me.ScaleHeight / 2) - (TexInfo.height / 3) - 150, (Me.ScaleWidth / 2) + (TexInfo.Width / 3), (Me.ScaleHeight / 2) + (TexInfo.height / 3) - 150
    'scr.CreateUserFont "menu", "Arial", 10, True, False, False
    
    
    Scene.RenderAllMeshes
    
    If bMain = True Then
     Main
    End If
    
    If bSing = True Then
     'Sing
     Exit Sub
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
       IsRunning = False
      End If
     End If
    End If
    
    TV.RenderToScreen
    
    If Inp.IsKeyPressed(TV_KEY_ESCAPE) = True Then
      Fin = 1
      
      IsRunning = False
        Set scr = Nothing
        Set Screen = Nothing
           Set Effect = Nothing
        MainMusic.Stop_
        End
        Exit Sub
    End If
    If Inp.IsKeyPressed(TV_KEY_RETURN) = True Then
      Fin = 1
      
      IsRunning = False
        Set scr = Nothing
        Set Screen = Nothing
        Sim.Refresh
        
        'start simulator
StartSim:
        DebugPanel.Show
        Sim.Show
        Start "Hello", False, Me
        Exit Sub
    End If
  DoEvents
  Loop
    Set Effect = Nothing
  MainMusic.Stop_
  End
  
ErrorHandler:
   MsgBox "An error occurred loading and initializing the TrueVision3D graphics engine." + vbCrLf + "Skyscraper requires TrueVision3D 6.2 to be installed." + vbCrLf + "It can be obtained at http://www.truevision3d.com" + vbCrLf + "Make sure it is installed properly and that all the libraries exist." + vbCrLf + vbCrLf + "Error source: " + Err.Source + vbCrLf + "Description: " + Err.Description, vbCritical
   End

End Sub

Private Sub Form_LostFocus()
'Focused = False
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
IsRunning = False
End Sub

Private Sub Form_Resize()
TV.ResizeDevice
End Sub

Private Sub Form_Unload(Cancel As Integer)

    'Call CleanUp
    End
End Sub

Private Sub IntroMusic_Timer()

MainMusic.Play

End Sub

Private Sub MainTimer_Timer()
Call MainLoop
End Sub

Private Sub WaitTimer_Timer(Index As Integer)
Elevator(Index).OpenDoor = -1
End Sub
