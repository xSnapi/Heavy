![Heavy Engine](https://github.com/xSnapi/Heavy/blob/dev/Images/heavy.png?raw=true)

# Heavy
## Engine for SFML games
The main goal of Heavy is to make it as close to SFML syntax as possible for example creating rigid body is easy as ```hv::RigidBody rb;``` and thats really it.
***

## Heavy uses:

![alt text](https://www.sfml-dev.org/images/logo.png "SFML")
![alt text](https://box2d.org/images/logo.svg "box2D")
![alt text](https://github.com/bombomby/optick/blob/master/samples/UnrealEnginePlugin/Resources/Icon128.png?raw=true "OPTICK")


### [SFML](https://github.com/SFML/SFML) | [Box2D](https://github.com/erincatto/box2d) | [Optick](https://github.com/bombomby/optick) | [ImGui](https://github.com/ocornut/imgui)


<br/>
<br/>

## Why "Heavy" tho? 
As inexperienced programmer I thought it will come up clumsy and inefficient so I called it heavy.
In the opposite it come up pretty well (beside few things).
***

## Heavy Features:
### Uses separate thread for drawing resulting in higher FPS

* **Runtime** - main class, runs the application
* **Asset Manager** - used to store textures etc.
* **Asset Loader** - used to asynchronously load assets
* **Camera** - overall camera controll features functions as shake etc.
* **Colliders** - different colliders for convex shapes featuring drawing
* **Collision** - collision detection and response
* **Event Dispatcher** - allows you to control events from everywhere
* **Fixed Clock** - featuring stop and start
* **Heavy Math and Utilities** - different math and conversion functions
* **Key Check** - checks user input can work in not repeat mode
* **Particle Emitter** - creating and controlling squared particles
* **Profile Timer** - used to time function
* **Random** - simple random numbers generator
* **Timer** - repeats it's given function every period of time
* **Animator** - handles sprite sheet animations
* ... More added soon
