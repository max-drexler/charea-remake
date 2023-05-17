# Charea Remake

## About

charea is a program for naming McIdas [AREA](https://www.ssec.wisc.edu/mcidas/doc/prog_man/current/formats-1.html#13797) files. 

[This](https://gitlab.ssec.wisc.edu/mdrexler/charea-remake) repo contains a remake of the [old charea](/lib/charea.c) program that was written in C.  

### The Remake

The goal of the remake is threefold:
 * Document how charea works.
 * Preserve 1-to-1 functionality in the remake.
 * Enhance future maintainability of the remake.

Documentation is contained in the file with comments.  
Testing verifies that the remake has the same functionality.  
Writing the remake in python ensures that it is maintainable.

## Use

Please note that python >= 2.7 is required to run the new charea remake.  

After installation, using `chmod +x charea` will allow it to run without directly using python from the command line.

## Author

This remake was created and maintained by Max Drexler.  

For any issues, email mndrexler@wisc.edu or submit [here](https://gitlab.ssec.wisc.edu/mdrexler/charea-remake/-/issues).