#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

#define MAX_NUM_ARGS 3
#define MIN_NUM_ARGS 2
#define DATA_LINE_SIZE 16


/**
 * Display tool usage.
 */
void display_usage(void);


/**
 * Read in_file binary and write a byte array declaration to out_file.
 * @param in_file Filename containing binary data.
 * @param out_file C filename to hold the array declaration.
 * @return number of bytes extracted from binary file.
 */
int process_bin_file(const char *in_filename, const char *out_filename);


/**
 *  ********* main ***********
 */
int main(int argc, char **argv)
{
    if( argc == 3 )
    {
        process_bin_file(argv[1], argv[2]);
    }
    else
    {
        display_usage();
    }
    
    return 0;
}


/*
 *  Display tool usage.
 */
void display_usage(void)
{
    cout << "Usage: bintoc <input_file> <output_file>\n";
}


/**
 * Read in_file binary and write a byte array declaration to out_file.
 * @param in_file Filename containing binary data.
 * @param out_file C filename to hold the array declaration.
 * @return number of bytes extracted from binary file.
 */
int process_bin_file(const char *in_filename, const char *out_filename)
{
    static char data;
    ifstream in_file;
    ofstream out_file;
    unsigned int num_bytes;
    unsigned int n;

    in_file.open(in_filename, ios::in | ios::binary | ios::ate);
    if( in_file.fail() )
    {
        cout << "Failed to open file '" << in_filename << "'\n";
        return 0;
    }
    else
    {
        num_bytes = in_file.tellg();
        in_file.seekg(ios::beg);
        cout << "Opened " << num_bytes << " byte file '" << in_filename << "'\n";
    }

    out_file.open(out_filename, ios::out | ios::trunc);
    if( out_file.fail() )
    {
        cout << "Failed to open file '" << out_filename << "'\n";
        in_file.close();
        return 0;
    }

    out_file << setfill('0');
    out_file << "static const uint8_t BINARY_DATA[] =\n{";
    for(n=0; n<num_bytes; n++)
    {
        in_file.read(&data, 1);
        if( in_file.eof() )
        {
            break;
        }
        else
        {
            if( n % DATA_LINE_SIZE == 0 )
            {
                out_file << "\n    0x";
            }
            else
            {
                out_file << ", 0x";
            }
            out_file << setw(2) << uppercase << hex << (int)data;
        }
        if( (n & 0xFF) == 0 )
        {
            cout << n << " bytes processed...\n";
        }
    }

    out_file << "\n}\n";

    in_file.close();
    out_file.close();

    cout << n << " bytes processed\n";
    return n;
}