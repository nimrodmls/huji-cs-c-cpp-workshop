#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <functional>
#include <map>

#include "User.h"

using movie_comperator = 
	std::function<bool(const sp_movie&, const sp_movie&)>;

using movie_db = 
	std::map<sp_movie, std::vector<double>, movie_comperator>;

class RecommendationSystem
{
public:
    explicit RecommendationSystem();

    // Explicitly defining copy & move to prevent implicit defs
    RecommendationSystem(
        const RecommendationSystem&) = delete;
    RecommendationSystem& operator=(
        const RecommendationSystem&) = delete;
    RecommendationSystem(
        const RecommendationSystem&&) = delete;
    RecommendationSystem& operator=(
        const RecommendationSystem&&) = delete;
    ~RecommendationSystem() = default;

    /**
     * adds a new movie to the system
     * @param name - name of movie
     * @param year - year it was made
     * @param features - features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(
        const std::string& name,
        int year,
        const std::vector<double>& features);

    /**
     * gets a shared pointer to movie in system
     * @param name - name of movie
     * @param year - year movie was made
     * @return shared pointer to movie in system
     */
    sp_movie get_movie(const std::string& name, int year) const;

    /**
     * A function that calculates the movie with
     * highest score based on movie features
     * @param user - The user to recommend to
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const User& user);

    /**
     * A function that calculates the movie with highest
     * predicted score based on ranking of other _movies
     * @param user - The user to recommend to
     * @param movie_count - The size of the movies
     *                      set to recommend from
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int movie_count);


    /**
     * Predict a user rating for a movie given argument
     * using item cf procedure with k most similar _movies.
     * @param user - ranking to use
     * @param movie -  movie to predict
     * @param movie_count - The size of the movies set to recommend
     *                      from, used in cf algorithm
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(
        const User& user, const sp_movie& movie, int movie_count);
private:
    movie_db _movies;
};

std::ostream& operator<<(std::ostream& os, const RecommendationSystem& rs);

#endif //RECOMMENDATIONSYSTEM_H
