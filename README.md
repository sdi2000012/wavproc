# wavproc

### A simple C program to edit .wav files

gcc -DMODE=1 -o wavproc wavproc.c

./wavproc < (example.wav)                       to find if a .wav file has any errors

gcc -DMODE=2 -o wavproc2 wavproc.c

./wavproc2 < (example.wav) > (example2.wav)     to create a .wav file that is a slowed down version of example.wav

gcc -DMODE=3 -o wavproc3 wavproc.c

./wavproc3 < (example.wav) > (example3.wav)     to create a .wav file that is a faster version of example.wav

gcc -DMODE=4 -o wavproc4 wavproc.c

./wavproc4 < (example.wav) > (example4.wav)     to create a .wav file that is only the left channel of example.wav

gcc -DMODE=5 -o wavproc5 wavproc.c

./wavproc5 < (example.wav) > (example5.wav)     to create a .wav file that is only the right channel of example.wav

gcc -DMODE=6 -o wavproc6 wavproc.c

./wavproc6 < (example.wav) > (example6.wav)     to create a .wav file that is a lower volume version example.wav

***Make sure you have the gcc compiler installed***
