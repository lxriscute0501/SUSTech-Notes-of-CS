WITH station_bus_count as (
    SELECT s.station_id, s.district, s.english_name, COUNT(*) as bus_cnt
    FROM stations as s
    JOIN bus_lines as b
        ON s.station_id = b.station_id
    WHERE s.district in ('Futian', 'Nanshan', 'Luohu')
    GROUP by s.station_id, s.district, s.english_name
),
ranked_stations as (
    SELECT station_id, district, english_name, bus_cnt, RANK() over (
            PARTITION by district
            ORDER by bus_cnt DESC
        ) as rnk
    FROM station_bus_count
)
SELECT district, english_name, bus_cnt
FROM ranked_stations
WHERE rnk <= 3
ORDER by station_id ASC;