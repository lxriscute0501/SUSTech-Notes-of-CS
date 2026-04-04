SELECT ld.line_id, ld.station_id, s.chinese_name
FROM line_detail ld
JOIN stations s
  ON ld.station_id = s.station_id
WHERE ld.num = 4
  AND ld.line_id in (
      SELECT line_id
      FROM line_detail
      GROUP BY line_id
      HAVING COUNT(*) = (
          SELECT MAX(cnt)
          FROM (
              SELECT COUNT(*) as cnt
              FROM line_detail
              GROUP by line_id
          ) t
      )
  )
ORDER by s.chinese_name ASC;