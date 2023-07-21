//
// Created by 24565 on 6/1/2022.
//

#include "RecommendationSystemLoader.h"
#include "UsersLoader.h"

int main(){
    Movie m("A",1999);
    std::cout << m << std::endl;
    auto rs = RecommendationSystemLoader::create_rs_from_movies(
        "C:\\Projects\\huji-cs-c-cpp-workshop\\ex5\\ex5\\RecommendationSystemLoader_input.txt");
    std::cout << *rs << std::endl;
    auto users = UsersLoader::create_users(
        "C:\\Projects\\huji-cs-c-cpp-workshop\\ex5\\ex5\\UsersLoader_input.txt",std::move(rs));
    std::cout << users[0] << std::endl;
}