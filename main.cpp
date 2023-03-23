#include <iostream>
#include "TolerantSearchTesting.h"
#include "IndexersTesting.h"
#include "BooleanSearch.h"
#include "TwowordCoordinateIndicesTesting.h"
#include "SPIMITesting.h"
using namespace testing;

int main()
{
	//tolerant_search::execute_testing();
	boolean_search::execute_boolean_search();
	//indexers::execute_testing();
	//two_word_index::execute_testing();
	//coordinate_index::execute_testing();
	//spimi::execute_testing();
	return EXIT_SUCCESS;
}