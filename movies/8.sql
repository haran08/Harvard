SELECT name FROM people WHERE id IN (
    SELECT stars.person_id FROM stars JOIN movies on stars.movie_id = movies.id WHERE movies.title LIKE '%Toy Story%'
)
