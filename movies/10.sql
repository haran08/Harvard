SELECT DISTINCT name FROM people WHERE id in (
    SELECT person_id FROM directors JOIN movies ON directors.movie_id = movies.id WHERE movies.id IN (
        SELECT movie_id FROM ratings WHERE rating >= 9.0
    )
);
