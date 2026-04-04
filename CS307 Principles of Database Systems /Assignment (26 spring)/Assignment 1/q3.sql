SELECT count(*) as Count 
FROM (
    SELECT station_id 
    FROM bus_lines 
    GROUP by station_id 
    HAVING count(*) > 20
    ) t;