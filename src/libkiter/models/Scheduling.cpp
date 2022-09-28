/*
 * Scheduling.cpp
 *
 *  Created on: 25 June 2021
 *      Author: Katherine
 */

#include "Scheduling.h"
#include <models/Dataflow.h>
#include <numeric>

void models::Scheduling::verbose_print() const {

	for (const auto&  key : this->_tasks_schedule) {
			auto task_vtx = _dataflow->getVertexById(key.first);

			std::cout << "Task " <<  _dataflow->getVertexName(task_vtx)
					<<  " : duration=[ " << commons::toString(_dataflow->getVertexPhaseDuration(task_vtx)) <<  "]"
					<<     " initial starts=[ " << commons::toString(key.second.initial_starts) << "]"
					<<     " periodic starts=[ " << commons::toString(key.second.periodic_starts.second) << "]"
					<<     " period=" <<  key.second.periodic_starts.first 
					<< std::endl;

	}
}


std::string models::Scheduling::asText () const{

		std::ostringstream returnStream;

		for (const auto& item : this->getTaskSchedule()) {
			ARRAY_INDEX tid = item.first;
			Vertex v = _dataflow->getVertexById(item.first);
			std::string  t_name = _dataflow->getVertexName(v);
			TIME_UNIT period = item.second.periodic_starts.first;//item.second.first;
			const std::vector<TIME_UNIT> &starts = item.second.initial_starts;//item.second.second;
			const std::vector<TIME_UNIT> &period_starts = item.second.periodic_starts.second;

			returnStream << std::setw(5) << tid << ") " << t_name
					<< " | starts:" << commons::toString(starts)
					<< " | period:" << period
					<< " | periodic starts:" << commons::toString(period_starts)
					<< " | durations:" << commons::toString(_dataflow->getVertexInitPhaseDuration(v))
			                           << ";[" << commons::toString(_dataflow->getVertexPhaseDuration(v)) << "]"
					<< std::endl;
		}


		return returnStream.str();

	}

std::string models::Scheduling::asASCII (size_t ls) const{


	std::ostringstream returnStream;
    ARRAY_INDEX idx = 0;

	for (const auto& item : this->getTaskSchedule()) {
		ARRAY_INDEX tid = item.first;
		Vertex v = _dataflow->getVertexById(item.first);
		const std::vector<TIME_UNIT> &starts = item.second.initial_starts;// initial starts
		const std::vector<TIME_UNIT> &period_starts = item.second.periodic_starts.second; //periodic starts
		TIME_UNIT period = item.second.periodic_starts.first;
		std::string start_str;
		std::string period_str;
		TIME_UNIT prev = 0;
		std::vector<TIME_UNIT> phases = _dataflow->getVertexPhaseDuration(v);
        size_t phase_count = phases.size();

		std::string line;
        size_t line_size = ls;
		// add to string 1) initial starts 2) trailing spaces (before periodic starts)
        size_t exec_idx = 0;
		for (TIME_UNIT i: starts) {
			std::string add_space(static_cast<unsigned long>(i - prev), ' ');
			start_str += add_space;
			std::string exec(static_cast<unsigned long>(phases[exec_idx % phase_count] - 1), '#');
			start_str += '@';
			start_str += exec;
			prev = i + exec.size() + 1;
			++exec_idx;
		}
		std::string leading_space(static_cast<unsigned long>(period_starts[0] - prev), ' ');
		start_str += leading_space; 
		if (start_str.length() <= line_size) {
			line += start_str;
            line_size -= start_str.length();
		}
		else {
			line += (start_str.substr(0, line_size));
            line_size = 0;
		}

		// create period string without leading spaces so to use for both first and subsequent periods
		if (!period_starts.empty()){
			prev = period_starts[0];
			for (TIME_UNIT i: period_starts) {
				std::string add_space(static_cast<unsigned long>(i - prev), ' ');
				period_str += add_space;
				std::string exec(static_cast<unsigned long>(phases[exec_idx % phase_count] - 1), '#');
				period_str += '@';
				period_str += exec;
				prev = i + exec.size() + 1;
				++exec_idx ;
			}
			// add trailing spaces after period starts (before next period)
			period_str += std::string(static_cast<unsigned long>((period + period_starts[0]) - prev), ' ');
			// need to complete the period for all phases (by looking ahead)
			if(phase_count > period_starts.size()){
				prev = period_starts[0];
				size_t phase_idx = 0;
				while (exec_idx % phase_count != 0){
					TIME_UNIT i = period_starts[phase_idx % period_starts.size()];
					std::string exec(static_cast<unsigned long>(phases[exec_idx % phase_count] - 1), '#');
					period_str += '@';
					period_str += exec;
					if (i == period_starts.back()){ // trailing spaces
						period_str += std::string (
                                static_cast<unsigned long>((period + period_starts[0]) - (i + exec.size()) - 1), ' ');
					} else { // add spaces
						period_str +=  std::string (
                                        static_cast<unsigned long>(period_starts[phase_idx % period_starts.size()] - i), ' ');
					}
					
					prev = i + exec.size() + 1;
					++phase_idx;
					++exec_idx;
				}
			}
			// continuously add period_str to line
			while (line_size > 0){
				if (line_size >= period_str.length()){
					line += period_str;
                    line_size -= period_str.length();
				} else {
					line += (period_str.substr(0, line_size));
                    line_size = 0;
				}
			}
		}

		++idx;

		returnStream << std::setw(5) << tid << " |" << line << std::endl;
	}

	return returnStream.str();
}



struct task_catalog {
	EXEC_COUNT cur_phase{};
	EXEC_COUNT task_Ni{};
	std::vector<TIME_UNIT> phase_durations;
	std::vector<TIME_UNIT> schedule;
};

bool models::Scheduling::is_valid_schedule () const{

	// Step 0: Setting up symbolic execution

	const models::Dataflow* g = this->getDataflow();
	scheduling_t s = this->getTaskSchedule();
	
	std::map<ARRAY_INDEX,EXEC_COUNT> buffer_load;

	{ForEachEdge(g,c) {
		buffer_load[g->getEdgeId(c)] = g->getPreload(c);
		// ###
		VERBOSE_INFO("EDGE ID: " << g->getEdgeId(c) << " | EDGE INIT " << buffer_load[g->getEdgeId(c)]);
		// ###
	}}


	std::map<ARRAY_INDEX, task_catalog> task_log;

	{ForEachVertex(g,t) {
		ARRAY_INDEX id = g->getVertexId(t); 
		EXEC_COUNT Ni =  g->getNi(t);
		task_catalog tc;
		tc.cur_phase = 0;
		tc.task_Ni = Ni;
		tc.phase_durations = g->getVertexPhaseDuration((g->getVertexById(id)));
		task_log[id] = tc;
	}}

	// Creating one centralized schedule vector for each task

	std::map<ARRAY_INDEX, EXEC_COUNT> exec_left;

	for (const std::pair<const ARRAY_INDEX, task_schedule_t>& task : s){

		ARRAY_INDEX task_id = task.first;

		task_log[task_id].schedule = task.second.initial_starts;
		exec_left[task_id] = task.second.initial_starts.size() + task_log[task_id].task_Ni;

		// total += task.second.initial_starts.size(); 

		for (int i = 0; i <= task_log[task_id].task_Ni; i++){
			TIME_UNIT task_period = s[task_id].periodic_starts.first;
			TIME_UNIT out_exec_time = s[task_id].periodic_starts.second[0];
			task_log[task_id].schedule.push_back(out_exec_time);
			s[task_id].periodic_starts.second.push_back(out_exec_time + task_period);
			s[task_id].periodic_starts.second.erase(s[task_id].periodic_starts.second.begin());
		}
	}

	// Filling out the schedules

	TIME_UNIT max_time = 0;
	for (auto task : task_log){
		if (task.second.schedule.back() > max_time){
			max_time = task.second.schedule.back();
		}
	}
	for (auto task : task_log){
		ARRAY_INDEX task_id = task.first;
		TIME_UNIT end_time = task.second.schedule.back();
		while (end_time < max_time){
			TIME_UNIT task_period = s[task_id].periodic_starts.first;
			TIME_UNIT out_exec_time = s[task_id].periodic_starts.second[0];
			task_log[task_id].schedule.push_back(out_exec_time);
			s[task_id].periodic_starts.second.push_back(out_exec_time + task_period);
			s[task_id].periodic_starts.second.erase(s[task_id].periodic_starts.second.begin());
			end_time = out_exec_time;	
		}
	};

	// ###
	for(const auto& task : task_log){ VERBOSE_INFO("TASK ID: " << task.first << " | PhaseDur: " << commons::toString(task.second.phase_durations) << " | Schedule: " << commons::toString(task.second.schedule)); }
	VERBOSE_INFO("REMAINING EXECUTIONS: " << commons::toString(exec_left));
	// ###
	
	// Step 1: Symbolic execution

	std::pair<ARRAY_INDEX, TIME_UNIT> next_task;

	// Tracking most recent next tasks of each task
	std::map<ARRAY_INDEX, TIME_UNIT> task_history ;
	
	// Initializing next task
	for (std::pair<const ARRAY_INDEX, task_catalog> task : task_log){
		if (task.second.schedule[0] == 0){
			next_task.first = task.first;
			next_task.second = 0;
		}
	}

	// ###
	VERBOSE_INFO("INIT TASK ID: " << next_task.first << " | EXEC TIME: " << next_task.second);
	// ###
    // This accumulate was already there, but I used a operator to adapt accumulate with map instead of vector
	while(std::accumulate(exec_left.begin(), exec_left.end(), 0,
                          [] (EXEC_COUNT value, const std::map<ARRAY_INDEX, EXEC_COUNT>::value_type& p)
    { return value + p.second; }) != 0){

		// Finding all tasks that completed execution before next_task is consumed

		std::vector<ARRAY_INDEX> out_tasks;

		for (const std::pair<const ARRAY_INDEX, task_catalog>& task : task_log){
			
			ARRAY_INDEX id = task.first;
			TIME_UNIT phase_time = (task_log[id].phase_durations)[static_cast<unsigned long>(task_log[id].cur_phase)];
			TIME_UNIT finish_time = phase_time + task_log[id].schedule[0];

			if (finish_time <= next_task.second){
				out_tasks.push_back(id);

				// ###
				VERBOSE_INFO("OUT TASK ID: " << id << " | FINISH TIME: " << finish_time << " | PHASE:" << task_log[id].cur_phase);
				// ###
			} 
		}

		// ###
		VERBOSE_INFO("OUT TASKS: " << commons::toString(out_tasks));
		// ###

		// Executing symbolic executon

		for (ARRAY_INDEX out_id : out_tasks){
			Vertex out_v = g->getVertexById(out_id);
			{ForOutputEdges(g,out_v,inE){
				TOKEN_UNIT reqCount = (g->getEdgeInVector(inE))[static_cast<unsigned long>(task_log[out_id].cur_phase)];
				TOKEN_UNIT inCount  = buffer_load[g->getEdgeId(inE)];
				buffer_load[g->getEdgeId(inE)] = inCount + reqCount;
			}}

		}

		Vertex in_v = g->getVertexById(next_task.first);
		{ForInputEdges(g,in_v,inE)	{
			TOKEN_UNIT reqCount = (g->getEdgeOutVector(inE))[static_cast<unsigned long>(task_log[next_task.first].cur_phase)];
			TOKEN_UNIT inCount  = buffer_load[g->getEdgeId(inE)];
			buffer_load[g->getEdgeId(inE)] = inCount - reqCount;
		
			// Invalid Schedule Conditon - negative buffer load
			if (buffer_load[g->getEdgeId(inE)] < 0) { return false; }
		}}

		for (ARRAY_INDEX out_id : out_tasks){
			task_log[out_id].cur_phase = static_cast<EXEC_COUNT>((static_cast<unsigned long>(
                    task_log[out_id].cur_phase + 1)) %
                                                                 task_log[out_id].phase_durations.size());
			task_log[out_id].schedule.erase(task_log[out_id].schedule.begin());
			if (exec_left[out_id] > 0){ exec_left[out_id]--; }
		}

		// ###
		VERBOSE_INFO("PREVIOUS TASK EXECUTION TIMES: " << commons::toString(task_history));
		// ###
		
		// ###
		VERBOSE_INFO("######## NEW ITERATION ###########");
		// ###

		// Finding next task in schedule
		task_history[next_task.first] = next_task.second;

		std::map<TIME_UNIT, ARRAY_INDEX> potential_next_tasks;

		for (std::pair<const ARRAY_INDEX, task_catalog> task : task_log){
		if (!(task.second.schedule.empty())){

			std::size_t i = 0;
			while(task.second.schedule[i] < next_task.second || task.second.schedule[i] <= task_history[task.first]){
				if (i + 1 == task.second.schedule.size()){ break; }
				i++;			
			}

			if (task.second.schedule[i] > task_history[task.first]){
			potential_next_tasks[task.second.schedule[i]] = task.first;
			}
		}}

		next_task.first = (potential_next_tasks.begin())->second;
		next_task.second = (potential_next_tasks.begin())->first;

		// ###
		for(const auto& task : task_log){ VERBOSE_INFO("TASK ID: " << task.first <<" | SCHEDULE " << commons::toString(task.second.schedule)); }
		VERBOSE_INFO("BUFFER LOAD: " << commons::toString(buffer_load));
		VERBOSE_INFO("REMAINING EXECUTIONS: " << commons::toString(exec_left));
		VERBOSE_INFO("NEXT TASK ID: " << next_task.first << " | EXEC TIME: " << next_task.second);
		// ### 

		// Invalid Schedule Condition - infeasible task execution
		TIME_UNIT task_duration = task_log[next_task.first].phase_durations[static_cast<unsigned long>(task_log[next_task.first].cur_phase)];
		if (not g->getReentrancyFactor(g->getVertexById(next_task.first))) // only when task has reentrancy loop
		if (next_task.second < task_history[next_task.first] + task_duration){
			VERBOSE_ERROR("Invalide scheduling condition: Next task " << next_task.first << " expected t=" << next_task.second << " is lower than t=" << task_history[next_task.first] + task_duration);
			return false;
		}

	}	

	return true;

}




	
