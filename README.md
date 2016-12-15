# Chippy

> A (Super) CHIP-8 virtual machine.

[![Build Status](https://img.shields.io/travis/jacobvaneijk/chippy.svg)](https://travis-ci.org/jacobvaneijk/chippy) [![License](https://img.shields.io/github/license/jacobvaneijk/chippy.svg)](LICENSE)

CHIP-8 is an interesting little programming language. Programs written in it are run on a CHIP-8 virtual machine. This repository contains the source code for my CHIP-8 virtual machine, which I made to get started with emulation.

## Building

1. Make sure you have installed the following dependencies:

    - autoconf
    - automake
    - git

2. Clone the source with `git` and `cd` into the cloned repository:

    ```sh
    $ git clone https://github.com/jacobvaneijk/chippy.git && cd chippy
    ```

3. Build:

    ```sh
    $ ./autogen.sh && ./configure && make
    ```

## License

Licensed under the terms of the [MIT license](LICENSE).
