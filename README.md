# net
Simple C++ Network Library

The goal of this project is to provide a simple, clean, and well documented
library built on top of the BSD sockets API.


## Examples

TODO

## Platforms

Linux

## Roadmap

TODO

## Alternatives

* Poco Net
* ASIO

## Notes
I initially started this project as a way to learn C++ and how to work with the
Berkeley sockets API. It is my opinion that the documentation for asio is 
challenging to follow. I found the asio library very strict in the sense that 
you must use the asio threading model and event model. I wanted something more 
flexible where I had complete control of the threads and could use my own event
loop or just as easily use an external event loop such as libev, libevent, or
libuv. 

