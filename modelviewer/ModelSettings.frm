VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmSettings 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Settings"
   ClientHeight    =   2460
   ClientLeft      =   2865
   ClientTop       =   2550
   ClientWidth     =   6030
   Icon            =   "ModelSettings.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2460
   ScaleWidth      =   6030
   Begin VB.CheckBox Check1 
      Caption         =   "Generate Materials (use this if model doesn't have textures)"
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   600
      Width           =   5295
   End
   Begin MSComctlLib.Slider Slider1 
      Height          =   375
      Left            =   1080
      TabIndex        =   2
      Top             =   120
      Width           =   4815
      _ExtentX        =   8493
      _ExtentY        =   661
      _Version        =   393216
      Min             =   100
      Max             =   10000
      SelStart        =   400
      TickFrequency   =   500
      Value           =   400
   End
   Begin VB.CommandButton Command1 
      Caption         =   "OK"
      Height          =   375
      Left            =   2400
      TabIndex        =   0
      Top             =   1920
      Width           =   1215
   End
   Begin VB.Label Label1 
      Caption         =   "Light Range"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   1815
   End
End
Attribute VB_Name = "frmSettings"
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

Private Sub Check1_Click()
If Check1.Value = 1 Then Viewer.QuickMesh = True
If Check1.Value = 0 Then Viewer.QuickMesh = False
End Sub

Private Sub Command1_Click()
Me.Hide
End Sub

Private Sub Slider1_Change()
Viewer.LightRange = Slider1.Value
End Sub
