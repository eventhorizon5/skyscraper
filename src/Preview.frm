VERSION 5.00
Begin VB.Form Preview 
   Caption         =   "3D Preview"
   ClientHeight    =   4785
   ClientLeft      =   1650
   ClientTop       =   1545
   ClientWidth     =   7065
   Icon            =   "Preview.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4785
   ScaleWidth      =   7065
End
Attribute VB_Name = "Preview"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
isRunning = False
End Sub

Private Sub Form_Resize()
TV.ResizeDevice
End Sub
