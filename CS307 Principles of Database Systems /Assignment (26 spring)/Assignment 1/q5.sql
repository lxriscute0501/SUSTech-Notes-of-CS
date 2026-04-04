SELECT s.english_name
FROM stations s
JOIN line_detail l
  ON s.station_id = l.station_id
WHERE l.line_id = 5
  AND s.station_id in (
      SELECT station_id
      FROM line_detail
      GROUP by station_id
      HAVING COUNT(DISTINCT line_id) >= 2
  )
ORDER by s.station_id ASC;