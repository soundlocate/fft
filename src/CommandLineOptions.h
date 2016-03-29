#ifndef _COMMANDLINEOPTIONS_H
#define _COMMANDLINEOPTIONS_H

#include <string>
#include <argp.h>

#include <iostream>

#include "config.h"

class CommandLineOptions {
public:
	CommandLineOptions(int argc, char ** argv) {
		argp_parse(&argp, argc, argv, 0, 0, &args);
	}

	std::string audioIp() {
		return args.audioIp;
	}

    unsigned short audioPort() {
		return args.audioPort;
	}

    unsigned short fftPort() {
		return args.fftPort;
	}
private:

	// documentation of the program
	std::string doc = "soundFFT -- apply fft on a audio stream\v"
		"AUDIOIP\t\tip address of a audio server\n"
		"AUDIOPORT\tthe respective port of the audio server\n"
		"PORT\t\tthe port to open the fftserver on";

	// documentation of non option arguments
    char args_doc[255] = "AUDIOIP AUDIOPORT PORT";

	// supported options (no custom options for now)
    struct argp_option options[2] = {
		{ 0 }
	};

	// struct for holding the argument info
	struct arguments
	{
		std::string audioIp;
	    unsigned short audioPort;

	    unsigned short fftPort;
	};

	// parsing of a single argument
	static error_t parse_opt (int key, char *arg, struct argp_state *state) {

		// get the struct for saving the arguments
		arguments * arguments = (struct arguments *) state->input;

		switch (key)
		{
		case ARGP_KEY_ARG: /* non option argument */
            // check if more than all needed arguments are passed (and abort if so)
			if (state->arg_num >= 3) {
				argp_usage(state);
			}

			// parse the arguments
			try {
				switch (state->arg_num) {
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
			if (state->arg_num < 3) {
				argp_usage(state);
			}
			break;

		default:
			return ARGP_ERR_UNKNOWN;
		}
		return 0;
	}

	// argp parser definiton
	struct argp argp = { options, parse_opt, args_doc, doc.c_str() };

	arguments args;
};

#endif
