/** ******************** *******
**auther:caiyunlong.2017.06.29
**email:ylcai@miivii.com
*******************************/
#include "db_common.h"
#include "mongdb_face.h"
#include "image_process.h"
#include "face_feature.h"
int featureDB::face_feature_get_person_count()
{
    return modb->lookup_db_person_count();
}
void featureDB::face_feature_getnamelist_db(std::vector<std::string> &namelist){
    namelist = modb->lookup_db_iterm_by_name();
}
int featureDB::face_feature_add_db(personinput person,Mat feature_input){
    int namecounter = 0;
    int feature_size=0;
    namecounter = modb->lookup_db_name_repeat(person.name);
    if (namecounter >= MAX_SAME_FEATURE){
        return 2;
    }
    if(get_facefeature_size(feature_input,feature_size)<0){
        cout<<"feature size is error"<<endl;
        return -1;
    }
    auto feature = make_unique<std::uint8_t[]>(feature_size);
    if(get_facefeature(feature_input,feature.get())<0){
        cout << "get feature error "<<endl;
        return -1;
    }
    modb->insert_people_info(person.name,person.gender,person.age,
                              feature.get(),feature_size,person.id_seq);
    return 0;
}
int featureDB::face_feature_query_db(string name,std::vector<databaseinfo> &result){
    modb->lookup_db_by_name(name,result);
    return 0;
}
void featureDB::face_feature_delete_db(string name){
    modb->delete_all_match_iterm(name);
}
void featureDB::face_feature_clear_db(){
    std::vector<std::string> namelist;
    namelist = modb->lookup_db_iterm_by_name();
    for (int i = 0; i < namelist.size(); ++i){
        face_feature_delete_db(namelist[i]);
    }
}
featureDB::featureDB(mongocxx::database db_out){
    modb = new mongodbparse(db_out);
}
featureDB::~featureDB(){
    delete modb;
}
