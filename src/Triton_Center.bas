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
'This module is temporary, since the Triton Center-specific data
'will be exported into a data file

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
CameraStartFloor = 0
CameraStartPositionX = 0
CameraStartPositionZ = -130
CameraStartDirection = Vector(0, 10, 90)
CameraStartRotation = Vector(0, 0, 0)
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
    .AddFloor "BrickTexture", -160, 150, 160, 46.25, 0, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, 160, -150, 0, 0, 0
    .AddFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, 0, 0, 0
    .AddFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, 0, 0, 0
    .AddFloor "BrickTexture", -12.5, 46.25, 12.5, 0, 0, 0, 0
    .AddFloor "BrickTexture", -52.5, 0, 52.5, -46.25, 0, 0, 0
    .AddFloor "BrickTexture", 160, -46.25, 130.5, 46.25, 0, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, -130.5, 46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "BrickTexture", -160, 150, 160, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, 160, -150, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", -12.5, 46.25, 12.5, 0, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", -52.5, 0, 52.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", 160, -46.25, 130.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, -130.5, 46.25, .FloorHeight - 0.5, 0, 0
        
    'Crawlspace bottom
    .AddFloor "BrickTexture", -160, 150, 160, 46.25, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, 160, -150, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", -12.5, 46.25, 12.5, 0, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", -52.5, 0, 52.5, -46.25, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", 160, -46.25, 130.5, 46.25, .FloorHeight, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, -130.5, 46.25, .FloorHeight, 0, 0
    
    'Crawlspace top
    .AddFloor "BrickTexture", -160, 150, 160, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, 160, -150, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", -12.5, 46.25, 12.5, 0, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", -52.5, 0, 52.5, -46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", 160, -46.25, 130.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddFloor "BrickTexture", -160, -46.25, -130.5, 46.25, .FloorHeight + .CrawlSpaceHeight, 0, 0
    
    'Crawlspace walls
    .AddWall "BrickTexture", -160, -150, 160, -150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall "BrickTexture", 160, -150, 160, 150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall "BrickTexture", 160, 150, -160, 150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0
    .AddWall "BrickTexture", -160, 150, -160, -150, .FloorHeight, .FloorHeight + .CrawlSpaceHeight, 0, 0

    'Level Walls
    .AddWall "BrickTexture", -160, -150, 160, -150, 0, .FloorHeight, 0, 0
    .AddWall "BrickTexture", 160, -150, 160, 150, 0, .FloorHeight, 0, 0
    .AddWall "BrickTexture", 160, 150, -160, 150, 0, .FloorHeight, 0, 0
    .AddWall "BrickTexture", -160, 150, -160, -150, 0, .FloorHeight, 0, 0
    
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
    .AddFloor "Marble4", -160, 150, 160, 46.25, 0, 0, 0
    .AddFloor "Marble4", -160, -46.25, 160, -150, 0, 0, 0
    .AddFloor "Marble4", 90.5, -46.25, 52.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", -52.5, -46.25, -90.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", -12.5, 46.25, 12.5, 0, 0, 0, 0
    .AddFloor "Marble4", -52.5, 0, 52.5, -46.25, 0, 0, 0
    .AddFloor "Marble4", 160, -46.25, 130.5, 46.25, 0, 0, 0
    .AddFloor "Marble4", -160, -46.25, -130.5, 46.25, 0, 0, 0
    
    'Call DrawElevatorWalls(i, 5, 1, True, True, True, True, True, False, False, False, False, False, False)
    'Call DrawElevatorWalls(i, 5, 2, True, True, True, True, True, False, False, False, False, False, False)
    'Call DrawElevatorWalls(i, 1, 3, True, True, True, True, True, True, True, True, True, True, True)
    'Call DrawElevatorWalls(i, 1, 4, True, True, True, True, True, True, True, True, True, True, True)
    
    'Ceiling
    .AddFloor "Ceiling1", -160, 150, 160, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -160, -46.25, 160, -150, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", 90.5, -46.25, 52.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -52.5, -46.25, -90.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -12.5, 46.25, 12.5, 0, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -52.5, 0, 52.5, -46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", 160, -46.25, 130.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    .AddFloor "Ceiling1", -160, -46.25, -130.5, 46.25, (.FloorHeight * 3) - 0.5, 0, 0
    
    'Lobby Front
    .AddWall "LobbyFront", -160 + 0.1, -150 + 0.1, 160 - 0.1, -150 + 0.1, (.FloorHeight * 3), 0, 3, 1
    .AddWall "LobbyFront", 160 - 0.1, -150 + 0.1, 160 - 0.1, 150 - 0.1, (.FloorHeight * 3), 0, 3, 1
    .AddWall "LobbyFront", 160 - 0.1, 150 - 0.1, -160 + 0.1, 150 - 0.1, (.FloorHeight * 3), 0, 3, 1
    .AddWall "LobbyFront", -160 + 0.1, 150 - 0.1, -160 + 0.1, -150 + 0.1, (.FloorHeight * 3), 0, 3, 1
End With

'mezzanine
i = 1
With Floor(i)
.FloorHeight = 64
.CrawlSpaceHeight = 0
.FloorAltitude = AltitudeCheck
AltitudeCheck = AltitudeCheck + .FloorHeight + .CrawlSpaceHeight
    .AddFloor "Granite", -90.5, 55, 90.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -90.5, 0, 90.5, -55, 0, 0, 0
    .AddFloor "Granite", 90.5, 46.25, 52.5, 0, 0, 0, 0
    .AddFloor "Granite", -52.5, 46.25, -90.5, 0, 0, 0, 0
    .AddFloor "Granite", -12.5, 46.25, 12.5, 0, 0, 0, 0
    
    .AddFloor "Ceiling1", -90.5, 55, 90.5, 46.25, -0.05, 0, 0
    .AddFloor "Ceiling1", -90.5, 0, 90.5, -55, -0.05, 0, 0
    .AddFloor "Ceiling1", 90.5, 46.25, 52.5, 0, -0.05, 0, 0
    .AddFloor "Ceiling1", -52.5, 46.25, -90.5, 0, -0.05, 0, 0
    .AddFloor "Ceiling1", -12.5, 46.25, 12.5, 0, -0.05, 0, 0
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
    .AddFloor "Granite", -160, 150, 160, 46.25, 0, 0, 0
    .AddFloor "Granite", -160, -46.25, 160, -150, 0, 0, 0
    .AddFloor "Granite", 90.5, -46.25, 52.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -52.5, -46.25, -90.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -12.5, 46.25, 12.5, 0, 0, 0, 0
    .AddFloor "Granite", -52.5, 0, 52.5, -46.25, 0, 0, 0
    .AddFloor "Granite", 160, -46.25, 130.5, 46.25, 0, 0, 0
    .AddFloor "Granite", -160, -46.25, -130.5, 46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "Marble3", -160, 150, 160, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -160, -46.25, 160, -150, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 90.5, -46.25, 52.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -52.5, -46.25, -90.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -12.5, 46.25, 12.5, 0, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -52.5, 0, 52.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 160, -46.25, 130.5, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -160, -46.25, -130.5, 46.25, .FloorHeight - 0.5, 0, 0
    
    'Crawlspace bottom
    .AddCrawlSpaceFloor "BrickTexture", -160, 150, 160, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, 160, -150, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -12.5, 46.25, 12.5, 0, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 0, 52.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 160, -46.25, 130.5, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, -130.5, 46.25, 0, 0, 0
    
    'Crawlspace top
    .AddCrawlSpaceFloor "BrickTexture", -160, 150, 160, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, 160, -150, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 90.5, -46.25, 52.5, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, -46.25, -90.5, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -12.5, 46.25, 12.5, 0, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 0, 52.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 160, -46.25, 130.5, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -160, -46.25, -130.5, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    
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
    .AddWall "Wall3", 160, 71.3, 90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 71.3, -70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -160, 71.3, -90.5, 71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 160, 71.3, 90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, 71.3, -70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -160, 71.3, -90.5, 71.3, 5.5, 19.5, 0, 0
    
    'bottom walls
    .AddWall "Wall3", 160, -71.3, 90.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -71.3, -70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -160, -71.3, -90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 160, -71.3, 90.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, -71.3, -70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -160, -71.3, -90.5, -71.3, 5.5, 19.5, 0, 0
    
    'top middle walls
    .AddWall "Wall3", 70, 46.25, 61.25 + 3.9, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 61.25 - 3.9, 46.25, 52.5, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 46.25, -61.25 - 3.9, 46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -61.25 + 3.9, 46.25, -52.5, 46.25, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, 46.25, 52.5, 46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -52.5, 46.25, -70, 46.25, 5.5, 19.5, 0, 0
    
    'service rooms
    .AddWall "Wall3", -70, 46.25, -70, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -70, 15, -52.5, 15, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 70, 46.25, 70, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, 15, 52.5, 15, .FloorHeight, 0, 0, 0
    
    'left hallway
    .AddWall "Wall3", -70, 150, -70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 130 - 3.9, -70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 90 - 3.9, -70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 90 - 3.9, -90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -150, -70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -130 + 3.9, -70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -90 + 3.9, -70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -90 + 3.9, -90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -70, 150, -70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, -150, -70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -71.3, 5.5, 19.5, 0, 0
    
    'right hallway
    .AddWall "Wall3", 70, 150, 70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 130 - 3.9, 70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 90 - 3.9, 70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 90 - 3.9, 90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -150, 70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -130 + 3.9, 70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -90 + 3.9, 70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -90 + 3.9, 90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, 150, 70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, -150, 70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -71.3, 5.5, 19.5, 0, 0
    
    'middle hallway extension
    .AddWall "Wall3", 12.5, 0, 12.5, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -12.5, 0, -12.5, -46.25, .FloorHeight, 0, 0, 0
    
    'bottom middle walls
    .AddWall "Wall3", 70, -46.25, 22.5 + 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 22.5 - 3.9, -46.25, 12.5, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -46.25, -22.5 - 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -22.5 + 3.9, -46.25, -12.5, -46.25, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, -46.25, 12.5, -46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, -46.25, -70, -46.25, 5.5, 19.5, 0, 0
    
    'Rooms
    .AddWall "Wall3", 160, 110, 90.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 160, -110, 90.5, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, 110, -70, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, -110, -70, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -160, 110, -90.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -160, -110, -90.5, -110, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 0, 71.3, 0, 150, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 0, -71.3, 0, -150, .FloorHeight, 0, 0, 0
        
    End With
Next i
    
For i = 41 To 79
    DoEvents
    With Floor(i)
    
    'Floor
    .AddFloor "Granite", 135, 150, -135, 46.25, 0, 0, 0
    .AddFloor "Granite", 135, -46.25, -135, -150, 0, 0, 0
    .AddFloor "Granite", 90.5, 46.25, 52.5, -46.25, 0, 0, 0
    .AddFloor "Granite", -52.5, 46.25, -90.5, -46.25, 0, 0, 0
    .AddFloor "Granite", 12.5, 46.25, -12.5, 0, 0, 0, 0
    .AddFloor "Granite", 52.5, 0, 52.5, 46.25, 0, 0, 0
    .AddFloor "Granite", 135, 46.25, 110.5, -46.25, 0, 0, 0
    .AddFloor "Granite", -135, 46.25, -110.5, -46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "Marble3", 135, 150, -135, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 135, -46.25, -135, -150, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 90.5, 46.25, 52.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -52.5, 46.25, -90.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 12.5, 46.25, -12.5, 0, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 52.5, 0, -52.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 135, 46.25, 110.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -135, 46.25, -110.5, -46.25, .FloorHeight - 0.5, 0, 0
    
    'Crawlspace bottom
    .AddCrawlSpaceFloor "BrickTexture", 135, 150, -135, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 135, -46.25, -135, -150, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 90.5, 46.25, 52.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 46.25, -90.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 12.5, 46.25, -12.5, 0, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 52.5, 0, -52.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 135, 46.25, 110.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -135, 46.25, -110.5, -46.25, 0, 0, 0
    
    'Crawlspace top
    .AddCrawlSpaceFloor "BrickTexture", 135, 150, -135, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 135, -46.25, -135, -150, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 90.5, 46.25, 52.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -52.5, 46.25, -90.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 12.5, 46.25, -12.5, 0, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 52.5, 0, -52.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 135, 46.25, 110.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -135, 46.25, -110.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    
    'Crawlspace walls
    .AddCrawlSpaceWall "BrickTexture", 135, 150, -135, 150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -135, 150, -135, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -135, -150, 135, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", 135, -150, 135, 150, .CrawlSpaceHeight, 0, 0, 0

    'If i = 79 Then
    'Call DrawElevatorWalls(Int(i), 5, 1, True, True, False, False, False, False, False, False, False, False, False)
    ''Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 3, False, True, True, True, True, True, True, True, True, True, True)
    'End If
    'If i <> 79 Then
    'Call DrawElevatorWalls(Int(i), 5, 1, True, False, False, False, False, False, False, False, False, False, False)
    ''Call DrawElevatorWalls(Int(i), 5, 2, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 3, False, True, True, True, True, True, True, True, True, True, True)
    'End If
    'If i <= 51 Then
    'Call DrawElevatorWalls(Int(i), 5, 2, True, True, False, False, False, False, False, False, False, False, False)
    'End If
    'If i > 51 Then
    'Call DrawElevatorWalls(Int(i), 5, 2, True, False, True, False, False, False, False, False, False, False, False)
    'End If
    
    'Room(I) Walls
    
    'top walls
    .AddWall "Wall3", 135, 71.3, 90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 71.3, -70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -135, 71.3, -90.5, 71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 135, 71.3, 90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, 71.3, -70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -135, 71.3, -90.5, 71.3, 5.5, 19.5, 0, 0
    
    'bottom walls
    .AddWall "Wall3", 135, -71.3, 90.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -71.3, -70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -135, -71.3, -90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 135, -71.3, 90.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, -71.3, -70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -135, -71.3, -90.5, -71.3, 5.5, 19.5, 0, 0
    
    .AddWall "Wall3", 70, 46.25, 61.25 + 3.9, 46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 61.25 - 3.9, 46.25, 52.5, 46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -70, 46.25, -61.25 - 3.9, 46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -61.25 + 3.9, 46.25, -52.5, 46.25, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 70, 46.25, 52.5, 46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -52.5, 46.25, -70, 46.25, 5.5, 19.5, 0, 0
    
    'service rooms
    .AddWall "Wall3", -70, 46.25, -70, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -70, 15, -52.5, 15, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 70, 46.25, 70, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, 15, 52.5, 15, .FloorHeight, 0, 0, 0
    
    'left hallway
    .AddWall "Wall3", -70, 150, -70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 130 - 3.9, -70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 90 - 3.9, -70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 130 - 3.9, -90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, 90 - 3.9, -90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -150, -70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -130 + 3.9, -70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -90 + 3.9, -70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -130 + 3.9, -90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -90.5, -90 + 3.9, -90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", -70, 150, -70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, 150, -90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, -150, -70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -90.5, -150, -90.5, -71.3, 5.5, 19.5, 0, 0
    
    'right hallway
    .AddWall "Wall3", 70, 150, 70, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 130 - 3.9, 70, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 90 - 3.9, 70, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 130 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 130 - 3.9, 90.5, 90 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, 90 - 3.9, 90.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -150, 70, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -130 + 3.9, 70, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -90 + 3.9, 70, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -130 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -130 + 3.9, 90.5, -90 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 90.5, -90 + 3.9, 90.5, -71.3, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, 150, 70, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, 150, 90.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, -150, 70, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 90.5, -150, 90.5, -71.3, 5.5, 19.5, 0, 0
    
    'middle hallway extension
    .AddWall "Wall3", 12.5, 0, 12.5, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -12.5, 0, -12.5, -46.25, .FloorHeight, 0, 0, 0
    
    'bottom middle walls
    .AddWall "Wall3", 70, -46.25, 22.5 + 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", 22.5 - 3.9, -46.25, 12.5, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -46.25, -22.5 - 3.9, -46.25, 19.5, 0, 0, 0
    .AddWall "Wall3", -22.5 + 3.9, -46.25, -12.5, -46.25, 19.5, 0, 0, 0
    
    .AddWall "Wall3", 70, -46.25, 12.5, -46.25, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, -46.25, -70, -46.25, 5.5, 19.5, 0, 0
    
    'Rooms
    .AddWall "Wall3", 135, 110, 90.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 135, -110, 90.5, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, 110, -70, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 70, -110, -70, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -135, 110, -90.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -135, -110, -90.5, -110, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 0, -71.3, 0, -150, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 0, 71.3, 0, 150, .FloorHeight, 0, 0, 0
    
    End With
Next i

For i = 81 To 114
    DoEvents
    With Floor(i)
    
        'Floor
    .AddFloor "Granite", 110, 150, -110, 46.25, 0, 0, 0
    .AddFloor "Granite", 110, -46.25, -110, -150, 0, 0, 0
    .AddFloor "Granite", 12.5, 46.25, -12.5, -46.25, 0, 0, 0
    .AddFloor "Granite", 110, 46.25, 52.5, -46.25, 0, 0, 0
    .AddFloor "Granite", -110, 46.25, -52.5, -46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "Marble3", 110, 150, -110, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 110, -46.25, -110, -150, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 12.5, 46.25, -12.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 110, 46.25, 52.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -110, 46.25, -52.5, -46.25, .FloorHeight - 0.5, 0, 0
    
    'Crawlspace bottom
    .AddCrawlSpaceFloor "Granite", 110, 150, -110, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "Granite", 110, -46.25, -110, -150, 0, 0, 0
    .AddCrawlSpaceFloor "Granite", 12.5, 46.25, -12.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "Granite", 110, 46.25, 52.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "Granite", -110, 46.25, -52.5, -46.25, 0, 0, 0
    
    'Crawlspace top
    .AddCrawlSpaceFloor "BrickTexture", 110, 150, -110, 46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 110, -46.25, -110, -150, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 12.5, 46.25, -12.5, -46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 110, 46.25, 52.5, -46.25, .CrawlSpaceHeight, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -110, 46.25, -52.5, -46.25, .CrawlSpaceHeight, 0, 0
    
    'Crawlspace walls
    .AddCrawlSpaceWall "BrickTexture", 110, 150, -110, 150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -110, 150, -110, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -110, -150, 110, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", 110, -150, 110, 150, .CrawlSpaceHeight, 0, 0, 0

    'If i >= 82 And i <= 98 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, True, True, True, True, True, True)
    'End If
    'If i >= 102 And i < 114 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, False, False, False, False, False, False, False, False, False)
    'End If
    'If i = 81 Or i = 99 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, False, False, False, False, False, False)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, True, True, True, True, True, True)
    'End If
    'If i = 100 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, False, False, False, False, True, True, True, True, True, True)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, False, False, False, False, False, False)
    'End If
    'If i = 114 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, False, False, False, False, False, False, False, False, False)
    'End If
    'If i = 101 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, True, True, False, False, False, True, True, True, True, True, True)
    'Call DrawElevatorWalls(Int(i), 2, 2, True, False, True, False, False, False, False, False, False, False, False)
    'End If
    
    'Right Hallways, Right Wall
    .AddWall "Wall3", 70, 150, 70, -150, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 70, 150, 70, 118 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 118 - 3.9, 70, 102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 102 - 3.9, 70, 60 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, 60 - 3.9, 70, -27 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -27 - 3.9, 70, -66 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -66 - 3.9, 70, -102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -102 - 3.9, 70, -116 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 70, -116 - 3.9, 70, -150, 19.5, 0, 0, 0
    
    'Left Hallways, Left Wall
    .AddWall "Wall3", -70, 150, -70, -150, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -70, 150, -70, 118 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 118 - 3.9, -70, 102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 102 - 3.9, -70, 60 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, 60 - 3.9, -70, -27 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -27 - 3.9, -70, -66 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -66 - 3.9, -70, -102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -102 - 3.9, -70, -116 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -70, -116 - 3.9, -70, -150, 19.5, 0, 0, 0
    
    'center bottom hallway, right wall
    .AddWall "Wall3", 12.5, -150, 12.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 12.5, -150, 12.5, -118 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, -118 + 3.9, 12.5, -102 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, -102 + 3.9, 12.5, -71.3, 19.5, 0, 0, 0
    
    'center bottom hallway, left wall
    .AddWall "Wall3", -12.5, -150, -12.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, -150, -12.5, -118 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, -118 + 3.9, -12.5, -102 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, -102 + 3.9, -12.5, -71.3, 19.5, 0, 0, 0
    
    'center top hallway, right wall
    .AddWall "Wall3", 12.5, 150, 12.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 12.5, 150, 12.5, 118 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, 118 - 3.9, 12.5, 102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, 102 - 3.9, 12.5, 71.3, 19.5, 0, 0, 0
    
    'center top hallway, left wall
    .AddWall "Wall3", -12.5, 150, -12.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, 150, -12.5, 118 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, 118 - 3.9, -12.5, 102 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, 102 - 3.9, -12.5, 71.3, 19.5, 0, 0, 0
    
    'Individual Rooms
    .AddWall "Wall3", 110, -110, 70, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 50, -110, 12.5, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, -110, -70, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -50, -110, -12.5, -110, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 110, -71.3, 70, -71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 50, -71.3, 12.5, -71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, -71.3, -70, -71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -12.5, -71.3, -50, -71.3, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 110, 110, 70, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 50, 110, 12.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, 110, -70, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -50, 110, -12.5, 110, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 110, 71.3, 70, 71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 50, 71.3, 12.5, 71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, 71.3, -70, 71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -12.5, 71.3, -50, 71.3, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 110, -46.25, 70, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, -46.25, -70, -46.25, .FloorHeight, 0, 0, 0
    
    'Room(i).AddWall "Wall3", 110, -20, 70, -20, .FloorHeight, 0, 0, 0
    'Room(i).AddWall "Wall3", -110, -20, -70, -20, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 110, 5, 70, 5, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -110, 5, -70, 5, .FloorHeight, 0, 0, 0
    
    'Left Bottom Hallway, Right Wall (no doors)
    .AddWall "Wall3", -50, -71.3, -50, -150, .FloorHeight, 0, 0, 0
    
    'Right Bottom Hallway, Left Wall (no doors)
    .AddWall "Wall3", 50, -71.3, 50, -150, .FloorHeight, 0, 0, 0
    
    'Left Top Hallway, Right Wall (no doors)
    .AddWall "Wall3", -50, 71.3, -50, 150, .FloorHeight, 0, 0, 0
    
    'Right Top Hallway, Left Wall (no doors)
    .AddWall "Wall3", 50, 71.3, 50, 150, .FloorHeight, 0, 0, 0
        
    End With
    Next i

For i = 118 To 129
    DoEvents
    With Floor(i)
    
    'Floor
    .AddFloor "Granite", 85, 150, -85, 46.25, 0, 0, 0
    .AddFloor "Granite", 85, -46.25, -85, -150, 0, 0, 0
    .AddFloor "Granite", 12.5, 46.25, -12.5, -46.25, 0, 0, 0
    .AddFloor "Granite", 85, 46.25, 32.5, -46.25, 0, 0, 0
    .AddFloor "Granite", -85, 46.25, -32.5, -46.25, 0, 0, 0
    
    'Ceiling
    .AddFloor "Marble3", 85, 150, -85, 46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 85, -46.25, -85, -150, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 12.5, 46.25, -12.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", 85, 46.25, 32.5, -46.25, .FloorHeight - 0.5, 0, 0
    .AddFloor "Marble3", -85, 46.25, -32.5, -46.25, .FloorHeight - 0.5, 0, 0
    
    'Crawlspace bottom
    .AddCrawlSpaceFloor "BrickTexture", 85, 150, -85, 46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 85, -46.25, -85, -150, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 12.5, 46.25, -12.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 85, 46.25, 32.5, -46.25, 0, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -85, 46.25, -32.5, -46.25, 0, 0, 0
    
    'Crawlspace top
    .AddCrawlSpaceFloor "BrickTexture", 85, 150, -85, 46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 85, -46.25, -85, -150, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 12.5, 46.25, -12.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", 85, 46.25, 32.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    .AddCrawlSpaceFloor "BrickTexture", -85, 46.25, -32.5, -46.25, .CrawlSpaceHeight - 0.05, 0, 0
    
    'Crawlspace walls
    .AddCrawlSpaceWall "BrickTexture", 85, 150, -85, 150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -85, 150, -85, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", -85, -150, 85, -150, .CrawlSpaceHeight, 0, 0, 0
    .AddCrawlSpaceWall "BrickTexture", 85, -150, 85, 150, .CrawlSpaceHeight, 0, 0, 0

    ''If i = 118 Or i = 129 Then
    'Call DrawElevatorWalls(Int(i), 2, 1, False, True, False, False, False, True, True, True, True, True, True)
    ''End If
    ''If i >= 119 And i <= 128 Then
    ''Call DrawElevatorWalls(Int(i), 2, 1, False, False, False, False, False, True, True, True, True, True, True)
    ''End If
    
    'Room(I) Walls
    .AddWall "Wall3", 60, 71.3, 12.5, 71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -12.5, 71.3, -60, 71.3, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 60, -46.25, 32.5, -46.25, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -32.5, -46.25, -60, -46.25, .FloorHeight, 0, 0, 0
    
    '.AddWall "Wall3", 60, 0, 32.5, 0, 25, 0, 0, 0
    '.AddWall "Wall3", -60, 0, -32.5, 0, 25, 0, 0, 0
    
    .AddWall "Wall3", 60, 46.3, 32.5, 46.3, .FloorHeight, 0, 0, 0
    'This wall is cut for the service door
    .AddWall "Wall3", -60, 46.3, -32.5, 46.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -60, 46.3, -39 - 3.9, 46.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -39 + 3.9, 46.3, 32.5, 46.3, 19.5, 0, 0, 0
        
    'Top Horizontal Hallway End Walls
    .AddWall "Wall3", 60, 71.3, 60, 46.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 60, 71.3, 60, 60 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 60, 60 - 3.9, 60, 46.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -60, 71.3, -60, 46.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -60, 71.3, -60, 60 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -60, 60 - 3.9, -60, 46.3, 19.5, 0, 0, 0
    
    'Bottom Horizontal Hallway End Walls
    .AddWall "Wall3", 60, -71.3, 60, -46.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 60, -71.3, 60, -60 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 60, -60 + 3.9, 60, -46.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -60, -71.3, -60, -46.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -60, -71.3, -60, -60 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -60, -60 + 3.9, -60, -46.3, 19.5, 0, 0, 0
    
    'Bottom Hallway
    .AddWall "Wall3", 12.5, -150, 12.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 12.5, -150, 12.5, -117 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, -117 + 3.9, 12.5, -101 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, -101 + 3.9, 12.5, -71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, -150, -12.5, -71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, -150, -12.5, -117 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, -117 + 3.9, -12.5, -101 - 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, -101 + 3.9, -12.5, -71.3, 19.5, 0, 0, 0
    
    'Top Hallway
    .AddWall "Wall3", 12.5, 150, 12.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", 12.5, 150, 12.5, 117 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, 117 - 3.9, 12.5, 101 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", 12.5, 101 - 3.9, 12.5, 71.3, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, 150, -12.5, 71.3, 5.5, 19.5, 0, 0
    .AddWall "Wall3", -12.5, 150, -12.5, 117 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, 117 - 3.9, -12.5, 101 + 3.9, 19.5, 0, 0, 0
    .AddWall "Wall3", -12.5, 101 - 3.9, -12.5, 71.3, 19.5, 0, 0, 0
    
    'Individual Rooms
    .AddWall "Wall3", 85, -110, 12.5, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -85, -110, -12.5, -110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 85, -71.3, 12.5, -71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -85, -71.3, -12.5, -71.3, .FloorHeight, 0, 0, 0
    
    .AddWall "Wall3", 85, 110, 12.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -85, 110, -12.5, 110, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", 85, 71.3, 60, 71.3, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -85, 71.3, -60, 71.3, .FloorHeight, 0, 0, 0
        
    .AddWall "Wall3", 85, 0, 32.5, 0, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -85, 0, -32.5, 0, .FloorHeight, 0, 0, 0
    
    'service room
    .AddWall "Wall3", -50, 46.25, -50, 20, .FloorHeight, 0, 0, 0
    .AddWall "Wall3", -50, 20, -32.5, 20, .FloorHeight, 0, 0, 0
        
    End With
    Next i

End Sub

Sub Triton_ProcessElevators()
Dim i As Integer
For i = 1 To 40
Elevator(i).ElevatorSpeed = 1
Elevator(i).Acceleration = 0.1
Next i

Elevator(1).CreateElevator 12.5, 30, 0, 0
Elevator(2).CreateElevator -12.5, 30, 0, 1
Elevator(3).CreateElevator 12.5, 15, 0, 0
Elevator(4).CreateElevator -12.5, 15, 0, 1
Elevator(5).CreateElevator 12.5, 0, 0, 0
Elevator(6).CreateElevator -12.5, 0, 0, 1
Elevator(7).CreateElevator 12.5, -15, 0, 0
Elevator(8).CreateElevator -12.5, -15, 0, 1
Elevator(9).CreateElevator 12.5, -30, 0, 0
Elevator(10).CreateElevator -12.5, -30, 0, 1

Elevator(11).CreateElevator -52.5, 30, 0, 0
Elevator(12).CreateElevator 52.5, 30, 0, 1
Elevator(13).CreateElevator -52.5, 15, 0, 0
Elevator(14).CreateElevator 52.5, 15, 0, 1
Elevator(15).CreateElevator -52.5, 0, 0, 0
Elevator(16).CreateElevator 52.5, 0, 0, 1
Elevator(17).CreateElevator -52.5, -15, 0, 0
Elevator(18).CreateElevator 52.5, -15, 0, 1
Elevator(19).CreateElevator -52.5, -30, 0, 0
Elevator(20).CreateElevator 52.5, -30, 0, 1

Elevator(21).CreateElevator 90.5, 30, 0, 0
Elevator(22).CreateElevator -90.5, 30, 0, 1
Elevator(23).CreateElevator 90.5, 15, 0, 0
Elevator(24).CreateElevator -90.5, 15, 0, 1
Elevator(25).CreateElevator 90.5, 0, 0, 0
Elevator(26).CreateElevator -90.5, 0, 0, 1
Elevator(27).CreateElevator 90.5, -15, 0, 0
Elevator(28).CreateElevator -90.5, -15, 0, 1
Elevator(29).CreateElevator 90.5, -30, 0, 0
Elevator(30).CreateElevator -90.5, -30, 0, 1

Elevator(31).CreateElevator -130.5, 30, 0, 0
Elevator(32).CreateElevator 130.5, 30, 0, 1
Elevator(33).CreateElevator -130.5, 15, 0, 0
Elevator(34).CreateElevator 130.5, 15, 0, 1
Elevator(35).CreateElevator -130.5, 0, 0, 0
Elevator(36).CreateElevator 130.5, 0, 0, 1
Elevator(37).CreateElevator -130.5, -15, 0, 0
Elevator(38).CreateElevator 130.5, -15, 0, 1
Elevator(39).CreateElevator -130.5, -30, 0, 0
Elevator(40).CreateElevator 130.5, -30, 0, 1

End Sub
