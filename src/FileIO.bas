Attribute VB_Name = "FileIO"
'Skyscraper 1.1 Alpha - File I/O processing routines
'Copyright ©2005 Ryan Thoryk
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

Public Sub LoadGlobals(FileName As String)

'Temporary calls to Triton module, since building is not in a data file yet
Call Triton_Globals
Exit Sub

End Sub

Public Sub LoadTextures(FileName As String)

'Temporary calls to Triton module, since building is not in a data file yet
Call Triton_LoadTextures
Exit Sub

End Sub

Public Sub LoadBuilding(FileName As String)

'Temporary calls to Triton module, since building is not in a data file yet
Call Triton_ProcessBasement
Call Triton_ProcessFloors
Exit Sub

End Sub

Public Sub LoadElevators(FileName As String)

'Temporary calls to Triton module, since building is not in a data file yet
Call Triton_ProcessElevators

End Sub
