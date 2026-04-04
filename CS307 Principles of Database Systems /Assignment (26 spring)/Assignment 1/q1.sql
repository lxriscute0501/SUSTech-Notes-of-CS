SELECT english_name, chinese_name
FROM stations
WHERE english_name ILIKE '%road%'
   OR english_name ILIKE '%hill%'
ORDER by station_id ASC;