TODOS:

- IO
	- better io system. Current one is too one dimentional
	- Input should be interpreted based on context i.e, GAME mode moves the character, DEBUG mode moves only the camera, MENU mode operates on the UI.

- menuing
	- go to new menu loop within main gameloop
	- may depend on implimenting a better IO callback system

- Job system:
	- how to check for job dependancies? within function? or within job?

template<typename Data>
struct Job
{
	void(*jobFunction)(Data)
	Data data;
};

template<typename Data>
void kick(Job<Data> job)
{
	jobFunction(job.data);
}
