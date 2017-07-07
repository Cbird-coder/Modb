/** ******************** *******
**auther:caiyunlong.2017.06.29
**email:ylcai@miivii.com
*******************************/

#include "db_common.h"
#include "mongdb_face.h"
#include "image_process.h"
///////////////add//////////////////////////////////
bsoncxx::types::value mongodbparse::upload_file_to_db(uint8_t *bytes,int len)
{
    string filename = "uploader_files";
    auto bucket = db.gridfs_bucket();
    auto uploader = bucket.open_upload_stream(filename);
    uploader.write(bytes, len);
    auto result = uploader.close();
    return result.id();
}
int mongodbparse::download_file_from_db(bsoncxx::types::value id,uint8_t* out_buffer)
{
    int chunk_count =0;
    auto bucket = db.gridfs_bucket();
    auto downloader = bucket.open_download_stream(id);
    auto file_length = downloader.file_length();
    if (file_length <=0){
        return -1;
    }
    auto bytes_counter = 0;
    auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
    auto buffer = make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));

    while(auto length_read=downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size))) 
    {
        memcpy(out_buffer+chunk_count*buffer_size,buffer.get(),buffer_size);
        bytes_counter += static_cast<std::int32_t>(length_read);
        chunk_count++;
    }
    return bytes_counter;
}
int mongodbparse::insert_people_info(string name,string gender,int age,uint8_t * bytes,int datalen,int id_seq){
    bsoncxx::types::value id = upload_file_to_db(bytes,datalen);
    //get data
    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc =builder
    <<"id_p"<<id_seq
    <<"name"<< name
    <<"gender"<<gender
    <<"age"<< age
    <<"feature"<<id
    <<"datalen"<<datalen<< finalize;
    collection.insert_one(doc.view());
    return 0;
}
//////////////////////////////////////////////////
///////////////delete////////////////////
int mongodbparse::delete_db_files(string name)
{
    auto bucket = db.gridfs_bucket();
    auto filter = document{} << "name"<< name << finalize;
    mongocxx::options::find opts{};
    opts.projection(document{} <<"feature"<<1<<finalize);
    auto cursor = collection.find(filter.view(), opts);
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["feature"];
        if (element.type() == bsoncxx::type::k_oid)
        {
            bsoncxx::types::value id(element.get_oid());
            bucket.delete_file(id);
        }
    }
    return 0;
}
int mongodbparse::delete_db_one_file(string name){
    auto bucket = db.gridfs_bucket();
    auto filter = document{} << "name"<< name << finalize;
    mongocxx::options::find opts{};
    opts.projection(document{} <<"feature"<<1<<finalize);
    auto cursor = collection.find(filter.view(), opts);
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["feature"];
        if (element.type() == bsoncxx::type::k_oid)
        {
            bsoncxx::types::value id(element.get_oid());
            bucket.delete_file(id);
            break;
        }
    }
    return 0;
}
int mongodbparse::delete_db_all_files(){
    auto bucket = db.gridfs_bucket();
    mongocxx::cursor cursor = collection.find({});
     for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["feature"];
        if (element.type() == bsoncxx::type::k_oid)
        {
            bsoncxx::types::value id(element.get_oid());
            bucket.delete_file(id);
        }
    } 
    return 0;
}

int mongodbparse::delete_all_match_iterm(string name){
    delete_db_files(name);
    bsoncxx::builder::stream::document filter_builder;
    filter_builder << "name"<< name;
    collection.delete_many(filter_builder.view());
    return 0;
}
int mongodbparse::delete_one_match_iterm(string name){
    delete_db_one_file(name);
    bsoncxx::builder::stream::document filter_builder;
    filter_builder << "name"<< name;
    collection.delete_one(filter_builder.view());

    return 0;
}
int mongodbparse::clear_database(){
    delete_db_all_files();
    collection.delete_many({});
    return 0;
}
int mongodbparse::drop_database(){//delete database from mongodb
    collection.drop();
    return 0;
}
///////////////////////////////////////
/////////////update database/////////////
int mongodbparse::update_db_name(string in_name,string out_name){

    bsoncxx::builder::stream::document filter_builder, update_builder;
        filter_builder << "name"
                       << in_name;
        update_builder << "$set" << open_document << "name"<< out_name << close_document<< 
                          "$currentDate" << open_document<< "lastModified" << true << close_document;
        collection.update_many(filter_builder.view(), update_builder.view());

        mongocxx::cursor cursor = collection.find({});
        for (const bsoncxx::document::view& doc : cursor) {
            std::cout << bsoncxx::to_json(doc) << std::endl;
        }
    return 0;
}
/////////////////////////////////////////////////////////////
///////////////////////look up database//////////////////////
std::vector<std::string> mongodbparse::lookup_db_iterm_by_name(){
    std::vector<std::string> namelist;
    mongocxx::cursor cursor = collection.find({});
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["name"];
        if (element.type() == bsoncxx::type::k_utf8)
        {
            std::string name = element.get_utf8().value.to_string();
            namelist.push_back(name);
        }
    }
    //delete repeat name
    sort(namelist.begin(), namelist.end());  
    namelist.erase(unique(namelist.begin(),namelist.end()),namelist.end());
    //return name list  
    return namelist;
}

int mongodbparse::lookup_db_person_count(){
    int counter_total = 0;
    mongocxx::cursor cursor = collection.find({});
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["name"];
        if (element.type() == bsoncxx::type::k_utf8)
        {
            ++counter_total;
        }
    }
    return counter_total;
}

int mongodbparse::lookup_db_name_repeat(string name){
    int counter = 0;
    mongocxx::cursor cursor = collection.find({});
    for (const bsoncxx::document::view& doc : cursor) {
        bsoncxx::document::element element = doc["name"];
        if (element.type() == bsoncxx::type::k_utf8)
        {
            std::string name_ele = element.get_utf8().value.to_string();
            if (name_ele == name){
                ++counter;
            }
        }
    }
    return counter;
}


int mongodbparse::lookup_db_by_name(string name,std::vector<databaseinfo> &persons)
{
    databaseinfo persontemp;
    bool get_age_gender = true;
    // Create the query filter
    auto filter = document{} << "name"<< name << finalize;
    // Create the find options with the projection
    mongocxx::options::find opts{};
    opts.projection(document{}
        <<"id_p"<<1 
        <<"feature"<<2
        <<"age"<<3
        <<"gender"<<4
        <<"datalen"<<5<< finalize);
    // Execute find with options
    auto cursor = collection.find(filter.view(), opts);
    for (const bsoncxx::document::view& doc : cursor) {
            bsoncxx::document::element element_id_p = doc["id_p"];
            if (element_id_p.type() == bsoncxx::type::k_int32){
                 persontemp.id_p  = element_id_p.get_int32().value;//get age from database
            }

            bsoncxx::document::element element_age = doc["age"];
            if (element_age.type() == bsoncxx::type::k_int32){
                persontemp.age  = element_age.get_int32().value;//get age from database
            }

            bsoncxx::document::element element_gender = doc["gender"];
            if (element_gender.type() == bsoncxx::type::k_utf8){
                persontemp.gender = element_gender.get_utf8().value.to_string();//get gender from database
            }
            bsoncxx::document::element element_datalen = doc["datalen"];
            if (element_datalen.type() == bsoncxx::type::k_int32){
                 persontemp.datalen = element_datalen.get_int32().value;//get gender from database
            }

            bsoncxx::document::element element = doc["feature"];
            if (element.type() == bsoncxx::type::k_oid){
                auto feature_out = make_unique<std::uint8_t[]>(persontemp.datalen);
                bsoncxx::types::value download_id(element.get_oid());
                download_file_from_db(download_id,feature_out.get());
                if(restore_facefeature(feature_out.get(),persontemp.feature) < 0)
                 {
                    cout << "wrong feature type"<<endl;
                    return -1;
                }
                //std::cout << bsoncxx::to_json(doc) << std::endl;
            }
            persons.push_back(persontemp);
    }
    return 0;
}
/////////////////////////////////////////////////////////////
///////////////// test //////////////////////////////
int mongodbparse::upload_download_test(){
        char tempchar[10];
        auto out_bytes = make_unique<std::uint8_t[]>(10); 
        std::uint8_t bytes[10] = {72, 101, 108, 108, 111, 87, 111, 114, 108, 100};
        bsoncxx::types::value id = upload_file_to_db(bytes,10);
        std::string oid = id.get_oid().value.to_string();
        std::cout << "OID: " << oid << std::endl;

        download_file_from_db(id,out_bytes.get());
        memcpy(tempchar,out_bytes.get(),10);
        cout << tempchar<<endl;
        return 0;
    }
int mongodbparse::insert_test(){
    std::vector<bsoncxx::document::value> documents;
    for(int i = 0; i < 100; i++) {
        documents.push_back(bsoncxx::builder::stream::document{} << 
            "name" << "hhhhhjjjj"<<
            "haha"<<i+1<<
            "feature"<<"hhh"<< 
            finalize);
    }
    collection.insert_many(documents);
    return 0;
}
mongodbparse::mongodbparse(mongocxx::database db_out){
    db=db_out;
    collection = db["facecollection"]; 
}
mongodbparse::~mongodbparse(){
}