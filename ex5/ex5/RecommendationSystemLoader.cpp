
#include "RecommendationSystemLoader.h"
#include <fstream>
#include <sstream>
#define YEAR_SEPARATOR '-'
#define ERROR_MSG "input file is incorrect"


/*ptr_type*/ RecommendationSystemLoader::create_rs_from_movies(const
std::string &movies_file_path) noexcept(false)
{
    /*ptr_type*/ rs = /*use the right std::make... */
    std::ifstream in_file;
    in_file.open(movies_file_path);
    std::string buffer;

    while (getline(in_file, buffer))
    {
        std::string movie_det;
        double ranking;
        std::istringstream splitted_line(buffer);
        splitted_line >> movie_det;
        std::vector<double> vec;
        while (splitted_line >> ranking)
        {
            if (ranking <= 0){
                throw std::invalid_argument(ERROR_MSG);
            }
            vec.push_back(ranking);
        }
        size_t end = buffer.find(YEAR_SEPARATOR);
        rs->add_movie(buffer.substr(0, end), std::stoi(buffer.substr(end + 1, buffer.length())), vec);
    }
    in_file.close();
    return rs;
}