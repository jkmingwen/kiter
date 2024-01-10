/*
 * This program is a Dataflow Analysis Tool : Kiter
 *
 * */

#include <string>
#include <iostream>
#include <commons/verbose.h>
#include <commons/basic_types.h>
#include <getopt.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <models/Dataflow.h>
#include <generators/NormalizedCycleGenerator.h>
#include <algorithms/liveness/LivenessModularDSE.h>

#define CSV_SEPARATOR ' '
#define CSV_HAS_HEADER false

//#define CSV_SEPARATOR ','
//#define CSV_HAS_HEADER true

struct dataset_generator_params {
    size_t thread;
    std::string input_file;
    std::string output_file;
    ARRAY_INDEX cycle_size;
    TOKEN_UNIT min_weight;
    TOKEN_UNIT max_weight;
    int verbosity;
    ARRAY_INDEX count = -1;

};

typedef  std::vector<TOKEN_UNIT> Cycle;


void print_cycle(const Cycle & cycle) {
    std::string delim = "";
    for (auto val : cycle) {
        std::cout << delim << val ;
        delim = CSV_SEPARATOR;
    }
}

class CycleGenerator {
public:


    CycleGenerator(TOKEN_UNIT cycle_size, TOKEN_UNIT min_weight, TOKEN_UNIT max_weight) : cycle_size(cycle_size), min_weight(min_weight) , max_weight(max_weight) {
    }
    void load(std::string filename) {
        current_cycle.clear();
        std::ifstream infile(filename);
        if (infile.good()) {
            std::string line;
            while (std::getline(infile, line)) {
                std::istringstream iss(line);
                Cycle cycle(cycle_size);
                for (TOKEN_UNIT i = 0; i < cycle_size; i++) {
                    if (!(iss >> cycle[i])) {
                        std::cerr << "Error parsing input file: invalid format" << std::endl;
                        return;
                    }
                }
                if (cycle[0] > max_weight) {
                    std::cerr << "Error parsing input file: invalid cycle weight" << std::endl;
                    return;
                }
                current_cycle = cycle;
            }
            infile.close();
        } else {
            std::cerr << "Error opening input file" << std::endl;
        }
    }

    bool has_next() {
        // returns true if there are more cycles to generate, false otherwise
        for (auto e : current_cycle){
            if (e != max_weight) return true;
        }
        return (current_cycle.empty());
    }



    const Cycle get_next_cycle(const Cycle& previous) {
        Cycle next = previous;
        for (int i = next.size() - 1; i >= 0; --i) {
            if (next[i] < this->max_weight) {
                next[i]++;
                break;
            } else {
                next[i] = this->min_weight;
            }
        }
        return next;
    }



    Cycle get_next() {

        if (current_cycle.empty()) {
            // initialize the first cycle
            current_cycle = Cycle(cycle_size, this->min_weight);
        } else {
            current_cycle = get_next_cycle(current_cycle);
        }
        return current_cycle;
    }


private:
    TOKEN_UNIT cycle_size;
    TOKEN_UNIT min_weight;
    TOKEN_UNIT max_weight;
    Cycle current_cycle;

};

models::Dataflow * create_graph(const Cycle & c) {
    models::Dataflow * g = generators::new_normalized_cycle( c.size() , c, std::vector<TIME_UNIT>(c.size(),1.0), std::vector<TOKEN_UNIT>(c.size(),0)) ;
    return g;
}



TOKEN_UNIT compute_minimal_size (const Cycle & c, size_t thread) {
    models::Dataflow *graph = create_graph(c);
    algorithms::dse::TokenConfigurationSet res = algorithms::dse::solve_liveness   (graph, false, thread) ;
    VERBOSE_ASSERT (res.getBestPerformancePoint()->getPerformance().throughput > 0, "Solution should be alive.");
    delete graph;
    return res.getBestPerformancePoint()->getCost();
}

dataset_generator_params parse_aguments (int argc, char **argv) {

    dataset_generator_params params;
    // ** Retreive arguments **
    int c;
    while ((c = getopt(argc, (char **)argv, "s:m:w:i:o:c:t:v:h")) != -1) {
        if (c == 'i') { params.input_file = optarg;}
        if (c == 'o') { params.output_file = optarg;}
        if (c == 's') { params.cycle_size =  std::atoi(optarg);}
        if (c == 'm') { params.min_weight =  std::atoi(optarg);}
        if (c == 'w') { params.max_weight =  std::atoi(optarg);}
        if (c == 'c') { params.count =  std::atoi(optarg);}
        if (c == 't') { params.thread =  std::atoi(optarg);}
        if (c == 'v') { params.verbosity =  std::atoi(optarg);}

        if (c == 'h') {
            std::cerr << "Command: dataset_generator -s cycle_size -w max_weight -i inputfile -o outputfile." << std::endl;
            exit(0);
        }
    }
    return params;
}

void print_csv_header (TOKEN_UNIT cycle_size) {
    for (TOKEN_UNIT i = 0 ; i < cycle_size; i++) {
        std::cout << "Z_" <<  i <<  CSV_SEPARATOR ;
    }
    std::cout << "liveness_condition" << std::endl;
}
void print_csv_line (const Cycle & cycle, size_t thread) {
    TOKEN_UNIT res = compute_minimal_size(cycle, thread);
    print_cycle(cycle);
    std::cout << CSV_SEPARATOR << res << std::endl;
}

int main (int argc, char **argv) {

    commons::set_verbose_mode(commons::WARNING_LEVEL); // default verbose mode is ...

    dataset_generator_params params = parse_aguments (argc, argv);
    commons::set_verbose_mode(params.verbosity);
    CycleGenerator generator (params.cycle_size, params.min_weight, params.max_weight);

    //if (params.input_file != "") generator.load(params.input_file);
    if (CSV_HAS_HEADER) print_csv_header(params.cycle_size);
    while (params.count != 0 and generator.has_next()) {
        Cycle cycle = generator.get_next();
        print_csv_line(cycle, params.thread);
        if (params.count > 0) params.count -= 1;
    }



    exit(0);

}
