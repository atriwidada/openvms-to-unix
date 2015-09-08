# openvms-to-unix
convert openvms textfiles into unix/dos format

Some OpenVMS textfiles has this format
{word: line length; string: ascii string} repeated until eof

There are 3 special cases:
a. zero line length. just ignore this. continue read next line length.
b. an extra byte after string, maybe a word padding?
c. two extra byte after string, purpose unknown
