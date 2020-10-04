/* Header of user functions */
#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

template <typename T>
inline std::set<T> more_intersection(std::vector<std::set<T> > vector_sets){
	std::set<T> result;
	std::set<T> buffer;
	buffer = vector_sets[vector_sets.size() - 1];
	vector_sets.pop_back();
	for ( auto input_set : vector_sets ) {
		std::set_intersection(input_set.begin(), input_set.end(), buffer.begin(), buffer.end(), std::inserter(result, result.begin()));
		buffer = result;
		result.swap(buffer);
		buffer.clear();
	}
	return result;
}

std::vector<boost::gregorian::date_period> setToPeriods(std::set<boost::gregorian::date>);

#endif //
