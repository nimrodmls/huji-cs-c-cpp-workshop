
#include "UsersLoader.h"

#define INIT_BUCKET_SIZE 8
#define ERROR_MSG "input file is incorrect"


std::vector<User>
UsersLoader::create_users
(const std::string &users_file_path, /*ptr_type*/ rs)
noexcept(false)
{
    /* different but still smart ptr_type*/ s_rs = /* use std::move*/
    std::ifstream in_file;
    std::vector<User> users;
    in_file.open(users_file_path);
    std::string buffer;
    getline(in_file, buffer);
    std::istringstream movies_names(buffer);
    std::vector<sp_movie> movies;
    while (movies_names >> buffer)
    {
        size_t end = buffer.find(YEAR_SEPARATOR);
        sp_movie m = s_rs->get_movie(buffer.substr(0, end),
                                      std::stoi(buffer.substr(end + 1, buffer.length())));
        movies.push_back(m);
    }
    while (getline(in_file, buffer))
    {
        std::string user_name;
        std::string ranking;
        std::istringstream splitted_line(buffer);
        splitted_line >> user_name;
        int i = 0;
        rank_map ranks(INIT_BUCKET_SIZE,sp_movie_hash,sp_movie_equal);
        while (splitted_line >> ranking)
        {

            if (ranking != "NA")
            {
                int rating = std::stoi(ranking);
                if (rating <= 0){
                    throw std::invalid_argument(ERROR_MSG);
                }
                ranks[movies[i]] = rating;
            }
            i++;
        }
        users.emplace_back(user_name, ranks, s_rs);
    }
    in_file.close();
    return users;
}
