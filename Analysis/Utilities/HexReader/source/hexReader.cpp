///@file hexReader.cpp
///@brief A program that outputs the unpacked events in a hex output
///@author C. R. Thornsberry and S. V. Paulauskas
///@date July 2, 2015

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>

#include <cstdlib>
#include <cstring>

#define HEAD 1145128264 // Run begin buffer
#define DATA 1096040772 // Physics data buffer
#define SCAL 1279345491 // Scaler type buffer
#define DEAD 1145128260 // Deadtime buffer
#define DIR 542263620   // "DIR "
#define PAC 541278544   // "PAC "
#define ENDFILE 541478725 // End of file buffer
#define ENDBUFF -1 // End of buffer marker

/* Extract a string from a character array. */
std::string csubstr(char *str_, unsigned int start_index_=0){
    std::string output = "";
    unsigned int index = start_index_;
    while(str_[index] != '\0' && str_[index] != ' '){
        output += str_[index];
        index++;
    }
    return output;
}

template <typename T>
std::string convert_to_hex(T input_, bool to_text_=false){
    std::bitset<sizeof(T)*8> set(input_);
    std::stringstream stream;
    if(!to_text_){ stream << std::hex << std::uppercase << set.to_ulong(); }
    else{ stream << std::uppercase << set.to_ulong(); }
    std::string output = stream.str();
    if(!to_text_ && output.size() < sizeof(T)*2){
        std::string temp = "0x";
        for(unsigned int i = output.size(); i < sizeof(T)*2; i++){
            temp += '0';
        }
        return temp + output;
    }
    else if(to_text_ && output.size() < (sizeof(T)+1)*2){
        std::string temp = "";
        for(unsigned int i = output.size(); i < (sizeof(T)+1)*2; i++){
            temp += ' ';
        }
        return temp + output;
    }

    if(!to_text_){ return "0x" + output; }
    return output;
}

bool show_raw = false;
bool convert = false;
bool show_zero = true;
bool do_search = false;

template <typename T>
void go(std::ifstream *input_, unsigned int &buff_count, unsigned int &good_buff_count, unsigned int &total_count,
        bool show_raw, bool convert, bool show_zero, bool do_search, T search_int, T buffer_select){
    bool good_buffer;
    int show_next = 0;
    unsigned int count = 0;
    unsigned int word_count = 0;

    T word;

    if(buffer_select != 0){ good_buffer = false; }
    else{ good_buffer = true; }

    while(true){
        input_->read((char*)&word, sizeof(T));
        if(input_->eof()){
            if(buff_count > 1){
                std::cout << " Buffer Size: " << word_count << " words\n";
                std::cout << "============================================================================================================================\n";
            }
            break;
        }

        if(do_search){
            if(show_next > 0){
                std::cout << convert_to_hex(word) << "  ";
                if(convert){ std::cout << convert_to_hex(word, true) << "  "; }
                show_next = show_next - 1;
                if(show_next <= 0){ std::cout << std::endl; }
            }
            else if(word == search_int){
                std::cout << convert_to_hex(word) << "  ";
                if(convert){ std::cout << convert_to_hex(word, true) << "  "; }
                show_next = 4;
            }
            total_count++;
            word_count++;
            continue;
        }

        // Check for end of buffer
        /*if(word == ENDBUFF || (!show_zero && word == 0)){
            total_count++;
            continue;
        }*/
        if((!show_zero && word == 0)){
            total_count++;
            word_count++;
            continue;
        }

        // Check for a new buffer
        if(word == HEAD || word == DATA || word == SCAL || word == DEAD || word == DIR || word == PAC || word == ENDFILE){ // new buffer
            buff_count++;
            if(buffer_select != 0){
                if(word == buffer_select){ good_buffer = true; }
                else{ good_buffer = false; }
            }

            if(good_buffer){
                good_buff_count++;
                if(buff_count > 1){
                    if(show_raw){ std::cout << "\n"; }
                    std::cout << "\n Buffer Size: " << word_count << " words\n";
                    std::cout << "============================================================================================================================\n";
                }
                std::cout << "\n============================================================================================================================\n";
                std::cout << " Buffer Number: " << buff_count << std::endl;
                std::cout << " Buffer Type: " << convert_to_hex(word);
                if(word == HEAD){ std::cout << " \"HEAD\"\n"; }
                else if(word == DATA){ std::cout << " \"DATA\"\n"; }
                else if(word == ENDFILE){ std::cout << " \"EOF \"\n"; }
                else if(word == SCAL){ std::cout << " \"SCAL\"\n"; }
                else if(word == DEAD){ std::cout << " \"DEAD\"\n"; }
                else if(word == DIR){ std::cout << " \"DIR\"\n"; }
                else if(word == PAC){ std::cout << " \"PAC\"\n"; }
                std::cout << " Total Count: " << total_count << " words\n";
                count = 0;
            }
            word_count = 0; // New buffer. Reset the buffer word count
        }

        if(!good_buffer){ // We don't care about this buffer
            total_count++;
            word_count++;
            continue;
        }

        total_count++;
        word_count++;
        if(show_raw){
            if(count == 0){ std::cout << "\n0000  "; }
            else if(count % 10 == 0){
                std::stringstream stream; stream << count;
                std::string temp_string = stream.str();
                std::string padding = "";
                if(temp_string.size() < 4){
                    for(unsigned int i = 0; i < (4 - temp_string.size()); i++){
                        padding += "0";
                    }
                }
                std::cout << "\n" << padding << temp_string << "  ";
            }
            std::cout << convert_to_hex(word) << "  "; count++;
            if(convert){ std::cout << convert_to_hex(word, true) << "  "; count++; }
        }
    }
}

/* Print help dialogue for command line options. */
void help(char * prog_name_){
    std::cout << "  SYNTAX: " << prog_name_ << " <filename> [options]\n";
    std::cout << "   --type <int>   | Only display buffers of a certain type\n";
    std::cout << "   --raw          | Display raw buffer words (hidden by default)\n";
    std::cout << "   --convert      | Attempt to convert words to Ascii characters\n";
    std::cout << "   --search <int> | Search for an integer in the stream\n";
    std::cout << "   --zero         | Suppress zero output\n";
    std::cout << "   --word <int>   | Specify the file word size\n";
    std::cout << "   --offset <int> | Specify the start word of the file\n\n";
    std::cout << "  Typical Buffer Types:\n";
    std::cout << "   \"HEAD\" 1145128264\n";
    std::cout << "   \"DATA\" 1096040772\n";  // Physics data buffer
    std::cout << "   \"SCAL\" 1279345491\n";  // Scaler type buffer
    std::cout << "   \"DEAD\" 1145128260\n";  // Deadtime buffer
    std::cout << "   \"DIR \" 542263620\n";   // "DIR "
    std::cout << "   \"PAC \" 541278544\n";   // "PAC "
    std::cout << "   \"EOF \" 541478725\n\n"; // End of buffer marker
}

int main(int argc, char *argv[]){
    if(argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)){
        help(argv[0]);
        return 1;
    }
    else if(argc < 2){
        std::cout << " Error: Invalid number of arguments to " << argv[0] << ". Expected 1, received " << argc-1 << ".\n";
        help(argv[0]);
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if(!input.is_open()){
        std::cout << " Error: failed to open input file\n";
        return 1;
    }

    std::streampos foffset = 0;

    int buffer_select = 0;
    int search_int = 0;
    int word_size = 4;
    bool show_raw = false;
    bool convert = false;
    bool show_zero = true;
    bool do_search = false;
    int index = 2;
    while(index < argc){
        if(strcmp(argv[index], "--type") == 0){
            if(index + 1 >= argc){
                std::cout << " Error! Missing required argument to '--type'!\n";
                help(argv[0]);
                return 1;
            }
            buffer_select = atoi(argv[++index]);
            std::cout << " Displaying only buffer type " << convert_to_hex(buffer_select) << "\n";
        }
        else if(strcmp(argv[index], "--raw") == 0){
            show_raw = true;
        }
        else if(strcmp(argv[index], "--convert") == 0){
            convert = true;
        }
        else if(strcmp(argv[index], "--search") == 0){
            if(index + 1 >= argc){
                std::cout << " Error! Missing required argument to '--search'!\n";
                help(argv[0]);
                return 1;
            }
            do_search = true;
            search_int = atoi(argv[++index]);
            std::cout << " Searching for " << search_int << " (" << convert_to_hex(search_int) << ")\n";
        }
        else if(strcmp(argv[index], "--zero") == 0){
            show_zero = false;
        }
        else if(strcmp(argv[index], "--word") == 0){
            if(index + 1 >= argc){
                std::cout << " Error! Missing required argument to '--word'!\n";
                help(argv[0]);
                return 1;
            }
            word_size = atoi(argv[++index]);
            if(word_size != 1 && word_size != 2 && word_size != 4 && word_size != 8){
                std::cout << " Error: Invalid word size (" << word_size << " bytes)!\n";
                return 1;
            }
            std::cout << " Using word size of " << word_size << " bytes\n";
        }
        else if(strcmp(argv[index], "--offset") == 0){
            if(index + 1 >= argc){
                std::cout << " Error! Missing required argument to '--offset'!\n";
                help(argv[0]);
                return 1;
            }
            foffset = strtoll(argv[++index], NULL, 0);
            std::cout << " Starting at word no. " << foffset << " in file.\n";
        }
        else{
            std::cout << " Error! Unrecognized option '" << argv[index] << "'!\n";
            help(argv[0]);
            return 1;
        }
        index++;
    }

    input.seekg(foffset*word_size);

    unsigned int good_buff_count = 0;
    unsigned int total_count = 0;
    unsigned int buff_count = 0;

    if(word_size == 1){ go<unsigned char>(&input, buff_count, good_buff_count, total_count, show_raw, convert, show_zero, do_search, search_int, buffer_select); }
    else if(word_size == 2){ go<unsigned short>(&input, buff_count, good_buff_count, total_count, show_raw, convert, show_zero, do_search, search_int, buffer_select); }
    else if(word_size == 4){ go<unsigned int>(&input, buff_count, good_buff_count, total_count, show_raw, convert, show_zero, do_search, search_int, buffer_select); }
    else{ go<unsigned long long>(&input, buff_count, good_buff_count, total_count, show_raw, convert, show_zero, do_search, search_int, buffer_select); }

    input.close();

    if(!do_search){
        std::cout << "\n\n Read " << total_count << " " << word_size << " byte words (";
        total_count *= word_size;
        std::cout << total_count << " bytes)\n";
        std::cout << "  Found " << buff_count << " total buffers\n";
        if(buffer_select != 0){ std::cout << "  Found " << good_buff_count << " " << convert_to_hex(buffer_select) << " buffers\n"; }
    }

    return 0;
}

