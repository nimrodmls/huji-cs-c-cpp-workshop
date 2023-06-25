//
// Created on 2/21/2022.
//




#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <sstream>
#include <fstream>
#include <vector>
#include "User.h"

#define YEAR_SEPARATOR '-'


class UsersLoader
{
private:


public:
    UsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file
     *        of the users and their movie ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<User> create_users(
        const std::string& users_file_path, 
        up_rec_system rs) noexcept(false);

};


#endif //USERFACTORY_H
