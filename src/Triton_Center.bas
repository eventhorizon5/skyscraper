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
Private AltitudeCheck As Single
Option Explicit

Public Sub Triton_Globals()
'Process building-wide specific items
CameraDefAltitude = 10
ElevatorShafts = 4
TotalFloors = 138 '139 total floors, 0 =lobby, 1=mezzanine, etc
Basements = 10
Elevators = 40
PipeShafts = 4
StairsNum = 4
ElevatorSpeed = 8
ElevatorFineTuneSpeed = 0.15
StartFloor = 0
End Sub

Public Sub Triton_LoadTextures()
  TextureFactory.LoadTexture App.Path + "\data\brick1.jpg", "BrickTexture"
  TextureFactory.LoadTexture App.Path + "\data\LobbyFront.jpg", "LobbyFront"
  TextureFactory.LoadTexture App.Path + "\data\windows11c.jpg", "MainWindows"
  TextureFactory.LoadTexture App.Path + "\data\granite.jpg", "Granite"
  'TextureFactory.LoadTexture App.Path+"\data\marbl3.jpg", "Marble3"
  'TextureFactory.LoadTexture App.Path+"\data\text12.jpg", "Marble3"
  TextureFactory.LoadTexture App.Path + "\data\symb5.jpg", "Marble3"
  TextureFactory.LoadTexture App.Path + "\data\marb047.jpg", "Marble4"
  TextureFactory.LoadTexture App.Path + "\data\elev1.jpg", "Elev1"
  TextureFactory.LoadTexture App.Path + "\data\textur15.jpg", "Wood1"
  TextureFactory.LoadTexture App.Path + "\data\text16.jpg", "Wood2"
  'TextureFactory.LoadTexture App.Path+"\data\text12.jpg", "Wall1"
  TextureFactory.LoadTexture App.Path + "\data\marbl3.jpg", "Wall1"
  TextureFactory.LoadTexture App.Path + "\data\marb123.jpg", "Wall2"
  'TextureFactory.LoadTexture App.Path+"\data\marbl3.jpg", "Wall2"
  'TextureFactory.LoadTexture App.Path+"\data\marb056.jpg", "Wall3"
  TextureFactory.LoadTexture App.Path + "\data\cutston.jpg", "Ceiling1"
  TextureFactory.LoadTexture App.Path + "\data\text12.jpg", "Wall3"
  TextureFactory.LoadTexture App.Path + "\data\text16.jpg", "ElevDoors"
  TextureFactory.LoadTexture App.Path + "\data\marb148.jpg", "ElevExtPanels"
  TextureFactory.LoadTexture App.Path + "\data\mason01.jpg", "Concrete"
  TextureFactory.LoadTexture App.Path + "\data\text13.jpg", "Stairs"
  TextureFactory.LoadTexture App.Path + "\data\wooddoor3.jpg", "Door1"
  TextureFactory.LoadTexture App.Path + "\data\wooddoor1.jpg", "Door2"
  TextureFactory.LoadTexture App.Path + "\data\servicedoor2.jpg", "StairsDoor"
  TextureFactory.LoadTexture App.Path + "\data\miscdoor8.jpg", "StairsDoor2"
  TextureFactory.LoadTexture App.Path + "\data\button1.jpg", "CallButtonsTex"
  TextureFactory.LoadTexture App.Path + "\data\sidewalk1r.jpg", "Road1"
  TextureFactory.LoadTexture App.Path + "\data\walkway.jpg", "Walkway"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner1.jpg", "Road2"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner2.jpg", "Road3"
  TextureFactory.LoadTexture App.Path + "\data\sidewalkcorner3.jpg", "Road4"
  TextureFactory.LoadTexture App.Path + "\data\roadfull.jpg", "Road5"
  TextureFactory.LoadTexture App.Path + "\data\windows08.jpg", "Windows8"
  TextureFactory.LoadTexture App.Path + "\data\windows11.jpg", "Windows11"
  TextureFactory.LoadTexture App.Path + "\data\downtown.jpg", "Downtown"
  TextureFactory.LoadTexture App.Path + "\data\suburbs.jpg", "Suburbs"
      
  TextureFactory.LoadTexture App.Path + "\data\top.jpg", "SkyTop"
  TextureFactory.LoadTexture App.Path + "\data\bottom.jpg", "SkyBottom"
  TextureFactory.LoadTexture App.Path + "\data\left.jpg", "SkyLeft"
  TextureFactory.LoadTexture App.Path + "\data\right.jpg", "SkyRight"
  TextureFactory.LoadTexture App.Path + "\data\front.jpg", "SkyFront"
  TextureFactory.LoadTexture App.Path + "\data\back.jpg", "SkyBack"
  TextureFactory.LoadTexture App.Path + "\objects\benedeti.jpg", "ColumnTex", , , TV_COLORKEY_NO
  TextureFactory.LoadTexture App.Path + "\data\plaque.jpg", "Plaque"
  TextureFactory.LoadTexture App.Path + "\data\floorsign.jpg", "FloorSign"
  TextureFactory.LoadTexture App.Path + "\data\floorsignballroom.jpg", "FloorSignBallroom"
  TextureFactory.LoadTexture App.Path + "\data\floorsignbalcony.jpg", "FloorSignBalcony"
  TextureFactory.LoadTexture App.Path + "\data\floorsignhotel.jpg", "FloorSignHotel"
  TextureFactory.LoadTexture App.Path + "\data\floorsignlobby.jpg", "FloorSignLobby"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmaint.jpg", "FloorSignMaint"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmez.jpg", "FloorSignMez"
  TextureFactory.LoadTexture App.Path + "\data\floorsignmechanical.jpg", "FloorSignMechanical"
  TextureFactory.LoadTexture App.Path + "\data\floorsignobservatory.jpg", "FloorSignObservatory"
  TextureFactory.LoadTexture App.Path + "\data\floorsignoffices.jpg", "FloorSignOffices"
  TextureFactory.LoadTexture App.Path + "\data\floorsignpool.jpg", "FloorSignPool"
  TextureFactory.LoadTexture App.Path + "\data\floorsignresidential.jpg", "FloorSignResidential"
  TextureFactory.LoadTexture App.Path + "\data\floorsignroof.jpg", "FloorSignRoof"
  TextureFactory.LoadTexture App.Path + "\data\floorsignskylobby.jpg", "FloorSignSkylobby"
  
  'loads all the floor indicator/button pictures
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\L.jpg", "ButtonL"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\M.jpg", "ButtonM"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\R.jpg", "ButtonR"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\open.jpg", "ButtonOpen"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\close.jpg", "ButtonClose"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\stop.jpg", "ButtonStop"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\alarm.jpg", "ButtonAlarm"
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\cancel.jpg", "ButtonCancel"
  Dim i As Integer
  For i = 2 To 138
  DoEvents
  TextureFactory.LoadTexture App.Path + "\data\floorindicators\" + Mid$(Str$(i), 2) + ".jpg", "Button" + Mid$(Str$(i), 2)
  Next i
End Sub

Public Sub Triton_ProcessBasement()
Dim i As Integer
AltitudeCheck = 0

For i = -1 To -10 Step -1
    With Floor(i)
    .FloorHeight = 25
    .CrawlSpaceHeight = 7
    AltitudeCheck = AltitudeCheck - (.FloorHeight + .CrawlSpaceHeight)
    .FloorAltitude = AltitudeCheck
    
    'Floor
    .AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, 0, 0, 0
    .AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, .FloorHeight - 0.5, 0, 0
        
    'Crawlspace bottom
    .AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, .FloorHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, .FloorHeight, 0, 0
    
    'Crawlspace top
    .AddFloor GetTex("BrickTexture"), -160, -150, 160, -46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, 46.25, 160, 150, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -90.5, -46.25, -52.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), 52.5, -46.25, 90.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -12.5, -46.25, 12.5, 0, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -52.5, 0, 52.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), -160, -46.25, -130.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor GetTex("BrickTexture"), 160, -46.25, 130.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    
    'Crawlspace walls
    .AddWall GetTex("BrickTexture"), -160, -150, 160, -150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall GetTex("BrickTexture"), 160, -150, 160, 150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall GetTex("BrickTexture"), 160, 150, -160, 150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall GetTex("BrickTexture"), -160, 150, -160, -150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0

    'Level Walls
    .AddWall GetTex("BrickTexture"), -160, -150, 160, -150, 0, .FloorHeight, 0, 0
    .AddWall GetTex("BrickTexture"), 160, -150, 160, 150, 0, .FloorHeight, 0, 0
    .AddWall GetTex("BrickTexture"), 160, 150, -160, 150, 0, .FloorHeight, 0, 0
    .AddWall GetTex("BrickTexture"), -160, 150, -160, -150, 0, .FloorHeight, 0, 0
    
    'Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 4, True, False, False, False, False, False, False, False, False, False, False)
    
    End With
    Next i
End Sub

Public Sub Triton_ProcessFloors()
Dim i As Integer
AltitudeCheck = 0
'Lobby
i = 0
With Floor(i)
.FloorHeight = 32
.CrawlSpaceHeight = 0
.FloorAltitude = AltitudeCheck
AltitudeCheck = AltitudeCheck + .FloorHeight + .CrawlSpaceHeight

    'Floor
    '.AddFloor "Marble4", -160, -150, 160, 150, 0, 0, 0
    .AddFloor "Marble4", -160, -150, 160, -46.25, 0, 0, 0
    .AddFloor "Marble4", -160, 46.25, 160, 150, 0, 0, 0
    .AddFloor "Marble4", -90.5, -46.25, -52.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", 52.5, -46.25, 90.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", -12.5, -46.25, 12.5, 0, 0, 0, 0
    .AddFloor "Marble4", -52.5, 0, 52.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", -160, -46.25, -130.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", 160, -46.25, 130.5, 46.25, 0, 0, 0
    
    'Call DrawElevatorWalls(i, 5, 1, True, True, True, True, True, False, False, False, False, False, False)
    'Call DrawElevatorWalls(i, 5, 2, True, True, True, True, True, False, False, False, False, False, False)
    'Call DrawElevatorWalls(i, 1, 3, True, True, True, True, True, True, True, True, True, True, True)
    'Call DrawElevatorWalls(i, 1, 4, True, True, True, True, True, True, True, True, True, True, True)
    
    'Ceiling
    .AddFloor "Ceiling1", -160, -150, 160, -46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -160, 46.25, 160, 150, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -90.5, -46.25, -52.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", 52.5, -46.25, 90.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -12.5, -46.25, 12.5, 0, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -52.5, 0, 52.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -160, -46.25, -130.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", 160, -46.25, 130.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    
    'Lobby Front
    .AddWall "LobbyFront", -160 + 0.1, -150 + 0.1, 160 - 0.1, -150 + 0.1, (.FloorHeight * 3), 0, 0, 0
    .AddWall "LobbyFront", 160 - 0.1, -150 + 0.1, 160 - 0.1, 150 - 0.1, (.FloorHeight * 3), 0, 0, 0
    .AddWall "LobbyFront", 160 - 0.1, 150 - 0.1, -160 + 0.1, 150 - 0.1, (.FloorHeight * 3), 0, 0, 0
    .AddWall "LobbyFront", -160 + 0.1, 150 - 0.1, -160 + 0.1, -150 + 0.1, (.FloorHeight * 3), 0, 0, 0
End With

'mezzanine
i = 1
With Floor(i)
.FloorHeight = 64
.CrawlSpaceHeight = 0
.FloorAltitude = AltitudeCheck
AltitudeCheck = AltitudeCheck + .FloorHeight + .CrawlSpaceHeight
    .AddFloor "Granite", -90.5, -55, 90.5, -46.25, 0, 0, 0
    .AddFloor "Granite", -90.5, 0, 90.5, 55, 0, 0, 0
    .AddFloor "Granite", -90.5, -46.25, -52.5, 0, 0, 0, 0
    .AddFloor "Granite", 52.5, -46.25, 90.5, 0, 0, 0, 0
    .AddFloor "Granite", -12.5, -46.25, 12.5, 0, 0, 0, 0
    
    .AddFloor "Ceiling1", -90.5, -55, 90.5, -46.25, -0.05, 0, 0
    .AddFloor "Ceiling1", -90.5, 0, 90.5, 55, -0.05, 0, 0
    .AddFloor "Ceiling1", -90.5, -46.25, -52.5, 0, -0.05, 0, 0
    .AddFloor "Ceiling1", 52.5, -46.25, 90.5, 0, -0.05, 0, 0
    .AddFloor "Ceiling1", -12.5, -46.25, 12.5, 0, -0.05, 0, 0
End With

'floors 2 to 39
For i = 2 To 39
With Floor(i)
DoEvents
.FloorHeight = 25
.CrawlSpaceHeight = 7
.FloorAltitude = AltitudeCheck
AltitudeCheck = AltitudeCheck + .FloorHeight + .CrawlSpaceHeight

    'Floor
    .AddFloor "Granite", -160, -150, 160, -46.25, 0, 0, 0
    .AddFloor "Granite", -160, 46.25, 160, 150, 0, 0, 0
    .AddFloor "Granite", -90.5, -46.25, -52.5, 46.25, 0, 0, 0
    .AddFloor "Granite", 52.5, -46.25, 90.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -12.5, -46.25, 12.5, 0, 0, 0, 0
    .AddFloor "Granite", -52.5, 0, 52.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -160, -46.25, -130.5, 46.25, 0, 0, 0
    .AddFloor "Granite", 160, -46.25, 130.5, 46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "Marble3", -160, -150, 160, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -160, 46.25, 160, 150, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -90.5, -46.25, -52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 52.5, -46.25, 90.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -12.5, -46.25, 12.5, 0, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -52.5, 0, 52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -160, -46.25, -130.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 160, -46.25, 130.5, 46.25, .FloorHeight - 0.5, 0, 0
    
    'Crawlspace bottom
    .AddCrawlSpaceFloor "BrickTexture", -160, -150, 160, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, 46.25, 160, 150, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -90.5, -46.25, -52.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 52.5, -46.25, 90.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -12.5, -46.25, 12.5, 0, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 0, 52.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, -130.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 160, -46.25, 130.5, 46.25, 0, 0, 0
    
    'Crawlspace top
    .AddCrawlSpaceFloor "BrickTexture", -160, -150, 160, -46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, 46.25, 160, 150, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -90.5, -46.25, -52.5, 46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 52.5, -46.25, 90.5, 46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -12.5, -46.25, 12.5, 0, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 0, 52.5, 46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, -130.5, 46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 160, -46.25, 130.5, 46.25, .CrawlSpaceHeight, 0, 0
    
    'Crawlspace walls
    .AddCrawlSpaceWall "BrickTexture", -160, -150, 160, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", 160, -150, 160, 150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", 160, 150, -160, 150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -160, 150, -160, -150, .CrawlSpaceHeight, 0, 0, 0

    'If i = 2 Or i = 39 Then
    'Call DrawElevatorWalls(Int(i), 5, 1, True, True, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 4, True, True, True, True, True, True, True, True, True, True, True)
    'End If
    'If i <> 2 And i <> 39 Then
    'Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 3, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 4, True, True, True, True, True, True, True, True, True, True, True)
    'End If
    
    'Room(I) Walls
    'top walls
    .AddWall "Wall3", -160, -71.3, -90.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -71.3, 70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 160, -71.3, 90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -160, -71.3, -90.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, -71.3, 70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 160, -71.3, 90.5, -71.3, 5.5, 19.5, 0, 0
    
    'bottom walls
    .AddWall "Wall3", -160, 71.3, -90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 71.3, 70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 160, 71.3, 90.5, 71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -160, 71.3, -90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, 71.3, 70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 160, 71.3, 90.5, 71.3, 5.5, 19.5, 0, 0
    
    'top middle walls
    .AddWall "Wall3", -70, -46.25, -61.25 - 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -61.25 + 3.9, -46.25, -52.5, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -46.25, 61.25 + 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 61.25 - 3.9, -46.25, 52.5, -46.25, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -70, -46.25, -52.5, -46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 52.5, -46.25, 70, -46.25, 5.5, 19.5, 0, 0
    
    'service rooms
    .AddWall "Wall3", 70, -46.25, 70, 46.25, 25, 0, 0, 0
    .AddWall "Wall3", 70, -15, 52.5, -15, 25, 0, 0, 0
    
    .AddWall "Wall3", -70, -46.25, -70, 46.25, 25, 0, 0, 0
    .AddWall "Wall3", -70, -15, -52.5, -15, 25, 0, 0, 0
    
    'left hallway
    .AddWall "Wall3", 70, -150, 70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -130 + 3.9, 70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -90 + 3.9, 70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -90 + 3.9, 90.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 150, 70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 130 - 3.9, 70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 90 - 3.9, 70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 90 - 3.9, 90.5, 71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, -150, 70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, 150, 70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 71.3, 5.5, 19.5, 0, 0
    
    'right hallway
    .AddWall "Wall3", -70, -150, -70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -130 + 3.9, -70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -90 + 3.9, -70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -90 + 3.9, -90.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 150, -70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 130 - 3.9, -70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 90 - 3.9, -70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 90 - 3.9, -90.5, 71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -70, -150, -70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, 150, -70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 71.3, 5.5, 19.5, 0, 0
    
    'middle hallway extension
    .AddWall "Wall3", -12.5, 0, -12.5, 46.25, 25, 0, 0, 0
    .AddWall "Wall3", 12.5, 0, 12.5, 46.25, 25, 0, 0, 0
    
    'bottom middle walls
    .AddWall "Wall3", -70, 46.25, -22.5 - 3.9, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -22.5 + 3.9, 46.25, -12.5, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 46.25, 22.5 + 3.9, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 22.5 - 3.9, 46.25, 12.5, 46.25, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -70, 46.25, -12.5, 46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 12.5, 46.25, 70, 46.25, 5.5, 19.5, 0, 0
    
    'Rooms
    .AddWall "Wall3", -160, -110, -90.5, -110, 25, 0, 0, 0
    .AddWall "Wall3", -160, 110, -90.5, 110, 25, 0, 0, 0
    .AddWall "Wall3", -70, -110, 70, -110, 25, 0, 0, 0
    .AddWall "Wall3", -70, 110, 70, 110, 25, 0, 0, 0
    .AddWall "Wall3", 160, -110, 90.5, -110, 25, 0, 0, 0
    .AddWall "Wall3", 160, 110, 90.5, 110, 25, 0, 0, 0
    
    .AddWall "Wall3", 0, -71.3, 0, -150, 25, 0, 0, 0
    .AddWall "Wall3", 0, 71.3, 0, 150, 25, 0, 0, 0
        
    End With
Next i

End Sub
