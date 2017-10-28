About the project:
 
This is an android sample project, that shows how to integrate PhysX engine to your application. Scene is filled with 11 spheres, you control one of it. PhysX library and header files are not included because of the license, so it WILL NOT COMPILE until you include it yourself into the "libs" directory. Also here you can find basic touch input handler, camera, lighting etc. Some architecture and technical decisions are far from perfection, but the main point is to show how to get things work.
 
About controls:

The left screen part is for moving, the right screen part for the camera control (press and hold).
 
About PhysX compilation:
 
I've been using the latest NDK, because of that I had to change makefiles, so here are additional steps I had to make:
 
1) Modify in all makefiles gnu-libstdc++/4.8 to gnu-libstdc++/4.9
2) In PhysXCommon.mk dont treat warnings as error, remove flag -Werror
3) Change GNU mode from -std=c99 to -std=gnu99 in all makefiles

P.S.

If you have any questions or comments, you can send it here on github or to email arestic@ya.ru .