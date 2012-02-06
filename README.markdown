Purpose
-------

The Graphical Session Manager was created to serve as a framework for graphical real-time applications - more specifically, for a train simulator project at an engineering school. It is my hope that someone else will find it useful too.

It's design goals were:

 * Modular architecture
 * Platform-agnostic
 * Graphics API agnostic
 * Multi-monitor capable
 * Easy to use
 * Reasonable build times
 
The support for multiple monitors was the factor making it necessary to create a new framework

(The last goal was a result of the disappointement experienced with the predecessor framework, which had the ambition of avoiding all run-time costs by making heavy use of templates. Even though the framework worked, it had become unmaintainable, and the build times were unacceptable.)

Introduction
------------

The best way to get started is to have a look at the source code of the GSMTestSuite, especially using the Visual Studio debugger. It was created as a simple test, opening a full-screen display and a window.
