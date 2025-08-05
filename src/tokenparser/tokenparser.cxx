#include <tokenparser/tokenparser.hxx>

namespace Tokenparser {

	std::istream  *_input_stream;
	std::ostream *_output_stream;


	void use(std::istream &input_stream) {
	        _input_stream = &input_stream;
	}

	void use(std::ostream &output_stream) {
	        _output_stream = &output_stream;
	}

	int proc() {
		while(_input_stream && _input_stream->peek() != EOF) {
			Token c_token;
			_input_stream >> c_token;

			*_output_stream << c_token.name << std::endl;
		}

		return 0;
	}
}
