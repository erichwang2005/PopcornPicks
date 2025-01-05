🎥 <h1> **Popcorn Picks** </h1>

Popcorn Picks is a movie recommendation program designed to help you and your friends find movies that match your shared preferences. Whether you’re into drama, horror, comedy, or action, Popcorn Picks ensures you can discover the perfect movie for your next watch party! This project was a collabortive effort between three team members, Eric Wang, Tristan Ratchev, and Abhinav Yarragunta



🚀 <h1> **Features** </h1>

<h2> Graph Implementation: </h2> 

  -Tripartite Movie Storage: Interconnected User, Genre, and Movie nodes.
  
  -Multi-User Scale: Dynamic addition of users and efficient recommendation generation.
  
  -Fast Recommendation: Optimized for computing shared genres and movies.



<h2> Hash Table Implementation</h2> 

  -Efficient Storage: Fast lookups and genre-based movie retrieval.
  
  -Dynamic Resizing: Handles large datasets of 10,000+ movies.
  
  -Personalized Recommendations: Based on shared preferences across all users.
  

💻 <h1> **Front-End Features** </h1>

The front-end interface of Popcorn Picks was built using HTML, CSS, and JavaScript. It provides an interactive way for users to input their preferences and get recommendations.

<h2> Key Buttons and Their Functions </h2> 

  -Add User: Enter a username to create a user profile.
  
  -Add Genre: Add a genre preference for the currently active user.
  
  -Finish Genres: Indicate that a user has completed selecting their genres.
  
  -Get Hash Recommendations: Generate movie recommendations using the Hash Table implementation.
  
  -Get Graph Recommendations: Generate movie recommendations using the Graph implementation.


<h2> User Flow </h2> 

  -Create a User: Enter a username and press Add User.
  
  -Add Genres: Input genres the user enjoys and click Add Genre.
  
  -Finish Selection: When done, press Finish Genres.
  
  -Repeat for All Users: Create more user profiles and repeat the genre input process.
  
  -Get Recommendations: Once all users are added, use the Hash or Graph recommendation button to generate a list of movies based on shared genres.

  
🛠️ <h1> **Libraries Used** </h1>

<h2> cpp-httplib-master: </h2> 

A lightweight HTTP server/client library used to handle communication between the front-end and back-end.

<h2> jsoncpp: </h2> 

Enables easy parsing and generation of JSON data to transfer movie and genre details efficiently between the server and client.

<h2> Node Modules: </h2> 

Used in the front-end development workflow to manage dependencies and streamline bundling


** <h1> Graph vs. Hash Table Time Complexity:** </h1>

Hash Table is faster because of its insertion and search method being of constant computational complexity.

This is also proven experimentally through the use of #include which allows us to track the time of each implementation. For the input using only 1 user and the genres selected being "drama" and "horror", the graph was shown to have a time of 23899531 microseconds, meanwhile the hashtable was shown to have a time of 15845764 microseconds. This shows that the hashtable was indeed faster than the graph.

In order to run the code in this repository, first clone the repository on to your local device, this can be through downloading it as a zip. Next, if you want to use a g++ complier. Run the following commands in order to execute the program:

"g++ -std=c++14 main.cpp movie.cpp -c -lws2_32"

"g++ -std=c++14 main.cpp movie.cpp -o output -lws2_32"

"program"

We hope you enjoy the program and find some new movies to watch!
