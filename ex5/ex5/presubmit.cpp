#include "UsersLoader.h"
#include "RecommendationSystemLoader.h"
#include "RecommendationSystem.h"
#include "Movie.h"
#include <iostream>
#include <string>
#define EXIT_SUCCESS_TEST 0
#define EXIT_FAIL_TEST 2
static std::string BASE_PATH = "C:\\Projects\\huji-cs-c-cpp-workshop\\ex5\\ex5\\test_io\\";
#define THRESHOLD 0.01
typedef int (*Test_Function) ();
int Test_1 ()
{
  std::string movie_name = "test";
  int movie_year = 2022;
  Movie test = Movie (movie_name, movie_year);

  std::cout << "-------------------------" << std::endl;
  std::cout << "Test Movie get_name" << std::endl;
  if (test.get_name () != movie_name)
    {
      std::cerr << "Test Movie get_name failed." << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test Movie get_name succeeded" << std::endl;
    }
  std::cout << "Test Movie get_name done" << std::endl;

  std::cout << "Test Movie get_year" << std::endl;
  if (test.get_year () != movie_year)
    {
      std::cerr << "Test Movie get_year failed." << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test Movie get_year succeeded" << std::endl;
    }
  std::cout << "Test Movie get_year done" << std::endl;
  return EXIT_SUCCESS_TEST;
}

int Test_2 ()
{
  std::string movie_name = "a_test";
  int movie_year = 2022;
  Movie a_new_test = Movie (movie_name, movie_year);
  movie_name = "b_test";
  movie_year = 2022;
  Movie b_test = Movie (movie_name, movie_year);
  movie_name = "test";
  movie_year = 1922;
  Movie a_test = Movie (movie_name, movie_year);

  std::cout << "-------------------------" << std::endl;
  std::cout << "Test Movies with different years" << std::endl;
  if (a_new_test < a_test)
    {
      std::cerr << "Test Movies with different years failed." << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test Movies with different years succeeded" << std::endl;
    }
  std::cout << "Test Movies with different years done" << std::endl;

  std::cout << "Test Movies with same years" << std::endl;
  if (b_test < a_new_test)
    {
      std::cerr << "Test Movies with same years failed." << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test Movies with same years succeeded" << std::endl;
    }
  std::cout << "Test Movies with same years done" << std::endl;

  std::cout << "Test Movies symmetrically" << std::endl;
  if (a_test < a_test)
    {
      std::cerr << "Test Movies symmetrically failed." << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test Movies symmetrically succeeded" << std::endl;
    }
  std::cout << "Test Movies symmetrically done" << std::endl;
  return EXIT_SUCCESS_TEST;
}

int Test_3 ()
{
  std::string movie_name = "test";
  int movie_year = 1999;
  RecommendationSystem rs = RecommendationSystem();
  sp_movie add_pointer = rs.add_movie (movie_name, movie_year, {1, 2, 3});
  sp_movie get_pointer = rs.get_movie (movie_name, movie_year);
  std::cout << "-------------------------" << std::endl;
  std::cout << "Test RecommendationSystemadder and getter pointer" << std::endl;
  if (add_pointer.get () != get_pointer.get ())
    {
      std::cerr << "Test RecommendationSystemadder and getter pointer failed."
                << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test RecommendationSystemadder and getter pointer succeeded"
                << std::endl;
    }
  std::cout << "Test RecommendationSystemadder and getter pointer done"
            << std::endl;
  return EXIT_SUCCESS_TEST;
}

int Test_4 ()
{
  RecommendationSystem rs1, rs2;
  sp_movie a = rs1.add_movie ("TheMagnificentAmbersons", 2001, {8, 6, 9, 1});
  sp_movie b = rs1.add_movie ("BrokebackMountain", 1966, {5, 3, 7, 4});
  sp_movie c = rs2.add_movie ("TheAvengers", 2012, {5, 3, 7, 5});

  std::cout << "-------------------------" << std::endl;
  std::cout << "Test RecommendationSystemadd movie for the movie" << std::endl;
  sp_movie d = rs1.add_movie ("Muriel", 1980, {7, 6, 8, 8});

  if (rs2.get_movie ("TheMagnificentAmbersons", 2001).get () != nullptr ||
      rs1.get_movie ("TheMagnificentAmbersons", 2001).get () != a.get () ||
      rs1.get_movie ("TheAvengers", 2012).get () != nullptr ||
      rs2.get_movie ("TheAvengers", 2012).get () != c.get ())
    {
      std::cerr << "Test RecommendationSystemadd movie for the movie failed."
                << std::endl;
      return EXIT_FAIL_TEST;
    }
  else
    {
      std::cout << "Test RecommendationSystemadd movie for the movie succeeded"
                << std::endl;
    }
  std::cout << "Test RecommendationSystemadd movie for the movie done"
            << std::endl;

  return EXIT_SUCCESS_TEST;
}

int TestPCompilation_1 ()
{

  const Movie m1 ("Test1", 1998);
  Movie m2 ("Test2", 1999);

  m1.get_name ();
  m2.get_name ();

  return 0;
}

int TestPCompilation_2 ()
{

  const Movie m1 ("Test1", 1998);
  Movie m2 ("Test2", 1999);

  m1.get_year ();
  m2.get_year ();

  return 0;
}

int TestPCompilation_3 ()
{

  const Movie m1 ("A", 1998);
  const Movie m2 ("B", 1996);
  std::cout << "m1:" << m1 << std::endl;
  std::cout << "m2:" << m2 << std::endl;

  std::cout << "m1 < m2:" << (m1 < m2) << std::endl;
  std::cout << "m2 < m1:" << (m2 < m1) << std::endl;

  Movie m3 ("C", 1999);
  std::cout << "m3:" << m3 << std::endl;

  std::cout << "m1 < m3:" << (m1 < m3) << std::endl;
  std::cout << "m3 < m1:" << (m3 < m1) << std::endl;

  return 0;
}

int TestPCompilation_4 ()
{
  auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH +"presubmit.in_m");
  std::vector<User> user = UsersLoader::create_users (BASE_PATH +"presubmit.in_u", std::move (rs));
  const User &u0 = user[0];
  const User &u1 = user[1];
  const User &u2 = user[2];
  const User &u3 = user[3];

  if (u0.get_name () != "Sofia" || u1.get_name () != "Michael"
      || u2.get_name () != "Nicole" || u3.get_name () != "Arik")
    return EXIT_FAIL_TEST;

  return EXIT_SUCCESS_TEST;
}

int TestComparison_4 ()
{

  auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m4");
  std::vector<User> user = UsersLoader::create_users (BASE_PATH + "presubmit.in_u4", std::move (rs));

  if (user[0].get_name () != "Amani" || user[1].get_name () != "Lauren"
      || user[2].get_name () != "Karsyn" || user[3].get_name () != "Michelle")
    {
      std::cout << "Loading a movie using UsersLoader didn't load the "
                   "correct names for users" << std::endl;
      return EXIT_FAIL_TEST;
    }

  return EXIT_SUCCESS_TEST;
}
int TestComparison_5 ()
{
  auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m5");
  std::vector<User> user = UsersLoader::create_users (BASE_PATH + "presubmit.in_u5", std::move (rs));

  if (user[0].get_recommendation_by_content ()->get_name () != "TheSilence"
      || user[2].get_recommendation_by_content ()->get_name () !=
         "HighandLow" ||
      user[4].get_recommendation_by_content ()->get_name () != "CityLights" ||
      user[6].get_recommendation_by_content ()->get_year () != 1992 ||
      user[14].get_recommendation_by_content ()->get_name () != "Casablanca")
    {
      std::cout
          << "get_recommendation_by_content from User didn't return the correct output (only negative scores)"
          << std::endl;
      return EXIT_FAIL_TEST;
    }

  return EXIT_SUCCESS_TEST;
}
int TestComparison_6 ()
{
  auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m6");
  std::vector<User> user = UsersLoader::create_users (BASE_PATH + "presubmit.in_u6", std::move (rs));

  if (std::abs (
      user[0].get_prediction_score_for_movie ("TheLastLaugh", 1989, 2) - 4.59)
      > THRESHOLD ||
      std::abs (
          user[5].get_prediction_score_for_movie ("TheLastLaugh", 1989, 2)
          - 6.38) > THRESHOLD ||
      std::abs (
          user[10].get_prediction_score_for_movie ("TheLastLaugh", 1989, 2)
          - 4.98) > THRESHOLD ||
      std::abs (
          user[15].get_prediction_score_for_movie ("TheLastLaugh", 1989, 2)
          - 4.84) > THRESHOLD ||
      //		std::abs(user[20].get_prediction_score_for_movie("TheLastLaugh", 1989, 2) - 7.60) > THRESHOLD ||
      std::abs (
          user[24].get_prediction_score_for_movie ("TheLastLaugh", 1989, 2)
          - 5.5) > THRESHOLD)
    {
      std::cout
          << "get_prediction_score_for_movie from User didn't return the correct output"
          << std::endl;
      return EXIT_FAIL_TEST;
    }

  return EXIT_SUCCESS_TEST;
}
int TestComparison_7 ()
{
  auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m7");
  std::vector<User> user = UsersLoader::create_users (BASE_PATH + "presubmit.in_u7", std::move (rs));

  if (user[0].get_recommendation_by_cf (4)->get_name () != "StrangersonaTrain"
      ||
      user[5].get_recommendation_by_cf (4)->get_name () != "DaysofBeingWild" ||
      user[10].get_recommendation_by_cf (4)->get_name ()
      != "SweetSmellofSuccess" ||
      user[15].get_recommendation_by_cf (4)->get_year () != 2012 ||
      user[19].get_recommendation_by_cf (4)->get_name () != "BlackOrpheus")
    {
      std::cout
          << "get_recommendation_by_cf from User didn't return the correct "
             "output" << std::endl;
      return EXIT_FAIL_TEST;
    }

  return EXIT_SUCCESS_TEST;
}
int TestComparison_14 ()
{

  auto rs = std::make_unique<RecommendationSystem> ();
  rs->add_movie ("TheMagnificentAmbersons", 2001, {8, 6, 9, 1, 1});
  rs->add_movie ("BrokebackMountain", 1966, {5, 3, 7, 4, 7});
  rs->add_movie ("Muriel", 1980, {7, 6, 8, 8, 10});
  rs->add_movie ("TheyWereExpendable", 1967, {1, 2, 8, 5, 9});
  rs->add_movie ("Harakiri", 1967, {2, 6, 6, 10, 5});

  std::vector<User> user = UsersLoader::create_users (BASE_PATH + "presubmit.in_u14", std::move (rs));
  sp_movie first_prediction = user[0].get_recommendation_by_cf (2);
  user[0].add_movie_to_rs ("Movie", 1980, {5, 3, 7, 5, 9}, 10);
  sp_movie second_prediction = user[0].get_recommendation_by_cf (2);

  if (first_prediction->get_name () != "TheMagnificentAmbersons"
      || first_prediction->get_year () != 2001
      || second_prediction->get_name () != "TheyWereExpendable"
      || second_prediction->get_year () != 1967)
    {
      std::cout << "Using User recommendation function didn't return "
                   "expected output"
                << std::endl;
      return EXIT_FAIL_TEST;
    }

  return EXIT_SUCCESS_TEST;
}
int TestComparison_15 ()
{
  try
    {
      auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m15");
//				"presubmit_18.in_m");
      std::cout << *rs << std::endl;
      std::cout << "You didn't throw an exception when loading an invalid "
                   "file for movies" << std::endl;
      return EXIT_FAIL_TEST;
    }
  catch (const std::exception &e)
    {
      return EXIT_SUCCESS_TEST;
    }
}
int TestComparison_16 ()
{
  try
    {
//        auto rs = std::make_unique<RecommendationSystem>();
//        rs->add_movie("TheMagnificentAmbersons",2001,{8,6,9,1,1});
//        rs->add_movie("BrokebackMountain",1966,{5,3,7,4,7});
//        rs->add_movie("Muriel",1980,{7,6,8,8,10});
//        rs->add_movie("TheyWereExpendable",1967,{1,2,8,5,9});
//        rs->add_movie("Harakiri",1967,{2,6,6,10,5});
      auto rs = RecommendationSystemLoader::create_rs_from_movies (BASE_PATH + "presubmit.in_m16");
      auto users = UsersLoader::create_users (BASE_PATH + "presubmit.in_u16", std::move (rs));
//				"presubmit_18.in_m");
      std::cout << "You didn't throw an exception when loading an invalid "
                   "file for users" << std::endl;
      return EXIT_FAIL_TEST;
    }
  catch (const std::exception &e)
    {
      return EXIT_SUCCESS_TEST;
    }
}

int main ()
{
  int status = 0;

  try
    {
      // check all function for RS exist
      RecommendationSystem rec;
      sp_movie a = rec.add_movie ("Batman", 2022, {1, 2, 3, 4});
      sp_movie b = rec.add_movie ("StarWars", 1977, {1, 2, 3, 5});
      sp_movie c = rec.add_movie ("ForestGump", 1994, {1, 2, 3, 4});

      // check all functions for user and UsersLoader exist and movie
      auto rs1 = RecommendationSystemLoader::create_rs_from_movies (
          BASE_PATH + "presubmit.in_m");
      std::vector<User> users = UsersLoader::create_users (
          BASE_PATH + "presubmit.in_u", std::move (rs1));
      sp_movie movie = users[0].get_recommendation_by_content ();
      double s = users[0].get_prediction_score_for_movie ("Titanic", 1997, 2);
      sp_movie m2 = users[0].get_recommendation_by_cf (2);
      if (s < 0)
        {
          std::cerr << "a score should be larger or equal to 0" << std::endl;
          status = 1;
        }

      // check movie functions and some results
      if (movie->get_name () != "Batman" || movie->get_year () != 2022)
        {
          status = 1;
          std::cerr
              << "Recommend by content with username=Sofia failed! Received="
              << *movie << " Expected=Batman (2022)" << std::endl;
        }
      double y = users[2].get_prediction_score_for_movie ("Twilight", 2008, 2);
      if (std::abs (y - 3.5244) > THRESHOLD)
        {
          status = 1;
          std::cerr
              << "Predict movie score for moviename=Twilight , username=Nicole, k=2 failed! Received="
              << y << " Expected=3.5244" << std::endl;
        }
      double z = users[2].get_prediction_score_for_movie ("Titanic", 1997, 2);
      if (std::abs (z - 5.46432) > THRESHOLD)
        {
          status = 1;
          std::cerr
              << "Predict movie score for moviename=Titanic, username=Nicole, k=2 failed! Received="
              << z << " Expected=5.46432" << std::endl;
        }
      if (status != 0)
        {
          std::cerr << "Failed presubmit tests, you have errors" << std::endl;
          return 1;
        }
    }
  catch (const std::exception &e)
    {
      std::cerr
          << "Your program crashed by arised exception - presubmit tests failed"
          << std::endl;
      return 2;
    }

  Test_Function additional_Tests[] = {Test_1, Test_2, Test_3, Test_4,
                                      TestPCompilation_1,
                                      TestPCompilation_2,
                                      TestPCompilation_3,
                                      TestPCompilation_4,
                                      TestComparison_4, TestComparison_6,
                                      TestComparison_5, TestComparison_7,
                                      TestComparison_14, TestComparison_15,
                                      TestComparison_16};

  try
    {
      for (auto test: additional_Tests)
        {

          status = test ();
          if (status != 0)
            {
              std::cerr
                  << "you have failed the presubmit please check your errors"
                  << std::endl;
              return 1;
            }
        }
    }
  catch (const std::exception &e)
    {
      std::cerr
          << "Your program crashed by arised exception - presubmit tests failed"
          << std::endl;
      return 2;
    }
  return 0;
}
