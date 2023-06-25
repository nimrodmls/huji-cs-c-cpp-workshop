#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <functional>
#include <map>

#include "User.h"

using movie_comperator = 
	std::function<bool(const sp_movie&, const sp_movie&)>;
using movie_rank_pair = std::pair<sp_movie, double>;
using priority_comparator =
    std::function<bool(
        const movie_rank_pair&, const movie_rank_pair&)>;

using movie_features = std::vector<double>;
using movie_db = 
	std::map<sp_movie, movie_features, movie_comperator>;

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

    friend std::ostream& operator<<(
        std::ostream& os, const RecommendationSystem& rs);

private:
    static up_rank_map _normalize_ranks(const rank_map& user_ranks);
    static double _get_norm(const movie_features& vec);
    static double _dot_product(
        const movie_features& vec1, const movie_features& vec2);
    static double _calculate_movie_similarity(
        const movie_features& preferences,
        const movie_features& features);

    movie_features _calculate_preferences(
        const rank_map& normalized_ranks);
    
    movie_db _movies;
    size_t _feature_count;
};

#endif //RECOMMENDATIONSYSTEM_H
