VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form LevelEdit 
   Caption         =   "Level Editor"
   ClientHeight    =   4140
   ClientLeft      =   4305
   ClientTop       =   2730
   ClientWidth     =   5025
   Icon            =   "LevelEdit.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4140
   ScaleWidth      =   5025
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton Command1 
      Caption         =   "Add"
      Height          =   375
      Left            =   2400
      TabIndex        =   12
      Top             =   3600
      Width           =   975
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Delete"
      Height          =   375
      Left            =   3480
      TabIndex        =   11
      Top             =   3600
      Width           =   975
   End
   Begin VB.CommandButton Command2 
      Caption         =   "OK"
      Height          =   375
      Left            =   360
      TabIndex        =   9
      Top             =   3600
      Width           =   1335
   End
   Begin VB.Frame Frame1 
      Caption         =   "Floor Information"
      Height          =   2535
      Left            =   2040
      TabIndex        =   1
      Top             =   480
      Width           =   2775
      Begin VB.TextBox Text2 
         Height          =   285
         Left            =   480
         TabIndex        =   7
         Top             =   1440
         Width           =   1815
      End
      Begin VB.TextBox Text1 
         Height          =   285
         Left            =   1680
         TabIndex        =   5
         Top             =   720
         Width           =   855
      End
      Begin MSForms.SpinButton SpinButton1 
         Height          =   375
         Left            =   720
         TabIndex        =   3
         Top             =   600
         Width           =   255
         Size            =   "450;661"
      End
      Begin VB.Label Label4 
         Alignment       =   2  'Center
         Caption         =   "Floor Type or Name"
         Height          =   255
         Left            =   480
         TabIndex        =   8
         Top             =   1080
         Width           =   1815
      End
      Begin VB.Label Label3 
         Alignment       =   2  'Center
         Caption         =   "Floor ID"
         Height          =   255
         Left            =   1680
         TabIndex        =   6
         Top             =   360
         Width           =   855
      End
      Begin VB.Label Label2 
         Alignment       =   2  'Center
         Caption         =   "Level #"
         Height          =   255
         Left            =   360
         TabIndex        =   4
         Top             =   240
         Width           =   615
      End
      Begin VB.Label Label1 
         BackColor       =   &H80000009&
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         Height          =   375
         Left            =   360
         TabIndex        =   2
         Top             =   600
         Width           =   375
      End
   End
   Begin VB.ListBox List1 
      Height          =   2790
      Left            =   120
      Sorted          =   -1  'True
      TabIndex        =   0
      Top             =   480
      Width           =   1695
   End
   Begin VB.Label Label5 
      Alignment       =   2  'Center
      Caption         =   "Levels"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   120
      Width           =   1335
   End
End
Attribute VB_Name = "LevelEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub Command1_Click()
If Text1.Text = "" Then MsgBox "Please enter a Floor ID", vbExclamation + vbOKOnly, "Level Editor": Exit Sub
If Text2.Text = "" Then MsgBox "Please enter a floor type or name", vbExclamation + vbOKOnly, "Level Editor": Exit Sub

List1.AddItem Label1.Caption + "-" + Text1.Text + "-" + Text2.Text
If Val(Label1.Caption) < DesignLowerBound Then DesignLowerBound = Val(Label1.Caption): ReDim Preserve DesignLevelGrid(DesignLowerBound To DesignUpperBound, 1) As String
If Val(Label1.Caption) > DesignUpperBound Then DesignUpperBound = Val(Label1.Caption): ReDim Preserve DesignLevelGrid(DesignLowerBound To DesignUpperBound, 1) As String

DesignLevelGrid(Val(Label1.Caption), 0) = Text1.Text
DesignLevelGrid(Val(Label1.Caption), 1) = Text2.Text
End Sub

Private Sub Command3_Click()
DesignLevelGrid(List1.ListIndex, 0) = ""
DesignLevelGrid(List1.ListIndex, 1) = ""
If List1.ListIndex = DesignLowerBound + 1 Then DesignLowerBound = List1.ListIndex
If List1.ListIndex = DesignUpperBound - 1 Then DesignUpperBound = List1.ListIndex
List1.RemoveItem List1.ListIndex
End Sub

Private Sub Form_Load()
ReDim DesignLevelGrid(0, 1) As String
End Sub

Private Sub List1_Click()
Label1.Caption = List1.ListIndex
Text1.Text = DesignLevelGrid(List1.ListIndex, 0)
Text2.Text = DesignLevelGrid(List1.ListIndex, 1)
End Sub

Private Sub SpinButton1_Change()
Label1.Caption = SpinButton1.Value
End Sub
