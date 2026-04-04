WITH line_station_extreme as (
    SELECT line_id, MIN(station_id) as min_id, MAX(station_id) as max_id
    FROM line_detail
    GROUP by line_id
)
SELECT lse.line_id, s1.english_name, s2.english_name
FROM line_station_extreme as lse
JOIN stations as s1
    ON s1.station_id = lse.min_id
JOIN stations as s2
    ON s2.station_id = lse.max_id
ORDER by lse.line_id;