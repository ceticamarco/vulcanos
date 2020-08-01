<div align="center">
<img src="imgs/iceOS_logo_full_concept.png" width="350px">
<br><br>
<a href="https://travis-ci.org/ice-bit/iceOS"><img src="https://travis-ci.org/ice-bit/iceOS.svg?branch=master" alt="Build Status" /></a>
<a href="LICENSE.md"><img src="https://img.shields.io/badge/license-GPLv3-blue"></a>
<img src="https://img.shields.io/github/repo-size/ice-bit/iceOS?color=purple" />
<img src="https://img.shields.io/badge/platform-intel--x86-brown" />
<img src="https://img.shields.io/badge/language-C-black" />  


<h6><i>Simple kernel written in C for Intel x86 CPUs</i></h6>
</div>

# About iceOS
<div align="center">
<img src="imgs/screenshot.png"  />
<h6><i>iceOS running under QEMU</h6></i>
</div><br /><br />

**iceOS** is a x86 monolithic kernel written in C from scratch following the UNIX philosophy. This project is just a student learning tool to know more about operating systems, do not expect nothing more than a toy.


## Installation
### Requirements
Before building this project you need to setup a [cross compiler](https://wiki.osdev.org/GCC_Cross-Compiler). Also install the following packages:  

- nasm
- bochs
- grub
- mtools(only for Arch Linux)

After that, you can build iceOS just by running the command listed below.  
1. Type `make all` to compile the system and to create an ISO
2. Type `make run` to start it in QEMU or `make bochs` to start it with bochs(only for debug purposes).

You can also find a ISO file 
[here](https://github.com/ice-bit/iceOS/raw/master/imgs/iceOS.iso)(md5sum: `342691301699a73102fa6a57abea6989`)

## Features
iceOS has the following features:  
- [x] VGA driver  
- [x] Interrupts  
- [x] PIC & PIT driver  
- [x] PS/2 driver  
- [x] Heap  
- [x] Paging
- [ ] VFS driver

## Tutorial
I'm also writing a tutorial about this project. If you are interested about OS development and you're looking for a detailed _step-by-step_ tutorial, have a look at my [website](https://osdev.marcocetica.com).


## License
iceOS is released under GPLv3, you can obtain a copy of this license by cloning this repository or by visiting [this](https://opensource.org/licenses/GPL-3.0) page.
