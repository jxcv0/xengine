TODOS:

- main character movement
	- movement direction depends on camera position
	- after animations are implimented, animation will depend also on if "aiming"

- IO
	- better io system. Current one is too one dimentional
	- Input should be interpreted based on context i.e, GAME mode moves the character, DEBUG mode moves only the camera, MENU mode operates on the UI.
	- Bitmask for input instead of loads of bool

- menuing
	- go to new menu loop within main gameloop
	- may depend on implimenting a better IO callback system

- Job system:
	- check for job dependancies within job function not within job
	- if function cannot run to completion because of a dependancy then push the job back onto the queue
