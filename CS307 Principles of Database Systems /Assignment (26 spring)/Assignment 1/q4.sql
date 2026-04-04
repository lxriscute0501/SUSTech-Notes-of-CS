SELECT station_id
FROM Station
WHERE bus_stop_count > 0
  AND bus_stop_count = (
      SELECT MIN(bus_stop_count)
      FROM Station
      WHERE bus_stop_count > 0
  )
ORDER by station_id ASC;