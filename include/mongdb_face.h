/** ******************** *******
**auther:caiyunlong.2017.06.29
**email:ylcai@miivii.com
*******************************/

#ifndef MONGODB_H_
#define MONGODB_H_
#include "db_common.h"
class mongodbparse
{
private:
    /*database and connection*/
    mongocxx::collection collection;
    mongocxx::database db;

    /*delete feature from database*/
    int delete_db_files(string name);
    int delete_db_one_file(string name);
    int delete_db_all_files();
     /*upload and download binary file to database */
    bsoncxx::types::value upload_file_to_db(uint8_t *bytes,int len);
    int download_file_from_db(bsoncxx::types::value id,uint8_t* out_buffer);
public:
    /* add bson document to database */
    int insert_people_info(string name,string gender,int age,uint8_t * bytes,int datalen,int id_seq);
    /* delete bson document from database*/
    int delete_all_match_iterm(string name);
    int delete_one_match_iterm(string name);
    int clear_database();
    int drop_database();//delete database from mongodb
    /*update some iterm in database*/
    int update_db_name(string in_name,string out_name);
    /*look up database,and find out the iterm*/
    int lookup_db_person_count();
    std::vector<std::string> lookup_db_iterm_by_name();
    int lookup_db_name_repeat(string name);
    int lookup_db_by_name(string name,std::vector<databaseinfo> &persons);
    /* some test function */
    int upload_download_test();
    int insert_test();
    /////////////////////////
    mongodbparse(mongocxx::database db_out);
    ~mongodbparse();

};
#endif
