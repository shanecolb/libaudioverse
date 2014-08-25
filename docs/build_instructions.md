#Building Libaudioverse#

Important status note: While this page is platform-neutral, Libaudioverse currently only builds on Windows.  Overcoming this shortcoming will happen in short order.

##Dependencies##

All of the following dependencies are required to build Libaudioverse.

- Libsndfile
- CMake
- Portaudio (Linux and Mac only; Libaudioverse bundles on windows)
- Python 2.7.x.

And the following Python packages (all available on PyPI):
- Jinja2
- Enum34 (for running the Python bindings only)
- Pycparser

##Setup##

At the moment, Libaudioverse's build system assumes windows in a few places.  Specifically, the directories to Libsndfile are hard-coded in exactly two places, and the paths therein may not be correct.  You may need to update both CMakeLists.txt and the top of bindings/build_bindings.py to point at your Libsndfile.  If this is the case and you used the pre-built binaries that come as an installer, then please report this as an issue so I can fix it.  This only applies to windows.

##Building##

Important note: you must do an out-of-source build with CMake and it must be in `repository_root/build`.  otherwise, the step of the build process that builds bindings will not be able to work properly, as it hard-codes a minimal amount of information required to bundle Libaudioverse.

The rest of the process is the standard CMake process.  Something like the following:

~~~
mkddir build
cd build
cmake -G "MY_GENERATOR" ..
my_generator_specific_build_command
~~~
