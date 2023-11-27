## Overview

This project is to create a game that can be played in the browser using C or C++ and using emscripten to compile it to play in the browswer

I don't know at the moment how to pass user input from the Javascript html page through to emscripten.  My plan is to start by going through tests
and looking for ones that prompt user interaction, then I will copy the code out from there and adjust it as input for a game.

So far I found this one named `test_keyboard_codes.c` which reads user input and shows emscripten API button information.

I am wondering if a smarter approach is to deal with the events in javascript and html and pass the needed 
information back to the C/C++ code, do the math, then pass that back to the JavaScript code b/c I think sending data
other than ints and floats back and forth is fairly ineffecient.  I haven't dived into how this test is handling input.
Maybe this is a good approach to take.

For now, the public directory will hold the output of the compilations and an express JS server will serve the pages
for local development.ß


Emscripten: https://emscripten.org/index.html


Initial compilation command: `emcc src/test_keyboard_codes.c -o public/index.html`

Compile command with SDL2: `emcc src/combine_sdl_and_keyboard.cpp -o public/index.html -sUSE_SDL=2`