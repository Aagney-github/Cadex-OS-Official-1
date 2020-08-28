# The Cadex OS
[![Travis CI Build Status](https://travis-ci.org/OpenCreeck/Cadex-OS-Official.svg?branch=master)](https://travis-ci.org/OpenCreeck/Cadex-OS-Official)

Cadex OS is a Unix-like Linux-based operating system. Cadex OS is Unix-like because some of the code in libc are from the source code of Unix (Minix), and Cadex OS is Linux-based because some of the core components used like the ELF parser and some of the lbraries are from older versions of Linux (2.1 and 0.1). This is a fork of the repo https://github.com/dthain/basekernel

If you want to build apps for something _**different**_ than Windows, Linux, or MacOS,
Cadex OS may be a good place to try out your new ideas. Cadex OS has all the libraries and APIs to create your own app.

Cadex OS can boot an Intel PC-compatible virtual machine in 32-bit protected
mode, with support for VESA framebuffer graphics, ATA hard disks, ATAPI optical
devices, process management, memory protection, simple graphics, and basic filesystem.
From there, it's your job to write user-level programs and expand the system.

## Features of Cadex OS
 - Cadex OS supports a small subset of libc, and more functions and libraries from libc need to be implemented.
 - Cadex OS has a small BASIC interpreter that can be used to write small programs.
 - Cadex OS supports graphical and text-based applications
 - Cadex OS uses ELF as the binary format
 - Cadex OS supports modules, but it is still in development and is not in the Makefile
 - Cadex OS will be useful if you are a beginner in C programming lanhguage and you want to write small programs.
 - Cadex OS has a small graphics library called TrueGL. TrueGL is also a UI library
 - Cadex OS has many useful string functions defined at `library/string.c`
 - Cadex OS uses some code from Musl libc for the libc in Cadex OS. Libraries that are from Musl libc should contain a license header on the top.

This repo was made by HyperCreeck and actively maintained by HyperCreeck and people in this Organisation.

To learn more, see the __[Cadex OS Wiki](https://github.com/opencreeck/Cadex-OS-Official/wiki).__
 
## Quick Start Instructions

#### Installing the toolchain

For building Cadex OS, you need to install the toolchain. 
Building toolchain from source is recommended, but prebuilt binaries are available.
 - If you are building toolcahin from source, visit [this](https://hypercreeck.cf/cadex/os/toolchain.php) site
 - If you don't wish to build the toolchain from source (not recommended), you can download prebuilt binaies for ubuntu from [here](https://hypercreeck.cf/cadex/os/toolchain.php)
 
#### Building the OS

If you are building on a Linux-x86 machine
and have the QEMU virtual machine installed:

```
git clone https://github.com/opencreeck/Cadex-OS-Official
cd Cadex-OS-Official
make
qemu-system-i386 -cdrom cadex.iso
```

And you should see something like this:

<img src=screenshot.png align=center>

After some initial boot messages, you will see the kernel shell prompt.
This allows you to take some simple actions before running the first
user level program.  For example, read the boot messages to see
which atapi unit the cdrom is mounted on.  Then, use the `mount` command
to mount the cdrom filesystem on that unit:

<pre>
mount atapi 2 cdromfs
</pre>

Use the `list` command to examine the root directory:

<pre>
list /
</pre>

And use the `run` command to run a program (for example, a simple screensaver):

<pre>
run /usr/bin/saver.exe
</pre>

## Cross-Compiling Instructions

If you are building on any other type of machine,
you will probably need to build a cross-compiler
using `./crosscompiler.sh` and then edit
`Makefile.config` to use the cross compiler binaries,
then execute `make` to create `cadex.iso`

