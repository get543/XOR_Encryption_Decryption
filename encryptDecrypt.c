#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Clear console
void clearConsole()
{
#ifdef _WIN32
    // For Windows
    system("cls");
#else
    // For Unix/Linux and MacOS
    system("clear");
#endif
}

void pressEnterToContinue()
{
    printf("\n\nPress Enter to continue...");
    getchar();  // Wait for the user to press Enter
}

void encryptTextToHex(const char *inString, const char *key, char *outHex) 
{
    size_t keyLength = strlen(key);
    size_t inLength = strlen(inString);

    for (size_t i = 0; i < inLength; i++) 
    {
        unsigned char encryptedChar = inString[i] ^ key[i % keyLength];
        sprintf(outHex + (i * 2), "%02X", encryptedChar);       // Convert each byte to hex
    }
    outHex[inLength * 2] = '\0';                                // Null-terminate the hex string
}

void decryptHexToText(const char *hexString, const char *key, char *outString) 
{
    size_t keyLength = strlen(key);
    size_t hexLength = strlen(hexString);
    size_t inLength = hexLength / 2;                            // Each byte is represented by 2 hex characters

    for (size_t i = 0; i < inLength; i++) 
    {
        unsigned char encryptedChar;
        sscanf(hexString + (i * 2), "%2hhX", &encryptedChar);   // Convert hex back to byte
        outString[i] = encryptedChar ^ key[i % keyLength];      // XOR to decrypt
    }
    outString[inLength] = '\0';                                 // Null-terminate the output string
}

void encryptDecryptText(int encOrDec, const char *key)
{
    char inputString[256], encryptedHex[512], decryptedText[256];

    // Clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);                // Flush stdin to remove leftover characters

    // Prompt user for input
    printf(encOrDec == 1 ? "\nText you want to encrypt: " : "\nText you want to decrypt: ");

    fgets(inputString, sizeof(inputString), stdin);             // Use fgets for safe input handling
    inputString[strcspn(inputString, "\n")] = 0;                // Remove the newline character if present

    // Validate input string
    if (strlen(inputString) == 0)
    {
        printf("Input string is empty. Please provide valid input.\n");
        pressEnterToContinue();
        return;
    }

    if (encOrDec == 1)
    {   
        // Encrypt the text into hex
        encryptTextToHex(inputString, key, encryptedHex);
        printf("\nEncrypted Text (Hex): \n%s", encryptedHex);
    }
    else if (encOrDec == 2)
    {
        // Decrypt the hex back into text
        decryptHexToText(inputString, key, decryptedText);
        printf("\nDecrypted Text: \n%s", decryptedText);
    }
}


void encryptDecryptFile(int encOrDec, char const *key)
{
    char sourceFilename[50];
    char destinationFilename[50];

    FILE *sourceFile, *destinationFile;

    // !Source filename to encrypt or decrypt
    printf(encOrDec == 1 ? "\nSource filename to encrypt: " : "\nSource filename to decrypt: ");
    scanf("%s", &sourceFilename);

    // !Destination for encrypted or decrypted file
    printf(encOrDec == 1 ? "Encrypted filename: " : "Decrypted filename: ");
    scanf("%s", &destinationFilename);

    if (strcmp(sourceFilename, destinationFilename) == 0)   // string compare (equal 0 means the same, equal negative/positive means different) 
    {
        printf("\nSource file name and destination file name cannot be the same.");
        pressEnterToContinue();
        return;
    }

    sourceFile = fopen(sourceFilename, "rb");               // Open file in read binary mode
    destinationFile = fopen(destinationFilename, "wb");     // Open file in write binary mode

    if (destinationFile == NULL || sourceFile == NULL)
    {
        printf("Error opening file.");
        fclose(sourceFile);
        fclose(destinationFile);
        pressEnterToContinue();
        return;
    }

    int byte;
    size_t i = 0;
    size_t keyLength = strlen(key);

    while ((byte = fgetc(sourceFile)) != EOF)
    {
        // Perform XOR operation with the key
        fputc(byte ^ key[i % keyLength], destinationFile);
    }

    // Success message
    if (encOrDec == 1)
        printf("\n%s has been successfully encrypted to %s\n", sourceFilename, destinationFilename);
    else if (encOrDec == 2)
        printf("\n%s has been successfully decrypted to %s\n", sourceFilename, destinationFilename);

    // Close file
    fclose(sourceFile);
    fclose(destinationFile);
}

// Main function
int main()
{
    int textOrFile;
    int encOrDec;

    char key[256]; // key

    // clearConsole();

    printf("-----------------------------------------\n");
    printf("|         Encrypt or Decrypt ?          |\n");
    printf("-----------------------------------------\n");
    printf("| 1 |           Encrypt                 |\n");
    printf("| 2 |           Decrypt                 |\n");
    printf("-----------------------------------------\n");
    printf("Choose: ");
    scanf("%d", &encOrDec);

    clearConsole();

    if (encOrDec == 1) // encrypt
    {
        printf("\n");
        printf("-----------------------------------------\n");
        printf("|           File or String ?            |\n");
        printf("-----------------------------------------\n");
        printf("| 1 |        Encrypt a file             |\n");
        printf("| 2 |        Encrypt a string           |\n");
        printf("-----------------------------------------\n");
        printf("Choose: ");
        scanf("%d", &textOrFile);

        printf("\nEnter your encryption key: ");
        scanf("%99s", &key);

        if (textOrFile == 1)
            encryptDecryptFile(encOrDec, key); // encrypt file
        else if (textOrFile == 2)
            encryptDecryptText(encOrDec, key); // ecrypt text
        else
            printf("\nInvalid option.\n");
    }
    else if (encOrDec == 2) // decrypt
    {
        printf("\n");
        printf("-----------------------------------------\n");
        printf("|               File or String ?        |\n");
        printf("-----------------------------------------\n");
        printf("| 1 |           Decrypt a file          |\n");
        printf("| 2 |           Decrypt a string        |\n");
        printf("-----------------------------------------\n");
        printf("Choose: ");
        scanf("%d", &textOrFile);

        printf("\nEnter your decryption key: ");
        scanf("%99s", &key);

        if (textOrFile == 1)
            encryptDecryptFile(encOrDec, key); // decrypt file
        else if (textOrFile == 2)
            encryptDecryptText(encOrDec, key); // decrypt text
        else
            printf("\nInvalid option.\n");
    }
    else
        printf("\nInvalid option.\n");

    

    pressEnterToContinue();

    return 0;
}
