WITH station_connection_count as (
    SELECT station_id, COUNT(*) as connection_count
    FROM bus_lines
    GROUP by station_id
),
connection_distribution as (
    SELECT connection_count, COUNT(*) as station_count
    FROM station_connection_count
    GROUP by connection_count
),
ranked_distribution as (
    SELECT connection_count, station_count,
        ROW_NUMBER() OVER (ORDER BY connection_count DESC) as row_number
    FROM connection_distribution
)
SELECT connection_count, station_count, ROUND(
        (station_count - LEAD(station_count) over (ORDER by row_number)) * 100.0
        / LEAD(station_count) over (ORDER by row_number), 1
    ) as percentage_increase, row_number
FROM ranked_distribution
ORDER by row_number;