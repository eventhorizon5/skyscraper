Attribute VB_Name = "Triton_Center"
'Skyscraper 1.1 Alpha
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
'
Option Explicit

Sub ProcessGlobals()
CameraAltitude = 10
ElevatorShafts = 4

End Sub
Sub Process118to129()
Dim FloorHeight As Single
Dim FloorAltitude As Single
    
    'Floors 118 to 129 (minus 10 feet)
    For i = 118 To 129
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 118 to 129... " + Str$(Int(((i - 118) / (129 - 118)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 46.25, 85, 150, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 46.25, FloorAltitude, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 85, -46.25, 32.5, 46.25, FloorAltitude, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 46.25, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 46.25, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 85, -46.25, 32.5, 46.25, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 46.25, 85, 150, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 46.25, (FloorAltitude + 25), ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 46.25, (FloorAltitude + 25), ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 46.25, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 1, False, True, False, False, False, True, True, True, True, True, True)
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -60, -71.3, -12.5, -71.3, 25, FloorAltitude, ((60 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 60, -71.3, 25, FloorAltitude, ((60 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -60, 46.25, -32.5, 46.25, 25, FloorAltitude, ((60 - 32.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 32.5, 46.25, 60, 46.25, 25, FloorAltitude, ((60 - 32.5) * 0.086), 1
    
    'Room(i).AddWall GetTex("Wall3"), -60, 0, -32.5, 0, 25, FloorAltitude, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 60, 0, 32.5, 0, 25, FloorAltitude, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -60, -46.3, -32.5, -46.3, 25, FloorAltitude, ((60 - 32.5) * 0.086), 1
    'This wall is cut for the service door
    Room(i).AddWall GetTex("Wall3"), 60, -46.3, 32.5, -46.3, 5.5, FloorAltitude + 19.5, ((60 - 32.5) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, -46.3, 39 + 3.9, -46.3, 19.5, FloorAltitude, ((60 - 39 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 39 - 3.9, -46.3, 32.5, -46.3, 19.5, FloorAltitude, ((39 - 32.5 - 3.9) * 0.086), 19.5 * 4
        
    'Top Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -60, -71.3, -60, -46.3, 5.5, FloorAltitude + 19.5, ((71.3 - 46.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), -60, -71.3, -60, -60 - 3.9, 19.5, FloorAltitude, ((71.3 - 60 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -60, -60 + 3.9, -60, -46.3, 19.5, FloorAltitude, ((60 - 46.3 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, -71.3, 60, -46.3, 5.5, FloorAltitude + 19.5, ((71.3 - 46.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, -71.3, 60, -60 - 3.9, 19.5, FloorAltitude, ((71.3 - 60 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, -60 + 3.9, 60, -46.3, 19.5, FloorAltitude, ((60 - 46.3 - 3.9) * 0.086), 19.5 * 4
    
    'Bottom Horizontal Hallway End Walls
    Room(i).AddWall GetTex("Wall3"), -60, 71.3, -60, 46.3, 5.5, FloorAltitude + 19.5, ((71.3 - 46.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), -60, 71.3, -60, 60 + 3.9, 19.5, FloorAltitude, ((71.3 - 60 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -60, 60 - 3.9, -60, 46.3, 19.5, FloorAltitude, ((60 - 46.3 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, 71.3, 60, 46.3, 5.5, FloorAltitude + 19.5, ((71.3 - 46.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, 71.3, 60, 60 + 3.9, 19.5, FloorAltitude, ((71.3 - 60 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 60, 60 - 3.9, 60, 46.3, 19.5, FloorAltitude, ((60 - 46.3 - 3.9) * 0.086), 19.5 * 4
    
    'Bottom Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 117 + 3.9, 19.5, FloorAltitude, ((150 - 117 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, 117 - 3.9, -12.5, 101 + 3.9, 19.5, FloorAltitude, ((117 - 101 - 3.9 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, 101 - 3.9, -12.5, 71.3, 19.5, FloorAltitude, ((101 - 71.3 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 117 + 3.9, 19.5, FloorAltitude, ((150 - 117 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, 117 - 3.9, 12.5, 101 + 3.9, 19.5, FloorAltitude, ((117 - 101 - 3.9 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, 101 - 3.9, 12.5, 71.3, 19.5, FloorAltitude, ((101 - 71.3 - 3.9) * 0.086), 19.5 * 4
    
    'Top Hallway
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -117 - 3.9, 19.5, FloorAltitude, ((150 - 117 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, -117 + 3.9, -12.5, -101 - 3.9, 19.5, FloorAltitude, ((117 - 101 - 3.9 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), -12.5, -101 + 3.9, -12.5, -71.3, 19.5, FloorAltitude, ((101 - 71.3 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), 5.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -117 - 3.9, 19.5, FloorAltitude, ((150 - 117 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, -117 + 3.9, 12.5, -101 - 3.9, 19.5, FloorAltitude, ((117 - 101 - 3.9 - 3.9) * 0.086), 19.5 * 4
    Room(i).AddWall GetTex("Wall3"), 12.5, -101 + 3.9, 12.5, -71.3, 19.5, FloorAltitude, ((101 - 71.3 - 3.9) * 0.086), 19.5 * 4
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -85, 110, -12.5, 110, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 85, 110, 12.5, 110, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -85, 71.3, -12.5, 71.3, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 85, 71.3, 12.5, 71.3, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -85, -110, -12.5, -110, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 85, -110, 12.5, -110, 25, FloorAltitude, ((85 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -85, -71.3, -60, -71.3, 25, FloorAltitude, ((85 - 60) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 85, -71.3, 60, -71.3, 25, FloorAltitude, ((85 - 60) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -85, 0, -32.5, 0, 25, FloorAltitude, ((85 - 32.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 85, 0, 32.5, 0, 25, FloorAltitude, ((85 - 32.5) * 0.086), 1
    
    'service room
    Room(i).AddWall GetTex("Wall3"), 50, -46.25, 50, -20, 25, FloorAltitude, ((46.25 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 50, -20, 32.5, -20, 25, FloorAltitude, ((50 - 32.5) * 0.086), 1
    
    Next i
    
End Sub

Sub Process2to39()
Dim FloorHeight As Single
Dim FloorAltitude As Single
    
    'Floors 2 to 39
    For i = 2 To 39
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 2 to 39... " + Str$(Int((i / 39) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -160, -150, 160, -46.25, FloorAltitude, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -160, 46.25, 160, 150, FloorAltitude, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -90.5, -46.25, -52.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 52.5, -46.25, 90.5, 46.25, FloorAltitude, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -52.5, 0, 52.5, 46.25, FloorAltitude, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -160, -46.25, -130.5, 46.25, FloorAltitude, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 160, -46.25, 130.5, 46.25, FloorAltitude, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -160, -150, 160, -46.25, (FloorAltitude + 25) - 0.5, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -160, 46.25, 160, 150, (FloorAltitude + 25) - 0.5, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -160, -46.25, -130.5, 46.25, (FloorAltitude + 25) - 0.5, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 160, -46.25, 130.5, 46.25, (FloorAltitude + 25) - 0.5, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, (FloorAltitude + 25), ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, (FloorAltitude + 25), ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25), ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25), ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, (FloorAltitude + 25), ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, (FloorAltitude + 25), ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, (FloorAltitude + FloorHeight) - 0.1, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 320, 300, 0, 0, Int(i), True, 1, 1)
    
    If i = 2 Or i = 39 Then
    Call DrawElevatorWalls(Int(i), 5, 1, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 4, True, True, True, True, True, True, True, True, True, True, True)
    End If
    If i <> 2 And i <> 39 Then
    Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 4, True, True, True, True, True, True, True, True, True, True, True)
    End If
    
    'Room(I) Walls
    'top walls
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, -90.5, -71.3, 19.5, FloorAltitude, ((160 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 19.5, FloorAltitude, ((70 + 70) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 160, -71.3, 90.5, -71.3, 19.5, FloorAltitude, ((160 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -160, -71.3, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((160 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 5.5, FloorAltitude + 19.5, ((70 + 70) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 160, -71.3, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((160 - 90.5) * 0.086), (5.5 * 0.08)
    
    'bottom walls
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, -90.5, 71.3, 19.5, FloorAltitude, ((160 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 19.5, FloorAltitude, ((70 + 70) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 160, 71.3, 90.5, 71.3, 19.5, FloorAltitude, ((160 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -160, 71.3, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((160 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 5.5, FloorAltitude + 19.5, ((70 + 70) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 160, 71.3, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((160 - 90.5) * 0.086), (5.5 * 0.08)
    
    'top middle walls
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -61.25 - 3.9, -46.25, 19.5, FloorAltitude, ((70 - 61.25 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -61.25 + 3.9, -46.25, -52.5, -46.25, 19.5, FloorAltitude, ((61.25 - 3.9 - 52.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 61.25 + 3.9, -46.25, 19.5, FloorAltitude, ((70 - 61.25 + 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 61.25 - 3.9, -46.25, 52.5, -46.25, 19.5, FloorAltitude, ((61.25 - 3.9 - 52.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -52.5, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 52.5, -46.25, 70, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    
    'service rooms
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 70, 46.25, 25, FloorAltitude, ((46.25 + 46.25) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 70, -15, 52.5, -15, 25, FloorAltitude, ((70 - 52.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -70, 46.25, 25, FloorAltitude, ((46.25 + 46.25) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -15, -52.5, -15, 25, FloorAltitude, ((70 - 52.5) * 0.086), 1
    
    'left hallway
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -130 + 3.9, 70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -90 + 3.9, 70, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -90 + 3.9, 90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 130 - 3.9, 70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 90 - 3.9, 70, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 90 - 3.9, 90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'right hallway
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -130 + 3.9, -70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -90 + 3.9, -70, -71.3, 19.5, FloorAltitude, ((90 - 70 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -90 + 3.9, -90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 130 - 3.9, -70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 90 - 3.9, -70, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 90 - 3.9, -90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'middle hallway extension
    Room(i).AddWall GetTex("Wall3"), -12.5, 0, -12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 0, 12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), (19.5 * 0.08)
    
    'bottom middle walls
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -22.5 - 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -22.5 + 3.9, 46.25, -12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 12.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 46.25, 22.5 + 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 22.5 - 3.9, 46.25, 12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 12.5 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -12.5, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 46.25, 70, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    
    'Rooms
    Room(i).AddWall GetTex("Wall3"), -160, -110, -90.5, -110, 25, FloorAltitude, ((160 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -160, 110, -90.5, 110, 25, FloorAltitude, ((160 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -110, 70, -110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, 110, 70, 110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 160, -110, 90.5, -110, 25, FloorAltitude, ((160 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 160, 110, 90.5, 110, 25, FloorAltitude, ((160 - 90.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 0, -71.3, 0, -150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 0, 71.3, 0, 150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    Next i

End Sub

Sub Process41to79()
Dim FloorHeight As Single
Dim FloorAltitude As Single
    
    'Floors 41 to 79 (minus 14 feet on both sides where 20=8 feet)
    For i = 41 To 79
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 41 to 79... " + Str$(Int(((i - 40) / (79 - 40)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -135, -150, 135, -46.25, FloorAltitude, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -135, 46.25, 135, 150, FloorAltitude, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -90.5, -46.25, -52.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 52.5, -46.25, 90.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -52.5, 0, 52.5, 46.25, FloorAltitude, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -135, -46.25, -110.5, 46.25, FloorAltitude, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 135, -46.25, 110.5, 46.25, FloorAltitude, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -135, -150, 135, -46.25, (FloorAltitude + 25) - 0.5, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -135, 46.25, 135, 150, (FloorAltitude + 25) - 0.5, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -135, -46.25, -110.5, 46.25, (FloorAltitude + 25) - 0.5, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 135, -46.25, 110.5, 46.25, (FloorAltitude + 25) - 0.5, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -150, 135, -46.25, (FloorAltitude + 25), ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, 46.25, 135, 150, (FloorAltitude + 25), ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25), ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25), ((52.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -46.25, -110.5, 46.25, (FloorAltitude + 25), ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 135, -46.25, 110.5, 46.25, (FloorAltitude + 25), ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -150, 135, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, 46.25, 135, 150, (FloorAltitude + FloorHeight) - 0.1, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -46.25, -110.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 135, -46.25, 110.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 270, 300, 0, 0, Int(i), True, 1, 1)
    
    If i = 79 Then
    Call DrawElevatorWalls(Int(i), 5, 1, True, True, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, False, True, True, True, True, True, True, True, True, True, True)
    End If
    If i <> 79 Then
    Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, False, True, True, True, True, True, True, True, True, True, True)
    End If
    If i <= 51 Then
    Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    End If
    If i > 51 Then
    Call DrawElevatorWalls(Int(i), 5, 2, True, False, True, False, False, False, False, False, False, False, False)
    End If
    
    'Room(I) Walls
    
    'top walls
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90.5, -71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 19.5, FloorAltitude, ((70 * 2) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, -71.3, 90.5, -71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 5.5, FloorAltitude + 19.5, ((70 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, -71.3, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    
    'bottom walls
    Room(i).AddWall GetTex("Wall3"), -135, 71.3, -90.5, 71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 19.5, FloorAltitude, ((70 * 2) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, 71.3, 90.5, 71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -135, 71.3, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 5.5, FloorAltitude + 19.5, ((70 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, 71.3, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -61.25 - 3.9, -46.25, 25, FloorAltitude, ((70 - 61.25 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -61.25 + 3.9, -46.25, -52.5, -46.25, 25, FloorAltitude, ((61.25 - 52.5 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 61.25 + 3.9, -46.25, 25, FloorAltitude, ((70 - 61.25 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 61.25 - 3.9, -46.25, 52.5, -46.25, 25, FloorAltitude, ((61.25 - 52.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -52.5, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 52.5, -46.25, 70, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    
    'service rooms
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 70, 46.25, 25, FloorAltitude, ((46.25 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 70, -15, 52.5, -15, 25, FloorAltitude, ((50 - 32.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -70, 46.25, 25, FloorAltitude, ((46.25 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -15, -52.5, -15, 25, FloorAltitude, ((70 - 52.5) * 0.086), 1
    
    'left hallway
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -130 + 3.9, 70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -90 + 3.9, 70, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -90 + 3.9, 90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 130 - 3.9, 70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 90 - 3.9, 70, 71.3, 19.5, FloorAltitude, ((90 - 70 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 90 - 3.9, 90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'right hallway
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -130 + 3.9, -70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -90 + 3.9, -70, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -90 + 3.9, -90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 130 - 3.9, -70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 90 - 3.9, -70, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 90 - 3.9, -90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'middle hallway extension
    Room(i).AddWall GetTex("Wall3"), -12.5, 0, -12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 0, 12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), 1
    
    'bottom middle walls
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -22.5 - 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -22.5 + 3.9, 46.25, -12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 3.9 - 12.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 46.25, 22.5 + 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 22.5 - 3.9, 46.25, 12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 3.9 - 12.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -12.5, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 46.25, 70, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    
    'Rooms
    Room(i).AddWall GetTex("Wall3"), -135, -110, -90.5, -110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -135, 110, -90.5, 110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -110, 70, -110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, 110, 70, 110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 135, -110, 90.5, -110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 135, 110, 90.5, 110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 0, -71.3, 0, -150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 0, 71.3, 0, 150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    Next i

End Sub

Sub Process81to114()
Dim FloorHeight As Single
Dim FloorAltitude As Single
    
    'Floors 81 to 114
    For i = 81 To 114
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 81 to 114... " + Str$(Int(((i - 81) / (114 - 81)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 110, -46.25, 52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, 46.25, 110, 150, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("Granite"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("Granite"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 220, 300, 0, 0, Int(i), True, 1, 1)
    
    If i >= 82 And i <= 98 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, True, True, True, True, True, True)
    End If
    If i >= 102 And i < 114 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, False, False, False, False, False, False, False, False, False)
    End If
    If i = 81 Or i = 99 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, True, True, True, True, True, True)
    End If
    If i = 100 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, False, False, False, False, False, False)
    End If
    If i = 114 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, False, False, False, False, False, False, False, False, False)
    End If
    If i = 101 Then
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, False, False, False, False, False, False)
    End If
    
    'Right Hallways, Right Wall
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, 150, 5.5, FloorAltitude + 19.5, ((150 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -118 - 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -118 + 3.9, -70, -102 - 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -102 + 3.9, -70, -60 - 3.9, 19.5, FloorAltitude, ((102 - 60 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -60 + 3.9, -70, 27 - 3.9, 19.5, FloorAltitude, ((60 - 27 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 27 + 3.9, -70, 66 - 3.9, 19.5, FloorAltitude, ((66 - 27 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 66 + 3.9, -70, 102 - 3.9, 19.5, FloorAltitude, ((102 - 66 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 102 + 3.9, -70, 116 - 3.9, 19.5, FloorAltitude, ((116 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 116 + 3.9, -70, 150, 19.5, FloorAltitude, ((150 - 116 - 3.9) * 0.086), (19.5 * 0.08)
    
    'Left Hallways, Left Wall
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, 150, 5.5, FloorAltitude + 19.5, ((150 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -118 - 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -118 + 3.9, 70, -102 - 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -102 + 3.9, 70, -60 - 3.9, 19.5, FloorAltitude, ((102 - 60 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -60 + 3.9, 70, 27 - 3.9, 19.5, FloorAltitude, ((60 - 27 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 27 + 3.9, 70, 66 - 3.9, 19.5, FloorAltitude, ((66 - 27 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 66 + 3.9, 70, 102 - 3.9, 19.5, FloorAltitude, ((102 - 66 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 102 + 3.9, 70, 116 - 3.9, 19.5, FloorAltitude, ((116 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 116 + 3.9, 70, 150, 19.5, FloorAltitude, ((150 - 116 - 3.9) * 0.086), (19.5 * 0.08)
    
    'center bottom hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, 150, -12.5, 118 + 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, 118 - 3.9, -12.5, 102 + 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, 102 - 3.9, -12.5, 71.3, 19.5, FloorAltitude, ((102 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    'center bottom hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 150, 12.5, 118 + 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 118 - 3.9, 12.5, 102 + 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 102 - 3.9, 12.5, 71.3, 19.5, FloorAltitude, ((102 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    'center top hallway, right wall
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, -150, -12.5, -118 - 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, -118 + 3.9, -12.5, -102 - 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -12.5, -102 + 3.9, -12.5, -71.3, 19.5, FloorAltitude, ((102 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    'center top hallway, left wall
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, -150, 12.5, -118 - 3.9, 19.5, FloorAltitude, ((150 - 118 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, -118 + 3.9, 12.5, -102 - 3.9, 19.5, FloorAltitude, ((118 - 102 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, -102 + 3.9, 12.5, -71.3, 19.5, FloorAltitude, ((102 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    'Individual Rooms
    Room(i).AddWall GetTex("Wall3"), -110, 110, -70, 110, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -50, 110, -12.5, 110, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, 110, 70, 110, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 50, 110, 12.5, 110, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 71.3, -70, 71.3, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -50, 71.3, -12.5, 71.3, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, 71.3, 70, 71.3, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 71.3, 50, 71.3, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -110, -70, -110, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -50, -110, -12.5, -110, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, -110, 70, -110, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 50, -110, 12.5, -110, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -71.3, -70, -71.3, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -12.5, -71.3, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, -71.3, 70, -71.3, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 12.5, -71.3, 50, -71.3, 25, FloorAltitude, ((50 - 12.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -110, 46.25, -70, 46.25, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, 46.25, 70, 46.25, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    
    'Room(i).AddWall GetTex("Wall3"), -110, 20, -70, 20, 25, FloorAltitude, 9, 1
    'Room(i).AddWall GetTex("Wall3"), 110, 20, 70, 20, 25, FloorAltitude, 2, 1
    
    Room(i).AddWall GetTex("Wall3"), -110, -5, -70, -5, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 110, -5, 70, -5, 25, FloorAltitude, ((110 - 70) * 0.086), 1
    
    'Left Bottom Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, 71.3, 50, 150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    'Right Bottom Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, 71.3, -50, 150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    'Left Top Hallway, Right Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), 50, -71.3, 50, -150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    'Right Top Hallway, Left Wall (no doors)
    Room(i).AddWall GetTex("Wall3"), -50, -71.3, -50, -150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    
    Next i

End Sub

Sub ProcessBasement()
Dim FloorHeight As Single
Dim FloorAltitude As Single

For i = -1 To BottomFloor Step -1
    
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    
    'Floor
    Room(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, FloorAltitude, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, FloorAltitude, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, FloorAltitude, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, FloorAltitude, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, FloorAltitude, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, FloorAltitude, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, FloorAltitude + 25 - 0.5, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, FloorAltitude + 25 - 0.5, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, FloorAltitude + 25 - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, FloorAltitude + 25 - 0.5, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude + 25 - 0.5, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, FloorAltitude + 25 - 0.5, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, FloorAltitude + 25 - 0.5, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, FloorAltitude + 25 - 0.5, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
        
    'Crawlspace bottom
    Room(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, FloorAltitude + 25, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, FloorAltitude + 25, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, FloorAltitude + 25, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, FloorAltitude + 25, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude + 25, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, FloorAltitude + 25, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, FloorAltitude + 25, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, FloorAltitude + 25, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace top
    Room(i).AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, FloorAltitude + FloorHeight - 0.1, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, FloorAltitude + FloorHeight - 0.1, ((160 + 160) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, FloorAltitude + FloorHeight - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, FloorAltitude + FloorHeight - 0.1, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude + FloorHeight - 0.1, ((12.5 + 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, FloorAltitude + FloorHeight - 0.1, ((52.5 + 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, FloorAltitude + FloorHeight - 0.1, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, FloorAltitude + FloorHeight - 0.1, ((160 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace walls
    Room(i).AddWall GetTex("BrickTexture"), -160, -150, 160, -150, (FloorHeight - 25), FloorAltitude + 25, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), 160, -150, 160, 150, (FloorHeight - 25), FloorAltitude + 25, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), 160, 150, -160, 150, (FloorHeight - 25), FloorAltitude + 25, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), -160, 150, -160, -150, (FloorHeight - 25), FloorAltitude + 25, ((160 + 160) * 0.086), 1

    'Level Walls
    Room(i).AddWall GetTex("BrickTexture"), -160, -150, 160, -150, FloorHeight, FloorAltitude, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), 160, -150, 160, 150, FloorHeight, FloorAltitude, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), 160, 150, -160, 150, FloorHeight, FloorAltitude, ((160 + 160) * 0.086), 1
    Room(i).AddWall GetTex("BrickTexture"), -160, 150, -160, -150, FloorHeight, FloorAltitude, ((160 + 160) * 0.086), 1
    
    Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 4, True, False, False, False, False, False, False, False, False, False, False)
    
    Next i
        
End Sub

Sub ProcessFloorHeights()
Dim height As Single
i = 0
For i = BottomFloor To TopFloor
DoEvents
height = 25 + CrawlSpaceHeight
If i = 1 Then height = height * 2
FloorHeightTable(i, 1) = height
Next i

End Sub

Sub ProcessFloorNames()
Dim FName As String
i = 0
For i = -10 To 138
DoEvents
If i < 0 Then FName = "B" + LTrim(Str$(Abs(i)))
If i = 0 Then FName = "L"
If i = 1 Then FName = "M"
If i > 1 And i < 138 Then FName = LTrim(Str$(i))
If i = 138 Then FName = "R"
Call SetFloorName(Int(i), FName)
Next i
End Sub


Sub ProcessLobby()
Dim FloorHeight As Single
Dim FloorAltitude As Single


DoEvents
Sim.Label2.Caption = "Processing Lobby..."
    i = 0
     
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
     
    'Floor
    'Room(i).AddFloor GetTex("Marble4"), -160, -150, 160, 150, 0, (FloorHeight * 2), 31
    Room(i).AddFloor GetTex("Marble4"), -160, -150, 160, -46.25, 0, ((160 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble4"), -160, 46.25, 160, 150, 0, ((160 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble4"), -90.5, -46.25, -52.5, 46.25, 0, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble4"), 52.5, -46.25, 90.5, 46.25, 0, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble4"), -12.5, -46.25, 12.5, 0, 0, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble4"), -52.5, 0, 52.5, 46.25, 0, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble4"), -160, -46.25, -130.5, 46.25, 0, ((160 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble4"), 160, -46.25, 130.5, 46.25, 0, ((160 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Mezzanine Level
    Room(i).AddFloor GetTex("Granite"), -90.5, -55, 90.5, -46.25, FloorHeight, ((90.5 * 2) * 0.086), ((55 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -90.5, 0, 90.5, 55, FloorHeight, ((90.5 * 2) * 0.086), (55 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -90.5, -46.25, -52.5, 0, FloorHeight, ((90.5 - 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 52.5, -46.25, 90.5, 0, FloorHeight, ((90.5 - 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorHeight, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    
    Room(i).AddFloor GetTex("Ceiling1"), -90.5, -55, 90.5, -46.25, FloorHeight - 0.1, ((90.5 * 2) * 0.086), ((55 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -90.5, 0, 90.5, 55, FloorHeight - 0.1, ((90.5 * 2) * 0.086), (55 * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -90.5, -46.25, -52.5, 0, FloorHeight - 0.1, ((90.5 - 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), 52.5, -46.25, 90.5, 0, FloorHeight - 0.1, ((90.5 - 52.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 0, FloorHeight - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, -150 + 0.1, 160 - 0.1, -150 + 0.1, (FloorHeight * 3), 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, -150 + 0.1, 160 - 0.1, 150 - 0.1, (FloorHeight * 3), 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), 160 - 0.1, 150 - 0.1, -160 + 0.1, 150 - 0.1, (FloorHeight * 3), 0, 3, 1
    Room(i).AddWall GetTex("LobbyFront"), -160 + 0.1, 150 - 0.1, -160 + 0.1, -150 + 0.1, (FloorHeight * 3), 0, 3, 1
                  
    'Lobby
    Call DrawElevatorWalls(0, 5, 1, True, True, True, True, True, False, False, False, False, False, False)
    Call DrawElevatorWalls(0, 5, 2, True, True, True, True, True, False, False, False, False, False, False)
    Call DrawElevatorWalls(0, 1, 3, True, True, True, True, True, True, True, True, True, True, True)
    Call DrawElevatorWalls(0, 1, 4, True, True, True, True, True, True, True, True, True, True, True)
    'Mezzanine
    Call DrawElevatorWalls(1, 5, 1, True, True, True, True, True, False, False, False, False, False, False)
    Call DrawElevatorWalls(1, 5, 2, True, True, True, True, True, False, False, False, False, False, False)
    Call DrawElevatorWalls(1, 1, 3, True, False, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(1, 1, 4, True, False, False, False, False, False, False, False, False, False, False)
    
    'Ceiling
    Room(i).AddFloor GetTex("Ceiling1"), -160, -150, 160, -46.25, (FloorHeight * 3) - 0.5, ((160 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -160, 46.25, 160, 150, (FloorHeight * 3) - 0.5, ((160 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -90.5, -46.25, -52.5, 46.25, (FloorHeight * 3) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), 52.5, -46.25, 90.5, 46.25, (FloorHeight * 3) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -12.5, -46.25, 12.5, 0, (FloorHeight * 3) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -52.5, 0, 52.5, 46.25, (FloorHeight * 3) - 0.5, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), -160, -46.25, -130.5, 46.25, (FloorHeight * 3) - 0.5, ((160 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Ceiling1"), 160, -46.25, 130.5, 46.25, (FloorHeight * 3) - 0.5, ((160 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
            
End Sub

Sub ProcessMisc()
DoEvents
 
    'Elevators
    Dim vv As Single
    Dim xx As Single
    For i = 1 To 4
    DoEvents
    If i = 1 Then j = 1
    If i = 2 Then j = 11
    If i = 3 Then j = 21
    If i = 4 Then j = 31
    
    If i = 1 Then vv = 28: xx = 12.5
    If i = 2 Then vv = 37: xx = 52.5
    If i = 3 Then vv = 106: xx = 90.5
    If i = 4 Then vv = 115: xx = 130.5
    
    ElevatorMesh(j).AddFloor GetTex("Wood2"), -vv, -29.9, -xx, -16, 0.1, 1, 1
    ElevatorMesh(j).AddFloor GetTex("Elev1"), -vv, -29.9, -xx, -16, 19.5, 2, 2
    ElevatorMesh(j).AddWall GetTex("Wood1"), -vv, -29.9, -xx, -29.9, 19.5, 0.1, 2, 2
    ElevatorMesh(j).AddWall GetTex("Wood1"), -vv, -16, -xx, -16, 19.5, 0.1, 2, 2
    ElevatorMesh(j).AddWall GetTex("Wood1"), -vv, -16, -vv, -29.9, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 1).AddFloor GetTex("Wood2"), vv, -30, xx, -16, 0.1, 1, 1
    ElevatorMesh(j + 1).AddFloor GetTex("Elev1"), vv, -30, xx, -16, 19.5, 2, 2
    ElevatorMesh(j + 1).AddWall GetTex("Wood1"), vv, -30, xx, -30, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 1).AddWall GetTex("Wood1"), vv, -16, xx, -16, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 1).AddWall GetTex("Wood1"), vv, -16, vv, -30, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 2).AddFloor GetTex("Wood2"), -vv, -15, -xx, -1, 0.1, 1, 1
    ElevatorMesh(j + 2).AddFloor GetTex("Elev1"), -vv, -15, -xx, -1, 19.5, 2, 2
    ElevatorMesh(j + 2).AddWall GetTex("Wood1"), -vv, -15, -xx, -15, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 2).AddWall GetTex("Wood1"), -vv, -1, -xx, -1, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 2).AddWall GetTex("Wood1"), -vv, -1, -vv, -15, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 3).AddFloor GetTex("Wood2"), vv, -15, xx, -1, 0.1, 1, 1
    ElevatorMesh(j + 3).AddFloor GetTex("Elev1"), vv, -15, xx, -1, 19.5, 2, 2
    ElevatorMesh(j + 3).AddWall GetTex("Wood1"), vv, -15, xx, -15, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 3).AddWall GetTex("Wood1"), vv, -1, xx, -1, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 3).AddWall GetTex("Wood1"), vv, -1, vv, -15, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 4).AddFloor GetTex("Wood2"), -vv, 0, -xx, 14, 0.1, 1, 1
    ElevatorMesh(j + 4).AddFloor GetTex("Elev1"), -vv, 0, -xx, 14, 19.5, 2, 2
    ElevatorMesh(j + 4).AddWall GetTex("Wood1"), -vv, 0, -xx, 0, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 4).AddWall GetTex("Wood1"), -vv, 14, -xx, 14, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 4).AddWall GetTex("Wood1"), -vv, 14, -vv, 0, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 5).AddFloor GetTex("Wood2"), vv, 0, xx, 14, 0.1, 1, 1
    ElevatorMesh(j + 5).AddFloor GetTex("Elev1"), vv, 0, xx, 14, 19.5, 2, 2
    ElevatorMesh(j + 5).AddWall GetTex("Wood1"), vv, 0, xx, 0, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 5).AddWall GetTex("Wood1"), vv, 14, xx, 14, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 5).AddWall GetTex("Wood1"), vv, 14, vv, 0, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 6).AddFloor GetTex("Wood2"), -vv, 15, -xx, 29, 0.1, 1, 1
    ElevatorMesh(j + 6).AddFloor GetTex("Elev1"), -vv, 15, -xx, 29, 19.5, 2, 2
    ElevatorMesh(j + 6).AddWall GetTex("Wood1"), -vv, 15, -xx, 15, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 6).AddWall GetTex("Wood1"), -vv, 29, -xx, 29, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 6).AddWall GetTex("Wood1"), -vv, 29, -vv, 15, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 7).AddFloor GetTex("Wood2"), vv, 15, xx, 29, 0.1, 1, 1
    ElevatorMesh(j + 7).AddFloor GetTex("Elev1"), vv, 15, xx, 29, 19.5, 2, 2
    ElevatorMesh(j + 7).AddWall GetTex("Wood1"), vv, 15, xx, 15, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 7).AddWall GetTex("Wood1"), vv, 29, xx, 29, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 7).AddWall GetTex("Wood1"), vv, 29, vv, 15, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 8).AddFloor GetTex("Wood2"), -vv, 30, -xx, 44, 0.1, 1, 1
    ElevatorMesh(j + 8).AddFloor GetTex("Elev1"), -vv, 30, -xx, 44, 19.5, 2, 2
    ElevatorMesh(j + 8).AddWall GetTex("Wood1"), -vv, 30, -xx, 30, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 8).AddWall GetTex("Wood1"), -vv, 44, -xx, 44, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 8).AddWall GetTex("Wood1"), -vv, 44, -vv, 30, 19.5, 0.1, 2, 2
    
    ElevatorMesh(j + 9).AddFloor GetTex("Wood2"), vv, 30, xx, 44, 0.1, 1, 1
    ElevatorMesh(j + 9).AddFloor GetTex("Elev1"), vv, 30, xx, 44, 19.5, 2, 2
    ElevatorMesh(j + 9).AddWall GetTex("Wood1"), vv, 30, xx, 30, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 9).AddWall GetTex("Wood1"), vv, 44, xx, 44, 19.5, 0.1, 2, 2
    ElevatorMesh(j + 9).AddWall GetTex("Wood1"), vv, 44, vv, 30, 19.5, 0.1, 2, 2
   
   'Floor Indicators
    If i = 1 Or i = 3 Then
    FloorIndicator(j).AddWall GetTex("ButtonL"), -(xx + 0.16), -29.5, -(xx + 0.16), -27.5, 1.5, 16, -1, 1
    FloorIndicator(j + 1).AddWall GetTex("ButtonL"), (xx + 0.16), -18.5, (xx + 0.16), -16.5, 1.5, 16, 1, 1
    FloorIndicator(j + 2).AddWall GetTex("ButtonL"), -(xx + 0.16), -29.5 + (15 * 1), -(xx + 0.16), -27.5 + (15 * 1), 1.5, 16, -1, 1
    FloorIndicator(j + 3).AddWall GetTex("ButtonL"), (xx + 0.16), -18.5 + (15 * 1), (xx + 0.16), -16.5 + (15 * 1), 1.5, 16, 1, 1
    FloorIndicator(j + 4).AddWall GetTex("ButtonL"), -(xx + 0.16), -29.5 + (15 * 2), -(xx + 0.16), -27.5 + (15 * 2), 1.5, 16, -1, 1
    FloorIndicator(j + 5).AddWall GetTex("ButtonL"), (xx + 0.16), -18.5 + (15 * 2), (xx + 0.16), -16.5 + (15 * 2), 1.5, 16, 1, 1
    FloorIndicator(j + 6).AddWall GetTex("ButtonL"), -(xx + 0.16), -29.5 + (15 * 3), -(xx + 0.16), -27.5 + (15 * 3), 1.5, 16, -1, 1
    FloorIndicator(j + 7).AddWall GetTex("ButtonL"), (xx + 0.16), -18.5 + (15 * 3), (xx + 0.16), -16.5 + (15 * 3), 1.5, 16, 1, 1
    FloorIndicator(j + 8).AddWall GetTex("ButtonL"), -(xx + 0.16), -29.5 + (15 * 4), -(xx + 0.16), -27.5 + (15 * 4), 1.5, 16, -1, 1
    FloorIndicator(j + 9).AddWall GetTex("ButtonL"), (xx + 0.16), -18.5 + (15 * 4), (xx + 0.16), -16.5 + (15 * 4), 1.5, 16, 1, 1
    End If
    If i = 2 Or i = 4 Then
    FloorIndicator(j + 1).AddWall GetTex("ButtonL"), (xx - 0.16), -29.5, (xx - 0.16), -27.5, 1.5, 16, -1, 1
    FloorIndicator(j).AddWall GetTex("ButtonL"), -(xx - 0.16), -18.5, -(xx - 0.16), -16.5, 1.5, 16, 1, 1
    FloorIndicator(j + 3).AddWall GetTex("ButtonL"), (xx - 0.16), -29.5 + (15 * 1), (xx - 0.16), -27.5 + (15 * 1), 1.5, 16, -1, 1
    FloorIndicator(j + 2).AddWall GetTex("ButtonL"), -(xx - 0.16), -18.5 + (15 * 1), -(xx - 0.16), -16.5 + (15 * 1), 1.5, 16, 1, 1
    FloorIndicator(j + 5).AddWall GetTex("ButtonL"), (xx - 0.16), -29.5 + (15 * 2), (xx - 0.16), -27.5 + (15 * 2), 1.5, 16, -1, 1
    FloorIndicator(j + 4).AddWall GetTex("ButtonL"), -(xx - 0.16), -18.5 + (15 * 2), -(xx - 0.16), -16.5 + (15 * 2), 1.5, 16, 1, 1
    FloorIndicator(j + 7).AddWall GetTex("ButtonL"), (xx - 0.16), -29.5 + (15 * 3), (xx - 0.16), -27.5 + (15 * 3), 1.5, 16, -1, 1
    FloorIndicator(j + 6).AddWall GetTex("ButtonL"), -(xx - 0.16), -18.5 + (15 * 3), -(xx - 0.16), -16.5 + (15 * 3), 1.5, 16, 1, 1
    FloorIndicator(j + 9).AddWall GetTex("ButtonL"), (xx - 0.16), -29.5 + (15 * 4), (xx - 0.16), -27.5 + (15 * 4), 1.5, 16, -1, 1
    FloorIndicator(j + 8).AddWall GetTex("ButtonL"), -(xx - 0.16), -18.5 + (15 * 4), -(xx - 0.16), -16.5 + (15 * 4), 1.5, 16, 1, 1
    End If
    
   'Button Panels
    If i = 1 Or i = 3 Then
    ElevatorMesh(j).AddWall GetTex("ElevExtPanels"), -(xx + 0.16), -29.7, -(xx + 0.16), -27.3, 7, 6, 1, 1
    ElevatorMesh(j + 1).AddWall GetTex("ElevExtPanels"), (xx + 0.16), -18.7, (xx + 0.16), -16.3, 7, 6, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("ElevExtPanels"), -(xx + 0.16), -29.7 + (15 * 1), -(xx + 0.16), -27.3 + (15 * 1), 7, 6, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("ElevExtPanels"), (xx + 0.16), -18.7 + (15 * 1), (xx + 0.16), -16.3 + (15 * 1), 7, 6, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("ElevExtPanels"), -(xx + 0.16), -29.7 + (15 * 2), -(xx + 0.16), -27.3 + (15 * 2), 7, 6, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("ElevExtPanels"), (xx + 0.16), -18.7 + (15 * 2), (xx + 0.16), -16.3 + (15 * 2), 7, 6, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("ElevExtPanels"), -(xx + 0.16), -29.7 + (15 * 3), -(xx + 0.16), -27.3 + (15 * 3), 7, 6, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("ElevExtPanels"), (xx + 0.16), -18.7 + (15 * 3), (xx + 0.16), -16.3 + (15 * 3), 7, 6, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("ElevExtPanels"), -(xx + 0.16), -29.7 + (15 * 4), -(xx + 0.16), -27.3 + (15 * 4), 7, 6, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("ElevExtPanels"), (xx + 0.16), -18.7 + (15 * 4), (xx + 0.16), -16.3 + (15 * 4), 7, 6, 1, 1
    End If
    If i = 2 Or i = 4 Then
    ElevatorMesh(j + 1).AddWall GetTex("ElevExtPanels"), (xx - 0.16), -29.7, (xx - 0.16), -27.3, 7, 6, 1, 1
    ElevatorMesh(j).AddWall GetTex("ElevExtPanels"), -(xx - 0.16), -18.7, -(xx - 0.16), -16.3, 7, 6, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("ElevExtPanels"), (xx - 0.16), -29.7 + (15 * 1), (xx - 0.16), -27.3 + (15 * 1), 7, 6, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("ElevExtPanels"), -(xx - 0.16), -18.7 + (15 * 1), -(xx - 0.16), -16.3 + (15 * 1), 7, 6, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("ElevExtPanels"), (xx - 0.16), -29.7 + (15 * 2), (xx - 0.16), -27.3 + (15 * 2), 7, 6, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("ElevExtPanels"), -(xx - 0.16), -18.7 + (15 * 2), -(xx - 0.16), -16.3 + (15 * 2), 7, 6, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("ElevExtPanels"), (xx - 0.16), -29.7 + (15 * 3), (xx - 0.16), -27.3 + (15 * 3), 7, 6, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("ElevExtPanels"), -(xx - 0.16), -18.7 + (15 * 3), -(xx - 0.16), -16.3 + (15 * 3), 7, 6, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("ElevExtPanels"), (xx - 0.16), -29.7 + (15 * 4), (xx - 0.16), -27.3 + (15 * 4), 7, 6, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("ElevExtPanels"), -(xx - 0.16), -18.7 + (15 * 4), -(xx - 0.16), -16.3 + (15 * 4), 7, 6, 1, 1
    End If
   
   'Plaques
    If i = 1 Or i = 3 Then
    Plaque(j).AddWall GetTex("Plaque"), -(xx + 0.16), -29.7, -(xx + 0.16), -27.3, 1, 13, -1, 1
    Plaque(j).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 1).AddWall GetTex("Plaque"), (xx + 0.16), -18.7, (xx + 0.16), -16.3, 1, 13, 1, 1
    Plaque(j + 1).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 1).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 2).AddWall GetTex("Plaque"), -(xx + 0.16), -29.7 + (15 * 1), -(xx + 0.16), -27.3 + (15 * 1), 1, 13, -1, 1
    Plaque(j + 2).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 2).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 3).AddWall GetTex("Plaque"), (xx + 0.16), -18.7 + (15 * 1), (xx + 0.16), -16.3 + (15 * 1), 1, 13, 1, 1
    Plaque(j + 3).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 3).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 4).AddWall GetTex("Plaque"), -(xx + 0.16), -29.7 + (15 * 2), -(xx + 0.16), -27.3 + (15 * 2), 1, 13, -1, 1
    Plaque(j + 4).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 4).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 5).AddWall GetTex("Plaque"), (xx + 0.16), -18.7 + (15 * 2), (xx + 0.16), -16.3 + (15 * 2), 1, 13, 1, 1
    Plaque(j + 5).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 5).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 6).AddWall GetTex("Plaque"), -(xx + 0.16), -29.7 + (15 * 3), -(xx + 0.16), -27.3 + (15 * 3), 1, 13, -1, 1
    Plaque(j + 6).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 6).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 7).AddWall GetTex("Plaque"), (xx + 0.16), -18.7 + (15 * 3), (xx + 0.16), -16.3 + (15 * 3), 1, 13, 1, 1
    Plaque(j + 7).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 7).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 8).AddWall GetTex("Plaque"), -(xx + 0.16), -29.7 + (15 * 4), -(xx + 0.16), -27.3 + (15 * 4), 1, 13, -1, 1
    Plaque(j + 8).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 8).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 9).AddWall GetTex("Plaque"), (xx + 0.16), -18.7 + (15 * 4), (xx + 0.16), -16.3 + (15 * 4), 1, 13, 1, 1
    Plaque(j + 9).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 9).SetColor RGBA(1, 1, 1, 0.1)
    End If
    If i = 2 Or i = 4 Then
    Plaque(j + 1).AddWall GetTex("Plaque"), (xx - 0.16), -29.7, (xx - 0.16), -27.3, 1, 13, -1, 1
    Plaque(j + 1).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 1).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j).AddWall GetTex("Plaque"), -(xx - 0.16), -18.7, -(xx - 0.16), -16.3, 1, 13, 1, 1
    Plaque(j).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 3).AddWall GetTex("Plaque"), (xx - 0.16), -29.7 + (15 * 1), (xx - 0.16), -27.3 + (15 * 1), 1, 13, -1, 1
    Plaque(j + 3).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 3).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 2).AddWall GetTex("Plaque"), -(xx - 0.16), -18.7 + (15 * 1), -(xx - 0.16), -16.3 + (15 * 1), 1, 13, 1, 1
    Plaque(j + 2).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 2).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 5).AddWall GetTex("Plaque"), (xx - 0.16), -29.7 + (15 * 2), (xx - 0.16), -27.3 + (15 * 2), 1, 13, -1, 1
    Plaque(j + 5).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 5).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 4).AddWall GetTex("Plaque"), -(xx - 0.16), -18.7 + (15 * 2), -(xx - 0.16), -16.3 + (15 * 2), 1, 13, 1, 1
    Plaque(j + 4).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 4).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 7).AddWall GetTex("Plaque"), (xx - 0.16), -29.7 + (15 * 3), (xx - 0.16), -27.3 + (15 * 3), 1, 13, -1, 1
    Plaque(j + 7).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 7).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 6).AddWall GetTex("Plaque"), -(xx - 0.16), -18.7 + (15 * 3), -(xx - 0.16), -16.3 + (15 * 3), 1, 13, 1, 1
    Plaque(j + 6).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 6).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 9).AddWall GetTex("Plaque"), (xx - 0.16), -29.7 + (15 * 4), (xx - 0.16), -27.3 + (15 * 4), 1, 13, -1, 1
    Plaque(j + 9).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 9).SetColor RGBA(1, 1, 1, 0.1)
    Plaque(j + 8).AddWall GetTex("Plaque"), -(xx - 0.16), -18.7 + (15 * 4), -(xx - 0.16), -16.3 + (15 * 4), 1, 13, 1, 1
    Plaque(j + 8).SetBlendingMode (TV_BLEND_ALPHA)
    Plaque(j + 8).SetColor RGBA(1, 1, 1, 0.1)
    End If
    
    'Interior Panels
    If i = 1 Or i = 3 Then
    ElevatorMesh(j).AddWall GetTex("Marble3"), -(xx + 0.15), -16, -(xx + 0.15), -19, 19.5, 0.1, 1, 1
    ElevatorMesh(j).AddWall GetTex("Marble3"), -(xx + 0.15), -30, -(xx + 0.15), -27, 19.5, 0.1, 1, 1
    ElevatorMesh(j + 1).AddWall GetTex("Marble3"), (xx + 0.15), -16, (xx + 0.15), -19, 19.5, 0.1, 1, 1
    ElevatorMesh(j + 1).AddWall GetTex("Marble3"), (xx + 0.15), -30, (xx + 0.15), -27, 19.5, 0.1, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("Marble3"), -(xx + 0.15), -16 + (15 * 1), -(xx + 0.15), -19 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("Marble3"), -(xx + 0.15), -30 + (15 * 1), -(xx + 0.15), -27 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("Marble3"), (xx + 0.15), -16 + (15 * 1), (xx + 0.15), -19 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("Marble3"), (xx + 0.15), -30 + (15 * 1), (xx + 0.15), -27 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("Marble3"), -(xx + 0.15), -16 + (15 * 2), -(xx + 0.15), -19 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("Marble3"), -(xx + 0.15), -30 + (15 * 2), -(xx + 0.15), -27 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("Marble3"), (xx + 0.15), -16 + (15 * 2), (xx + 0.15), -19 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("Marble3"), (xx + 0.15), -30 + (15 * 2), (xx + 0.15), -27 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("Marble3"), -(xx + 0.15), -16 + (15 * 3), -(xx + 0.15), -19 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("Marble3"), -(xx + 0.15), -30 + (15 * 3), -(xx + 0.15), -27 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("Marble3"), (xx + 0.15), -16 + (15 * 3), (xx + 0.15), -19 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("Marble3"), (xx + 0.15), -30 + (15 * 3), (xx + 0.15), -27 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("Marble3"), -(xx + 0.15), -16 + (15 * 4), -(xx + 0.15), -19 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("Marble3"), -(xx + 0.15), -30 + (15 * 4), -(xx + 0.15), -27 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("Marble3"), (xx + 0.15), -16 + (15 * 4), (xx + 0.15), -19 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("Marble3"), (xx + 0.15), -30 + (15 * 4), (xx + 0.15), -27 + (15 * 4), 19.5, 0.1, 1, 1
    End If
    If i = 2 Or i = 4 Then
    ElevatorMesh(j + 1).AddWall GetTex("Marble3"), (xx - 0.15), -16, (xx - 0.15), -19, 19.5, 0.1, 1, 1
    ElevatorMesh(j + 1).AddWall GetTex("Marble3"), (xx - 0.15), -30, (xx - 0.15), -27, 19.5, 0.1, 1, 1
    ElevatorMesh(j).AddWall GetTex("Marble3"), -(xx - 0.15), -16, -(xx - 0.15), -19, 19.5, 0.1, 1, 1
    ElevatorMesh(j).AddWall GetTex("Marble3"), -(xx - 0.15), -30, -(xx - 0.15), -27, 19.5, 0.1, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("Marble3"), (xx - 0.15), -16 + (15 * 1), (xx - 0.15), -19 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 3).AddWall GetTex("Marble3"), (xx - 0.15), -30 + (15 * 1), (xx - 0.15), -27 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("Marble3"), -(xx - 0.15), -16 + (15 * 1), -(xx - 0.15), -19 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 2).AddWall GetTex("Marble3"), -(xx - 0.15), -30 + (15 * 1), -(xx - 0.15), -27 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("Marble3"), (xx - 0.15), -16 + (15 * 2), (xx - 0.15), -19 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 5).AddWall GetTex("Marble3"), (xx - 0.15), -30 + (15 * 2), (xx - 0.15), -27 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("Marble3"), -(xx - 0.15), -16 + (15 * 2), -(xx - 0.15), -19 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 4).AddWall GetTex("Marble3"), -(xx - 0.15), -30 + (15 * 2), -(xx - 0.15), -27 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("Marble3"), (xx - 0.15), -16 + (15 * 3), (xx - 0.15), -19 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 7).AddWall GetTex("Marble3"), (xx - 0.15), -30 + (15 * 3), (xx - 0.15), -27 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("Marble3"), -(xx - 0.15), -16 + (15 * 3), -(xx - 0.15), -19 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 6).AddWall GetTex("Marble3"), -(xx - 0.15), -30 + (15 * 3), -(xx - 0.15), -27 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("Marble3"), (xx - 0.15), -16 + (15 * 4), (xx - 0.15), -19 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 9).AddWall GetTex("Marble3"), (xx - 0.15), -30 + (15 * 4), (xx - 0.15), -27 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("Marble3"), -(xx - 0.15), -16 + (15 * 4), -(xx - 0.15), -19 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorMesh(j + 8).AddWall GetTex("Marble3"), -(xx - 0.15), -30 + (15 * 4), -(xx - 0.15), -27 + (15 * 4), 19.5, 0.1, 1, 1
    End If
    
    'Interior Doors
    If i = 1 Or i = 3 Then
    ElevatorInsDoorL(j).AddWall GetTex("ElevDoors"), -(xx + 0.1), -19.05, -(xx + 0.1), -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j).AddWall GetTex("ElevDoors"), -(xx + 0.1), -23.05, -(xx + 0.1), -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 1).AddWall GetTex("ElevDoors"), (xx + 0.1), -19.05, (xx + 0.1), -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 1).AddWall GetTex("ElevDoors"), (xx + 0.1), -23.05, (xx + 0.1), -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 2).AddWall GetTex("ElevDoors"), -(xx + 0.1), -19.05 + (15 * 1), -(xx + 0.1), -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 2).AddWall GetTex("ElevDoors"), -(xx + 0.1), -23.05 + (15 * 1), -(xx + 0.1), -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 3).AddWall GetTex("ElevDoors"), (xx + 0.1), -19.05 + (15 * 1), (xx + 0.1), -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 3).AddWall GetTex("ElevDoors"), (xx + 0.1), -23.05 + (15 * 1), (xx + 0.1), -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 4).AddWall GetTex("ElevDoors"), -(xx + 0.1), -19.05 + (15 * 2), -(xx + 0.1), -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 4).AddWall GetTex("ElevDoors"), -(xx + 0.1), -23.05 + (15 * 2), -(xx + 0.1), -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 5).AddWall GetTex("ElevDoors"), (xx + 0.1), -19.05 + (15 * 2), (xx + 0.1), -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 5).AddWall GetTex("ElevDoors"), (xx + 0.1), -23.05 + (15 * 2), (xx + 0.1), -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 6).AddWall GetTex("ElevDoors"), -(xx + 0.1), -19.05 + (15 * 3), -(xx + 0.1), -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 6).AddWall GetTex("ElevDoors"), -(xx + 0.1), -23.05 + (15 * 3), -(xx + 0.1), -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 7).AddWall GetTex("ElevDoors"), (xx + 0.1), -19.05 + (15 * 3), (xx + 0.1), -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 7).AddWall GetTex("ElevDoors"), (xx + 0.1), -23.05 + (15 * 3), (xx + 0.1), -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 8).AddWall GetTex("ElevDoors"), -(xx + 0.1), -19.05 + (15 * 4), -(xx + 0.1), -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 8).AddWall GetTex("ElevDoors"), -(xx + 0.1), -23.05 + (15 * 4), -(xx + 0.1), -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 9).AddWall GetTex("ElevDoors"), (xx + 0.1), -19.05 + (15 * 4), (xx + 0.1), -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 9).AddWall GetTex("ElevDoors"), (xx + 0.1), -23.05 + (15 * 4), (xx + 0.1), -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    End If
    If i = 2 Or i = 4 Then
    ElevatorInsDoorL(j + 1).AddWall GetTex("ElevDoors"), (xx - 0.1), -19.05, (xx - 0.1), -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 1).AddWall GetTex("ElevDoors"), (xx - 0.1), -23.05, (xx - 0.1), -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j).AddWall GetTex("ElevDoors"), -(xx - 0.1), -19.05, -(xx - 0.1), -22.95, 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j).AddWall GetTex("ElevDoors"), -(xx - 0.1), -23.05, -(xx - 0.1), -27.05, 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 3).AddWall GetTex("ElevDoors"), (xx - 0.1), -19.05 + (15 * 1), (xx - 0.1), -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 3).AddWall GetTex("ElevDoors"), (xx - 0.1), -23.05 + (15 * 1), (xx - 0.1), -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 2).AddWall GetTex("ElevDoors"), -(xx - 0.1), -19.05 + (15 * 1), -(xx - 0.1), -22.95 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 2).AddWall GetTex("ElevDoors"), -(xx - 0.1), -23.05 + (15 * 1), -(xx - 0.1), -27.05 + (15 * 1), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 5).AddWall GetTex("ElevDoors"), (xx - 0.1), -19.05 + (15 * 2), (xx - 0.1), -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 5).AddWall GetTex("ElevDoors"), (xx - 0.1), -23.05 + (15 * 2), (xx - 0.1), -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 4).AddWall GetTex("ElevDoors"), -(xx - 0.1), -19.05 + (15 * 2), -(xx - 0.1), -22.95 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 4).AddWall GetTex("ElevDoors"), -(xx - 0.1), -23.05 + (15 * 2), -(xx - 0.1), -27.05 + (15 * 2), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 7).AddWall GetTex("ElevDoors"), (xx - 0.1), -19.05 + (15 * 3), (xx - 0.1), -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 7).AddWall GetTex("ElevDoors"), (xx - 0.1), -23.05 + (15 * 3), (xx - 0.1), -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 6).AddWall GetTex("ElevDoors"), -(xx - 0.1), -19.05 + (15 * 3), -(xx - 0.1), -22.95 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 6).AddWall GetTex("ElevDoors"), -(xx - 0.1), -23.05 + (15 * 3), -(xx - 0.1), -27.05 + (15 * 3), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 9).AddWall GetTex("ElevDoors"), (xx - 0.1), -19.05 + (15 * 4), (xx - 0.1), -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 9).AddWall GetTex("ElevDoors"), (xx - 0.1), -23.05 + (15 * 4), (xx - 0.1), -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorL(j + 8).AddWall GetTex("ElevDoors"), -(xx - 0.1), -19.05 + (15 * 4), -(xx - 0.1), -22.95 + (15 * 4), 19.5, 0.1, 1, 1
    ElevatorInsDoorR(j + 8).AddWall GetTex("ElevDoors"), -(xx - 0.1), -23.05 + (15 * 4), -(xx - 0.1), -27.05 + (15 * 4), 19.5, 0.1, 1, 1
    End If
    Next i
    
    'Move some elevators to their starting places
    ElevatorMesh(5).SetPosition ElevatorMesh(5).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(5).GetPosition.z
    ElevatorMesh(6).SetPosition ElevatorMesh(6).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(6).GetPosition.z
    ElevatorMesh(7).SetPosition ElevatorMesh(7).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(7).GetPosition.z
    ElevatorMesh(8).SetPosition ElevatorMesh(8).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(8).GetPosition.z
    ElevatorMesh(9).SetPosition ElevatorMesh(9).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(9).GetPosition.z
    ElevatorMesh(10).SetPosition ElevatorMesh(10).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(10).GetPosition.z
    ElevatorMesh(15).SetPosition ElevatorMesh(5).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(5).GetPosition.z
    ElevatorMesh(16).SetPosition ElevatorMesh(6).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(6).GetPosition.z
    ElevatorMesh(17).SetPosition ElevatorMesh(7).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(7).GetPosition.z
    ElevatorMesh(18).SetPosition ElevatorMesh(8).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(8).GetPosition.z
    ElevatorMesh(19).SetPosition ElevatorMesh(9).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(9).GetPosition.z
    ElevatorMesh(20).SetPosition ElevatorMesh(10).GetPosition.X, GetFloorAltitude(80), ElevatorMesh(10).GetPosition.z
    ElevatorFloor(5) = 80: CurrentFloorExact(5) = 80
    ElevatorFloor(6) = 80: CurrentFloorExact(6) = 80
    ElevatorFloor(7) = 80: CurrentFloorExact(7) = 80
    ElevatorFloor(8) = 80: CurrentFloorExact(8) = 80
    ElevatorFloor(9) = 80: CurrentFloorExact(9) = 80
    ElevatorFloor(10) = 80: CurrentFloorExact(10) = 80
    ElevatorFloor(15) = 80: CurrentFloorExact(15) = 80
    ElevatorFloor(16) = 80: CurrentFloorExact(16) = 80
    ElevatorFloor(17) = 80: CurrentFloorExact(17) = 80
    ElevatorFloor(18) = 80: CurrentFloorExact(18) = 80
    ElevatorFloor(19) = 80: CurrentFloorExact(19) = 80
    ElevatorFloor(20) = 80: CurrentFloorExact(20) = 80
    ElevatorInsDoorL(5).SetPosition ElevatorInsDoorL(5).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(5).GetPosition.z
    ElevatorInsDoorL(6).SetPosition ElevatorInsDoorL(6).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(6).GetPosition.z
    ElevatorInsDoorL(7).SetPosition ElevatorInsDoorL(7).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(7).GetPosition.z
    ElevatorInsDoorL(8).SetPosition ElevatorInsDoorL(8).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(8).GetPosition.z
    ElevatorInsDoorL(9).SetPosition ElevatorInsDoorL(9).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(9).GetPosition.z
    ElevatorInsDoorL(10).SetPosition ElevatorInsDoorL(10).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(10).GetPosition.z
    ElevatorInsDoorL(15).SetPosition ElevatorInsDoorL(5).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(5).GetPosition.z
    ElevatorInsDoorL(16).SetPosition ElevatorInsDoorL(6).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(6).GetPosition.z
    ElevatorInsDoorL(17).SetPosition ElevatorInsDoorL(7).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(7).GetPosition.z
    ElevatorInsDoorL(18).SetPosition ElevatorInsDoorL(8).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(8).GetPosition.z
    ElevatorInsDoorL(19).SetPosition ElevatorInsDoorL(9).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(9).GetPosition.z
    ElevatorInsDoorL(20).SetPosition ElevatorInsDoorL(10).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorL(10).GetPosition.z
    ElevatorInsDoorR(5).SetPosition ElevatorInsDoorR(5).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(5).GetPosition.z
    ElevatorInsDoorR(6).SetPosition ElevatorInsDoorR(6).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(6).GetPosition.z
    ElevatorInsDoorR(7).SetPosition ElevatorInsDoorR(7).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(7).GetPosition.z
    ElevatorInsDoorR(8).SetPosition ElevatorInsDoorR(8).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(8).GetPosition.z
    ElevatorInsDoorR(9).SetPosition ElevatorInsDoorR(9).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(9).GetPosition.z
    ElevatorInsDoorR(10).SetPosition ElevatorInsDoorR(10).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(10).GetPosition.z
    ElevatorInsDoorR(15).SetPosition ElevatorInsDoorR(5).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(5).GetPosition.z
    ElevatorInsDoorR(16).SetPosition ElevatorInsDoorR(6).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(6).GetPosition.z
    ElevatorInsDoorR(17).SetPosition ElevatorInsDoorR(7).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(7).GetPosition.z
    ElevatorInsDoorR(18).SetPosition ElevatorInsDoorR(8).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(8).GetPosition.z
    ElevatorInsDoorR(19).SetPosition ElevatorInsDoorR(9).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(9).GetPosition.z
    ElevatorInsDoorR(20).SetPosition ElevatorInsDoorR(10).GetPosition.X, GetFloorAltitude(80), ElevatorInsDoorR(10).GetPosition.z
    
End Sub

Sub ProcessOtherFloors()
Dim FloorHeight As Single
Dim FloorAltitude As Single

'Floor 40
    
    i = 40
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -135, -150, 135, -46.25, FloorAltitude, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -135, 46.25, 135, 150, FloorAltitude, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -90.5, -46.25, -52.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 52.5, -46.25, 90.5, 46.25, FloorAltitude, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -52.5, 0, 52.5, 46.25, FloorAltitude, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -135, -46.25, -130.5, 46.25, FloorAltitude, ((135 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 135, -46.25, 130.5, 46.25, FloorAltitude, ((135 - 130.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -135, -150, 135, -46.25, (FloorAltitude + 25) - 0.5, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -135, 46.25, 135, 150, (FloorAltitude + 25) - 0.5, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25) - 0.5, ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -135, -46.25, -130.5, 46.25, (FloorAltitude + 25) - 0.5, ((135 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 135, -46.25, 130.5, 46.25, (FloorAltitude + 25) - 0.5, ((135 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -150, 135, -46.25, (FloorAltitude + 25), ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, 46.25, 135, 150, (FloorAltitude + 25), ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((90.5 - 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + 25), ((90.5 + 52.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + 25), ((52.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -46.25, -130.5, 46.25, (FloorAltitude + 25), ((135 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 135, -46.25, 130.5, 46.25, (FloorAltitude + 25), ((135 - 130.5) * 0.086), ((46.25 + 46.25) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -150, 135, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, 46.25, 135, 150, (FloorAltitude + FloorHeight) - 0.1, ((135 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((90.5 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((52.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -135, -46.25, -110.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 135, -46.25, 110.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((135 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 270, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 4, False, False, False, False, False, False, False, False, False, False, False)
    
    Call DrawElevatorWalls(Int(i), 5, 1, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 3, False, True, True, True, True, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    
    'Room(I) Walls
    
    'top walls
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90.5, -71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 19.5, FloorAltitude, ((70 * 2) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, -71.3, 90.5, -71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -135, -71.3, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -71.3, 70, -71.3, 5.5, FloorAltitude + 19.5, ((70 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, -71.3, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    
    'bottom walls
    Room(i).AddWall GetTex("Wall3"), -135, 71.3, -90.5, 71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 19.5, FloorAltitude, ((70 * 2) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, 71.3, 90.5, 71.3, 19.5, FloorAltitude, ((135 - 90.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -135, 71.3, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 71.3, 70, 71.3, 5.5, FloorAltitude + 19.5, ((70 * 2) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 135, 71.3, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((135 - 90.5) * 0.086), (5.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -61.25 - 3.9, -46.25, 25, FloorAltitude, ((70 - 61.25 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -61.25 + 3.9, -46.25, -52.5, -46.25, 25, FloorAltitude, ((61.25 - 52.5 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 61.25 + 3.9, -46.25, 25, FloorAltitude, ((70 - 61.25 - 3.9) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 61.25 - 3.9, -46.25, 52.5, -46.25, 25, FloorAltitude, ((61.25 - 52.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -52.5, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 52.5, -46.25, 70, -46.25, 5.5, FloorAltitude + 19.5, ((70 - 52.5) * 0.086), (5.5 * 0.08)
    
    'service rooms
    Room(i).AddWall GetTex("Wall3"), 70, -46.25, 70, 46.25, 25, FloorAltitude, ((46.25 - 20) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 70, -15, 52.5, -15, 25, FloorAltitude, ((50 - 32.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), -70, -46.25, -70, 46.25, 25, FloorAltitude, ((46.25 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -15, -52.5, -15, 25, FloorAltitude, ((70 - 52.5) * 0.086), 1
    
    'left hallway
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -130 + 3.9, 70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, -90 + 3.9, 70, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -90 + 3.9, 90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 130 - 3.9, 70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 90 - 3.9, 70, 71.3, 19.5, FloorAltitude, ((90 - 70 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 90 - 3.9, 90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), 70, -150, 70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, -150, 90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 150, 70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 90.5, 150, 90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'right hallway
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -130 + 3.9, -70, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, -90 + 3.9, -70, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -130 - 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -90 + 3.9, -90.5, -71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 130 - 3.9, -70, 90 + 3.9, 19.5, FloorAltitude, ((130 - 90 - 3.9 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 90 - 3.9, -70, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 130 + 3.9, 19.5, FloorAltitude, ((150 - 130 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, FloorAltitude, ((130 - 3.9 - 90 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 90 - 3.9, -90.5, 71.3, 19.5, FloorAltitude, ((90 - 71.3 - 3.9) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, -150, -70, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, -150, -90.5, -71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -70, 150, -70, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -90.5, 150, -90.5, 71.3, 5.5, FloorAltitude + 19.5, ((150 - 71.3) * 0.086), (5.5 * 0.08)
    
    'middle hallway extension
    Room(i).AddWall GetTex("Wall3"), -12.5, 0, -12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 12.5, 0, 12.5, 46.25, 25, FloorAltitude, (46.25 * 0.086), 1
    
    'bottom middle walls
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -22.5 - 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), -22.5 + 3.9, 46.25, -12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 3.9 - 12.5) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 70, 46.25, 22.5 + 3.9, 46.25, 19.5, FloorAltitude, ((70 - 22.5 - 3.9) * 0.086), (19.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 22.5 - 3.9, 46.25, 12.5, 46.25, 19.5, FloorAltitude, ((22.5 - 3.9 - 12.5) * 0.086), (19.5 * 0.08)
    
    Room(i).AddWall GetTex("Wall3"), -70, 46.25, -12.5, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    Room(i).AddWall GetTex("Wall3"), 12.5, 46.25, 70, 46.25, 5.5, FloorAltitude + 19.5, ((70 - 12.5) * 0.086), (5.5 * 0.08)
    
    'Rooms
    Room(i).AddWall GetTex("Wall3"), -135, -110, -90.5, -110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -135, 110, -90.5, 110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, -110, 70, -110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), -70, 110, 70, 110, 25, FloorAltitude, ((70 * 2) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 135, -110, 90.5, -110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 135, 110, 90.5, 110, 25, FloorAltitude, ((135 - 90.5) * 0.086), 1
    
    Room(i).AddWall GetTex("Wall3"), 0, -71.3, 0, -150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    Room(i).AddWall GetTex("Wall3"), 0, 71.3, 0, 150, 25, FloorAltitude, ((150 - 71.3) * 0.086), 1
    

'Floor 80
    i = 80
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    'Room(i).AddFloor GetTex("Granite"), -110, -46.25, -110.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    'Room(i).AddFloor GetTex("Granite"), 110, -46.25, 110.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, 46.25, 110, 150, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    'Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -110.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    'Room(i).AddFloor GetTex("Marble3"), 110, -46.25, 110.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    'CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -110.5, 46.25, (FloorAltitude + 25), ((110 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    'CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 110.5, 46.25, (FloorAltitude + 25), ((110 - 110.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 220, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 3, False, False, False, False, False, False, False, False, False, False, False)
    
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, True, True, True, True, True, True, True, True, True)
    Call DrawElevatorWalls(Int(i), 2, 2, True, True, True, True, True, True, True, True, True, True, True)


'Floor 115
    i = 115
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 110, -46.25, 52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, 46.25, 110, 150, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 220, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 2, True, True, True, False, False, False, False, False, False, False, False)
    
    
'Floor 116
    i = 116
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 110, -46.25, 52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, 46.25, 110, 150, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 220, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 2, True, True, True, False, False, False, False, False, False, False, False)
  
'Floor 117
    i = 117
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -110, -150, 110, -46.25, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, 46.25, 110, 150, FloorAltitude, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 46.25, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -110, -46.25, -52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Granite"), 110, -46.25, 52.5, 46.25, FloorAltitude, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -110, -150, 110, -46.25, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, 46.25, 110, 150, (FloorAltitude + 25) - 0.5, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25) - 0.5, ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + 25), ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + 25), ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 52.5, 46.25, (FloorAltitude + 25), ((110 - 52.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -150, 110, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, 46.25, 110, 150, (FloorAltitude + FloorHeight) - 0.1, ((110 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -110, -46.25, -32.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 110, -46.25, 32.5, 46.25, (FloorAltitude + FloorHeight) - 0.1, ((110 - 32.5) * 0.086), ((46.25 * 2) * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 220, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, False, False, False, False, False, False)
    Call DrawElevatorWalls(Int(i), 2, 2, True, False, False, False, False, False, False, False, False, False, False)
    

'Floor 130
    i = 130
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, FloorAltitude, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 85, -46.25, 32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 85, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + 25), ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 2, 1, False, True, False, False, False, False, False, False, False, False, False)
    
    
End Sub

Sub ProcessOtherFloors2()
Dim FloorHeight As Single
Dim FloorAltitude As Single

'Floor 131
    i = 131
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, FloorAltitude, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 85, -46.25, 32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 85, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + 25), ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, False, False, False, False, False, False, False, False, False)
    
    
'Floor 132
    i = 132
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, FloorAltitude, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 85, -46.25, 32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 85, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + 25), ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, True, True, True, False, False, False, False, False, False)
    
    
'Floor 133
    i = 133
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 0, 85, 150, FloorAltitude, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 85, -46.25, 32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 85, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + 25), ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + 25), ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + 25), ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 85, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, False, False, False, False, False, False, False, False, False)
    
    'Room(I) Walls
    Room(i).AddWall GetTex("Wall3"), -85, 35, 85, 35, 25, FloorAltitude, 9, 1
        

'Floor 134
    i = 134
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    Room(i).AddFloor GetTex("Granite"), -85, -150, 85, -46.25, FloorAltitude, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, -46.25, -32.5, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 32.5, -46.25, 85, 0, FloorAltitude, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    
    'modified floor for pool
    Room(i).AddFloor GetTex("Granite"), -50, 0, 63, 41, FloorAltitude, ((63 + 50) * 0.086), (41 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -85, 0, -50, 150, FloorAltitude, ((85 - 50) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 63, 0, 85, 150, FloorAltitude, ((85 - 63) * 0.086), (150 * 0.08)
    
    Room(i).AddFloor GetTex("Marble3"), -85, -150, 85, -46.25, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, 0, 85, 150, (FloorAltitude + 25) - 0.5, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -85, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 32.5, -46.25, 85, 0, (FloorAltitude + 25) - 0.5, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
     
    'Crawlspace Bottom
    Room(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, FloorAltitude + 25, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, FloorAltitude + 25, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 32.5, -46.25, 85, 0, FloorAltitude + 25, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude + 25, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    Room(i).AddFloor GetTex("BrickTexture"), -85, -150, 85, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -85, 0, 85, 150, (FloorAltitude + FloorHeight) - 0.1, ((85 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -85, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 32.5, -46.25, 85, 0, (FloorAltitude + FloorHeight) - 0.1, ((85 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
        
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, True, True, True, False, False, False, False, False, False)
       

'Floor 135
    i = 135
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, FloorAltitude, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -60, 0, 60, 150, FloorAltitude, ((60 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 60, -46.25, 32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (i * FloorHeight) + (FloorHeight * 2) - 0.5, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -60, 0, 60, 15, (i * FloorHeight) + (FloorHeight * 2) - 0.5, ((60 * 2) * 0.086), (15 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (i * FloorHeight) + (FloorHeight * 2) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, 0, (i * FloorHeight) + (FloorHeight * 2) - 0.5, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 60, -46.25, 32.5, 0, (i * FloorHeight) + (FloorHeight * 2) - 0.5, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, True, True, True, False, False, False, False, False, False)
         

'Floor 136
    i = 136
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("Granite"), -60, -150, 60, -46.25, FloorAltitude, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Granite"), -60, 0, 60, 15, FloorAltitude, ((60 * 2) * 0.086), (15 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), -60, -46.25, -32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Granite"), 60, -46.25, 32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("Marble3"), -60, -150, 60, -46.25, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -60, 0, 60, 150, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), -60, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("Marble3"), 60, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (FloorAltitude + 25), ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 15, (FloorAltitude + 25), ((60 * 2) * 0.086), (15 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, (FloorAltitude + 25), ((60 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, (FloorAltitude + 25), ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 150, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 120, 300, 0, 0, Int(i), True, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 5, 1, False, True, True, True, True, False, False, False, False, False, False)
   
    
'Floor 137
    i = 137
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    'Floor
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, FloorAltitude, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 150, FloorAltitude, ((60 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, FloorAltitude, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 12.5, 46.25, FloorAltitude, ((52.5 - 12.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, FloorAltitude, ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Ceiling
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 150, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), (150 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), (46.25 * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, (FloorAltitude + 25) - 0.5, ((60 * 2) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Bottom
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (FloorAltitude + 25), ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 150, (FloorAltitude + 25), ((60 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, (FloorAltitude + 25), ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -52.5, 0, 12.5, 46.25, (FloorAltitude + 25), ((52.5 - 12.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, (FloorAltitude + 25), ((60 - 32.5) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, (FloorAltitude + 25), ((60 - 32.5) * 0.086), (46.25 * 0.08)
    
    'Crawlspace Top
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, 0, 60, 150, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), (150 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((12.5 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), (46.25 * 0.08)
    CrawlSpace(i).AddFloor GetTex("BrickTexture"), 60, -46.25, 32.5, 0, (FloorAltitude + FloorHeight) - 0.1, ((60 * 2) * 0.086), (46.25 * 0.08)
    
    'Crawlspace walls
    Call CreateWallBox(CrawlSpace(i), "BrickTexture", 170, 300, 0, 0, Int(i), True, 1, 1)
    
    'Walls
    Call CreateWallBox(Room(i), "BrickTexture", 120 - 0.2, 300 - 0.2, 0, 0, Int(i), False, 1, 1)
    
    Call DrawElevatorWalls(Int(i), 7, 1, False, True, False, False, False, False, False, False, False, False, False)
        

'Roof Layout
i = 138
    FloorHeight = GetFloorHeight(Int(i))
    FloorAltitude = GetFloorAltitude(Int(i))
    DoEvents
    Sim.Label2.Caption = "Processing Floors 40, 80, 115 to 117 and 130 to 138... " + Str$(Int(((i - 24) / (138 - 24)) * 100)) + "%"
    
    Room(i).AddFloor GetTex("BrickTexture"), -60, -150, 60, -46.25, FloorAltitude, ((60 * 2) * 0.086), ((150 - 46.25) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, -15.42, 60, 150, FloorAltitude, ((60 * 2) * 0.086), ((150 - 15.42) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -60, -46.25, -32.5, -15.42, FloorAltitude, ((60 - 32.5) * 0.086), ((46.25 - 15.42) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), 12.5, -46.25, 60, -15.42, FloorAltitude, ((60 - 12.5) * 0.086), ((46.25 - 15.42) * 0.08)
    Room(i).AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, -15.42, FloorAltitude, ((12.5 * 2) * 0.086), ((46.25 - 15.42) * 0.08)
    
    Room(i).AddFloor GetTex("BrickTexture"), -32.5, -46.25, -12.5, -15.42, (FloorAltitude + 25) + 0.1, 2, 2.4
    
    Call DrawElevatorWalls(Int(i), 7, 1, False, True, False, False, False, False, False, False, False, False, False)

End Sub

Sub ProcessOutside()
DoEvents

'Outside of Building
    
    Landscape.AddFloor GetTex("Downtown"), 2290, -1140, 7000, 1140, 0, 0.5, 0.25
    Landscape.AddFloor GetTex("Downtown"), -1900, -1140, -7000, 1140, 0, 0.5, 0.25
    Landscape.AddFloor GetTex("Downtown"), -7000, -1140, 7000, -7000, 0, 1.5, 0.75
    Landscape.AddFloor GetTex("Downtown"), -7000, 1140, 7000, 7000, 0, 1.5, 0.75
    
    Landscape.AddFloor GetTex("Suburbs"), 7000, -7000, 100000, 7000, 0, 10, 1
    Landscape.AddFloor GetTex("Suburbs"), -7000, -7000, -100000, 7000, 0, 10, 1
    Landscape.AddFloor GetTex("Suburbs"), -100000, -7000, 100000, -100000, 0, 10, 10
    Landscape.AddFloor GetTex("Suburbs"), -100000, 7000, 100000, 100000, 0, 10, 10
    
    'concrete below buildings
    Landscape.AddFloor GetTex("Walkway"), 1650, -790, 2290, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 1650, -320, 2290, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 1650, -200, 2290, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 1650, 320, 2290, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 1650, 790, 2290, 1140, -0.1, 4, 4
    
    Landscape.AddFloor GetTex("Walkway"), 940, -790, 1580, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 940, -320, 1580, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 940, -200, 1580, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 940, 320, 1580, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 940, 790, 1580, 1140, -0.1, 4, 4
        
    Landscape.AddFloor GetTex("Walkway"), 230, -790, 870, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 230, -320, 870, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 230, -200, 870, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 230, 320, 870, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 230, 790, 870, 1140, -0.1, 4, 4
    
    Landscape.AddFloor GetTex("Walkway"), 160, -790, -480, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 160, -320, -480, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 160, -200, -480, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 160, 320, -480, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), 160, 790, -480, 1140, -0.1, 4, 4
    
    Landscape.AddFloor GetTex("Walkway"), -550, -790, -1190, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -550, -320, -1190, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -550, -200, -1190, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -550, 320, -1190, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -550, 790, -1190, 1140, -0.1, 4, 4
    
    Landscape.AddFloor GetTex("Walkway"), -1260, -790, -1900, -1140, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -1260, -320, -1900, -670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -1260, -200, -1900, 200, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -1260, 320, -1900, 670, -0.1, 4, 4
    Landscape.AddFloor GetTex("Walkway"), -1260, 790, -1900, 1140, -0.1, 4, 4
    
    'Main East/West Road
    Landscape.AddFloor GetTex("Road1"), 1650, -1140, 2290, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, -1200, 2290, -1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, -1140, 1580, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, -1200, 1580, -1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, -1140, 870, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, -1200, 870, -1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, -1140, -480, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, -1200, -480, -1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, -1140, -1190, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, -1200, -1190, -1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, -1140, -1900, -1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, -1200, -1900, -1260, 0, 4, -1
    
    Landscape.AddFloor GetTex("Road1"), 1650, -670, 2290, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, -730, 2290, -790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, -670, 1580, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, -730, 1580, -790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, -670, 870, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, -730, 870, -790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, -670, -480, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, -730, -480, -790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, -670, -1190, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, -730, -1190, -790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, -670, -1900, -730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, -730, -1900, -790, 0, 4, -1
    
    Landscape.AddFloor GetTex("Road1"), 1650, -200, 2290, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, -260, 2290, -320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, -200, 1580, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, -260, 1580, -320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, -200, 870, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, -260, 870, -320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, -200, -480, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, -260, -480, -320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, -200, -1190, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, -260, -1190, -320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, -200, -1900, -260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, -260, -1900, -320, 0, 4, -1
    
    Landscape.AddFloor GetTex("Road1"), 1650, 200, 2290, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, 260, 2290, 320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, 200, 1580, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, 260, 1580, 320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, 200, 870, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, 260, 870, 320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, 200, -480, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, 260, -480, 320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, 200, -1190, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, 260, -1190, 320, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, 200, -1900, 260, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, 260, -1900, 320, 0, 4, -1
    
    Landscape.AddFloor GetTex("Road1"), 1650, 670, 2290, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, 730, 2290, 790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, 670, 1580, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, 730, 1580, 790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, 670, 870, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, 730, 870, 790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, 670, -480, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, 730, -480, 790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, 670, -1190, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, 730, -1190, 790, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, 670, -1900, 730, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, 730, -1900, 790, 0, 4, -1
    
    Landscape.AddFloor GetTex("Road1"), 1650, 1140, 2290, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 1650, 1200, 2290, 1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 940, 1140, 1580, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 940, 1200, 1580, 1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 230, 1140, 870, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 230, 1200, 870, 1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), 160, 1140, -480, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), 160, 1200, -480, 1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -550, 1140, -1190, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -550, 1200, -1190, 1260, 0, 4, -1
    Landscape.AddFloor GetTex("Road1"), -1260, 1140, -1900, 1200, 0, 4, 1
    Landscape.AddFloor GetTex("Road1"), -1260, 1200, -1900, 1260, 0, 4, -1
    
    'North/South roads
    Landscape.AddFloor GetTex("Road5"), 2290, -790, 2360, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 2290, -320, 2360, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 2290, -200, 2360, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 2290, 320, 2360, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 2290, 790, 2360, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), 1580, -790, 1650, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 1580, -320, 1650, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 1580, -200, 1650, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 1580, 320, 1650, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 1580, 790, 1650, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), 870, -790, 940, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 870, -320, 940, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 870, -200, 940, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 870, 320, 940, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 870, 790, 940, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), 160, -790, 230, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 160, -320, 230, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 160, -200, 230, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 160, 320, 230, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), 160, 790, 230, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), -480, -790, -550, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -480, -320, -550, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -480, -200, -550, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -480, 320, -550, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -480, 790, -550, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), -1190, -790, -1260, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1190, -320, -1260, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1190, -200, -1260, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1190, 320, -1260, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1190, 790, -1260, 1140, 0, 1, 5
    
    Landscape.AddFloor GetTex("Road5"), -1900, -790, -1970, -1140, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1900, -320, -1970, -670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1900, -200, -1970, 200, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1900, 320, -1970, 670, 0, 1, 5
    Landscape.AddFloor GetTex("Road5"), -1900, 790, -1970, 1140, 0, 1, 5
    
    'Intersections
    Landscape.AddFloor GetTex("Road4"), 1580, -790, 1650, -670, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 1580, -320, 1650, -200, 0, 1, 1
    Landscape.AddFloor GetTex("Road4"), 1580, 200, 1650, 320, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 1580, 670, 1650, 790, 0, 1, 1
    
    Landscape.AddFloor GetTex("Road4"), 870, -790, 940, -670, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 870, -320, 940, -200, 0, 1, 1
    Landscape.AddFloor GetTex("Road4"), 870, 200, 940, 320, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 870, 670, 940, 790, 0, 1, 1
    
    Landscape.AddFloor GetTex("Road4"), 230, -790, 160, -670, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 230, -320, 160, -200, 0, 1, 1
    Landscape.AddFloor GetTex("Road4"), 230, 200, 160, 320, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), 230, 670, 160, 790, 0, 1, 1
    
    Landscape.AddFloor GetTex("Road4"), -550, -790, -480, -670, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), -550, -320, -480, -200, 0, 1, 1
    Landscape.AddFloor GetTex("Road4"), -550, 200, -480, 320, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), -550, 670, -480, 790, 0, 1, 1
    
    Landscape.AddFloor GetTex("Road4"), -1260, -790, -1190, -670, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), -1260, -320, -1190, -200, 0, 1, 1
    Landscape.AddFloor GetTex("Road4"), -1260, 200, -1190, 320, 0, 1, -1
    Landscape.AddFloor GetTex("Road4"), -1260, 670, -1190, 790, 0, 1, 1
       
    
    'Other Towers
    
    '*** Building directly south of the Triton Center
    Buildings.AddWall GetTex("Windows11"), -160, 400, 160, 400, 32 * 15, 0, (160 * 2) * 0.02, 15
    Buildings.AddWall GetTex("Windows11"), -160, 600, 160, 600, 32 * 15, 0, (160 * 2) * 0.02, 15
    Buildings.AddWall GetTex("Windows11"), -160, 400, -160, 600, 32 * 15, 0, (600 - 400) * 0.02, 15
    Buildings.AddWall GetTex("Windows11"), 160, 400, 160, 600, 32 * 15, 0, (600 - 400) * 0.02, 15
    Buildings.AddFloor GetTex("Concrete"), -160, 400, 160, 600, 32 * 15, 30, 30
    
    '*** Building directly west of the Triton Center
    Buildings.AddWall GetTex("Windows11"), 470, -150, 320, -150, 30 * 36, 0, (470 - 320) * 0.02, 36
    Buildings.AddWall GetTex("Windows11"), 470, 150, 320, 150, 30 * 36, 0, (470 - 320) * 0.02, 36
    Buildings.AddWall GetTex("Windows11"), 470, -150, 470, 150, 30 * 36, 0, (150 * 2) * 0.02, 36
    Buildings.AddWall GetTex("Windows11"), 320, -150, 320, 150, 30 * 36, 0, (150 * 2) * 0.02, 36
    Buildings.AddFloor GetTex("Concrete"), 470, -150, 320, 150, 30 * 36, 30, 30
    
    '*** 2 Buildings directly west of the Triton Center
    Buildings.AddWall GetTex("Windows11"), 820, -125, 600, -125, 35 * 86, 0, (820 - 600) * 0.02, 86
    Buildings.AddWall GetTex("Windows11"), 820, 125, 600, 125, 35 * 86, 0, (820 - 600) * 0.02, 86
    Buildings.AddWall GetTex("Windows11"), 820, -125, 820, 125, 35 * 86, 0, (125 * 2) * 0.02, 86
    Buildings.AddWall GetTex("Windows11"), 600, -125, 600, 125, 35 * 86, 0, (125 * 2) * 0.02, 86
    Buildings.AddFloor GetTex("Concrete"), 820, -125, 600, 125, 35 * 86, 30, 30
    
    '*** Building directly east of the Triton Center
    Buildings.AddWall GetTex("Windows11"), -400, -100, -250, -100, 32 * 45, 0, (400 - 250) * 0.02, 45
    Buildings.AddWall GetTex("Windows11"), -400, 100, -250, 100, 32 * 45, 0, (400 - 250) * 0.02, 45
    Buildings.AddWall GetTex("Windows11"), -400, -100, -400, 100, 32 * 45, 0, (100 * 2) * 0.02, 45
    Buildings.AddWall GetTex("Windows11"), -250, -100, -250, 100, 32 * 45, 0, (100 * 2) * 0.02, 45
    Buildings.AddFloor GetTex("Concrete"), -400, -100, -250, 100, 32 * 45, 30, 30
    
    '*** 3 buildings west of the Triton Center
    
    Buildings.AddWall GetTex("Windows11"), 1300, -100, 970, -100, 32 * 25, 0, (1300 - 970) * 0.02, 25
    Buildings.AddWall GetTex("Windows11"), 1300, 100, 970, 100, 32 * 25, 0, (1300 - 970) * 0.02, 25
    Buildings.AddWall GetTex("Windows11"), 1300, -100, 1300, 100, 32 * 25, 0, (100 * 2) * 0.02, 25
    Buildings.AddWall GetTex("Windows11"), 970, -100, 970, 100, 32 * 25, 0, (100 * 2) * 0.02, 25
    Buildings.AddFloor GetTex("Concrete"), 1300, -100, 970, 100, 32 * 25, 30, 30
    
    '*** 2 buildings east of the Triton Center
    
    Buildings.AddWall GetTex("Windows11"), -820, -100, -650, -100, 32 * 65, 0, (820 - 650) * 0.02, 65
    Buildings.AddWall GetTex("Windows11"), -820, 100, -650, 100, 32 * 65, 0, (820 - 650) * 0.02, 65
    Buildings.AddWall GetTex("Windows11"), -820, -100, -820, 100, 32 * 65, 0, (100 * 2) * 0.02, 65
    Buildings.AddWall GetTex("Windows11"), -650, -100, -650, 100, 32 * 65, 0, (100 * 2) * 0.02, 65
    Buildings.AddFloor GetTex("Concrete"), -820, -100, -650, 100, 32 * 65, 30, 30
    
    '*** 1 south, 1 east
    Buildings.AddWall GetTex("Windows11"), -400, 400, -250, 400, 32 * 12, 0, (400 - 250) * 0.02, 12
    Buildings.AddWall GetTex("Windows11"), -400, 600, -250, 600, 32 * 12, 0, (400 - 250) * 0.02, 12
    Buildings.AddWall GetTex("Windows11"), -400, 400, -400, 600, 32 * 12, 0, (600 - 400) * 0.02, 12
    Buildings.AddWall GetTex("Windows11"), -250, 400, -250, 600, 32 * 12, 0, (600 - 400) * 0.02, 12
    Buildings.AddFloor GetTex("Concrete"), -400, 400, -250, 600, 32 * 12, 30, 30
    
    '*** 1 south, 2 east
    Buildings.AddTriangle GetTex("Windows11"), -650, 0, 400, -870, 32 * 45, 500, -650, 0, 600, (600 - 400) * 0.02, 45
    Buildings.AddTriangle GetTex("Windows11"), -1090, 0, 400, -870, 32 * 45, 500, -1090, 0, 600, (600 - 400) * 0.02, 45
    Buildings.AddTriangle GetTex("Windows11"), -650, 0, 400, -870, 32 * 45, 500, -1090, 0, 400, (1090 - 650) * 0.02, 45
    Buildings.AddTriangle GetTex("Windows11"), -650, 0, 600, -870, 32 * 45, 500, -1090, 0, 600, (1090 - 650) * 0.02, 45
        
    External.AddWall GetTex("LobbyFront"), -160, -150, 160, -150, 32 * 3, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, -150, 160, 150, 32 * 3, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), 160, 150, -160, 150, 32 * 3, 0, 3, 1
    External.AddWall GetTex("LobbyFront"), -160, 150, -160, -150, 32 * 3, 0, 3, 1

For i = 2 To 39
DoEvents
    External.AddWall GetTex("MainWindows"), -160, -150, 160, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (160 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 160, -150, 160, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 160, 150, -160, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (160 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), -160, 150, -160, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
Next i


For i = 40 To 79
DoEvents
    External.AddWall GetTex("MainWindows"), -135, -150, 135, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (135 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 135, -150, 135, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 135, 150, -135, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (135 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), -135, 150, -135, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
Next i

For i = 80 To 117
DoEvents
    External.AddWall GetTex("MainWindows"), -110, -150, 110, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (110 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 110, -150, 110, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 110, 150, -110, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (110 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), -110, 150, -110, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
Next i
    
For i = 118 To 134
DoEvents
    External.AddWall GetTex("MainWindows"), -85, -150, 85, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (85 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 85, -150, 85, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 85, 150, -85, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (85 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), -85, 150, -85, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
  
Next i

For i = 135 To 136
DoEvents
    External.AddWall GetTex("MainWindows"), -60, -150, 60, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (60 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 60, -150, 60, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), 60, 150, -60, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (60 * 2) * 0.023, 1
    External.AddWall GetTex("MainWindows"), -60, 150, -60, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), (150 * 2) * 0.023, 1
Next i

i = 137
    External.AddWall GetTex("Concrete"), -60, -150, 60, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, -150, 60, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), 7 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), 60, 150, -60, 150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), 3 * 4, 1 * 4
    External.AddWall GetTex("Concrete"), -60, 150, -60, -150, GetFloorHeight(Int(i)), GetFloorAltitude(Int(i)), 7 * 4, 1 * 4
    
'Landings
    Buildings.AddFloor GetTex("BrickTexture"), -160, -150, -135, 150, GetFloorAltitude(40), 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 160, -150, 135, 150, GetFloorAltitude(40), 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -135, -150, -110, 150, GetFloorAltitude(80), 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 135, -150, 110, 150, GetFloorAltitude(80), 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -110, -150, -85, 150, GetFloorAltitude(118), 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 110, -150, 85, 150, GetFloorAltitude(118), 10, 10

    Buildings.AddFloor GetTex("BrickTexture"), -85, -150, -60, 150, GetFloorAltitude(135), 10, 10
    Buildings.AddFloor GetTex("BrickTexture"), 85, -150, 60, 150, GetFloorAltitude(135), 10, 10

'Antennae
    '1
    Buildings.AddWall GetTex("Concrete"), -25, 10, -25, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 15, -25, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 10, -25, 10, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), -20, 10, -20, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    
    '2
    Buildings.AddWall GetTex("Concrete"), 25, 10, 25, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 15, 25, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 10, 25, 10, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    Buildings.AddWall GetTex("Concrete"), 20, 10, 20, 15, 20 * 32, GetFloorAltitude(138), 1 * 2, 20 * 4
    
End Sub

Sub InitObjectsForFloor(Floor As Integer)
Dim FloorHeight As Single
Dim FloorAltitude As Single
FloorHeight = GetFloorHeight(Floor)
FloorAltitude = GetFloorAltitude(Floor)

'If Test1 = False Then Exit Sub
'Test1 = False
'Exit Sub
If Floor = 1 Then

''Column 1
'i = 1 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 1)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 45
'Objects(i).SetPosition -75, 5, 45
'Objects(i).Optimize

''Column 2
'i = 2 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 2)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, 15
''Objects(i).Optimize

''Column 3
'i = 3 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 3)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -15
''Objects(i).Optimize

''Column 4
'i = 4 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 4)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition -75, 13.5, -45
''Objects(i).Optimize

''Column 5
'i = 5 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 5)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 45
''Objects(i).Optimize

''Column 6
'i = 6 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 6)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, 15
''Objects(i).Optimize

''Column 7
'i = 7 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 7)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -15
''Objects(i).Optimize

''Column 8
'i = 8 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 8)
'Objects(i).LoadXFile App.Path + "\objects\column.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.105, 0.105, 0.105
'Objects(i).SetTexture GetTex("ColumnTex")
'Objects(i).SetPosition 75, 13.5, -45
''Objects(i).Optimize

''Door 1
'i = 9 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 9)
'Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, 0, -36.4

''Door 2
'i = 10 + (150 * (Floor - 1))
'Call Init_Objects(Floor, 10)
'Objects(i).AddWall GetTex("StairsDoor2"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
''Objects(i).SetColor (RGBA(10, 10, 10, 1))
''Objects(i).ScaleMesh 0.45, 0.535, 0.535
''Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetMeshName ("DoorB " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
'Objects(i).SetPosition -12.8, FloorHeight, -36.4

End If

If Floor >= 2 And Floor <= 39 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 2 To 27
'Windows 2 to 27
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 1) * 12) + ((j - 1) * 0.15), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 28
i = 28 + (150 * (Floor - 1))
Call Init_Objects(Floor, 28)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 29 To 54
'Windows 29 to 54
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 154 - ((j - 28) * 12) + ((j - 28) * 0.15), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

'Window 55
i = 55 + (150 * (Floor - 1))
Call Init_Objects(Floor, 55)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 159.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 56 to 79
For j = 56 To 79
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 159.5, FloorAltitude - 0.5, 143 - ((j - 55) * 12) + ((j - 55) * 0.1)
'Objects(i).Optimize
Next j

'Window 80
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -159.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 81 to 104
For j = 81 To 104
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -159.5, FloorAltitude - 0.5, 143 - ((j - 80) * 12) + ((j - 80) * 0.1)
'Objects(i).Optimize
Next j

'Door 01
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -130

'Door 02
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -90

'Door 03
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 130

'Door 04
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 90

'Door 05
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, -130

'Door 06
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, -90

'Door 07
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, 130

'Door 08
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, 90

'Door 09
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -130

'Door 10
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -90

'Door 11
i = 116 + (150 * (Floor - 1))
Call Init_Objects(Floor, 116)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 130

'Door 12
i = 117 + (150 * (Floor - 1))
Call Init_Objects(Floor, 117)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 90

'Door 13
i = 118 + (150 * (Floor - 1))
Call Init_Objects(Floor, 118)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, -130

'Door 14
i = 119 + (150 * (Floor - 1))
Call Init_Objects(Floor, 119)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, -90

'Door 15
i = 120 + (150 * (Floor - 1))
Call Init_Objects(Floor, 120)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, 130

'Door 16
i = 121 + (150 * (Floor - 1))
Call Init_Objects(Floor, 121)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, 90

'Service Door
i = 122 + (150 * (Floor - 1))
Call Init_Objects(Floor, 122)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61.25, FloorAltitude, -46.25

'Service Door 2
i = 123 + (150 * (Floor - 1))
Call Init_Objects(Floor, 123)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61.25, FloorAltitude, -46.25

'Door 17
i = 124 + (150 * (Floor - 1))
Call Init_Objects(Floor, 124)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 22.5, FloorAltitude, 46.25

'Door 18
i = 125 + (150 * (Floor - 1))
Call Init_Objects(Floor, 125)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -22.5, FloorAltitude, 46.25

End If

If Floor >= 40 And Floor <= 79 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 134.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 134.5, FloorAltitude - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -134.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -134.5, FloorAltitude - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 129, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 72
'Windows 52 to 72
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 51) * 12) + ((j - 51) * 0.1), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 73
i = 73 + (150 * (Floor - 1))
Call Init_Objects(Floor, 73)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 129, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 74 To 94
'Windows 74 to 94
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 129 - ((j - 73) * 12) + ((j - 73) * 0.1), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

'Door 01
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -130

'Door 02
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -90

'Door 03
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 130

'Door 04
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 90

'Door 05
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, -130

'Door 06
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, -90

'Door 07
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, 130

'Door 08
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 90.5, FloorAltitude, 90

'Door 09
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -130

'Door 10
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -90

'Door 11
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 130

'Door 12
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 90

'Door 13
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, -130

'Door 14
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, -90

'Door 15
i = 110 + (150 * (Floor - 1))
Call Init_Objects(Floor, 110)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, 130

'Door 16
i = 111 + (150 * (Floor - 1))
Call Init_Objects(Floor, 111)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -90.5, FloorAltitude, 90

'Service Door
i = 112 + (150 * (Floor - 1))
Call Init_Objects(Floor, 112)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 61.25, FloorAltitude, -46.25

'Service Door 2
i = 113 + (150 * (Floor - 1))
Call Init_Objects(Floor, 113)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition -61.25, FloorAltitude, -46.25

'Door 17
i = 114 + (150 * (Floor - 1))
Call Init_Objects(Floor, 114)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 22.5, FloorAltitude, 46.25

'Door 18
i = 115 + (150 * (Floor - 1))
Call Init_Objects(Floor, 115)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorC " + Str$(i))
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -22.5, FloorAltitude, 46.25

End If

If Floor >= 80 And Floor <= 117 Then

'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 109.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 109.5, FloorAltitude - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -109.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -109.5, FloorAltitude - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 103, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 68
'Windows 52 to 68
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 51) * 12) + ((j - 51) * 0.15), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 69
i = 69 + (150 * (Floor - 1))
Call Init_Objects(Floor, 69)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 103, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 70 To 86
'Windows 70 to 86
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 103 - ((j - 69) * 12) + ((j - 69) * 0.15), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

End If

If Floor >= 81 And Floor <= 114 Then

'Door 01
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -118

'Door 02
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -102

'Door 03
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, -60

'Door 04
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 27

'Door 05
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 66

'Door 06
i = 93 + (150 * (Floor - 1))
Call Init_Objects(Floor, 93)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 102

'Door 07
i = 94 + (150 * (Floor - 1))
Call Init_Objects(Floor, 94)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -70, FloorAltitude, 116

'Door 08
i = 95 + (150 * (Floor - 1))
Call Init_Objects(Floor, 95)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -118

'Door 09
i = 96 + (150 * (Floor - 1))
Call Init_Objects(Floor, 96)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -102

'Door 10
i = 97 + (150 * (Floor - 1))
Call Init_Objects(Floor, 97)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, -60

'Door 11
i = 98 + (150 * (Floor - 1))
Call Init_Objects(Floor, 98)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 27

'Door 12
i = 99 + (150 * (Floor - 1))
Call Init_Objects(Floor, 99)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 66

'Door 13
i = 100 + (150 * (Floor - 1))
Call Init_Objects(Floor, 100)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 102

'Door 14
i = 101 + (150 * (Floor - 1))
Call Init_Objects(Floor, 101)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 70, FloorAltitude, 116

'Door 15
i = 102 + (150 * (Floor - 1))
Call Init_Objects(Floor, 102)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, 118

'Door 16
i = 103 + (150 * (Floor - 1))
Call Init_Objects(Floor, 103)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, 102

'Door 17
i = 104 + (150 * (Floor - 1))
Call Init_Objects(Floor, 104)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, 118

'Door 18
i = 105 + (150 * (Floor - 1))
Call Init_Objects(Floor, 105)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, 102

'Door 19
i = 106 + (150 * (Floor - 1))
Call Init_Objects(Floor, 106)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, -118

'Door 20
i = 107 + (150 * (Floor - 1))
Call Init_Objects(Floor, 107)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, -102

'Door 21
i = 108 + (150 * (Floor - 1))
Call Init_Objects(Floor, 108)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, -118

'Door 22
i = 109 + (150 * (Floor - 1))
Call Init_Objects(Floor, 109)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, -102

End If

If Floor >= 118 And Floor <= 134 Then
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 84.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 84.5, FloorAltitude - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -84.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -84.5, FloorAltitude - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 78, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 64
'Windows 52 to 64
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 51) * 12) + ((j - 51) * 0.15), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 65
i = 65 + (150 * (Floor - 1))
Call Init_Objects(Floor, 65)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 78, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 66 To 78
'Windows 66 to 78
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 78 - ((j - 65) * 12) + ((j - 65) * 0.15), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

End If

If Floor >= 118 And Floor <= 129 Then

'Door 01
i = 80 + (150 * (Floor - 1))
Call Init_Objects(Floor, 80)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -60, FloorAltitude, -60

'Door 02
i = 81 + (150 * (Floor - 1))
Call Init_Objects(Floor, 81)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 60, FloorAltitude, -60

'Door 03
i = 82 + (150 * (Floor - 1))
Call Init_Objects(Floor, 82)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -60, FloorAltitude, 60

'Door 04
i = 83 + (150 * (Floor - 1))
Call Init_Objects(Floor, 83)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 60, FloorAltitude, 60

'Door 05
i = 84 + (150 * (Floor - 1))
Call Init_Objects(Floor, 84)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, 117

'Door 06
i = 85 + (150 * (Floor - 1))
Call Init_Objects(Floor, 85)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, 101

'Door 07
i = 86 + (150 * (Floor - 1))
Call Init_Objects(Floor, 86)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, 117

'Door 08
i = 87 + (150 * (Floor - 1))
Call Init_Objects(Floor, 87)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, 101

'Door 09
i = 88 + (150 * (Floor - 1))
Call Init_Objects(Floor, 88)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, -117

'Door 10
i = 89 + (150 * (Floor - 1))
Call Init_Objects(Floor, 89)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, -1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorD " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition -12.5, FloorAltitude, -101

'Door 11
i = 90 + (150 * (Floor - 1))
Call Init_Objects(Floor, 90)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, -117

'Door 12
i = 91 + (150 * (Floor - 1))
Call Init_Objects(Floor, 91)
Objects(i).AddWall GetTex("Door1"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
Objects(i).SetMeshName ("DoorB " + Str$(i))
Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetPosition 12.5, FloorAltitude, -101

'Service Door
i = 92 + (150 * (Floor - 1))
Call Init_Objects(Floor, 92)
Objects(i).AddWall GetTex("StairsDoor"), -3.9, 0, 3.9, 0, 19.5, 0, 1, 1
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.45, 0.535, 0.535
'Objects(i).SetTexture GetTex("Wood2")
'Objects(i).SetRotation 0, 1.56, 0
Objects(i).SetMeshName ("DoorA " + Str$(i))
Objects(i).SetPosition 39, FloorAltitude, -46.3

End If

If Floor = 133 Or Floor = 134 Then
i = 79 + (150 * (Floor - 1))
Call Init_Objects(Floor, 79)
Objects(i).Load3DSMesh App.Path + "\objects\swimpool.3ds", , True, , True, True
Objects(i).SetRotation 0, 0, 0
Objects(i).ScaleMesh 0.2, 0.2, 0.2
Objects(i).SetPosition 6.643, -1 + ((134 * FloorHeight) + FloorHeight), 95
'Objects(i).Optimize
End If

If Floor = 135 Or Floor = 136 Then
Floor = 135
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
'Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 59.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 59.5, FloorAltitude - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -59.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -59.5, FloorAltitude - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 53, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 53, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 18, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 62 To 71
'Windows 62 to 71
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window4.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.178, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 53 - ((j - 65) * 12) + ((j - 65) * 0.15), FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 18 - ((j - 65) * 12) + ((j - 65) * 0.15), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

'-----------------------------
Floor = 136
'Window 1
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 59.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 2 to 25
For j = 2 To 25
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition 59.5, FloorAltitude - 0.5, 143 - ((j - 1) * 12) + ((j - 1) * 0.1)
'Objects(i).Optimize
Next j

'Window 26
i = 26 + (150 * (Floor - 1))
Call Init_Objects(Floor, 26)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -59.5, FloorAltitude - 0.5, 143
'Objects(i).Optimize

'Windows 27 to 50
For j = 27 To 50
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetRotation 0, -1.58, 0
'Objects(i).SetPosition 154, FloorAltitude - 0.5, -149.5
Objects(i).SetPosition -59.5, FloorAltitude - 0.5, 143 - ((j - 26) * 12) + ((j - 26) * 0.1)
'Objects(i).Optimize
Next j

'Window 51
i = 51 + (150 * (Floor - 1))
Call Init_Objects(Floor, 51)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 53, FloorAltitude - 0.5, 149.5
'Objects(i).Optimize

For j = 52 To 60
'Windows 52 to 60
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 53 - ((j - 51) * 12) + ((j - 51) * 0.15), FloorAltitude - 0.5, 149.5
'Objects(i).Optimize
Next j

'Window 61
i = 61 + (150 * (Floor - 1))
Call Init_Objects(Floor, 61)
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
'Objects(i).SetPosition 154, FloorAltitude - 0.5, 149.5
Objects(i).SetPosition 18, FloorAltitude - 0.5, -149.5
'Objects(i).Optimize

For j = 62 To 71
'Windows 62 to 71
i = j + (150 * (Floor - 1))
Call Init_Objects(Floor, Int(j))
Objects(i).LoadXFile App.Path + "\objects\window5.x", True
Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.136, 0.136, 0.136
Objects(i).SetTexture GetTex("Ceiling1")
Objects(i).SetPosition 18 - ((j - 65) * 12) + ((j - 65) * 0.15), FloorAltitude - 0.5, -149.5
'Objects(i).Optimize
Next j

End If

If Floor = 137 Then
i = 1 + (150 * (Floor - 1))
Call Init_Objects(Floor, 1)
Objects(i).LoadXFile App.Path + "\objects\piping.x", True
'Objects(i).SetColor (RGBA(10, 10, 10, 1))
Objects(i).ScaleMesh 0.04, 0.04, 0.04
'Objects(i).SetTexture GetTex("ColumnTex")
Objects(i).SetPosition 0, 10 + FloorAltitude, 60
Objects(i).Optimize

End If

If Floor = 138 Then

End If

End Sub


