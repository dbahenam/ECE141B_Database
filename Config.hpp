//
//  Config.hpp
//
//  Created by rick gessner on 2/27/22.
//

#ifndef Config_h
#define Config_h
#include <sstream>
#include <filesystem>
#include "Timer.hpp"

namespace ECE141 {

  enum class CacheType : int {block=0, rows, views};

  struct Config {

    static const char* getDBExtension() {return ".db";}

	static const std::string getStoragePath() {

		#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)

		  return std::filesystem::temp_directory_path().string();

		#elif __APPLE__ || defined __linux__ || defined __unix__

		  return std::string("/tmp");

		#endif

	  }
    
    static std::string getDBPath(const std::string &aDBName) {
      std::ostringstream theStream;
      theStream << Config::getStoragePath() << "/" << aDBName << ".db";
      return theStream.str();
    }
      
    static Timer& getTimer() {
      static Timer theTimer;
      return theTimer;
    }
        
  };

}

#endif /* Config_h */
