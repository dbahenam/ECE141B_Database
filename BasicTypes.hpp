//
//  BasicTypes.hpp
//  RGAssignement1
//
//  Created by rick gessner on 3/9/21.
//

#ifndef BasicTypes_h
#define BasicTypes_h

#include <string>
#include <variant>
#include <map>
#include<optional>
#include<vector>


namespace ECE141 {

	enum class DataTypes {
		no_type = 'N', bool_type = 'B', datetime_type = 'D',
		float_type = 'F', int_type = 'I', varchar_type = 'V', Count = 6,
	};
	using StringList = std::vector<std::string>;
	using StringMap = std::map<std::string, std::string>;
	using StringOpt = std::optional<std::string>;
	using IntOpt = std::optional<uint32_t>;
	using Value = std::variant<bool, int, double, std::string>;
	using ValueOpt = std::optional<Value>;
	using ValueList = std::vector<Value>;
	using KeyValues = std::map<const std::string, Value>;
}

#endif /* BasicTypes_h */

