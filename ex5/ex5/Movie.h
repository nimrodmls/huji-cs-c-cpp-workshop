#ifndef INC_23B_C_C__EX5_MOVIE_H
#define INC_23B_C_C__EX5_MOVIE_H

#include <iostream>
#include <vector>
#include <memory>

#define HASH_START 17

class Movie;
typedef std::shared_ptr<Movie> sp_movie;

/**
 * those declartions and typedefs are given to you
 * and should be used in the ex
 */
typedef std::size_t (*hash_func)(const sp_movie& movie);
typedef bool (*equal_func)(const sp_movie& m1,const sp_movie& m2);
std::size_t sp_movie_hash(const sp_movie& movie);
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2);

/**
 * @class Movie
 * @brief Stores movie metadata
 */
class Movie
{
public:
    /**
     * constructor
     * @param name - name of movie
     * @param year - year it was made
     */
    Movie(const std::string& name, int year);
    ~Movie() = default;

    /**
     * returns the name of the movie
     * @return const ref to name of movie
     */
    const std::string& get_name() const;

    /**
     * returns the year the movie was made
     * @return year movie was made
     */
    int get_year() const;

private:
    std::string _name;
    int _release_year;
};

/**
 * operator<< for movie
 * @param os - ostream to output info with
 * @param movie - movie to output
 */
std::ostream& operator<<(std::ostream& os, const Movie& movie); 

/**
 * Comparison of order between two _movies' years
 * @param rhs - right hand side
 * @param lhs - left hand side
 * @return returns true if (lhs.year) < rhs.year or
 *         (rhs.year == lhs.year & lhs.name < rhs.name)
 *         else return false
 */
bool operator<(const Movie& lhs, const Movie& rhs);


#endif //INC_23B_C_C__EX5_MOVIE_H
