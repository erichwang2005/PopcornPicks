document.addEventListener('DOMContentLoaded', () => {
    const movieList = document.getElementById('movies');
    const genreInput = document.getElementById('genre-input');
    const addGenreButton = document.getElementById('add-genre');
    const finishGenresButton = document.getElementById('finish-genres');
    const genresList = document.getElementById('genres');
    const userList = document.getElementById('user-list');
    const usernameInput = document.getElementById('username');
    const addUserButton = document.getElementById('add-user');
    const hashRecommendationsButton = document.getElementById('get-hash-recommendations');
    const graphRecommendationsButton = document.getElementById('get-graph-recommendations');
    const recommendedMoviesList = document.getElementById('recommended-movies');

    let users = [];
    let currentGenres = [];
    let currentUser = null;

    // Fetch and display all movies
    fetch('http://localhost:8081/movies')
        .then(response => {
            if (!response.ok) {
                throw new Error(`Failed to fetch movies. Status: ${response.status}`);
            }
            return response.json();
        })
        .then(movies => {
            movieList.innerHTML = ''; // Clear existing content
            if (movies.length === 0) {
                const li = document.createElement('li');
                li.textContent = 'No movies available.';
                movieList.appendChild(li);
            } else {
                movies.forEach(movie => {
                    const li = document.createElement('li');
                    li.textContent = `${movie.title} (${movie.genre}) - ${movie.description}`;
                    movieList.appendChild(li);
                });
            }
        })
        .catch(error => {
            console.error('Error fetching movies:', error);
            const li = document.createElement('li');
            li.textContent = 'Error fetching movies.';
            movieList.appendChild(li);
        });

    // Add a new user
    addUserButton.addEventListener('click', () => {
        const username = usernameInput.value.trim();
        if (!username) {
            alert('Please enter a username.');
            return;
        }
        if (users.find(user => user.name === username)) {
            alert('Username already exists.');
            return;
        }
        currentUser = { name: username, genres: [] };
        users.push(currentUser);
        const li = document.createElement('li');
        li.textContent = username;
        userList.appendChild(li);
        usernameInput.value = '';
        genresList.innerHTML = ''; // Clear previous user's genres
        currentGenres = [];
    });

    // Add genre for the current user
    addGenreButton.addEventListener('click', () => {
        const genre = genreInput.value.trim();
        if (genre && currentUser) {
            currentGenres.push(genre);
            const li = document.createElement('li');
            li.textContent = genre;
            genresList.appendChild(li);
            genreInput.value = '';
        } else {
            alert(currentUser ? 'Please enter a genre.' : 'Please add a user first.');
        }
    });

    // Finish entering genres for the current user
    finishGenresButton.addEventListener('click', () => {
        if (currentUser) {
            currentUser.genres = currentGenres;
            alert(`${currentUser.name} has finished entering genres.`);
            currentUser = null;
            currentGenres = [];
            genresList.innerHTML = '';
        } else {
            alert('No user is currently active.');
        }
    });

    // Fetch recommendations for all users
    function getRecommendations(endpoint) {
        if (users.length === 0) {
            alert('Please add at least one user before getting recommendations.');
            return;
        }
        const commonGenres = users.reduce((acc, user) => acc.filter(genre => user.genres.includes(genre)), users[0]?.genres || []);

        if (commonGenres.length === 0) {
            alert('No common genres found among users.');
            return;
        }

        recommendedMoviesList.innerHTML = '<li>Loading recommendations...</li>';

        fetch(endpoint, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
                'Accept': 'application/json',
            },
            body: JSON.stringify({ genres: commonGenres }),
        })
            .then(response => {
                if (!response.ok) {
                    return response.text().then(text => {
                        throw new Error(`HTTP error! Status: ${response.status}, Body: ${text}`);
                    });
                }
                return response.json();
            })
            .then(data => {
                recommendedMoviesList.innerHTML = '';
                const recommendations = data.recommendations || [];
                if (recommendations.length === 0) {
                    const li = document.createElement('li');
                    li.textContent = 'No recommendations found.';
                    recommendedMoviesList.appendChild(li);
                } else {
                    recommendations.forEach(movie => {
                        const li = document.createElement('li');
                        li.textContent = `${movie.title} (${movie.genre}) - ${movie.description}`;
                        recommendedMoviesList.appendChild(li);
                    });
                }
                if (data.time_taken) {
                    const li = document.createElement('li');
                    li.textContent = `Time taken: ${data.time_taken} microseconds`;
                    recommendedMoviesList.appendChild(li);
                }
            })
            .catch(error => {
                console.error('Error fetching recommendations:', error);
                recommendedMoviesList.innerHTML = '';
                const li = document.createElement('li');
                li.textContent = 'Error fetching recommendations.';
                recommendedMoviesList.appendChild(li);
            });
    }

    hashRecommendationsButton.addEventListener('click', () => getRecommendations('http://localhost:8081/recommend/hash'));
    graphRecommendationsButton.addEventListener('click', () => getRecommendations('http://localhost:8081/recommend/graph'));
});