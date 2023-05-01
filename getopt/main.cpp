/*	A modest getopt tutorial - example really.
	Perry Kivolowitz
	Carthage College
*/

#include <iostream>
#include <getopt.h>
#include <string>

using namespace std;

/*	This program is a modest tutorial on the use of getopt.

	getopt is a very old C library that facilitates the parsing of
	command line arguments. This tutorial (more like an example) is not
	complete. You can find documentation on getopt online.
*/


int main(int argc, char * argv[]) {
	/*	getopt is typically isolated in a function or method
		specifically intended to process command line options. Below, I
		have defined some variables of different types and will pass
		these to the getopt managing function. Notice I have initialized
		these variables with default values.

		That is, if the corresponding option is not specified on the
		command line, the default value is retained. 

		One of these variables will be *required* - you'll see how this
		can be implemented.
	*/

	/*	Forward references to supporting functions. Of course, the
		forward references would not be needed using a c++ class or by
		reordering these functions.
	*/
	void PrintUsage();
	bool HandleOptions(int argc, char ** argv, int &, bool &, char **, string &, float &);

	int an_integer = 17;
	float a_float = 12.345;
	bool a_bool = false;
	char * a_c_string = nullptr;
	string a_cpp_string_this_will_be_required;

	if (!HandleOptions(argc, argv, an_integer, a_bool, &a_c_string, a_cpp_string_this_will_be_required, a_float)) {
		PrintUsage();
		return 1;
	}
	cout << "An integer: " << an_integer << endl;
	cout << "A bool:     " << a_bool << endl;
	cout << "A c-string: ";
    cout << ((a_c_string == nullptr) ? "" : a_c_string) << endl;
	cout << "A string:   " << a_cpp_string_this_will_be_required << endl;
	cout << "A float:    " << a_float << endl;
	return 0;
}

/*	This is not the traditional usage format but is fairly informative
*/
void PrintUsage() {
	cerr << "Usage:" << endl;
    cerr << "-h              prints this help and exits\n";
	cerr << "-s string       this is required." << endl;
	cerr << "-i integer      optional - an integer" << endl;
	cerr << "-b              optional - if present a boolean is set to true" << endl;
	cerr << "-c string       optional - a c string" << endl;
	cerr << "-s string       optional - a c++ string" << endl;
	cerr << "-f float        optional - a float" << endl;
	cerr << "Note that without care, a string is just one \"word\"" << endl;
}

/*  Having a massive number of parameters is NOT the only way to do this
    or even the preferred way to do this. Think of a better way that
    doesn't use global variables.
*/

bool HandleOptions(int argc, char ** argv, int & an_int, 
    bool & a_bool, char ** a_c_string, string & a_cpp_string, 
    float & a_float)
{
	int c;

	/*	getopt must be passed argc and argv - after all these are how
		command line arguments are passed to programs.

		The third parameter is a format string defining the valid
		command line options as well as if a specific option requires a
		parameter. This is controlled by whether or not a colon (':')
		comes after the option character. In the following example, 's'
		requires a parameter but 'b' does not.

		When there are no more members of argv to attempt parsing,
		getopt return -1.

		Notice there is no requirement to keep to any particular order
		of options.
	*/

	/*	Notice where optarg is declared? Me neither. It is a global char
		pointer brought in with getopt.h. It is initialized to a command
		line option's argument if it has one. Bad things can happen if
		you make use of optarg when a command line option does NOT have
		an argument.
	*/
	while ((c = getopt(argc, argv, "bhf:i:c:s:")) != -1) {
		switch (c) {
			default:
			case 'h':
				// Short circuit forcing the usage to be printed by
				// returning false from this function.
				return false;

			case 'i':
				an_int = atoi(optarg);
				break;

			case 'b':
				a_bool = true;
				break;

			case 'c':
				*a_c_string = optarg;
				break;

			case 's':
				a_cpp_string = string(optarg);
				break;

			case 'f':
				a_float = atof(optarg);
				break;
		}
	}

	/*	The s option is to be required. If the string variable still has
		length 0, the s option was not given. Note that this is just one
		construction of the test. I can use this construction because
		there is only one way HandleOptions can fail.
	*/
	return a_cpp_string.size() > 0;
}
