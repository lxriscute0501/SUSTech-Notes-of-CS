WITH station_connection_count as (
    SELECT station_id, COUNT(*) as connection_count
    FROM connections
    GROUP by station_id
),
connection_distribution as (
    SELECT connection_count, COUNT(*) as station_count
    FROM station_connection_count
    GROUP by connection_count
),
ranked_distribution as (
    SELECT connection_count, station_count, ROW_NUMBER() over (ORDER by connection_count DESC) as row_number
    FROM connection_distribution
)
SELECT connection_count, station_count,
    CASE
        WHEN LAG(station_count) over (ORDER by row_number) is NULL then NULL
        ELSE ROUND(
            (station_count - LAG(station_count) over (ORDER by row_number)) * 100.0
            / station_count, 1
        )::text || '%'
    END as percentage_increase, row_number
FROM ranked_distribution
ORDER by row_number;