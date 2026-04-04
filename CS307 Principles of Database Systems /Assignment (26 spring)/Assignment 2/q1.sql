SELECT ld1.station_id, s.chinese_name, ld1.line_id as line_id1, ld2.line_id as line_id2
FROM line_detail as ld1
JOIN line_detail as ld2
    on ld1.station_id = ld2.station_id and ld1.line_id < ld2.line_id
JOIN stations as s
    on s.station_id = ld1.station_id
WHERE ld1.line_id in (1, 2, 3) and ld2.line_id in (1, 2, 3)
ORDER by ld1.station_id ASC, line_id1 ASC, line_id2 ASC;