WITH station_bus_count as (
    SELECT s.station_id, s.district, COUNT(*) as bus_stop_count
    FROM stations as s
    JOIN bus_lines as b
        on s.station_id = b.station_id
    GROUP by s.station_id, s.district
),
district_avg AS (
    SELECT district, AVG(bus_stop_count) as avg_bus_stop_count
    FROM station_bus_count
    GROUP by district
)
SELECT sbc.district, COUNT(*) as count
FROM station_bus_count as sbc
JOIN district_avg as da
    ON sbc.district = da.district
WHERE sbc.bus_stop_count > da.avg_bus_stop_count
GROUP by sbc.district
ORDER by sbc.district;