# Chippy

> A (Super) CHIP-8 virtual machine.

[![License](https://img.shields.io/github/license/jacobvaneijk/chippy.svg)](LICENSE)

CHIP-8 is an interesting little programming language. Programs written in it are run on a CHIP-8 virtual machine. This repository contains the source code for my CHIP-8 virtual machine, which I made to get started with emulation.

## Building

1. Make sure you have installed the following dependencies:

    - meson
    - git

2. Clone the source with `git` and `cd` into the cloned repository:

    ```sh
    $ git clone https://github.com/jacobvaneijk/chippy.git && cd chippy
    ```

3. Build:

    ```sh
    $ meson build
    ```

## License

Licensed under the terms of the [MIT license](LICENSE).
