//
//  FolderReader.hpp
//  Database5
//
//  Created by rick gessner on 4/4/20.
//  Copyright © 2020 rick gessner. All rights reserved.
//

#ifndef FolderReader_h
#define FolderReader_h

#include <string>
#include <filesystem>
#include <fstream>

namespace fs =  std::filesystem;

namespace ECE141 {
  
  using FileVisitor = std::function<bool(const std::string&)>;

  class FolderReader {
  public:
            FolderReader(const char *aPath) : path(aPath) {}
    virtual ~FolderReader() {}
    
    virtual bool exists(const std::string &aFilename) {
      std::ifstream theStream(aFilename);
      return !theStream ? false : true;
    }
    
    virtual void each(const std::string &anExt,
                      const FileVisitor &aVisitor) const {
     fs::path thePath(path); // make a filesystem path with given path name
     for(auto& anEntry : fs::directory_iterator(path)){
         if(!anEntry.is_directory()){ // not a directory
             fs::path anEntryPath(anEntry.path()); //make path of entry
             if(anEntryPath.extension() != anExt){ //check extension
                 continue;
             }
             else{
                 aVisitor(anEntryPath.string()); //call visitor
             }
         }
     }
    };
    
    std::string path;
  };
  
}

#endif /* FolderReader_h */
