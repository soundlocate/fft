#ifndef _COMMANDLINEOPTIONS_H
#define _COMMANDLINEOPTIONS_H

#include <argp.h>
#include <string>

#include <iostream>

#include "config.h"

class CommandLineOptions {
public:
    CommandLineOptions(int argc, char ** argv) {
        argp_parse(&argp, argc, argv, 0, 0, &args);
    }

    std::string audioIp() { return args.audioIp; }

    bool useFPGA() { return args.useFPGA; }

    unsigned short audioPort() { return args.audioPort; }

    unsigned short fftPort() { return args.fftPort; }

    unsigned int fftSize() { return args.fftSize; }

    unsigned int windowingFunction() { return args.windowingFunction; }

    unsigned int samplerate() { return args.samplerate; }

    unsigned int fftPerSec() { return args.fftPerSec; }

    double threshold() { return args.threshold; }

private:
    // documentation of the program
    std::string doc = "soundFFT -- apply fft on a audio stream\v"
                      "AUDIOIP\t\tip address of a audio server\n"
                      "AUDIOPORT\tthe respective port of the audio server\n"
                      "PORT\t\tthe port to open the fftserver on";

    // documentation of non option arguments
    char args_doc[255] = "AUDIOIP AUDIOPORT PORT";

    // "-f " + fftSize, "-s " + samplerate, "-r " + fftPerSec, "-w " +
    // windowingFunction

    // supported options (no custom options for now)
    struct argp_option options[7] = {
        {"usefpga", 'u', 0, 0,
         "convert the data to float (use if connecting to a fpga)"},
        {"fftsize", 'f', "COUNT", 0, "size of fft"},
        {"samplerate", 's', "COUNT", 0, "samplerate"},
        {"fftpersec", 'r', "COUNT", 0, "number of ffts per sec"},
        {"threshold", 't', "DOUBLE", 0, "filter threshold"},
        {"windowingfunction", 'w', "STRING", 0, "one of [hamm, hann, rect]"},
        {0}};

    // struct for holding the argument info
    struct arguments {
        std::string    audioIp;
        bool           useFPGA = false;
        unsigned short audioPort;

        unsigned short fftPort;

        unsigned int windowingFunction = 0; // 0 == hamm, 1 == hann, 2 == rect
        unsigned int fftSize           = 192000;
        unsigned int samplerate        = 192000;
        unsigned int fftPerSec         = 10;
        double       threshold         = 0.01;
    };

    // parsing of a single argument
    static error_t parse_opt(int key, char * arg, struct argp_state * state) {

        // get the struct for saving the arguments
        arguments * arguments = (struct arguments *)state->input;

        switch(key) {
		case 'u':
			arguments->useFPGA = true;
			break;
        case 't':
            try {
                arguments->threshold = std::stod(arg);
            } catch(std::invalid_argument) {
                std::cout << "invalid threshold " << arg << std::endl;
                argp_usage(state);
            }

            break;
        case 's':
            try {
                arguments->samplerate = std::stoi(arg);
            } catch(std::invalid_argument) {
                std::cout << "invalid samplerate " << arg << std::endl;
                argp_usage(state);
            }

            break;
        case 'f':
            try {
                arguments->fftSize = std::stoi(arg);
            } catch(std::invalid_argument) {
                std::cout << "invalid fftSize " << arg << std::endl;
                argp_usage(state);
            }

            break;
        case 'r':
            try {
                arguments->fftPerSec = std::stoi(arg);
            } catch(std::invalid_argument) {
                std::cout << "invalid fftPerSec " << arg << std::endl;
                argp_usage(state);
            }

            break;
        case 'w': {
            std::string s(arg);
            if(s == "hamm") {
                arguments->windowingFunction = 0;
            } else if(s == "hann") {
                arguments->windowingFunction = 1;
            } else if(s == "rect") {
                arguments->windowingFunction = 2;
            } else {
                std::cout << "invalid windowing function " << arg << std::endl;
                argp_usage(state);
            }

            break;
        }
        case ARGP_KEY_ARG: /* non option argument */
            // check if more than all needed arguments are passed (and abort if
            // so)
            if(state->arg_num >= 3) { argp_usage(state); }

            // parse the arguments
            try {
                switch(state->arg_num) {
                case 0: /* audioip */
                    arguments->audioIp = std::string(arg);
                    break;
                case 1: /* audioport */
                    arguments->audioPort = std::stoi(arg);
                    break;
                case 2:
                    arguments->fftPort = std::stoi(arg);
                    break;
                }
            } catch(std::invalid_argument) {
                std::cout << "invalid port: " << arg << std::endl;
                argp_usage(state);
            }

            break;

        case ARGP_KEY_END: /* end of arguments */
                           // check for too few non option arguments
            if(state->arg_num < 3) { argp_usage(state); }
            break;

        default:
            return ARGP_ERR_UNKNOWN;
        }
        return 0;
    }

    // argp parser definiton
    struct argp argp = {options, parse_opt, args_doc, doc.c_str()};

    arguments args;
};

#endif
