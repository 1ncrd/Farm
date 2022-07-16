# The_Pig_Farm

This is a small game project based on Qt. It's my programming course final assignment in college. The project mainly uses C++.

## Version 1.0

Implement a part of function including the basic UI, automatical pig growth.
This version use only single threads which causes the interface to be a bit sluggish.

> PS: Actually the program logic is easy to design. The design of graphical interface and the interaction with the data takes a quite long time in that It's my first time to write the code about this.
> The front-end code is so annoying. Sometimes I browsing the browser for a long while just to realize some simple things.

## Version 2.0

The most significant update in this version is that it use multiple threads to separate UI display from data calculation. This greatly improves the interface's smoothness。
At the same time, it realize a part of function of the display of pig sale records.

## Version 3.0

Add the archive management system. Now the program can read and write the archive.

## trivia(琐事)

There are a lot of signal-and-slot function in the program. Except for dealing with the UI, a major reason is that the program uses multiple threads and have to use the signal_and_slot function to pass information across threads.
