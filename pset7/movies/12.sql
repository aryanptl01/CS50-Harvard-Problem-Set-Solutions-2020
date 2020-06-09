SELECT title FROM movies WHERE id IN (
SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE people.name = "Johnny Depp")
INTERSECT
SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE people.name = "Helena Bonham Carter"));