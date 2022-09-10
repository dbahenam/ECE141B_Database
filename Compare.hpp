//
//  compare.hpp
//  Created by rick gessner on 4/30/21.
//

#ifndef compare_h
#define compare_h

#include <sstream>
#include "BasicTypes.hpp"

namespace ECE141{

//Equal ------------------------------------------
template <typename T>
bool isEqual(const T &arg1, const T &arg2) {
  return arg1==arg2;
}

bool isEqual(const std::string &aStr1, const std::string &aStr2) {
  return aStr1==aStr2;
}

template <typename T1, typename T2>
bool isEqual(const T1 &arg1, const T2 &arg2) {
  return static_cast<T1>(arg2)==arg1;
}

template <typename T1>
bool isEqual(const T1 &arg1, const std::string &aStr) {
  std::stringstream temp;
  temp << arg1;
  std::string theStr=temp.str();
  bool theResult=theStr==aStr;
  return theResult;
}

bool isEqual(const std::string &aStr, const bool &arg2) {
  return isEqual(arg2,aStr);
}

bool isEqual(const std::string &aStr, const int &arg2) {
  return isEqual(arg2,aStr);
}

bool isEqual(const std::string &aStr, const double &arg2) {
  return isEqual(arg2,aStr);
}

// Less than ------------------------------------------
template <typename T>
bool isLess(const T &arg1, const T &arg2) {
  return arg1 < arg2;
}

bool isLess(const std::string &aStr1, const std::string &aStr2) {
  return aStr1 < aStr2;
}

template <typename T1, typename T2>
bool isLess(const T1 &arg1, const T2 &arg2) {
  return static_cast<T1>(arg2) < arg1;
}

template <typename T1>
bool isLess(const T1 &arg1, const std::string &aStr) {
  std::stringstream temp;
  temp << arg1;
  std::string theStr=temp.str();
  bool theResult = theStr < aStr;
  return theResult;
}

bool isLess(const std::string &aStr, const bool &arg2) {
  return isLess(arg2,aStr);
}

bool isLess(const std::string &aStr, const int &arg2) {
  return isLess(arg2,aStr);
}

bool isLess(const std::string &aStr, const double &arg2) {
  return isLess(arg2,aStr);
}

// Greater than ------------------------------------------
template <typename T>
bool isGreater(const T &arg1, const T &arg2) {
  return arg1 > arg2;
}

bool isGreater(const std::string &aStr1, const std::string &aStr2) {
  return aStr1 > aStr2;
}

template <typename T1, typename T2>
bool isGreater(const T1 &arg1, const T2 &arg2) {
  return static_cast<T1>(arg2) > arg1;
}

template <typename T1>
bool isGreater(const T1 &arg1, const std::string &aStr) {
  std::stringstream temp;
  temp << arg1;
  std::string theStr=temp.str();
  bool theResult = theStr > aStr;
  return theResult;
}

bool isGreater(const std::string &aStr, const bool &arg2) {
  return isGreater(arg2,aStr);
}

bool isGreater(const std::string &aStr, const int &arg2) {
  return isGreater(arg2,aStr);
}

bool isGreater(const std::string &aStr, const double &arg2) {
  return isGreater(arg2,aStr);
}

//--------- feel free to add more operators... ------


struct Compare{

  static bool equals(Value &aLHS, Value &aRHS) {
	bool theResult=false;

	std::visit([&](auto const &aLeft) {
	 std::visit([&](auto const &aRight) {
	   theResult = isEqual(aLeft,aRight);
	 },aRHS);
	},aLHS);
	return theResult;
  }
  
  static bool lessThan(Value &aLHS, Value &aRHS){
	bool theResult = false;
	
	std::visit([&](auto const &aLeft){
	  std::visit([&](auto const &aRight){
		theResult = isLess(aLeft, aRight);
	  }, aRHS);
	}, aLHS);
	return theResult;
  }
  
  static bool greaterThan(Value &aLHS, Value &aRHS){
	bool theResult = false;
	
	std::visit([&](auto const &aLeft){
	  std::visit([&](auto const &aRight){
		theResult = isGreater(aLeft, aRight);
	  }, aRHS);
	}, aLHS);
	return theResult;
  }

};

}

#endif /* compare_h */
