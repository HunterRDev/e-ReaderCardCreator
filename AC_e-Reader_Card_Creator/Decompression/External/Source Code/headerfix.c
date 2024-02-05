#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void exitTool()
{
    printf("\n\nPress ENTER to exit...");
    while(1)
    {
        if (getchar())
            break;
    }
}

unsigned short getDataChecksum(unsigned char *data, int dataSize)
{
    unsigned short i, sum = 0;

    for(i = 0; i < dataSize; i+=2)
    {
        sum += ((data[i] << 8) + data[i + 1]);
    }

    return ~sum & 0xFFFF;
}

unsigned char getHeaderChecksum(unsigned char *header)
{
    unsigned short i, xor = 0;

    xor ^= header[0xC];
    xor ^= header[0xD];
    xor ^= header[0x10];
    xor ^= header[0x11];
    for(i = 0x26; i < 0x2D; i++)
    {
        xor ^= header[i];
    }
    return xor & 0xFF;
}

unsigned char getGlobalChecksum(unsigned char *header, unsigned char *data, int dataSize)
{
    short i, j, sum = 0, xor = 0;

    for(i = 0; i < 0x2F; i++)
    {
        sum += header[i];
    }

    for(i = 0; i < dataSize/0x30; i++)
    {
        xor = 0;
        for(j = 0; j < 0x30; j++)
        {
            xor ^= data[(i * 0x30) + j];
        }
        sum += xor;
    }
    return ~sum & 0xFF;
}

int main(int argc, char** argv)
{
    FILE *card;
    unsigned char *data, *header;
    int headerSize = 0x30, cardSize = 0, dataSize = 0;

    printf("e-Card Headerfix - fixes the header of decoded e-Cards\n");

    if(argc != 2)
    {
        printf("\ndrop a decoded e-Card onto the exe\n");
        exitTool();
        return 0;
    }

    card = fopen(argv[1], "rb+");
    if(!card)
    {
        printf("\ne-Card loading failed!\n");
        exitTool();
        return 0;
    }

    fseek(card, 0, SEEK_END);
    cardSize = ftell(card);
    rewind(card);

    if(cardSize % 0x30 != 0 || cardSize <= 0x30)
    {
        printf("\nInvalid card size!\n");
        exitTool();
        return 0;
    }

    header = (unsigned char*)malloc(headerSize);
    if(header == NULL)
    {
        printf("\nMemory allocation error!\n");
        exitTool();
        return 0;
    }
    fread(header, 1, headerSize, card);
    rewind(card);

    unsigned char signature[8] = {0x4E, 0x49, 0x4E, 0x54, 0x45, 0x4E, 0x44, 0x4F}; // NINTENDO
    int x;
    for(x = 0; x < 8; x++)
    {
        if(header[0x1A + x] != signature[x])
        {
            printf("\nInvalid e-Card signature!\n");
            exitTool();
            return 0;
        }
    }

    dataSize = (header[0x06] << 8) + header[0x07];

    data = (unsigned char*)malloc(dataSize);
    if(data == NULL)
    {
        printf("\nMemory allocation error!\n");
        exitTool();
        return 0;
    }
    fseek(card, 0x30, SEEK_SET);
    fread(data, 1, dataSize, card);
    rewind(card);

    unsigned short dataChecksum = getDataChecksum(data, dataSize);
    header[0x13] = (unsigned char)(dataChecksum >> 8);
    header[0x14] = (unsigned char)(dataChecksum & 0xFF);

    unsigned char headerChecksum = getHeaderChecksum(header);
    header[0x2E] = (unsigned char)headerChecksum;

    unsigned char globalChecksum = getGlobalChecksum(header, data, dataSize);
    header[0x2F] = (unsigned char)globalChecksum;

    fwrite(header, 1, headerSize, card);

    free(data);
    free(header);
    fclose(card);

    printf("\nChecksums updated!\n");

    return 0;
}