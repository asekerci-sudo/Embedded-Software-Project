The building works  with bazel and I have done the integration tests by running moserial on Linux Manjaro. To execute one integration test copy it from one of the app files and paste it into the main.c file.

Cli commands have the syntax: command commandParameter startTime periodicTime. The command and the nedded parameters are seperated by an empty space. As moserial is
not able to send more than 20 letters, parts of the command are restricted to a number of letters. The commands are inserting the tasks into the scheduler, they can be executed
perodically by setting the periodic time unequal greater than zero. If you need to run a non parametric task periodically, you need to give them parameters even if they dont use it.
I am aware that you could just add void functions too but as this method with giving parameters nonetheless works quite fine I just ignored it (less codesize).

It is also possible to leave out startTime and periodicTime , so command commandParameter is valid too!

The echo command can output one parameter not seperated by a space, as the opposite was not mentioned in the task description (extra parsing work too!).

The commands are first read into the buffer until a enter signal comes. Then the letters are read out until the parsing ends. Each subcommand has its own static array. These
static arrays are than used as paramaters in the InterruptSchedulerAdd function.
Tasks have also a taskID, which is increased at a global scope evertime a task gets inserted. The delete command can delete a Task by specifying the needed TaskID.

When nothing is transmitted or received the Arduino enters into a sleep mode, which is the idle sleep mode, until it is being waked up of course.

I have optimized my application for the polling mode, so use it in polling mode!

I have implemented multiple additional tasks. We have the zero and dec function, the first is to reset the counter to zero and the second is to decrease the counter by 1.
Then we have the sineoff and sineon functions. These are to turn the timer1 on and off. As the sine functions "locks" PORTB, you can enter these commands to enter your other light
functions as it is needed. You could run it in another way automatically and also prioritize light based task in a different way but I did not implement it.
Then there is the reset function set the scheduler and queue pointers to null and reinitialize it again to clear out all previous tasks by one command.

I did not separate the commands and the commandHandler from the main application as I am in the opinion that it is not needed to do it by design because my CommandHandler is not versatile enough to be used as a library function(I have definitely). Furthermore running the CLI is a full integration test itself, so there is no need to the integration test it seperately.

I have added a dice command which has 2^16 random sequences through storing a number in the eeprom. The first time I initiate the command I load the first cell of the eeprom, increment it by 1 and store it again. Thats why have
a different seed everytime for the pseudo random sequence, concluding by that a different random sequence every first run of the dice command.
