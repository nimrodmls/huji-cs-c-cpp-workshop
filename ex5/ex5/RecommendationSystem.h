#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <functional>
#include <map>
#include <queue>

#include "User.h"

// Defines the prototype for comparison between movies
// which complies with the definition of an STD comperator
using movie_comperator = 
	std::function<bool(const sp_movie&, const sp_movie&)>;

// Movie-Rank pairing for use in the priority queue
using movie_rank_pair = std::pair<sp_movie, double>;

// Defines the prototype for comparsion between nodes of a
// priority queue, to determine the maximal values
using priority_comparator =
    std::function<bool(
        const movie_rank_pair&, const movie_rank_pair&)>;

// Defines the structure of the priority queue (max heap)
// for the scores prediction (whilst we take the k highest)
using scores_max_heap = std::priority_queue<
    movie_rank_pair,
    std::vector<movie_rank_pair>,
    priority_comparator>;

// Defines the structure of the internal movie database
using movie_db = 
	std::map<sp_movie, movie_features, movie_comperator>;

/**
 * @class RecommendationSystem
 * @brief Gives recommendation for a user based on its ratings
 */
class RecommendationSystem
{
public:
    explicit RecommendationSystem();
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
        const movie_features& features);

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
	sp_movie recommend_by_content(const User& user) const;

    /**
     * A function that calculates the movie with highest
     * predicted score based on ranking of other _movies
     * @param user - The user to recommend to
     * @param movie_count - The size of the movies
     *                      set to recommend from
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(
        const User& user, int movie_count) const;

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
        const User& user, 
        const sp_movie& movie, 
        int movie_count) const;

    /**
     * Pretty-printing all the movies in the system with their
     * respectable year of release
     * @param os - The stream to give the output to
     * @param rs - The object to output
     * @return the same stream as given
     */
    friend std::ostream& operator<<(
        std::ostream& os, const RecommendationSystem& rs);

private:
    /**
     * Normalizing the given ranks with the average of all ranks
     * @param user_ranks - The rankings of a user
     * @return The normalized ranks
     */
    static up_rank_map normalize_ranks(const rank_map& user_ranks);

    /**
     * Calculating the norm of the given vector
     * @param vec - The vector to calculate its norm
     * @return The norm
     */
    static double get_norm(const movie_features& vec);

    /**
     * Calculating the dot product of 2 vectors
     * Vectors must be of the same length - It is NOT validated
     * @param vec1 - Vector 1 ("lhs")
     * @param vec2 - Vector 2 ("rhs")
     * @return The dot product of the vectors
     */
    static double dot_product(
        const movie_features& vec1, const movie_features& vec2);

    /**
     * Calculating the similarity between a preference vector of
     * a user and the features vector of another movie
     * The similarity is measured as the angle between the 2 vectors
     * Vectors should be of the same length - It is NOT validated
     * @param preferences - Preferences of the user
     * @param features - The features of the movie
     * @return The similarity as value between -1 and 1 with greater
     *         being more similar
     */
    static double calculate_movie_similarity(
        const movie_features& preferences,
        const movie_features& features);

    /**
     * Calculating the preferences of a user based on
     * the movies he watched
     * @param normalized_ranks - The rankings of each movie
     *        after normalization.
     * @return The preferences vector
     */
    movie_features calculate_preferences(
        const rank_map& normalized_ranks) const;
    
    movie_db _movies;
    size_t _feature_count;
};

#endif //RECOMMENDATIONSYSTEM_H
