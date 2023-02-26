This project is an implementation of the SHA256 algorithm
done in the context of the Cryptography subject at University.
The algorithm definition and result correctness were ensured
in the following sources, respectively:
https://en.wikipedia.org/wiki/SHA-2
https://sha256algorithm.com

The usha256 program takes as input the file path from where to
read the initial data and prints to stdout the converted
sha256 value.

The binary is compiled using GNU Make with the command:
```
make
```

For more information, execute:
```
make help
```
