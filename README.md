# Runlevels

The runlevel metaphor as applied to hackerspaces.

## The Watering and Care of your Runlevel

The runlevel metaphor is an idea originally introduced to general purpose
computering in System V Unix. For more information, I suggest reading the
authoritive reference on the subject:

https://en.wikipedia.org/wiki/Runlevel

This package aims to implement the runlevel metaphor as a kind of distributed
ethereal number accessible through a Graviton cloud. This implementation has
three properties:

* Resiliant to network effects, hardware degredation, hacking, etc
* Accessible from anywhere with zero configuration or effort
* The ability to monitor its value in real-time

Using Graviton, A service that implements ```net:noisebridge:runlevel``` is exposed on
the network. It provides two methods, ```get``` and ```set```. ```set``` accepts
a single integer parameter named "runlevel". ```get``` takes no arguments and
returns the integer runlevel.

A ```runlevel-changed``` event is broadcast whenever ```set``` is called.

Clients browse the default cloud for the ```net:nosiebridge:runlevel``` service,
and get/set/watch the runlevel as appropriate.

Future iterations will implement a consensus algorithm such that multiple
servers may consense upon the same runlevel.

## Building

You'll need to install the required dependencies:

* Graviton - https://graviton.phrobo.net/

This is built with a simple makefile using pkg-config:

    $ make

## Running a server

    $ ./spacekit-runlevel serve

## Getting the current runlevel

    $ ./spacekit-runlevel get

## Setting the current runlevel

    $ ./spacekit-runlevel set 5

## Watching for changes to the runelvel

    $ ./spacekit-runlevel watch
