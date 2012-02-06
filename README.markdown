Purpose
-------

The Graphical Session Manager was created to serve as a framework for graphical real-time applications - more specifically, for a train simulator project at an engineering school. It is my hope that someone else will find it useful too.

It's design goals were:

 * Modular architecture
 * Platform-agnostic
 * Graphics API agnostic
 * Multi-monitor capable
 * Providing text rendering functionality under OpenGL
 * Easy to use
 * Reasonable build times
 
The support for multiple monitors was the key factor in the decision to create my own framework, rather than trying to use an existing one (of which there are many).

Of course, the support for rendering text under OpenGL actually has little to do with the core functionality of framework that aims to be graphics API agnostic. Yet that requirement was very important for the specific project this framework was built for, so the feature was included from the start.

(And the last goal was the result of the disappointement experienced with the predecessor framework, which had the ambition of avoiding all run-time costs by making heavy use of templates. Even though that actually worked, it had become unmaintainable, and the build times were unacceptable.)

Implementation overview
-----------------------

A few key points:

 * The framework is based on *components*. Component management is extremely basic, as the sole purpose of having components is to make the framework modules exchangeable. All components are singletons (and there are only two at this point, though more will be added in the future); and they are selected at build-time, not at run-time.
 
 * The two components so far are the *Session Manager* and the *Font Provider*. The Session Manager is responsible for managing windows and full-screen displays; the Font Provider gives access to fonts.
 
 * Support for OpenGL is built into the Session Manager. However, that only means making sure that windows and screens will be able to use OpenGL: the Session Manager does not itself *use* OpenGL, or provide any OpenGL-related functionality.
 
 * OpenGL-specific functionality *is* provided, however, through a non-core module `GSM/opengl/opengl.hpp`. This mainly consists (so far) of text rendering functionality (which is based on the Font Provider's ability to rasterize fonts).
 
 * Like most frameworks of this kind, windows and screens are represented, on the user code side, by objects that must implement an interface. In the case of GSM, that interface is IDisplay.

Getting started
---------------

The best way to get started is to have a look at the source code of the GSMTestSuite, and perhaps stepping through it with the debugger. It's as yet a very simple test program, opening a full-screen display and a window, and displaying some text.
