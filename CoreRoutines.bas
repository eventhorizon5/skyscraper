Attribute VB_Name = "CoreRoutines"
'Skycraper 0.7 Alpha
'Copyright (C) 2003 Ryan Thoryk
'http://www.tliquest.net/skyscraper
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
                  
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, 75, -1, 1
                  
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, 0, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, 0, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, 0, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, 0, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, 0, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, 0, (10 * 0.086) * 4, 3 * 4
    
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 7.5, 19.5, (10.3 * 0.086) * 4, (7.5 * 0.086) * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 3.5, 19.5 + 27, (10.3 * 0.086) * 4, (3.5 * 0.086) * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 25, 50, (10.3 * 0.086) * 4, (25 * 0.086) * 2
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 0, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -39.5 - 1, 19.5, 0, (16.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 46.25 - 0.1, 19.5, 0, (62.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -32.5 + 0.1, -12.5 - 0.1, -30 - 0.1, 19.5, 0, (62.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, 46.25 - 0.1, 7.5, 19.5, 8, (7.5 * 0.08)
    'right cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 0, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, 0, 2, 2
    'left side wall start
    'Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 25, 0, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -16 - 0.1, 12.5 + 0.1, 46.25 - 0.1, 19.5, 0, (62.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 7.5, 19.5, 8, (7.5 * 0.08)
    'left side wall end
    
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 25, 0, 8, 2
    'right cut side wall start
    'Shafts.AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 0, 1, 2
    'Shafts.AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 0, 4, 2
    'right cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 0, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 0, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 25, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 25, 8, 2
    'right cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, -30 + 0.1, 19.5, 27, (16.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 46.25 - 0.1, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, 46.25 - 0.1, 7.5, 27 + 19.5, 8, (7.5 * 0.08)
    'right cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 25, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25, 12.5 + 0.1, -46.25, 25, 25, 2, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25, 12.5 + 0.1, 46.25, 25, 25, 8, 2
    'left side wall start
    'Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25, 12.5 + 0.1, 46.25, 25, 25, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -16 - 0.1, 12.5 + 0.1, 46.25 - 0.1, 19.5 + 27, 0, (62.25 * 0.086), (19.5 * 0.08)
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 7.5, 19.5 + 27, 8, (7.5 * 0.08)
    'left side wall end
    
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 25, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 25, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -46.25 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -46.25 + 0.1, -12.5 - 0.1, 46.25 - 0.1, 21, 54, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 46.25 - 0.1, -32.5 + 0.1, 46.25 - 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 46.25 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, 50, 8, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, 46.25 - 0.1, 21, 54, 8, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 46.25 - 0.1, 32.5 - 0.1, 46.25 - 0.1, 25, 50, 2, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 46.25 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, 50, 8, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 0, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, -40.3, 19.5, 0, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -16, -12.5, 46.25, 19.5, 0, (62.25 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -32.5, -12.5, -30, 19.5, 0, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 7.5, 19.5, 8, (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 0, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 0, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 0, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 19.5, 0, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 7.5, 19.5, 8, (7.5 * 0.08)
    'left cut side wall end
    
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
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, -40.3, 19.5, 27, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -16, -12.5, 46.25, 19.5, 27, (62.25 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -32.5, -12.5, -30, 19.5, 27, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 7.5, 19.5 + 27, 8, (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 25, 2, 2
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 25, 25, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 19.5, 27, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 7.5, 19.5 + 27, 8, (7.5 * 0.08)
    'left cut side wall end
    'cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, -30, 25, 25, 1, 2
    'ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -16, 12.5, 46.25, 25, 25, 4, 2
    'cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, 46.25, 32.5, 46.25, 25, 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, 46.25, 32.5, -46.25, 25, 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, -46.25, -12.5, -46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 21, 54, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -12.5, 46.25, -32.5, 46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), -32.5, 46.25, -32.5, -46.25, 25, 50, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall2"), 32.5, -46.25, 12.5, -46.25, 25, 50, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall2"), 12.5, -46.25, 12.5, 46.25, 21, 54, 8, 2
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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall3"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 46.25, 19.5, (i * 25) + 25, (62.25 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 5.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 46.25, -32.5, 46.25, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 46.25, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 46.25, 32.5, 46.25, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 46.25, 32.5, -46.25, 25, (i * 25) + 25, 8, 2
     
    
    
    'Room(I) Walls
    'Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), -160, 46.3, -32.5, 46.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, 46.3, 160, 46.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -160, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 32.5, -46.3, 160, -46.3, 25, (i * 25) + 25, 2, 1
    
    'top walls
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, -110 - 3.9, -71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -110 + 3.9, -71.3, -45 - 3.9, -71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -45 + 3.9, -71.3, -20 - 3.9, -71.3, 19.5, (i * 25) + 25, ((45 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -20 + 3.9, -71.3, 45 - 3.9, -71.3, 19.5, (i * 25) + 25, ((45 + 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 45 + 3.9, -71.3, 110 - 3.9, -71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110 + 3.9, -71.3, 160, -71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, 160, -71.3, 5.5, (i * 25) + 25 + 19.5, ((160 * 2) * 0.086), (5.5 * 0.08)
    
    'bottom walls
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, -110 - 3.9, 71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -110 + 3.9, 71.3, -45 - 3.9, 71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -45 + 3.9, 71.3, 20 - 3.9, 71.3, 19.5, (i * 25) + 25, ((45 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 71.3, 45 - 3.9, 71.3, 19.5, (i * 25) + 25, ((45 + 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 45 + 3.9, 71.3, 110 - 3.9, 71.3, 19.5, (i * 25) + 25, ((110 - 45) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110 + 3.9, 71.3, 160, 71.3, 19.5, (i * 25) + 25, ((160 - 110) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, 160, 71.3, 5.5, (i * 25) + 25 + 19.5, ((160 * 2) * 0.086), (5.5 * 0.08)
    
    'top middle walls
    Room(i).AddWall GetTex("Wall3"), -160, -46.25, -108 - 3.9, -46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -108 + 3.9, -46.25, -87 - 3.9, -46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -87 + 3.9, -46.25, -32.5, -46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.25, 39 - 3.9, -46.25, 19.5, (i * 25) + 25, ((39 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 39 + 3.9, -46.25, 87 - 3.9, -46.25, 19.5, (i * 25) + 25, ((87 - 39) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 87 + 3.9, -46.25, 108 - 3.9, -46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 108 + 3.9, -46.25, 160, -46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -160, -46.25, -32.5, -46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, -46.25, 160, -46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, ((46.25 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 50, -20, 32.5, -20, 25, (i * 25) + 25, ((50 - 32.5) * 0.086), 1
    
    'bottom middle walls
    Room(i).AddWall GetTex("Wall3"), -160, 46.25, -108 - 3.9, 46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -108 + 3.9, 46.25, -87 - 3.9, 46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -87 + 3.9, 46.25, -32.5, 46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.25, 87 - 3.9, 46.25, 19.5, (i * 25) + 25, ((87 - 32) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 87 + 3.9, 46.25, 108 - 3.9, 46.25, 19.5, (i * 25) + 25, ((108 - 87) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 108 + 3.9, 46.25, 160, 46.25, 19.5, (i * 25) + 25, ((160 - 108) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -160, 46.25, -32.5, 46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.25, 160, 46.25, 5.5, (i * 25) + 25 + 19.5, ((160 - 32.5) * 0.086), (5.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -96, -71.3, -96, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -96, 71.3, -96, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -32, -71.3, -32, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -32, 71.3, -32, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 32, -71.3, 32, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 32, 71.3, 32, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -71.3, 96, -150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 96, 71.3, 96, 150, 25, (i * 25) + 25, ((150 - 71.3) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 96, -46.3, 96, 46.3, 25, (i * 25) + 25, ((46.3 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -96, -46.3, -96, 46.3, 25, (i * 25) + 25, ((46.3 * 2) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -160, 0, -32, 0, 25, (i * 25) + 25, ((160 - 32) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 160, 0, 32, 0, 25, (i * 25) + 25, ((160 - 32) * 0.086), 1
    
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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 30.83, 19.5, (i * 25) + 25, (46.83 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 30.83, 5.5, 19.5 + (i * 25) + 25, (76.83 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 30.83, -32.5, 30.83, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 30.83, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 30.83, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 30.83, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 30.83, 32.5, 30.83, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 30.83, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

    'Room(I) Walls
    
    'Top of upper hallway
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, -71.3, -37 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, -71.3, -20 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -20 + 3.9, -71.3, 37 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, -71.3, 90 - 3.9, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, -71.3, 135, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom of lower hallway
    Room(i).AddWall GetTex("Wall3"), -135, 55.83, 135, 55.83, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), -135, 55.83, -90 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 55.83, -37 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, 55.83, 20 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 55.83, 37 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, 55.83, 90 - 3.9, 55.83, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, 55.83, 135, 55.83, 19.5, (i * 25) + 25, 2, 1
    
    'Top of lower hallway
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -32.5, 30.83, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, 30.83, -90 - 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 30.83, -72 - 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -72 + 3.9, 30.83, -32.5, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 30.83, 32.5, 30.83, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, 30.83, 90 + 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 - 3.9, 30.83, 72 + 3.9, 30.83, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 72 - 3.9, 30.83, 32.5, 30.83, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom of upper hallway
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -135, -46.3, -90 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, -46.3, -72 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -72 + 3.9, -46.3, -32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 135, -46.3, 90 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 90 - 3.9, -46.3, 72 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 72 - 3.9, -46.3, 40 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 40 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
        
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
    
    'Room(i).AddWall GetTex("Wall3"), -135, -71.3, 135, -71.3, 25, (i * 25) + 25, 9, 1
    
    'Lower room split wall
    Room(i).AddWall GetTex("Wall3"), -135, 102, 135, 102, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), -135, 102, -90 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -90 + 3.9, 102, -37 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), -37 + 3.9, 102, 20 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 20 + 3.9, 102, 37 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 37 + 3.9, 102, 90 - 3.9, 102, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 90 + 3.9, 102, 135, 102, 19.5, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, 4, 1
    Room(i).AddWall GetTex("Wall3"), 50, -20, 32.5, -20, 25, (i * 25) + 25, 4, 1
    
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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 15.41, 19.5, (i * 25) + 25, (32 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 15.41, 5.5, 19.5 + (i * 25) + 25, (62 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 15.41, -32.5, 15.41, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 15.41, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 15.41, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 15.41, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 15.41, 32.5, 15.41, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 15.41, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    'Room(i).AddWall GetTex("Wall3"), -50, 40, -12.5, 40, 25, (i * 25) + 25, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 12.5, 40, 50, 40, 25, (i * 25) + 25, 2, 1
    
    'Bottom Horizontal Hallway, 2 Top Walls
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -32.5, 15.41, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 15.41, -61 - 3.9, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -61 + 3.9, 15.41, -32.5, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 15.41, 32.5, 15.41, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 15.41, 61 + 3.9, 15.41, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 61 - 3.9, 15.41, 32.5, 15.41, 19.5, (i * 25) + 25, 9, 1
    
    'Top Horizontal Hallway, 2 Bottom Walls
    Room(i).AddWall GetTex("Wall3"), -70, -46.3, -32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -46.3, -61 - 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -61 + 3.9, -46.3, -32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.3, 61 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 61 - 3.9, -46.3, 39 + 3.9, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 39 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 9, 1
        
    'Right Hallways, Right Wall
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, 150, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -118 + 3.9, -70, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -102 + 3.9, -70, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, -60 + 3.9, -70, 27 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 27 + 3.9, -70, 66 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 66 + 3.9, -70, 102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 102 + 3.9, -70, 116 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -70, 116 + 3.9, -70, 150, 19.5, (i * 25) + 25, 9, 1
    
    'Left Hallways, Left Wall
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, 150, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -118 + 3.9, 70, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -102 + 3.9, 70, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, -60 + 3.9, 70, 27 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 27 + 3.9, 70, 66 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 66 + 3.9, 70, 102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 102 + 3.9, 70, 116 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 70, 116 + 3.9, 70, 150, 19.5, (i * 25) + 25, 9, 1
    
    'center bottom hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 40, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 116 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 116 - 3.9, -12.5, 102 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 102 - 3.9, -12.5, 66 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 66 - 3.9, -12.5, 40, 19.5, (i * 25) + 25, 9, 1
    
    'center bottom hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 40, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 116 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 116 - 3.9, 12.5, 102 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 102 - 3.9, 12.5, 66 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 66 - 3.9, 12.5, 40, 19.5, (i * 25) + 25, 9, 1
    
    'center top hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -118 + 3.9, -12.5, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -102 + 3.9, -12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'center top hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -118 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -118 + 3.9, 12.5, -102 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -102 + 3.9, 12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -110, 110, -70, 110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 110, -12.5, 110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 110, 70, 110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 110, 12.5, 110, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 75, -70, 75, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 75, -12.5, 75, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 75, 70, 75, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 75, 12.5, 75, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 40, -70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 40, -12.5, 40, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, 40, 70, 40, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 40, 12.5, 40, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -110, -70, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -110, -12.5, -110, 25, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 110, -110, 70, -110, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -110, 12.5, -110, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -71.3, -70, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -71.3, 70, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -10, -32.5, -10, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 110, -10, 32.5, -10, 25, (i * 25) + 25, 2, 1
    
    'Left Bottom Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, 40, 50, 150, 25, (i * 25) + 25, 9, 1
    
    'Right Bottom Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, 40, -50, 150, 25, (i * 25) + 25, 2, 1
    
    'Left Top Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -150, 25, (i * 25) + 25, 9, 1
    
    'Right Top Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -150, 25, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 45, -46.25, 45, -25, 25, (i * 25) + 25, 4, 1
    Room(i).AddWall GetTex("Wall3"), 45, -25, 32.5, -25, 25, (i * 25) + 25, 4, 1
    
    
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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 19.5, (i * 25) + 25, (16 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 5.5, 19.5 + (i * 25) + 25, (46.25 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 25, -12.5, 25, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 25, 50, 25, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, 0, -32.5, 0, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 32.5, 0, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -50, -46.3, -32.5, -46.3, 25, (i * 25) + 25, 9, 1
    'This wall is cut for the service door
    Room(i).AddWall GetTex("Wall3"), 50, -46.3, 32.5, -46.3, 5.5, (i * 25) + 25 + 19.5, 2, 1
    Room(i).AddWall GetTex("Wall3"), 50, -46.3, 39 + 3.9, -46.3, 19.5, (i * 25) + 25, 2, 1
    Room(i).AddWall GetTex("Wall3"), 39 - 3.9, -46.3, 32.5, -46.3, 19.5, (i * 25) + 25, 2, 1
        
    'Top Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, -60 + 3.9, -50, -46.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -46.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -60 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, -60 + 3.9, 50, -46.3, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 0, -50, 13 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -50, 13 + 3.9, -50, 25, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 0, 50, 13 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 50, 13 + 3.9, 50, 25, 19.5, (i * 25) + 25, 9, 1
    
    'Bottom Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 126 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 126 - 3.9, -12.5, 115 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 115 - 3.9, -12.5, 67 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 67 - 3.9, -12.5, 54 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, 54 - 3.9, -12.5, 25, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 25, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 126 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 126 - 3.9, 12.5, 115 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 115 - 3.9, 12.5, 67 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 67 - 3.9, 12.5, 54 + 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 54 - 3.9, 12.5, 25, 19.5, (i * 25) + 25, 9, 1
    
    'Top Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -117 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -117 + 3.9, -12.5, -101 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), -12.5, -101 + 3.9, -12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, (i * 25) + 25 + 19.5, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -117 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -117 + 3.9, 12.5, -101 - 3.9, 19.5, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -101 + 3.9, 12.5, -71.3, 19.5, (i * 25) + 25, 9, 1
    
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
    Room(i).AddWall GetTex("Wall3"), -85, -71.3, -50, -71.3, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -71.3, 50, -71.3, 25, (i * 25) + 25, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -20, -32.5, -20, 25, (i * 25) + 25, 9, 1
    Room(i).AddWall GetTex("Wall3"), 85, -20, 32.5, -20, 25, (i * 25) + 25, 2, 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, (i * 25) + 25, 4, 1
    
    Next i

End Sub

Sub DestroyObjects(Floor As Integer)
'currently, 150 objects per floor (150*138)
For i = (1 + (150 * (Floor - 1))) To (150 + (150 * (Floor - 1)))
'The destroymesh function is broken
On Error Resume Next
Objects(i).Enable False
'Sleep 2
Scene.DestroyMesh Objects(i)
Set Objects(i) = Nothing
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
'Objects(i).Optimize

'Column 2
i = 2 + (150 * (Floor - 1))
Call Init_Objects(Floor, 2)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, 15
'Objects(i).Optimize

'Column 3
i = 3 + (150 * (Floor - 1))
Call Init_Objects(Floor, 3)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, -15
'Objects(i).Optimize

'Column 4
i = 4 + (150 * (Floor - 1))
Call Init_Objects(Floor, 4)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition -75, 13.5, -45
'Objects(i).Optimize

'Column 5
i = 5 + (150 * (Floor - 1))
Call Init_Objects(Floor, 5)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, 45
'Objects(i).Optimize

'Column 6
i = 6 + (150 * (Floor - 1))
Call Init_Objects(Floor, 6)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, 15
'Objects(i).Optimize

'Column 7
i = 7 + (150 * (Floor - 1))
Call Init_Objects(Floor, 7)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, -15
'Objects(i).Optimize

'Column 8
i = 8 + (150 * (Floor - 1))
Call Init_Objects(Floor, 8)
Objects(i).Load3DsMesh App.Path + "\objects\column.3ds", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.105, 0.105, 0.105
Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 75, 13.5, -45
'Objects(i).Optimize

'Door 1
i = 9 + (150 * (Floor - 1))
Call Init_Objects(Floor, 9)
Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, 0, -36.4

'Door 2
i = 10 + (150 * (Floor - 1))
Call Init_Objects(Floor, 10)
Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, 27, -36.4

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

'Stairway Door
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

'Door - Room 01
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 110, (Floor * 25) + 25, -71.3

'Door - Room 03
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 45, (Floor * 25) + 25, -71.3

'Door - Room 05
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -20, (Floor * 25) + 25, -71.3

'Door - Room 07
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -45, (Floor * 25) + 25, -71.3

'Door - Room 09
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -110, (Floor * 25) + 25, -71.3

'Door - Room 02
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 108, (Floor * 25) + 25, -46.25

'Door - Room 08
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -108, (Floor * 25) + 25, -46.25

'Door - Room 04
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 87, (Floor * 25) + 25, -46.25

'Door - Room 06
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -87, (Floor * 25) + 25, -46.25

'Service Door (shaft)
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door - Room 10
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 110, (Floor * 25) + 25, 71.3

'Door - Room 12
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 45, (Floor * 25) + 25, 71.3

'Door - Room 14
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 71.3

'Door - Room 16
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -45, (Floor * 25) + 25, 71.3

'Door - Room 18
i = 120 + (150 * (Floor - 1))
Call Init_Objects(Floor, 120)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -110, (Floor * 25) + 25, 71.3

'Door - Room 11
i = 121 + (150 * (Floor - 1))
Call Init_Objects(Floor, 121)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 108, (Floor * 25) + 25, 46.25

'Door - Room 17
i = 122 + (150 * (Floor - 1))
Call Init_Objects(Floor, 122)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -108, (Floor * 25) + 25, 46.25

'Door - Room 13
i = 123 + (150 * (Floor - 1))
Call Init_Objects(Floor, 123)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 87, (Floor * 25) + 25, 46.25

'Door - Room 15
i = 124 + (150 * (Floor - 1))
Call Init_Objects(Floor, 124)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -87, (Floor * 25) + 25, 46.25

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

'Stairway Door
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

'Door 01
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, -71.3

'Door 03
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, -71.3

'Door 05
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -20, (Floor * 25) + 25, -71.3

'Door 07
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, -71.3

'Door 09
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, -71.3

'Door 02
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, -46.25

'Door 04
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 72, (Floor * 25) + 25, -46.25

'Service Room
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 40, (Floor * 25) + 25, -46.25

'Door 06
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -72, (Floor * 25) + 25, -46.25

'Door 08
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, -46.25

'Door 10a
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 55.83

'Door 12a
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, 55.83

'Door 14a
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 55.83

'Door 16a
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, 55.83

'Door 18a
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 55.83

'Door 11
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 30.83

'Door 13
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 72, (Floor * 25) + 25, 30.83

'Door 15
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -72, (Floor * 25) + 25, 30.83

'Door 17
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 30.83

'Door 10b
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 90, (Floor * 25) + 25, 102

'Door 12b
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 37, (Floor * 25) + 25, 102

'Door 14b
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 20, (Floor * 25) + 25, 102

'Door 16b
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -37, (Floor * 25) + 25, 102

'Door 18b
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -90, (Floor * 25) + 25, 102



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

'Stairway Door
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor >= 80 And Floor <= 115 Then

'Door 06
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -102

'Door 07
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -102

'Door 05
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -102

'Door 08
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -102

'Door 02
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -118

'Door 03
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -118

'Door 01
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -118

'Door 04
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -118

'Door 09
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, -60

'Door 11
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, -60

'Service Door
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door 13
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 27

'Door 16
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 27

'Door 18
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 66

'Door 19
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 66

'Door 17
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 66

'Door 20
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 66

'Door 22
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 102

'Door 23
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 102

'Door 21
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 102

'Door 24
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 102

'Door 26
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 116

'Door 27
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 116

'Door 25
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, (Floor * 25) + 25, 116

'Door 28
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, (Floor * 25) + 25, 116

'Door 12
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61, (Floor * 25) + 25, -46.25

'Door 10
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61, (Floor * 25) + 25, -46.25

'Door 15
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition -61, (Floor * 25) + 25, 15.41

'Door 14
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorC " + Str$(i))
Objects(i).SetPosition 61, (Floor * 25) + 25, 15.41


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

'Stairway Door
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor >= 118 And Floor <= 132 Then

'Door 01
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -117

'Door 02
i = 81 + (150 * (Floor - 1))
Call Init_Objects(Floor, 81)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -117

'Door 03
i = 82 + (150 * (Floor - 1))
Call Init_Objects(Floor, 82)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, -101

'Door 04
i = 83 + (150 * (Floor - 1))
Call Init_Objects(Floor, 83)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, -101

'Door 05
i = 84 + (150 * (Floor - 1))
Call Init_Objects(Floor, 84)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 50, (Floor * 25) + 25, -60

'Door 06
i = 85 + (150 * (Floor - 1))
Call Init_Objects(Floor, 85)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -50, (Floor * 25) + 25, -60

'Service Door
i = 86 + (150 * (Floor - 1))
Call Init_Objects(Floor, 86)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetPosition 39, (Floor * 25) + 25, -46.25

'Door 07
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 50, (Floor * 25) + 25, 13

'Door 08
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -50, (Floor * 25) + 25, 13

'Door 09
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 54

'Door 10
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 54

'Door 11
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 67

'Door 12
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 67

'Door 13
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 115

'Door 14
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 115

'Door 15
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, (Floor * 25) + 25, 126

'Door 16
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, (Floor * 25) + 25, 126


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

'Stairway Door
i = 75 + (150 * (Floor - 1))
Call Init_Objects(Floor, 75)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

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

'Stairway Door
i = 75 + (150 * (Floor - 1))
Call Init_Objects(Floor, 75)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

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

'Stairway Door
i = 2 + (150 * (Floor - 1))
Call Init_Objects(Floor, 2)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

If Floor = 138 Then

'Stairway Door
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.8, (Floor * 25) + 25, -36.4

End If

End Sub
Sub Init_Objects(Floor As Integer, ObjectIndex As Integer)
'currently, 150 objects per floor (150*138)
i = ObjectIndex + (150 * (Floor - 1))
Set Objects(i) = New TVMesh
Set Objects(i) = Scene.CreateMeshBuilder("Objects " + Str$(i))
'MsgBox (Str$(i))
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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
        
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -30 + 0.1, -12.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, 0 - 0.1, -32.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, 0 - 0.1, -32.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -30 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -30 + 0.1, 12.5 + 0.1, 0 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, 0 - 0.1, 32.5 - 0.1, 0 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, 0 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 19.5, (i * 25) + 25, (16 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 5.5, 19.5 + (i * 25) + 25, (46.25 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, 0, 19.5, (i * 25) + 25, (16 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 0, 5.5, 19.5 + (i * 25) + 25, (46.25 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, 0, -32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, 0, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 0, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, 0, 32.5, 0, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, 0, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 75, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
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
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, -12.5, 19.5, (i * 25) + 25, (3.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -12.5, 5.5, 19.5 + (i * 25) + 25, (34 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -12.5, -32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -12.5, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -12.5, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -12.5, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -12.5, 32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -12.5, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

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
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 75, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 75, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 75, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 75, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, (i * 25) + 25, 0, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -30.5 + 0.1, -12.5 - 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30.5 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 3, 2
    
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -46.25 + 0.1, 12.5 + 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -46.25 + 0.1, 12.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), 12.5 + 0.1, -15.42 - 0.1, 32.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), 32.5 - 0.1, -15.42 - 0.1, 32.5 - 0.1, -46.25 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -16, -12.5, -12.5, 19.5, (i * 25) + 25, (4.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -12.5, 5.5, 19.5 + (i * 25) + 25, (34 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("Wall1"), -12.5, -12.5, -32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), -32.5, -12.5, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -46.25, 12.5, -46.25, 25, (i * 25) + 25, 2, 2
    
    'left cut side wall start
    'ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, 46.25, 25, 0, 8, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -12.5, 19.5, (i * 25) + 25, 8, (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -46.25, 12.5, -12.5, 7.5, 19.5 + (i * 25) + 25, 8, (7.5 * 0.08)
    'left cut side wall end
    
    ShaftsFloor(i).AddWall GetTex("Wall1"), 12.5, -12.5, 32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("Wall1"), 32.5, -12.5, 32.5, -46.25, 25, (i * 25) + 25, 8, 2

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
    
    External.AddWall GetTex("Concrete"), -60, -150, 60, -150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, -150, 60, 150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, 150, -60, 150, 25, (i * 25) + 25, 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), -60, 150, -60, -150, 25, (i * 25) + 25, 7 * 4, 1 * 4
    
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, -150 + 0.1, 60 - 0.1, -150 + 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, -150 + 0.1, 60 - 0.1, 150 - 0.1, 25, (i * 25) + 25, 7, 1
    Room(i).AddWall GetTex("BrickTexture"), 60 - 0.1, 150 - 0.1, -60 + 0.1, 150 - 0.1, 25, (i * 25) + 25, 3, 1
    Room(i).AddWall GetTex("BrickTexture"), -60 + 0.1, 150 - 0.1, -60 + 0.1, -150 + 0.1, 25, (i * 25) + 25, 7, 1
    
    'Fake Stairway Doors for other floors
    Room(i).AddWall GetTex("StairsDoor"), -12.8, -36.4 - 3.9, -12.8, -36.4 + 4.3, 19.5, (i * 25) + 25 + 25, -1, 1
    
    'Stairwell Walls
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 50, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 50, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 50, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 50, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 50, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 50, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -30.5 + 0.1, -12.5 - 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    'Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30 + 0.1, -12.5 - 0.1, -30 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30.5 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, -12.5, 19.5, (i * 25) + 25, (4.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -12.5, 5.5, 19.5 + (i * 25) + 25, (34 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -12.5, -32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -12.5, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
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
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -46.25 + 0.1, -12.5 - 0.5, -40.3, 25, (i * 25) + 25, ((46.25 - 40.3) * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -32.5, -12.5 - 0.5, -30, 25, (i * 25) + 25, (2.5 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -32.5 + 0.1, -46.25 + 0.1, -32.5 + 0.1, -30 - 1, 25, (i * 25) + 25, (16.25 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -46.25 + 0.1, -32.5, -46.25 + 0.1, 25, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5, -30 - 1, -32.5, -30 - 1, 25, (i * 25) + 25, (20 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 6, -46.25 + 7.71, -12.5 - 16, -46.25 + 7.71, 25, (i * 25) + 25, (10 * 0.086) * 4, 3 * 4
    Stairs(i).AddWall GetTex("Concrete"), -12.5 - 0.5, -40.3, -12.5 - 0.5, -30, 5.5, 19.5 + (i * 25) + 25, (10.3 * 0.086) * 4, 0.2 * 4
    
    'Elevators Internal
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -30.5 + 0.1, -12.5 - 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 1, 2
    'Shafts.AddWall GetTex("Wall1"), -12.5, -46.25, -12.5, -15.42, 25, (I * 25) + 25, 3, 2
    'cut side wall start
    'Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30.5 + 0.1, -12.5 - 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -16 - 0.1, -12.5 - 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 3, 2
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -30.5 + 0.1, -12.5 - 0.1, -16 - 0.1, 5.5, (i * 25) + 25 + 19.5, 0.8, 0.44
    'cut side wall end
    Shafts.AddWall GetTex("BrickTexture"), -12.5 - 0.1, -15.42 - 0.1, -32.5 + 0.1, -15.42 - 0.1, 25, (i * 25) + 25, 1, 2
    Shafts.AddWall GetTex("BrickTexture"), -32.5 + 0.1, -15.42 - 0.1, -32.5 + 0.1, -30.5 + 0.1, 25, (i * 25) + 25, 3, 2
    
    'Elevators External
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -46.25, -12.5, -46.25, 25, (i * 25) + 25, 2, 2
    'Shafts.AddWall GetTex("Wall2"), -12.5, -46.25, -12.5, 46.25, 25, 0, 8, 2
    'right cut side wall start
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -40.3, 19.5, (i * 25) + 25, (6 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -16, -12.5, -12.5, 19.5, (i * 25) + 25, (4.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -32.5, -12.5, -30, 19.5, (i * 25) + 25, (2.5 * 0.086), (19.5 * 0.08)
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -46.25, -12.5, -12.5, 5.5, 19.5 + (i * 25) + 25, (34 * 0.086), (7.5 * 0.08)
    'right cut side wall end
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -12.5, -12.5, -32.5, -12.5, 25, (i * 25) + 25, 2, 2
    ShaftsFloor(i).AddWall GetTex("BrickTexture"), -32.5, -12.5, -32.5, -46.25, 25, (i * 25) + 25, 8, 2
    
End Sub

Function GetVar(Location As Integer, Index As Integer) As Integer
'This function loads lines from a data file, and returns the desired value
Dim TempString As String
Dim CurrentIndex As Integer
Dim StringStart As Integer
Dim StringEnd As Integer
Dim GetVarTemp As String
CurrentIndex = 0
Open FileName For Random As #1
Get #1, Location, TempString
Close #1
For i = 1 To Len(TempString)
If Mid$(TempString, i, 1) = "," Then CurrentIndex = CurrentIndex + 1
If CurrentIndex = Index - 1 Then GoTo GetVar2
Next i
GetVar2:

If CurrentIndex > 0 Then StringStart = i + 1 Else StringStart = i
    For j = StringStart To Len(TempString) + 1
    If Mid$(TempString, j, 1) = "," Or j = Len(TempString) Then StringEnd = j + 1
    Next j
GetVarTemp = Mid$(TempString, StringStart, StringEnd - StringStart)
GetVar = Val(GetVarTemp)

End Function

Sub DrawElevatorButtons()
'Elevator Buttons
    
    'This sub should be split up into sections once more elevators are made
    'that is so certain elevators go to certain floors
    'each panel can cover 60 floors
    
    
    '------------------------------------------
    'Load textures
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\135.jpg", "Button1"
    'TextureFactory.LoadTexture App.Path + "\data\floorindicators\M.jpg", "Button2"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\136.jpg", "Button3"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\137.jpg", "Button4"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\R.jpg", "Button5"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\130.jpg", "Button6"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\131.jpg", "Button7"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\132.jpg", "Button8"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\133.jpg", "Button9"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\134.jpg", "Button10"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\115.jpg", "Button11"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\116.jpg", "Button12"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\117.jpg", "Button13"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\118.jpg", "Button14"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\129.jpg", "Button15"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\110.jpg", "Button16"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\111.jpg", "Button17"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\112.jpg", "Button18"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\113.jpg", "Button19"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\114.jpg", "Button20"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\105.jpg", "Button21"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\106.jpg", "Button22"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\107.jpg", "Button23"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\108.jpg", "Button24"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\109.jpg", "Button25"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\100.jpg", "Button26"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\101.jpg", "Button27"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\102.jpg", "Button28"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\103.jpg", "Button29"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\104.jpg", "Button30"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\95.jpg", "Button31"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\96.jpg", "Button32"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\97.jpg", "Button33"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\98.jpg", "Button34"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\99.jpg", "Button35"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\90.jpg", "Button36"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\91.jpg", "Button37"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\92.jpg", "Button38"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\93.jpg", "Button39"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\94.jpg", "Button40"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\85.jpg", "Button41"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\86.jpg", "Button42"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\87.jpg", "Button43"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\88.jpg", "Button44"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\89.jpg", "Button45"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\80.jpg", "Button46"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\81.jpg", "Button47"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\82.jpg", "Button48"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\83.jpg", "Button49"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\84.jpg", "Button50"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\2.jpg", "Button51"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\39.jpg", "Button52"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\40.jpg", "Button53"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\65.jpg", "Button54"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\79.jpg", "Button55"
    
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "Button56"
    'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "Button57"
    'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "Button58"
    'TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "Button59"
    TextureFactory.LoadTexture App.Path + "\data\floorindicators\M.jpg", "Button60"
        
    '-------------------------------
        
    Buttons(135).AddWall GetTex("Button1"), -12.67, -27.8, -12.67, -27.5, 0.3, 12, -1, 1
    'Buttons(1).AddWall GetTex("Button2"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12, -1, 1
    Buttons(136).AddWall GetTex("Button3"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12, -1, 1
    Buttons(137).AddWall GetTex("Button4"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12, -1, 1
    Buttons(138).AddWall GetTex("Button5"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12, -1, 1
    
    Buttons(130).AddWall GetTex("Button6"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 0.5, -1, 1
    Buttons(131).AddWall GetTex("Button7"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 0.5, -1, 1
    Buttons(132).AddWall GetTex("Button8"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 0.5, -1, 1
    Buttons(133).AddWall GetTex("Button9"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 0.5, -1, 1
    Buttons(134).AddWall GetTex("Button10"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 0.5, -1, 1
    
    Buttons(115).AddWall GetTex("Button11"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 1, -1, 1
    Buttons(116).AddWall GetTex("Button12"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 1, -1, 1
    Buttons(117).AddWall GetTex("Button13"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 1, -1, 1
    Buttons(118).AddWall GetTex("Button14"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 1, -1, 1
    Buttons(129).AddWall GetTex("Button15"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 1, -1, 1
    
    Buttons(110).AddWall GetTex("Button16"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 1.5, -1, 1
    Buttons(111).AddWall GetTex("Button17"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 1.5, -1, 1
    Buttons(112).AddWall GetTex("Button18"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 1.5, -1, 1
    Buttons(113).AddWall GetTex("Button19"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 1.5, -1, 1
    Buttons(114).AddWall GetTex("Button20"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 1.5, -1, 1
    
    Buttons(105).AddWall GetTex("Button21"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 2, -1, 1
    Buttons(106).AddWall GetTex("Button22"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 2, -1, 1
    Buttons(107).AddWall GetTex("Button23"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 2, -1, 1
    Buttons(108).AddWall GetTex("Button24"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 2, -1, 1
    Buttons(109).AddWall GetTex("Button25"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 2, -1, 1
    
    Buttons(100).AddWall GetTex("Button26"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 2.5, -1, 1
    Buttons(101).AddWall GetTex("Button27"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 2.5, -1, 1
    Buttons(102).AddWall GetTex("Button28"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 2.5, -1, 1
    Buttons(103).AddWall GetTex("Button29"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 2.5, -1, 1
    Buttons(104).AddWall GetTex("Button30"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 2.5, -1, 1
    
    Buttons(95).AddWall GetTex("Button31"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 3, -1, 1
    Buttons(96).AddWall GetTex("Button32"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 3, -1, 1
    Buttons(97).AddWall GetTex("Button33"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 3, -1, 1
    Buttons(98).AddWall GetTex("Button34"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 3, -1, 1
    Buttons(99).AddWall GetTex("Button35"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 3, -1, 1
    
    Buttons(90).AddWall GetTex("Button36"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 3.5, -1, 1
    Buttons(91).AddWall GetTex("Button37"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 3.5, -1, 1
    Buttons(92).AddWall GetTex("Button38"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 3.5, -1, 1
    Buttons(93).AddWall GetTex("Button39"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 3.5, -1, 1
    Buttons(94).AddWall GetTex("Button40"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 3.5, -1, 1
    
    Buttons(85).AddWall GetTex("Button41"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 4, -1, 1
    Buttons(86).AddWall GetTex("Button42"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 4, -1, 1
    Buttons(87).AddWall GetTex("Button43"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 4, -1, 1
    Buttons(88).AddWall GetTex("Button44"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 4, -1, 1
    Buttons(89).AddWall GetTex("Button45"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 4, -1, 1
    
    Buttons(80).AddWall GetTex("Button46"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 4.5, -1, 1
    Buttons(81).AddWall GetTex("Button47"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 4.5, -1, 1
    Buttons(82).AddWall GetTex("Button48"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 4.5, -1, 1
    Buttons(83).AddWall GetTex("Button49"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 4.5, -1, 1
    Buttons(84).AddWall GetTex("Button50"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 4.5, -1, 1
    
    Buttons(2).AddWall GetTex("Button51"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 5, -1, 1
    Buttons(39).AddWall GetTex("Button52"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 5, -1, 1
    Buttons(40).AddWall GetTex("Button53"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 5, -1, 1
    Buttons(65).AddWall GetTex("Button54"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 5, -1, 1
    Buttons(79).AddWall GetTex("Button55"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 5, -1, 1
    
    Buttons(-1).AddWall GetTex("Button56"), -12.67, -27.8, -12.67, -27.5, 0.3, 12 - 5.5, -1, 1
    'Buttons(1).AddWall GetTex("Button57"), -12.67, -27.8 - 0.4, -12.67, -27.5 - 0.4, 0.3, 12 - 5.5, -1, 1
    'Buttons(1).AddWall GetTex("Button58"), -12.67, -27.8 - 0.8, -12.67, -27.5 - 0.8, 0.3, 12 - 5.5, -1, 1
    'Buttons(1).AddWall GetTex("Button59"), -12.67, -27.8 - 1.2, -12.67, -27.5 - 1.2, 0.3, 12 - 5.5, -1, 1
    Buttons(0).AddWall GetTex("Button60"), -12.67, -27.8 - 1.6, -12.67, -27.5 - 1.6, 0.3, 12 - 5.5, -1, 1
    
   
End Sub

Sub CheckElevatorButtons()

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons(-1).GetMeshName Then
    Buttons(-1).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync = True
    OpenElevator = -1
    GotoFloor = -1
End If

If CollisionResult.GetCollisionMesh.GetMeshName = Buttons(0).GetMeshName Then
    Buttons(0).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync = True
    OpenElevator = -1
    GotoFloor = 0.1
End If

For i = 2 To 138
If CollisionResult.GetCollisionMesh.GetMeshName = Buttons(i).GetMeshName Then
    Buttons(i).SetColor RGBA(1, 1, 0, 1)
    ElevatorSync = True
    OpenElevator = -1
    GotoFloor = i
End If
Next i
End Sub

Sub OpenDoor()
OpeningDoor = OpeningDoor + 1

'DoorRotated values:
'0 is horizontal, opens downward
'1 is vertical, opens to the left
'2 is horizontal, opens upward
'3 is vertical, opens to the right
On Error Resume Next

Objects(DoorNumber).RotateY (OpeningDoor / 110)
If DoorRotated = 0 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (OpeningDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (OpeningDoor / 38)
If DoorRotated = 1 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (OpeningDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (OpeningDoor / 48)
If DoorRotated = 2 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (OpeningDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (OpeningDoor / 38)
If DoorRotated = 3 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (OpeningDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (OpeningDoor / 48)

If OpeningDoor = 17 Then
    OpeningDoor = 0
    If DoorRotated = 0 Then Objects(DoorNumber).SetMeshName ("DoorAO " + Str$(DoorNumber))
    If DoorRotated = 1 Then Objects(DoorNumber).SetMeshName ("DoorBO " + Str$(DoorNumber))
    If DoorRotated = 2 Then Objects(DoorNumber).SetMeshName ("DoorCO " + Str$(DoorNumber))
    If DoorRotated = 3 Then Objects(DoorNumber).SetMeshName ("DoorDO " + Str$(DoorNumber))
End If
End Sub

Sub CloseDoor()
ClosingDoor = ClosingDoor - 1
On Error Resume Next
Objects(DoorNumber).RotateY -(ClosingDoor / 110)
If DoorRotated = 0 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (ClosingDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (ClosingDoor / 38)
If DoorRotated = 1 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X - (ClosingDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (ClosingDoor / 48)
If DoorRotated = 2 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (ClosingDoor / 48), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z + (ClosingDoor / 38)
If DoorRotated = 3 Then Objects(DoorNumber).SetPosition Objects(DoorNumber).GetPosition.X + (ClosingDoor / 38), Objects(DoorNumber).GetPosition.Y, Objects(DoorNumber).GetPosition.z - (ClosingDoor / 48)

If ClosingDoor = 0 And DoorRotated = 0 Then Objects(DoorNumber).SetMeshName ("DoorA " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 1 Then Objects(DoorNumber).SetMeshName ("DoorB " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 2 Then Objects(DoorNumber).SetMeshName ("DoorC " + Str$(DoorNumber))
If ClosingDoor = 0 And DoorRotated = 3 Then Objects(DoorNumber).SetMeshName ("DoorD " + Str$(DoorNumber))

End Sub

Sub SlowToFPS(ByVal FrameRate As Long)
Dim lngTicksPerFrame As Long
Static lngOldTickCount As Long
lngTicksPerFrame = 1000 / FrameRate
While GetTickCount() < lngOldTickCount
Sleep 5
Wend
lngOldTickCount = GetTickCount() + lngTicksPerFrame
End Sub
