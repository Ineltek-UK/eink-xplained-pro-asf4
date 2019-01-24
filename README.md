# eink-xplained-pro-asf4

## Introduction

Eink Xplained Pro ASF4 Software Library with example projects for use with Microchip SAM MCUs.

This software library is designed to work with the following displays and driver ICs:

- ED029TC1 : UC8151
- *HINK-E0154A05 : SSD1608 (not yet implemented)*
- ED057TC2 : UC8159

It also includes several open source fonts in varying sizes and weights.

Note that the software library and each example project has a **doxygen** folder with documentation inside (open *doxygen.html* to get started).

#### Note
> The library only allows a single display to be connected to the MCU.

## Usage 

The folder structure should be maintained after download as the example projects use relative paths to reference the software library. Note as well that the repository should be downloaded to your root C: folder as the folder structure can cause Windows to produce errors such as:
> The source file name(s) are larger than is supported by the file system

## Compilation Information

This software was written for the GNU GCC compiler using Atmel Studio 7.0 and requires ASF version 4 (originally compiled from Atmel START version 1.4.1810). Other compilers may or may not work.
