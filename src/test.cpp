/** ******************** *******
**auther:caiyunlong.2017.06.29
**email:ylcai@miivii.com
*******************************/
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include "face_feature.h"

int main(int, char**) { 
    /* 使用database 程序的入口必须先创建数据库连接*/
    mongocxx::instance inst{};
    mongocxx::client client{mongocxx::uri{}};
    mongocxx::database facedb= client["facedb"];
    /***********************/
    featureDB parse_test(facedb);
    /******************/
    Mat feature_input;
    int age;
    string gender;
    std::vector<string> names;
    int flag0,flag1;
    vector<string>::iterator it;
    vector<databaseinfo> person_result;

    personinput testperson;
    testperson.name = "常山赵子龙";
    testperson.gender = "男";
    testperson.age = 27;
    testperson.id_seq = 897578;
    /******************/
    feature_input = imread("./data/1.jpg",1);
    cout<<"person count:"<<parse_test.face_feature_get_person_count()<<endl;
    flag0 = parse_test.face_feature_add_db(testperson,feature_input);
    if (flag0 < 0 || flag0 == 2)
        goto end;
     parse_test.face_feature_getnamelist_db(names);
    //test
    for(it=names.begin();it!=names.end();++it)
    {
        cout << "name:"<<*it<<endl;
    }
    // test end
    parse_test.face_feature_delete_db("阿瞒");
    cout<<"after delete:::::::::"<<endl;
    parse_test.face_feature_getnamelist_db(names);
    //test
    for(it=names.begin();it!=names.end();++it)
    {
        cout << "name:"<<*it<<endl;
    }
    // test end
    flag1 = parse_test.face_feature_query_db("常山赵子龙",person_result);
    if (flag1 < 0)
        goto end;
    for (int i = 0; i < person_result.size(); i++)
    {
        char showstr[100] ="database";
        sprintf(showstr,"feature_out_%d",i);
        imshow(showstr,person_result[i].feature);
        waitKey(-1);
        cout << "final gender:"<<person_result[i].gender<<endl;
        cout << "final age:"<<person_result[i].age<<endl;
        cout << "final id:"<<person_result[i].id_p<<endl;
    }
end: cout<<"game over"<<endl;
 //parse_test.face_feature_clear_db();
}
