Attribute VB_Name = "Module1"
Global TV As TVEngine
Global Scene As TVScene
Global Mesh As TVMesh
Global External As TVMesh
Global Room(138) As TVMesh
Global Shafts As TVMesh
Global Elevator1 As TVMesh
Global Elevator1DoorL As TVMesh
Global Elevator1DoorR As TVMesh
Global ElevatorDoorL(138) As TVMesh
Global ElevatorDoorR(138) As TVMesh
Global Stairs(138) As TVMesh
Global SoundEngine As TVSoundEngine
Global ElevatorMusic As TVSoundWave3D
Global Elevator1Sounds As TVSoundWave3D
Global Listener As TVListener
Global ListenerDirection As D3DVECTOR
'Global MousePositionX As Long
'Global MousePositionY As Long
Global Focused As Boolean
Global LineTest As D3DVECTOR
Global LineTest2 As D3DVECTOR
Global KeepAltitude As Integer
Global ElevatorSync As Boolean

Global Atmos As New TVAtmosphere
Global sngPositionX As Single
Global sngPositionY As Single
Global sngPositionZ As Single
Global snglookatX As Single
Global snglookatY As Single
Global snglookatZ As Single
Global sngAngleX As Single
Global sngAngleY As Single
Global sngWalk As Single
Global sngStrafe As Single

Global Inp As TVInputEngine
Global TextureFactory As TVTextureFactory
Global linestart As D3DVECTOR
Global lineend As D3DVECTOR
Global elevator1start As D3DVECTOR
Global isRunning As Boolean
Global Camera As TVCamera
Global ColRes As TV_COLLISIONRESULT
Global i As Integer
Global ElevatorEnable As Single
Global ElevatorDirection As Integer '1=up -1=down
Global OpenElevatorLoc As Single
Global ElevatorCheck As Integer
Global ElevatorCheck2 As Integer
Global ElevatorCheck3 As Integer
Global ElevatorCheck4 As Integer


Global GotoFloor As Integer
Global CurrentFloor As Integer
Global CurrentFloorExact As Single
Global DistanceToTravel As Single
Global Destination As Single
Global OriginalLocation As Single
Global StoppingDistance As Single
Global FineTune As Boolean
Global OpenElevator As Integer '1=open -1=close
Global ElevatorFloor As Integer
Global CameraFloor As Integer
Global PartialFloor As Single

