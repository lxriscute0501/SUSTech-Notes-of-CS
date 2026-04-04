SELECT l.line_id, s.english_name
FROM line_detail l
JOIN stations s
  ON l.station_id = s.station_id
WHERE l.line_id in (1, 2, 3)
  AND l.num = (
      SELECT MAX(l2.num)
      FROM line_detail l2
      WHERE l2.line_id = l.line_id
  )
ORDER by l.line_id ASC;