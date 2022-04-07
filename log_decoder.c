#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define size 1166

typedef struct in
{
    int ID;
    int FrameNb;
    int Timestamp;
    char Payload[50];
    char Checksum[10];
} in;

typedef struct out
{
    int ID;
    int FrameNb;
    int Timestamp;
    float PositionX;
    float PositionY;
    float VelocityX;
    float VelocityY;
    bool ChecksumOK;
    bool TimeoutOK;
    int FrameDropCnt;

} out;

int main(int argc, char *argv[])
{
    if (argc == 3)
    {

        FILE *inputFile = fopen(argv[1], "r");
        FILE *outputFile = fopen(argv[2], "w+");

        if (!inputFile)
        {
            printf("Error in opening the file. Please check the input file.");
            return 0;
        }
        if (!outputFile)
        {
            printf("Error in opening the file. Please check the output file.");
            return 0;
        }
        char buff[size]; // store first lines
        int rowCount = 0;
        int fieldCount = 0;
        int dropedFrames = 0;

        in ins[size];
        out outs[size];

        int i = 0;
        while (fgets(buff, size, inputFile))
        {
            fieldCount = 0;
            rowCount++;
            if (rowCount == 1)
                continue; //skip the labels
            char *field = strtok(buff, ",");
            while (field)
            {
                if (fieldCount == 0)
                    ins[i].ID = atoi(field);
                if (fieldCount == 1)
                    ins[i].FrameNb = atoi(field);
                if (fieldCount == 2)
                    ins[i].Timestamp = atoi(field);
                if (fieldCount == 3)
                    //ins[i].Payload = atoi(field);
                    strcpy(ins[i].Payload, field);
                if (fieldCount == 4)
                    strcpy(ins[i].Checksum, field);
                field = strtok(NULL, ",");
                fieldCount++;
            }
            i++;
        }
        fclose(inputFile);

        fprintf(outputFile, "%s", "ID");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "FrameNb");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "Timestamp");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "PositionX");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "PositionY");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "VelocityX");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "VelocityY");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "ChecksumOK");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "TimeoutOK");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "FrameDropCnt");
        fprintf(outputFile, "%s", ",");
        fprintf(outputFile, "%s", "\n");

        for (int i = 0; i < 1165; i++)
        {
            outs[i].ID = ins[i].ID;
            fprintf(outputFile, "%d,", outs[i].ID);
            outs[i].FrameNb = ins[i].FrameNb;
            fprintf(outputFile, "%d,", outs[i].FrameNb);
            outs[i].Timestamp = ins[i].Timestamp;
            fprintf(outputFile, "%d,", outs[i].Timestamp);
            outs[i].FrameDropCnt = dropedFrames;
            if (ins[i].ID == 15)
            {
                char posX[50];
                char posY[50];

                uint16_t checkSum;
                uint16_t checkSumIn = strtol(ins[i].Checksum, NULL, 16);
                //Obtaining the first 16 bits and the least 16 bits
                for (int j = 0; j < 4; j++)
                {
                    posY[j] = ins[i].Payload[j];
                    posX[j] = ins[i].Payload[j + 4];
                }
                //Converting to decimal the hexa value
                uint16_t x_position = strtol(posX, NULL, 16);
                uint16_t firstSecondByte = x_position;

                outs[i].PositionX = (float)x_position / 100;
                fprintf(outputFile, "%.2f,", outs[i].PositionX);

                uint16_t y_position = strtol(posY, NULL, 16);
                uint16_t thirdFourthByte = y_position;

                //Summing each byte in order to calculate the checkSum
                if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 256)
                {
                    checkSum = 256 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 256 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 512)
                {
                    checkSum = 512 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 512 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 768)
                {
                    checkSum = 768 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 768 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 1024)
                {
                    checkSum = 1024 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }

                outs[i].PositionY = ((float)y_position + (-32767)) / 1000;

                fprintf(outputFile, "%.3f,,,", outs[i].PositionY);

                if (checkSum == checkSumIn)
                {
                    outs[i].ChecksumOK = true;
                    fprintf(outputFile, "%s,", "true");
                }
                else
                {
                    outs[i].ChecksumOK = false;
                    fprintf(outputFile, "%s,", "false");
                }
                if (ins[i].FrameNb * 25 == ins[i].Timestamp || ins[i].FrameNb * 25 == ins[i].Timestamp + 1 || ins[i].FrameNb * 25 == ins[i].Timestamp + 2 || ins[i].FrameNb * 25 == ins[i].Timestamp - 1 || ins[i].FrameNb * 25 == ins[i].Timestamp - 2)
                {
                    outs[i].TimeoutOK = true;
                    fprintf(outputFile, "%s,", "true");
                }
                else
                {
                    outs[i].TimeoutOK = false;

                    fprintf(outputFile, "%s,", "false");
                    dropedFrames++;
                }
            }
            if (ins[i].ID == 78)
            {

                char veloX[50];
                char veloY[50];
                uint16_t checkSum;
                uint16_t checkSumIn = strtol(ins[i].Checksum, NULL, 16);
                for (int j = 0; j < 4; j++)
                {
                    veloY[j] = ins[i].Payload[j];
                    veloX[j] = ins[i].Payload[j + 4];
                }

                uint16_t x_velocity = strtol(veloX, NULL, 16);
                uint16_t firstSecondByte = x_velocity;

                outs[i].VelocityX = ((float)x_velocity + (-32767)) / 1000;

                fprintf(outputFile, ",,%.3f,", outs[i].VelocityX);

                uint16_t y_velocity = strtol(veloY, NULL, 16);
                uint16_t thirdFourthByte = y_velocity;
                if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 256)
                {
                    checkSum = 256 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 256 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 512)
                {
                    checkSum = 512 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 512 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 768)
                {
                    checkSum = 768 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }
                else if (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 > 768 && firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256 <= 1024)
                {
                    checkSum = 1024 - (firstSecondByte / 256 + firstSecondByte % 256 + thirdFourthByte / 256 + thirdFourthByte % 256);
                }

                outs[i].VelocityY = ((float)y_velocity + (-32767)) / 1000;
                fprintf(outputFile, "%.3f,", outs[i].VelocityY);
                if (checkSum == checkSumIn)
                {
                    outs[i].ChecksumOK = true;
                    fprintf(outputFile, "%s,", "true");
                }
                else
                {
                    outs[i].ChecksumOK = false;
                    fprintf(outputFile, "%s,", "false");
                }

                if (ins[i].FrameNb * 50 == ins[i].Timestamp || ins[i].FrameNb * 50 == ins[i].Timestamp + 1 || ins[i].FrameNb * 50 == ins[i].Timestamp + 2 || ins[i].FrameNb * 50 == ins[i].Timestamp + 3 || ins[i].FrameNb * 50 == ins[i].Timestamp - 1 || ins[i].FrameNb * 50 == ins[i].Timestamp - 2 || ins[i].FrameNb * 50 == ins[i].Timestamp - 3)
                {
                    outs[i].TimeoutOK = true;

                    fprintf(outputFile, "%s,", "true");
                }
                else
                {
                    outs[i].TimeoutOK = false;

                    fprintf(outputFile, "%s,", "false");
                    dropedFrames++;
                }
            }

            fprintf(outputFile, "%d\n", outs[i].FrameDropCnt);
        }
    }
    else
    {
        printf("\nIn order to use Log Decoder, please provide an input .csv file and an output .csv file.\nFor example: log_decoder.exe input_log.csv output_log.csv");
    }
    return 0;
}