/*
 * luawraperror.h:
 *  Error functions used on conversion failure and lua runtime failure.
 */

#ifndef LUAWRAP_LUAWRAPERROR_H_
#define LUAWRAP_LUAWRAPERROR_H_

#include <string>
#include <luawrap/config.h>

namespace luawrap {
	int errorfunc(lua_State *L);
	std::string conversion_error_string(const std::string& type,
			const std::string& object_path, const std::string& object_repr);
}

#ifdef LUAWRAP_USE_EXCEPTIONS

#include <stdexcept>

struct lua_State;

namespace luawrap {
	class Error: public std::runtime_error {
	public:
		Error(const std::string& msg) :
				std::runtime_error(msg) {

		}
	};

	/* Make sure conversion errors are convenient to expand upon further down the stack */
	class ConversionError: public Error {
	public:
		ConversionError(const std::string& type, const std::string& object_path,
				const std::string& object_repr) :
				Error(conversion_error_string(type, object_path, object_repr)), _type(
						type), _object_path(object_path), _object_repr(
						object_repr) {

		}
		~ConversionError() throw() {
		}
		const std::string& type() const {
			return _type;
		}
		const std::string& object_path() const {
			return _object_path;
		}
		const std::string& object_repr() const {
			return _object_repr;
		}
	private:
		std::string _type, _object_path, _object_repr;
	};

	inline void error(const std::string& msg) {
		throw Error(msg);
	}

	inline void conversion_error(const std::string& type,
			const std::string& object_path, const std::string& object_repr) {
		throw ConversionError(type, object_path, object_repr);
	}
}

#else

#include <cstdio>
#include <cstdlib>

namespace luawrap {
	inline void error(const std::string& msg) {
		fprintf(stderr, msg);
		fflush(stderr);
		abort();
	}

	inline void conversion_error(const std::string& type,
			const std::string& object_path, const std::string& object_repr) {
		error(conversion_error_string(type, object_path, object_repr));
	}
}

#endif

#endif /* LUAWRAP_LUAWRAPERROR_H_ */
