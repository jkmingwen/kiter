/*
 * This program is a Dataflow Analysis Tool : Kiter
 *
 * */

#include <string>
#include <iostream>
#include <commons/KiterRegistry.h>
#include <commons/verbose.h>
#include <printers/printers.h>
#include <commons/commons.h>



inline double tock() {
	static struct timespec last;
	struct timespec clockData;
	clock_gettime(CLOCK_MONOTONIC, &clockData);
	double diff = clockData.tv_sec - last.tv_sec + (clockData.tv_nsec - last.tv_nsec) / 1000000000.0;
	last = clockData;
	return diff;
}
//!< List of algorithms







inline void activate_verbose(const std::string p) {
	if (commons::fromString<int>(p) <= 0) {
		VERBOSE_INFO("activate_verbose with unsupported value: " << p);
		commons::add_custom_verbose_mode(p);
		//std::exit(EXIT_FAILURE);
	} else {
		VERBOSE_INFO("activate_verbose with level: " << p);
		commons::set_verbose_mode(commons::fromString<int>((optarg)));
	}
}

int main (int argc, char **argv)
{

	// ** default arguments
	std::string filename = "";
	std::vector<std::pair<std::string,parameters_list_t>> algos;
	std::vector<std::pair<std::string,parameters_list_t>> generators;
	parameters_list_t parameters;
	commons::set_verbose_mode(commons::WARNING_LEVEL); // default verbose mode is ...

	// ** Retreive arguments **
	int c;
	while ((c = getopt(argc, (char **)argv, "f:g:a:v:p:h")) != -1) {
		if (c == 'f') {
			filename = optarg;
		}
		if (c == 'a') {
			VERBOSE_INFO("Store algo " << optarg);
			algos.push_back(std::pair<std::string,parameters_list_t>(optarg,parameters));
			parameters.clear();
		}
		if (c == 'g') {
			VERBOSE_INFO("Store generator " << optarg);
			generators.push_back(std::pair<std::string,parameters_list_t>(optarg,parameters));
			parameters.clear();
		}
		if (c == 'v') {
			activate_verbose(optarg);
		}
		if (c == 'p') {
			std::string arg = optarg;
			if (arg.find('=',0) != arg.npos) {
				parameters[arg.substr(0,arg.find('=',0))] = arg.substr(arg.find('=',0)+1,arg.size());
			}

		}
		if (c == 'h') {
			std::cerr << "Command: kiter -f FILENAME or -g GENERATOR, then -a ALGORITHM." << std::endl;
			KiterRegistry<>::print_all(std::cerr);
			exit(0);
		}
	}

	VERBOSE_DEBUG("Parameter parsing is done: " << algos.size() << " algo identified.");

	/*
	 * Here, we gathered two important values :
	 * - filename (default is stdin)
	 * - algo (default is print)
	 *
	 * */

	models::Dataflow* csdf = nullptr;


    if ((filename == "") and (generators.size() == 0)) {
        VERBOSE_ERROR("Unsupported arguments: need a filename or a generator");
        std::cerr << " No input file or enable to read it, please us -f FILENAME or -g GENERATOR." << std::endl;
        exit(1);
    }


	// Step 1 = Load XML file if any
	if (filename != "") {
		VERBOSE_INFO("Read XML file : " << filename);
		csdf = printers::readSDF3File(filename);
	}

	if (generators.size() > 1) {
		VERBOSE_ERROR("Unsupported case, only one generator can be used.");
	}

	if (csdf and (generators.size() > 0)) {
		VERBOSE_ERROR("Unsupported case, cannot used file (-f) and generator (-g) at the same time.");
	}

	if (!csdf) {
		for ( std::vector<std::pair<std::string,parameters_list_t>>::iterator it = generators.begin() ; it != generators.end() ; it++ ) {
			std::string name = (*it).first;
			const generator_t* generator = KiterRegistry<generator_t>::get(name);
			if (generator)  {
				VERBOSE_INFO ("Run " << generator->name);
				tock();
				csdf = generator->fun(it->second);
				double duration = tock();
				VERBOSE_INFO (generator->name << " duration=" << duration);
			} else {
				std::cerr << " Unsupported generator (-g NAME) or no filename (-f FILENAME), list of supported generator is " << std::endl;
                KiterRegistry<generator_t>::print(std::cerr);
				exit(1);
			}

		}
	}


	if (!csdf) {
		VERBOSE_ERROR("Error with XML file or Generator.");
		std::cerr << " No input file or enable to read it, please us -f FILENAME or -g GENERATOR." << std::endl;
		exit(1);
	}


	// Step 2 = Check parameters
	for ( parameters_list_t::iterator it = parameters.begin() ; it != parameters.end() ; it++ ) {
		VERBOSE_DEBUG("Additionnal parameter found: '" << it->first << "' = '" << it->second << "'");
	}




	// Step 4 = Apply selected algorithm
	for ( std::vector<std::pair<std::string,parameters_list_t>>::iterator it = algos.begin() ; it != algos.end() ; it++ ) {
		std::string name = (*it).first;
        const transformation_t* transformation = KiterRegistry<transformation_t>::get(name);
        const buffer_sizing_t* buffer_sizing = KiterRegistry<buffer_sizing_t>::get(name);
        const printer_t* printer = KiterRegistry<printer_t>::get(name);

        tock();

        if (transformation) {
            VERBOSE_INFO ("Run transformation " << transformation->name);
            (*it).second.insert(parameters.begin(),parameters.end());
            transformation->fun(csdf,(*it).second);
        } else if (buffer_sizing) {
            VERBOSE_INFO ("Run buffer sizing " << transformation->name);
            (*it).second.insert(parameters.begin(),parameters.end());
            BufferSizingResult res = buffer_sizing->fun(csdf,(*it).second);
            VERBOSE_INFO ("Total size is " << res.total_size());

        } else if (printer) {
            VERBOSE_INFO ("Run printer " << transformation->name);
            (*it).second.insert(parameters.begin(),parameters.end());
            printer->fun(csdf,(*it).second);

        } else {
            std::cerr << " Unsupported algorithm (-a " << name << "), list of supported algorithms is " << std::endl;
            KiterRegistry<>::print_all(std::cerr);
            exit(1);
        }

        double duration = tock();
        VERBOSE_INFO (transformation->name << " duration=" << duration);
	}



	exit(0);

}
