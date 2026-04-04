SELECT COUNT(DISTINCT station_id) as count
FROM Station
WHERE district = 'Longhua District'
  AND line_name <> 'No. 5 line';