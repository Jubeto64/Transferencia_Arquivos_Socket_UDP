#include <stdlib.h>
#include <stdio.h>

typedef struct{
//Buffers e chunks...
char ChunkID[4], Format[4], Subchunk1ID[4], Subchunk2ID[4];
int ChunkSize, Subchunk1Size, SampleRate, ByteRate, Subchunk2Size;
short AudioFormat, NumChannels, BlockAlign, BitsPerSample;
short *Data;
}headerWAV;

short* createData(FILE *input, headerWAV header){
short *Data = (short*) malloc(sizeof(short)*(header.Subchunk2Size)*(header.BitsPerSample/8));
return Data;
}

headerWAV readWAV(FILE *input, headerWAV header){
//Leitura do arquivo WAV
fread(header.ChunkID, 1, 4, input);
fread(&header.ChunkSize, 4, 1, input);
fread(header.Format, 1, 4, input);
fread(header.Subchunk1ID, 1, 4, input);
fread(&header.Subchunk1Size, 4, 1, input);
fread(&header.AudioFormat, 2, 1, input);
fread(&header.NumChannels, 2, 1, input);
fread(&header.SampleRate, 4, 1, input);
fread(&header.ByteRate, 4, 1, input);
fread(&header.BlockAlign, 2, 1, input);
fread(&header.BitsPerSample, 2, 1, input);
fread(header.Subchunk2ID, 1, 4, input);
fread(&header.Subchunk2Size, 4, 1, input);

short *Buffer = createData(input, header);
header.Data = Buffer;
fread(header.Data, header.BitsPerSample/8, header.Subchunk2Size/(header.BitsPerSample/8), input);
fclose(input);

return header;
}

headerWAV writeWAV(FILE *output, headerWAV header){
//Escrita do arquivo WAV
fwrite(header.ChunkID, 1, 4, output);
fwrite(&header.ChunkSize, 4, 1, output);
fwrite(header.Format, 1, 4, output);
fwrite(header.Subchunk1ID, 1, 4, output);
fwrite(&header.Subchunk1Size, 4, 1, output);
fwrite(&header.AudioFormat, 2, 1, output);
fwrite(&header.NumChannels, 2, 1, output);
fwrite(&header.SampleRate, 4, 1, output);
fwrite(&header.ByteRate, 4, 1, output);
fwrite(&header.BlockAlign, 2, 1, output);
fwrite(&header.BitsPerSample, 2, 1, output);
fwrite(header.Subchunk2ID, 1, 4, output);
fwrite(&header.Subchunk2Size, 4, 1, output);

fwrite(header.Data, header.BitsPerSample/8, header.Subchunk2Size/(header.BitsPerSample/8), output);
fclose(output);

return header;
}

int main(){
int i;
headerWAV header, headerOut;

FILE *input = fopen("som.wav", "rb"), *output = fopen("myFile.wav", "wb");

if(input == NULL)
printf("Arquivo nao pode ser aberto!!!\n");

header = readWAV(input, header);

if(output == NULL)
printf("Arquivo nao pode ser aberto!!!\n");

headerOut = writeWAV(output, header);

printf("\n");
system("PAUSE");
}