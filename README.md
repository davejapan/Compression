# Compression
Multithreaded implementation of Run Length Encoding

How to BUILD

After cloning the project, in root directory-
make

How to RUN

After successful build, under build directory, there would be file named run_length_encoding

./run_length_encoding <encode/decode> <file to be encoded> <encoded file path> (Number of threads: Optional)

Issue

Interestingly, even while encoding 1GB sized file, multithreaded run takes significantly more time, that too increasing as number of threads increase.

Note

run length encoding is terrible candidate for image/binary compression.
