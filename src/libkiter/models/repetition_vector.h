/*
 * repetition_vector.h
 *
 *  Created on: 22 avr. 2013
 *      Author: toky
 *
 *      The computeRepetitionVector function is hightly inspired by the one from SDF3 tool
 *      Source: http://www.es.ele.tue.nl/sdf3/
 *
 */

#ifndef REPETITION_VECTOR_H_
#define REPETITION_VECTOR_H_

namespace models {
	class Dataflow;
        class NaiveDataflow;
}

bool computeRepetitionVector(models::Dataflow *from);

bool generic_repetition_vector (models::NaiveDataflow *from) ;


#endif /* REPETITION_VECTOR_H_ */
