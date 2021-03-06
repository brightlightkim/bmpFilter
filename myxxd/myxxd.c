#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/

FILE *parseCommandLine(int argc, char **argv, int *bits) {
  if (argc > 2) {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0)) {
    *bits = TRUE;
  } else {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsHex(unsigned char *data, size_t size) {
  unsigned int i;
  unsigned char *pc = (unsigned char*) data;

  for (i = 0; i < size; i++){
    if (i < size){
      printf("%02x", *(pc + i));
      printf("%02x ", *(pc + (i+1)));
      i++;
    }
  }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size) {
  for (unsigned int i = 0; i < size; i++){
    printf("%c", data[i]);
  }
}

void readAndPrintInputAsHex(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}


void printDataAsBits(unsigned char *data, size_t size){

  unsigned int i;

  int binary [7] = {0, };
  unsigned char *dataValue [16];
  memcpy( dataValue, data, size);

  for (i = 0; i < 8; i++){
    if (dataValue[i] != 0){
      if ((dataValue[i] % 2) == 1){
        binary [i] = 1;
      } else {
        binary [i] = 0;
      }
      dataValue[i] /= 2;
    }
    else {
      binary [i] = 0;
    }
  }

  for (i = 7; i >= 0; i--){
    printf("%d", binary[i]);
  }


  /*
  for (i = 0; i < size; i++){
    position = 0;
    while(TRUE){
      binary[position] = *(data + i) % 2;
      *(data + i)  = *(data + i) / 2;

      position++;

      if (*(data + i) == 0){
        break;
      }
    }
    for (int i = 7; i >= 0; i--){
      printf("%u", binary[i]);
    }
    printf(" ");
  }
  */
}
/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input) {
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0) {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

int main(int argc, char **argv) {
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE) {
    readAndPrintInputAsHex(input);
  } else {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
