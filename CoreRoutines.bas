Attribute VB_Name = "CoreRoutines"
'Skycraper 0.7 Alpha
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

Sub ProcessLobby()
Form1.Print "Processing Lobby..."
    i = 1
    Room(i).AddFloor GetTex("Marble4"), -160, -150, 160, 150, 0, 50, 31
    
    'Mezzanine Level
    Room(i).AddFloor GetTex("Granite"), -80, -55, 80, -46.25, 27, 30, 1
    Room(i).AddFloor GetTex("Granite"), -80, 46.25, 80, 55, 27, 30, 1
    Room(i).AddFloor GetTex("Granite"), -80, -46.25, -32.5, 46.25, 27, 6, 10
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 80, 46.25, 27, 6, 10
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, 27, 2, 10
    
    Room(i).AddFloor GetTex("Ceiling1"), -80, -55, 80, -46.25, 26.9, 30, 1
    Room(i).AddFloor GetTex("Ceiling1"), -80, 46.25, 80, 55, 26.9, 30, 1
    Room(i).AddFloor GetTex("Ceiling1"), -80, -46.25, -32.5, 46.25, 26.9, 11, 10
    Room(i).AddFloor GetTex("Ceiling1"), 32.5, -46.25, 80, 46.25, 26.9, 11, 10
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 46.25, 26.9, 2, 10
    
    'End of Mezzanine
    
    External.AddWall GetTex("LobbyFront"), -160, -150, 160, -150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, -150, 160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, 150, -160, 150, 75, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -160, 150, -160, -150, 75, 0, 3, 1
              
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, -150 + 0.1, 160 - 0.1, -150 + 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, -150 + 0.1, 160 - 0.1, 150 - 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, 150 - 0.1, -160 + 0.1, 150 - 0.1, 75, 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, 150 - 0.1, -160 + 0.1, -150 + 0.1, 75, 0, 3, 1
              
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 75, 0, 2, 6
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, 0, 2, 6
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, 0, 2, 6
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 75, 0, 2, 6
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, 0, 2, 6
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 0, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, 0, 0.8, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 46.25 - 0.1, 25, 0, 4, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 7.5, 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 0, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, 0, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 25, 0, 8, 2
    'cut side wall start
    'Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 0, 1, 2
    'Shafts.AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 0, 4, 2
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 0, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 25, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 25, 8, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, 25, 0.8, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 46.25 - 0.1, 25, 25, 4, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 3.5, 27 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 25, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25, 12.5 + 0.1, -46.25, 25, 25, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25, 12.5 + 0.1, 46.25, 25, 25, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 25, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, 46.25 - 0.1, 25, 50, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 50, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 25, 50, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 50, 8, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 0, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, -30, 25, 0, 0.8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -16, -12.5, 46.25, 25, 0, 4, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -30, -12.5, -16, 7.5, 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 0, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 0, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 0, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    'cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 0, 1, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 0, 4, 2
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 0, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 0, 8, 2
    
    'ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 25, 2, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 25, 8, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 25, 2, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 25, 8, 2
    
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 25, 2, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 25, 8, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 25, 2, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 25, 2, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 25, 8, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, -30, 25, 25, 0.8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -16, -12.5, 46.25, 25, 25, 4, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -30, -12.5, -16, 3.5, 27 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 25, 8, 2
    'cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 25, 1, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 25, 4, 2
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 50, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 50, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 50, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 50, 8, 2
    
    'Ceiling
    Room(i).AddFloor GetTex("Ceiling1"), -160, -150, 160, -46.25, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, 46.25, 160, 150, (i * 25) + 49.5, 10, 5
    Room(i).AddFloor GetTex("Ceiling1"), -160, -46.25, -32.5, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), 32.5, -46.25, 160, 46.25, (i * 25) + 49.5, 3, 5
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 49.5, 2, 5
    
    Call InitObjectsForFloor(Int(i))
    
End Sub
Sub Process2to39()

    'Floors 2 to 39
    For i = 2 To 39
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -160, -150, 160, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -160, -150, 160, -46.25, (i * 25) + 25, 30, 5
    Room(i).AddFloor GetTex("Granite"), -160, 46.25, 160, 150, (i * 25) + 25, 30, 5
    Room(i).AddFloor GetTex("Granite"), -160, -46.25, -32.5, 46.25, (i * 25) + 25, 11, 10
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 160, 46.25, (i * 25) + 25, 11, 10
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 25, 2, 10
    
    Room(i).AddFloor GetTex("Marble3"), -160, -150, 160, -46.25, (i * 25) + 49.5, 30, 5
    Room(i).AddFloor GetTex("Marble3"), -160, 46.25, 160, 150, (i * 25) + 49.5, 30, 5
    Room(i).AddFloor GetTex("Marble3"), -160, -46.25, -32.5, 46.25, (i * 25) + 49.5, 11, 10
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 160, 46.25, (i * 25) + 49.5, 11, 10
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (i * 25) + 49.5, 2, 10
    
    External.AddWall GetTex("MainWindows"), -160, -150, 160, -150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), 160, -150, 160, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 160, 150, -160, 150, 25, (i * 25) + 25, 7.6, 1
    External.AddWall GetTex("MainWindows"), -160, 150, -160, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 46.25 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 46.25, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 46.25, -32.5, 46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 46.25, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 46.25, 32.5, 46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 46.25, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, 160, 71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -160, 46.3, -32.5, 46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.3, 160, 46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -160, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 160, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -96, -71.3, -96, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -96, 71.3, -96, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -32, -71.3, -32, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -32, 71.3, -32, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 32, -71.3, 32, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32, 71.3, 32, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -71.3, 96, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 96, 71.3, 96, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -46.3, 96, 46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -96, -46.3, -96, 46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -160, 0, -32, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 160, 0, 32, 0, 25, (i * 25) + 25, 2, 1
    
    Next i
Form1.Print "done"
    'Landings
    External.AddFloor GetTex("BrickTexture"), -160, -150, -135, 150, (40 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 160, -150, 135, 150, (40 * 25) + 25, 10, 10

End Sub
Sub Process40to79()
    'Floors 40 to 79 (minus 14 feet on both sides where 20=8 feet)
    For i = 40 To 79
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -135, -150, 135, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -135, -150, 135, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -135, 30.83, 135, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -135, -46.25, -32.5, 30.83, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 135, 30.83, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 30.83, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -135, -150, 135, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -135, 30.83, 135, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -135, -46.25, -32.5, 30.83, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 135, 30.83, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 30.83, (i * 25) + 49.5, 1, 3
   
    External.AddWall GetTex("MainWindows"), -135, -150, 135, -150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), 135, -150, 135, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 135, 150, -135, 150, 25, (i * 25) + 25, 6.5, 1
    External.AddWall GetTex("MainWindows"), -135, 150, -135, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 30.83 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 30.83 - 0.1, -32.5 + 0.1, 30.83 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 30.83 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 30.83 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 30.83 - 0.1, 32.5 - 0.1, 30.83 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 30.83 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
        
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 30.83, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 30.83, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 30.83, -32.5, 30.83, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 30.83, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 30.83, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 30.83, 32.5, 30.83, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 30.83, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, 55.83, 135, 55.83, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -32.5, 30.83, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 30.83, 135, 30.83, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 135, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -81, -71.3, -81, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -81, 55.83, -81, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -27, -71.3, -27, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -27, 55.83, -27, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 27, -71.3, 27, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 27, 55.83, 27, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 81, -71.3, 81, -150, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 81, 55.83, 81, 150, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), 81, -46.3, 81, 30.83, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -81, -46.3, -81, 30.83, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, 0, -32, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 0, 32, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, 102, 135, 102, 25, (i * 25) + 25, 2, 1
    
    Next i
Form1.Print "done"
    
    External.AddFloor GetTex("BrickTexture"), -135, -150, -110, 150, (80 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 135, -150, 110, 150, (80 * 25) + 25, 10, 10

End Sub
Sub Process80to117()
    'Floors 80 to 117
    For i = 80 To 117
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -110, -150, 110, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, 15.41, 110, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -32.5, 15.41, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 110, 15.41, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 15.41, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, 15.41, 110, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -32.5, 15.41, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 110, 15.41, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 15.41, (i * 25) + 49.5, 1, 3
    
    External.AddWall GetTex("MainWindows"), -110, -150, 110, -150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), 110, -150, 110, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 110, 150, -110, 150, 25, (i * 25) + 25, 5, 1
    External.AddWall GetTex("MainWindows"), -110, 150, -110, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 15.41 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 15.41 - 0.1, -32.5 + 0.1, 15.41 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 15.41 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 15.41 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 15.41 - 0.1, 32.5 - 0.1, 15.41 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 15.41 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 15.41, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 15.41, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 15.41, -32.5, 15.41, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 15.41, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 15.41, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 15.41, 32.5, 15.41, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 15.41, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 70, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -70, 40, -12.5, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 40, 70, 40, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -32.5, 15.41, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 15.41, 70, 15.41, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 70, -46.3, 25, (i * 25) + 25, 2, 1
        
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, -70, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -71.3, 70, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 15.41, 70, 40, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 40, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -110, 120, -12.5, 120, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, 120, 12.5, 120, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -110, 90, -12.5, 90, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, 90, 12.5, 90, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -110, 40, -70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, 40, 70, 40, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -110, -12.5, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -110, -46.3, -70, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -46.3, 70, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
    
    
    Next i
Form1.Print "done"
    External.AddFloor GetTex("BrickTexture"), -110, -150, -85, 150, (118 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 110, -150, 85, 150, (118 * 25) + 25, 10, 10

End Sub
Sub Process118to132()
    'Floors 118 to 133 (minus 10 feet)
    For i = 118 To 132
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -85, -150, 85, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * 25) + 49.5, 1, 3
     
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 0 - 0.1, -32.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 0 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 0 - 0.1, 32.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 0 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 25, -12.5, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 0, 50, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
        
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 25, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -85, 90, -12.5, 90, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 90, 12.5, 90, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 120, -12.5, 120, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 120, 12.5, 120, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 60, -12.5, 60, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 60, 12.5, 60, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 25, -50, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -110, -12.5, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, -46.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
    
    Next i

End Sub

Sub DestroyObjects(Floor As Integer)
'currently, 150 objects per floor (150*138)
For i = (1 + (150 * (Floor - 1))) To (150 + (150 * (Floor - 1)))
Scene.DestroyMesh Objects(i)
'Objects(i) = Null
Next i

'objectindex + (150 * (currentfloor - 1 ))

End Sub
Sub InitObjectsForFloor(Floor As Integer)

If Floor = 1 Then

'Column 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, 45
Objects(i).Optimize

'Column 2
i = 2 + (150 * (Floor - 1))
Call Init_Objects(Floor, 2)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, 15
Objects(i).Optimize

'Column 3
i = 3 + (150 * (Floor - 1))
Call Init_Objects(Floor, 3)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, -15
Objects(i).Optimize

'Column 4
i = 4 + (150 * (Floor - 1))
Call Init_Objects(Floor, 4)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, -45
Objects(i).Optimize

'Column 5
i = 5 + (150 * (Floor - 1))
Call Init_Objects(Floor, 5)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, 45
Objects(i).Optimize

'Column 6
i = 6 + (150 * (Floor - 1))
Call Init_Objects(Floor, 6)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, 15
Objects(i).Optimize

'Column 7
i = 7 + (150 * (Floor - 1))
Call Init_Objects(Floor, 7)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, -15
Objects(i).Optimize

'Column 8
i = 8 + (150 * (Floor - 1))
Call Init_Objects(Floor, 8)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, -45
Objects(i).Optimize

End If

If Floor >= 2 And Floor <= 39 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 2 To 27
'Windows 2 to 27
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 1) * 12) + ((j - 1) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 28
i = 28 + (150 * (Floor - 1))
Call Init_Objects(Floor, 28)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 29 To 54
'Windows 29 to 54
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 28) * 12) + ((j - 28) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'Window 55
i = 55 + (150 * (Floor - 1))
Call Init_Objects(Floor, 55)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 159.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 56 to 79
For j = 56 To 79
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 159.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 55) * 12) + ((j - 55) * 0.1)
'Objects(i).Optimize
Next j

'Window 80
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -159.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 81 to 104
For j = 81 To 104
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -159.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 80) * 12) + ((j - 80) * 0.1)
'Objects(i).Optimize
Next j

End If

If Floor >= 40 And Floor <= 79 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 134.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 134.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -134.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -134.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 129, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 72
'Windows 52 to 72
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 51) * 12) + ((j - 51) * 0.1), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 73
i = 73 + (150 * (Floor - 1))
Call Init_Objects(Floor, 73)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 129, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 74 To 94
'Windows 74 to 94
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 73) * 12) + ((j - 73) * 0.1), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

End If

If Floor >= 80 And Floor <= 117 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 109.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 109.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -109.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -109.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 103, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 68
'Windows 52 to 68
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 69
i = 69 + (150 * (Floor - 1))
Call Init_Objects(Floor, 69)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 103, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 70 To 86
'Windows 70 to 86
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 69) * 12) + ((j - 69) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

End If

If Floor >= 118 And Floor <= 134 Then
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 84.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 84.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -84.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -84.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 78, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 64
'Windows 52 to 64
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 65
i = 65 + (150 * (Floor - 1))
Call Init_Objects(Floor, 65)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 78, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 66 To 78
'Windows 66 to 78
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

End If

If Floor = 133 Or Floor = 134 Then
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).Load3DsMesh App.Path + "\objects\swimpool.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).SetRotation 0, -1.58, 0
Objects(i).ScaleMesh 0.2, 0.2, 0.2
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 0 + ((134 * 25) + 25), 92
'Objects(i).Optimize
End If

If Floor = 135 Or Floor = 136 Then
Floor = 135
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 62 To 74
'Windows 62 to 74
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window4.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

'-----------------------------
Floor = 136
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition 59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), -149.5
Objects(i).SetPosition -59.5, 12.01 + ((Floor * 25) + 25), 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), 12.01 + ((Floor * 25) + 25), 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, 12.01 + ((Floor * 25) + 25), 149.5
Objects(i).SetPosition 53, 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize

For j = 62 To 74
'Windows 62 to 74
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).Load3DsMesh App.Path + "\objects\window5.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), 12.01 + ((Floor * 25) + 25), -149.5
'Objects(i).Optimize
Next j

End If

If Floor = 137 Then
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).Load3DsMesh App.Path + "\objects\piping.3ds", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.04, 0.04, 0.04
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 10 + ((Floor * 25) + 25), 60
Objects(i).Optimize
End If

If Floor = 138 Then
End If

End Sub
Sub Init_Objects(Floor As Integer, ObjectIndex As Integer)
'currently, 150 objects per floor (150*138)
i = ObjectIndex + (150 * (Floor - 1))
Set Objects(i) = New TVMesh
Set Objects(i) = Scene.CreateMeshBuilder("Objects " + Str$(i))

'objectindex + (150 * (currentfloor - 1 ))
    
End Sub

Sub ProcessOtherFloors()
i = 133
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -85, -150, 85, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (i * 25) + 25, 1, 3
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * 25) + 49.5, 1, 3
     
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 0 - 0.1, -32.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 0 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 0 - 0.1, 32.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 0 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
        
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 25, -12.5, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, 0, 50, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
        
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 25, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 25, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Individual Rooms
    'Room(i).AddWall GetTex("Wall3"), -85, 90, -12.5, 90, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 90, 12.5, 90, 25, (i * 25) + 25, 2, 1
    'Room(i).AddWall GetTex("Wall3"), -85, 120, -12.5, 120, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 120, 12.5, 120, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, 60, 85, 60, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 60, 12.5, 60, 25, (i * 25) + 25, 2, 1
    'Room(i).AddWall GetTex("Wall3"), -85, 25, -50, 25, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 85, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -110, -12.5, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -85, -46.3, -50, -46.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -46.3, 50, -46.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
        
i = 134
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -85, -150, 85, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, (i * 25) + 25, 10, 3
    'Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, (i * 25) + 25, 3, 3
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, (i * 25) + 25, 1, 3
    
    'modified floor for pool
    Room(i).AddFloor GetTex("Granite"), -50, 0, 50, 60, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), -85, 0, -50, 150, (i * 25) + 25, 10, 3
    Room(i).AddFloor GetTex("Granite"), 50, 0, 85, 150, (i * 25) + 25, 10, 3
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (i * 25) + 49.5, 10, 3
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (i * 25) + 49.5, 3, 3
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * 25) + 49.5, 1, 3
     
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, 25, (i * 25) + 25, 4, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 0 - 0.1, -32.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 0 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 0 - 0.1, 32.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 0 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
        
Form1.Print "done"
    External.AddFloor GetTex("BrickTexture"), -85, -150, -60, 150, (135 * 25) + 25, 10, 10
    External.AddFloor GetTex("BrickTexture"), 85, -150, 60, 150, (135 * 25) + 25, 10, 10
    
Form1.Print "Processing Floors 135 to 137";
    'Floors 135 to 137 (minus 6 feet)
    
    'Floor 135
    
    'For i = 135 To 137
    i = 135
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("Granite"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("Marble3"), -60, -15.42, 60, 0, (i * 25) + 49.5, 8, 1
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -15.42 - 0.1, 32.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -15.42 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -15.42, 32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -15.42, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Floor 136
    i = 136
    Form1.Print ".";
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("Granite"), -60, -15.42, 60, 0, (i * 25) + 25, 8, 1
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("Marble3"), -60, -15.42, 60, 150, (i * 25) + 49.5, 8, 8.6
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -15.42 - 0.1, 32.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -15.42 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -15.42, 32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -15.42, 32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
    'Floor 137
    i = 137
    Form1.Print ".";
    'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 32.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 49.5, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 49.5, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, (i * 25) + 49.5, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 49.5, 2, 2.4
    
    'External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    'External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    'External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    External.AddWall GetTex("Ceiling1"), -60, -150, 60, -150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("Ceiling1"), 60, -150, 60, 150, 25, (i * 25) + 25, 7, 1
    External.AddWall GetTex("Ceiling1"), 60, 150, -60, 150, 25, (i * 25) + 25, 3, 1
    External.AddWall GetTex("Ceiling1"), -60, 150, -60, -150, 25, (i * 25) + 25, 7, 1
    
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, -150 + 0.1, 60 - 0.1, -150 + 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, -150 + 0.1, 60 - 0.1, 150 - 0.1, 25, (i * 25) + 25, 7, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, 150 - 0.1, -60 + 0.1, 150 - 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, 150 - 0.1, -60 + 0.1, -150 + 0.1, 25, (i * 25) + 25, 7, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
Form1.Print "done"


'Roof Layout
'Mesh.AddFloor GetTex("Granite"), -60, -150, 60, 150, (I * 25) + 25, 10, 10
i = 138
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (i * 25) + 25, 8, 3.8
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, (i * 25) + 25, 8, 8.6
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, (i * 25) + 25, 1.4, 2.4
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, (i * 25) + 25, 2, 2.4
    
    Room(i).AddFloor GetTex("BrickTexture"), -32.5, -46.25, -12.5, -15.42, (i * 25) + 50.1, 2, 2.4
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, 2, 2
    Stairs(i).AddWall GetTex("Ceiling1"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, 2, 2
    'Stairs(i).AddWall GetTex("Ceiling1"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, 2, 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -30, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, -15.42, 25, (i * 25) + 25, 3, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -30, -12.5, -16, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -15.42, -32.5, -15.42, 25, (i * 25) + 25, 1, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -15.42, -32.5, -46.25, 25, (i * 25) + 25, 3, 2
    
End Sub
