/*it has 6 modes, use:

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
*/

#include <stdio.h>

unsigned long littleendian(int n) {         //gets n bytes in little endian and creates the number they represent
    int sum, mul, i, ch;
    sum = 0;
    mul = 1;                                //multiplier, every byte represents a number that is 256 times greater than the previous one
    for (i = 1 ; i <= n ; i++){
        ch = getchar();
        sum += ( mul * ch);
        mul *= 256;
    }
    return sum;
}

int text(char a, char b, char c, char d) {      //gets 4 characters and if they are the same as the bytes it reads, it returns 0, else it returns 1
    int ch1 , ch2 , ch3 , ch4;
    ch1 = getchar();
    ch2 = getchar();
    ch3 = getchar();
    ch4 = getchar();
    if (ch1 != a || ch2 != b || ch3 != c || ch4 != d){
        return 1;
    }
    else
    {
        return 0;
    }
}

void put(int num, int n){                   //gets a number and creates the equivalent in little endian form using n bytes. It's the reverse of littlendian
    int i, div;
    div = 1;
    for ( i = 1 ; i <= n ; i++){
        putchar ((num / div) % 256);
        div *= 256;
    }
}


void puttext(char a, char b, char c, char d) {          //puts 4 bytes, it is used to put text such as RIFF
    putchar(a);
    putchar(b);
    putchar(c);
    putchar(d);
}


int main(void) {
    int byte, byte2, bytecounter, SizeOfExtra, BytesPerSec, SampleRate, BlockAlign, MonoStereo, BitsPerSample, SizeOfData, format, WaveFormat, RIFF, WAVE, data, fmt ;
    signed int bitnum;
    unsigned long SizeOfFile;
    bytecounter = 0;                    //counts the bytes after SizeOfData
    SizeOfExtra = 0;                    //counts extra bytes (when the bytecounter passes the SizeOfData, we have a few extra bytes)
                RIFF = text('R','I','F','F');
            if (RIFF == 1 ) {           //what it read was not RIFF, so we have an error
                fprintf (stderr ,"ERROR! \"RIFF\" not found\n");
                return 1;               //so it ends here, this process happens everytime we find an error
            }

        SizeOfFile = littleendian(4);
        fprintf (stderr, "size of file: %ld bytes\n", SizeOfFile);

            WAVE = text('W','A','V','E');
            if ( WAVE == 1 ) {
                fprintf (stderr ,"ERROR! \"WAVE\" not found\n");
                return 1;
            }

            fmt = text('f','m','t',' ');
            if (fmt == 1 ) {
                fprintf (stderr ,"ERROR! \"fmt \" not found\n");
                return 1;
            }

        format = littleendian(4);
        fprintf (stderr, "size of format chunks: %d\n",format);
        if (format != 16) {
            fprintf (stderr, "ERROR! size of format chunck should be 16\n");        //format <> 16 => error
            return 1;
        }

        WaveFormat = littleendian(2);
        fprintf (stderr, "WAVE type format: %d\n", WaveFormat);
        if (WaveFormat != 1) {
            fprintf (stderr, "ERROR! WAVE type format should be 1\n");          //WaveFormat <> 1 => error
            return 1;
        }

        MonoStereo = littleendian(2);
        fprintf (stderr, "mono/stereo: %d\n", MonoStereo);
        if (MonoStereo != 1 && MonoStereo != 2){
            fprintf (stderr, "ERROR! mono/stereo should be 1 or 2\n");          //MonoStereo<> 1 , 2 => error
            return 1;
        }

        SampleRate = littleendian(4);
        fprintf (stderr, "sample rate: %d\n", SampleRate);

        BytesPerSec = littleendian(4);
        fprintf (stderr, "bytes/sec: %d\n", BytesPerSec);

        BlockAlign = littleendian(2);
        fprintf (stderr, "block alignment: %d\n", BlockAlign);
        if (BytesPerSec != SampleRate * BlockAlign){
            fprintf (stderr ,"ERROR! bytes/second should be sample rate x block alignment\n");      //BytesPerSec <> SampleRate * BlockAlign => error
            return 1;
        }

        BitsPerSample = littleendian(2);
        fprintf (stderr, "bits/sample: %d\n", BitsPerSample);
        if (BitsPerSample != 8 && BitsPerSample != 16){
            fprintf (stderr ,"ERROR! bits/sample should be 8 or 16\n");         //BitsPerSample <> 8,16 => error
            return 1;
        }
        else if (BlockAlign != (BitsPerSample/8) * MonoStereo) {
            fprintf (stderr ,"ERROR! block alignment should be (bits per sample /8) x (mono/stereo)\n");        //BlockAlign <> (BitsPerSample/8) * MonoStereo => error
            return 1;
        }

            data = text('d','a','t','a');
            if (data == 1 ) {
                fprintf (stderr ,"ERROR! \"data\" not found\n");
                return 1;
            }

        SizeOfData = littleendian(4);
        fprintf (stderr, "size of data chunk: %d\n",SizeOfData);

                                                        //modes pt.1
        if (MODE == 2){                                 //mode 2: plays the sound in half the speed
            puttext ('R','I','F','F');
            put(SizeOfFile,4);
            puttext ('W','A','V','E');
            puttext ('f','m','t',' ');
            put(format,4);
            put(WaveFormat,2);
            put(MonoStereo,2);
            put(SampleRate / 2 ,4);                     //half the speed => every second it uses half the samples as before
            put(BytesPerSec / 2 ,4);                    //half the speed => half the bytes every second
            put(BlockAlign ,2);
            put(BitsPerSample,2);
            puttext('d','a','t','a');
            put(SizeOfData,4);
        }

        if (MODE == 3){                                 //mode 3: plays the sound in double the speed
            puttext ('R','I','F','F');
            put(SizeOfFile,4);
            puttext ('W','A','V','E');
            puttext ('f','m','t',' ');
            put(format,4);
            put(WaveFormat,2);
            put(MonoStereo,2);
            put(SampleRate * 2 ,4);                     //double the speed => every second it uses twice as many samples as before
            put(BytesPerSec * 2 ,4);                    //double the speed => twice as many bytes every second
            put(BlockAlign,2);
            put(BitsPerSample,2);
            puttext('d','a','t','a');
            put(SizeOfData,4);
        }

        if (MODE == 4){                                 //mode 4: left channel when stereo, same when mono
            puttext ('R','I','F','F');
            if (MonoStereo == 2){
                put(SizeOfData / 2 + SizeOfFile - SizeOfData ,4);   //SizeOfFile = 36 + SizeOfData + SizeOfExtra
                }
            else {
                put(SizeOfFile,4);
                }
            puttext ('W','A','V','E');
            puttext ('f','m','t',' ');
            put(format,4);
            put(WaveFormat,2);
            put(1,2);
            put(SampleRate ,4);
            if (MonoStereo == 2){
                put(BytesPerSec / 2 ,4);        //uses every other byte
                put(BlockAlign / 2 ,2);
            }
            else {
                put(BytesPerSec,4);
                put(BlockAlign,2);
            }
            put(BitsPerSample,2);
            puttext('d','a','t','a');
            if (MonoStereo == 2){
                put(SizeOfData / 2 ,4);         //half the data
            }
            else{
                put(SizeOfData,4);
            }
        }

        if (MODE == 5){                 //mode 5: right channel
            puttext ('R','I','F','F');
            if (MonoStereo == 2){
                put(SizeOfData / 2 + SizeOfFile - SizeOfData ,4);
                }
            else {
                put(SizeOfFile,4);
                }
            puttext ('W','A','V','E');
            puttext ('f','m','t',' ');
            put(format,4);
            put(WaveFormat,2);
            put(1,2);
            put(SampleRate ,4);
            if (MonoStereo == 2){
                put(BytesPerSec / 2 ,4);
                put(BlockAlign / 2 ,2);
            }
            else {
                put(BytesPerSec,4);
                put(BlockAlign,2);
            }
            put(BitsPerSample,2);
            puttext('d','a','t','a');
            if (MonoStereo == 2){
                put(SizeOfData / 2 ,4);
            }
            else{
                put(SizeOfData,4);
            }
        }

        if (MODE == 6){                     //mode 6: 1/8th the volume
            puttext ('R','I','F','F');
            put(SizeOfFile,4);
            puttext ('W','A','V','E');
            puttext ('f','m','t',' ');
            put(format,4);
            put(WaveFormat,2);
            put(MonoStereo,2);
            put(SampleRate,4);
            put(BytesPerSec,4);
            put(BlockAlign,2);
            put(BitsPerSample,2);
            puttext('d','a','t','a');
            put(SizeOfData,4);
        }                                   //end of modes pt.1

        byte = getchar();                   //reads every remaining byte (data and extra)
        while (byte != EOF) {
            bytecounter ++;
                                            //modes pt.2
            if (MODE == 2 || MODE == 3){    //puts the bytes without any changes
                putchar (byte);
            }

            if (MODE == 4){
                if (MonoStereo == 1){       //when it's mono, it puts the byes without any changes
                    putchar(byte);
                }
                else {                      //stereo
                    if (BitsPerSample == 8){                                                //every byte is a sample,so odd bytes are left channel and even bytes are right channel
                        if (bytecounter % 2 == 1 && bytecounter <= SizeOfData){             //this byte is left channel and it is not yet on the extra bytes
                            putchar (byte);
                        }
                        else if (bytecounter > SizeOfData){                                 //extra bytes, it puts every extra byte
                            putchar (byte);
                        }
                    }else if (BitsPerSample == 16){                                         //every 2 bytes is a sample, so the 1st and 2nd bytes are left channel while the 3rd and 4th are right
                        if ((bytecounter % 4 == 1 ||bytecounter % 4 == 2)  && bytecounter <= SizeOfData){
                            putchar (byte);
                        }
                        else if (bytecounter > SizeOfData){
                            putchar (byte);
                        }
                    }
                }
            }

            if (MODE == 5){                 //same thing as MODE 4 but it only puts the right channel
                if (MonoStereo == 1){
                    putchar(byte);
                }
                else {
                    if (BitsPerSample == 8){
                        if (bytecounter % 2 == 0 && bytecounter <= SizeOfData){
                            putchar (byte);
                        }
                        else if (bytecounter > SizeOfData){
                            putchar (byte);
                        }
                    }else if (BitsPerSample == 16){
                        if ((bytecounter % 4 == 3 ||bytecounter % 4 == 0)  && bytecounter <= SizeOfData){
                            putchar (byte);
                        }
                        else if (bytecounter > SizeOfData){
                            putchar (byte);
                        }
                    }
                }
            }

            if (MODE == 6){
                if (bytecounter <= SizeOfData){             //it doesn't change the extra bytes
                    if (BitsPerSample == 8){                //1 sample = 1 byte, the higher the number the higher the volume, division by 8 means 1/8th of the original volume
                        putchar(byte/8);
                    }
                   else {                                   //2 bytes => a number that belongs here: [-32768,32767]
                        byte2 = getchar();                  //reads the next byte and stores it in a different variable
                        bitnum = byte + 256 * byte2;        //here it is always positive because byte and byte2 are unsigned
                        if (bitnum > 32767) {               //in that case the sample is a negative number
                            bitnum = -32768 + (bitnum - 32768); //2's compliment
                        }
                            put(bitnum / 8 ,2);
                            bytecounter++;                  //it read 2 bytes and bytecounter has only icreased once, so it increases it again
                        }
                    }
                else
                {
                    putchar(byte);
                }
            }
                                                            //end of modes pt.2
            byte = getchar();                               //gets the next byte
            if (bytecounter > SizeOfData) {                 //wit has read all of the data and it reads extra data
                SizeOfExtra ++;
            }
        }
        if (bytecounter < SizeOfData ){
            fprintf (stderr, "ERROR! insufficient data\n");
            return 1;
        }
        if (SizeOfFile != 36 + SizeOfData + SizeOfExtra) {
            fprintf (stderr, "ERROR! bad file size\n");
            return 1;
        }
    return 0;
}
