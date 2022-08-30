#include <iostream>
#include "magnum/fjson/parser.h"
#include "magnum/fjson/json_object.h"
// #include"../test/json/json_object.h"
// #include"../test/json/parser.h"
#include <fstream>

using namespace fjson;
// using namespace json;

struct Base
{
    int pp;
    std::string qq;

    START_FROM_JSON
    pp = from("pp", int);
    qq = from("qq", std::string);
    END_FROM_JSON

    START_TO_JSON
    to("pp") = pp;
    to("qq") = qq;
    END_TO_JSON
};

struct Mytest
{
    int id;
    std::string name;
    Base q;

    START_TO_JSON
    to_struct("base", q);
    to("id") = id;
    to("name") = name;
    END_TO_JSON

    START_FROM_JSON
    id = from("id", int);
    name = from("name", std::string);
    from_struct("base", q);
    END_FROM_JSON
};

void test_class_serialization()
{
    Mytest test{.id = 32, .name = "fda"};
    auto item = Parser::FromJSON<Mytest>(R"({"base":{"pp":0,"qq":""},"id":32,"name":"fda"} )");
    // auto item = Parser::FromJson<Mytest>(R"({"base":{"pp":0,"qq":""},"id":32,"name":"fda"} )"); // serialization
    std::cout << Parser::ToJSON(item);                                                          // deserialization
}

void test_string_parser()
{
    {
        std::ifstream fin("../../../../test/test.json");
        std::string text((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
        Parser p;
        p.init(text);
        auto q = p.parse();
        std::ofstream fout("../../../../test/test_out.json");
        fout << q.to_string();
    }
}

int main()
{
    test_class_serialization();
    test_string_parser();
}