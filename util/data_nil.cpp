#include "data_nil.h"

#include <sstream>

#include <math/math.h>

namespace am {
namespace util {
namespace data {

	const int Nil::TYPE = 7;
	const char *Nil::TYPENAME = "Nil";

	Nil::Nil()
	{
	}
	Nil::~Nil()
	{
	}

	IData *Nil::clone() const
	{
		return new Nil();
	}

	bool Nil::isNil() const
	{
		return true;
	}

	std::string Nil::toLua() const
	{
		return "nil";
	}

	Nil *Nil::checkDataType(IData *data, const char *className)
	{
		return IData::checkDataType<Nil>(data, className);
	}

}
}
}
