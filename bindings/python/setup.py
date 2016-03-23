from setuptools import setup, find_packages
from glob import glob


__version__ = '0.8.0'

setup(
    name = 'libaudioverse',
    version = __version__,
    author = "Austin Hicks",
    author_email = "camlorn@camlorn.net",
    url = "http://github.com/camlorn/libaudioverse",
    description = "A library for 3D, environmental audio, and synthesis.",
    long_description = open('readme.rst').read(),
    package_dir = {'libaudioverse': 'libaudioverse'},
    packages = find_packages(),
    package_data = {'libaudioverse':
        ['*.dll']
    },
    classifiers = [
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Programming Language :: Python',
        'License :: OSI Approved :: GNU General Public License (GPL)',
        'Topic :: Software Development :: Libraries'
    ],
    zip_safe = False,
    install_requires = ['six'],
    extras_require = { #This comes from wheel docs, but by the PEP explaining environment markers shouldn't work. But it does.
        ':python_version <= "3.4"': ['enum34'],
    }
)
