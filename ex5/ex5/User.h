#ifndef USER_H
#define USER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include "Movie.h"

// Forward declaration, to prevent circular includes
class RecommendationSystem;
// Ptrs for Recommendataion System
using up_rec_system = std::unique_ptr<RecommendationSystem>;
using sp_rec_system = std::shared_ptr<RecommendationSystem>;

typedef std::unordered_map<sp_movie, double, hash_func,equal_func> rank_map;
using up_rank_map = std::unique_ptr<rank_map>;

class User
{
public:
	/**
	 * Constructor for the class
	 */
	User(const std::string& username, 
	     rank_map user_ranks, 
		 const sp_rec_system& rec_system);

	// Explicitly defining copy & move to prevent implicit defs
	User(const User&) = delete;
	User& operator=(const User&) = delete;
	User(const User&&) = delete;
	User& operator=(const User&&) = delete;
	~User() = default;

	/**
	 * a getter for the user's name
	 * @return the username
	 */
	const std::string& get_name() const;

	/**
	 * a getter for the ranks map
	 * @return
	 */
	const rank_map& get_ranks() const;

	/**
	 * function for adding a movie to the DB
	 * @param name name of movie
     * @param year year it was made
	 * @param features a vector of the movie's features
	 * @param rate the user rate for this movie
	 */
	void add_movie_to_rs(const std::string& name, int year,
                         const std::vector<double>& features,
                         double rate);

	/**
	 * returns a recommendation according to the movie's content
	 * @return recommendation
	 */
	sp_movie get_recommendation_by_content() const;

	/**
	 * returns a recommendation according to the
	 * similarity recommendation method
	 * @param movie_count - the number of the most
	 *						similar movies to calculate by
	 * @return The recommended movie
	 */
	sp_movie get_recommendation_by_cf(int movie_count) const;

	/**
	 * predicts the score for a given movie
	 * @param name - the name of the movie
	 * @param year - the year the movie was created
	 * @param movie_count - the parameter which represents
	 *					    the number of the most similar movies
	 *					    to predict the score by
	 * @return predicted score for the given movie
	 */
	double get_prediction_score_for_movie(
		const std::string& name, int year, int movie_count) const;

private:
	std::string _username;
	rank_map _user_ranking;
	sp_rec_system _recommendation_system;
};

/**
 * output stream operator
 * @param os the output stream
 * @param user the user
 * @return output stream
 */
std::ostream&& operator<<(const std::ostream& os, const User& user);

#endif //USER_H
