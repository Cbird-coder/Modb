/**
***mongodb common use header
*Copyright (C) 2017
*create by Caiyunlong
*/
#ifndef _DB_COMMON_
#define _DB_COMMON_
#include "image_process.h"
//CXX STL
#include <cstdint>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string.h>
//mongodb
///bson 
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
///mongodb
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/gridfs/bucket.hpp>

//CXX
using namespace std;
//bson
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::stdx::make_unique;

typedef struct databaseinfo
{
    string gender;
    int id_p;
    int age;
    int datalen;
    Mat feature;
} databaseinfo;
#endif //end _DB_COMMON_
